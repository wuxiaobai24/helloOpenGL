#include <glad/glad.h>//GLAD用于管理OpenGL的函数指针
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 800

using namespace std;

//用于设置uniform的全局变量
float xOffset = 0;
float mix_level = 0;
float vOffset = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//回调函数，在window大小被改变时，调用
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	//检查输入（即是否按下某个键) 在Render Loop中执行，为简洁性，独立一个函数
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//Esc
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) { //C，平移，向右
		xOffset += 0.005;
		if (xOffset > 1) xOffset = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // A, mix_level
		mix_level += 0.001;
		if (mix_level >= 1) mix_level = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) { // A, mix_level
		vOffset += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) { // A, mix_level
		vOffset -= 0.01;
	}
}

//创建窗口
GLFWwindow* initWindow() {
	//testGlm();

	glfwInit();	//初始化GLFW
	//配置GLFW中的OpenGL版本号为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
	//设置为Core-Profile,这样我们只能OpenGL功能的一个子集
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口对象
	GLFWwindow* window =
		glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);//设置当前线程的主上下文
	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Fialed to initialize GLAD" << endl;
		exit(-1);
	}
	//设置视口
	glViewport(0, 0, 800, 600);
	//对窗口注册一个回调函数，在窗口大小变化时被调用
	//在窗口第一次显示时，也会被调用
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

int main() {
	GLFWwindow *window = initWindow();//创建窗口
	//创建shader
	Shader shader("..\\helloworld\\shader\\vertex.vs",
		"..\\helloworld\\shader\\fragment.fs");
	/*
	float vertices[] = {	//四个顶点
		//位置					//颜色				//纹理坐标
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f,1.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f,0.0f,
		 -0.5f, -0.5f, 0.0f,	0.0f, 0.0f,1.0f,	0.0f,0.0f,
		 -0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f,1.0f,
	};

	*/
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	int indices[] = {
		0,1,3,
		1,2,3,
	};

	unsigned int VBO,VAO,EBO; //对象id
	glGenBuffers(1, &VBO);//产生一个VBO对象	
	glGenVertexArrays(1, &VAO);//创建一个VAO
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);//绑定VAO
						   
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//把顶点数组复制到缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//把indices复制到缓冲中
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//告知OpenGL如何解释这些顶点数据
	//参数：顶点属性（位置值，先前在vertexShader源码中设置了location为0了）,
	//	   顶点属性的大小（vec3),数据的类型，是否标准化，位置数据在缓冲中起始位置的Offset
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);//位置
	glEnableVertexAttribArray(0);//已在shader中设置location
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));//颜色
	glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//纹理
	//glEnableVertexAttribArray(2);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);//绑定		  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//为当前绑定的纹理对象设置环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_MIRRORED_REPEAT, GL_NEAREST);//设置过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//加载图片
	stbi_set_flip_vertically_on_load(true);//设置翻转
	int width, height, nrChannels;
	unsigned char *data = stbi_load("..\\container.jpg", &width, &height, &nrChannels,0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture" << endl;
	}
	//释放图像内存
	stbi_image_free(data);
	
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);//绑定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	data = stbi_load("..\\moonRising.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);

	shader.use();
	glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);
	shader.setInt("ourTexture2", 1);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);	//开启深度测试
	while (!glfwWindowShouldClose(window)) {//渲染循环（Render Loop)
		processInput(window);//检查输入（按钮）

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空颜色缓冲
		//绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//改变uniform
		shader.setFloat("xOffset", xOffset);
		shader.setFloat("mix_level", mix_level);
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++) {
			//glm::mat4 trans;
			glm::mat4 model;//模型矩阵
			glm::mat4 view;//观察矩阵
			glm::mat4 projection;//投影矩阵
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			view = glm::translate(view, glm::vec3(vOffset, 0.0f, -4.0f));
			projection =
				glm::perspective(glm::radians(45.0f), float(SCREENWIDTH) / float(SCREENHEIGHT),
					0.10f, 100.0f);


			shader.setMat4fv("model", model);
			shader.setMat4fv("view", view);
			shader.setMat4fv("projection", projection);

			//glBindVertexArray(VAO);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
			glfwSwapBuffers(window);//交换颜色缓冲
			glfwPollEvents();//检查是否有触发事件
	}


	glfwTerminate();//释放之前分配的所有资源
	return 0;
}
