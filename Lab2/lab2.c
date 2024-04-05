/*
 *
 * CSEE 4840 Lab 2 for 2019
 *
 * Name/UNI: Yuqi Lin (yl5334), Yelin Mao (ym3000), Hongkuan Yu (hy2819)
 */
#include "fbputchar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "usbkeyboard.h"
#include <pthread.h>

/* Update SERVER_HOST to be the IP address of
 * the chat server you are connecting to
 */
/* arthur.cs.columbia.edu */
#define SERVER_HOST "128.59.19.114"
#define SERVER_PORT 42000

#define BUFFER_SIZE 256

/*
 * References:
 *
 * https://web.archive.org/web/20130307100215/http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 *
 * http://www.thegeekstuff.com/2011/12/c-socket-programming/
 * 
 */

int sockfd; /* Socket file descriptor */

int row=1; 

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

pthread_t network_thread;
void *network_thread_f(void *);
void show(char buffer[], int n);

int main()
{
  int err;

  struct sockaddr_in serv_addr;

  struct usb_keyboard_packet packet;
  int transferred;

  if ((err = fbopen()) != 0) {
    fprintf(stderr, "Error: Could not open framebuffer: %d\n", err);
    exit(1);
  }
  
  clean(0, 48); //Clean whole screen

  /* Open the keyboard */
  if ( (keyboard = openkeyboard(&endpoint_address)) == NULL ) {
    fprintf(stderr, "Did not find a keyboard\n");
    exit(1);
  }
    
  /* Create a TCP communications socket */
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "Error: Could not create socket\n");
    exit(1);
  }

  /* Get the server address */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVER_PORT);
  if ( inet_pton(AF_INET, SERVER_HOST, &serv_addr.sin_addr) <= 0) {
    fprintf(stderr, "Error: Could not convert host IP \"%s\"\n", SERVER_HOST);
    exit(1);
  }

  /* Connect the socket to the server */
  if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "Error: connect() failed.  Is the server running?\n");
    exit(1);
  }

  /* Start the network thread */
  pthread_create(&network_thread, NULL, network_thread_f, NULL);

  char buffer[BUFFER_SIZE];
  int i=0, CharNo=0;
  memset(buffer, 0, BUFFER_SIZE);

  /* Look for and handle keypresses */
  clean(46, 48); //Clean the input area
  fbputchar('_', 46, 0); //Initial cursor position

  static int last_keycode = 0;
  for (;;) {
    libusb_interrupt_transfer(keyboard, endpoint_address,
                              (unsigned char *)&packet, sizeof(packet),
                              &transferred, 0);
    
    if (transferred == sizeof(packet)) {
        //Solve key conflict
        if (packet.keycode[0] == 0) {
            last_keycode = 0;
            continue;
        }
        if (packet.keycode[0] == last_keycode) {
            continue;
        }

        last_keycode = packet.keycode[0];
        //Esc
        if (packet.keycode[0] == 0x29) {
            break;
        }
        switch (packet.keycode[0]) {
            case 0x2a: //Backspace
                if (i > 0) {
                    memmove(&buffer[i-1], &buffer[i], CharNo - i);
                    buffer[--CharNo] = '\0';
                    i--;
                }
                break;
            case 0x50: //Left Arrow
                if (i > 0) i--;
                break;
            case 0x4f: //Right Arrow
                if (i < CharNo) i++;
                break;
            case 0x28: //Enter
                if (CharNo > 0) { //Send if there are some inputs
                    write(sockfd, buffer, strlen(buffer));
                    clean(46, 48);
                    i = CharNo = 0;
                    memset(buffer, 0, BUFFER_SIZE);
                }
                break;
            default:
                if (CharNo < BUFFER_SIZE - 2) {
                    int ascii = keyValue(packet.modifiers, packet.keycode[0]);
                    if (ascii > 0) {
                        if (i <= CharNo) {
                            memmove(&buffer[i + 1], &buffer[i], CharNo - i);
                            buffer[i] = (char)ascii;
                            i++;
                            CharNo++;
                        }
                        if (CharNo >= BUFFER_SIZE - 2) {
                            CharNo = BUFFER_SIZE - 2;
                        }
                    }
                }
        }

        
        clean(46, 48);
        fbputs(buffer, 46, 0);
        int cursorRow = i >= 128 ? 47 : 46;
        int cursorCol = i % 128;
        fbputchar('_', cursorRow, cursorCol);
    }
  }

  /* Terminate the network thread */
  pthread_cancel(network_thread);

  /* Wait for the network thread to finish */
  pthread_join(network_thread, NULL);

  return 0;
}

void *network_thread_f(void *ignored)
{
  char recvBuf[BUFFER_SIZE];
  int n;
  /* Receive data */
  while ( (n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0 ) {
    recvBuf[n] = '\0';
    show(recvBuf, n);
  }
  return NULL;
}

//Show characters on screen
void show(char recvBuf[], int n) {
    int neededRows = n / 128 + (n % 128 != 0);

    //Detect if the received-messages screen is full
    if (row + neededRows > 44) {
        clean(0, 44); 
        row = 1; 
    }

    fbputs(recvBuf, row, 0);
    row += neededRows;
}
