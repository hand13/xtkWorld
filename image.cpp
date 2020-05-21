#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
bool LoadTextureFromFile(const char * filename,::ID3D11Device * pd3dDevice,ID3D11ShaderResourceView ** out_srv,int * out_width,int * out_height) {
  int img_width = 0,img_height = 0;
  unsigned char * image_data = stbi_load(filename,&img_width,&img_height,NULL,4);
  if(image_data == NULL) {
    return false;
  }
  D3D11_TEXTURE2D_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Width = img_width;
  desc.Height = img_height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;

  ID3D11Texture2D *pTexture = NULL;
  D3D11_SUBRESOURCE_DATA subResource;
  subResource.pSysMem = image_data;
  subResource.SysMemPitch = desc.Width * 4;
  subResource.SysMemSlicePitch = 0;
  pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

  // Create texture view
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = desc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;
  pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
  pTexture->Release();

  *out_width = img_width;
  *out_height = img_height;
  stbi_image_free(image_data);

    return true;
}
