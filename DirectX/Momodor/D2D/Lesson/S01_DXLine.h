#pragma once
////////////////////////////////////////////////////////
// Direct에서의 Line Rendering Class
// Input Assembly->Vertex Shader->Pixel Shader->RasterS->OutMerger
////////////////////////////////////////////////////////
class S01_DXLine : public Scene
{
public:
	// 정점에 관한 정의 , DX에서 별도로 제공하지 않음
	// 개발자가 정의 후에 Device에 Loading한다
	struct Vertex   
	{
		D3DXVECTOR3 Poisition;    // f f f f(x)
		D3DXCOLOR   Color;
	};


public:
	void  Update() override;       
	void  Render() override;      
private:
	void  CreateVertexBuffer();  // IA 정점을 생성 
	void  CreateShader();        // Vertex Shader, Pixel Shader --> 계산방법
	void  CreateInputLayout();   // 계산한 방법-->렌더링

private:  
	Vertex               vertices[10];                 // 정점 데이터
	ID3D11Buffer         *m_pVertextBuffer = nullptr;  // vertices를 넣을 정점 Buffer
	ID3D11VertexShader   *m_pVertexShader  = nullptr;  // Vertex Stage
	ID3D11PixelShader    *m_pPixelShader = nullptr;
	ID3D10Blob           *m_pVsBlob = nullptr;
	ID3D10Blob           *m_pPsBlob = nullptr;
	ID3D11InputLayout    *m_pInputLayout = nullptr;
public:
	S01_DXLine();
	~S01_DXLine();
};

