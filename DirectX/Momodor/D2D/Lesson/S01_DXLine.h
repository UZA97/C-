#pragma once
////////////////////////////////////////////////////////
// Direct������ Line Rendering Class
// Input Assembly->Vertex Shader->Pixel Shader->RasterS->OutMerger
////////////////////////////////////////////////////////
class S01_DXLine : public Scene
{
public:
	// ������ ���� ���� , DX���� ������ �������� ����
	// �����ڰ� ���� �Ŀ� Device�� Loading�Ѵ�
	struct Vertex   
	{
		D3DXVECTOR3 Poisition;    // f f f f(x)
		D3DXCOLOR   Color;
	};


public:
	void  Update() override;       
	void  Render() override;      
private:
	void  CreateVertexBuffer();  // IA ������ ���� 
	void  CreateShader();        // Vertex Shader, Pixel Shader --> �����
	void  CreateInputLayout();   // ����� ���-->������

private:  
	Vertex               vertices[10];                 // ���� ������
	ID3D11Buffer         *m_pVertextBuffer = nullptr;  // vertices�� ���� ���� Buffer
	ID3D11VertexShader   *m_pVertexShader  = nullptr;  // Vertex Stage
	ID3D11PixelShader    *m_pPixelShader = nullptr;
	ID3D10Blob           *m_pVsBlob = nullptr;
	ID3D10Blob           *m_pPsBlob = nullptr;
	ID3D11InputLayout    *m_pInputLayout = nullptr;
public:
	S01_DXLine();
	~S01_DXLine();
};

