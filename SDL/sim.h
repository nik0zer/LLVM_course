#include <stdlib.h>
#define SIM_X_SIZE 512
#define SIM_Y_SIZE 512
#define NULL_PTR NULL

void simFlush();
void simPutPixel(int x, int y, int argb);
int simRand(int start, int end);

void simInit();
void app();
int simExit();