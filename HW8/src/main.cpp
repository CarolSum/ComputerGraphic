#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "shader.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

using namespace std;

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

vector<glm::vec3> p;
vector<glm::vec3>::iterator currPointIter;
bool isLeftButtonPressed = false;

// 将数据归一化到[-1, 1]
auto glfwPos2nocPos = [](const glm::vec3 p) -> glm::vec3 {
    glm::vec3 res;
    res.x = (2 * p.x) / SCR_WIDTH - 1;
    res.y = 1 - (2 * p.y) / SCR_HEIGHT;
    res.z = 0.0f;
    return res;
};


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

// 查找点击范围是否有控制点
vector<glm::vec3>::iterator ifClickedControlPoint(float xpos, float ypos, const float threshold) {
    vector<glm::vec3>::iterator res = p.end();

    auto dist = [&xpos, &ypos](const vector<glm::vec3>::iterator iter) -> float {
        return pow((xpos - iter->x), 2) + pow((ypos - iter->y), 2);
    };
    for (auto iter = p.begin(); iter != p.end(); ++iter) {
        auto dis = dist(iter);
        if (dis < threshold) {
            if (res == p.end()) { res = iter; }
            else { res = (dist(res) < dis) ? res : iter; }
        }
    }

    return res;
}

void addPoint(const float xpos, const float ypos) {
    if ( ifClickedControlPoint(xpos, ypos, 0.01) == p.end()) {
        p.push_back(glm::vec3(xpos, ypos, 0.0f));
    }
}


bool isPointInVector(const float xpos, const float ypos) {
    return find(p.begin(), p.end(), glm::vec3(xpos, ypos, 0.0f)) != p.end();
}

// 鼠标点击回调
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec3 mousePos = glfwPos2nocPos(glm::vec3(xpos, ypos, 0.0f));

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            // 点击左键判断是否已有控制点
            isLeftButtonPressed = true;
            if (!isPointInVector(mousePos.x, mousePos.y)) {
                // 若没有则新增一个控制点
                addPoint(mousePos.x, mousePos.y);
            }
        }

        if (action == GLFW_RELEASE) {
            currPointIter = p.end();
            isLeftButtonPressed = false;
        }
    }

    // 点击右键时删除最后一个控制点
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (p.size() > 0) {
            p.pop_back();
        }
    }
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HOMEWORK 8", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// 初始化 shader 对象
	Shader pointShader = Shader("Shader/point.vs", "Shader/point.fs");
	Shader curveShader = Shader("Shader/curve.vs", "Shader/curve.fs");

	// 初始化变量
	ImVec4 clearColor = ImVec4(0.9764f, 0.9686f, 0.9098f, 1.0f);

    // 生成顶点数据 t
    float step = 0.001;
    vector<float> data;
    data.resize(int(1 / step));
    for (int i = 0; i < data.size(); ++i) {
        data[i] = i * step;
    }

	// 初始化VAO, VBO
    unsigned int VAO;
	unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

	// 绑定曲线的顶点属性 t  
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    p.clear();
    currPointIter = p.end();

   
    float pointColor[3] = { 1.0f, 0.5f, 0.2f };
    GLuint pVAO, pVBO;
    glGenVertexArrays(1, &pVAO);
    glGenBuffers(1, &pVBO);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
        glfwPollEvents();
       
        int scr_width, scr_height;
        glfwGetFramebufferSize(window, &scr_width, &scr_height);
        glViewport(0, 0, scr_width, scr_height);
      
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

        if (isLeftButtonPressed) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
			glm::vec3 mousePos = glfwPos2nocPos(glm::vec3(xpos, ypos, 0.0f));

            // 若鼠标点中位置有控制点，则调整该控制点的坐标
            currPointIter = ifClickedControlPoint(mousePos.x, mousePos.y, 0.01);
            if (currPointIter != p.end())
                *currPointIter = glfwPos2nocPos(glm::vec3(xpos, ypos, 0.0f));
        }

        glBindVertexArray(pVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);

        auto controlPoints2dataVector = []() -> vector<GLfloat> {
            vector<GLfloat> res;
            res.clear();
            for (int i = 0; i < p.size(); ++i) {
                res.push_back(p[i].x);
                res.push_back(p[i].y);
                res.push_back(p[i].z);
            }
            return res;
        };

        // 将屏幕坐标记录的控制点渲染出来
        auto pointData = controlPoints2dataVector();
        glBufferData(GL_ARRAY_BUFFER, pointData.size() * sizeof(GLfloat), pointData.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        pointShader.use();
        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 0, pointData.size() / 3);
        glBindVertexArray(0);

        // 渲染贝塞尔曲线
		if (p.size() > 0) {
			curveShader.use();
            curveShader.setInt("n", p.size());
			curveShader.set3FloatVec("p", p);
			curveShader.setFloat3("curveColor", pointColor);
			glBindVertexArray(VAO);
			glPointSize(2.0f);
			glDrawArrays(GL_POINTS, 0, data.size());
			glBindVertexArray(0);
		}

		// 交换缓冲区
		glfwSwapBuffers(window);
	}

	// 释放VAO、VBO、EBO资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &pVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &pVBO);

	// 释放资源
	glfwTerminate();
	return 0;
}