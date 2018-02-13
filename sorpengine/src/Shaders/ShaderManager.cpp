#include "Globals.hpp"
#include "GL/glew.h"
#include "ShaderManager.hpp"
#include "Utils.hpp"

bool ShaderManager::init()
{
	// to be done when reading from config / custom shaders used
	int program = Load("test1", "resources/shaders/test1.vsh", "resources/shaders/test1.fsh");
	return program != 0;
}

int ShaderManager::Load(const std::string& name, const std::string& vertex_shader_name, const std::string& fragment_shader_name)
{
	// GLSL shader examples in www.khronos.org/opengl/wiki/Example_Code
	std::map<std::string, int, LessString>::iterator it;
	it = programs.find(name);
	if (it != programs.end())
	{
		return (*it).second;
	}

	GLuint vertex_shader;
	GLuint fragment_shader;
	FILE* fp;
	GLint size;
	char *buffer;
	GLint result;

	GLuint program = glCreateProgram();

	if (!vertex_shader_name.empty())
	{
		fp = fopen(vertex_shader_name.c_str(), "r");
		if (fp == nullptr) {
			Utils::log("Error opening file: %s\n", strerror(errno));
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			size = ftell(fp) + 1;
			buffer = (char*)malloc(size);
			memset(buffer, 0, size);
			rewind(fp);
			fread(buffer, size, 1, fp);
			buffer[size - 1] = '\0';

			fclose(fp);

			vertex_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_shader, 1, &buffer, &size);
			glCompileShader(vertex_shader);
			free(buffer);


			result = 0;
			glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				// error display management
				GLint maxLength = 0;
				glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(vertex_shader, maxLength, &maxLength, &errorLog[0]);
				PrintErrorLog("Vertex Shader Compilation Failed", errorLog);
			}
			else
			{
				glAttachShader(program, vertex_shader);
			}
			glDeleteShader(vertex_shader);
		}
	}

	if (!fragment_shader_name.empty())
	{
		fp = fopen(fragment_shader_name.c_str(), "r");
		if (fp == nullptr) {
			Utils::log("Error opening file: %s\n", strerror(errno));
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			size = ftell(fp) + 1;
			buffer = (char*)malloc(size);
			memset(buffer, 0, size);

			rewind(fp);
			fread(buffer, size - 1, 1, fp);
			buffer[size - 1] = '\0';
			fclose(fp);

			fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment_shader, 1, &buffer, &size);
			glCompileShader(fragment_shader);
			free(buffer);

			result = 0;
			glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				// error display management
				GLint maxLength = 0;
				glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &errorLog[0]);
				PrintErrorLog("Fragment Shader Compilation Failed", errorLog);
			}
			else
			{
				glAttachShader(program, fragment_shader);
			}
			glDeleteShader(fragment_shader);
		}
	}

	glLinkProgram(program);

	result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		//The program is useless now. So delete it.
		glDeleteProgram(program);

		//Provide the infolog in whatever manner you deem best.
		PrintErrorLog("Program link failed", infoLog);
		//Exit with failure.
		return -1;
	}

	programs[name] = program;
	return program;
}

void ShaderManager::CleanUp() {}

int	ShaderManager::GetUniformLocation(const std::string& program, const std::string& uniform) const
{
	auto it = programs.find(program.c_str());
	if (it == programs.end())
	{
		return 0;
	}
	return glGetUniformLocation((*it).second, uniform.c_str());
}

void ShaderManager::UseProgram(const std::string& name) 
{
	auto it = programs.find(name.c_str());
	if (it == programs.end())
	{
		return;
	}
	glUseProgram((*it).second);
}


void ShaderManager::UnuseProgram() 
{
	glUseProgram(0);
}

ShaderManager* ShaderManager::GetInstance()
{
	return nullptr;
}

void ShaderManager::PrintErrorLog(const std::string& header, const std::vector<GLchar> &log_to_print) const
{
	std::ofstream output;
	output.open(kOutputFile, std::ios::out | std::ios::trunc);
	output << "------------------------------------" << std::endl;
	output << header.c_str() << std::endl;
	output << "------------------------------------" << std::endl;
	for (std::vector<GLchar>::const_iterator it = log_to_print.cbegin(); it != log_to_print.cend(); ++it)
		output << (*it);
	output << std::endl << std::endl;
	output.close();
}