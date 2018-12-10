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
int global;
int main()
{
    std::cout << "Program uses fdstream.h lib by Nicolai M. Josuttis ~ modified by Przmyslaw Stawczyk\n\n";
/*
 * We need to obtain 3 separate slave
 * terminals to see how simulation goes on
 *
 * Each slave will introduce himself with Hello World
 */
/*
    boost::fdostream slave[3]; //terminal streams
    for(auto & slv : slave)
    {
       slv = get_ptmx_stream();
       slv << "Hello World : from slave ";
    }
*/
    srand(time(NULL));
    //boost::fdostream slave1(get_ptmx());
    //boost::fdostream slave2(get_ptmx());
    //boost::fdostream slave3(get_ptmx());
    std::thread workers[3];
/*
    std::thread new_thread([]{
        Car_Type test;
        for(int i = 0; i < 100; i++)
        {
            std::cout << test.is_car() << "  " << test.get_dest() << std::endl;
            test = Car_Type(rand(),rand(),rand());
            global = i;
            std::cout << global << std::endl;
        }
    });
    new_thread.join();

    workers[1] = std::thread([](int nr = 1){
        Car_Type new_car;
        Car_Type current;
        while(!my_exit)
        {
            if(!new_car.is_car())
            {
                do{
                    new_car = Car_Type(rand(),rand(),rand());
                }while(new_car.get_dest() == nr);
            }

            current = buffers[nr-1].get_car(&new_car);

            if(current.get_dest() == nr)
            {
                current = Car_Type();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(current.is_car())
            {
                buffers[nr%3].put_car(current);
            }
        }
    });
    workers[1].join();
    */

    int input1, input2;
    while(my_exit == false)
    {
        printf("Choose option:\n\n"
               "\t1. Begin\n"
               "\t2. Exit\n");
        if(scanf("%d", &input1) != 1)
        {
            input1 = 0;
        }
        while(getc(stdin) != '\n');
        switch (input1)
        {
            case 1 :
                for(int i = 0; i < 3; i++)
                {
                   workers[i] = std::thread(do_work, (i+1));
                }
                while(!(my_exit.load()))
                {
                    printf("\tPress key: 1. to Exit\n");
                    if(scanf("%d", &input2) != 1)
                    {
                        input2 = 0;
                    }
                    while(getc(stdin) != '\n');
                    switch (input2)
                    {
                        case 1:
                            my_exit = true;
                            for(int i = 0; i < 3; i++)
                            {
                                workers[i].join();
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

        current = buffers[nr-1].get_car(&new_car);

        if(current.get_dest() == nr)
        {
            current = Car_Type();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(current.is_car())
        {
            buffers[nr%3].put_car(current);
        }
    }
}