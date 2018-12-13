#include "position.hpp"
#include <queue>

#ifndef __PID_H_
#define __PID_H_

float calculerAngle(float x1, float y1, float x2, float y2);

class PID { // PID asservissement en position
    public:
        PID();
        PID(float p, float i, float d, float erreurSeuil, float accumulateurSeuil);
        void AccumulerErreur(float erreur);
        float calculerConsigne();
        float getConsigne();
        virtual float calculerErreur() = 0; // A override dans les classes filles
        void stockerErreur();

        void reset();
        float getRampe();

        float erreur;
        float erreurSeuil;

        float accumulateur;
        std::queue<float> fifo;

        float p;
        float i;
        float d;

        short actionFinished;

        Position pos;

        Timer time;
        float accumulateurMax;
};

class PIDDistance : public PID {
    public:
        PIDDistance(float p, float i, float d, float erreurSeuil, float accumulateurSeuil);

        float calculerErreur();
        void setCommande(float x, float y);
        void reset();

        float commande_x;
        float commande_y;
};

class PIDAngle : public PID {
    public:
        PIDAngle(float p, float i, float d, float erreurSeuil, float accumulateurSeuil);

        float calculerErreur();
        void setCommande(float theta);

        float commande_theta;
};

#endif // __PID_H_
