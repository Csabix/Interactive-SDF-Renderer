#include "MyApp.h"
#include "GLCHECK.h"
#include "FileIO.h"

CMyApp::CMyApp() : debug(iternum, uniforms, gpu.current, gpu.target, cam), uniforms(debug){}

bool CMyApp::Init()
{
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
	glDisable(GL_SCISSOR_TEST);		glDisable(GL_SAMPLE_MASK);
	glDisable(GL_SAMPLE_COVERAGE);	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);		glDisable(GL_BLEND);
	glDisable(GL_DITHER);

	cam = gCamera(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

	//gpu.ping.set(1280,720);
	//gpu.pong.set(1280,720);
	//editor.Resize(1280,720);
	Resize(1280, 720);
		
	editor.RebuildShowShortcut();
	editor.Open("Shaders/SDF/mandelbulb_iq.frag");
	editor.RebuildCompShortcut();

	//const short depth_data = 0x0000;
	const float init_zeros[4] = {0.f,0.f,0.f,0.f};
	for(auto &i : init_textures)
		i.set(1, 1, 4, 32, true, init_zeros);

	vao.on();
	return true;
}

void CMyApp::Clean(){}

void CMyApp::Update()
{
	editor.compute.On();					//Activating 1st program
	uniforms.Iteration(iternum);			//Heuristics (needs impoovements) : user defined
	if(0 == iternum)
	{
		for(auto i = 0; i < 4; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, init_textures[i].get());
		}
		uniforms << cam;

		gpu.target = &gpu.ping;				//Render to ping first
		gpu.current = &gpu.pong;			//This will be current
	}
	else
	{
		gpu.current->bindTextures();		//Read the current state
	}
	gpu.target->on();						//Activating FBO
	editor.compute << uniforms;				//Sending uniforms
 	vao.draw();							 	//Draw rectangle
	
	std::swap(gpu.current, gpu.target);		//swapping FBO-s
	++iternum;	//TODO? implement Restart() function : might be slower ?
}

void CMyApp::Render()
{
	this->PrezentationRender();
	if(!editor.isProgramValid_AND_Reset()) iternum = 0;

	auto ticks = SDL_GetTicks();
	static Uint32 last_time = ticks;
	float delta_time = (ticks - last_time) / 1000.0f;
	if(cam.Update(delta_time)) iternum = 0;
	last_time = ticks;
	
	FBO::Default();								//Default FBO
	editor.show.On();							//Activating 2nd program
	uniforms.setShowUniformsToShaderProg(editor.show);

	(debug.show_currentstate ? gpu.current : gpu.target)->bindTextures();
	vao.draw();									//Drawing
	if(iternum > 30) Sleep(glm::min<int>((int)(iternum*0.75), 13));

	editor.Render();	debug.Render();			//Renders GUI to VBOs
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if(cam.KeyboardDown(key)) iternum = 0;
	switch(key.keysym.sym)
	{
	case SDLK_RIGHT: ++presentation; this->PresentationUpdate(); break;
	case SDLK_LEFT: --presentation; this->PresentationUpdate(); break;
	case SDLK_SPACE: case SDLK_PAUSE: debug.pause = !debug.pause; break;
	case SDLK_RETURN: iternum = 0; break;
	case SDLK_c: cam = gCamera(glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); iternum = 0; break;
	case SDLK_KP_PLUS: cam.SetSpeed(cam.GetSpeed()*2.f); break;
	case SDLK_KP_MINUS: cam.SetSpeed(cam.GetSpeed()*0.5f); break;
	}
	editor.KeyboardDown(key.keysym.sym);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	if(cam.KeyboardUp(key)) iternum = 0;
	editor.KeyboardUp(key.keysym.sym);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if(cam.MouseMove(mouse)) iternum = 0;
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse){}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse){}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
	if(wheel.y == 1) cam.SetSpeed(cam.GetSpeed()*1.25);
	else if(wheel.y == -1) cam.SetSpeed(cam.GetSpeed()*0.75);
}

void CMyApp::Resize(int _w, int _h)
{
	iternum = 0;	//reset calculation
	glViewport(0, 0, _w, _h );
	gpu.ping.set(_w, _h);
	gpu.pong.set(_w, _h);
	uniforms.setWH(_w, _h);
	editor.Resize((float)_w, (float)_h);
	debug.Resize(_w, _h);
}

float start_time = SDL_GetTicks()/1000.f;
int sub_prez_framecnt = 0;

