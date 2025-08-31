#pragma once
#include "../include/network.h"
#include <cassert>
#include <iostream>

int main()
{
    Network n("resources/social_network.txt");

    User *u = n.getUser(0);
    assert(u != nullptr);

    assert(n.validLogin("Aled Montes"));
    assert(!n.validLogin("Aled Montas"));

    n.addPost(0, "Hello world!", 0, false, 0, true);
    assert(!n.displayPosts("Aled Montes", 1, true).empty());

    std::cout << "All tests passed!\n";
    return 0;
}
