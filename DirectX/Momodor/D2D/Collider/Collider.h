#pragma once
/////////////////////////////////////////////////////////////
// 충돌 Check용 Class
////////////////////////////////////////////////////////////
class Collider
{
public:
	void			Update(Matrix V, Matrix P);
	void			Render();

	// General
	static         bool  AaBB(Collider *A, Collider *B);
	static         bool  AaBB(Matrix A, Matrix B);
	static         bool  ClippingPolygon(Collider *A, Collider *B);
	static         bool  ClippingPolygon(Matrix A, Matrix B);
	static         bool  IsPtInRegion(vector<Vector2*> m_cvPolygons, Vector2 position);
	               bool  IsPtInRect(Vector2 position);
    static         bool  IntersectionLine(Vector2 AP1, Vector2 AP2, Vector2 BP1, Vector2 BP2, Vector2& out);

	// getter
	Vector2			GetPosition() { return m_Position; }
	Vector2			GetScale() { return m_Scale; }
	Vector3			GetRotation() { return m_Rotation; }
	Matrix          GetWorld() { return m_WorldMatrix; }

    // setter
	void            SetParent(GameObject* parent) { m_Parent = parent; }
	void			SetColor(float r, float g, float b){ color = Color(r, g, b, 1); }
	void			SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void			SetPosition(Vector2 position) { m_Position = position; }
	void			SetScale(float x, float y)    { m_Scale    = Vector2(x, y); }
	void			SetScale(Vector2 scale)       { m_Scale    = scale; }
	void			SetRotation(float x, float y, float z) { m_Rotation = Vector3(x, y, z); }
	void			SetRotation(Vector3 rotation) { m_Rotation = rotation; }
private:
	void			CreateVertexBuffer();
	void			UpdateWorld();
	Matrix          m_WorldMatrix;

private:
	struct Vertex
	{
		Vector3 Position;
	};

	Vector2        m_Position = Vector2(0.0f, 0.0f);
	Vector2        m_Scale = Vector2(1.0f, 1.0f);
	Vector3        m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	Color          color = Color(0, 1, 0, 1);
	ID3D11Buffer   *m_pVertexBuffer = nullptr;  // 정점Buffer
	class Shader   *m_pShader;
	GameObject     *m_Parent  = nullptr;       // GameObect
public:
	Collider();
	~Collider();
};
