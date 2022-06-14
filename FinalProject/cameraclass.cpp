////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	m_DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	m_yaw = 0;
	m_pitch = 0;

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&m_position);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x * 0.0174532925f;
	yaw   = m_rotation.y * 0.0174532925f;
	roll  = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}

void CameraClass::Update()
{
	XMMATRIX rotationMatrix;

	rotationMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0);

	m_lookAt = XMVector3TransformCoord(m_DefaultForward, rotationMatrix);
	m_lookAt = XMVector3Normalize(m_lookAt);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(m_yaw);

	m_right = XMVector3TransformCoord(m_DefaultRight, RotateYTempMatrix);
	m_up = XMVector3TransformCoord(m_up, RotateYTempMatrix);
	m_forward = XMVector3TransformCoord(m_DefaultForward, RotateYTempMatrix);

	XMVECTOR position = XMLoadFloat3(&m_position);

	position += m_moveLeftRight * m_right;
	position += m_moveBackForward * m_forward;

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;

	m_lookAt = position + m_lookAt;

	m_viewMatrix = XMMatrixLookAtLH(position, m_lookAt, m_up);

	XMStoreFloat3(&m_position, position);
}

void CameraClass::Reset()
{
	m_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	m_yaw = 0.0f;
	m_pitch = 0.0f;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}