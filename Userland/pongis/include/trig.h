//
// Created by UD on 29/5/2025.
//

#ifndef ARQUI_TPE_TRIG_H
#define ARQUI_TPE_TRIG_H

#ifndef PI
#define PI 3.141592653589793
#endif

/**
 * @brief Convert degrees to radians.
 * @param degrees
 * @return
 */
double to_radians(double degrees);

/**
 * @brief Convert radians to degrees.
 * @param radians
 * @return
 */
 double to_degrees(double radians);

 /**
  * @brief Normalize an angle in radians to the range [-π, π].
  * @param x
  * @return
  */
double normalize_radians(double x);

/**
 * @brief Calculate sine using Taylor series expansion.
 * @param x Angle in radians
 * @return Sine of x
 */
double sin_taylor(double x);

/**
 * @brief Calculate cosine using Taylor series expansion.
 * @param x Angle in radians
 * @return Cosine of x
 */
double cos_taylor(double x);

double sin_deg(double degrees);

double cos_deg(double degrees);

#endif //ARQUI_TPE_TRIG_H
