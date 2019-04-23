#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const std::wstring& path);
	void Bind( Graphics& gfx ) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;
};