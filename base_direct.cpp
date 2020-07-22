#include "base_direct.h"
#include <iostream>
#include <d3dcompiler.h>
#include <wincodec.h>
bool DirectWorld::init(HWND hWnd) {
  inited = false;
  DXGI_SWAP_CHAIN_DESC desc;
  ZeroMemory(&desc,sizeof(desc));
  desc.OutputWindow = hWnd;
  desc.BufferCount = 2;
  desc.BufferDesc.Width = 0;
  desc.BufferDesc.Height = 0;
  desc.BufferDesc.Format = ::DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.BufferDesc.RefreshRate.Numerator = 60;
  desc.BufferDesc.RefreshRate.Denominator = 1;
  desc.Flags = ::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Windowed = TRUE;
  desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  UINT createDeviceFlags = 0;
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  D3D_FEATURE_LEVEL featureLevel;
  const D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_11_0};
  if(::D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,createDeviceFlags,levels,1,D3D11_SDK_VERSION,&desc
        ,&m_swapChain,&m_device,&featureLevel,&m_context) != S_OK) {
    return inited;
  }
  this->createRenderTarget();
  if(initResource()) {
    inited = true;
  }
  return inited;
}

void DirectWorld::createRenderTarget() {
  ID3D11Texture2D * pBackBuffer;
  m_swapChain->GetBuffer(0,IID_PPV_ARGS(&pBackBuffer));
  m_device->CreateRenderTargetView(pBackBuffer,NULL,m_mainRenderTargetView.ReleaseAndGetAddressOf());
  pBackBuffer->Release();
}

DirectWorld::DirectWorld() {
  inited = false;
}
DirectWorld::~DirectWorld() {
}
ComPtr<ID3D11Device>  DirectWorld::getDevice(){
  return m_device;
}
ComPtr<ID3D11DeviceContext>  DirectWorld::getContext(){
  return m_context;
}
ComPtr<IDXGISwapChain>  DirectWorld::getSwapChain(){
  return m_swapChain;
}
ComPtr<ID3D11RenderTargetView>  DirectWorld::getMainRenderTargetView(){
  return m_mainRenderTargetView;
}


void DirectWorld::setViewport() {
      ID3D11Texture2D * backBuffer;
      m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
      D3D11_TEXTURE2D_DESC backBufferDesc;
      ZeroMemory(&backBufferDesc,sizeof(backBufferDesc));
      backBuffer->GetDesc(&backBufferDesc);
      D3D11_VIEWPORT viewport;
      viewport.TopLeftX = 0.0f;
      viewport.TopLeftY = 0.0f;
      viewport.Width = static_cast<float>(backBufferDesc.Width);
      viewport.Height = static_cast<float>(backBufferDesc.Height);
      viewport.MinDepth = D3D11_MIN_DEPTH;
      viewport.MaxDepth = D3D11_MAX_DEPTH;
      backBuffer->Release();
      m_context->RSSetViewports(1, &viewport);
}

HRESULT DirectWorld::compileD3DFile(const WCHAR * filename,LPCSTR entryPoint,LPCSTR profile,ID3DBlob ** outData) {
  UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
      flags |= D3DCOMPILE_DEBUG;
#endif
      // Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11-class hardware.
      ID3DBlob* shaderBlob = nullptr;
      ID3DBlob* errorBlob = nullptr;
      HRESULT hr = ::D3DCompileFromFile( filename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                       entryPoint, profile,
                                       flags, 0, &shaderBlob, &errorBlob );
      if(FAILED(hr)) {
          std::cout<<"failed"<<std::endl;
        if(shaderBlob) {
          shaderBlob->Release();
        }
        if(errorBlob){
          std::cout<<"failed err"<<std::endl;
          std::cout<<(char *)errorBlob->GetBufferPointer()<<std::endl;
          errorBlob->Release();
        }
        return hr;
      }
      *outData = shaderBlob;
      return hr;
}

bool DirectWorld::createVertexShaderFromFile(const WCHAR * filename,LPCSTR entryPoint,ID3D11VertexShader ** outShader) {
  ID3DBlob * blob = NULL;
  HRESULT hr = this->compileD3DFile(filename,entryPoint,"vs_5_0",&blob);
  if(FAILED(hr)) {
    std::cout<<"failed to compileD3DFile"<<std::endl;
    return false;
  }
  ::ID3D11VertexShader * vertexShader;
  m_device->CreateVertexShader(blob->GetBufferPointer(),blob->GetBufferSize(),NULL,&vertexShader);
  *outShader = vertexShader;
  blob ->Release();
  if(FAILED(hr)) {
    std::cout<<"failed to create layout\n";
    return false;
  }
  return true;
}

bool DirectWorld::createPixelShaderFromFile(const WCHAR * filename,LPCSTR entryPoint,ID3D11PixelShader ** outShader) {
  ID3DBlob * blob = NULL;
  HRESULT hr = this->compileD3DFile(filename,entryPoint,"ps_5_0",&blob);
  if(FAILED(hr)) {
    return false;
  }
  ::ID3D11PixelShader * pixelShader;
  m_device->CreatePixelShader(blob->GetBufferPointer(),blob->GetBufferSize(),NULL,&pixelShader);
  *outShader = pixelShader;
  blob->Release();
  return true;
}