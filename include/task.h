#pragma once
#include "../common/common.h"
#include "Game.h"

/*所有的时间以毫秒为单位*/
typedef int myTime;
#define MYTIME_DIFF(a, b) ((myTime)(a) - (myTime)(b))

myTime task_get_time(void); /*得到系统当前运行的时间*/

void task_delay(myTime msecs);

typedef void (*Task_Func)(int arg, Game* game); /*用户的回调函数*/

#define FILE_NUM_MAX    4	/*最多add的文件任务个数*/
#define TIMER_NUM_MAX   4	/*最多add的定时器任务个数*/

/*添加一个文件任务, 当fd可读时, 会自动调用callback函数*/
void task_add_file(int fd, Game *game, Task_Func callback);

/*增加一个定时器任务, 每隔period时间, 会自动调用callback函数*/
void task_add_timer(myTime period, Game *game, Task_Func callback);

void task_delete_file(int fd); /*删除文件任务*/
void task_delete_timer(int period); /*删除定时器任务*/
void task_loop(void); /*进入任务循环, 该函数不返回*/

/*非阻塞方式读/写文件, 返回实际读/写的字节数*/
int myRead_nonblock(int fd, void *p, int n);
int myWrite_nonblock(int fd, void *p, int n);
