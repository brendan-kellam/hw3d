#include "Camera.h"

Camera::Camera()
	: translationSpeed(10.0f)
{
	pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	posVector = XMLoadFloat3(&pos);
	rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix(); 
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadian = DirectX::XMConvertToRadians(fovDegrees);
	this->projection = DirectX::XMMatrixPerspectiveLH(fovRadian, aspectRatio, nearZ, farZ);
}

void Camera::SetPosition(const DirectX::XMVECTOR& newPos)
{
	XMStoreFloat3(&pos, newPos);
	posVector = newPos;
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	pos = DirectX::XMFLOAT3(x, y, z);
	posVector = XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const DirectX::XMVECTOR dPos)
{
	SetPosition(DirectX::XMVectorAdd(posVector, dPos));
}

void Camera::AdjustPosition(float dx, float dy, float dz)
{
	SetPosition(pos.x + dx, pos.y + dy, pos.z + dz);
}

void Camera::SetRotation(const DirectX::XMVECTOR& newRot)
{
	rotVector = newRot;
	XMStoreFloat3(&rot, rotVector);
	UpdateViewMatrix();
}

void Camera::SetRotation(float pitch, float yaw, float roll) {
	rot = DirectX::XMFLOAT3(pitch, yaw, roll);
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const DirectX::XMVECTOR& dRot)
{
	SetRotation(DirectX::XMVectorAdd(rotVector, dRot));
}

void Camera::AdjustRotation(float dPitch, float dYaw, float dRoll)
{
	SetRotation(rot.x + dPitch, rot.y + dYaw, rot.z + dRoll); 
}

void Camera::SetLookAtPos(DirectX::XMFLOAT3 lookAtPos)
{
	if (lookAtPos.x == pos.x && lookAtPos.y == pos.y && lookAtPos.z == pos.z) {
		return;
	}
	
	lookAtPos.x = pos.x - lookAtPos.x;
	lookAtPos.y = pos.y - lookAtPos.y;
	lookAtPos.z = pos.z - lookAtPos.z;

	float pitch = 0.0f, yaw = 0.0f;
	
	if (lookAtPos.y != 0.0f) {
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	if (lookAtPos.x != 0.0f) {
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0) {
		yaw += DirectX::XM_PI;
	}


	SetRotation(pitch, yaw, 0.0f);

}

void Camera::UpdateViewMatrix() {
	// Calculate cam rotation matrix
	DirectX::XMMATRIX camRot = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	// Calculate unit vector of cam target based off cam forward value transformed by cam rotation
	DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRot);
	// Adjust cam target to be offset by the camera's current position
	camTarget = DirectX::XMVectorAdd(camTarget, posVector);
	// Calculate up direction based on current rotation
	DirectX::XMVECTOR upDir = DirectX::XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRot);
	// Rebuild view matrix
	view = DirectX::XMMatrixLookAtLH(posVector, camTarget, upDir);

	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
	vecForward = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	vecBackward = DirectX::XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	vecLeft = DirectX::XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	vecRight = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

}
