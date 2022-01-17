#include "framework.h"
#include "Keyboard.h"

// 0x0000  이전에 누른적이 없고 호출 시점에서 안눌린 상태
// 0x8000  이전에 누른적이 없고 호출 시점에서 눌린 상태
// 0x8001  이전에 누른적이 있고 호출 시점에서 눌린 상태
// 0x0001  이전에 누른적이 있고 호출 시점에서 안눌린 상태


//////////////////////////////////////////////////////
// 생성자 / 소멸자
/////////////////////////////////////////////////////
Keyboard::Keyboard()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		SetKeyDown(i, false);
		SetKeyUp(i, false);
	}
}

Keyboard::~Keyboard()
{
}
bool Keyboard::Down(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) // 0x8000  이전에 누른적이 없고 호출 시점에서 눌린 상태
	{
		if (!this->GetKeyDown()[key])
		{
			this->SetKeyDown(key, true);
			return true;
		}
	}
	else
		this->SetKeyDown(key, false);
	return false;
}

bool Keyboard::Up(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		this->SetKeyUp(key, true);
	else
	{
		if (this->GetKeyUp()[key])
		{
			this->SetKeyUp(key, false);
			return true;
		}

	}
	return false;
}

bool Keyboard::Press(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;
	return false;
}

bool Keyboard::Toggle(int key)
{
	return false;
}


