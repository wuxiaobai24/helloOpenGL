#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include<string>
#include<fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int Program;	//≥Ã–ÚID

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	
	void use();//º§ªÓ
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4fv(const std::string &name, glm::mat4);
private:
	
};

#endif