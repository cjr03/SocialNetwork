//
//  network.cpp
//  
//
//  Created by Cole Roberts on 9/21/23.
//

#pragma once
#include "../include/network.h"
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "user.cpp"

Network::Network(){
    users_ = std::vector<User*> ();
};

Network::Network(std::vector<User*> copy){
    users_ = copy;
}

Network::Network(const char* file){
    if(std::size_t(readUsers(file)) == numUsers()) return;
}

Network::~Network(){};

void Network::addUser(User* user){
    assert(user != nullptr);
    users_.push_back(user);
    user->setUserId(users_.size());
}

int Network::addConnection(std::string s1, std::string s2){
    size_t id1 = getId(s1);
    size_t id2 = getId(s2);
    for (auto v : getUser(id1)->getFriends()) {
        if (v == id2) return 0;
    }
    getUser(id1)->addFriend(id2);
    getUser(id2)->addFriend(id1);
    return -1;
}

int Network::removeConnection(std::string s1, std::string s2){
    if(s1 == s2) {
        std::cout << "No connection between the same user allowed.";
        return -1;
    }
    size_t id1 = getId(s1);
    size_t id2 = getId(s2);
    if(id1 == size_t(-1)) std::cout << "User 1 does not exist.\n";
    if(id2 == size_t(-1)) std::cout << "User 2 does not exist.\n";
    if(id1 != size_t(-1) and id2 != size_t(-1)) {
        getUser(id1)->deleteFriend(id2);
        getUser(id2)->deleteFriend(id1);
        return 0;
    }
    return -1;
}

std::size_t Network::getId(std::string name){
    for(size_t allUsers = 0; allUsers < users_.size(); ++allUsers){
        if(users_[allUsers]->getName() == name) return users_[allUsers]->getUserId();
    }
    std::cout << "No Id found.\n";
    return -1;
}

int Network::readUsers(const char* fname) {
    std::ifstream file;
    file.open(fname);
    if (!file.is_open()) {
        return -1; 
    }

    int n;
    file >> n;
    for (int i = 0; i < n; i ++) {
        std::size_t id;
        std::string first_name, last_name, name;
        int year;
        int zip;
        std::vector<std::size_t> friends;
        friends.clear();

        file >> id;
        file >> first_name >> last_name;
        file >> year;
        file >> zip;
        name = first_name + " " + last_name;

        std::string line;
        std::getline(file, line); 
        std::getline(file, line);
        std::stringstream ss(line);

        std::size_t id2;
        while(ss >> id2) {
            friends.push_back(id2);
        }

        User* u = new User(name, year, zip, friends, id);
        addUser(u);
    }
    return 0;
}

int Network::writeUsers(char *fname){
    std::string userstring, userfriends;
    std::ofstream networkFile (fname);
    if(networkFile.fail()) return -1;
    std::cout << "ID\tName\tBirth Year\tZip Code\n";
    for (size_t i = 0; i < numUsers(); ++i) {
        User* user = getUser(i + 1);
        std::cout << user->getUserId() << "\t" << user->getName() << "\t" << user->getYear() << "\t" << user->getZip() << std::endl;
    }
    return 0;
}

std::size_t Network::numUsers(){
    return users_.size();
}

User* Network::getUser(std::size_t id){
    for(User* user : users_){
        if(user->getUserId() == id) return user;
    }
    return new User();
}

std::vector<size_t> Network::shortestPath(size_t from, size_t to) {
    if (from < 1 or from > numUsers() or to < 1 or to > numUsers()) {
        return std::vector<size_t>();
    }
    std::queue<size_t> q;
    std::vector<bool> visited(numUsers(), 0);
    std::vector<size_t> prev(numUsers(), 0);
    q.push(from);
    prev[from-1] = from;
    while (!q.empty()) {
        size_t currentId = q.front();
        q.pop();
        if (visited[currentId - 1]) continue;
        visited[currentId - 1] = true;
        for (auto friendId : getUser(currentId)->getFriends()) {
            if (!visited[friendId - 1]) {
                q.push(friendId);
                if (prev[friendId - 1] == (unsigned long)(-1) && friendId != from) {
                    prev[friendId - 1] = currentId;
                }
            }
        }
    }
    std::vector<size_t> output;
    size_t cur = from;
    while (cur != to) {
        output.push_back(cur);
        cur = prev[cur - 1];
    }
    output.push_back(to);
    std::reverse(output.begin(), output.end());
    return output;
}

