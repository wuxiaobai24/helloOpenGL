#include "shader.h"
using namespace std;

Shader::Shader(const GLchar* vertexPath, const GLchar*fragmentPath) {
	
	//���ļ��ж�ȡ������ɫ����Ƭ����ɫ��
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//������ɫ��
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FRILED\n" << infoLog << endl;
	}

	//��ɫ������
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);

	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << endl;
	}
	//ɾ����ɫ��
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}


void Shader::use() {
	glUseProgram(this->Program);
}

void Shader::setBool(const string &name, bool value) const {
	glUniform1i(glGetUniformLocation(this->Program, name.c_str()), (int)value);
}

void Shader::setInt(const string& name, int value) const {
	glUniform1i(glGetUniformLocation(this->Program, name.c_str()), (int)value);
}

void Shader::setFloat(const string& name, float value) const {
	glUniform1f(glGetUniformLocation(this->Program, name.c_str()), (float)value);
}