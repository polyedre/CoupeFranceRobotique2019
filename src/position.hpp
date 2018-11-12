#ifndef __POSITION_H_
#define __POSITION_H_

#include "mbed.h"
#include "encoder.hpp"

class Position
{
    public:
        Position(Encoder* encod_l, Encoder* encod_r);
        short get_x();
        short get_y();
        short get_theta();
        void update();
    private:
        float x;
        float y;
        float theta;
        Encoder* encod_l;
        Encoder* encod_r;

};

#endif // __POSITION_H_
