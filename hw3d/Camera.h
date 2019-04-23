#pragma once
#include <DirectXMath.h>


class Camera
{
public:
	Camera();

	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const DirectX::XMMATRIX& GetViewMatrix() const { return view; }
	const DirectX::XMMATRIX& GetProjectionMatrix() const { return projection; }

	const DirectX::XMVECTOR& GetPositionVector() const { return posVector; }
	const DirectX::XMFLOAT3& GetPositionFloat3() const { return pos; }
	const DirectX::XMVECTOR& GetRotationVector() const { return rotVector; }
	const DirectX::XMFLOAT3& GetRotationFloat3() const { return rot; }

	void SetPosition(const DirectX::XMVECTOR& newPos);
	void SetPosition(float x, float y, float z);

	void AdjustPosition(const DirectX::XMVECTOR dPos);
	void AdjustPosition(float dx, float dy, float dz);

	void SetRotation(const DirectX::XMVECTOR& newRot);
	void SetRotation(float pitch, float yaw, float roll);

	void AdjustRotation(const DirectX::XMVECTOR& dRot);
	void AdjustRotation(float dPitch, float dYaw, float dRoll);
	void SetLookAtPos(DirectX::XMFLOAT3 lookAtPos);

	const DirectX::XMVECTOR& GetUpwardVector() const { return DEFAULT_UP_VECTOR; }
	const DirectX::XMVECTOR& GetDownwardVector() const { return DEFAULT_DOWN_VECTOR; }
	const DirectX::XMVECTOR& GetForwardVector() { return vecForward; }
	const DirectX::XMVECTOR& GetLeftVector() { return vecLeft; }
	const DirectX::XMVECTOR& GetRightVector() { return vecRight; }
	const DirectX::XMVECTOR& GetBackwardVector() { return vecBackward; }

	const float GetTranslationSpeed() { return translationSpeed; }

private:
	
	void UpdateViewMatrix();

	DirectX::XMVECTOR posVector;
	DirectX::XMVECTOR rotVector;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;

	const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_DOWN_VECTOR = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR vecForward;
	DirectX::XMVECTOR vecLeft;
	DirectX::XMVECTOR vecRight;
	DirectX::XMVECTOR vecBackward;

	float translationSpeed;
};