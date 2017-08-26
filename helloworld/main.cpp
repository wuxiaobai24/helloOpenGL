#include <glad/glad.h>//GLAD���ڹ���OpenGL�ĺ���ָ��
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//�ص���������window��С���ı�ʱ������
	glViewport(0, 0, width, height);
}
float xOffset = 0;
void processInput(GLFWwindow *window) {
	//������루���Ƿ���ĳ����) ��Render Loop��ִ�У�Ϊ����ԣ�����һ������

	//Esc
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		xOffset += 0.01;
		if (xOffset > 1) xOffset = -1;
	}
}

//��������
GLFWwindow* initWindow() {
	glfwInit();	//��ʼ��GLFW
				//����GLFW�е�OpenGL�汾��Ϊ3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾��
												  //����ΪCore-Profile,��������ֻ��OpenGL���ܵ�һ���Ӽ�
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ���
	GLFWwindow* window =
		glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(-1);
	}
	//���õ�ǰ�̵߳���������
	glfwMakeContextCurrent(window);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Fialed to initialize GLAD" << endl;
		exit(-1);
	}

	//�����ӿ�
	glViewport(0, 0, 800, 600);

	//�Դ���ע��һ���ص��������ڴ��ڴ�С�仯ʱ������
	//�ڴ��ڵ�һ����ʾʱ��Ҳ�ᱻ����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

int main() {

	GLFWwindow *window = initWindow();

	Shader shader("..\\helloworld\\shader\\vertex.vs",
		"..\\helloworld\\shader\\fragment.fs");

	//��������
	float vertices[] = {
		//λ��				//��ɫ
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int VBO; //����id
	glGenBuffers(1, &VBO);//����һ��VBO����
	unsigned int VAO;	
	glGenVertexArrays(1, &VAO);//����һ��VAO
	glBindVertexArray(VAO);//��VAO

	//�Ѷ������鸴�Ƶ�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//��֪OpenGL��ν�����Щ��������
	//�������������ԣ�λ��ֵ����ǰ��vertexShaderԴ����������locationΪ0�ˣ�,
	//	   �������ԵĴ�С��vec3),���ݵ����ͣ��Ƿ��׼����λ�������ڻ�������ʼλ�õ�Offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//��Ⱦѭ����Render Loop)
	while (!glfwWindowShouldClose(window)) {
		processInput(window);//������루��ť��

		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����
		
		//��������
		shader.use();//������Ⱦ����
		
		//�ı�uniform
		shader.setFloat("xOffset", xOffset);
		//cout << xOffset << endl;
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//������ɫ����
		glfwPollEvents();//����Ƿ��д����¼�
	}


	glfwTerminate();//�ͷ�֮ǰ�����������Դ
	return 0;
}
