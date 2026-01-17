
#ifndef H_3MAXELS
#define H_3MAXELS
#define IMAGE_HEIGHT 9*30
#define IMAGE_WIDTH 12*30
#define IMAGE_SIZE (IMAGE_HEIGHT * IMAGE_WIDTH * 4) // RGBA
#define M_PI 3.14159265358979323846f
#define M_PI2 6.28318530717958647692f
static unsigned int buffer[IMAGE_HEIGHT][IMAGE_WIDTH] = {0};

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef char i8;
typedef short i16;
typedef int i32;

typedef struct {
  float x, y;
} vec2;

typedef struct {
  float x, y, z, w;
} vec4;

int jprintf(char *a, ...);
float dot(vec2 a, vec2 b);
float abs(float x);
float sin(float x);
float cos(float x);
float tanh(float x);
float exp(float x);
float pow(float n, float x);

#endif