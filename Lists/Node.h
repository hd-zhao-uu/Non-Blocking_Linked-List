#ifndef NODE_H  // include guard
#define NODE_H

#include <mutex>

struct Node {
    int value;
    Node* next;
    std::mutex lock;

    Node() {}
    Node(int value) : value(value) {}
};

#endif