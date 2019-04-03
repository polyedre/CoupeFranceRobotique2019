#include "pid.hpp"
#include "position.hpp"
#include "encoder.hpp"
#include "config.hpp"
#include "base.hpp"
#include <queue>
#include <math.h>

// Durée de la rampe en seconde
#define RAMPE_LENGHT 2.0f

extern int debug_monitor;

/* --- PID général --- */

PID::PID(){} //kewa?

PID::PID(float _p, float _i, float _d, float _erreurSeuil, float _accumulateurSeuil, Position* position){

    p = _p;
    i = _i;
    d = _d;

    accumulateur = 0;
    actionFinished = 0;
    derivee_data[0] = 0;
    derivee_data[1] = 0;
    derivee_data[2] = 0;

    pos = position;

    accumulateurSeuil = _accumulateurSeuil;
    erreurSeuil = _erreurSeuil;
    time.start();

    last_time = time.read();

    for (int i = 0; i < 1000; i++) {
        fifo.push(0);
    }
}

float PID::calculerConsigne(){
    // TODO : Ajouter la dérivée
    return (p * erreur + i * accumulateur + 0 * getDerivee());// * getRampe();
}

static void tab_shift(float *tab, int tab_size) {
    for (int i = tab_size; i > 0; i--) {
        tab[i] = tab[i-1];
    }
}

float PID::getDerivee() {
    float dt = 1 / 1000.0f;
    // printf("%f, %f, %f, %f\n", derivee_data[0], derivee_data[1], derivee_data[2], dt);
    return (((derivee_data[0] - derivee_data[1]) / dt) +   ((derivee_data[1] - derivee_data[2]) / dt)) / 2;
}

void PID::AccumulerErreur(float erreur){
    accumulateur += erreur;
    accumulateur -= fifo.back();

    fifo.push(erreur);
    fifo.pop();

    tab_shift(derivee_data, 3);
    derivee_data[0] = erreur;
}

float PID::getConsigne(){
    erreur = this->calculerErreur();
    if (abs(erreur) < erreurSeuil) {
        actionFinished = 1;
        reset();
    } else {
    AccumulerErreur(erreur);
    actionFinished = 0;
    }
    return calculerConsigne();
}

float PID::getRampe(){
    if (time.read() <  RAMPE_LENGHT) return time.read() / RAMPE_LENGHT;
    return 1;
}

void PID::reset(){
    accumulateur = 0;
    time.reset();
}

/* --- PID en DISTANCE --- */

PIDDistance::PIDDistance(){}

PIDDistance::PIDDistance(float _p, float _i, float _d, float _erreurSeuil, float _accumulateurSeuil, Position* position) :
    PID(_p, _i, _d, _erreurSeuil, _accumulateurSeuil, position) {}

float PIDDistance::calculerErreur(){
   float x = pos->get_x();
   float y = pos->get_y();

   float err = sqrt(carre(x - commande_x) + carre(y - commande_y));

   if (debug_monitor) printf("ED:%.2f ", err);

   return err;
}

void PIDDistance::setCommande(float x, float y){
    commande_x = x;
    commande_y = y;
    PID::reset();
    this->time.reset();
}

/* --- PID en ANGLE --- */

PIDAngle::PIDAngle(){}

PIDAngle::PIDAngle(float _p, float _i, float _d, float _erreurSeuil, float _accumulateurSeuil, Position* position) :
    PID(_p, _i, _d, _erreurSeuil, _accumulateurSeuil, position) {}

float PIDAngle::calculerErreur(){
   float theta = pos->get_theta();
   float err = 0;
   float err_devant = modulo_angle(commande_theta - theta);
   float err_derriere = modulo_angle(commande_theta - (theta + PI));
   if (abs(err_devant) < abs(err_derriere)) {
       err = err_devant;
   } else {
       err = err_derriere;
   }
  
   //  printf("Erreur Angle : %f, comm = %f, theta = %f\r\n", err, commande_theta, theta);
   if (debug_monitor) printf("EA:%.2f \n", err);
   return err;
}

void PIDAngle::setCommande(float theta){
   commande_theta = theta;
}

float calculerAngle(float x1, float y1, float x2, float y2)
{
    /*
     * On calcule l'angle entre le vecteur (0, 1) et le vecteur (x2 - x1, y2 - y1)
     * En calculant le produit scalaire de ces deux vecteur divisé par le produit de
     * leur normes.
     * norme_v représente la norme de ce 2ème vecteur.
     * */


    if (abs(x1 - x2) < 0.02) { // Le robot et sa destination sont sur une même ligne
                               // horizontale
        if (y1 > y2) return -1.6f; // 90 deg
        else return 1.6f;
    }

    float norme_v = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    float rapport = (x2-x1)/norme_v;
    if (rapport < 0) return acos(rapport);
    else return -acos(rapport);
}
