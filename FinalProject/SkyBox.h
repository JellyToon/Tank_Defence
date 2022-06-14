#pragma once

#include <iostream>
#include <vector>
#include <d3d11.h>
#include <directxmath.h>

#include "modelclass.h"
#include "AlignedAllocationPolicy.h"

class D3DClass;
class TextureShaderClass;
class DDSTextureLoader;

class SkyBox : public AlignedAllocationPolicy<16>
{
public:
	struct cbPerObject
	{
		XMMATRIX WVP;
		XMMATRIX World;
	};

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v)
			: pos(x, y, z), tex(u, v) {}

		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

	cbPerObject cbPerObj;

public:
	SkyBox();
	~SkyBox();

public:
	bool Initialize(ID3D11Device*);
	bool Render(D3DClass*, TextureShaderClass*, XMMATRIX, XMMATRIX, XMFLOAT3);
	void Shutdown();

	bool Initialize(D3DClass*);
	void CreateSphere(D3DClass*, int, int);
	void CreateShader(D3DClass*);
	void CleanUp();
	void Update(D3DClass*, XMMATRIX, XMMATRIX, XMVECTOR);

private:
	virtual bool Frame(float);

private:
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;
	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;
	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;

	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;

	ID3D11Buffer* cbPerObjectBuffer;

	XMMATRIX  Rotationx;
	XMMATRIX  Rotationy;
	XMMATRIX  Rotationz;

	ModelClass* m_lf; //left
	ModelClass* m_up; //up
	ModelClass* m_dn; //dn
	ModelClass* m_ft; //front
	ModelClass* m_bk; //back
	ModelClass* m_Model; //right

	XMMATRIX  m_matWorld;
};

