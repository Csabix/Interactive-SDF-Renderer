#pragma once

/*
	Nanoszekundum pontosságú (*) GPU idõzítõ.

	Fontos: nem lehet egymásba ágyazni a különbözõ timer-ek kéréseit sem, amíg
	az OpenGL specifikáció nem engedélyezi a nested glBegin/EndQuery-ket!

	(*): driver és GPU függõ
*/

#include <GL\glew.h>
#include <chrono>
class GPU_Timer
{
public:
	GPU_Timer(void);
	~GPU_Timer(void);

	void Start();
	void Stop();
	inline void swap() { act = 1 - act; }

	inline GLuint64 GetLastDeltaNano(){	return last_delta;}
	//inline double GetLastDeltaMicro(){	return last_delta / 1000.0f;}
	inline double GetLastDeltaMilli(){	return last_delta / 1000000.0f;}

private:
	GLuint		queries[2];
	int act = 0;
	GLuint64	last_delta;
};

class CPU_Timer
{
protected:
	std::chrono::high_resolution_clock::time_point start, finish;
public:
	inline void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	inline void Finish()
	{
		finish = std::chrono::high_resolution_clock::now();
	}
	inline double GetSeconds()
	{
		return std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
	}
};