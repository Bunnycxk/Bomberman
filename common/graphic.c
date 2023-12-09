#include "common.h"
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

static int LCD_FB_FD;
static int *LCD_FB_BUF = NULL;
static int DRAW_BUF[SCREEN_WIDTH*SCREEN_HEIGHT];

static struct area {
	int x1, x2, y1, y2;
} update_area = {0,0,0,0};

#define AREA_SET_EMPTY(pa) do {\
	(pa)->x1 = SCREEN_WIDTH;\
	(pa)->x2 = 0;\
	(pa)->y1 = SCREEN_HEIGHT;\
	(pa)->y2 = 0;\
} while(0)

#define abs(a) ((a) > 0 ? (a) : (- (a)))
#define min(a, b) ((a) < (b) ? (a) : (b)) 
#define swap(a, b) {\
	int t = a;\
	a = b;\
	b = t;\
}

void fb_init(const char *dev)
{
	int fd;
	struct fb_fix_screeninfo fb_fix;
	struct fb_var_screeninfo fb_var;

	if(LCD_FB_BUF != NULL) return; /*already done*/

	//进入终端图形模式
	fd = open("/dev/tty0",O_RDWR,0);
	ioctl(fd, KDSETMODE, KD_GRAPHICS);
	close(fd);

	//First: Open the device
	if((fd = open(dev, O_RDWR)) < 0){
		printf("Unable to open framebuffer %s, errno = %d\n", dev, errno);
		return;
	}
	if(ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix) < 0){
		printf("Unable to FBIOGET_FSCREENINFO %s\n", dev);
		return;
	}
	if(ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0){
		printf("Unable to FBIOGET_VSCREENINFO %s\n", dev);
		return;
	}

	printf("framebuffer info: bits_per_pixel=%u,size=(%d,%d),virtual_pos_size=(%d,%d)(%d,%d),line_length=%u,smem_len=%u\n",
		fb_var.bits_per_pixel, fb_var.xres, fb_var.yres, fb_var.xoffset, fb_var.yoffset,
		fb_var.xres_virtual, fb_var.yres_virtual, fb_fix.line_length, fb_fix.smem_len);

	//Second: mmap
	void *addr = mmap(NULL, fb_fix.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(addr == (void *)-1){
		printf("failed to mmap memory for framebuffer.\n");
		return;
	}

	if((fb_var.xoffset != 0) ||(fb_var.yoffset != 0))
	{
		fb_var.xoffset = 0;
		fb_var.yoffset = 0;
		if(ioctl(fd, FBIOPAN_DISPLAY, &fb_var) < 0) {
			printf("FBIOPAN_DISPLAY framebuffer failed\n");
		}
	}

	LCD_FB_FD = fd;
	LCD_FB_BUF = addr;

	//set empty
	AREA_SET_EMPTY(&update_area);
	return;
}

static void _copy_area(int *dst, int *src, struct area *pa)
{
	int x, y, w, h;
	x = pa->x1; w = pa->x2-x;
	y = pa->y1; h = pa->y2-y;
	src += y*SCREEN_WIDTH + x;
	dst += y*SCREEN_WIDTH + x;
	while(h-- > 0){
		memcpy(dst, src, w*4);
		src += SCREEN_WIDTH;
		dst += SCREEN_WIDTH;
	}
}

static int _check_area(struct area *pa)
{
	if(pa->x2 == 0) return 0; //is empty

	if(pa->x1 < 0) pa->x1 = 0;
	if(pa->x2 > SCREEN_WIDTH) pa->x2 = SCREEN_WIDTH;
	if(pa->y1 < 0) pa->y1 = 0;
	if(pa->y2 > SCREEN_HEIGHT) pa->y2 = SCREEN_HEIGHT;

	if((pa->x2 > pa->x1) && (pa->y2 > pa->y1))
		return 1; //no empty

	//set empty
	AREA_SET_EMPTY(pa);
	return 0;
}

void fb_update(void)
{
	if(_check_area(&update_area) == 0) return; //is empty
	_copy_area(LCD_FB_BUF, DRAW_BUF, &update_area);
	AREA_SET_EMPTY(&update_area); //set empty
	return;
}

/*======================================================================*/

static void * _begin_draw(int x, int y, int w, int h)
{
	int x2 = x+w;
	int y2 = y+h;
	if(update_area.x1 > x) update_area.x1 = x;
	if(update_area.y1 > y) update_area.y1 = y;
	if(update_area.x2 < x2) update_area.x2 = x2;
	if(update_area.y2 < y2) update_area.y2 = y2;
	return DRAW_BUF;
}

