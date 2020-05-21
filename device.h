#pragma once
#include <windows.h>
#include <d3d11.h>
#include <directxmath.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
class DirectWorld {
  private :
    ComPtr<ID3D11Device>  m_device;
    ComPtr<ID3D11DeviceContext>  m_context;
    ComPtr<IDXGISwapChain>  m_swapChain;
    ComPtr<ID3D11RenderTargetView>  m_mainRenderTargetView;
    ComPtr<ID3D11PixelShader>  m_pixelShader;
    ComPtr<ID3D11VertexShader>  m_vertexShader;
    ComPtr<ID3D11Buffer>  m_indexBuffer;
    ComPtr<ID3D11Buffer>  m_vertexBuffer;
    ComPtr<ID3D11InputLayout>  m_inputLayout;
    ComPtr<ID3D11ShaderResourceView> m_texture;
    ComPtr<ID3D11SamplerState>  m_samplerState;
    bool inited ;
  public:
    DirectWorld();
    ~DirectWorld();
    void cleanupDevice();
    void cleanupRenderTarget();
    void createRenderTarget();
    bool init(HWND hwnd);
    bool initResource();
    ComPtr<ID3D11Device>  getDevice();
    ComPtr<ID3D11DeviceContext>  getContext();
    ComPtr<IDXGISwapChain>  getSwapChain();
    ComPtr<ID3D11RenderTargetView>  getMainRenderTargetView();
    void draw();
    void setViewport();
    HRESULT compileD3DFile(const WCHAR * filename,LPCSTR entryPoint,LPCSTR profile,ID3DBlob ** outData);
    bool createVertexShaderFromFile(const WCHAR * filename,LPCSTR entryPoint,ID3D11VertexShader ** outShader);
    bool createPixelShaderFromFile(const WCHAR  * filename,LPCSTR entryPoint,ID3D11PixelShader ** outShader);
    bool createVertexBuffer();
    bool createSampler();
};
