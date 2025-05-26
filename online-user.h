#include <iostream>
#include <string> 
#include <assert.h>

#include "red-black-tree.h"

int index (char c) {
    if(c == ':') return 0;
    if(c >= 'a' and c <= 'z') return c - 'a' + 1;
    return c - '0' + 27; // '0' to '9' are 27 to 37
}

namespace user {
    // we create a prefix tree that stores online users
    // for each user and device, we creata a string that is "user:device" and inserts it in prefix tree
    struct node {
        node* children[37]; // 26 letters + 10 digits + 1 for ':'
        bool isEndOfUser;
        int sessionID = -1; 

        node() : isEndOfUser(false) {
            for(int i = 0; i < 37; ++i) {
                children[i] = nullptr;
            }
        }
    } * root;   

    void insert(const std::string &user_device) {
        node *current = root;
        for (int i = 0; i < user_device.size(); ++i) {
            int idx = index(user_device[i]);
            if (current->children[idx] == nullptr) {
                current->children[idx] = new node();
            }
            if(i + 1 < user_device.size() and user_device[i + 1] == ':') {
                current->children[idx]->isEndOfUser = true; // Mark the end of user
            }
            current = current->children[idx];
        }
    }

    bool online(const std::string &user_device) {
        node *current = root;
        for (int i = 0; i < user_device.size(); ++i) {
            int idx = index(user_device[i]);
            if (current->children[idx] == nullptr) {
                return false; // user or device not found
            }
            current = current->children[idx];
        }
        return current->sessionID != -1; // Check if it's an end of user and has a session ID
    }

    void remove_user_device(const std::string &user_device) {
        node *current = root;
        for (int i = 0; i < user_device.size(); ++i) {
            int idx = index(user_device[i]);
            if (current->children[idx] == nullptr) {
                return;
            }
            current = current->children[idx];
        }
        current->sessionID = -1;
    }

    void dfs(node * temp) {
        if(temp->sessionID != -1) {
            server::remove(temp->sessionID); // Remove the session from the server
            temp->sessionID = -1; // Reset sessionID
        }
        for (int i = 0; i < 37; ++i) {
            if(temp->children[i] != nullptr) {
                dfs(temp->children[i]);
            }
        }
    }

    void remove_user(const std::string &user) {
        node * current = root;
        for (int i = 0; i < user.size(); ++i) {
            if(user[i] == ':') {
                dfs(current);
                return;
            }
            int idx = index(user[i]);
            if(current->children[idx] == nullptr) {
                return;
            }
            current = current->children[idx];
        }
    }

    int get_sessionID(const std::string &user_device) {
        static int sessionID = 0; 
        node * current = root;
        for (int i = 0; i < user_device.size(); ++i) {
            int idx = index(user_device[i]);
            if(current->children[idx] == nullptr) {
                return -1; // user or device not found
            }
            current = current->children[idx];
        }
        // assert(current->isEndofDevice); // Ensure that we are at the end of a device
        if(current->sessionID == -1) {
            current->sessionID = ++sessionID;
        }
        return current->sessionID;
    }
};