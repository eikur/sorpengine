#pragma once

#include <stdlib.h>
#include <vector>
#include <map>
#include <fstream>

class ShaderManager
{
	const std::string& kOutputFile = "shader_log.txt";

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

	bool init();
	int Load(const std::string& name, const std::string& vertex_shader, const std::string& fragment_shader);
	void CleanUp();

	int	GetUniformLocation(const std::string& program, const std::string& uniform) const;
	void UseProgram(const std::string& name);
	void UnuseProgram();

	static ShaderManager* GetInstance();

	void PrintErrorLog(const std::string& header, const std::vector<GLchar> &log_to_print) const;
};
