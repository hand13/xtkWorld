#include "util.h"
#include <windows.h>
#include <cstdio>
void enableConsole() {
    AllocConsole();  //create console
    SetConsoleTitle("debug console");
    FILE* tempFile = nullptr;
    freopen_s(&tempFile, "conin$", "r+t", stdin);
    freopen_s(&tempFile, "conout$", "w+t", stdout);
}
unsigned int readFile2Buffer(void ** address,const char * filename) {
  FILE * hf = NULL;
  void * m = ::malloc(2048);
  int length = 0;
  if(::fopen_s(&hf,filename,"r")) {
    length = fread_s(m,2048,1,2048,hf);
  }
  *address = m;
  return length;
}
