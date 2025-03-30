#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <random>
struct someObject
{
private:
    int field_1;
    char field_2;
    bool owned;
public:
    someObject(int f1, char f2) : field_1(f1), field_2(f2), owned(true) {}
    someObject() : field_1(10), field_2('z'), owned(true) {}

    int getField1() const
    {
        return field_1;
    }
    char getField2() const
    {
        return field_2;
    }
    void setField1(int field)
    {
        field_1 = field;
    }
    void setField2(char field)
    {
        field_2 = field;
    }
    ~someObject() {
        if (owned) {}
    }
    void setOwned(bool own) { owned = own; }
    bool getOwned()
    {
        return owned;
    }
    bool operator<(const someObject& temp) const {
        return field_1 < temp.field_1;
    }

    bool operator>(const someObject& temp) const {
        return field_1 > temp.field_1;
    }
};

template <typename T>
struct arr
{
    int currentSize;
    int maxSize;
    T* array;

    arr() : currentSize(0), maxSize(10), array(new T[10]) {}
    ~arr() {
        deleteAll();
        delete[] array;
    }
   
    void addTail(const T& temp)
    {
        if (currentSize >= maxSize)
        {
            resize();
        }
        array[currentSize++] = temp;
    }

    void resize()
    {
        maxSize *= 2;
        T* temp = new T[maxSize];
        for (int i = 0; i < currentSize; ++i)
        {
            temp[i] = array[i];
        }
        delete[] array;
        array = temp;
    }
    
    T findByIndex(int index)
    {
        if (index >= currentSize || index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        return array[index];
    }
   
    void replaceByIndex(int index, const T& temp)
    {
        if (index >= currentSize || index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        array[index] = temp;
    }
  
    void deleteAll()
    {
        if constexpr (std::is_pointer<T>::value) {

            for (int i = 0; i < currentSize; ++i) {
                if (array[i] != nullptr && array[i]->getOwned())
                    delete array[i];
            }
        }
        currentSize = 0;
    }

    void bubbleSort() {
        for (int i = 0; i < currentSize - 1; ++i) {
            for (int j = 0; j < currentSize - i - 1; ++j) {
                bool swap = false;
                if constexpr (std::is_pointer<T>::value)
                {
                    swap = *array[j] > *array[j + 1];
                }
                else {
                    swap = array[j] > array[j + 1];
                }
                if (swap) {
                    std::swap(array[j], array[j + 1]);
                }
            }
        }
    }
   
    std::string toString()
    {
        int max = 5;
        std::ostringstream output;
        for (int i = 0; i < max && i < currentSize; ++i)
        {
            if constexpr (std::is_arithmetic<T>::value) {
                output << "(" << array[i] << ") ";
            }
            else if constexpr (std::is_pointer<T>::value)
            {
                output << "(" << array[i]->getField1() << " , " << array[i]->getField2() << ") ";

            }
            else {
                output << "(" << array[i].getField1() << " , " << array[i].getField2() << ") ";
            }
        }
        if (currentSize > 10)
        {
            output << "... ";
        }
        if (currentSize >= 5)
        {
            for (int i = currentSize - max; i < currentSize; ++i)
            {
                if constexpr (std::is_arithmetic<T>::value) {
                    output << "(" << array[i] << ") ";
                }
                else if constexpr (std::is_pointer<T>::value)
                {
                    output << "(" << array[i]->getField1() << " , " << array[i]->getField2() << ") ";

                }
                else {
                    output << "(" << array[i].getField1() << " , " << array[i].getField2() << ") ";
                }
            }
        }
        return output.str();
    }
};

int main()
{
    srand(time(0));
    char letters[] = "abcdefghijklmnopqrstuvwxyz";

    arr<someObject*>* a1 = new arr<someObject*>();
    
    const int order = 5;
    const int n = pow(10, order);
    clock_t t1 = clock();
    double max_time_per_element = 0.0;
    double total_time_per_elements = 0.0;
    for (int i = 0; i < n; i++)
    {
        std::cout << "i: " << i << std::endl;
        int x = rand() % 10000 + 1;
        char y = letters[rand() % 26];
        someObject* temp = new someObject(x, y);
        clock_t t1_element = clock();
        a1->addTail(temp);
        clock_t t2_element = clock();
        double time_per_element = (t2_element - t1_element) / (double)CLOCKS_PER_SEC;
        total_time_per_elements += time_per_element;
        if (time_per_element > max_time_per_element)
        {
            max_time_per_element = time_per_element;
            std::cout << "New max time per element: " << max_time_per_element << " at index: " << i << "ooo" <<  std::endl;
            system("pause");
        }
    }
    double avg_time_per_element = total_time_per_elements / n;
    clock_t t2 = clock();
    double total = (t2 - t1) / (double)CLOCKS_PER_SEC;
    std::cout << std::endl << "Total time for adding: " << total << std::endl;
    std::cout << std::endl << "Longest time for single addition: " << max_time_per_element << std::endl;
    //std::cout << std::fixed << std::setprecision(10);
    std::cout << std::endl << "Average time per addition: " << avg_time_per_element * 1000000 << "us" << std::endl;
    std::cout << std::endl << "Array size: " << a1->currentSize << std::endl;
    std::cout << std::endl << "Array: " << a1->toString() << std::endl;


    delete a1;
    return 0;
}

