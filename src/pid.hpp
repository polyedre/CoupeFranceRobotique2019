#include "position.hpp"
#include <queue>

#ifndef __PID_H_
#define __PID_H_

class PID { // PID asservissement en position
    public:
        PID();
        PID(float p, float i, float d, Position* pos);
        void AccumulerErreur(float erreur);
        float calculerConsigne();
        float getConsigne();
        float calculerErreur(); // A override dans les classes filles
        void stockerErreur();

        float erreur;
        float accumulateur;
        std::queue<float> fifo;

        float p;
        float i;
        float d;

        Position* pos;
};

class PIDDistance : public PID {
    public:
        PIDDistance(float p, float i, float d, Position* pos);
        float calculerErreur();

        void setCommande(float x, float y);

        float commande_x;
        float commande_y;
};

class PIDAngulaire : public PID {
    public:
        PIDAngulaire(float p, float i, float d, Position* pos);
        float calculerErreur();

        void setCommande(float theta);

        float commande_theta;
};

#endif // __PID_H_
