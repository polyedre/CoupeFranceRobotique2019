#include "position.hpp"
#include <queue>

#ifndef __PID_H_
#define __PID_H_

class PID { // PID asservissement en position
    public:
        PID();
        PID(float p, float i, float d);
        void AccumulerErreur(float erreur);
        float calculerConsigne();
        float getConsigne();
        float calculerErreur(); // A override dans les classes filles
        void stockerErreur();

        void reset();
        float getRampe();

        float erreur;
        float accumulateur;
        std::queue<float> fifo;

        float p;
        float i;
        float d;

        Position pos;

        Timer time;
};

class PIDDistance : public PID {
    public:
        PIDDistance(float p, float i, float d);

        float calculerErreur();
        void setCommande(float x, float y);
        void reset();

        float commande_x;
        float commande_y;
};

class PIDAngulaire : public PID {
    public:
        PIDAngulaire(float p, float i, float d);

        float calculerErreur();
        void setCommande(float theta);
        void reset();

        float commande_theta;
};

#endif // __PID_H_
