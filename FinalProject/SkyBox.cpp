#include "SkyBox.h"

#include "d3dclass.h"
#include "textureshaderclass.h"
#include "DDSTextureLoader.h"

SkyBox::SkyBox()
{
	XMMATRIX matScale;
	matScale = XMMatrixScaling(500.0f, 500.0f, 500.0f);

	m_matWorld = matScale * m_matWorld;
}


SkyBox::~SkyBox()
{
	Shutdown();
}

//bool SkyBox::Initialize(ID3D11Device* device)
//{
//	bool result;
//	m_lf = new ModelClass;
//	result = m_lf->InitializePlane(device, L"./data/SkyBox/left.dds", 100, 100);
//	if (!result) return false;
//
//	m_Model = new ModelClass;
//	result = m_Model->InitializePlane(device, L"./data/SkyBox/right.dds", 100, 100);
//	if (!result) return false;
//
//	m_up = new ModelClass;
//	result = m_up->InitializePlane(device, L"./data/SkyBox/top.dds", 100, 100);
//	if (!result) return false;
//
//	m_dn = new ModelClass;
//	result = m_dn->InitializePlane(device, L"./data/SkyBox/bottom.dds", 100, 100);
//	if (!result) return false;
//
//	m_ft = new ModelClass;
//	result = m_ft->InitializePlane(device, L"./data/SkyBox/front.dds", 100, 100);
//	if (!result) return false;
//
//	m_bk = new ModelClass;
//	result = m_bk->InitializePlane(device, L"./data/SkyBox/back.dds", 100, 100);
//	if (!result) return false;
//
//	return true;
//}
//
//bool SkyBox::Render(D3DClass* d3d, TextureShaderClass* textureShaderClass, XMMATRIX viewMatrix, XMMATRIX projMatrix, 
//	XMFLOAT3 camPos)
//{
//	bool result;
//	XMMATRIX matScale, matTrans, matRotX, matRotY, matRotZ;
//	matScale = XMMatrixScaling(10.f, 10.f, 10.f);
//
//	matTrans = XMMatrixTranslation(camPos.x + 493.6f, camPos.y + 493.6f, camPos.z - 493.6f);
//	matRotX = XMMatrixRotationX(float(XMConvertToRadians(90.f)));
//	matRotZ = XMMatrixRotationZ(float(XMConvertToRadians(90.f)));
//	m_matWorld = matScale * matRotZ * matRotX * matTrans;
//	m_Model->Render(d3d->GetDeviceContext());
//	result = textureShaderClass->Render(d3d->GetDeviceContext(), m_Model->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, 
//		m_Model->GetTexture());
//	if (!result) return false;
//
//	matTrans = XMMatrixTranslation(camPos.x - 493.6f, camPos.y + 493.6f, camPos.z + 493.6f);
//	matRotX = XMMatrixRotationX(float(XMConvertToRadians(90.f)));
//	matRotZ = XMMatrixRotationZ(float(XMConvertToRadians(-90.f)));
//	m_matWorld = matScale /** matRotY*/ * matRotZ * matRotX * matTrans;
//	m_lf->Render(d3d->GetDeviceContext());
//	textureShaderClass->Render(d3d->GetDeviceContext(), m_lf->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, 
//		m_lf->GetTexture());
//
//	//matRotX = XMMatrixRotationX(float(XMConvertToRadians(90.f)));
//	matRotZ = XMMatrixRotationZ(float(XMConvertToRadians(180.f)));
//	matRotY = XMMatrixRotationY(float(XMConvertToRadians(-90.f)));
//	matTrans = XMMatrixTranslation(camPos.x - 493.6f, camPos.y + 493.6f, camPos.z - 493.6f);
//	m_matWorld = matScale * matRotY * matRotZ * matTrans;
//	m_up->Render(d3d->GetDeviceContext());
//	textureShaderClass->Render(d3d->GetDeviceContext(), m_up->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, 
//		m_up->GetTexture());
//
//	matTrans = XMMatrixTranslation(camPos.x + 493.6f, camPos.y - 493.6f, camPos.z - 493.6f);
//	matRotY = XMMatrixRotationY(float(XMConvertToRadians(-90.f)));
//	m_matWorld = matScale * matRotY * matTrans;
//
//	m_dn->Render(d3d->GetDeviceContext());
//	textureShaderClass->Render(d3d->GetDeviceContext(), m_dn->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, 
//		m_dn->GetTexture());
//
//	//matRotY = XMMatrixRotationY(float(XMConvertToRadians(180.f)));
//	matTrans = XMMatrixTranslation(camPos.x+493.6f, camPos.y + 493.6f, camPos.z + 493.6f);
//	matRotY = XMMatrixRotationY(float(XMConvertToRadians(-90.f)));
//	matRotX = XMMatrixRotationX(float(XMConvertToRadians(-90.f)));
//	matRotZ = XMMatrixRotationZ(float(XMConvertToRadians(90.f)));
//	m_matWorld = matScale * matRotY * matRotX * matRotZ * matTrans;
//	m_ft->Render(d3d->GetDeviceContext());
//	textureShaderClass->Render(d3d->GetDeviceContext(), m_ft->GetIndexCount(), m_matWorld, viewMatrix, projMatrix, 
//		m_ft->GetTexture());
//
//	matRotX = XMMatrixRotationX(float(XMConvertToRadians(90.f)));
//	matTrans = XMMatrixTranslation(camPos.x - 493.6f, camPos.y + 493.6f, camPos.z - 493.6f);
//	m_matWorld = matScale * matRotX * matTrans;
//
//	m_bk->Render(d3d->GetDeviceContext());
//	textureShaderClass->Render(d3d->GetDeviceContext(), m_bk->GetIndexCount(), m_matWorld, viewMatrix, projMatrix,
//		m_bk->GetTexture());
//	return true;
//}

