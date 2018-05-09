#ifndef _FPSCLSS_H_
#define _FPSCLSS_H_

#pragma comment (lib,"winmm.lib")

#include <mmstream.h>
#include <Windows.h>


class FpsClass{
public :
	FpsClass();
	FpsClass(const FpsClass&);
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime; // 	0 to 4,294,967,295
};

#endif