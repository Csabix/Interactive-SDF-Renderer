#include "gTimer.h"
#include "GLCHECK.h"

GPU_Timer::GPU_Timer(void)
{
	glGenQueries(2, queries);
}

GPU_Timer::~GPU_Timer(void)
{
	glDeleteQueries(2, queries);
}

void GPU_Timer::Start()
{
	glBeginQuery(GL_TIME_ELAPSED, queries[act]);
}

void GPU_Timer::Stop()
{
	glEndQuery(GL_TIME_ELAPSED);
	glGetQueryObjectui64v(queries[act], GL_QUERY_RESULT, &last_delta);
}