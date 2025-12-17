
#include "3maxels.h"

unsigned char * getBuffer(){
    return (unsigned char *)buffer;
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
// slow huge computation even when compiting with -O3
void draw_shader(int frame){
    int width = 12*35;
    int height =9*35;
    vec2 r = {(float)width, (float)height};
    float t = ((float)frame/240)*M_PI2;
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


float dot(vec2 a, vec2 b){
    return ((a.x * b.x) + (a.y * b.y));
}

float abs(float x){
    if (x < 0) return -x;
    return x;
}

float fact(int x) {
    float f = 1;
    for (int i = 2; i <= x; i++) f *= i;
    return f;
}

float pow(float n, float x){
    float res = 1.0f;
    for(int i = 0; i < x; i++)res *= n;
    return res;
}

float sin(float x) {
    float sum = 0;
    float term;
    int n;

    // range check
    while (x > M_PI) x -= M_PI2;
    while (x < -M_PI) x += M_PI2;

    for (n = 0; n < 10; n++) {
        term = (n % 2 == 0 ? 1 : -1) * 
               (pow(x, 2*n + 1) / fact(2*n + 1));
        sum += term;
    }

    return sum;
}

float cos(float x) {
    // range chek 
    // while (x > M_PI) x -= M_PI2;
    // while (x < -M_PI) x += M_PI2;

    // float term = 1;
    // float sum = term;

    // for (int n = 1; n < 10; n++) { 
    //     term = -term * x * x / ((2*n-1) * (2*n));
    //     sum += term;
    // }

    // return sum;
    return sin(x + M_PI2);
}

// pre calculate values for speed
static const float inv[] = {1.0f,
    1.0f/1.0f, 1.0f/2.0f, 1.0f/3.0f, 1.0f/4.0f, 1.0f/5.0f,
    1.0f/6.0f, 1.0f/7.0f, 1.0f/8.0f, 1.0f/9.0f, 1.0f/10.0f,
    1.0f/11.0f, 1.0f/12.0f, 1.0f/13.0f, 1.0f/14.0f, 1.0f/15.0f,
    1.0f/16.0f, 1.0f/17.0f, 1.0f/18.0f, 1.0f/19.0f, 1.0f/20.0f,
    1.0f/21.0f, 1.0f/22.0f, 1.0f/23.0f, 1.0f/24.0f, 1.0f/25.0f,
    1.0f/26.0f, 1.0f/27.0f, 1.0f/28.0f, 1.0f/29.0f, 1.0f/30.0f
};

float exp(float x){
    float sum = 1.0f;
    float term = 1.0f;
    for (int n = 1; n < 30; n++) {
        term *= x * inv[n];
        sum += term;
        if (term < 1e-7 && term > -1e-7) break;
    }
    return sum;
}



// tanh(x)=ex+e−xex−e−x​
float tanh(float x){
    // return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
    return 1.5 * x / (1 + abs(1.5 * x));
}

