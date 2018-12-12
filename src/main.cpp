#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <iostream>
#include <thread>
#include <atomic>

#include <pthread.h>
#include "../include/terminal-stream.h"
#include "../include/fdstream.h"
#include "../include/buffer.h"

void do_work(int nr);

std::atomic<bool> my_exit(false);
Buffer buffers[3];

int main()
{
    srand(time(NULL));
    std::thread workers[3];

    auto input = []{
        int a;
        while(!(std::cin >> a))
        {
            std::cin.clear();
            std::cin.sync();
            std::cin.ignore(100, '\n');
            std::cout << "Wrong input\n";
        }
        return a;
    };

    while(my_exit == false)
    {
        std::cout <<"Choose option:\n\n"
                    <<"\t1. Begin\n"
                    <<"\t2. Exit\n";

        switch (input())
        {
            case 1 :
                for(int i = 0; i < 3; i++)
                {
                   workers[i] = std::thread(do_work, (i+1));
                }
                while(!(my_exit.load()))
                {
                    std::cout << "\tPress key: 1. to Exit\n";
                    switch (input())
                    {
                        case 1:
                            my_exit = true;
                            for(auto & it : workers)
                            {
                                it.join();
                            }
                            break;
                        default:
                            std::cout << "Not an option\n";
                            break;
                    }
                }
                break;
            case 2 :
                my_exit = true;
                break;
            default:
                std::cout << "Not an option\n";
                break;
        }
    }
    return 0;
}

void do_work(int nr)
{
    Car_Type new_car;
    Car_Type current;
    while(!my_exit)
    {
        if(!new_car.is_car())
        {
            do {
                new_car = Car_Type(rand(), rand(), rand());

            }while(new_car.get_dest() == nr);
        }

        current = buffers[nr-1].get_car(&new_car, nr);

        if(current.get_dest() == nr)
        {
            current = Car_Type();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(800 + rand()%400));
        if(current.is_car())
        {
            buffers[nr%3].put_car(current);
        }
    }
}