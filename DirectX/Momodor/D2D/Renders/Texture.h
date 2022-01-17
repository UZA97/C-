#pragma once
//////////////////////////////////////////////////
// Texture ( Sprite ) Class --> Image 1컷을 저장하고
// rendering하는 class
// Animation Class
//   ---> Aniamtion Clip Class
//         --> Texture Class
//////////////////////////////////////////////////
class Texture
{
public:
	void  Update(Matrix V, Matrix P);
	void  Render();
public:   // Setter
	void  SetPosition(float x, float y)			{ m_Position = Vector2(x, y); }
	void  SetPosition(Vector2 position)			{ m_Position = position; }
	void  SetScale(float x, float y)			{ m_Scale = Vector2(x, y); }
	void  SetScale(Vector2 scale)				{ m_Scale = scale; }
	void  SetRotation(float x, float y,float z) { m_Rotation = Vector3(x, y,z); }
	void  SetRotation(Vector3 rotation)			{ m_Rotation = rotation; }
	void  SetOffset(float x, float y)           { m_SpriteOffset = Vector2(x, y); }
	void  SetOffset(Vector2 offset)             { m_SpriteOffset = offset; }
	void  SetOffsetSize(float x, float y)       { m_SpriteSize = Vector2(x, y); }
	void  SetOffsetSize(Vector2 offsetSize)     { m_SpriteSize = offsetSize; }
	void  SetTech(int tech)						{ m_nTech = tech; }
	void  SetPass(int pass)                     { m_nPass = pass; }
	void  SetFloatConstant(string strName, float delta);
public:   // Getter
	Vector2  GetPosition()						{ return m_Position; }
	Vector2  GetScale()						    { return m_Scale; }
	Vector3  GetRotation()                      { return m_Rotation; }
	Vector2  GetTextureSize()					{ return m_SpriteSize; }
	Shader*  GetShader()						{ return m_pShader; }
	Matrix   GetWorld();
	Vector2  GetRealTextureSize();
private:
	void  CreateShaderResourceView(wstring strImageFile);
	void  CreateVertexBuffer(float startX, float startY, float endX, float endY);
	void  UpDateBlock();    // Uv값을 m_SpriteOffset,m_SpriteSize을 이용해서 변경
	void  UpdateWorld();
private:
	struct Vertex {
		Vector3  Position;   // 정점에 대한 좌표  0~1.0f
		Vector2  Uv;         // Image X(U),Y(V)  0~1.0f
	};

	Vector2   m_Scale        = Vector2(1.0f, 1.0f);       // Scale
	Vector2   m_Position     = Vector2(0.0f, 0.0f);       // 좌표
	Vector3   m_Rotation     = Vector3(0.0f, 0.0f,0.0f);  // 회전
	Vector2   m_ImageSize    = Vector2(1.0f, 1.0f);       // Image Size
	Vector2   m_TextureSize  = Vector2(1.0f, 1.0f);       // Rendering되는 Texture Size
	Vector2   m_SpriteOffset = Vector2(0.0f, 0.0f);       // startX, startY
	Vector2   m_SpriteSize   = Vector2(1.0f, 1.0f);       // endX - startX
	wstring   m_strImageFile;
	int       m_nTech = 0;
	int       m_nPass = 0;
	float     m_Time = 0.0f;
	class     Shader     *m_pShader = nullptr;
private:  // DirectX11
	ID3D11Buffer               *m_pVertexBuffer = nullptr;    // 정점 Buffer
	ID3D11ShaderResourceView   *m_pSRV          = nullptr;
public:
	Texture(wstring strImageFile, wstring strShader);
	Texture(wstring strImageFile, wstring strShader, int startX, int startY, int endX, int endY );
	~Texture();
};
