import random

def map_generator(mif_file, width=8):
    # Generate 1200 random 0s or 1s
    random_bits = [random.randint(0, 1) for _ in range(1200)]

    # Prepare the content to write to the file
    content = "\n".join(f"{i}: {bit};" for i, bit in enumerate(random_bits))

    # Open the MIF file for writing
    with open(mif_file, 'w') as file:
        # Write the MIF header
        file.write(f'WIDTH = {width};\n')
        file.write(f'DEPTH = {1200};\n')
        file.write('ADDRESS_RADIX = DEC;\n')
        file.write('DATA_RADIX = HEX;\n')
        file.write('CONTENT BEGIN\n')
        
        file.write(content)
        file.write('\n')

        # End the MIF content
        file.write('END;\n')

map_generator('output.mif')


