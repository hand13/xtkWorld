#include "device.h"
#include <iostream>
#include <d3dcompiler.h>
#include "image.h"
bool DirectWorld::init(HWND hWnd) {
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
    return false;
  }
  if(!this->initResource()){
      return false;
  }
  this->createRenderTarget();
  inited = true;
  return true;
}


void DirectWorld::cleanupRenderTarget() {
  if(m_mainRenderTargetView ) {
    m_mainRenderTargetView->Release();
    m_mainRenderTargetView = NULL;
  }
}

void DirectWorld::cleanupDevice() {
  cleanupRenderTarget();
  if(m_swapChain) {
    m_swapChain->Release();
    m_swapChain = NULL;
  }
  if(m_context) {
    m_context->Release();
    m_context = NULL;
  }
  if(m_device) {
    m_device->Release();
    m_device = NULL;
  }
}

void DirectWorld::createRenderTarget() {
  ID3D11Texture2D * pBackBuffer;
  m_swapChain->GetBuffer(0,IID_PPV_ARGS(&pBackBuffer));
  m_device->CreateRenderTargetView(pBackBuffer,NULL,&m_mainRenderTargetView);
  pBackBuffer->Release();
}

DirectWorld::DirectWorld() {
  m_device = NULL;
  m_context = NULL;
  m_swapChain = NULL;
  m_mainRenderTargetView = NULL;
  m_indexBuffer = NULL; 
  m_vertexBuffer = NULL;
  m_inputLayout = NULL;
  m_pixelShader = NULL;
  m_vertexShader = NULL;
  inited = false;
}
DirectWorld::~DirectWorld() {
  this->cleanupDevice();
}
ID3D11Device * DirectWorld::getDevice(){
  return m_device;
}
ID3D11DeviceContext * DirectWorld::getContext(){
  return m_context;
}
IDXGISwapChain * DirectWorld::getSwapChain(){
  return m_swapChain;
}
ID3D11RenderTargetView * DirectWorld::getMainRenderTargetView(){
  return m_mainRenderTargetView;
}


void DirectWorld::draw() {
  if(inited) {
 m_context->OMSetRenderTargets(
                1,
                &m_mainRenderTargetView,
                nullptr // Use no depth stencil.
                );
 const float clearColor[4] = { 0.5f, 0.0f, 0.5f, 1.0f };
 m_context->ClearRenderTargetView(
                m_mainRenderTargetView,
                clearColor
                );
  this->setViewport();
 m_context->IASetInputLayout(m_inputLayout);
 UINT stride = sizeof(float) * 5;
 UINT offset = 0;
 m_context->IASetVertexBuffers(0,1,&m_vertexBuffer,&stride,&offset);
 m_context->IASetIndexBuffer(m_indexBuffer,DXGI_FORMAT_R16_UINT,0);
 m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 m_context->VSSetShader(m_vertexShader,NULL,0);
 m_context->PSSetShader(m_pixelShader,NULL,0);
 m_context->PSSetSamplers(0,1,&m_samplerState);
 if(m_texture) {
   m_context->PSSetShaderResources(0,1,&m_texture);
 }
 m_context->DrawIndexed(6,0,0);
 m_swapChain->Present(1, 0);
  }
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

   const D3D11_INPUT_ELEMENT_DESC basicVertexLayoutDesc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
            };
  hr= m_device->CreateInputLayout(
      basicVertexLayoutDesc,
      ARRAYSIZE(basicVertexLayoutDesc),
      blob->GetBufferPointer(),
      blob->GetBufferSize(),
      &m_inputLayout
      );
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

bool DirectWorld::createVertexBuffer() {
 float triangleVertices[] =
          {
            -0.5f, -0.5f,0.5    ,0.0f,0.0f,
            0.5f,  -0.5f,0.5    ,1.0f,0.0f,
             0.5f, 0.5f,0.5     ,1.0f,1.0f,
            -0.5f, 0.5f,0.5     ,0.0f,1.0f,
          };

          unsigned short triangleIndices[] =
          {
              0,3,2,0,2,1
          };

          D3D11_BUFFER_DESC vertexBufferDesc;
          ZeroMemory(&vertexBufferDesc,sizeof(vertexBufferDesc));
          vertexBufferDesc.ByteWidth = sizeof(float) * ARRAYSIZE(triangleVertices);
          vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
          vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
          vertexBufferDesc.CPUAccessFlags = 0;
          vertexBufferDesc.MiscFlags = 0;
          vertexBufferDesc.StructureByteStride = 0;

          D3D11_SUBRESOURCE_DATA vertexBufferData;
          vertexBufferData.pSysMem = triangleVertices;
          vertexBufferData.SysMemPitch = 0;
          vertexBufferData.SysMemSlicePitch = 0;

              m_device->CreateBuffer(
                  &vertexBufferDesc,
                  &vertexBufferData,
                  &m_vertexBuffer
                  );
          D3D11_BUFFER_DESC indexBufferDesc;
          indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(triangleIndices);
          indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
          indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
          indexBufferDesc.CPUAccessFlags = 0;
          indexBufferDesc.MiscFlags = 0;
          indexBufferDesc.StructureByteStride = 0;

          D3D11_SUBRESOURCE_DATA indexBufferData;
          indexBufferData.pSysMem = triangleIndices;
          indexBufferData.SysMemPitch = 0;
          indexBufferData.SysMemSlicePitch = 0;

              m_device->CreateBuffer(
                  &indexBufferDesc,
                  &indexBufferData,
                  &m_indexBuffer
                  );
          return true;
}
bool DirectWorld::initResource() {
  if(!this->createVertexShaderFromFile(L"e:\\vertex.hlsl","main",&m_vertexShader)) {
    std::cout<<"vertex failed"<<std::endl;
    return false;
  }
  if(!this->createPixelShaderFromFile(L"e:\\pixel.hlsl","main",&m_pixelShader)) {
    std::cout<<"pixel failed"<<std::endl;
    return false;
  }
  if(!this->createVertexBuffer()) {
    std::cout<<"buffer failed"<<std::endl;
    return false;
  }
  if(!this->createSampler()) {
    std::cout<<"create sample failed"<<std::endl;
    return false;
  }

  int my_image_width;
  int my_image_height;
  bool ret = LoadTextureFromFile("e:/test.jpg",m_device, &m_texture, &my_image_width, &my_image_height);
  if(!ret) {
    std::cout<<"failed to load image"<<std::endl;
    return false;
  }
  return true;
}
bool DirectWorld::createSampler() {
D3D11_SAMPLER_DESC samplerDesc;
ZeroMemory(&samplerDesc, sizeof(samplerDesc));
samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
samplerDesc.MaxAnisotropy = 0;
samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
samplerDesc.MipLODBias = 0.0f;
samplerDesc.MinLOD = 0;
samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
samplerDesc.BorderColor[0] = 0.0f;
samplerDesc.BorderColor[1] = 0.0f;
samplerDesc.BorderColor[2] = 0.0f;
samplerDesc.BorderColor[3] = 0.0f;
    m_device->CreateSamplerState(
        &samplerDesc,
        &m_samplerState);
    return true;
}
