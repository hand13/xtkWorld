#include "direct_world.h"
#include <WICTextureLoader.h>
#include <DirectXColors.h>
const WCHAR * VS_PATH=L"e:\\vs.hlsl";
const WCHAR * PS_PATH=L"e:\\ps.hlsl";
bool TheWorld::initResource() {
    DirectX::CreateWICTextureFromFile(getDevice().Get(),L"e:\\test.jpg",nullptr,
    m_texture.ReleaseAndGetAddressOf());
    std::vector<ObjShape> oss = loadFromFile("e:\\cube.obj");
    if(oss.size() <= 0) {
        return false;
    }
    os = oss[0];

    float vertices[] = {
        -0.5,-0.5,0.0    ,0.0,1.0,
        0.5,-0.5,0.0     ,1.0,1.0,
        0.5,0.5,0.0     ,1.0,0.0,
        -0.5,0.5,0.0     ,0.0,0.0
    };
    unsigned short indices[] = 
    {0,3,2,2,1,0};
    // unsigned short indices[] = 
    // {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
    //  17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,34,35};
    D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory(&vertexDesc,sizeof(vertexDesc));
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(float) * ARRAYSIZE(vertices);
    // vertexDesc.ByteWidth = sizeof(float) * os.vertices.size();
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.CPUAccessFlags = 0;
    vertexDesc.StructureByteStride = 0;
    vertexDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vertexBufferData;
    vertexBufferData.pSysMem = vertices;
    // vertexBufferData.pSysMem = os.vertices.data();
    vertexBufferData.SysMemPitch= 0;
    vertexBufferData.SysMemSlicePitch= 0;

    if(FAILED(getDevice()->CreateBuffer(&vertexDesc,&vertexBufferData,vertexBuffer.GetAddressOf()))){
        return false;
    }

    D3D11_BUFFER_DESC indexDesc;
    ZeroMemory(&indexDesc,sizeof(indexDesc));
    indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexDesc.ByteWidth = sizeof(float) * ARRAYSIZE(indices);
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    indexDesc.CPUAccessFlags = 0;
    vertexDesc.StructureByteStride = 0;
    vertexDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA indexBufferData;
    indexBufferData.pSysMem = indices;
    indexBufferData.SysMemPitch= 0;
    indexBufferData.SysMemSlicePitch= 0;
    if(FAILED(getDevice()->CreateBuffer(&indexDesc,&indexBufferData,indexBuffer.GetAddressOf()))){
        return false;
    }
    D3D11_BUFFER_DESC constantDesc;
    constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantDesc.ByteWidth = sizeof(ConstantBuffer);
    constantDesc.Usage = D3D11_USAGE_DEFAULT;
    constantDesc.StructureByteStride = 0;
    constantDesc.MiscFlags = 0;
    constantDesc.CPUAccessFlags = 0;
    if(FAILED(getDevice()->CreateBuffer(&constantDesc,nullptr,constantBuffer.GetAddressOf()))) {
        return false;
    }
    if(!this->createVertexShaderFromFile(VS_PATH,"main",vs.GetAddressOf())) {
        return false;
    }
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
    };
    if(!this->createInputLayout(VS_PATH,"main",layoutDesc
    ,ARRAYSIZE(layoutDesc),layout.GetAddressOf())) {
        return false;
    }
    if(!this->createPixelShaderFromFile(PS_PATH,"main",ps.GetAddressOf())) {
        return false;
    }
    return true;
}
void TheWorld::draw() {
    UINT stride = 4 * 5;
    // UINT stride = 36 * 5;
    UINT offset = 0;
    getContext()->OMSetRenderTargets(1,getMainRenderTargetView().GetAddressOf(),nullptr);
    const float clearColor[] = {0.0,0.0,0.0,1.0};
    getContext()->ClearRenderTargetView(getMainRenderTargetView().Get(),clearColor);
    this->setViewport();
    getContext()->IASetVertexBuffers(0,1,vertexBuffer.GetAddressOf(),&stride,&offset);
    getContext()->IASetIndexBuffer(indexBuffer.Get(),DXGI_FORMAT_R16_UINT,0);
    getContext()->IASetInputLayout(layout.Get());
    getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    getContext()->VSSetShader(vs.Get(),nullptr,0);
    constantBufferData.modelMatrix = Matrix::CreateRotationZ(3.14/60) * constantBufferData.modelMatrix;
    getContext()->UpdateSubresource(constantBuffer.Get(),0,0,&constantBufferData,0,0);
    getContext()->VSSetConstantBuffers(0,1,constantBuffer.GetAddressOf());
    getContext()->PSSetShader(ps.Get(),nullptr,0);
    getContext()->DrawIndexed(6,0,0);
    // getContext()->DrawIndexed(36,0,0);
    getSwapChain()->Present(1,0);
}
