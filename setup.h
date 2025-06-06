#include <iostream>
#include <chrono>
#include <map>

using namespace std;

long long convertTime(int year, int month, int day, int hour, int minute, int second) {
    // chuyền thời gian từ năm, tháng, ngày, giờ, phút, giây sang dạng loại long long
    tm t = {};
    t.tm_year = year - 1900; // tm_year is years since 1900
    t.tm_mon = month - 1; // tm_mon is 0-11
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    return chrono::system_clock::to_time_t(chrono::system_clock::from_time_t(mktime(&t)));
}

int userID(string user) {
    static int id = 0;
    static map<string, int> userMap;
    if(userMap.find(user) == userMap.end()) {
        userMap[user] = ++id;
    }
    return userMap[user];

}

int deviceID(string device) {
    static int id = 0;
    static map<string, int> deviceMap;
    if(deviceMap.find(device) == deviceMap.end()) {
        deviceMap[device] = ++id;
    }
    return deviceMap[device];
}

int sessionID = 0;

int get_new_sessionID() {
    return ++sessionID;
}