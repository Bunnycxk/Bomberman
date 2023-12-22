#pragma once
#include <set>

#include "../common/common.h"
#include "Config.h"
#include "Background.h"
#include "Role.h"
#include "Text.h"
#include "Gamepad.h"
#include "Map.h"
#include "Bluetooth.h"

enum screen_type {GAME_START, GAME_MENU, GAMING, GAME_OVER, GAME_ROOM};

enum game_start_text {ONE_PLAYER, CREATE_ROOM, JOIN_ROOM, TEXT_COUNT};

enum direction_type {DIRECTION_UP = -1, DIRECTION_DOWN = 1};
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
    int touch_fd;                       // 触摸屏的文件句柄
    std::vector<std::shared_ptr<Text>> all_room;    // 房间
    int room_cur;                            // 当前选择房间
    int room_num;                   
    screen_type screen;                      // 游戏场景
    std::string send_buf;
    std::vector<std::shared_ptr<Picture>> icon[4];    //玩家头像
public:
    int bluetooth_fd[4];                // 蓝牙句柄
    bool join_player[4];
    Bluetooth bluetooth;                             
    int frame_lock_flag;
    int player_num, max_player_num;                         // 玩家个数
    uint my_id;         
    bool is_server;                          // 是否是服务器
    Config config;
    std::set< std::shared_ptr<Object> >object;             // 用 set 维护object
    std::shared_ptr<Role> player[4];            // 玩家
    std::shared_ptr<Background> background;    // 背景
    // std::shared_ptr<Objectbomb;       
    std::shared_ptr<Map> mp;                 // 地图
    uint time_to_over;                       // 剩余游戏时间
    bool start_flag;
public:
    
    Game(/* args */);
    ~Game();

    void init(screen_type screen);
    void run();
    screen_type get_screen();
    int get_room_cur();
    int get_room_num();
    void set_room_cur(int data);
    // void switch_to_text(direction_type type);
    void switch_screen(screen_type scr);
    
    void generate_probs();

    void room_flush();

    void add_send_info(const std::string &content);
    void send_buf_clear();
    void send();
    void sever_update();
    void update(const char *cmd, uint len, bool add_flag);
    //void create_map();
};

void gamepad_event_cb(int fd, Game *game);

void touch_event_cb(int fd, Game *game);

void draw(int fd, Game *game);

void game_generate_probs(int fd, Game *game);

void listen_rfcomm_file(int fd, Game *game);

void bluetooth_event_cb(int fd, Game *game);