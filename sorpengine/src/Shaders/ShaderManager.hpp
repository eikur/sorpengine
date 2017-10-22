#pragma once

#include <stdlib.h>
#include <vector>
#include <map>
#include <fstream>

class ShaderManager
{
	const char* output_file = "shader_log.txt";

	struct LessString
	{
		bool operator()(const std::string left, const std::string right) const
		{
			return ::strcmp(left.c_str(), right.c_str()) < 0;
		}
	};

	typedef std::map<std::string, int, LessString> ProgramList;

	ProgramList programs;
	static std::auto_ptr<ShaderManager> instance;

public:
	ShaderManager() {}
	~ShaderManager() = default;

	int Load(const char* name, const char* vertex_shader, const char* fragment_shader);
	void CleanUp();

	int	GetUniformLocation(const char* name, const char* uniform);
	void UseProgram(const char* name);
	void UnuseProgram();

	static ShaderManager* GetInstance();

	void PrintErrorLog(const char* header, const std::vector<GLchar> &log_to_print) const;
};
