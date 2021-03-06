#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

struct RangeIndex
{
    int begin_index;
    int end_index;
    bool empty() const { return begin_index == end_index; }
    size_t size() const { return end_index-begin_index; }
    
    struct RangeIndexIterator
    {
        int index;
        bool operator!=(const RangeIndexIterator& rg_it) const { return rg_it.index != index; }
        void operator++() { index++; }
        auto& operator*() { return index; }
    };
      
    RangeIndexIterator begin() { return RangeIndexIterator{begin_index}; }
    RangeIndexIterator end() { return RangeIndexIterator{end_index}; }
};


template<typename T>
struct Range
{
    T& container;
    RangeIndex indexes;

    auto begin() { return std::next(container.begin(), indexes.begin_index); }
    auto end() { return std::next(container.begin(), indexes.end_index); }
    
    bool empty() const { return indexes.empty(); }
    size_t size() const { return indexes.size(); }

    auto& operator[](const int i) const
    {
        assert(i >= 0 && i <= indexes.size() - 1);
        return *std::next(container.begin(), indexes.begin_index + i);
    };

    auto& operator[](const int i)
    {
        assert(i >= 0 && i <= indexes.size() - 1);
        return *std::next(container.begin(), indexes.begin_index + i);
    };
};


template<typename T>
Range<T> range(T& container, int begin, int end)
{
    assert( end <= container.size() );
    return Range<T>{container, begin, end};
}

template<typename T>
Range<T&&> range(T&& container, int begin, int end)
{
    assert( end <= container.size() );
    return Range<T&&>{container, begin, end};
}

template<typename T>
Range<T> range(T& container)
{
   return Range<T>{container, 0, (int)container.size()};
}


template<typename T> std::ostream& operator<<(std::ostream& o, Range<T> rg)
{
    if (rg.empty())
        return o << "{}";
    o << " [" << rg.indexes.begin_index << ", " << rg.indexes.end_index << "]: {";

    for (auto& x : rg)
      o << x << ", ";
    o << "\b}";

    return o;
}

template<typename T> void reverse(Range<T> rg)
{
   std::reverse(rg.begin(), rg.end());
}

template<typename T> void sort(Range<T> rg)
{
    std::sort(rg.begin(), rg.end());
}

struct MyVector
{
    int nb_copy = 0;
    std::vector<int> v;

    MyVector()
    {
        ++nb_copy;
    }

    MyVector(const MyVector& mv)
    {
        v = mv.v;
        ++nb_copy;
    }

    int  operator[](const int i) const { return v[i]; }
    int& operator[](const int i)       { return v[i]; }

    MyVector& operator=(const MyVector& mv)
    {
        v = mv.v;
        return *this;
    }

    ~MyVector()
    {
        std::cout << "total copy = " << nb_copy << std::endl;
    }

    auto begin() { return v.begin(); }
    auto end() { return v.end(); }
    size_t size() const { return v.size(); }
};


// void f(const MyVector& v)
// {
//     v[0];
// }
// void f(const Range<MyVector>& r)
// {
//     r[0];
// }


int main()
{
    MyVector v1;
    v1.v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "range(v1): " << range(v1) << std::endl; // range over the whole vector

    auto r1 = range(v1, 2, 7); // chope un bout du vecteur
    std::cout << "r1: " << r1 << std::endl;

    for (int i = 0; i < 8; ++i)
        std::cout << "r1[" << i << "]: " << r1[i] << std::endl;

    return 0;
}
