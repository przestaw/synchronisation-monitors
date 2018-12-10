//
// Created by przemek on 12/10/18.
//

#ifndef COCURRENT_BUFFER_H
#define COCURRENT_BUFFER_H

#include <condition_variable>
#include "car-type.h"
#include "fdstream.h"

#define BUF_SIZE 10

class Buffer
{
private:
    std::mutex my_mutex;
    std::condition_variable buf_n_full;
    Car_Type car_buf[BUF_SIZE];
    boost::fdostream my_stream;
    int begin;
    int end;
    int size;
    void print();
public:
    Buffer();
    Buffer(Car_Type first_car);

    Car_Type get_car();
    Car_Type get_car(Car_Type *approching);
    Car_Type get_car(Car_Type *approching, int nr);

    void put_car(Car_Type &next_car);

    bool is_n_full();
};

#endif //COCURRENT_BUFFER_H
