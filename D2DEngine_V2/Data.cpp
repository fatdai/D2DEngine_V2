//
//  Data.cpp
//  D2DEngine
//
//  Created by dai on 15/8/4.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#include "Data.h"
#include <cstdlib>
#include <memory>


namespace D2D {
    
    const Data Data::Null;
    
    Data::Data() :
    _bytes(nullptr),
    _size(0)
    {
        printf("In the empty constructor of Data.\n");
    }
    
    Data::Data(Data&& other) :
    _bytes(nullptr),
    _size(0)
    {
        printf("In the move constructor of Data.\n");
        move(other);
    }
    
    Data::Data(const Data& other) :
    _bytes(nullptr),
    _size(0){
        printf("In the copy constructor of Data.\n");
        copy(other._bytes, other._size);
    }
    
    Data::~Data(){
        printf("deallocing Data: %p\n", this);
        clear();
    }
    
    Data& Data::operator= (const Data& other){
        printf("In the copy assignment of Data.\n");
        copy(other._bytes, other._size);
        return *this;
    }
    
    Data& Data::operator= (Data&& other){
        printf("In the move assignment of Data.\n");
        move(other);
        return *this;
    }
    
    void Data::move(Data& other){
        _bytes = other._bytes;
        _size = other._size;
        
        other._bytes = nullptr;
        other._size = 0;
    }
    
    bool Data::isNull() const
    {
        return (_bytes == nullptr || _size == 0);
    }
    
    unsigned char* Data::getBytes() const
    {
        return _bytes;
    }
    
    ssize_t Data::getSize() const
    {
        return _size;
    }
    
    void Data::copy(unsigned char* bytes, const ssize_t size)
    {
        clear();
        
        if (size > 0)
        {
            _size = size;
            _bytes = (unsigned char*)malloc(sizeof(unsigned char) * _size);
            memcpy(_bytes, bytes, _size);
        }
    }
    
    void Data::fastSet(unsigned char* bytes, const ssize_t size)
    {
        _bytes = bytes;
        _size = size;
    }
    
    void Data::clear()
    {
        free(_bytes);
        _bytes = nullptr;
        _size = 0;
    }

}