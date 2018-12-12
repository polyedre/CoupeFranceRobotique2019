#include "pid.hpp"
#include "position.hpp"
#include <queue>
#include <math.h>

// Durée de la rampe en seconde
#define RAMPE_LENGHT 5

/* --- PID général --- */

PID::PID(){}

PID::PID(float _p, float _i, float _d){

    p = _p;
    i = _i;
    d = _d;

    accumulateur = 0;
    pos.init();
}

float PID::calculerConsigne(){
    // TODO : Ajouter la dérivée
    return p * erreur + i * accumulateur;
}

void PID::AccumulerErreur(float erreur){
    erreur += erreur;
}

float PID::getConsigne(){
    erreur = calculerErreur();
    AccumulerErreur(erreur);
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

PIDDistance::PIDDistance(float _p, float _i, float _d) :
    PID(_p, _i, _d) {}

// FIXME : Fonction mal placée
float carre(float a){
    return a * a;
}

float PIDDistance::calculerErreur(){
   float x = pos->get_x();
   float y = pos->get_y();

   float err = sqrt(carre(x - commande_x) + carre(y - commande_y));
   fifo.push(err);
   fifo.pop();
   return err;
}

void PIDDistance::setCommande(float x, float y){
    commande_x = x;
    commande_y = y;
    reset();
}

/* --- PID en ANGLE --- */

PIDAngulaire::PIDAngulaire(float _p, float _i, float _d) :
    PID(_p, _i, _d) {}

float PIDAngulaire::calculerErreur(){
   float theta = pos->get_theta();
   float err = commande_theta - theta; // TODO vérifier le sens !
   fifo.push(err);
   fifo.pop();
   return err;
}

void PIDAngulaire::setCommande(float theta){
   commande_theta = theta;
   reset();
}

