#include "mbed.h"
#include "encoder.hpp"
#include "navigation/navigateur.hpp"
#include "navigation/vecteur2D.hpp"
#include "position.hpp"
#include "pid.hpp"
#include <math.h>

DigitalOut led(LED3);
Serial usb(USBTX, USBRX);

Encoder enc_l(TIM4);
Encoder enc_r(TIM3);

Position pos(&enc_l, &enc_r);
Navigateur nav(&pos);

int main()
{
  // bt.baud(9600); // Bluetooth
  usb.baud(115200); // USB

  usb.printf("\r\nInitialisation du programme.\r\n");

  Vecteur2D destination(1,1);
  nav.set_destination(&destination);

  return 0;
}
