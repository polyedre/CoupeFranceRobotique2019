#ifndef __CONFIG_H_
#define __CONFIG_H_

#define ENCODER_TIM_LEFT TIM4

#define ENCODER_TIM_RIGHT TIM3

#define PI 3.141592653589793
// Il y a un terme correctif
#define DIAMETER (0.247f * 0.9190936949450285f) // largeur en mètres entre les deux encodeurs
// ( ATTENTION : la valeur est arbitraire !)
#define ENCODEUR_ECHELLE 45703.0f // tick par mètres
// Ancienne valeur : 43723.0f // tick par mètres

#endif // __CONFIG_H_
