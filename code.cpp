#include <iostream>
#include <string>
#include <sstream>
#include "online-user.h"
// #include "red-black-tree.h"

void login(std::string Time, std::string user, std::string device) {
    std::string user_device = user + ":" + device;
    user::insert(user_device);
    int sessionID = user::get_sessionID(user_device);
    server::node * temp;
    if(server::find(temp, sessionID)) {
        std::cout << "User " << user << " already logged in!" << std::endl;
        temp->lastActiveTime = temp->loginTime = Time; 
        return;
    } 
    temp = new server::node(sessionID, Time, user, device);
    // std::cout << "User not found, creating new session..." << std::endl;
    server::insert(temp);
    std::cout << "User " << user << " logged into " << device << " device successfully! (session : " << sessionID << ")" << std::endl;
}

void logout_in_a_device(std::string user, std::string device) {
    std::string user_device = user + ":" + device;
    int sessionID = user::get_sessionID(user_device);
    // std::cout << "session : " << sessionID << std::endl; 
    user::remove_user_device(user_device);
    server::node * temp;
    if(server::find(temp, sessionID)) {
        server::remove(sessionID);
        std::cout << "User " << user << " logged out of " << device << " device successfully!" << std::endl;
    } else {
        std::cout << "User not found!" << std::endl;
    }
}

void logout_all_devices(std::string user) {
    user::remove_user(user);
    std::cout << "User " << user << " logged out from all devices successfully!" << std::endl;
}

void action_users(std::string Time, std::string user, std::string device, std::string action) {
    if(not user::online(user + ":" + device)) {
        std::cout << "User not found" << std::endl;
        return;
    }
    int sessionID = user::get_sessionID(user + ":" + device);
    server::node * temp;
    server::find(temp, sessionID); 
    if(temp) {
        temp->lastActiveTime = Time;
        std::cout << "User " << user << "performed action: " << action << " on device: " << device << std::endl;
    } else {
        std::cout << "User not found!" << std::endl;
    }
}

void init() {
    server::root = new server::node(0, "", "", ""); // Dummy root node
    user::root = new user::node(); // Initialize the prefix tree root
}

signed main() {
    freopen("text.txt", "r", stdin);
    init();
    std::string line;
    std::string Time, user, device, action;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue; // bỏ dòng trống

        std::istringstream ss(line);
        std::string Time, user, device, action;
        ss >> Time >> user >> device;

        std::getline(ss >> std::ws, action); // lấy phần còn lại của dòng
        if (action == "logout") {
            logout_in_a_device(user, device);
        } else if (action == "login") {
            login(Time, user, device);
        } else {
            action_users(Time, user, device, action);
        }
    }
}