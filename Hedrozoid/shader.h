#pragma once

#ifndef SHADER_H

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

typedef std::string string;

class Shader
{
public:
	// The program ID
	unsigned int ID;

	// Constructor reads and builds the shader
	Shader(string vertexPath, string fragmentPath);

	// Use/Activate the shader
	void use();

	// Utility uniform functions
	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
};

#endif // !SHADER_H