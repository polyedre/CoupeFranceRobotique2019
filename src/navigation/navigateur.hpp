#include "../position.hpp"
#include "vecteur2D.hpp"
#include "../pid.hpp"
#include "mbed.h"

#define NAV_PRECISION 0.01f // Précision de la position en mètres
#define PWM_PERIOD 0.000033F
/**
 * Classe pour le navigateur.
 * Celui-ci est capable de piloter le robot par mouvement élémentaire pour le conduire jusqu'au point donné.
 */
class Navigateur
{
    public:
        Navigateur();
        Navigateur(Position* position, PwmOut *m_l, PwmOut *m_r, DigitalOut *_d_l, DigitalOut *_d_r, Encoder * encod_l, Encoder * encod_r);
        /**
         * Définit "cible" comme nouveau point à atteindre par le navigateur.
         */
        void set_destination(float x, float y);
        void update();
        void print_pos();
        Position* position;
        void rotate_by(float angle);
        void avancer(float distance);
        void reset();
        void updatePos();
        PIDVitesse pid_v_l;
        PIDVitesse pid_v_r;
        PIDDistance pid_d;
        PIDAngle pid_a;

    private:
        float cible_x = 0;
        float cible_y = 0;
        float angle_reel;
        PwmOut* m_l;
        PwmOut* m_r;
        DigitalOut* d_l;
        DigitalOut* d_r;

};
