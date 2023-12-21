#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>

/* FPS  */
#define FPS 30



/*======================== Map ============================*/

#define MENU_ICON_LEFT 96
#define MENU_ICON_TOP 172
#define MENU_ICON_RIGHT 352
#define MENU_ICON_BOTTTOM 428

#define MENU_ICON_FLUSH_LEFT 352
#define MENU_ICON_FLUSH_RIGHT 416
#define MENU_ICON_FLUSH_TOP  364
#define MENU_ICON_FLUSH_BOTTOM 428

#define MENU_MOVE_LEFT 160
#define MENU_MOVE_TOP 492
#define MENU_STOP_LEFT 288
#define MENU_STOP_TOP 492

#define MENU_CREATE_LEFT 800
#define MENU_CREATE_TOP 460
#define MENU_CREATE_RIGHT 928
#define MENU_CREATE_BOTTOM 524

#define MENU_START_LEFT 800
#define MENU_START_TOP 460
#define MENU_START_RIGHT 928
#define MENU_START_BOTTOM 524

#define MENU_ROOM_LIST_TOP 108

#define MENU_ROOM_LEFT 480
#define MENU_ROOM_TOP 172
#define MENU_ROOM_HEIGHT 64
#define MENU_ROOM_WIDTH 448

#define MENU_ROOM_FLUSH_LEFT 800
#define MENU_ROOM_FLUSH_RIGHT 928
#define MENU_ROOM_FLUSH_TOP 76
#define MENU_ROOM_FLUSH_BOTTOM 140

#define MAP_CELL_SIZE 64
#define MAP_CELL_HALF (MAP_CELL_SIZE >> 1)
#define MAP_ROW 8
#define MAP_COLUMN 12

#define MAP_LEFT 	224
#define MAP_TOP 	44
#define MAP_RIGHT   992
#define MAP_BOTTOM 	556

#define get_map_x(x) (((x) - MAP_LEFT) / MAP_CELL_SIZE)
#define get_map_y(y) (((y) - MAP_TOP) / MAP_CELL_SIZE)

#define get_pixel_x(x) ((x) * MAP_CELL_SIZE + MAP_LEFT)
#define get_pixel_y(y) ((y) * MAP_CELL_SIZE + MAP_TOP)

enum map_type {MAP_EMPTY, MAP_DESTRUCTIBLE, MAP_INDESTRUCTIBLE, MAP_PROPS_SPEED, MAP_PROBS_HEALTH, MAP_PROBS_BOMB, MAP_PROBS_PUSH, MAP_PROBS_ATTACK, MAP_BOMB, MAP_BRUST, MAP_EXPLOSION};
#define PROBS_NUM 5

#define NAME_TOP 52
#define AVATAR_TOP 92
#define STATUS_TOP 220
#define STATUS_LEFT 48

/*======================== task.c ============================*/

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

/*======================== image.c ============================*/
#define FB_COLOR(r,g,b)	(0xff000000|(r<<16)|(g<<8)|b)

#define FB_COLOR_RGB_8880	1
#define FB_COLOR_RGBA_8888	2
#define FB_COLOR_ALPHA_8	3

#define COLOR_WHITE FB_COLOR(255,255,255)
#define COLOR_BLACK FB_COLOR(0,0,0)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int color_type; /* FB_COLOR_XXXX */
	int pixel_w, pixel_h;
	int line_byte;
	char *content; /*4 byte align*/
} fb_image;

fb_image * fb_new_image(int color_type, int w, int h, int line_byte);
void fb_free_image(fb_image *image);

fb_image * fb_read_jpeg_image(const char *file);
fb_image * fb_read_png_image(const char *file);

/*得到一个图片的子图片,子图片和原图片共享颜色内存*/
fb_image *fb_get_sub_image(fb_image *img, int x, int y, int w, int h);

typedef struct {
	int bytes;	//UTF-8编码所占字节数
	int advance_x; //x方向步进距离
	int left;	//左跨距
	int top;	//上跨距
} fb_font_info;

void font_init(const char *font_file);
fb_image * fb_read_font_image(const char *text, int pixel_size, fb_font_info *format);

#ifdef __cplusplus
}
#endif

/*=========================== graphic.c ===============================*/
#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	600


#ifdef __cplusplus
extern "C" {
#endif

void fb_init(const char *dev);
void fb_update(void);

/*lab2*/
void fb_draw_pixel(int x, int y, int color);
void fb_draw_rect(int x, int y, int w, int h, int color, int *img);
void fb_draw_border(int x, int y, int w, int h, int color);
void fb_draw_line(int sx, int sy, int dx, int dy, int color);

/*lab3*/
void fb_draw_image(int x, int y, fb_image *image, int color);
void fb_draw_text(int x, int y, const char *text, int font_size, int color);

void fb_mix_pixel(char* col_1, char *col_2);

void fb_draw_15_circle(int x, int y, int color, int* img);
void fb_draw_track(int x1, int y1, int x2, int y2, int color, int* img);

#ifdef __cplusplus
}
#endif

/*=========================== input.c ===============================*/
/*lab4*/
#define TOUCH_NO_EVENT	0
#define TOUCH_PRESS	1
#define TOUCH_MOVE	2
#define TOUCH_RELEASE	3
#define TOUCH_ERROR	9
#define FINGER_NUM_MAX	5

#ifdef __cplusplus
extern "C" {
#endif

int touch_init(const char *dev); /*返回touch_fd*/
int touch_read(int touch_fd, int *x, int *y, int *finger);

/*=========================== board.c ===============================*/
/*和硬件平台相关的函数*/
void * board_video_get_addr(void);

/*声音采样格式是 s16/mono, 采样率是3.072MHz */
void board_audio_record(uint16_t *BufAddr, int nsamples);
void board_audio_play(uint16_t *BufAddr, int nsamples);

#ifdef __cplusplus
}
#endif

#endif /* _COMMON_H_ */