#pragma
#include "base_direct.h"
#include <Effects.h>
#include <memory>
#include <SimpleMath.h>
#include <CommonStates.h>
#include "obj.h"
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
struct ConstantBuffer {
    Matrix modelMatrix;
    Matrix viewMatrix;
    Matrix projectionMatrix;
};
class TheWorld:public DirectWorld {
    private:
    ComPtr<ID3D11ShaderResourceView> m_texture;
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11InputLayout> layout;
    std::unique_ptr<DirectX::BasicEffect> effect;
    std::unique_ptr<DirectX::CommonStates> commonStates;
    ConstantBuffer constantBufferData;
    ObjShape os;
    void updateView();
    public: 
    TheWorld(){
    }
    virtual bool initResource() override;
    virtual void draw() override;
    float length = 7.f;
    Vector3 angle = {0.f,0.f,0.f};
};