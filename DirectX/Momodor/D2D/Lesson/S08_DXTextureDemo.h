#pragma once
// World 
// View
// Project
// ���� Class�� ����
class S08_DXTextureDemo : public Scene
{
public:
	struct Vertex     // Texture�� ���� ����
	{
		Vector3 Position; // ������ǥ
		Vector2 Uv;       // ������ ������ ���� ��ǥ 
	};


public:
	void Update() override;
	void Render() override;
private:
	void CreateVertexBuffer();
private:
	wstring                   m_ImageFile = L"./_Textures/wizard.png";
	//wstring                   m_ImageFile = L"./_Textures/bullet2.bmp";
	Vertex					  vertices[6];
	ID3D11Buffer		      *m_pVertexBuffer = nullptr;  // ����Buffer
	class Shader			  *m_pShader;
	ID3D11ShaderResourceView  *m_pSRV = nullptr;         // Image File
	class  Texture            *m_pTexture;
public:
	S08_DXTextureDemo();
	~S08_DXTextureDemo();
};