void SkyBox::Shutdown()
{
	if (m_lf)
	{
		m_lf->Shutdown();
		delete m_lf;
		m_lf = 0;
	}

	if (m_up)
	{
		m_up->Shutdown();
		delete m_up;
		m_up = 0;
	}

	if (m_dn)
	{
		m_dn->Shutdown();
		delete m_dn;
		m_dn = 0;
	}

	if (m_ft)
	{
		m_ft->Shutdown();
		delete m_ft;
		m_ft = 0;
	}

	if (m_bk)
	{
		m_bk->Shutdown();
		delete m_bk;
		m_bk = 0;
	}
}


bool SkyBox::Frame(float frameTime)
{
	return true;
}

bool SkyBox::Initialize(D3DClass* d3d)
{
	CreateSphere(d3d, 10, 10);
	CreateShader(d3d);
	return true;
}

void SkyBox::CleanUp()
{
	sphereIndexBuffer->Release();
	sphereVertBuffer->Release();

	SKYMAP_VS->Release();
	SKYMAP_PS->Release();
	SKYMAP_VS_Buffer->Release();
	SKYMAP_PS_Buffer->Release();

	smrv->Release();

	DSLessEqual->Release();
}

void SkyBox::CreateSphere(D3DClass* d3d, int LatLines, int LongLines)
{
	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i * LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i * LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i * LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	d3d->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1) * LongLines + j + 1;

			indices[k + 3] = (i + 1) * LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1) * LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i * LongLines) + LongLines;
		indices[k + 1] = (i * LongLines) + 1;
		indices[k + 2] = ((i + 1) * LongLines) + LongLines;

		indices[k + 3] = ((i + 1) * LongLines) + LongLines;
		indices[k + 4] = (i * LongLines) + 1;
		indices[k + 5] = ((i + 1) * LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	d3d->GetDevice()->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);
}

void SkyBox::CreateShader(D3DClass* d3d)
{
	HRESULT result;
	
	result = D3DCompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &SKYMAP_VS_Buffer, 0);
	result = D3DCompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &SKYMAP_PS_Buffer, 0);

	result = d3d->GetDevice()->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	result = d3d->GetDevice()->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	ID3D11Texture2D* SMTexture = 0;

	result = CreateDDSTextureFromFileEx(d3d->GetDevice(), L"./data/SkyBox/skybox.dds", 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE,
		0, D3D11_RESOURCE_MISC_TEXTURECUBE, false, (ID3D11Resource**)&SMTexture, nullptr);

	result = d3d->GetDevice()->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	result = d3d->GetDevice()->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
}

void SkyBox::Update(D3DClass* d3d, XMMATRIX view, XMMATRIX projection, XMVECTOR camPosition)
{
	XMMATRIX Scale, Translation;

	sphereWorld = XMMatrixIdentity();

	Scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);

	Translation = XMMatrixTranslation(XMVectorGetX(camPosition), XMVectorGetY(camPosition), XMVectorGetZ(camPosition));

	sphereWorld = Scale * Translation;


	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	d3d->GetDeviceContext()->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	d3d->GetDeviceContext()->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

	XMMATRIX WVP = sphereWorld * view * projection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(sphereWorld);
	d3d->GetDeviceContext()->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d->GetDeviceContext()->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	d3d->GetDeviceContext()->PSSetShaderResources(0, 1, &smrv);
	//d3d->GetDeviceContext()->PSSetSamplers(0, 1, &CubesTexSamplerState);

	d3d->GetDeviceContext()->VSSetShader(SKYMAP_VS, 0, 0);
	d3d->GetDeviceContext()->PSSetShader(SKYMAP_PS, 0, 0);
	// Set the new depth/stencil and RS states
	DSLessEqual = d3d->GetDSLessEqual();
	d3d->GetDeviceContext()->OMSetDepthStencilState(DSLessEqual, 0);
	d3d->GetDeviceContext()->RSSetState(d3d->GetRSCullNone());
	d3d->GetDeviceContext()->DrawIndexed(NumSphereFaces * 3, 0, 0);

	d3d->GetDeviceContext()->RSSetState(d3d->GetRasterState());
	// Set the default VS shader and depth/stencil state
	//d3d->GetDeviceContext()->VSSetShader(VS, 0, 0);
	//d3d->GetDeviceContext()->OMSetDepthStencilState(NULL, 0);
}