void fb_draw_pixel(int x, int y, int color)
{
	if(x<0 || y<0 || x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT) return;
	int *buf = _begin_draw(x,y,1,1);
/*---------------------------------------------------*/
	*(buf + y*SCREEN_WIDTH + x) = color;
/*---------------------------------------------------*/
	return;
}

void fb_draw_rect(int x, int y, int w, int h, int color)
{
	if(x < 0) { w += x; x = 0;}
	if(x+w > SCREEN_WIDTH) { w = SCREEN_WIDTH-x;}
	if(y < 0) { h += y; y = 0;}
	if(y+h >SCREEN_HEIGHT) { h = SCREEN_HEIGHT-y;}
	if(w<=0 || h<=0) return;
	int *buf = _begin_draw(x,y,w,h);
/*---------------------------------------------------*/
	int i_end = x + w, j_end = y + h, offset = y * SCREEN_WIDTH;
	for (int j = y; j < j_end; j++){
		for (int i = x; i < i_end; i++)
		*(buf + offset + i) = color;
		offset += SCREEN_WIDTH;
	}

	//printf("you need implement fb_draw_rect()\n"); exit(0);

/*---------------------------------------------------*/
	return;
}

/*
	画一个15*15大小的固定圆
*/
const int dx[]={3,5,6,6,7,7,8,8,8,7,7,6,6,5,3};

void fb_draw_15_circle(int x, int y, int color){
	if(x - 7 < 0 || y - 7 < 0 || x + 7 >= SCREEN_WIDTH || y + 7>= SCREEN_HEIGHT) return;
	int *buf = _begin_draw(x-7,y-7,15,15), offset = (y - 7) * SCREEN_WIDTH;
	for (int nowy = y - 7, line = 0; nowy <= y + 7; nowy++){
		int len = dx[line++];
		for (int nowx = x - len; nowx <= x + len; nowx++)
		*(buf + offset + nowx) = color;
		offset += SCREEN_WIDTH;
	}
	return;
}

void fb_draw_track(int x1, int y1, int x2, int y2, int color){
	int dx = abs(x1 - x2), dy = abs(y1 - y2), flag = 0;
	if (dx < dy){
		swap(x1, y1); swap(x2, y2); swap(dx, dy);
		flag = 1;
	}
	if (x1 > x2){
		swap(x1, x2) swap(y1, y2);
	}

	int err = dx / 2, y = y1;
	for (int x = x1; x < x2; x++){
		if (flag) fb_draw_15_circle(y, x, color);else fb_draw_15_circle(x, y, color);
		err -= dy;
		if (err < 0) {
			y += y1 < y2 ? 1 : -1;
			err += dx;
		}
	}
}
void fb_draw_line(int x1, int y1, int x2, int y2, int color)
{
/*---------------------------------------------------*/
	// use Bresenham line algorithm
 	// 不知道自己有没有正确理解算法，以下是我的理解：
	// 假设直线是 x1 < x2 && y1 < y2，且 dx > dy，即斜率在 0 ~ 1 之间
	// 则 y = (dy / dx) * (x - x1) + y1
	// 每次将点 x++ 而 y 要么不变,要么 y++
	// 对于 y 不变，存在误差 err_1 = | ( (dy / dx) * (x - x1) + y1)   -   y | 
	// 对于 y++，存在误差 err_2 = |(dy / dx) * (x - x1) + y1   -   (y + 1) |
	// 选择 err 小的情况来更新 y
	// 可以发现两种情况的 err 都不会超过 1，因此均分的情况是0.5
	// 因此上面的操作等价于
	// 当 x++ 时 err 会增加 dy/dx，当 err 超过 0.5 时 y++。
	// 考虑取浮点运算
	// 由于 dx 不变，将 err 都乘上 dx
	// 于是每次 err * dx 会增加 dy, 当 err * dx 超过 0.5 * dx 时 y++
	int dx = abs(x1 - x2), dy = abs(y1 - y2), flag = 0;
	if (dx < dy){
		swap(x1, y1); swap(x2, y2); swap(dx, dy);
		flag = 1;
	}
	if (x1 > x2){
		swap(x1, x2) swap(y1, y2);
	}

	int err = dx >> 1, y = y1;
	for (int x = x1; x < x2; x++){
		if (flag) fb_draw_pixel(y, x, color);else fb_draw_pixel(x, y, color);
		err -= dy;
		if (err < 0) {
			y += y1 < y2 ? 1 : -1;
			err += dx;
		}
	}
	//printf("you need implement fb_draw_line()\n"); exit(0);
	
/*---------------------------------------------------*/
	return;
}


