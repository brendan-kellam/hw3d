#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"

Box::Box( Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist )
	:
	r( rdist( rng ) ),
	droll( ddist( rng ) ),
	dpitch( ddist( rng ) ),
	dyaw( ddist( rng ) ),
	dphi( odist( rng ) ),
	dtheta( odist( rng ) ),
	dchi( odist( rng ) ),
	chi( adist( rng ) ),
	theta( adist( rng ) ),
	phi( adist( rng ) )
{
	if (!DrawableBase::IsStaticInitialized()) {
		struct Vertex
		{
			struct
			{
				float x;
				float y;
				float z;
				float u;
				float v;
			} pos;
		};
		const std::vector<Vertex> vertices =
		{
			{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f},
			{ 1.0f,-1.0f,-1.0f, 1.0f, 1.0f},
			{ -1.0f,1.0f,-1.0f, 0.0f, 0.0f},
			{ 1.0f,1.0f,-1.0f, 1.0f, 0.0f},
			{ -1.0f,-1.0f,1.0f, 0.0f, 1.0f},
			{ 1.0f,-1.0f,1.0f, 1.0f, 1.0f},
			{ -1.0f,1.0f,1.0f, 0.0f, 0.0f},
			{ 1.0f,1.0f,1.0f, 1.0f, 0.0f},
		};

		DrawableBase::AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));


		auto pvs = std::make_unique<VertexShader>(gfx, L"TexVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		DrawableBase::AddStaticBind(std::move(pvs));

		DrawableBase::AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexPixelShader.cso"));

		const std::vector<unsigned short> indices =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		// 	const ConstantBuffer2 cb2 =
		// 	{
		// 		{
		// 			{ 1.0f,0.0f,1.0f },
		// 			{ 1.0f,0.0f,0.0f },
		// 			{ 0.0f,1.0f,0.0f },
		// 			{ 0.0f,0.0f,1.0f },
		// 			{ 1.0f,1.0f,0.0f },
		// 			{ 0.0f,1.0f,1.0f },
		// 		}
		// 	};
		// 	AddBind( std::make_unique<PixelConstantBuffer<ConstantBuffer2>>( gfx,cb2 ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		DrawableBase::AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		DrawableBase::AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		DrawableBase::AddStaticBind(std::make_unique<Texture>(gfx, L"..\\Data\\Textures\\galaxy.jpg"));
	}
	else {
		SetIndexFromStatic();
	}
	
	// Non static binds (unique to a given box)
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

}

void Box::Update( float dt ) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
				
	//return DirectX::XMMatrixIdentity();
}
