#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#include <fstream>
using namespace std;

#include "textureclass.h"

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, const char*, const WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, const char*, float, float);

private:
	bool LoadFontData(const char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};

