#include "../position.hpp"
#include "vecteur2D.hpp"
#include "../pid.hpp"
#include "mbed.h"

#define NAV_PRECISION 0.01f // Précision de la position en mètres

/**
 * Classe pour le navigateur.
 * Celui-ci est capable de piloter le robot par mouvement élémentaire pour le conduire jusqu'au point donné.
 */
class Navigateur
{
    public:
        Navigateur();
        Navigateur(Position* position, PwmOut *m_l, PwmOut *m_r);
        /**
         * Définit "cible" comme nouveau point à atteindre par le navigateur.
         */
        void set_destination(Vecteur2D* cible);
        void update();
        void print_pos();

    private:
        Position* position;
        Vecteur2D* cible;
        float angle_reel;
        PIDDistance pid_d;
        PIDAngle pid_a;
        PwmOut* m_l;
        PwmOut* m_r;

};