void CMyApp::PresentationUpdate()
{
	start_time = SDL_GetTicks()/1000.f;
	uniforms.algorithms.shadow = Uniforms::SHADOW_4;
	switch(presentation)
	{
	case 0: case 1:
		editor.Open("Shaders/SDF/csg.frag");
		editor.RebuildCompShortcut();
		uniforms.shadows.step_multipier = 0.5;
		debug.ambient.start_iternum = 10;
		uniforms.shadows.light_pos = glm::vec3(0.58, 19.4, 26);
		uniforms.shadows.light_radius = 3;
		break;
	case 2:
		editor.Open("Shaders/SDF/aprajafalva.frag");
		editor.RebuildCompShortcut();		
		uniforms.shadows.step_multipier = 1;
		uniforms.shadows.light_pos = glm::vec3(18.6f, -0.7f, 3.3f);
		uniforms.shadows.light_radius = 1;
		break;
	case 3:
		editor.Open("Shaders/SDF/metasdf.frag");
		editor.RebuildCompShortcut();
		uniforms.shadows.step_multipier = 0.5;
		uniforms.shadows.light_radius = 0.5;
		uniforms.shadows.light_pos = glm::vec3(0.053, 2.149, 1.101);
		break;
	case 4: case 5: case 6: case 7:
		editor.Open("Shaders/SDF/mandelbulb_iq.frag");
		editor.RebuildCompShortcut();
		uniforms.shadows.light_pos = glm::vec3(18.6f, -0.7f, 3.3f);
		cam.LookAt(glm::vec3(0));
		uniforms.shadows.step_multipier = 1;
		uniforms.shadows.light_radius = 0.5;
		break;

	}
	debug.times.optimize = true;
	debug.pause = false;
	debug.turn_pause_to_true = false;
	sub_prez_framecnt = 0;
	PrezentationRender();
}

