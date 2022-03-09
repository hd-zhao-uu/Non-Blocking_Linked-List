#include <iostream>
#include <limits>
#include "Node.h"

class FineList {
   private:
    Node* head = new Node(std::numeric_limits<int>::min());
    Node* tail = new Node(std::numeric_limits<int>::max());

   public:
    FineList() { head->next = tail; }
    ~FineList() {
        Node* node = NULL;

        while (node != tail) {
            node = head->next;
            delete head;
            head = node;
        }

        delete tail;
    }

    bool add(int value) {
        Node *pre, *cur;
        bool ret;

        head->lock.lock();
        pre = head;
        cur = pre->next;
        cur->lock.lock();

        while (cur->value < value) {
            pre->lock.unlock();
            pre = cur;
            cur = cur->next;
            cur->lock.lock();
        }

        if (cur->value == value) {
            ret = false;
        }

        else {
            Node* node = new Node(value);
            node->next = cur;
            pre->next = node;
            ret = true;
        }

        cur->lock.unlock();
        pre->lock.unlock();
        return ret;
    }

    bool rmv(int value) {
        Node *pre, *cur;
        bool ret;

        head->lock.lock();
        pre = head;
        cur = pre->next;
        cur->lock.lock();

        while (cur->value < value) {
            pre->lock.unlock();
            pre = cur;
            cur = cur->next;
            cur->lock.lock();
        }

        if (cur->value == value) {
            pre->next = cur->next;
            ret = true;
        } else
            ret = false;

        cur->lock.unlock();
        pre->lock.unlock();
        return ret;
    }

    bool ctn(int value) {
        Node *pre, *cur;
        bool ret;

        head->lock.lock();
        pre = head;
        cur = pre->next;
        cur->lock.lock();

        while (cur->value < value) {
            pre->lock.unlock();
            pre = cur;
            cur = cur->next;
            cur->lock.lock();
        }

        if (cur->value == value)
            ret = true;
        else
            ret = false;

        cur->lock.unlock();
        pre->lock.unlock();
        return ret;
    }

    void printList() {
        Node* cur = head->next;
        std::cout << "[";
        while (cur != tail) {
            if (cur != head->next)
                std::cout << ", ";
            std::cout << cur->value;
            cur = cur->next;
        }
        std::cout << "]" << std::endl;
    }
};