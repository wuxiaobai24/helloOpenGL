#include <glad/glad.h>//GLAD���ڹ���OpenGL�ĺ���ָ��
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//�ص���������window��С���ı�ʱ������
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	//������루���Ƿ���ĳ����) ��Render Loop��ִ�У�Ϊ����ԣ�����һ������

	//Esc
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//GLSL������ɫ����Դ����
const char *vertexShaderSource =
	"#version 330 core\n"	//�汾����
	"layout (location = 0) in vec3 aPos;\n" //in ���� ���룿
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"//���ö�����ɫ�������
	"}\n\0";

const char *fragmentShagerSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"//��out�����������
	"void main()\n"
	"{\n"
	"    FragColor = vec4(1.0f,0.5f,0.2f,1.0f); \n"//�����������ɫ
	"}\n\0";


int main() {
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
		return -1;
	}
	//���õ�ǰ�̵߳���������
	glfwMakeContextCurrent(window);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Fialed to initialize GLAD" << endl;
		return -1;
	}

	//�����ӿ�
	glViewport(0, 0, 800, 600);

	//�Դ���ע��һ���ص��������ڴ��ڴ�С�仯ʱ������
	//�ڴ��ڵ�һ����ʾʱ��Ҳ�ᱻ����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//��������
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO; //����id
	glGenBuffers(1, &VBO);//����һ��VBO����
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //������󶨵�GL_ARRAY_BUFFERĿ����
	//��֮ǰ����Ķ������ݸ��Ƶ�������ڴ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//������ɫ��
	unsigned int vertexShader;//����һ����ɫ������
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//����ɫ��Դ�루������һ��c�ַ����У����ӵ���ɫ�������в�����
	//��������ɫ������Դ���ַ���������ɫ��Դ�룬������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	//�������Ƿ�ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << endl;
	}

	//Ƭ����ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShagerSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << endl;
	}


	//��ɫ������
	//����һ���������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//����ɫ�����ӵ����������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//������ɫ��

	//�鿴�Ƿ�ɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
			<< infoLog << endl;
	}

	//���ӳɹ������ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	

	//����һ��VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//��VAO
	glBindVertexArray(VAO);

	//�Ѷ������鸴�Ƶ�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//��֪OpenGL��ν�����Щ��������
	//�������������ԣ�λ��ֵ����ǰ��vertexShaderԴ����������locationΪ0�ˣ�,
	//	   �������ԵĴ�С��vec3),���ݵ����ͣ��Ƿ��׼����λ�������ڻ�������ʼλ�õ�Offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//��Ⱦѭ����Render Loop)
	while (!glfwWindowShouldClose(window)) {
		processInput(window);//������루��ť��

		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����
		
		//��������
		glUseProgram(shaderProgram);//������Ⱦ����
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);



		glfwSwapBuffers(window);//������ɫ����
		glfwPollEvents();//����Ƿ��д����¼�
	}


	glfwTerminate();//�ͷ�֮ǰ�����������Դ
	return 0;
}
