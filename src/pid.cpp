#include "pid.hpp"
#include "position.hpp"
#include <queue>
#include <math.h>

/* --- PID général --- */

PID::PID(){}

PID::PID(float _p, float _i, float _d, Position* _pos){

    p = _p;
    i = _i;
    d = _d;
    pos = _pos;

    accumulateur = 0;
}

float PID::calculerConsigne(){
    return p * erreur + i * accumulateur; // TODO : Ajouter la dérivée quand la fifo sera implémentée
}

void PID::AccumulerErreur(float erreur){
    erreur += erreur;
}

float PID::getConsigne(){
    erreur = calculerErreur();
    AccumulerErreur(erreur);
    return calculerConsigne();
}

/* --- PID en DISTANCE --- */

PIDDistance::PIDDistance(float _p, float _i, float _d, Position* _pos) :
    PID(_p, _i, _d, _pos) {}

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
}

/* --- PID en ANGLE --- */

PIDAngulaire::PIDAngulaire(float _p, float _i, float _d, Position* _pos) :
    PID(_p, _i, _d, _pos) {}

float PIDAngulaire::calculerErreur(){
   float theta = pos->get_theta();
   float err = commande_theta - theta; // TODO vérifier le sens !
   fifo.push(err);
   fifo.pop();
   return err;
}

void PIDAngulaire::setCommande(float theta){
   commande_theta = theta;
}

