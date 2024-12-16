//
//  post.hpp
//  Assignment 3
//
//  Created by Cole Roberts on 10/10/23.
//

#pragma once
#ifndef post_hpp
#define post_hpp
#include <stdio.h>
#include <string>

class Post {
public:
    Post();
    Post(std::size_t id, std::string message, std::size_t likes);
    ~Post(){};
    virtual std::string displayPost();
    std::size_t getPostId() {return id_;}
    std::size_t getLikes() {return likes_;}
    std::string getMessage() {return message_;}
    void setPostId(std::size_t id) {id_ = id;}
    void setLikes(std::size_t likes) {likes_ = likes;}
    void setMessage(std::string message) {message_ = message;}
    virtual bool getPublicity() {return true;}
private:
    std::size_t id_;
    std::string message_;
    std::size_t likes_;
};

class IncomingPost: public Post {
public:
    IncomingPost();
    IncomingPost(std::string authorname, std::size_t id, std::string message, std::size_t likes, bool publicity);
    ~IncomingPost(){};
    std::string displayPost();
    bool getPublicity() {return isPublic_;}
private:
    bool isPublic_ = false;
};
#endif /* post_hpp */
