#include "direct_world.h"
#include <WICTextureLoader.h>
#include <DirectXColors.h>
using DirectX::SimpleMath::Vector3;
bool TheWorld::initResource() {
    if(FAILED(DirectX::CreateWICTextureFromFile(getDevice().Get(),L"e:\\test.jpg",nullptr,
    m_texture.ReleaseAndGetAddressOf()))) {
        return false;
    }
    constantBufferData.modelMatrix = Matrix::Identity;
    constantBufferData.viewMatrix = Matrix::CreateLookAt(Vector3(7.f,0.f,0.f)
    ,Vector3::Zero,Vector3::UnitZ);
    constantBufferData.projectionMatrix = Matrix::CreatePerspectiveFieldOfView(70.f/180.f,
    1,0.1f,20.f);
    effect = std::make_unique<DirectX::BasicEffect>(getDevice().Get());
    effect->SetWorld(constantBufferData.modelMatrix);
    effect->SetView(constantBufferData.viewMatrix);
    effect->SetProjection(constantBufferData.projectionMatrix);
    effect->SetTexture(m_texture.Get());
    effect->SetTextureEnabled(true);
    effect->EnableDefaultLighting();
    std::vector<ObjShape> oss = loadFromFile("cube.obj");
    if(oss.size() <= 0) {
        return false;
    }
    os = oss[0];

    D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory(&vertexDesc,sizeof(vertexDesc));
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(float) * os.vertices.size();
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.CPUAccessFlags = 0;
    vertexDesc.StructureByteStride = 0;
    vertexDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vertexBufferData;
    vertexBufferData.pSysMem = os.vertices.data();
    vertexBufferData.SysMemPitch= 0;
    vertexBufferData.SysMemSlicePitch= 0;

    if(FAILED(getDevice()->CreateBuffer(&vertexDesc,&vertexBufferData,vertexBuffer.GetAddressOf()))){
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        {"SV_POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0},
    };
    void const * shaderByteCode;
    size_t byteCodeLength;
    effect->GetVertexShaderBytecode(&shaderByteCode,&byteCodeLength);
    if(FAILED(getDevice()->CreateInputLayout(layoutDesc,ARRAYSIZE(layoutDesc),shaderByteCode,byteCodeLength,layout.GetAddressOf()))) {
        return false;
    }
    return true;
}
void TheWorld::draw() {
    UINT size = 4 + 3 + 2;
    UINT stride = sizeof(float) * size;
    UINT offset = 0;
    getContext()->OMSetRenderTargets(1,getMainRenderTargetView().GetAddressOf(),nullptr);
    setViewport();
    const float clearColor[] = {0.0,0.0,0.0,1.0};
    getContext()->ClearRenderTargetView(getMainRenderTargetView().Get(),clearColor);
    // constantBufferData.modelMatrix = Matrix::CreateRotationX(0.1) * constantBufferData.modelMatrix;
    // effect->SetWorld(constantBufferData.modelMatrix);
    effect->Apply(getContext().Get());
    getContext()->IASetVertexBuffers(0,1,vertexBuffer.GetAddressOf(),&stride,&offset);
    getContext()->IASetInputLayout(layout.Get());
    getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    getContext()->Draw(os.vertices.size()/size,0);
    getSwapChain()->Present(1,0);
}
