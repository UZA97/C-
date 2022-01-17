#pragma once
class S05_DXEffect : public Scene
{
public:
	   struct Vertex
	   {
		   Vector3 Position;
		   Color   Color;
	   };


public:
	void Update() override;
	void Render() override;

private:
	void  CreateVertexBuffer();

private:
	Color          color = Color(0, 1, 0, 1);
	Vertex         vertices[10];
	ID3D11Buffer   *m_pVertexBuffer = nullptr;  // Á¤Á¡Buffer
	class Shader   *m_pShader;
public:
	S05_DXEffect();
	~S05_DXEffect();
};
