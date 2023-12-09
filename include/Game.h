#pragma once
#include <set>
#include <memory>
#include "../common/common.h"
#include "Gamepad.h"
#include "Object.h"
#include "Config.h"
#include "Map.h"


/*
    Game 是游戏实体
    需要维护一个Object堆，支持：
        1. 获取当前场景的所有 Object，并按 priority 排序，用于绘图
        2. 当 Object 的 time_to_live 为 0 时删除该 Object
        3. 增加一个 Object
        4. 对指定 Object 做状态更新:
            当有手柄输入时对 role 进行修改，通过 player 来做修改。
            状态栏可能发现变化
        5. 
*/
class Game
{
private:
    /* data */
    int gamepad_fd;                     // 游戏手柄的文件句柄

public:
    Config config;
    std::set< std::shared_ptr<Object> >object;             // 用 set 维护object
    std::shared_ptr<Object> player;       // 玩家
    std::shared_ptr<Object> backgroud;    // 背景
    std::shared_ptr<Object> bomb;        
    std::shared_ptr<Map> mp;

public:
    Game(/* args */);
    ~Game();

    void init();
    void run();
    void create_map();
};

void gamepad_event_cb(int fd, Game *game);

void draw(int fd, Game *game);