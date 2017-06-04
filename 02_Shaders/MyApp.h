#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include "ScreenVAO.h"
#include "FBO.h"
#include "Image2D.h"
#include "ShaderProg.h"
#include "Uniforms.h"
#include "DebugWindow.h"
#include "TextEditor.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp() = default;
	bool Init();
	void Clean();
	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

	int presentation = 0;
	void PresentationUpdate();
	void PrezentationRender();
	//constant, dummy, utility objects
	ScreenVAO vao;
	Image2D init_textures[4];

	//gpu state holders
	struct
	{
		GPUState ping;
		GPUState pong;
		GPUState *current;
		GPUState *target;
	}gpu;
	Uniforms uniforms;

	//user interface
	gCamera cam;
	GUI::TextEditor editor;
	GUI::DebugWindow debug;

	//other state variables
	int iternum = 0;
};

//Possible Improovements:
// - One FBO
	// - Simply not pinponging - may not work
	// - Using One program as described below
// - One Shader program
	// - Show or Calculate subroutine, supress output from latter
	// - Image load/store
	// - 