#pragma once
////////////////////////////////////////////////////////
// Direct������ Line Rendering Class
// Input Assembly->Vertex Shader->Pixel Shader->RasterS->OutMerger
////////////////////////////////////////////////////////
class S04_DXCircle : public Scene
{
public:
	struct Vertex       // ���� ����, DX11���� ������ ����X, ������
	{
		Vector3 Position;
		Color   Color;
	};

public:
	void  Update() override;       
	void  Render() override;      

private:  
	void  CreateVertexBuffer();  // m_pVertexBuffer ����
	void  CreateShader();        // VS,PS, Blob
	void  CreateInputLayout();
	Vertex  vertices[361*2];
	int   m_nVertexCount = 0;
	float m_Time = 0.0f;

private:
	ID3D11Buffer         *m_pVertexBuffer = nullptr;  // ����Buffer
	ID3D11VertexShader   *m_pVertexShader = nullptr;  // Vertex Stage
	ID3D11PixelShader    *m_pPixelShader = nullptr;   // Pixel Stage
	ID3D10Blob           *m_pVsBlob = nullptr;        // HLSl --> Blob
	ID3D10Blob           *m_pPsBlob = nullptr;        // HLSl --> Blob
	ID3D11InputLayout    *m_pInputLayout = nullptr;


public:
	S04_DXCircle();
	~S04_DXCircle();
};

