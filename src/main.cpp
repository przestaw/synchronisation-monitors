#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <iostream>

#include "../include/terminal-stream.h"
#include "../include/fdstream.h"
#include "../include/buffer.h"

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
    //boost::fdostream slave1(get_ptmx());
    //boost::fdostream slave2(get_ptmx());
    //boost::fdostream slave3(get_ptmx());

    Buffer buffers[3];
    //TODO: require printing implementation in put/get

    int exit = 0;
    int input1;
    while(!exit)
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
                //TODO : Make program
                // CREATE THREADS
                break;
            case 2 :
                exit = 1;
                break;
            default:
                std::cout << "Not an option\n";
                break;
        }
    }
    return 0;
}

void do_work(Buffer in, Buffer out, int nr)
{
    Car_Type new_car;
    Car_Type current;
    while(1)
    {
        if(new_car.is_car())
        {
            do{
                new_car = Car_Type(rand(),rand(),rand());
            }while(new_car.get_dest() == nr);
        }

        current = in.get_car(&new_car);

        if(current.get_dest() == nr)
        {
            current = Car_Type();
        }
        //TODO : SLEEP
        if(current.is_car())
        {
            out.put_car(current);
        }
    }
}