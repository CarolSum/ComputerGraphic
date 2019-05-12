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

unsigned int planeVAO;
unsigned int cubeVAO;

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

void RenderScene(Shader &shader)
{
    // plane
    glm::mat4 model;
    shader.setMat4("model", glm::value_ptr(model));
    shader.setFloat3("objectColor", glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // cube
    model = glm::mat4();
    shader.setMat4("model", glm::value_ptr(model));
    shader.setFloat3("objectColor", glm::value_ptr(glm::vec3(0.384f, 0.749f, 0.678f)));
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HOMEWORK 7", NULL, NULL);
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
	Shader lampShader = Shader("Shader/basic.vs", "Shader/basic.fs");
	Shader shader = Shader("Shader/Phong.vs", "Shader/Phong.fs");
    Shader depthShader = Shader("Shader/depth_mapping.vs", "Shader/depth_mapping.fs");


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
	glGenVertexArrays(1, &planeVAO);
    glGenVertexArrays(1, &cubeVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 绑定VAO
	glBindVertexArray(cubeVAO);
	// 绑定cube顶点位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 绑定cube顶点法线属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    
    // 设置光线
    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 设置平面
    GLfloat planeVertices[] = {
        // 位置          // 法线       
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 
        -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f,
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f,
        25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f
    };

    // 绑定平面的VAO、VBO
    GLuint planeVBO;
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    // Configure depth map FBO
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // Create depth texture
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    // 创建2D纹理贴图
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 防止纹理贴图在远处重复渲染
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("shadowMap", 1);

    // render loop 控制变量
	// 光线位置
	glm::vec3 lightPos(1.8f, 4.0f, 0.7f);

    int mode = 0;

	// 摄像机参数
    float cameraPos[3] = { -0.4f, 5.4f, -3.5f };
    float lookAtCenter[3] = { 0.0f, 0.0f, 0.0f };
    float angle = 45.0f;
    float perspective[4] = { 5.0f, 5.0f , 0.1f, 100.0f };

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

        ImGui_ImplGlfwGL3_NewFrame();

        ImGui::Begin("Options");
        // 模式选择框
        ImGui::RadioButton("Projection Shading", &mode, 0);
        ImGui::RadioButton("Ortho Shading", &mode, 1);

        if (ImGui::CollapsingHeader("lighting options")) {
            ImGui::SliderFloat("lightPos.x", &lightPos.x, -5.0f, 5.0f, "X = %.1f");
            ImGui::SliderFloat("lightPos.y", &lightPos.y, -5.0f, 5.0f, "Y = %.1f");
            ImGui::SliderFloat("lightPos.z", &lightPos.z, -5.0f, 5.0f, "Z = %.1f");
        }

        if (ImGui::CollapsingHeader("Camera options")) {
            ImGui::SliderFloat("CamX", &cameraPos[0], -10.0f, 10.0f, "CamX = %.1f");
            ImGui::SliderFloat("CamY", &cameraPos[1], -10.0f, 10.0f, "CamY = %.1f");
            ImGui::SliderFloat("CamZ", &cameraPos[2], -10.0f, 10.0f, "CamZ = %.1f");
            ImGui::InputFloat3("Look at center", lookAtCenter, 2);
            ImGui::SliderFloat("Perspective angle", &angle, 0.0f, 360.0f, "angle = %.0f");
            ImGui::InputFloat4("Perspect paras", perspective, 2);
        }

        ImGui::End();


        // -------------------------------------------
        // 1. Render depth of scene to texture (from light's perspective)
        // - Get light projection/view matrix.
        glCullFace(GL_FRONT);
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        GLfloat near_plane = 1.0f, far_plane = 7.5f;
        if (mode == 1) {
            // 正交投影矩阵 
            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        }
        else {
            // 透射投影矩阵
           lightProjection = glm::perspective(124.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        }
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // - render scene from light's point of view
        depthShader.use();
        depthShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        RenderScene(depthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 设回原先的culling face
        glCullFace(GL_BACK);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 projection = glm::perspective(angle, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(
                glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]),
                glm::vec3(lookAtCenter[0], lookAtCenter[1], lookAtCenter[2]),
                glm::vec3(0, 1, 0)
            );
        shader.setMat4("projection", glm::value_ptr(projection));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setVec3("viewPos", glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]));
        shader.setVec3("lightPos", lightPos);
        shader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
        glActiveTexture(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        RenderScene(shader);

        // 渲染光源
        lampShader.use();
        lampShader.setMat4("projection", glm::value_ptr(projection));
        lampShader.setMat4("view", glm::value_ptr(view));
        glm::mat4 model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMat4("model", glm::value_ptr(model));
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// 交换缓冲区
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 释放VAO、VBO、EBO资源
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

	// 释放资源
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}