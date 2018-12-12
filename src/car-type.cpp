//
// Created by przemek on 12/10/18.
//

#include "../include/car-type.h"

Car_Type::Car_Type() :dest(-1), prio(-1)
{}

Car_Type::Car_Type(int seed1, int seed2, int seed3)
{
    if(seed1 % CAR_P_1 == 0 || seed1 % CAR_P_2 == 0)
    {
        this->prio = 0;
        if(seed2 % CAR_PRIO_P == 0)
            this->prio = 1;

        this->dest = (1 + (seed3 % CAR_DEST_NR));

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