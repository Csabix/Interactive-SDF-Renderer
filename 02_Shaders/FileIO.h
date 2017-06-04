#pragma once
#include <string>
#include <vector>
typedef std::vector<std::string> FileList;

inline std::string concat(const std::string &s, int i) { return s + std::to_string(i); }
inline std::string concat(const std::string &s, int i, int j) { return s + std::to_string(i) + std::to_string(j); }

namespace FileIO
{
	bool loadFileIntoString(const std::string &__restrict path, std::string &__restrict code);
	void loadFilesIntoString(const FileList &__restrict files, std::string &__restrict code);

	bool SaveTo(const char *__restrict path, const char* __restrict text, const size_t = 0, std::ostream *__restrict log = nullptr);
	bool SaveFormattedData(const char *__restrict path, const float*__restrict values, size_t width, size_t height, size_t stride, std::ostream *__restrict log = nullptr);
	bool SaveUnformattedData(const char * __restrict path, const char * __restrict values, size_t size, std::ostream * __restrict log = nullptr);
	bool OpenFrom(const char *__restrict path, std::vector<char> &__restrict text, std::ostream *__restrict log = nullptr);
	/*
	bool loadFileIntoBufferString(const char *  path, BufferString &text);
	inline bool loadFileIntoBufferString(const std::string &str, BufferString &text)
	{
		loadFileIntoBufferString(str.c_str(), text);
	}*/

	std::string getCompFSCodeBegin();
	std::string getCompFSCodeEnd();
	std::string getCompVSCode();
	std::string getShowFSCode();
	std::string getShowVSCode();

};