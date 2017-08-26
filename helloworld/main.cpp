#include <glad/glad.h>//GLAD用于管理OpenGL的函数指针
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//回调函数，在window大小被改变时，调用
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	//检查输入（即是否按下某个键) 在Render Loop中执行，为简洁性，独立一个函数

	//Esc
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//创建窗口
GLFWwindow* initWindow() {
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
	//设置当前线程的主上下文
	glfwMakeContextCurrent(window);

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

	GLFWwindow *window = initWindow();

	Shader shader("..\\helloworld\\shader\\vertex.vs",
		"..\\helloworld\\shader\\fragment.fs");

	//三个顶点
	float vertices[] = {
		//位置				//颜色
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int VBO; //对象id
	glGenBuffers(1, &VBO);//产生一个VBO对象
	unsigned int VAO;	
	glGenVertexArrays(1, &VAO);//创建一个VAO
	glBindVertexArray(VAO);//绑定VAO

	//把顶点数组复制到缓冲中
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//告知OpenGL如何解释这些顶点数据
	//参数：顶点属性（位置值，先前在vertexShader源码中设置了location为0了）,
	//	   顶点属性的大小（vec3),数据的类型，是否标准化，位置数据在缓冲中起始位置的Offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//渲染循环（Render Loop)
	while (!glfwWindowShouldClose(window)) {
		processInput(window);//检查输入（按钮）

		//渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲
		
		//绘制物体
		shader.use();//激活渲染程序

		float timeValue = (float)glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shader.Program, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//交换颜色缓冲
		glfwPollEvents();//检查是否有触发事件
	}


	glfwTerminate();//释放之前分配的所有资源
	return 0;
}
