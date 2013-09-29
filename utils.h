#ifndef UTILS_H
#define UTILS_H

#include <string>

///@return the contents of 'filename'
std::string fileToString(const char *filename);

///@return the bytes for the given PNG 'filename'
unsigned char* loadPngTexture(const char *filename, int *width, int *height,
  bool *hasAlpha);

///@return 0 on successful initialization of graphics context
int initGraphics();

#endif

