from PIL import Image
OUTPUT_WIDTH = 128
OUTPUT_HEIGHT = 128
CROP_AND_SCALE = True

images = ["/Users/jonathanlow/Desktop/image/grass_block.jpg"]
#add images to this list
#images = ["/Users/my_directory/test.png", "/Users/blah/grass_block.jpg"]
str_arr = "int img[" + str(len(images)) + "][" + str(OUTPUT_WIDTH) + "][" + str(OUTPUT_HEIGHT) + "] ="
str_arr += "{"

for im_loc in range(len(images)):
    im = Image.open(images[im_loc])
    if CROP_AND_SCALE:
        if(im.size[0] > im.size[1]):
            im = im.crop((0,0, im.size[1], im.size[1]))
        else:
            im = im.crop((0,0, im.size[0], im.size[0]))
        im.thumbnail((OUTPUT_WIDTH,OUTPUT_HEIGHT), Image.ANTIALIAS)
    str_arr += "{"
    for x in range(OUTPUT_WIDTH):
        str_arr += "{"
        for y in range(OUTPUT_HEIGHT):
            cur_pixel = im.getpixel((x,y))
            rgb565 = 0
            rgb565 = (((cur_pixel[0] & 0b11111000)<<8) + ((cur_pixel[1] & 0b11111100)<<3)+(cur_pixel[2]>>3))
            str_arr += str(rgb565)
            if y == OUTPUT_HEIGHT-1:
                pass
            else:
                str_arr += ","
        if x == OUTPUT_WIDTH-1:
            str_arr += "}"
        else:
            str_arr += "},"
    if im_loc == len(images)-1:
        str_arr += "}"
    else:
        str_arr += "},"
    im.close()
str_arr += "}"
str_arr += ";"

#print(str_arr)

#write to .hpp file

text_file = open("/Users/jonathanlow/Desktop/image/images.hpp", "w")


#text_file = open("/Users/my_directory.../.../images.hpp", "w")
n = text_file.write(str_arr)
text_file.close()

