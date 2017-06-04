#include "FileIO.h"
#include <fstream>
#include <iostream>
#include <vector>

typedef std::vector<std::string> FileList;

FileList getBeginShaderProgramFiles()
{
	FileList files;
	files.emplace_back("primitives");
	files.emplace_back("header");
	for(auto &file : files)
		file = "Shaders/" + file + ".frag";
	return files;
}

FileList getEndShaderProgramFiles()
{
	FileList files;
	files.emplace_back("shade");
	files.emplace_back("main");

	files.emplace_back("SphereTrace/spheretrace_1");
	files.emplace_back("SphereTrace/spheretrace_2");
	files.emplace_back("SphereTrace/spheretrace_3");

	files.emplace_back("Antialias/antialias_0");

	files.emplace_back("Norm/norm_0");
	files.emplace_back("Norm/norm_1");
	files.emplace_back("Norm/norm_2");

	files.emplace_back("Ambient/ambient_0");
	files.emplace_back("Ambient/ambient_1");

	files.emplace_back("Shadow/shadow_0");
	files.emplace_back("Shadow/shadow_1");
	files.emplace_back("Shadow/shadow_2");
	files.emplace_back("Shadow/shadow_3");
	files.emplace_back("Shadow/shadow_4");
	for(auto &file : files)
		file = "Shaders/" + file + ".frag";
	return files;
}

bool FileIO::loadFileIntoString(const std::string &__restrict path, std::string &__restrict code)
{
	std::ifstream file(path);
	if(!file.is_open())
	{
		std::cout << "Hiba a " << path << " megnyitásakor." << std::endl;
		return false;
	}
	std::string line;
	while(getline(file, line))
		code += line + '\n';
	file.close();
	return true;
}

void FileIO::loadFilesIntoString(const FileList &__restrict files, std::string &__restrict code)
{
	for(auto & path : files)
	{
		code += "\n#line 0 \"" + path + "\"\n\n";
		if(!loadFileIntoString(path, code))
			code += "\n// Hiba a " + path + " shader fájl betöltésekor!\n\n";
	}
}
bool FileIO::SaveTo(const char *__restrict path, const char *__restrict text, const size_t length, std::ostream *__restrict log)
{
	std::ofstream out(path, std::ofstream::binary);	//Start from end
	if(!out.is_open())
	{
		(log != nullptr ? *log : std::cout) << "Could not Open file : " << path << std::endl;
		return false;
	}
	out.write(text, (length == 0 ? strlen(text) : length));
	out.close(); return true;
}

bool FileIO::SaveFormattedData(const char *__restrict path, const float *__restrict values, size_t width, size_t height, size_t stride, std::ostream *__restrict log)
{
	std::ofstream out(path); //for use in matlab
	if(!out.is_open())
	{
		(log != nullptr ? *log : std::cout) << "Could not Open file : " << path << std::endl;
		return false;
	}
	out.precision(16);
	for(int j = 0; j < height; ++j)
	{
		for(int i = 0; i < width; ++i)
		{
			out << values[width*j + i] << ' ';
		}
		out << std::endl;
	}
	out.close();
	return true;
}

bool FileIO::SaveUnformattedData(const char *__restrict path, const char *__restrict values, size_t size, std::ostream *__restrict log)
{
	std::ofstream out(path, std::ofstream::binary);
	if(!out.is_open())
	{
		(log != nullptr ? *log : std::cout) << "Could not Open file : " << path << std::endl;
		return false;
	}
	out.write((char*)(values), size);
	out.close();
	return true;
}

bool FileIO::OpenFrom(const char *__restrict path, std::vector<char> &__restrict text, std::ostream *__restrict log)
{
	std::ifstream in(path, std::ifstream::binary | std::ifstream::ate);
	if(!in.is_open())
	{
		(log != nullptr ? *log : std::cout) << "Could not Open file : " << path << std::endl;
		return false;
	}
	std::streamsize length = in.tellg();
	text.resize((size_t)length,'\0');
	in.seekg(0, in.beg);
	in.read(text.data(), length);

	in.close(); return true;
}

std::string FileIO::getCompFSCodeBegin()
{
	std::string code = "#version 450";
	std::vector<std::string> files = getBeginShaderProgramFiles();
	loadFilesIntoString(files, code);
	return code;
}

std::string FileIO::getCompFSCodeEnd()
{
	std::string code;
	std::vector<std::string> files = getEndShaderProgramFiles();
	loadFilesIntoString(files, code);
	return code;
}

std::string FileIO::getCompVSCode()
{
	std::string code = "#version 450\n#line 1 \"Shaders/_Unique/compute.vert\"\n";
	loadFileIntoString("Shaders/_Unique/compute.vert", code);
	return code;
}

std::string FileIO::getShowFSCode()
{
	std::string code = "#version 450\n#line 1 \"Shaders/_Unique/main.frag\"\n";
	loadFileIntoString("Shaders/_Unique/main.frag", code);
	return code;
}

std::string FileIO::getShowVSCode()
{
	std::string code = "#version 450\n#line 1 \"Shaders/_Unique/show.vert\"\n";
	loadFileIntoString("Shaders/_Unique/show.vert", code);
	return code;
}
