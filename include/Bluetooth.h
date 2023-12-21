#pragma once
#include <vector>
#include <string>
#include "../common/common.h"

struct Room_info{
    std::string mac;
    std::string name;
};

class Bluetooth
{
private:
    /* data */
public:
    std::vector<Room_info> room;
    Bluetooth(/* args */);

    ~Bluetooth();
    void listen(int n, const std::string &room_name);
    void connect(int id);
    void scan();
    void send(int fd, const char *content);
};


