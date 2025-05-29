//
// Created by UD on 29/5/2025.
//
//
//#include "trig.h"
//
//
////double sin_taylor(double x){
////    double term = x; // First term of the series
////    double result = term;// Initialize result with the first term
////    double x_squared = x * x;// Square of x for the series
////    int sign = -1;
////
////    for (int n = 3; n <= 15; n += 2) {
////        term *= x_squared / (n * (n - 1)); // Calculate the next term in the series
////        result += sign * term; // Add or subtract the term from the result
////        sign *= -1; // Alternate the sign for the next term
////    }
////    return result;
////}
//
//double cos_taylor(double x){
//    double term = 1; // First term of the series
//    double result = term; // Initialize result with the first term
//    double x_squared = x * x; // Square of x for the series
//    int sign = -1;
//
//    for (int n = 2; n <= 14; n += 2) {
//        term *= x_squared / (n * (n - 1)); // Calculate the next term in the series
//        result += sign * term; // Add or subtract the term from the result
//        sign *= -1; // Alternate the sign for the next term
//    }
//    return result;
//}
//
//double to_radians(double degrees) {
//    return degrees * (PI / 180.0); // Convert degrees to radians
//}
//
//double to_degrees(double radians) {
//    return radians * (180.0 / PI); // Convert radians to degrees
//}
//
//double normalize_radians(double x){
//    while (x>PI) x -= 2*PI; // Reduce x to the range [-π, π]
//    while (x<-PI) x += 2*PI;
//    return x;
//}
//
//double sin_deg(double degrees) {
//    return sin_taylor(normalize_radians(to_radians(degrees))); // Calculate sine using Taylor series
//}
//
//double cos_deg(double degrees) {
//    return cos_taylor(normalize_radians(to_radians(degrees))); // Calculate cosine using Taylor series
//}
//


