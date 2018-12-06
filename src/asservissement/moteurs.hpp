#include "mbed.h"
#include "encoder.hpp"
#include "position.hpp"

#define PIN_Moteur_droit PB_13
#define PIN_Moteur_gauche PB_15

class Controleur
{
    public:
        Controleur(Position* position);
        void avancer(float distance);
        void rotation(float angle);


    private:
        float puissance;
        PwmOut* moteur_droit;
        PwmOut* moteur_gauche;
        Position* position;
        // il y aura aussi les moteurs
};