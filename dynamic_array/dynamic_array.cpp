#include <cstddef>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <utility>
constexpr std::size_t kInitialSize = 0;

class DynamicArray {
    public:
        // Constructor
        DynamicArray(std::size_t initial_capacity = 0) {
            size_ = kInitialSize;
            capacity_ = initial_capacity;
            if(initial_capacity > 0) {
                data_ = new int[initial_capacity];
            } else {
                data_ = nullptr;
            }
        }

        // Copy Constructor
        DynamicArray(const DynamicArray& other) {
            data_ = new int[other.size()];
            size_ = other.size();
            capacity_ = other.size();
            for (std::size_t i = 0; i < other.size(); i++) {
                data_[i] = other[i];
            }
        }

        // Move Constructor
        DynamicArray(DynamicArray&& other) noexcept {
            data_ = other.data_;
            size_ = other.size();
            capacity_ = other.capacity();
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        // Destructor
        ~DynamicArray() {
            delete[] data_;
        }

        std::size_t size() const{
            return size_;
        }

        std::size_t capacity() const{
            return capacity_;
        }

        void push_back(int value) {
            if (size_ == capacity_) {
                // Have to grow array
                grow();
            }

            data_[size_] = value;
            size_++;
        }

        int& operator[](std::size_t index) {
            return data_[index];
        }

        const int& operator[](std::size_t index) const {
            return data_[index];
        }

        // Copy Assignment
        DynamicArray &operator=(const DynamicArray &other) {
            size_ = other.size();
            capacity_ = other.size();
            delete[] data_;
            data_ = new int[other.size()];
            for (std::size_t i = 0; i < other.size(); i++) {
                data_[i] = other[i];
            }
            return *this;
        }

        // Move Assignment
        DynamicArray& operator=(DynamicArray&& other) noexcept {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size();
            capacity_ = other.capacity();
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
            return *this;
        }

        int& at(std::size_t index) {
            if (size_ <= index) {
                throw std::out_of_range("Invalid index: " + std::to_string(index));
            } else {
                return data_[index];
            }
        }

        const int& at(std::size_t index) const {
            if (size_ <= index) {
                throw std::out_of_range("Invalid index: " + std::to_string(index));
            } else {
                return data_[index];
            }
        }

        int pop_back() {
            assert(size_ > 0);
            int value = data_[size_ - 1];
            size_--;
            return value;
        }

        int& back() {
            assert(size_ > 0);
            return data_[size_ - 1];
        }

        const int& back() const {
            assert(size_ > 0);
            return data_[size_ - 1];
        }

        int& front() {
            assert(size_ > 0);
            return data_[0];
        }

        const int& front() const {
            assert(size_ > 0);
            return data_[0];
        }

        void clear() {
            size_ = 0;
        }

    private:
        std::size_t capacity_;
        std::size_t size_;
        int* data_;

        void grow() {
            // Create a temporary pointer to not lose existing data
            int* tmpdata = data_;
            if (capacity_ == 0) {
                data_ = new int[1];
                capacity_ = 1;
            } else {
                // Allocate the bigger memory
                data_ = new int[capacity_ * 2];
                capacity_ = capacity_ * 2;
                // Copy each element on old memory to new one
                for(std::size_t i = 0; i < size_; i++) {
                    data_[i] = tmpdata[i];
                }
            }
            delete[] tmpdata;
        }
};

// ---------- Test helpers ----------

int g_failures = 0;

void check(bool condition, const std::string& description) {
    std::cout << (condition ? "  PASS  " : "  FAIL  ") << description << std::endl;
    if (!condition) {
        g_failures++;
    }
}

// Returns an array holding first, first+1, ..., first+count-1
DynamicArray makeArray(int first, std::size_t count) {
    DynamicArray arr;
    for (std::size_t i = 0; i < count; i++) {
        arr.push_back(first + static_cast<int>(i));
    }
    return arr;
}

// True if arr holds exactly first, first+1, ..., first+count-1
bool holdsSequence(const DynamicArray& arr, int first, std::size_t count) {
    if (arr.size() != count) {
        return false;
    }
    for (std::size_t i = 0; i < count; i++) {
        if (arr[i] != first + static_cast<int>(i)) {
            return false;
        }
    }
    return true;
}

// ---------- Exercise 1: push_back and growth ----------

void testGrowth() {
    std::cout << "Exercise 1: push_back and growth" << std::endl;

    DynamicArray arr;
    check(arr.size() == 0 && arr.capacity() == 0, "new array is empty");

    std::size_t expected_capacity = 1;
    bool doubles_correctly = true;
    for (int i = 1; i <= 20; i++) {
        arr.push_back(i);
        if (arr.size() > expected_capacity) {
            expected_capacity *= 2;
        }
        if (arr.capacity() != expected_capacity) {
            doubles_correctly = false;
        }
    }
    check(doubles_correctly, "capacity goes 1, 2, 4, 8, 16, 32");
    check(holdsSequence(arr, 1, 20), "holds 1..20 after growing");

    DynamicArray preallocated(5);
    check(preallocated.size() == 0 && preallocated.capacity() == 5, "constructor with capacity 5");
}

// ---------- Exercise 2: access and removal ----------

void testAccessAndRemoval() {
    std::cout << "Exercise 2: access and removal" << std::endl;

    DynamicArray arr = makeArray(1, 5);  // 1 2 3 4 5
    check(arr.at(0) == 1 && arr.at(4) == 5, "at() reads valid indexes");
    check(arr.front() == 1, "front() is the first element");
    check(arr.back() == 5, "back() is the last element");

    bool threw = false;
    try {
        arr.at(arr.size());
    } catch (const std::out_of_range& e) {
        threw = true;
        std::cout << "        (message: " << e.what() << ")" << std::endl;
    }
    check(threw, "at(size()) throws std::out_of_range");

    check(arr.pop_back() == 5, "pop_back() returns the last element");
    check(arr.size() == 4 && arr.back() == 4, "pop_back() shrinks size by one");

    std::size_t capacity_before = arr.capacity();
    arr.clear();
    check(arr.size() == 0, "clear() makes size 0");
    check(arr.capacity() == capacity_before, "clear() keeps capacity");

    for (std::size_t i = 0; i < capacity_before; i++) {
        arr.push_back(7);
    }
    check(arr.capacity() == capacity_before, "refilling after clear() does not grow");
}

// ---------- Exercise 3: Rule of Five ----------

void testCopyConstructor() {
    std::cout << "Exercise 3: copy constructor" << std::endl;

    DynamicArray original = makeArray(1, 5);
    DynamicArray copy(original);
    check(holdsSequence(copy, 1, 5), "copy has the same elements");
    check(&copy[0] != &original[0], "copy has its own memory block");

    copy[0] = 999;
    check(original[0] == 1, "changing the copy does not change the original");

    // Copy of an array whose size is smaller than its capacity, then grow it
    DynamicArray cleared = makeArray(1, 8);
    cleared.clear();
    DynamicArray clearedCopy(cleared);
    clearedCopy.push_back(42);
    check(clearedCopy.size() == 1 && clearedCopy[0] == 42, "copy of a cleared array can push_back");
}

void testCopyAssignment() {
    std::cout << "Exercise 3: copy assignment" << std::endl;

    DynamicArray original = makeArray(1, 5);

    DynamicArray smaller = makeArray(100, 2);
    smaller = original;
    check(holdsSequence(smaller, 1, 5), "assign into a smaller array");

    DynamicArray bigger = makeArray(100, 50);
    bigger = original;
    check(holdsSequence(bigger, 1, 5), "assign into a bigger array");

    bigger[0] = 999;
    check(original[0] == 1, "changing the target does not change the source");

    // Assigning through a reference so the compiler doesn't warn about a = a
    DynamicArray& sameArray = original;
    original = sameArray;
    check(holdsSequence(original, 1, 5), "self-assignment (a = a) keeps the elements");
}

void testMoveConstructor() {
    std::cout << "Exercise 3: move constructor" << std::endl;

    DynamicArray source = makeArray(1, 5);
    int* sourceBlock = &source[0];

    DynamicArray destination(std::move(source));
    check(holdsSequence(destination, 1, 5), "destination has the elements");
    check(&destination[0] == sourceBlock, "destination took the same block (no copy)");
    check(source.size() == 0 && source.capacity() == 0, "source is empty after the move");

    source.push_back(42);
    check(source.size() == 1 && source[0] == 42, "source can be reused after the move");

    DynamicArray emptySource;
    DynamicArray fromEmpty(std::move(emptySource));
    check(fromEmpty.size() == 0 && fromEmpty.capacity() == 0, "moving an empty array");
}

void testMoveAssignment() {
    std::cout << "Exercise 3: move assignment" << std::endl;

    DynamicArray source = makeArray(1, 5);
    int* sourceBlock = &source[0];

    DynamicArray destination = makeArray(100, 3);
    destination = std::move(source);
    check(holdsSequence(destination, 1, 5), "destination has the elements");
    check(&destination[0] == sourceBlock, "destination took the same block (no copy)");
    check(source.size() == 0 && source.capacity() == 0, "source is empty after the move");

    // Moving through a reference so the compiler doesn't warn about a = std::move(a)
    DynamicArray& sameArray = destination;
    destination = std::move(sameArray);
    check(holdsSequence(destination, 1, 5), "self-move (a = std::move(a)) keeps the elements");
}

int main() {
    testGrowth();
    testAccessAndRemoval();
    testCopyConstructor();
    testCopyAssignment();
    testMoveConstructor();
    testMoveAssignment();

    std::cout << std::endl << g_failures << " test(s) failed" << std::endl;

    // Uncomment to see the assert stop the program on an empty array
    // DynamicArray empty;
    // empty.pop_back();

    return g_failures == 0 ? 0 : 1;
}
