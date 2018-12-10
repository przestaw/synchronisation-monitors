//
// Created by przemek on 12/10/18.
//

#ifndef COCURRENT_CAR_TYPE_H
#define COCURRENT_CAR_TYPE_H

class Car_Type{
private:
    int prio;
    int dest;
public:
    Car_Type(int seed1, int seed2, int seed3);
    Car_Type();

    bool is_car();
    bool is_prio();
    int get_dest();
};

#endif //COCURRENT_CAR_TYPE_H
