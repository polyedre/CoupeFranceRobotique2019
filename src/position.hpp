#ifndef __POSITION_H_
#define __POSITION_H_

#include "mbed.h"
#include "encoder.hpp"

class Position
{
    public:
        Position(Encoder* encod_l, Encoder* encod_r);
        float get_x();
        float get_y();
        float get_theta();
        void update();
    private:
        float x;
        float y;
        float theta;
        Encoder* encod_l;
        Encoder* encod_r;

};

#endif // __POSITION_H_
