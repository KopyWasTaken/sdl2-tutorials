#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

bool init();
bool loadMedia();
void close(); 

#endif // !MAIN_H
