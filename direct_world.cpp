#include "direct_world.h"
#include <WICTextureLoader.h>
#include <DirectXColors.h>
bool TheWorld::initResource() {
    DirectX::CreateWICTextureFromFile(getDevice().Get(),L"e:\\test.jpg",nullptr,
    m_texture.ReleaseAndGetAddressOf());
    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(getContext().Get());
    ComPtr<ID3D11Resource> resource;
    m_texture->GetResource(resource.GetAddressOf());
    ComPtr<ID3D11Texture2D> cat;
    resource.As(&cat);
    D3D11_TEXTURE2D_DESC desc;
    cat->GetDesc(&desc);
    m_origin.x = float(desc.Width/2);
    m_origin.y = float(desc.Height/2);
    m_screenPos.x = 0.f;
    m_screenPos.y = 0.f;
    m_states = std::make_unique<DirectX::CommonStates>(getDevice().Get());
    return true;
}
void TheWorld::draw() {
    getContext()->OMSetRenderTargets(1,getMainRenderTargetView().GetAddressOf(),nullptr);
    this->setViewport();
    m_spriteBatch->Begin();
    m_spriteBatch->Draw(m_texture.Get(),m_screenPos,nullptr,DirectX::Colors::White,0.f,m_origin);
    m_spriteBatch->End();
    getSwapChain()->Present(1,0);
}