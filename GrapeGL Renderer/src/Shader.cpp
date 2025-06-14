#include "Shader.h"
#include <fstream>
#include <filesystem>
#include "Common.h"

std::vector<Shader> Shader::Shaders;

bool Shader::DidCompile()
{
	return FailedCompile == ShaderType::none;
}

void Shader::Activate()
{
	if (DidCompile())
		glUseProgram(ProgramID);
}

void Shader::deActivate()
{
	glUseProgram(0);
}

std::string Shader::GetName() { return Name; }

std::string Shader::GetVertexShader() { return VertexShader; }

std::string Shader::GetFragmentShader() { return FragmentShader; }

void Shader::Destroy()
{
	if (DidCompile())
		glDeleteProgram(ProgramID);
}

bool Shader::LoadAll()
{
	Shaders = std::vector<Shader>();

	namespace fs = std::filesystem;
	fs::path current_path = std::filesystem::current_path();

	for (const auto& entry : fs::directory_iterator(current_path))
	{
		if (fs::is_regular_file(entry))
		{ 
			Log(entry.path().string());
			Log(entry.path().extension().string());
			Load(entry.path().string());
		}
	}
	return true;
}

bool Shader::Load(std::string fileName)
{
	std::filesystem::path path = fileName;
	if (path.extension().string() != ".glsl")
		return false;
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file.is_open())
	{
		return false; 
		//TODO: Log Error
	}


	std::string vertexShader;
	std::string fragmentShader;

	std::string line;
	ShaderType type = ShaderType::none;
	while (std::getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::Vertex;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::Fragment;
			}

			continue;
		}

		if (type == ShaderType::Vertex)
			vertexShader += "\n" + line;
		else if (type == ShaderType::Fragment)
			fragmentShader += "\n" + line;
	}
	const char* vShaderCode = vertexShader.c_str();
	const char* fShaderCode = fragmentShader.c_str();

	ShaderType shaderError = ShaderType::none;

	unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vShaderCode, NULL);
	glCompileShader(vertexID);

	GLint success;
	GLchar infoLog[1024];
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexID, 1024, NULL, infoLog);
		//shaderError ^= ShaderType::Vertex;
		Error(infoLog);
	}

	unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fShaderCode, NULL);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentID, 1024, NULL, infoLog);
		//shaderError ^= ShaderType::Fragment;
		Error(infoLog);
	}

	unsigned int ID = glCreateProgram();
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	glLinkProgram(ID);

	glGetShaderiv(vertexID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexID, 1024, NULL, infoLog);
		//shaderError ^= ShaderType::Linking;
		Error(infoLog);
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);


	std::string filename = path.filename().string();

	Shader shader = Shader(vertexShader, fragmentShader, filename, vertexID, fragmentID, ID, shaderError);
	Shaders.push_back(shader);
	return shaderError == ShaderType::none;
}

Shader* Shader::GetLoadedShader(std::string name)
{
	return nullptr;
}

Shader* Shader::GetLoadedShader(int index)
{
	if (index < Shaders.size())
		return &Shaders[index];
	return nullptr;
}

std::vector<Shader*> Shader::GetAllShaders()
{
	std::vector<Shader*> output;
	for(int i = 0; i < Shaders.size(); i++)
	{
		output.push_back(&Shaders[i]);
	}
	return output;
}

void Shader::DestroyAll()
{
	for (Shader shader : Shaders)
	{
		shader.Destroy();
	}
}


inline Shader::ShaderType operator|(Shader::ShaderType lhs, Shader::ShaderType rhs)
{
	using T = std::underlying_type_t<Shader::ShaderType>;
	return static_cast<Shader::ShaderType>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline Shader::ShaderType operator&(Shader::ShaderType lhs, Shader::ShaderType rhs)
{
	using T = std::underlying_type_t<Shader::ShaderType>;
	return static_cast<Shader::ShaderType>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline Shader::ShaderType operator^(Shader::ShaderType lhs, Shader::ShaderType rhs)
{
	using T = std::underlying_type_t<Shader::ShaderType>;
	return static_cast<Shader::ShaderType>(static_cast<T>(lhs) ^ static_cast<T>(rhs));
}


inline Shader::ShaderType& operator|=(Shader::ShaderType& lhs, Shader::ShaderType rhs)
{
	lhs = lhs | rhs;
	return lhs;
}

inline Shader::ShaderType& operator&=(Shader::ShaderType& lhs, Shader::ShaderType rhs)
{
	lhs = lhs & rhs;
	return lhs;
}

inline Shader::ShaderType& operator^=(Shader::ShaderType& lhs, Shader::ShaderType rhs)
{
	lhs = lhs ^ rhs;
	return lhs;
}
