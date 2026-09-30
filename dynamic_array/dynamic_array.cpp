#include <cstddef>
#include <iostream>
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

        void pushback(int value) {
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

        const int& operator[](std::size_t index) const{
            return data_[index];
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
    for(int i = 1; i <= 20; i++) {
        arr.pushback(i);
    }
    for(int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << std::endl;
    }


    DynamicArray arr2(5);
    for(int i = 21; i <= 40; i++) {
        arr2.pushback(i);
    }
    for(int i = 0; i < arr2.size(); i++) {
        std::cout << arr2[i] << std::endl;
    }

    return 0;
}