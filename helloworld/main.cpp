#include <glad/glad.h>//GLAD���ڹ���OpenGL�ĺ���ָ��
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//�ص���������window��С���ı�ʱ������
	glViewport(0, 0, width, height);
}
float xOffset = 0;
float mix_level = 0;
void processInput(GLFWwindow *window) {
	//������루���Ƿ���ĳ����) ��Render Loop��ִ�У�Ϊ����ԣ�����һ������

	//Esc
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		xOffset += 0.005;
		if (xOffset > 1) xOffset = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mix_level += 0.005;
		if (mix_level >= 1) mix_level = 0;
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

	//�ĸ�����
	float vertices[] = {
		//λ��					//��ɫ				//��������
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f,1.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f,0.0f,
		 -0.5f, -0.5f, 0.0f,	0.0f, 0.0f,1.0f,	0.0f,0.0f,
		 -0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f,1.0f
	};

	int indices[] = {
		0,1,3,
		1,2,3
	};

	unsigned int VBO; //����id
	glGenBuffers(1, &VBO);//����һ��VBO����
	unsigned int VAO;	
	glGenVertexArrays(1, &VAO);//����һ��VAO
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);//��VAO


						   //�Ѷ������鸴�Ƶ�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//��֪OpenGL��ν�����Щ��������
	//�������������ԣ�λ��ֵ����ǰ��vertexShaderԴ����������locationΪ0�ˣ�,
	//	   �������ԵĴ�С��vec3),���ݵ����ͣ��Ƿ��׼����λ�������ڻ�������ʼλ�õ�Offset
	//λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


						   //����
						   //��������
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);//��

										  //Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//���ù��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_MIRRORED_REPEAT, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//���÷�ת
	stbi_set_flip_vertically_on_load(true);

	//����ͼƬ
	int width, height, nrChannels;
	unsigned char *data = stbi_load("..\\container.jpg", &width, &height, &nrChannels,0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture" << endl;
	}
	//�ͷ�ͼ���ڴ�
	stbi_image_free(data);
	
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);//��

										   //Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//����ͼƬ
	
	data = stbi_load("..\\moonRising.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture" << endl;
	}
	//�ͷ�ͼ���ڴ�
	stbi_image_free(data);

	shader.use();
	glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);
	shader.setInt("ourTexture2", 1);

	

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//��Ⱦѭ����Render Loop)
	while (!glfwWindowShouldClose(window)) {
		processInput(window);//������루��ť��

		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����
		
		//��������
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//�ı�uniform
		shader.setFloat("xOffset", xOffset);
		shader.setFloat("mix_level", mix_level);

		shader.use();//������Ⱦ����
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//������ɫ����
		glfwPollEvents();//����Ƿ��д����¼�
	}


	glfwTerminate();//�ͷ�֮ǰ�����������Դ
	return 0;
}
