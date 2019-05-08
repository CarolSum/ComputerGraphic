#pragma once
//
//  shader.hpp
//  Glitter
//
//  Created by lijiehong on 2019/3/10.
//

#ifndef shader_hpp
#define shader_hpp

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	// ����ID
	unsigned int ID;
	Shader() {}
    Shader(const Shader& _shader) {
        ID = _shader.ID;
    }

	// ��������ȡ��������ɫ��
	Shader(const char* vertexPath, const char* fragmentPath);
	// ʹ��/�������
	void use();
	// uniform���ߺ���
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	 void setFloat3(const std::string &name, const float vec[]) const;
	void setFloat4(const std::string &name, const float vec[]) const;
	void setMat4(const std::string &name, const float vec[]) const;
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif /* shader_hpp */
