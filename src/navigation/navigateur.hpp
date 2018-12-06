#include "position.hpp"
#include "navigation/vecteur2D.hpp"
#include "asservissement/moteurs.hpp"

#define NAV_PRECISION 0.01f // Précision de la position en mètres

/**
 * Classe pour le navigateur.
 * Celui-ci est capable de piloter le robot par mouvement élémentaire pour le conduire jusqu'au point donné.
 */
class Navigateur
{
    public:
        Navigateur(Position* position, Controleur* controleur);
        /**
         * Définit "cible" comme nouveau point à atteindre par le navigateur.
         */
        void chg_destination(Vecteur2D* cible);
        void update();

    private:
        Position* position;
        Controleur* controleur;
        Vecteur2D* cible;
        float angle_reel;
};