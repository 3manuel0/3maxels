
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
    int t;
    char r = (color >> 24) & 0xFF;
    char g = (color >> 16) & 0xFF;
    char b = (color >> 8) & 0xFF; 
    char a = color & 0xFF;
    for(u32 i = 0; i < IMAGE_SIZE; i++){
        if( i > 1200 * height * 4 + 1){
                return;
        }
        for(int j = 0; j < height; j++){
            if( i < ((width + (1200 * j) ) * 4) && i >= 1200 * j * 4){
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

        }
    }
};
unsigned char randomNumber();

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

