#pragma
#include "base_direct.h"
#include <Effects.h>
#include <memory>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <CommonStates.h>
class TheWorld:public DirectWorld {
    private:
    ComPtr<ID3D11ShaderResourceView> m_texture;
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    std::unique_ptr<DirectX::CommonStates> m_states;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;
    public: 
    TheWorld(){
    }
    virtual bool initResource();
    virtual void draw();
};