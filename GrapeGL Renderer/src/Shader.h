#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader
{
public:

	Shader() : Name(""), VertexShader(""), FragmentShader(""), VertexID(0), FragmentID(0), ProgramID(0), FailedCompile(ShaderType::none) {};

	bool DidCompile();

	void Activate();
	void deActivate();

	std::string GetName();
	std::string GetVertexShader();
	std::string GetFragmentShader();


    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ProgramID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ProgramID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


	void Destroy();

	static bool LoadAll();
	static bool Load(std::string fileName);

	static Shader* GetLoadedShader(std::string name);
	static Shader* GetLoadedShader(int index);
	static std::vector<Shader*> GetAllShaders();

	static void DestroyAll();

	enum ShaderType
	{
		none = 0,
		Vertex = 1 << 1,
		Fragment = 1 << 2,
		Linking = 1 << 3,
		All = 7
	};
private:

	Shader(std::string vertexShader, std::string fragmentShader, std::string name, unsigned int vertexID, unsigned int fragmentID, unsigned int programID, ShaderType compile) : Name(name), VertexShader(vertexShader), FragmentShader(fragmentShader), VertexID(vertexID), FragmentID(fragmentID), ProgramID(programID), FailedCompile(compile) { }

	static std::vector<Shader> Shaders;

	std::string Name;
	std::string VertexShader;
	std::string FragmentShader;

	unsigned int VertexID;
	unsigned int FragmentID;
	unsigned int ProgramID;





	

	ShaderType FailedCompile;
};