void fb_mix_pixel(char* col_1, char *col_2){
	int alpha = col_2[3];
	switch (alpha)
	{
		case 0:
			break;
		case 255:
			col_1[0] = col_2[0];
			col_1[1] = col_2[1];
			col_1[2] = col_2[2];
		default:
			col_1[0] += ((col_2[0] - col_1[0]) * alpha) >> 8;
			col_1[1] += ((col_2[1] - col_1[1]) * alpha) >> 8;
			col_1[2] += ((col_2[2] - col_1[2]) * alpha) >> 8;
			break;
	}
}

void fb_draw_image(int x, int y, fb_image *image, int color)
{
	if(image == NULL) return;

	int ix = 0; //image x
	int iy = 0; //image y
	int w = image->pixel_w; //draw width
	int h = image->pixel_h; //draw height

	if(x<0) {w+=x; ix-=x; x=0;}
	if(y<0) {h+=y; iy-=y; y=0;}
	
	if(x+w > SCREEN_WIDTH) {
		w = SCREEN_WIDTH - x;
	}
	if(y+h > SCREEN_HEIGHT) {
		h = SCREEN_HEIGHT - y;
	}
	if((w <= 0)||(h <= 0)||(x>=SCREEN_WIDTH)||(y>=SCREEN_HEIGHT)) return;

	int *buf = _begin_draw(x,y,w,h);
/*---------------------------------------------------------------*/
	char *dst = (char *)(buf + y*SCREEN_WIDTH + x);
	char *src = image->content + iy * image->line_byte; //不同的图像颜色格式定位不同
/*---------------------------------------------------------------*/

	int alpha;
	int ww;
	if(image->color_type == FB_COLOR_RGB_8880) /*lab3: jpg*/
	{
		src += ix << 2;
		//printf("line_byte = %d w = %d h = %d\n",image->line_byte, w, h);
		int copy_byte = min(image->line_byte, w << 2), buf_offset = 0, img_offset = 0;
		for (int i = 0; i < h; i++){
			memcpy(dst + buf_offset, src + img_offset, copy_byte);
			buf_offset += SCREEN_WIDTH << 2;
			img_offset += image->line_byte;
		}
		//printf("you need implement fb_draw_image() FB_COLOR_RGB_8880\n"); exit(0);
		return;
	}
	else if(image->color_type == FB_COLOR_RGBA_8888) /*lab3: png*/
	{
		src += ix << 2;
		//printf("line_byte = %d w = %d h = %d\n",image->line_byte, w, h);
		for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++){
			char *buf_p = dst + i * (SCREEN_WIDTH << 2) + (j << 2);
			char *image_p = (src + i * image->line_byte + (j << 2));
			fb_mix_pixel(buf_p, image_p);
		}
//		printf("you need implement fb_draw_image() FB_COLOR_RGBA_8888\n"); exit(0);
		return;
	}
	else if(image->color_type == FB_COLOR_ALPHA_8) /*lab3: font*/
	{
		src += ix;
		//printf("%d %d line_byte = %d w = %d h = %d\n",x,y, image->line_byte, w, h);
		for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++){
			char *buf_p = dst + (i * (SCREEN_WIDTH << 2) + (j << 2));
			int B = color & 0xff, G = (color >> 8) & 0xff, R = (color >> 16) & 0xff;
			int alpha = *(src + i * image->line_byte + j);
			switch (alpha)
			{
				case 255:
					buf_p[0] = B;
					buf_p[1] = G;
					buf_p[2] = R;

				default:
					buf_p[0] += ((B - buf_p[0]) * alpha) >> 8;
					buf_p[1] += ((G - buf_p[1]) * alpha) >> 8;
					buf_p[2] += ((R - buf_p[2]) * alpha) >> 8;
					break;
			}
		}
		//printf("you need implement fb_draw_image() FB_COLOR_ALPHA_8\n"); exit(0);
		return;
	}
/*---------------------------------------------------------------*/
	return;
}

void fb_draw_border(int x, int y, int w, int h, int color)
{
	if(w<=0 || h<=0) return;
	fb_draw_rect(x, y, w, 1, color);
	if(h > 1) {
		fb_draw_rect(x, y+h-1, w, 1, color);
		fb_draw_rect(x, y+1, 1, h-2, color);
		if(w > 1) fb_draw_rect(x+w-1, y+1, 1, h-2, color);
	}
}

/** draw a text string **/
void fb_draw_text(int x, int y, char *text, int font_size, int color)
{
	fb_image *img;
	fb_font_info info;
	int i=0;
	int len = strlen(text);
	while(i < len)
	{
		img = fb_read_font_image(text+i, font_size, &info);
		if(img == NULL) break;
		fb_draw_image(x+info.left, y-info.top, img, color);
		fb_free_image(img);

		x += info.advance_x;
		i += info.bytes;
	}
	return;
}

