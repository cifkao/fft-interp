#ifndef FFT_INTERP_H
#define FFT_INTERP_H

/**
 * Performs a fast Fourier transform on a vector of length n (must be a
 * power of 2). The output array may be the same as the input array.
 * Returns 0 if memory allocation fails.
 */
int fft(double complex const *input, double complex *output, size_t n);

/**
 * Interpolates a given set of equally spaced data points.
 * Input: (f(0), f(2pi/n), f(2*2pi/n), ..., f((n-1)*2pi/n))
 *   Note that if n is not a power of 2, it shall be rounded
 *   to the nearest higher power of 2 and the data shall
 *   be padded with zeros.
 * Output: an array A of length N+1 (where N the smallest power
 *   of 2 greater than or equal to n) such that
 *
 *     f(x) = A[0] + A[1]*cos(x) + A[2]*sin(x) +
 *                   A[3]*cos(2x) + A[4]*sin(2x) +
 *                   ...
 *                   A[n-1]*cos(nx/2) + A[n]*cos(nx/2).
 *
 * 
 * Output may point to the same array as input if it has sufficient
 * length.
 * Returns 0 if memory allocation fails.
 */
int trig_interpolate(double const *input, double *output, size_t n);

#endif
