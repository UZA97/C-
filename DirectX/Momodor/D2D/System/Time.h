#pragma once
////////////////////////////////////////////////////////
// 게임프로그램에서 여러곳에 사용이된다.  플레이어/몬스터 움직
//  Time, 키보드, 마우스 etc은 싱글톤(단한개만 생성되게 만드는법)
//  으로 만든다
//  만드는 방법
//  1. 생성자/소멸자를  public --> private로 
//  2. public함수로  생성하는 함수를 만든다
////////////////////////////////////////////////////////
class Time
{
public:
	// 2. public함수로  생성하는 함수를 만든다
	static Time* GetInstance()
	{
		static  Time singletoneInstance;
		return  &singletoneInstance;
	}

public:
	void  Update();                    // 시간을 Update
	void  Update(float lockFPS);       // 시간을 Update    Frame Per Second
	float Delta() { return timeElased; }
	float GetRunning() { return m_RunningTime; }
	float timeElased = 0.0f;          // 이전 프레임으로 부터 경과된 시간
	float GetFPS() {return m_FramePerSecond;}
private:
	INT64 m_TickPerSecond = 0;         // 초당 Tick Count
	INT64 m_CurrentTime   = 0;         // 현재 시간
	INT64 m_LastTime      = 0;         // 이전 시간
	INT64 m_LastFPSUpdate = 0;         // 마지막 FPS 업데이트 시간
	INT64 m_FPSUpdateInterval = 0;     // FPS 업데이트 간격
	UINT  m_FrameCount = 0;            // Frame 수
	float m_RunningTime = 0.0f;        // 진행시간 ( Start --> Current)
	float m_FramePerSecond = 0.0f;     // FPS

private:  // 1. 생성자/소멸자를  public --> private로 
	Time();
	~Time();
};

