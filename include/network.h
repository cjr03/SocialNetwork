//
//  network.h
//  
//
//  Created by Cole Roberts on 9/21/23.
//

#pragma once
#include "user.h"
#include <vector>
#include <string>

class Network {
public:

    // pre: none
    // post: a network with no users is created
    Network();

    // pre: none
    // post: a network is created from the copy
    Network(std::vector<User*> copy);

    Network(const char* file);


    // pre: none
    // post: network and its methods are destroyed
    ~Network();


    // pre: (User* != nullptr)
    // post: user is pushed onto the users_ vector and becomes a part of the network
    void addUser(User* user);

    // pre: none
    // post: the user associated with s1 has the id of s2 pushed onto its friends_ vector and vise versa
    int addConnection(std::string s1, std::string s2);

    // pre: none
    // post: the user associated with s1 has the id of s2 removed from its friends_ vector and vise versa
    int removeConnection(std::string s1, std::string s2);

    // pre: none
    // post: returns the id of the user associated with name
    std::size_t getId(std::string name);

    // pre: none
    // post: prints the contents of file fname to the concole
    int readUsers(const char* fname);

    // pre: none
    // post: writes to a file named fname all of the users data in the users_ vector
    int writeUsers(char *fname);

    // pre: none
    // post: returns the number of users
    std::size_t numUsers();

    // pre: none
    // post: returns the shortest path between 2 users
    std::vector<size_t> shortestPath(size_t from, size_t to);

    // pre: none
    // post: returns the users "distance" away from them
    std::vector<std::size_t> distanceUser (std::size_t from, std::size_t& to, std::size_t distance);

    // pre: none
    // post: returns sugguested friends to who
    std::vector<std::size_t> suggestFriends(std::size_t who, std::size_t& score);

    // pre: none
    // post: returns users who are only friends with one other user and vise versa
    std::vector<std::vector<std::size_t>> groups();

    // pre: none
    // post: returns a pointer to the user associated with the id given
    User* getUser(std::size_t id);

    // pre: none
    // post: adds a post to the post vector inside of a specific user
    void addPost(std::size_t ownerId, std::string message, std::size_t likes, bool incoming, std::size_t author, bool isPublic);

    // pre: non
    // post: returns a string of all posts
    std::string displayPosts(std::string name, std::size_t howMany, bool showOnlyPublic);

    // pre: file opens
    // post: posts are written to the network
    int readPosts(const char* filename);

    bool validLogin(std::string name);
private:
    std::vector<User*> users_;
};

