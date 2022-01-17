#pragma once
// Rectangle Class
class Rect
{
public:
	   struct Vertex
	   {
		   Vector3 Position;
		   Color   Color;
	   };


public:
	void	Update(Matrix V, Matrix P);
	void	Render();

public:
	bool    IsPtInRect(Vector2 position);
	bool    IsPtInRegion(Vector2 position);


	// getter
	Vector2 GetPosition() { return m_Position; }
	Matrix   GetMatrix()   { return m_World; }
    // setter
	void SetColor(float r, float g, float b , float a=1){ color = Color(r, g, b, a); }
	void SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void SetPosition(Vector2 position) { m_Position = position; }
	void SetScale(float x, float y)    { m_Scale    = Vector2(x, y); }
	void SetScale(Vector2 scale)       { m_Scale    = scale; }
	void			SetRotation(float x, float y, float z) { m_Rotation = Vector3(x, y, z); }
	void			SetRotation(Vector3 rotation) { m_Rotation = rotation; }

private:
	void  CreateVertexBuffer();
	void  UpdateWorld();
	int   OuterCode(Vector2 BoxMin, Vector2 BoxMax, Vector2 position);


private:
	Matrix         m_World;
	Vector2        m_Position = Vector2(0.0f, 0.0f);
	Vector2        m_Scale = Vector2(1.0f, 1.0f);
	Vector3        m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	Color          color = Color(0, 1, 0, 1);
	Vertex         vertices[10];
	ID3D11Buffer   *m_pVertexBuffer = nullptr;  // Á¤Á¡Buffer
	class Shader   *m_pShader;
public:
	Rect();
	Rect(Vector2 position, Vector2 scale);
	~Rect();
};
