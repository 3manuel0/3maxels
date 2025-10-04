
#include "3maxels.h"

unsigned char * setBackgroundColor(int color){
    char t = 0;
    char r = (color >> 24) & 0xFF;
    char g = (color >> 16) & 0xFF;
    char b = (color >> 8) & 0xFF; 
    char a = color & 0xFF;

    for(u32 i = 0; i < IMAGE_SIZE; i++){
        if(t == 3){
            buffer[i] = a;
        }
        if(t == 0){
            buffer[i] = r;
        }
        if(t == 1){
            buffer[i] = g;
        }
        if(t == 2){
            buffer[i] = b;
        }
        t++;
        if(t == 4){
            t = 0;
        }
    }
    return buffer;
}

void drawRectangle(int x, int y, int width, int height, int color){
    char r = (color >> 24) & 0xFF;
    char g = (color >> 16) & 0xFF;
    char b = (color >> 8) & 0xFF; 
    char a = color & 0xFF;
    width *= 4;
    height *= 4;
    // int offset = (x * 4) + (y * 4 * IMAGE_WIDTH) ;
    for(int j = 1; j < height + 1; j++){
        int offset = (x * 4 * width) + (y * 4) ;
        printf("offset = %d image_size = %d", offset, IMAGE_SIZE);
        for(int i = offset; i < offset + width - 4 ; i+=4){
            buffer[i] = r;
            buffer[i + 1] = g;
            buffer[i + 2] = b;
            buffer[i + 3] = a;
        }
    }
};

static unsigned int seed = 12345;

unsigned char randomNumber(){
    seed = seed * 1103515245 + 12345; // Simple LCG 
    return seed % 255; 
}

void change_buffer(){
    for(int i = 0; i < IMAGE_SIZE; i++){
        buffer[i] = randomNumber();
    }
}

