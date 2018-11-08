#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "mbed.h"

class Encoder
{
    public:
        Encoder(TIM_TypeDef* TIM);
        void reset();
        int get();
    private:
};


void TIM3_EncoderInit();

void TIM4_EncoderInit();

#endif // __ENCODER_H_
