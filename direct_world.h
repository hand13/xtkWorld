#pragma
#include "base_direct.h"
#include <Effects.h>
#include <memory>
#include <SimpleMath.h>
using DirectX::SimpleMath::Matrix;
struct ConstantBuffer {
    Matrix modelMatrix;
    Matrix viewMatrix;
    Matrix projectionMatrix;
};
class TheWorld:public DirectWorld {
    private:
    ComPtr<ID3D11ShaderResourceView> m_texture;
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;
    ComPtr<ID3D11InputLayout> layout;
    ComPtr<ID3D11Buffer> constantBuffer;
    ComPtr<ID3D11VertexShader> vs;
    ComPtr<ID3D11PixelShader> ps;
    ConstantBuffer constantBufferData;
    public: 
    TheWorld(){
    }
    virtual bool initResource() override;
    virtual void draw() override;
};