# 目标文件名
TARGET := Bomberman

# 编译器
CC := gcc
CXX := g++

CFLAGS := -Wall -O2 
LDFLAGS := -Wall

# C 文件
C_SRCS := $(wildcard ./common/*.c)

$(info C_SRCS : $(C_SRCS))

# /common/graphic.c ./common/image.c ./common/task.c ./common/touch.c

CXX_SRCS := $(wildcard ./src/*.cpp) main.cpp

# CXX_SRCS := ./src/Game.cpp ./src/Backgroud.cpp ./src/Config.cpp ./src/Item.cpp main.cpp

$(info CXX_SRCS :$(CXX_SRCS))

OBJS := $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)

INCLUDE := -I./common/external/include -I./common -I./include

LIBS := -L./common/external/lib -ljpeg -lfreetype -lpng -lasound -lz -lc -lm -ljsoncpp

DEBUG_FLAG := $(CFLAGS)

%.o : %.cpp

	$(CXX) $(DEBUG_FLAG) $< -c -o $@ $(INCLUDE) $(LIBS);

%.o : %.c

	$(CC) $(DEBUG_FLAG) $< -c -o  $@ $(INCLUDE) $(LIBS);


$(TARGET) : $(OBJS)

	$(CXX) $(DEBUG_FLAG) $^ -o $@ $(INCLUDE) $(LIBS);

	@echo "> build success <"


clean:
	rm -f $(OBJS) $(TARGET)