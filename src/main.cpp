#include "mbed.h"
#include "encoder.hpp"
#include "position.hpp"
#include "pid.hpp"
#include <math.h>

DigitalOut led(LED3);
Serial usb(USBTX, USBRX);

PIDDistance pid_distance(0.001, 0.001, 1, 0.05, 100);
PIDAngle pid_angle(0.01, 0.001, 1, 1, 100);

int main()
{
  // bt.baud(9600); // Bluetooth
  usb.baud(115200); // USB

  printf("\r\nInitialisation du programme.\r\n");


  pid_distance.setCommande(1, 1);

  while (!pid_distance.actionFinished) {

    float destination_x = pid_distance.commande_x;
    float destination_y = pid_distance.commande_y;
    float x = pid_distance.pos.get_x();
    float y = pid_distance.pos.get_y();
    float theta = pid_distance.pos.get_theta();

    pid_angle.setCommande(calculerAngle(x, y, destination_x, destination_y));

    wait(0.1);
    float consigne_distance = pid_distance.getConsigne();
    printf("Consigne : %f, x : %f, y : %f, theta : %f, erreur : %f\r\n",
           consigne_distance, x,
           y, theta,
           pid_distance.erreur);

    pid_distance.pos.x += consigne_distance * cos(theta);
    pid_distance.pos.y += consigne_distance * sin(theta);
    pid_distance.pos.theta += pid_angle.getConsigne();
  }

  return 0;
}
