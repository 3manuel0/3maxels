
#ifndef H_3MAXELS
#define H_3MAXELS
#define IMAGE_HEIGHT 700
#define IMAGE_WIDTH 1200
#define IMAGE_SIZE 1200 * 700 * 4 // RGBA
static unsigned int buffer[IMAGE_HEIGHT][IMAGE_WIDTH] = {0};

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef char i8;
typedef short i16;
typedef int i32;
int jprintf(char *a, ...);

#endif