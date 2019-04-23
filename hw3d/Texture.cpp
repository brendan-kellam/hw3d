#include "Texture.h"
#include "GraphicsThrowMacros.h"
#include <WICTextureLoader.h>

Texture::Texture(Graphics& gfx, const std::wstring& path)
{
	INFOMAN(gfx);

	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&sd, &pSamplerState));
	GFX_THROW_INFO(DirectX::CreateWICTextureFromFile(GetDevice(gfx), path.c_str(), nullptr, pTexture.GetAddressOf()));
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0, 1, pTexture.GetAddressOf());
	GetContext(gfx)->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
}
