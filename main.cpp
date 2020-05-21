#include <windows.h>
#include "main.h"
#include <fmt/format.h>
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "util.h"
#include "device.h"
#include "image.h"
static DirectWorld directWorld;
int WINAPI WinMain(HINSTANCE hi,HINSTANCE ,LPSTR ,int cmd) {
  enableConsole();
  WNDCLASSEX wc = {sizeof(WNDCLASSEX),CS_CLASSDC,WndProc,0L,0L,hi,NULL,NULL,NULL,NULL,TEXT("testing"),NULL};
  ::RegisterClassEx(&wc);
  fmt::print("register windowclassex\n");
  HWND hwnd = CreateWindow(TEXT("testing"),TEXT("ho"),WS_OVERLAPPEDWINDOW,100,100,600,600,NULL,NULL,wc.hInstance,NULL);
  ::ShowWindow(hwnd,cmd);
  ::UpdateWindow(hwnd);
  bool flag = true;
  if(!directWorld.init(hwnd)) {
    directWorld.cleanupDevice();
    fmt::print("init failed\n");
    flag = false;
  }





  MSG msg;
  ZeroMemory(&msg,sizeof(msg));
  while(msg.message != WM_QUIT) {
    if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
      ::TranslateMessage(&msg);
      ::DispatchMessageA(&msg);
      continue;
    }
    directWorld.draw();
  }
  FreeConsole();
  directWorld.cleanupDevice();
  return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
  switch(msg) {
    case WM_SIZE:
      if(wParam != SIZE_MINIMIZED) {
        if(directWorld.getSwapChain() != NULL) {
          directWorld.cleanupRenderTarget();
          directWorld.getSwapChain()->ResizeBuffers(0,(UINT)LOWORD(lParam),(UINT)HIWORD(lParam),DXGI_FORMAT_UNKNOWN,0);
          directWorld.createRenderTarget();
        }
      }
      break;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;
  }
  return ::DefWindowProc(hwnd,msg,wParam,lParam);
}
