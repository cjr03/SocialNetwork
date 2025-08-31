//
//  user.cpp
//  Assignment 1
//
//  Created by Cole Roberts on 9/20/23.
//

#pragma once
#include "../include/user.h"
#include "post.cpp"
#include <cstdio>
#include <sstream>

User::User(std::string name, size_t year, size_t zip, std::vector<size_t> friends, size_t id){
    name_ = name;
    year_ = year;
    zip_ = zip;
    id_ = id;
    friends_ = friends;
}

User::User(){
    name_ = "User";
    year_ = 2000;
    zip_ = 12345;
    id_ = 0;
}

User::~User(){

}

void User::addFriend(size_t id){
    for(size_t friends = 0; friends < friends_.size(); ++friends){
        if(id == friends_[friends]) return;
    }
    friends_.push_back(id);
}

void User::deleteFriend(size_t id){
    for (size_t x = 0; x < friends_.size(); ++x) {
      if (friends_[x] == id) {
        friends_.erase(friends_.begin() + x);
        break;
      }
    }
}


size_t User::getUserId() {
    return id_;
}


std::string User::getName() {
    return name_;
}

size_t User::getYear() {
    return year_;
}

size_t User::getZip() {
    return zip_;
}

void User::setUserId(size_t _id) {
    id_ = _id;
}

void User::setName(std::string name) {
    name_ = name;
}

void User::setYear(size_t year) {
    year_ = year;
}

void User::setZip(size_t zip) {
    zip_ = zip;
}

std::vector<size_t> User::getFriends() {
    return friends_;
}

void User::addPost(Post* post){
    posts_.push_back(post);
    return;
}

std::string User::displayPosts(std::size_t howMany, bool showOnlyPublic) {
    std::string result;
    std::size_t postsToDisplay = std::min(howMany, posts_.size());
    for (std::size_t i = 0; i < postsToDisplay; ++i) {
        if (!showOnlyPublic || posts_[i]->getPublicity()) {
            result += posts_[i]->displayPost();
          }
        result += "\n\n";
      }
    return result;
}


std::vector<Post*> User::getPosts(){
    return posts_;
}



