#pragma once
////////////////////////////////////////////////////////
// Direct에서의 Line Rendering Class
// Input Assembly->Vertex Shader->Pixel Shader->RasterS->OutMerger
////////////////////////////////////////////////////////
class S03_DXRectangle : public Scene
{
public:
	struct Vertex       // 정점 선언, DX11에서 별도로 선언X, 개발자
	{
		Vector3 Position;
		Color   Color;
	};

public:
	void  Update() override;       
	void  Render() override;      

private:  
	void  CreateVertexBuffer();  // m_pVertexBuffer 생성
	void  CreateShader();        // VS,PS, Blob
	void  CreateInputLayout();
	Vertex  vertices[10];

private:
	ID3D11Buffer         *m_pVertexBuffer = nullptr;  // 정점Buffer
	ID3D11VertexShader   *m_pVertexShader = nullptr;  // Vertex Stage
	ID3D11PixelShader    *m_pPixelShader = nullptr;   // Pixel Stage
	ID3D10Blob           *m_pVsBlob = nullptr;        // HLSl --> Blob
	ID3D10Blob           *m_pPsBlob = nullptr;        // HLSl --> Blob
	ID3D11InputLayout    *m_pInputLayout = nullptr;


public:
	S03_DXRectangle();
	~S03_DXRectangle();
};

