#pragma once
#include "input.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../common/common.h"

#define GAMEPAD_LEFT_BUTTON_TOUCH 0
#define GAMEPAD_RIGHT_BUTTON_TOUCH 1
#define GAMEPAD_LEFT_BUTTON_RELEASE 2
#define GAMEPAD_RIGHT_BUTTON_RELEASE 3
#define GAMEPAD_UP_BUTTON_TOUCH 4
#define GAMEPAD_DOWN_BUTTON_TOUCH 5
#define GAMEPAD_UP_BUTTON_RELEASE 6
#define GAMEPAD_DOWN_BUTTON_RELEASE 7
#define GAMEPAD_X_TOUCH 8
#define GAMEPAD_X_RELEASE 9
#define GAMEPAD_Y_TOUCH 10
#define GAMEPAD_Y_RELEASE 11
#define GAMEPAD_A_TOUCH 12
#define GAMEPAD_A_RELEASE 13
#define GAMEPAD_B_TOUCH 14
#define GAMEPAD_B_RELEASE 15
#define GAMEPAD_ERROR 254
#define GAMEPAD_NO_EVENT 255

int gamepad_read(int gamepad_fd);

int gamepad_init(const char *dev);