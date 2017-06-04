//file input and output related functions of TextEditor
#include "TextEditor.h"

void GUI::TextEditor::Open(const char * file)
{
	if(FileIO::OpenFrom(file, text, &log))
	{
		if(file != filenames.current)
		{
			strcpy_s(filenames.current, file);
			filenames.recents.emplace(file);
		}
		text.resize(text.size() + 100);
		sprintf_s(winnames.editor, "%s     [SDF Editor]##SdfEditor", filenames.current);
	}
}

void GUI::TextEditor::Save(const char * file)
{
	if(FileIO::SaveTo(file, text.data(), 0, &log))
	{
		strcpy_s(filenames.current, file);
		filenames.recents.emplace(file);
		sprintf_s(winnames.editor, "%s     [SDF Editor]##SdfEditor", filenames.current);
	}
}

void GUI::TextEditor::SaveShortcut()
{
	if(0 == strcmp(filenames.current, "untitled.frag"))
	{
		strcpy_s(filenames.buffer, filenames.current);
		states.window.is_open_savepopup = true;
	}
	else
		Save(filenames.current);
}

void GUI::TextEditor::SaveAsShortcut()
{
	strcpy_s(filenames.buffer, filenames.current);
	states.window.is_open_savepopup = true;
}

void GUI::TextEditor::OpenShortcut()
{
	strcpy_s(filenames.buffer, filenames.current);
	states.window.is_open_openpopup = true;
}

void GUI::TextEditor::NewFileShortcut()
{
	const char str[] = "vec3 color(in vec3 p, in vec3 v)\n{\n\treturn vec3(1);\n}\n\nfloat sdf(in vec3 p, in vec3 v)\n{\n\treturn min(sphere(p,v,1),planeXY(p, v));\n}\n";
	text.assign(std::begin(str), std::end(str));
	text.resize(text.size() + 100);
	strcpy_s(filenames.current, "untitled.frag");
}