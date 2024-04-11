#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <iomanip>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "sstream"


class DatumTypeHelper
{
public:
	std::string mat4x4ToString(const glm::mat4x4& matrix);
	std::string vec4ToString(const glm::vec4& vector);
	glm::vec4 Str2Vec4(const std::string& string);
	glm::mat4x4 Str2Mat4x4(const std::string& string);
};

