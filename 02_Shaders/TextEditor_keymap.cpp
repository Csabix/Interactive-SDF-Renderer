//Keyboard interraction functions of TextEditor
#include "TextEditor.h"

void GUI::TextEditor::KeyboardDown(SDL_Keycode key)
{
	switch(key)
	{
	case SDLK_RCTRL:	case SDLK_LCTRL:	states.key.ctrl = true;		break;
	case SDLK_RSHIFT:	case SDLK_LSHIFT:	states.key.shift = true;		break;
	case SDLK_RALT:		case SDLK_LALT:		states.key.alt = true;		break;
	case SDLK_F11:  Resize(sizes.full_width, sizes.full_height); break;
	}
	if(states.key.ctrl && states.window.p_open && !states.window.is_open_openpopup && !states.window.is_open_savepopup)
	{
		switch(key)
		{
		case SDLK_n: NewFileShortcut(); break;
		case SDLK_s: (states.key.shift ? SaveAsShortcut() : SaveShortcut()); break;
		case SDLK_o: (states.key.shift ? OpenShortcut() : Open(filenames.current)); break;
		case SDLK_b: BuildShortcut(); break;
		case SDLK_r: (states.key.shift ? RebuildShowShortcut() : RebuildCompShortcut()); break;
		default:break;
		}
	}
}

void GUI::TextEditor::KeyboardUp(SDL_Keycode key)
{
	switch(key)
	{
	case SDLK_RCTRL:	case SDLK_LCTRL:	states.key.ctrl = false;		break;
	case SDLK_RSHIFT:	case SDLK_LSHIFT:	states.key.shift = false;		break;
	case SDLK_RALT:		case SDLK_LALT:		states.key.alt = false;		break;
	}
}