#pragma once
class  Texture;
class Background
{
public: 
	void   Update(Matrix V, Matrix P);  // Shader V,P
	void   Render();

public:  // Setter
	void    SetPosition(Vector2 position) { m_Position = position; }
private:
	Texture *m_pTexture = nullptr;
	Texture *m_pBush[2];
	Texture *m_pCloud[2];
	Vector2  m_Position = Vector2(0.0f, 0.0f);
	float    m_Time = 0.0f;
public:
	Background();
	~Background();
};