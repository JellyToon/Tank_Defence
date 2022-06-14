#pragma once

#include "FontClass.h"
#include "FontShaderClass.h"

#include "AlignedAllocationPolicy.h"


class TextClass : public AlignedAllocationPolicy<16>
{
private:
	struct SentenceType
	{
		ID3D11Buffer* vertexBuffer, * indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, XMMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX);
	//bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetPolyCount(int, ID3D11DeviceContext*);
	bool SetObjCount(int, ID3D11DeviceContext*);
	bool SetScreenSize(int, int, ID3D11DeviceContext*);
	bool SetKey(int, ID3D11DeviceContext*);
	
	bool SetHP(int, ID3D11DeviceContext*);
	bool SetTimer(int, ID3D11DeviceContext*);

	bool SetVictory(ID3D11DeviceContext*);


private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, XMMATRIX, XMMATRIX);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	XMMATRIX m_baseViewMatrix;

	SentenceType* m_sentence1; // FPS
	SentenceType* m_sentence2; // CPU
	SentenceType* m_sentence3; // Poly
	SentenceType* m_sentence4; // Obj
	SentenceType* m_sentence5; // ScreenX
	SentenceType* m_sentence6; // ScreenY
	SentenceType* m_sentence7; // HP
	SentenceType* m_sentence8; // Timer
};

