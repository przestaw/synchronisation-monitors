//
// Created by przemek on 12/10/18.
//

#include "../include/buffer.h"
#include "../include/terminal-stream.h"

Buffer::Buffer() :begin(0), end(0), size(0)
{
    for(auto & it : car_buf)
    {
        it = Car_Type();
    }
    my_stream = get_ptmx_stream();
    my_stream << "Hello World : from slave ";
}

Buffer::Buffer(Car_Type first_car) : Buffer() 
{
    put_car(first_car);
}

Car_Type Buffer::get_car()
{
    std::lock_guard<std::mutex> lockGuard(my_mutex);
    Car_Type car;
    if(size == 0)
    {
        car = Car_Type();
    }else
    {
        car = car_buf[begin];
        car_buf[begin] = Car_Type();
        begin = (begin + 1)% BUF_SIZE; //+1
        --size;
        buf_n_full.notify_one();
    }
    return car;
}

Car_Type Buffer::get_car(Car_Type *approching)
{
    std::lock_guard<std::mutex> lockGuard(my_mutex);
    Car_Type car;
    if(size == 0 || approching->is_prio())
    {
        car = *approching;
        *approching = Car_Type();
    }else
    {
        car = car_buf[begin];
        car_buf[begin] = Car_Type();
        begin = (begin + 1)% BUF_SIZE; //+1
        --size;
        buf_n_full.notify_one();
    }
    return car;
}

Car_Type Buffer::get_car(Car_Type *approching, int nr)
{
    std::lock_guard<std::mutex> lockGuard(my_mutex);
    Car_Type car;
    if(size == 0 || approching->is_prio())
    {
        car = *approching;
        *approching = Car_Type();
    }else
    {
        car = car_buf[begin];
        car_buf[begin] = Car_Type();
        begin = (begin + 1)% BUF_SIZE; //+1
        --size;
        buf_n_full.notify_one();
    }
    return car;
}

void Buffer::put_car(Car_Type &next_car)
{
    std::unique_lock<std::mutex> uniqueLock(my_mutex);

    buf_n_full.wait(uniqueLock, std::bind(&Buffer::is_n_full, this));

    size = size + 1;

    if(next_car.is_prio())//if priority move to the begining
    {
        begin = (begin + 9)% BUF_SIZE; //-1
        car_buf[begin] = next_car;
    }else
    {
        car_buf[end] = next_car;
        end = (end + 1)% BUF_SIZE; //+1
    }
}

void Buffer::print() 
{
    int i = (begin + 9 )% BUF_SIZE;
    my_stream << "| "; 
    for(int j = 0; j < 10; j++)
    {
        if(car_buf[i].is_car())
        {
            if(car_buf[i].is_prio())
            {
                my_stream << "[P] ";
            } 
            else
            {
                my_stream << "[%d] ";
            }
        }
        else
        {
            my_stream << "[ ] ";
        }
        i = (i+9)%10;
    }
    my_stream << ": ";
}

bool Buffer::is_n_full()
{
    return (size < BUF_SIZE);
}