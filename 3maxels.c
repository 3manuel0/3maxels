
#include "3maxels.h"

unsigned char * getBuffer(){
    return (unsigned char *)buffer;
}

unsigned char * setBackgroundColor(i32 color){

    // reversing the color from 0x6b5b95ff to 0xff955b6b because of Little-endian
    i32 new_color = ((color << 24) & 0xff000000) | ((color << 8) & 0xff0000) | ((color >> 8)  & 0xff00) | ((color >> 24)& 0xff);
    for(u32 i = 0; i < IMAGE_HEIGHT; i++){
        for(u32 j = 0; j < IMAGE_WIDTH; j++){
            buffer[i][j] = new_color;
        }
    }
    jprintf("%d", buffer);
    return (u8 *)buffer;
}

void drawRectangle(i32 x, i32 y, i32 width, i32 height, i32 color){
    
    if(width > IMAGE_WIDTH || height > IMAGE_HEIGHT) return;

    // reversing the color from 0x6b5b95ff to 0xff955b6b because of Little-endian
    i32 new_color = ((color << 24) & 0xff000000) | ((color << 8) & 0xff0000) | ((color >> 8)  & 0xff00) | ((color >> 24)& 0xff);
    for(u32 i = y; i < y + height; i++){
        for(u32 j = x; j < x + width; j++){
            if(i < IMAGE_HEIGHT && j < IMAGE_WIDTH){
                buffer[i][j] = new_color;
            }
        }
    }
};


// this is just for testing (yes this function is AI generated, I had no idea how to create a randome function)
static unsigned int seed = 12345;
unsigned char randomNumber(){
    seed = seed * 1103515245 + 12345;
    return seed % 255; 
}
// --------------------------------------------------------------------------

void change_buffer(){
    for(i32 i = 0; i < IMAGE_SIZE; i++){
        *((u8 *)buffer + i) = randomNumber();
    }
}

