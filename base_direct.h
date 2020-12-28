#pragma once
#include <windows.h>
#include <d3d11.h>
#include <directxmath.h>
#include <wrl/client.h>
#include <memory>
using Microsoft::WRL::ComPtr;
class DirectWorld {
  private :
    ComPtr<ID3D11Device>  m_device;
    ComPtr<ID3D11DeviceContext>  m_context;
    ComPtr<IDXGISwapChain>  m_swapChain;
    ComPtr<ID3D11RenderTargetView>  m_mainRenderTargetView;
    bool inited ;
  public:
    DirectWorld();
    ~DirectWorld();
    void createRenderTarget();
    bool init(HWND hwnd);
    virtual bool initResource() = 0;
    virtual void draw() = 0;
    ComPtr<ID3D11Device>  getDevice();
    ComPtr<ID3D11DeviceContext>  getContext();
    ComPtr<IDXGISwapChain>  getSwapChain();
    ComPtr<ID3D11RenderTargetView>  getMainRenderTargetView();
    void setViewport();
    HRESULT compileD3DFile(const WCHAR * filename,LPCSTR entryPoint,LPCSTR profile,ID3DBlob ** outData);
    bool createVertexShaderFromFile(const WCHAR * filename,LPCSTR entryPoint,ID3D11VertexShader ** outShader);
    bool createPixelShaderFromFile(const WCHAR  * filename,LPCSTR entryPoint,ID3D11PixelShader ** outShader);
    bool DirectWorld::createInputLayout(const WCHAR *filename,LPCSTR entryPoint
    ,const D3D11_INPUT_ELEMENT_DESC * desc,UINT num,ID3D11InputLayout ** layout);
};
