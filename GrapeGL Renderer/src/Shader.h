#pragma once
#include <string>
#include <vector>
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

