#include <windows.h>
#include "main.h"
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "direct_world.h"
static TheWorld theWorld;
int WINAPI WinMain(HINSTANCE hi,HINSTANCE ,LPSTR ,int cmd) {
  WNDCLASSEX wc = {sizeof(WNDCLASSEX),CS_CLASSDC,WndProc,0L,0L,hi,NULL,NULL,NULL,NULL,TEXT("testing"),NULL};
  ::RegisterClassEx(&wc);
  HWND hwnd = CreateWindow(TEXT("testing"),TEXT("ho"),WS_OVERLAPPEDWINDOW,100,100,600,600,NULL,NULL,wc.hInstance,NULL);
  bool flag = true;
  if(!theWorld.init(hwnd)) {
    flag = false;
    return -1;
  }

  ::ShowWindow(hwnd,cmd);
  ::UpdateWindow(hwnd);
  MSG msg;
  ZeroMemory(&msg,sizeof(msg));
  while(msg.message != WM_QUIT) {
    if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
      ::TranslateMessage(&msg);
      ::DispatchMessageA(&msg);
      continue;
    }
    theWorld.draw();
  }
  return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
  switch(msg) {
    case WM_SIZE:
      if(wParam != SIZE_MINIMIZED) {
        if(theWorld.getSwapChain() != NULL) {
          theWorld.getSwapChain()->ResizeBuffers(0,(UINT)LOWORD(lParam),(UINT)HIWORD(lParam),DXGI_FORMAT_UNKNOWN,0);
          theWorld.createRenderTarget();
        }
      }
      break;
    case WM_LBUTTONUP:
      return 0;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;
  }
  return ::DefWindowProc(hwnd,msg,wParam,lParam);
}
