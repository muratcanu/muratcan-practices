#include <cstddef>
#include <iostream>
#include <string>

template <typename T>
class LinkedList {
    public:
    LinkedList() {
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    ~LinkedList() {
        Node* it = head_;
        while(it != nullptr) {
            Node* next = it->next;
            delete it;
            it = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const{
        return size_;
    }

    void push_back(const T& value) {
        Node* n = new Node{value, nullptr};
        if (head_ == nullptr) {
            head_ = n;
            tail_ = n;
        } else {
            tail_->next = n;
            tail_ = n;
        }
        size_++;
    }

    void push_front(const T& value) {
        Node* n = new Node{value, nullptr};
        if (head_ == nullptr) {
            head_ = n;
            tail_ = n;
        } else {
            n->next = head_;
            head_ = n;
        }
        size_++;
    }

    T& front() {
        return head_->value;
    }

    const T& front() const {
        return head_->value;
    }

    T& back() {
        return tail_->value;
    }

    const T& back() const {
        return tail_->value;
    }

    const bool empty() {
        return size_ == 0;
    }

    const void print() {
        Node* it = head_;
        while(it != nullptr) {
            std::cout << std::to_string(it->value) << std::endl;
            it = it->next;
        }
    }

    private:
        struct Node {
            T value;
            Node* next;
        };

        Node* head_;
        Node* tail_;
        std::size_t size_;
    
};

int g_failures = 0;

void check(bool condition, const std::string& description) {
    std::cout << (condition ? "  PASS  " : "  FAIL  ") << description << std::endl;
    if (!condition) {
        g_failures++;
    }
}

void testBasics() {
    std::cout << "Exercise 1: basics" << std::endl;

    LinkedList<int> list1;
    check(list1.size() == 0, "new linked list is empty");
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);
    list1.push_back(4);
    list1.push_back(5);
    check(list1.size() == 5 && list1.front() == 1 && list1.back() == 5, "linked list got elements pushed back");
    list1.push_front(0);
    check(list1.size() == 6 && list1.front() == 0 && list1.back() == 5, "Linked list got elements pushed front");
    list1.print();
}

int main() {
    testBasics();

    
    return g_failures == 0 ? 0 : 1;
}