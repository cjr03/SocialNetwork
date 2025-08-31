//
//  post.cpp
//  Assignment 3
//
//  Created by Cole Roberts on 10/10/23.
//

#pragma once
#include "../include/post.h"
#include <sstream>

Post::Post(){
    id_ = -1;
    message_ = "";
    likes_ = 0;
}

Post::Post(std::size_t id, std::string message, std::size_t likes){
    id_ = id;
    message_ = message;
    likes_ = likes;
}

std::string Post::displayPost(){
    return message_ + "\nLiked by " + std::to_string(likes_) + " people.\n";
}

IncomingPost::IncomingPost(){
    setPostId(-1);
    setMessage("");
    setLikes(0);
    isPublic_ = true;
}

IncomingPost::IncomingPost(std::string authorname, std::size_t id, std::string message, std::size_t likes, bool publicity){
    setPostId(id);
    setMessage(message + "\nFrom: " + authorname);
    setLikes(likes);
    isPublic_ = publicity;
}

std::string IncomingPost::displayPost(){
    return (isPublic_ ? "Public:\n" : "Private:\n") + getMessage() + "\nLiked by " + std::to_string(getLikes()) + " people.\n";
}

