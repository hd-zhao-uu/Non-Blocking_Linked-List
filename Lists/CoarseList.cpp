#include <limits>
#include <iostream>
#include <mutex>
#include <vector>
#include "Node.h"

class CoarseList{
    private:
        Node* head = new Node(std::numeric_limits<int>::min()); 
        Node* tail = new Node(std::numeric_limits<int>::max());
        std::mutex mutex;
        int threadId;


    public:
        CoarseList() {
            head->next = tail;
        }

        ~CoarseList() {
            Node* node = NULL;

            while(node != tail) {
                node = head->next;
                delete head;
                head = node;
            }

            delete tail;
        }

        bool add(int value) {
            mutex.lock();
            Node* pre, *cur;
            pre = head;
            cur = head->next;

            while(cur->value < value) {
                pre = cur;
                cur = cur->next;
            }

            // the value is in the set, fail to add
            if(cur->value == value) {
                mutex.unlock();
                return false;
            }
            else {
                // the value is not in the set, add
                Node* node = new Node(value);
                node->next = cur;
                pre->next = node;
                mutex.unlock();
                return true;
            }
        }

        bool rmv(int value) {
            mutex.lock();
            Node* pre, *cur;
            pre = head;
            cur = head->next;

            while(cur->value < value) {
                pre = cur;
                cur = cur->next;
            }

            // the value is in the set, remove
            if(cur->value == value) {
                pre->next = cur->next;
                delete cur;
                mutex.unlock();
                return true;
            } 
            else {
                mutex.unlock();
                return false;
            }
        }

        bool ctn(int value) {
            mutex.lock();
            Node* pre, *cur;
            pre = head;
            cur = head->next;

            while(cur->value < value) {
                pre = cur;
                cur = cur->next;
            }

            // the value is in the set, 
            if(cur->value == value) {
                mutex.unlock();
                return true;
            } 
            else {
                mutex.unlock();
                return false;
            }
        }

        void printList() {
            Node* cur = head->next;
            std::cout << "[";
            while (cur != tail) {
                if(cur != head->next)
                    std::cout << ", ";
                std::cout << cur->value;
                cur = cur->next;
            }
            std::cout << "]" << std::endl;
        }

};


