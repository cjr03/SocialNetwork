//
//  user.h
//  
//
//  Created by Cole Roberts on 9/20/23.
//

#pragma once
#include <vector>
#include <string>
#include "post.h"

class User
{
public:
    // pre: none
    // post: creates a user with the specified name, year, zipcode, friend list, and id
    User(std::string name, size_t year, size_t zip, std::vector<size_t> friends, size_t id);

    // pre: none
    // post: creates a blank user with placeholder values
    User();

    // pre: none
    // post: destroys user and its methods
    ~User();

    // pre: none
    // post: adds a friend id to the friends_ vector
    void addFriend(size_t id);

    // pre: none
    // post: removes the given id from the user friends_ vector
    void deleteFriend(size_t id);

    // pre: none
    // post: returns User Id
    size_t getUserId();

    // pre: none
    // post: returns User Name
    std::string getName();

    // pre: none
    // post: returns User Year
    size_t getYear();

    // pre: none
    // post: returns User Zip
    size_t getZip();

    // pre: none
    // post: sets User Id to _id
    void setUserId(size_t _id);

    // pre: none
    // post: sets User Name to name
    void setName(std::string name);

    // pre: none
    // post: sets User Year to year
    void setYear(size_t year);

    // pre: none
    // post: sets User Zip to zip
    void setZip(size_t zip);

    // pre: none
    // post: sets User Friends to friends_
    std::vector<size_t> getFriends();

    // pre: post exists
    // post: post is added to user posts
    void addPost(Post *post);

    // pre:
    // post: posts are printed
    std::string displayPosts(std::size_t howMany, bool showOnlyPublic);

    // pre:
    // post:
    std::vector<Post *> getPosts();

private:
    size_t id_;
    std::string name_;
    size_t year_;
    size_t zip_;
    std::vector<size_t> friends_;
    std::vector<Post *> posts_;
};
