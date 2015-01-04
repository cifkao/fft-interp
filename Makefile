fft-interp: src/main.c src/fft-interp.c src/fft-interp.h
	gcc src/main.c src/fft-interp.c -std=c99 -D_POSIX_C_SOURCE=1 -lm -o fft-interp
