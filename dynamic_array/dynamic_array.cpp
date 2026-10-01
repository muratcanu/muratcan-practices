#include <cstddef>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstdlib>
constexpr std::size_t kInitialSize = 0;

class DynamicArray {
    public:
        DynamicArray(std::size_t initial_capacity = 0) {
            size_ = kInitialSize;
            capacity_ = initial_capacity;
            if(initial_capacity > 0) {
                data_ = new int[initial_capacity];
            } else {
                data_ = nullptr;
            }
        }

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

int main() {
    DynamicArray arr;
    for(std::size_t i = 1; i <= 20; i++) {
        arr.push_back(i);
    }
    for(std::size_t i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << std::endl;
    }


    DynamicArray arr2(5);
    for(std::size_t i = 21; i <= 40; i++) {
        arr2.push_back(i);
    }
    for(std::size_t i = 0; i < arr2.size(); i++) {
        std::cout << arr2[i] << std::endl;
    }

    try {
        DynamicArray arr3;
        std::cout << arr3.at(5) << std::endl;
    } catch(const std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "something else: " << e.what() << std::endl;
    }

    DynamicArray arr4(10);
    for(int i = 0; i < 10; i++) {
        arr4.push_back(rand() % 101);
    }
    std::cout << arr4.pop_back() << std::endl;
    std::cout << arr4.back() << std::endl;
    std::cout << arr4.front() << std::endl;
    std::cout << "Size: " + std::to_string(arr4.size()) + " | Capacity: " + std::to_string(arr4.capacity()) << std::endl;
    // Lets clear
    arr4.clear();
    for(int i = 0; i < 10; i++) {
        arr4.push_back(rand() % 101);
    }
    std::cout << "Size: " + std::to_string(arr4.size()) + " | Capacity: " + std::to_string(arr4.capacity()) << std::endl;

    DynamicArray arr5;
    std::cout << arr5.pop_back() << std::endl;
    std::cout << arr5.back() << std::endl;
    std::cout << arr5.front() << std::endl;
    std::cout << "Size: " + std::to_string(arr5.size()) + " | Capacity: " + std::to_string(arr5.capacity()) << std::endl;
    // Lets clear
    arr5.clear();
    std::cout << "Size: " + std::to_string(arr5.size()) + " | Capacity: " + std::to_string(arr5.capacity()) << std::endl;

    return 0;
}