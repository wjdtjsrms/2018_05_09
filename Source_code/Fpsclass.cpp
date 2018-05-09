
#include "../Header_file/FpsClass.h"

FpsClass::FpsClass(){

}

FpsClass::FpsClass(const FpsClass&){

}
FpsClass::~FpsClass(){

}

void FpsClass::Initialize(){
	m_count = 0;
	m_fps = 0;
	m_startTime = timeGetTime(); //define mmstream.h
	return;
}

void FpsClass::Frame(){
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000)){ //timeGetTime bigger than m_startTime+1000 mean is pass the one Frame?
		m_fps = m_count;
		m_count = 0;
		m_startTime = timeGetTime();
	}
	return;
}

int FpsClass::GetFps(){
	return m_fps; 
}
