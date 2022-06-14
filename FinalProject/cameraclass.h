////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass : public AlignedAllocationPolicy<16>
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void Update();
	void GetViewMatrix(XMMATRIX&);

	void Reset();

	float GetCamYaw() { return m_yaw; }
	float GetCamPitch() { return m_pitch; }

	void SetMoveLeftRight(float LR) { m_moveLeftRight = LR; }
	void SetMoveBackForward(float BF) { m_moveBackForward = BF; }
	void SetCamYaw(float yaw) { m_yaw = yaw; }
	void SetCamPitch(float pitch) { m_pitch = pitch; }

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;

	XMVECTOR m_lookAt;

	XMVECTOR m_up;
	XMVECTOR m_right;
	XMVECTOR m_forward;

	XMVECTOR m_DefaultRight;
	XMVECTOR m_DefaultForward;

	float m_yaw;
	float m_pitch;

	float m_moveLeftRight;
	float m_moveBackForward;
};

#endif