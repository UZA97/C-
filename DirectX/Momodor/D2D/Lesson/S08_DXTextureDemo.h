#pragma once
// World 
// View
// Project
// 향후 Class로 변경
class S08_DXTextureDemo : public Scene
{
public:
	struct Vertex     // Texture에 대한 정점
	{
		Vector3 Position; // 정점좌표
		Vector2 Uv;       // 질감을 입히기 위한 좌표 
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
	ID3D11Buffer		      *m_pVertexBuffer = nullptr;  // 정점Buffer
	class Shader			  *m_pShader;
	ID3D11ShaderResourceView  *m_pSRV = nullptr;         // Image File
	class  Texture            *m_pTexture;
public:
	S08_DXTextureDemo();
	~S08_DXTextureDemo();
};
