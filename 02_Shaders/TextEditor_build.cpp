//build related fucntions of Text Editor
#include "TextEditor.h"


bool GUI::TextEditor::BuildComputeFragShader()
{
	log.str(std::string());	//log.clear() is inherited form ios for clearing error state !!
	std::string extra_line = std::string("\n#line 1 \"") + filenames.current + "\"\n";
	return compute.AttachShader(GL_FRAGMENT_SHADER, { source_codes.begin.c_str(), extra_line.data(), text.data(), source_codes.end.c_str() });
}

bool GUI::TextEditor::ReloadFragShaderConstCode()
{
	source_codes.begin = FileIO::getCompFSCodeBegin();
	source_codes.end = FileIO::getCompFSCodeEnd();
	return true;
}

bool GUI::TextEditor::BuildComputeVertShader()
{
	log.str(std::string());	//log.clear() is inherited form ios for clearing error state
	return compute.AttachShader(GL_VERTEX_SHADER, FileIO::getCompVSCode());
}

bool GUI::TextEditor::LinkComputeProgram()
{
	compute.BindAttribLoc(0, "pos");
	states.is_valid_program = false;
	return compute.Link();
}

bool GUI::TextEditor::BuildShowFragShader()
{
	log.str(std::string());	//log.clear() is inherited form ios for clearing error state
	return show.AttachShader(GL_FRAGMENT_SHADER, FileIO::getShowFSCode());
}

bool GUI::TextEditor::BuildShowVertShader()
{
	log.str(std::string());	//log.clear() is inherited form ios for clearing error state
	return show.AttachShader(GL_VERTEX_SHADER, FileIO::getShowVSCode());
}

bool GUI::TextEditor::LinkShowProgram()
{
	show.BindAttribLoc(0, "pos");
	return show.Link();
}

void GUI::TextEditor::BuildShortcut()
{
	if(BuildComputeFragShader())
		LinkComputeProgram();
}

void GUI::TextEditor::RebuildCompShortcut()
{
	if(ReloadFragShaderConstCode())
		if(BuildComputeFragShader())
			if(BuildComputeVertShader())
				LinkComputeProgram();
}

void GUI::TextEditor::RebuildShowShortcut()
{
	if(BuildShowFragShader())
		if(BuildShowVertShader())
			LinkShowProgram();
}