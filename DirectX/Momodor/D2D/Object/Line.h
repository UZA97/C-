#pragma once
///////////////////////////////////////////////
// Line Class
//////////////////////////////////////////////
class Line
{
public:
	   struct Vertex
	   {
		   Vector3 Position;
	   };


public:
	void	Update(Matrix V, Matrix P);
	void	Render();
	void    AddVertex(float x, float y);
	void    End() { CreateVertexBuffer(); }
public:
	Vector2* GetPosition(int id) { return m_cvVertexDBs[id]; }
	int      GetVertexSize() { return (int)m_cvVertexDBs.size(); }
private:
	void  CreateVertexBuffer();
	void  UpdateWorld();


private:
	Color            color = Color(0, 1, 0, 1);
	ID3D11Buffer     *m_pVertexBuffer = nullptr;  // Á¤Á¡Buffer
	class Shader     *m_pShader;
	vector<Vector2*>  m_cvVertexDBs;
	Vertex           *m_pVertices = nullptr;
public:
	Line();
	Line(Vector2 position1, Vector2 position2);
	~Line();
};
