//
// Created by przemek on 12/10/18.
//
#include <functional>

#include "../include/buffer.h"
#include "../include/terminal-stream.h"

Buffer::Buffer() :begin(0), end(0), size(0), my_stream(get_ptmx())
{
    for(auto & it : car_buf)
    {
        it = Car_Type();
    }
    //my_stream = get_ptmx_stream();
    my_stream << "Hello World : from slave ";
}

Buffer::Buffer(Car_Type first_car) : Buffer() 
{
    put_car(first_car);
}

Car_Type Buffer::get_car() //NOPRINT
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
    if(approching->is_car())
    {
        if(car_buf[begin].is_prio() && size != 0)
        {
            car = car_buf[begin];
            car_buf[begin] = Car_Type();
            begin = (begin + 1)% BUF_SIZE; //+1
            my_stream << "       ";
            this->print();
            my_stream << " -> [P]" << std::endl; //TODO : GONE
            --size;
            buf_n_full.notify_one(); //last thing before return
        }
        else
        {
            car = *approching;
            *approching = Car_Type();
            my_stream << "       ";
            this->print();
            if(car.is_prio())
            {
                my_stream << " <:P:> *appeared*" << std::endl;
            }
            else
            {
                my_stream << " <:" << car.get_dest()<< ":> *appeared*" << std::endl;
            }
        }
    }
    else
    {
        if(size != 0)
        {
            car = car_buf[begin];
            car_buf[begin] = Car_Type();
            begin = (begin + 1)% BUF_SIZE; //+1
            my_stream << "       ";
            this->print();
            if(car.is_prio())
            {
                my_stream << " -> [P]" << std::endl; //TODO : GONE
            }
            else
            {
                my_stream << " -> [" << car.get_dest() << "]" << std::endl; //TODO : GONE
            }
            --size;
            buf_n_full.notify_one(); //last thing before return
        }
        else
        {
            my_stream << "       ";
            this->print();
            my_stream << "       *none*" << std::endl;
        }
    }


    /*
    if(size == 0 || approching->is_prio())
    {
        car = *approching;
        *approching = Car_Type();
        my_stream << "       ";
        this->print();
        if(car.is_prio())
        {
            my_stream << " <:P:> *appeared*" << std::endl;
        }
        else
        {
            my_stream << "       *none*" << std::endl;
        }
    }else
    {
        car = car_buf[begin];
        car_buf[begin] = Car_Type();

        my_stream << "       ";
        this->print();
        if(car.is_prio())
        {
            my_stream << " -> [P]" << std::endl;
        }
        else
        {
            my_stream << " -> [" << car.get_dest() << "]" << std::endl;
        }

        begin = (begin + 1)% BUF_SIZE; //+1
        --size;
        buf_n_full.notify_one();
    }
     */
    return car;
}

Car_Type Buffer::get_car(Car_Type *approching, int nr) //NOPRINT
{
    std::lock_guard<std::mutex> lockGuard(my_mutex);
    Car_Type car;
    if(approching->is_car())
    {
        if(car_buf[begin].is_prio() && size != 0)
        {
            car = car_buf[begin];
            car_buf[begin] = Car_Type();
            begin = (begin + 1)% BUF_SIZE; //+1
            my_stream << "       ";
            this->print();
            if(car.get_dest() == nr)
            {
                my_stream << " -> [P] *GONE*" << std::endl;
            }
            else
            {
                my_stream << " -> [P]" << std::endl;
            }
            --size;
            buf_n_full.notify_one(); //last thing before return
        }
        else
        {
            car = *approching;
            *approching = Car_Type();
            my_stream << "       ";
            this->print();
            if(car.is_prio())
            {
                my_stream << " <:P:> *appeared*" << std::endl;
            }
            else
            {
                my_stream << " <:" << car.get_dest()<< ":> *appeared*" << std::endl;
            }
        }
    }
    else
    {
        if(size != 0)
        {
            car = car_buf[begin];
            car_buf[begin] = Car_Type();
            begin = (begin + 1)% BUF_SIZE; //+1
            my_stream << "       ";
            this->print();
            if(car.is_prio())
            {
                my_stream << " -> [P]" ;
            }
            else
            {
                my_stream << " -> [" << car.get_dest() << "]";
            }
                //Looks ugly but does it's job
            if(car.get_dest() == nr)
            {
                my_stream << " *GONE*" << std::endl;
            }
            else
            {
                my_stream << std::endl;
            }
            --size;
            buf_n_full.notify_one(); //last thing before return
        }
        else
        {
            my_stream << "       ";
            this->print();
            my_stream << "       *none*" << std::endl;
        }
    }


    /*
    if(size == 0 || approching->is_prio())
    {
        car = *approching;
        *approching = Car_Type();
        my_stream << "       ";
        this->print();
        if(car.is_prio())
        {
            my_stream << " <:P:> *appeared*" << std::endl;
        }
        else
        {
            my_stream << "       *none*" << std::endl;
        }
    }else
    {
        car = car_buf[begin];
        car_buf[begin] = Car_Type();

        my_stream << "       ";
        this->print();
        if(car.is_prio())
        {
            my_stream << " -> [P]" << std::endl;
        }
        else
        {
            my_stream << " -> [" << car.get_dest() << "]" << std::endl;
        }

        begin = (begin + 1)% BUF_SIZE; //+1
        --size;
        buf_n_full.notify_one();
    }
     */
    return car;
}

void Buffer::put_car(Car_Type &next_car)
{
    std::unique_lock<std::mutex> uniqueLock(my_mutex);

    buf_n_full.wait(uniqueLock, std::bind(&Buffer::is_n_full, this));

    size = size + 1;

    if(next_car.is_prio())//if priority move to the begining
    {
        begin = (begin + BUF_SIZE -1)% BUF_SIZE; //-1
        car_buf[begin] = next_car;
        my_stream << "[P]*-> ";

    }else
    {
        car_buf[end] = next_car;
        end = (end + 1)% BUF_SIZE; //+1
        my_stream << "[" << next_car.get_dest() << "] -> ";
    }
    this->print();
    my_stream << std::endl;
}

void Buffer::print() 
{
    int i = (begin + BUF_SIZE -1 )% BUF_SIZE;
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
                my_stream << "[" << car_buf[i].get_dest() << "] ";
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