std::vector<std::size_t> Network::distanceUser(std::size_t from, std::size_t& to, std::size_t distance) {
    std::vector<bool> visited(numUsers(), false);
    std::stack<std::size_t> s;
    s.push(from);
    visited[from - 1] = true;
    std::vector<std::size_t> userDistance(numUsers(), 0);
    std::vector<std::size_t> path;
    while (!s.empty()) {
        std::size_t current = s.top();
        if (userDistance[current - 1] == distance) {
            to = current;
            path.push_back(from);
            while (!s.empty()) {
                path.push_back(s.top());
                s.pop();
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        bool found = false;
        for (std::size_t userfriend : getUser(current)->getFriends()) {
            if (!visited[userfriend - 1]) {
                s.push(userfriend);
                visited[userfriend - 1] = true;
                userDistance[userfriend - 1] = userDistance[current - 1] + 1;
                found = true;
            }
        }
        if(!found){
            s.pop();
        }
    }
    to = -1;
    return path;
}


std::vector<std::size_t> Network::suggestFriends(std::size_t who, std::size_t& score) {
    score = 0;
    std::vector<std::size_t> suggestedFriends;
    std::vector<std::size_t> userFriends = getUser(who)->getFriends();
    if (userFriends.empty())  return suggestedFriends;
    std::vector<bool> visited(numUsers(), false);
    for (std::size_t userFriendId : userFriends) {
        if (userFriendId == who) continue;
        std::vector<std::size_t> friendFriends = getUser(userFriendId)->getFriends();
        size_t tempscore = 0;
        for (std::size_t friendFriendId : friendFriends) {
            if (friendFriendId == who || std::find(suggestedFriends.begin(), suggestedFriends.end(), friendFriendId) != suggestedFriends.end()) {
                continue;
            }
            std::vector<std::size_t> friendFriendFriends= getUser(friendFriendId)->getFriends();
            for (std::size_t friendFriendFriendId : friendFriendFriends) {
                if (friendFriendFriendId == who) {
                    tempscore = -10000000;
                    continue;
                }
                if (std::find(userFriends.begin(), userFriends.end(), friendFriendFriendId) != userFriends.end()) ++tempscore;
            }
            if (tempscore > score) {
                suggestedFriends.clear();
                suggestedFriends.push_back(friendFriendId);
                score = tempscore;
                visited[friendFriendId - 1] = true;
            } else if (tempscore == score && !visited[friendFriendId - 1]) {
                suggestedFriends.push_back(friendFriendId);
                visited[friendFriendId - 1] = true;
            }
        }
    }
    for(size_t x = 0; x<suggestedFriends.size(); ++x){
        for(auto b: getUser(who)->getFriends()){
            if(suggestedFriends[x] == b) {
                suggestedFriends.erase(suggestedFriends.begin() + x);
            }
        }
    }
    return suggestedFriends;
}

std::vector<std::vector<std::size_t>> Network::groups() {
    std::vector<std::vector<std::size_t>> connectedComponents;
    std::vector<bool> visited(numUsers() + 1, false);
    for (std::size_t userId = 1; userId <= numUsers(); ++userId) {
        if (!visited[userId]) {
            std::vector<std::size_t> connectedUsers;
            std::stack<std::size_t> stack;
            stack.push(userId);
            visited[userId] = true;
            connectedUsers.push_back(userId);
            while (!stack.empty()) {
                std::size_t currentId = stack.top();
                std::vector<size_t> userFriends = getUser(currentId)->getFriends();
                stack.pop();
                for (std::size_t friendId : userFriends) {
                    if (!visited[friendId]) {
                        stack.push(friendId);
                        visited[friendId] = true;
                        connectedUsers.push_back(friendId);
                    }
                }
            }
            connectedComponents.push_back(connectedUsers);
        }
    }
    return connectedComponents;
}

void Network::addPost(std::size_t ownerId, std::string message, std::size_t likes, bool incoming, std::size_t author, bool isPublic){
    if(incoming) {
        getUser(ownerId)->addPost(new IncomingPost(getUser(author)->getName(), (getUser(ownerId)->getPosts().size()+1), message, likes, isPublic));
    } else {
        getUser(ownerId)->addPost(new Post(getUser(ownerId)->getPosts().size()+1, message, likes));
    }
}

std::string Network::displayPosts(std::string name, std::size_t howMany, bool showOnlyPublic){
    return getUser(getId(name))->displayPosts(howMany, showOnlyPublic);
}

int Network::readPosts(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the posts file." << std::endl;
        return -1;
    }

    int n;
    file >> n;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    for (int x = 0; x < n; ++x) {
        std::string line, message, author, publicity;
        std::size_t postId, likes, userId;
        bool isPublic = true;
        std::getline(file, line);
        std::stringstream sstream(line);
        sstream >> postId;
        std::getline(file, message);
        message.erase(std::remove(message.begin(), message.end(), '\t'), message.end());
        file >> likes;
        file >> userId;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume the newline after userId.
        std::getline(file, line);
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
        if (line[1] == 'u' or line[1] == 'r') {
            if(line[1] == 'u') isPublic = true;
            else isPublic = false;
            std::stringstream sstream(line);
            sstream >> publicity;
            std::getline(file, author);
            author.erase(std::remove(author.begin(), author.end(), '\t'), author.end());
            IncomingPost* tmp = new IncomingPost(author, postId, message, likes, isPublic);
            getUser(userId)->addPost(tmp);
        } else {
            Post* tmp = new Post(postId, message, likes);
            getUser(userId)->addPost(tmp);
        }
    }

    file.close();
    return 0;
}

bool Network::validLogin(std::string name){
    for(auto user: users_){
        if(user->getName() == name) return true;
    }
    return false;
}

