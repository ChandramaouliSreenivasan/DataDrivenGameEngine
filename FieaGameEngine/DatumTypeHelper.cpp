#include "pch.h"
#include "DatumTypeHelper.h"




//convert a glm matrix 4x4 into a std::string
std::string DatumTypeHelper::mat4x4ToString(const glm::mat4x4& matrix) {
    std::ostringstream oss;

    // Iterate through the matrix elements
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // Convert each element to a string and append to the result
            oss << matrix[i][j];

            // Add a comma and space unless it's the last element in the row
            if (j < 3) {
                oss << ",";
            }
        }

        // Add a newline character after each row
        oss << ",";
    }

    return oss.str();
}


//convert a glm vector to a std::string
std::string DatumTypeHelper::vec4ToString(const glm::vec4& vector) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(4);

    ss << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";

    return ss.str();
}

//convert a string to glm vector, essentially the reverse of the above described function
glm::vec4 DatumTypeHelper::Str2Vec4(const std::string& string) {
    std::string Str = string;
    Str.erase(std::remove(Str.begin(), Str.end(), '('), Str.end());
    Str.erase(std::remove(Str.begin(), Str.end(), ')'), Str.end());

    std::stringstream ss(Str);
    float x, y, z, w;
    char comma;
    ss >> x >> comma >> y >> comma >> z >> comma >> w;
    glm::vec4 Vector(x, y, z, w);
    return Vector;

}

glm::mat4x4 DatumTypeHelper::Str2Mat4x4(const std::string& string) {
    std::istringstream valueStream(string);
    glm::mat4x4 retVal;
    for (int i = 0; i < 16; i++) {
        std::string valstr;
        std::getline(valueStream, valstr, ',');
        float value;
        std::istringstream(valstr) >> value;
        retVal[i / 4][i % 4] = value;
    }
    return retVal;
}