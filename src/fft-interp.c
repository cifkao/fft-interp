#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <tgmath.h>
#include <string.h>
#include "fft-interp.h"

# define M_PI 3.14159265358979323846

size_t reverse(size_t k, size_t n){
  if(n<=2) return k;
  size_t h = n/2;
  return reverse(k%h, h) * 2 + (k>=h);
}

int fft(double complex const *input, double complex *output, size_t n){
  size_t j, k;
  double complex w = exp(I * 2 * M_PI / n); // primitive n-th root of unity
  double complex *y = (double complex *)malloc(n*sizeof(double complex));
  if(y==NULL) return 0;

  for(k=0; k<n; ++k){
    y[k] = input[reverse(k, n)];
  }

  size_t b = 1;
  while(b<n){
    for(j=0; j<n; j += 2*b){
      for(k=0; k<b; ++k){
        double complex d = pow(w, n * k / (2*b)) * y[j+k+b];
        y[j+k+b] = y[j+k] - d;
        y[j+k]   = y[j+k] + d;
      }
    }
    b *= 2;
  }

  memcpy(output, y, n*sizeof(double complex));
  free(y);

  return 1;
}

double *trig_interpolate(double const *input, size_t n){
  // round up to a power of 2
  size_t n_ = 1;
  while(n_<n) n_ *= 2;

  double complex *x = (double complex *)malloc(n_*sizeof(double complex));
  if(x==NULL) return NULL;
  double *out = (double *)malloc((n_+1)*sizeof(double));
  if(out==NULL){ free(out); return NULL; }
  
  size_t i;
  for(i=0; i<n; ++i)
    x[i] = (double complex)input[i];

  // pad with zeros
  for(; i<n_; ++i)
    x[i] = 0;

  // Mr. Fourier
  if(!fft(x, x, n_)){
    free(x);
    free(out);
    return NULL;
  }
  
  // extract coefficients
  out[0] = creal(x[0])/n_;
  for(i=1; i<=n_/2; ++i){
    int mul = 2-(i==n_/2);
    out[2*i-1] = creal(x[i])*mul/n_;
    out[2*i]   = cimag(x[i])*mul/n_;
  }

  return out;
}

