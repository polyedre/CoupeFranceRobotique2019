#ifndef __BASE_H_
#define __BASE_H_

/**
 * @brief Return the squared value of a
 * 
 * @param a 
 * @return float 
 */
float carre(float a);

/**
 * @brief Return angle between 0 and 2 PI
 * 
 * @param angle 
 * @return float 
 */
float modulo_angle_absolu(float angle);

/**
 * @brief Return angle between -PI and PI
 * 
 * @param angle 
 * @return float 
 */
float modulo_angle_relatif(float angle);

/**
 * @brief Convert angle from radian to degree
 * 
 * @param angle 
 * @return float 
 */
float convert_degree(float angle);

#endif // __BASE_H_
