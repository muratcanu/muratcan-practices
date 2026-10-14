#include <cstddef>
#include <iostream>
#include <string>
#include <stdexcept>

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

    void pop_front() {
        if (head_ != nullptr) {
            if (size_ == 1) {
                delete head_;
                head_ = nullptr;
                tail_ = nullptr;
                size_ = 0;
            } else {
                Node* n = head_;
                head_ = n->next;
                size_--;
                delete n;
            }
        }
    }

    void pop_back() {
        if (tail_ != nullptr) {
            if (size_ == 1)
            {
                delete head_;
                head_ = nullptr;
                tail_ = nullptr;
                size_ = 0;
            } else {
                Node *it = head_;
                Node *behind = nullptr;
                while (it->next != nullptr) {
                    behind = it;
                    it = it->next;
                }
                behind->next = nullptr;
                tail_ = behind;
                size_--;
                delete it;
            }
        }
    }

    T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Invalid index: " + std::to_string(index));
        } else {
            Node* n = head_;
            for (std::size_t i = 0; i < index; i++) {
                n = n->next;
            }
            return n->value;
        }
    }

    const T& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Invalid index: " + std::to_string(index));
        } else {
            Node* n = head_;
            for (std::size_t i = 0; i < index; i++) {
                n = n->next;
            }
            return n->value;
        }
    }

    void clear() {
        Node* n = head_;
        while (n != nullptr) {
            Node* tmp = n->next;
            delete n;
            n = tmp;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
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
    check(list1.size() == 6 && list1.front() == 0 && list1.back() == 5, "linked list got elements pushed front");
    list1.print();
}

void testRemovalAndSafeAccess() {
    std::cout << "Exercise 2: removal and safe access" << std::endl;

    LinkedList<int> list1;
    for (int i = 0; i < 10; i++) {
        list1.push_back(i);
    }
    check(list1.size() == 10 && list1.front() == 0 && list1.back() == 9, "linked list created");
    list1.pop_front();
    check(list1.size() == 9 && list1.front() == 1 && list1.back() == 9, "element at the front popped");
    list1.pop_back();
    check(list1.size() == 8 && list1.front() == 1 && list1.back() == 8, "element at the back popped");
    list1.pop_front();
    list1.pop_back();
    list1.pop_front();
    list1.pop_back();
    list1.pop_front();
    list1.pop_back();
    list1.pop_front();
    list1.pop_back();
    check(list1.size() == 0, "linked list is empty");

    list1.push_back(7);
    list1.push_back(8);
    check(list1.size() == 2 && list1.front() == 7 && list1.back() == 8, "push_back works after popping until empty");

    list1.pop_front();
    list1.pop_front();
    check(list1.size() == 0, "linked list emptied with pop_front");
    list1.push_back(9);
    check(list1.size() == 1 && list1.front() == 9 && list1.back() == 9, "push_back works after pop_front emptied the list");
}

void testAtAndClear() {
    std::cout << "Exercise 2: at and clear" << std::endl;

    LinkedList<int> list1;
    for (int i = 10; i < 15; i++) {
        list1.push_back(i);
    }
    check(list1.at(0) == 10, "at(0) returns the first element");
    check(list1.at(2) == 12, "at(2) returns a middle element");
    check(list1.at(4) == 14, "at(size - 1) returns the last element");

    list1.at(1) = 99;
    check(list1.at(1) == 99, "at() returns a reference that can modify the element");

    const LinkedList<int>& constList = list1;
    check(constList.at(0) == 10 && constList.at(4) == 14, "const at() returns first and last elements");

    bool threw = false;
    try {
        list1.at(5);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    check(threw, "at(size) throws std::out_of_range");

    LinkedList<int> emptyList;
    threw = false;
    try {
        emptyList.at(0);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    check(threw, "at(0) on an empty list throws std::out_of_range");

    list1.clear();
    check(list1.size() == 0 && list1.empty(), "clear() empties the list");

    list1.push_back(1);
    list1.push_back(2);
    check(list1.size() == 2 && list1.front() == 1 && list1.back() == 2, "push_back works after clear()");

    emptyList.clear();
    check(emptyList.size() == 0, "clear() on an empty list does nothing");

    list1.clear();
    list1.clear();
    check(list1.size() == 0, "clear() twice in a row is safe");
}

int main() {
    testBasics();
    testRemovalAndSafeAccess();
    testAtAndClear();

    
    return g_failures == 0 ? 0 : 1;
}