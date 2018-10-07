#pragma once

#include <stdlib.h>
#include <vector>
#include <map>
#include <fstream>
#include <memory>

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
	int load(const std::string& name, const std::string& vertex_shader, const std::string& fragment_shader);
	void finalize();

	int	getUniformLocation(const std::string& program, const std::string& uniform) const;
	void useProgram(const std::string& name);
	void unuseProgram();

	static ShaderManager* getInstance();

	void printErrorLog(const std::string& header, const std::vector<GLchar> &log_to_print) const;
};
