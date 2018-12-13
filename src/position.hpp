#ifndef __POSITION_H_
#define __POSITION_H_

#include "mbed.h"
#include "encoder.hpp"

class Position
{
    public:
        Position();
        Position(Encoder* encod_l, Encoder* encod_r);
        float get_x();
        float get_y();
        float get_theta();
        void update();
        void reset();
        void init();
    // private:
        float x;
        float y;
        float theta;
        Encoder* encod_l;
        Encoder* encod_r;

};

#endif // __POSITION_H_
