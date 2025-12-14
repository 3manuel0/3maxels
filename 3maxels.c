
#include "3maxels.h"

unsigned char * getBuffer(){
    return (unsigned char *)buffer;
}
typedef struct {
    float x, y;
}vec2;

typedef struct {
    float x, y, z, w;
}vec4;

float dot(vec2 a, vec2 b){
    return ((a.x * b.x) + (a.y * b.y));
}

float abs(float n){
    if (n < 0) return n * -1;
    return n;
}

int get_image_size(){
    return IMAGE_SIZE;
}
int get_image_width(){
    return IMAGE_WIDTH;
}
int get_image_height(){
    return IMAGE_HEIGHT;
}

unsigned char * setBackgroundColor(i32 color){

    // reversing the color from 0x6b5b95ff to 0xff955b6b because of Little-endian
    i32 new_color = ((color << 24) & 0xff000000) | ((color << 8) & 0xff0000) | ((color >> 8)  & 0xff00) | ((color >> 24)& 0xff);
    for(u32 i = 0; i < IMAGE_HEIGHT; i++){
        for(u32 j = 0; j < IMAGE_WIDTH; j++){
            buffer[i][j] = new_color;
        }
    }
    // jprintf("%d", buffer);
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


// doing my best to translate the shader from XorDev
// link: https://x.com/XorDev/status/1894123951401378051
// shader code:
// ##########################################################################################
// vec2 p=(FC.xy*2.-r)/r.y,l,v=p*(1.-(l+=abs(.7-dot(p,p))))/.2;                             #
// for(float i;i++<8.;o+=(sin(v.xyyx)+1.)*abs(v.x-v.y)*.2)v+=cos(v.yx*i+vec2(0,i)+t)/i+.7;  #  
// o=tanh(exp(p.y*vec4(1,-1,-2,0))*exp(-4.*l.x)/o);                                         #
// ##########################################################################################

void draw_shader(int frame){
    int width = 16*13;
    int height =9*13;
    vec2 r = {(float)width, (float)height};
    float t = ((float)frame/240)*2*M_PI;
    for (u32 y = 0; y < IMAGE_HEIGHT; y++){
        for (u32 x = 0; x < IMAGE_WIDTH; x++) {
            if( x < width && y < height){
                vec4 o = {0};
                vec2 FC = {(float)x, (float)y};
                // vec2 p = (FC.xy * 2.0 - r) / r.y;
                // = (2x - r.x, 2y - r.y)
                vec2 p = {(FC.x * 2 - r.x) / r.y, (FC.y * 2 - r.y) / r.y};
                float l = 0.0f;
                l += abs(0.7f - dot(p, p));
                // l.x += abs(0.7 - dot(p, p));
                // l.y += abs(0.7 - dot(p, p));
                // vec2 p=(FC*2.-r)/r.y,l,i,v=p*(l+=4.-4.*abs(.7-dot(p,p)));
                // vec2 v = p * (1.0 - (l += abs(0.7 - dot(p, p)))) / 0.2;
                vec2 v = p;
                v.x = p.x * (1.0f - l) / 0.2f;
                v.y = p.y * (1.0f - l) / 0.2f;

                for (float i = 1.0f; i < 8.0f; i++) {
                    // o += (sin(v.xyyx) + 1.0) * abs(v.x - v.y) * 0.2;
                    o.x += (sin(v.x) + 1.0f) * 0.2f * abs(v.x - v.y);
                    o.y += (sin(v.y) + 1.0f) * 0.2f * abs(v.x - v.y);
                    o.z += (sin(v.y) + 1.0f) * 0.2f * abs(v.x - v.y);
                    o.w += (sin(v.x) + 1.0f) * 0.2f * abs(v.x - v.y);
                    // v += cos(v.yx * i + vec2(0.0, i) + t) / i + 0.7;
                    float old_vx = v.x;
                    float old_vy = v.y;
                    v.x += cos(old_vy * i + t) / i + 0.7;
                    v.y +=  cos(old_vx * i + i + t) / i + 0.7;
                }

                // o = tanh(
                //         exp(p.y * vec4(1.0, -1.0, -2.0, 0.0)) *
                //         exp(-4.0 * l.x) / o
                //     );
                o.x = tanh(exp(p.y - 4*l) / o.x);
                o.y = tanh(exp(-p.y - 4*l) / o.y);
                o.z = tanh(exp(-2*p.y - 4*l) / o.z);
                o.w = tanh(exp(-4*l) / o.w);
                // jprintf("%d %d", y, x);
                // buffer[x][y] = *(int*)(&o);
                *(char *)&buffer[y][x] = o.x * 255;
                *((char *)&buffer[y][x] + 1) = o.y * 255;
                *((char *)&buffer[y][x] + 2) = o.z * 255;
                *((char *)&buffer[y][x] + 3) = (char)255;
            }
            // jprintf("%d", *(int*)&o);
        }
    }
}


