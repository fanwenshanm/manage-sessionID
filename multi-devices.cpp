#include <iostream>
#include <unordered_map>
#include <sstream>

#include "setup.h"
#include "last-active.h"
#include "sessionID.h"

using namespace std;

void logintoServer(long long loginTime, int user, int device) {
    session::node * temp = nullptr; 
    session::find(temp, user);
    if(temp == nullptr) {
        temp = new session::node(get_new_sessionID(), loginTime, user, device);
        last_active::node * lastActiveNode = new last_active::node(loginTime, user);
        last_active::insert(lastActiveNode);
        session::insert(temp);
    } else {
        last_active::remove(temp->lastActiveTime);
        temp->lastActiveTime = loginTime;
        last_active::node * lastActiveNode = new last_active::node(loginTime, user);
        last_active::insert(lastActiveNode);
        temp->device = device;
        temp->loginTime = loginTime;
        temp->sessionID = get_new_sessionID();
    }
    cout << "User logged in successfully! (session ID is : " << temp->sessionID << ")" << endl;
}

void logoutFromServer(int user) {
    session::node * temp = nullptr; 
    if(temp == nullptr) {
        cout << "User not found!" << endl;
        return;
    }
    last_active::remove(temp->lastActiveTime);
    session::remove(user);
    cout << "User logged out successfully!" << endl;
}

void activateUser(int user, long long time) {
    session::node * temp = nullptr; 
    session::find(temp, user);
    if(temp == nullptr) {
        cout << "User not found!" << endl;
        return;
    }
    if(temp->lastActiveTime + 600 < time) { // 600 = 10 minutes
        last_active::remove(temp->lastActiveTime);
        session::remove(user);
        cout << "User is inactive for too long, please log in again!" << endl;
        return;
    }
    last_active::remove(temp->lastActiveTime);
    temp->lastActiveTime = time;
    last_active::node * lastActiveNode = new last_active::node(time, user);
    last_active::insert(lastActiveNode);
    cout << "User activated successfully!" << endl;
}

void parseLine(const string &line) {
    istringstream iss(line);
    string action, timeStr, user, device = "unknown";
    iss >> timeStr >> user >> device;
    getline(iss, action);
    if(not action.empty() and action[0] == ' ') {
        action = action.substr(1); 
    }
    
    int year, month, day, hour, minute, second;
    sscanf(timeStr.c_str(), "%d-%d-%dT%d:%d:%dZ", &year, &month, &day, &hour, &minute, &second);
    long long time = convertTime(year, month, day, hour, minute, second);
    int user_id = userID(user + ":" + device);
    int device_id = deviceID(device);
    if(action == "login") {
        logintoServer(time, user_id, device_id);
    } else if(action == "logout") {
        logoutFromServer(user_id);
    } else {
        activateUser(user_id, time);
    }
}

signed main() {
    freopen("user_sessions_multidevices_log.txt", "r", stdin);
    // freopen("text.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    string line; 
    session::root = new session::node();
    last_active::root = new last_active::node();
    while(getline(cin, line)) {
        parseLine(line);
    }
}

