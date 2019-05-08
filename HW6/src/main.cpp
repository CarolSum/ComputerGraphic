#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <vector>

#include "shader.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;


// 用户输入回调
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// 窗口大小变化的回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main() {
	// 初始化glfw窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HOMEWORK 5", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 启用深度检测
	glEnable(GL_DEPTH_TEST);

	// 初始化 shader 对象
	Shader gouraudShader = Shader("Shader/Gouraud.vs", "Shader/Gouraud.fs");
	Shader phongShader = Shader("Shader/Phong.vs", "Shader/Phong.fs");

	// 绑定ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	// 设置 GUI 风格
	ImGui::StyleColorsClassic();

	// 初始化变量
	ImVec4 clearColor = ImVec4(0.9764f, 0.9686f, 0.9098f, 1.0f);

	// 顶点数据
	float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };


	// 初始化VAO, VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// 绑定VAO
	glBindVertexArray(VAO);
	// 绑定顶点位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 绑定顶点法线属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    
    // 光照因子系数
    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    float shininess = 16;

	// lighting
	glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

    int mode = 0;
	// 控制光源移动变量
    bool isMovingLighting = false;

	// 摄像机参数
    float cameraPos[3] = { 2.0f, 1.2f, 5.0f };
    float perspect[4] = { 5.0f, 5.0f , 0.1f, 100.0f };

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Options");
		ImGui::Text("Click Btns To Show Diff Mode~");

        // 模式选择按钮
		ImGui::RadioButton("Gouraud Shading", &mode, 0);
        ImGui::RadioButton("Phong Shading", &mode, 1);
        ImGui::Checkbox("Lighting Moving", &isMovingLighting);

        if (ImGui::CollapsingHeader("lighting options")) {
			// 利用进度条控件控制各种参数
            ImGui::SliderFloat("lightPosition.x", &lightPosition.x, -5.0f, 5.0f, "X = %.1f");
            ImGui::SliderFloat("lightPosition.y", &lightPosition.y, -5.0f, 5.0f, "Y = %.1f");
            ImGui::SliderFloat("lightPosition.z", &lightPosition.z, -5.0f, 5.0f, "Z = %.1f");

            ImGui::SliderFloat("ambient strength", &ambientStrength, 0.0f, 1.0f, "ambient = %.1f");
            ImGui::SliderFloat("specular strength", &specularStrength, 0.0f, 1.0f, "specular = %.1f");
            ImGui::SliderFloat("shininess", &shininess, 0.0f, 128.0f, "shininess = %.0f");
        }
		ImGui::End();


		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(0);

		Shader lightingShader;
        if (mode == 0) {
            lightingShader = gouraudShader;
        } else {
            lightingShader = phongShader;
        }

		// 调用shader并设置相应的参数
        lightingShader.use();
        lightingShader.setFloat3("objectColor", glm::value_ptr(glm::vec3(0.384f, 0.749f, 0.678f)));
        lightingShader.setFloat3("lightColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        lightingShader.setFloat("ambientStrength", ambientStrength);
        lightingShader.setFloat("specularStrength", specularStrength);
        lightingShader.setInt("shininess", shininess);


        // view、projection 变换
		glm::mat4 view = glm::lookAt(
            glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0, 1, 0)
        );
        glm::mat4 projection = glm::perspective(45.0f, (float)perspect[0] / (float)perspect[1], perspect[2], perspect[3]);
       
        lightingShader.setMat4("view", glm::value_ptr(view));
        lightingShader.setMat4("projection", glm::value_ptr(projection));
        lightingShader.setFloat3("viewPos", glm::value_ptr(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2])));

        if (isMovingLighting) {
            lightPosition.x = 0.5f + abs(sin(glfwGetTime())) * 1.0f;
            lightPosition.y = abs(sin(glfwGetTime() / 2.0f)) * 1.0f;
        }
        lightingShader.setFloat3("lightPos", glm::value_ptr(lightPosition));
 
        // model变换
        glm::mat4 model;
        lightingShader.setMat4("model", glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

		// Imgui render
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// 交换缓冲区
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 释放VAO、VBO、EBO资源
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// 释放资源
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}