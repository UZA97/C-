#pragma once
#define KEYMAX 256
////////////////////////////////////////////////////////////
// 키보드,Time,마우스 등등은  싱글톤베이스로 만든다
////////////////////////////////////////////////////////////
class Keyboard
{
public:
	static Keyboard* GetInstance()
	{
		static Keyboard singleToneInstance;   // class 가 생성
		return &singleToneInstance;           // class의 주소를 return
	}
public:
	bool Down(int key);                       // Key가 한번 눌러진 경우
	bool Up(int key);                         // Key가 한번 Up된 경우
	bool Press(int key);                      // Key를 계속적으로 눌르는 경우
	bool Toggle(int key);                     // Key를 On/Off
private:
	bitset<KEYMAX>  m_KeyUP;                  // UP된 정보
	bitset<KEYMAX>  m_KeyDown;                // Down된 정보
	bitset<KEYMAX>  GetKeyUp()   { return m_KeyUP; }
	bitset<KEYMAX>  GetKeyDown() { return m_KeyDown; }
	void  SetKeyDown(int key, bool state) { m_KeyDown.set(key, state); }
	void  SetKeyUp(int key, bool state)   { m_KeyUP.set(key, state); }
private: // 생성자/소멸자 private로 한다
	Keyboard();
	~Keyboard();

};

