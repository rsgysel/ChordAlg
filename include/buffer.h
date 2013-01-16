#ifndef BUFFER_H
#define BUFFER_H

#include<limits>
#include<vector>

template< typename T >
class buffer
{
    public:
        buffer(int size);
        ~buffer();

        void clear();                   // clear buffer
        bool isDirty(int) const;
        T& operator[](int);
        const T& operator[](int) const;
    private:
        buffer();
        void reset();

        int size_;
        unsigned int calculation_id_;
        std::vector< unsigned int > dirty_;      // data_ is dirty if dirty_[i] is not calculation_id_
        std::vector< T > data_;
};

template< typename T >
buffer< T >::buffer(int size) : size_(size){
    dirty_.resize(size_);
    data_.resize(size_);
    reset();
    return;
}

template< typename T >
buffer< T >::~buffer(){}

// increments calculation_id_ counter, checking for overflow
template< typename T >
void buffer< T >::clear(){
    if(calculation_id_ == numeric_limits< int >::max()) // calculation_id_ + 1)
        reset();
    ++calculation_id_;
    return;
}

// resets calculation_id_ counter and turns all data dirty
template< typename T >
void buffer< T >::reset(){
    calculation_id_ = 1;
    for(int i = 0; i < size_; ++i)
        dirty_[i] = 0;
    return;
}

// check if data_[i] is dirty
template< typename T >
bool buffer< T >::isDirty(int i) const {
    return dirty_[i] == calculation_id_;
}

// access data_[i]. this operation cleans the data. up to user to check if data was clean
template< typename T >
T& buffer< T >::operator[](int i){
    dirty_[i] = calculation_id_;    // force ith data to be clean
    return data_[i];
}

// access data_[i]. up to user to check if data was clean
template< typename T >
const T& buffer< T >::operator[](int i) const {
    return data_[i];
}

#endif // BUFFER_H