void CMyApp::PrezentationRender()
{
	static float start_0 = 0;
	switch(presentation)
	{
	case 0:
	{
		const float F = 4.5;
		float t = SDL_GetTicks() / 1000.f - start_time;
		float fi = t* 3.1416f * 2/8.f;
		glm::vec3 eye = glm::vec3(30*cos(fi), 30*sin(fi), 10);
		cam.SetView(eye, glm::vec3(0, 0, 7), cam.GetUp());
		iternum = 0;
		start_0 = start_time;
	}
	break;
	case 1:
	{
		const float F = 4.5;
		const glm::vec3 camera_1 = glm::vec3(-25.53, 11.28, 28.10);
		float t = SDL_GetTicks() / 1000.f - start_time;
		float t2 = SDL_GetTicks() / 1000.f - start_0;
		if(t < F)
		{
			/*float x = glm::smoothstep<float>(0, 1, t / F);
			float fi = x*3.14f * 2;
			glm::vec3 eye = glm::vec3((22 + x * 10)*cosf(fi), (20 + x * 10)*sinf(fi), 5 + 20 * x);
			cam.SetView(eye, glm::vec3(0, 0, 1), cam.GetUp());*/
			float fi = t2 * 3.1416f * 2 / 8.f;
			glm::vec3 p = glm::vec3(30 * cos(fi), 30 * sin(fi), 10);
			float x = glm::smoothstep<float>(0, 1, t / F);
			//x = glm::smoothstep<float>(0, 1, x);
			glm::vec3 eye = glm::mix(p, camera_1,glm::vec3(x));
			cam.SetView(eye, glm::vec3(0, 0, 7), cam.GetUp());
			iternum = 0;
		}
		else
		{
			if(sub_prez_framecnt ==0) iternum = 0;
			++sub_prez_framecnt;
			uniforms.shadows.step_multipier = 0.1;
		}
	}
		break;
	case 2:
	{
		const float F = 2.5;
		const float T = 0.3;
		float t = SDL_GetTicks() / 1000.f - start_time;
		if(t < F)
		{
			glm::vec3 a1 = glm::vec3(9.87, 64.86, 32.37);
			glm::vec3 a2 = glm::vec3(9.15, 46.31, 19.35);
			glm::vec3 c1 = glm::vec3(9.15, -8.7, 3.14);
			glm::vec3 c2 = glm::vec3(24.16, 7.8, -0.97);
			glm::vec3 b1 = (a1 + c1)*0.5f;
			glm::vec3 b2 = a2*0.05f + c2*0.95f;
			float x = glm::smoothstep<float>(0, 1, t / F);
			glm::vec3 ab1 = a1*(1 - x) + b1*x;
			glm::vec3 bc1 = b1*(1 - x) + c1*x;
			glm::vec3 ab2 = a2*(1 - x) + b2*x;
			glm::vec3 bc2 = b2*(1 - x) + c2*x;
			glm::vec3 eye = ab1*(1 - x) + bc1*x;
			glm::vec3 at = ab2*(1 - x) + bc2*x;
			cam.SetView(eye, at, cam.GetUp());
			iternum = 0;
		}
		else
		{
			uniforms.shadows.step_multipier = 0.2;
			uniforms.algorithms.shadow = Uniforms::SHADOW_2;
			debug.times.optimize = false;
			debug.pause = true;
			if(sub_prez_framecnt % 17 == 0)			debug.pause = false;
			if(sub_prez_framecnt % (17*35) == 0)	iternum = 0;
			++sub_prez_framecnt;
		}
	}
	break;
	case 3:	//metasdf
	{
		const float F = 2.0;
		float t = SDL_GetTicks() / 1000.f - start_time;
		if(t < F)
		{
			float x = glm::smoothstep<float>(0, 1, t / F);
			glm::vec3 a = glm::vec3(2,2,1.9);
			glm::vec3 b = glm::vec3(1.5,-1.5,0.3);
			glm::vec3 eye = a*(1-x)+b*x;
			cam.SetView(eye, glm::vec3(0), cam.GetUp());
			iternum = 0;
		}
		else
		{
			uniforms.shadows.step_multipier = 0.1;
		}
	}
	break;
	case 4:
	{

		const float F = 10;
		float t = SDL_GetTicks() / 1000.f - start_time;
		if(t < F)
		{
			float x = glm::smoothstep<float>(0, 1, t / F);
			float fi = (float)(x*M_PI * 2 + M_PI / 4.0);
			glm::vec3 eye = glm::vec3(2.5*cos(fi), 2.5*sin(fi), 2.0);
			cam.SetView(eye, cam.GetAt(), cam.GetUp());
			iternum = 0;
		}
	}
	break;
	case 5:
	{
		const float F = 4.5;
		float t = SDL_GetTicks() / 1000.f - start_time;
		if(t < F)
		{
			float x = glm::smoothstep<float>(0, 1, powf(t / F, 0.2f));
			float fi = (float)(M_PI / 4.0);
			glm::vec3 a = glm::vec3(2.5*cos(fi), 2.5*sin(fi), 2.0);
			glm::vec3 b = glm::vec3(0.41, 0.33, 0.41)*1.05f;
			glm::vec3 eye = a*(1 - x) + b*x;
			cam.SetView(eye, cam.GetAt(), cam.GetUp());
			iternum = 0;
		}
	}
	break;
	case 6:
	{
		const float F = 5.0;
		float t = SDL_GetTicks() / 1000.f - start_time;
		if(t < F)
		{
			float x;
			glm::vec3 a = glm::vec3(0.41, 0.33, 0.41)*1.05f;
			glm::vec3 b = glm::vec3(2.5);
			glm::vec3 c = glm::vec3(-0.37, 0.14, 1.12)*1.025f;
			x = glm::smoothstep<float>(0, 1, powf(t / F, 2));
			glm::vec3 ab = a*(1 - x) + b*x;
			x = glm::smoothstep<float>(0, 1, powf(t / F, 0.5));
			glm::vec3 bc = b*(1 - x) + c*x;
			x = glm::smoothstep<float>(0, 1, powf(t / F, 1));
			glm::vec3 eye = ab*(1 - x) + bc*x;
			cam.SetView(eye, cam.GetAt(), cam.GetUp());
			iternum = 0;
		}
		else
		{
			uniforms.shadows.step_multipier = 0.5;
		}
	}
	break;
	case 7:	//mandelbulb
	{
		const float F = 8.5;
		float t = SDL_GetTicks() / 1000.f - start_time;
		if(t < F)
		{
			float x = glm::smoothstep<float>(0, 1, powf(t / F, 0.5))*10-9;
			glm::vec3 a1 = glm::vec3(0.9440, 0.2072, 0.5348);
			glm::vec3 b1 = glm::vec3(0.8368, 0.2635, 0.5658);
			glm::vec3 eye = a1*(1 - x) + b1*x;
			x = glm::smoothstep<float>(0, 1, powf(t / F, 0.5));
			glm::vec3 a2 = glm::vec3(0.8528, 0.1251, 0.4643);
			glm::vec3 b2 = eye + glm::normalize(b1-a1);
			//glm::vec3 b2 = glm::vec3(0.8343, 0.2648, 0.5665);
			glm::vec3 c2 = glm::vec3(0.7227, 0.2128, 0.5717);
			glm::vec3 at = (a2*(1 - x) + b2*x)*(1-x) + (b2*(1 - x) + c2*x)*x;
			cam.SetView(eye, at, cam.GetUp());
			iternum = 0;
		}
	}
	break;
	}
}