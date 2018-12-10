//
// Created by przemek on 12/10/18.
//

#include "../include/car-type.h"

Car_Type::Car_Type() :dest(-1), prio(-1)
{}

Car_Type::Car_Type(int seed1, int seed2, int seed3)
{
    if(seed1 % 3 == 0 || seed1 % 5 == 0)
    {
        this->prio = 0;
        if(seed2 % 8 == 0)
            this->prio = 1;

        this->dest = (1 + (seed3 % 3));

    }else{
        this->prio = -1;
        this->dest = -1;
    }
}

bool Car_Type::is_car() const
{
    return (prio >= 0 && dest >= 0);
}

bool Car_Type::is_prio() const
{
    return (prio > 0);
}

int Car_Type::get_dest() const
{
    return dest;
}