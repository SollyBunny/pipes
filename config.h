
#include <stdlib.h>

// Whether the characters bold
#define OPTTBOLD
// Time between frames in us
#define OPTTIME 10000
// Chance for the pipe to change direction
#define OPTCHANCE 80
// Iterations between each eraser tick (undefine to disable)
#define OPTERASERIT 5
// Number of iterations to presimulate
#define OPTPRESIM 5000

// Symbols to use for pipes
const char chars[][4] = {"┃", "━", "┏", "┓", "┗", "┛"};

// Set color to the color for the matrix line
#define DEFAULTR 50
#define DEFAULTG 200
#define DEFAULTB 50
#define V 50 // Variance

inline static int getvariance(int v) {
	return (rand() % v) - (v / 2);
}
inline static unsigned char clampedadd(unsigned char c, int v) {
	if (v > 0) {
		if (c > 254 - v) return 255;
	} else {
		if (c < 1 - v) return 0;
	}
	return c + v;
}
inline static void randcolor(unsigned char *colors, unsigned char r, unsigned char g, unsigned char b) {
	colors[0] = clampedadd(r, getvariance(V));
	colors[1] = clampedadd(g, getvariance(V));
	colors[2] = clampedadd(b, getvariance(V));
}
