#include "../include/Gamepad.h"

int gamepad_init(const char *dev)
{
	int fd = open(dev, O_RDONLY);
	if(fd < 0){
		printf("gamepad event open %s error!errno = %d\n", dev, errno);
		return -1;
	}
	return fd;
}

static int last_event;


int gamepad_read(int gamepad_fd)
{
	struct input_event data;
	int n;
	if((n = read(gamepad_fd, &data, sizeof(data))) != sizeof(data)){
		printf("gamepad_read error %d, errno=%d\n", n, errno);
		return GAMEPAD_ERROR;
	}
	printf("event read: type-code-value = %d-%d-%d\n", data.type, data.code, data.value);
	switch (data.type)
    {
    case EV_ABS:
        switch (data.code)
        {
        case ABS_X:
            switch (data.value)
            {
            case 0:
                last_event = GAMEPAD_LEFT_BUTTON_TOUCH;
                return GAMEPAD_LEFT_BUTTON_TOUCH;
            case 255:
                last_event = GAMEPAD_RIGHT_BUTTON_TOUCH;
                return GAMEPAD_RIGHT_BUTTON_TOUCH;
            case 127:
                if (last_event == GAMEPAD_LEFT_BUTTON_TOUCH){
                    last_event = GAMEPAD_LEFT_BUTTON_RELEASE;
                    return GAMEPAD_LEFT_BUTTON_RELEASE;
                }else
                if (last_event == GAMEPAD_RIGHT_BUTTON_TOUCH){
                    last_event = GAMEPAD_RIGHT_BUTTON_RELEASE;
                    return GAMEPAD_RIGHT_BUTTON_RELEASE;
                }
            default:
                break;
            }
            break;
        
        case ABS_Y:
            switch (data.value)
            {
            case 0:
                last_event = GAMEPAD_UP_BUTTON_TOUCH;
                return GAMEPAD_UP_BUTTON_TOUCH;
            case 255:
                last_event = GAMEPAD_DOWN_BUTTON_TOUCH;
                return GAMEPAD_DOWN_BUTTON_TOUCH;
            case 127:
                if (last_event == GAMEPAD_UP_BUTTON_TOUCH){
                    last_event = GAMEPAD_UP_BUTTON_RELEASE;
                    return GAMEPAD_UP_BUTTON_RELEASE;
                }else
                if (last_event == GAMEPAD_DOWN_BUTTON_TOUCH){
                    last_event = GAMEPAD_DOWN_BUTTON_RELEASE;
                    return GAMEPAD_DOWN_BUTTON_RELEASE;
                }
            default:
                break;
            }
        default:
            break;
        }
        break;
    case EV_KEY:
        switch (data.code)
        {
        case BTN_TRIGGER:
            if (data.value) 
                return GAMEPAD_X_TOUCH;
            else
                return GAMEPAD_X_RELEASE;
            break;
        case BTN_TOP:
            if (data.value) 
                return GAMEPAD_Y_TOUCH;
            else
                return GAMEPAD_Y_RELEASE;
            break;
        case BTN_THUMB:
            if (data.value) 
                return GAMEPAD_A_TOUCH;
            else
                return GAMEPAD_A_RELEASE;
            break;
        case BTN_THUMB2:
            if (data.value) 
                return GAMEPAD_B_TOUCH;
            else
                return GAMEPAD_B_RELEASE;
            break;
        case BTN_BASE3:
            if (data.value) 
                return GAMEPAD_SELECT_TOUCH;
            else
                return GAMEPAD_SELECT_RELEASE;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
	return GAMEPAD_NO_EVENT;
}
