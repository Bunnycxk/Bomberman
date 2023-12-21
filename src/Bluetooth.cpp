
#include<cstring>
#include "../include/Bluetooth.h"
#include "task.h"
Bluetooth::Bluetooth(/* args */)
{
}

Bluetooth::~Bluetooth()
{
}


void Bluetooth::listen(int n, const std::string &room_name){
    std::string cmd = "bluetoothctl system-alias " + room_name + "_room";
    if (system(cmd.c_str()) == -1) return;
    for (int i = 1; i<= n; i++){
        if (system("bluetoothctl discoverable on && bluetoothctl pairabel on") == -1) return;
        system("rfcomm -r watch  0 1 &");
    }
}

void Bluetooth::scan(){
    room.clear();
    system("bluetoothctl --timeout 5 scan on &");
    if (system("bluetoothctl devices | grep _room > ./temp/bluetooth_devices.out") == -1) return ;
    FILE *devices_file = fopen("./temp/bluetooth_devices.out", "r");
    if (!devices_file) return;
    char mac[20],name[50];
    while(!feof(devices_file)){
        fscanf(devices_file,"Device %s %s\n",mac, name);
        room.push_back(Room_info{std::string(mac),std::string(name)});
        printf("%s %s\n",mac, name);
    }
}

void Bluetooth::connect(int id){
    if (id >= room.size()) {
        printf("connect error\n");
        return;
    }
    std::string cmd = "sudo rfcomm -r connect 0 " + room[id].mac + " 1 &";
    printf("cmd = %s \n",cmd.c_str());
    if (system(cmd.c_str()) == -1) {
        printf("connect error\n");
        return;
    }
}

void Bluetooth::send(int fd, const char *content){
    myWrite_nonblock(fd, (void *)content, strlen(content));
}