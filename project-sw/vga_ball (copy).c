/* * Device driver for the VGA video generator
 *
 * A Platform device implemented using the misc subsystem
 *
 * Stephen A. Edwards
 * Columbia University
 *
 * References:
 * Linux source: Documentation/driver-model/platform.txt
 *               drivers/misc/arm-charlcd.c
 * http://www.linuxforu.com/tag/linux-device-drivers/
 * http://free-electrons.com/docs/
 *
 * "make" to build
 * insmod vga_ball.ko
 *
 * Check code style with
 * checkpatch.pl --file --no-tree vga_ball.c
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "vga_ball(copy).h"

#define DRIVER_NAME "vga_ball"

/*
 * Information about our device
 */
struct vga_ball_dev {
	struct resource res; /* Resource: our registers */
	void __iomem *virtbase; /* Where registers can be accessed in memory */
        vga_ball_arg_t game_state;
} dev;

/*
 * Write segments of a single digit
 * Assumes digit is in range and the device information has been set up
 */

static void write_game_state(vga_ball_game_state_t *state) 
{
	int i;
	iowrite32(state->player1.x, dev.virtbase);
        iowrite32(state->player1.y, dev.virtbase + 4);
        iowrite32(state->player1.alive, dev.virtbase + 8);
        iowrite32(state->player2.x, dev.virtbase + 12);
        iowrite32(state->player2.y, dev.virtbase + 16);
	iowrite32(state->player2.alive, dev.virtbase + 20);
         
        for (i = 0; i < MAX_BOMBS; i++){
            const bomb_t *bomb = &state->bombs[i];
            iowrite32(bomb->active, dev.virtbase + 24);
            iowrite32(bomb->x, dev.virtbase + 28);
            iowrite32(bomb->y, dev.virtbase + 32);
            iowrite32(bomb->timer, dev.virtbase + 36);
            iowrite32(bomb->exploded, dev.virtbase + 40); 
            iowrite32(bomb->fire_center, dev.virtbase + 44);
            iowrite32(bomb->fire_up, dev.virtbase + 48);
            iowrite32(bomb->fire_down, dev.virtbase + 52);
            iowrite32(bomb->fire_left, dev.virtbase + 56);
            iowrite32(bomb->fire_right, dev.virtbase + 60);

        dev.game_state = *state;

}



/*
 * Handle ioctl() calls from userspace:
 * Read or write the segments on single digits.
 * Note extensive error checking of arguments
 */
static long vga_ball_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	vga_ball_arg_t vla;
        //vga_ball_position_t position;

	switch (cmd) {
	case VGA_BALL_WRITE_STATE:
		if (copy_from_user(&vla, (vga_ball_arg_t *) arg,
				   sizeof(vga_ball_arg_t)))
			return -EACCES;
		write_game_state(&vla.game_state);
		break;

	case VGA_BALL_READ_STATE:
	  	vla.game_state = dev.game_state;
		if (copy_to_user((vga_ball_arg_t *) arg, &vla,
				 sizeof(vga_ball_arg_t)))
			return -EACCES;
		break;
        
	default:
		return -EINVAL;
	}

	return 0;
}

/* The operations our device knows how to do */
static const struct file_operations vga_ball_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl = vga_ball_ioctl,
};

/* Information about our device for the "misc" framework -- like a char dev */
static struct miscdevice vga_ball_misc_device = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= DRIVER_NAME,
	.fops		= &vga_ball_fops,
};

/*
 * Initialization code: get resources (registers) and display
 * a welcome message
 */
static int __init vga_ball_probe(struct platform_device *pdev)
{
	int ret;

	/* Register ourselves as a misc device: creates /dev/vga_ball */
	ret = misc_register(&vga_ball_misc_device);

	/* Get the address of our registers from the device tree */
	ret = of_address_to_resource(pdev->dev.of_node, 0, &dev.res);
	if (ret) {
		ret = -ENOENT;
		goto out_deregister;
	}

	/* Make sure we can use these registers */
	if (request_mem_region(dev.res.start, resource_size(&dev.res),
			       DRIVER_NAME) == NULL) {
		ret = -EBUSY;
		goto out_deregister;
	}

	/* Arrange access to our registers */
	dev.virtbase = of_iomap(pdev->dev.of_node, 0);
	if (dev.virtbase == NULL) {
		ret = -ENOMEM;
		goto out_release_mem_region;
	}
        
        /* Initialize game state */
        dev.game_state.player1.x = 0;  // top-left corner
        dev.game_state.player1.y = 0;
        dev.game_state.player1.alive = 1;  // Player is alive

        dev.game_state.player2.x = 639;  // bottom-right corner
        dev.game_state.player2.y = 479; 
        dev.game_state.player2.alive = 1;  // Player is alive

        for (int i = 0; i < 2; i++) {
                dev.game_state.bombs[i].active = 0; // No bombs are active initially
                dev.game_state.bombs[i].exploded = 0;
        }

	return 0;

out_release_mem_region:
	release_mem_region(dev.res.start, resource_size(&dev.res));
out_deregister:
	misc_deregister(&vga_ball_misc_device);
	return ret;
}

/* Clean-up code: release resources */
static int vga_ball_remove(struct platform_device *pdev)
{
	iounmap(dev.virtbase);
	release_mem_region(dev.res.start, resource_size(&dev.res));
	misc_deregister(&vga_ball_misc_device);
	return 0;
}

/* Which "compatible" string(s) to search for in the Device Tree */
#ifdef CONFIG_OF
static const struct of_device_id vga_ball_of_match[] = {
	{ .compatible = "csee4840,vga_ball-1.0" }, /**/
	{},
};
MODULE_DEVICE_TABLE(of, vga_ball_of_match);
#endif

/* Information for registering ourselves as a "platform" driver */
static struct platform_driver vga_ball_driver = {
	.driver	= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(vga_ball_of_match),
	},
	.remove	= __exit_p(vga_ball_remove),
};

/* Called when the module is loaded: set things up */
static int __init vga_ball_init(void)
{
	pr_info(DRIVER_NAME ": init\n");
	return platform_driver_probe(&vga_ball_driver, vga_ball_probe);
}

/* Calball when the module is unloaded: release resources */
static void __exit vga_ball_exit(void)
{
	platform_driver_unregister(&vga_ball_driver);
	pr_info(DRIVER_NAME ": exit\n");
}

module_init(vga_ball_init);
module_exit(vga_ball_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stephen A. Edwards, Columbia University");
MODULE_DESCRIPTION("VGA ball driver");
