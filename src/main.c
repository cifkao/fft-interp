#define _POSIX_C_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include "fft-interp.h"

int main(){
  size_t n, i;
  double spacing = NAN;
  char buff[100];

  int terminal = isatty(fileno(stdin));

  if(terminal) printf("Number of points: ");
  // must be a power of 2 if you want periodic output!
  fgets(buff, sizeof(buff), stdin);
  if(sscanf(buff, "%lu", &n)<1)
    return 1;

  if(terminal) printf("Spacing [2pi/%lu]: ", n);
  // leave blank to use the default period of 2pi
  fgets(buff, sizeof(buff), stdin);
  sscanf(buff, "%lf", &spacing);

  // round up to a power of 2
  size_t n_ = 1;
  while(n_<n) n_ *= 2;

  double *x = (double *)malloc((n_+1)*sizeof(double));

  if(terminal) printf("Values:\n");
  for(i=0; i<n; ++i){
    scanf("%lf", &x[i]);
  }

  if(terminal) printf("\n");

  trig_interpolate(x, x, n);

  double epsilon = 1e-9; // get rid of riddiculously small coefficients
  printf("f(x) = %g", fabs(x[0])>epsilon ? x[0] : 0);
  for(i=1; i<=n_/2; ++i){
    double cos = x[2*i-1];
    double sin = x[2*i];
    if(isnan(spacing)){ // spacing is 2pi/n
      if(fabs(cos)>epsilon)
        printf("%+g*cos(%g*x)", cos, ((float)i*n)/n_);
      if(fabs(sin)>epsilon)
        printf("%+g*sin(%g*x)", sin, ((float)i*n)/n_);
    }else{ // custom spacing
      if(fabs(cos)>epsilon)
        printf("%+g*cos(%g*pi*x)", cos, 2*i/(n_*spacing));
      if(fabs(sin)>epsilon)
        printf("%+g*sin(%g*pi*x)", sin, 2*i/(n_*spacing));
    }
  }
  printf("\n");

  free(x);
  return 0;
}
