#include "../include/stypox/gl/shader.h"

namespace stypox::gl {
	using Tstr = std::string;

	void Shader::create() {
		remove();
		m_programId = glCreateProgram();
		m_vertexShId = glCreateShader(GL_VERTEX_SHADER);
		m_fragmentShId = glCreateShader(GL_FRAGMENT_SHADER);
		m_idsCreated = true;
	}

	Shader::Shader() {}
	Shader::Shader(const std::filesystem::path& vertexShFilename, const std::filesystem::path& fragmentShFilename) {
		compileFile(vertexShFilename, fragmentShFilename);
	}
	Shader::~Shader() {
		remove();
	}

	Shader::Shader(Shader&& other) :
		m_vertexShId{other.m_vertexShId},
		m_fragmentShId{other.m_fragmentShId},
		m_programId{other.m_programId},
		m_fileLog{std::move(other.m_fileLog)},
		m_idsCreated{other.m_idsCreated} {
		other.m_idsCreated = false;
	}
	Shader& Shader::operator= (Shader&& other) {
		m_fileLog = std::move(other.m_fileLog);
		remove();
		if (other.m_idsCreated) {
			m_vertexShId = other.m_vertexShId;
			m_fragmentShId = other.m_fragmentShId;
			m_programId = other.m_programId;
			m_idsCreated = true;
			other.m_idsCreated = false;
		}
		return *this;
	}
	
	void Shader::remove() {
		if (m_idsCreated) {
			glDeleteProgram(m_programId);
			glDeleteShader(m_vertexShId);
			glDeleteShader(m_fragmentShId);
			m_idsCreated = false;
		}
	}
	
	void Shader::compileFile(const std::filesystem::path& vertexShFilename, const std::filesystem::path& fragmentShFilename) {
		m_fileLog = "";

		File vertexFile{vertexShFilename},
			fragmentFile{fragmentShFilename};
		Tstr vertexSource{vertexFile.str()},
			fragmentSource{fragmentFile.str()};

		if (!vertexFile.isOpen()) {
			m_fileLog += "Unable to read vertex shader source code from \"" + vertexShFilename.string() + "\"; ";
		}
		if (!fragmentFile.isOpen()) {
			m_fileLog += "Unable to read fragment shader source code from \"" + fragmentShFilename.string() + "\"; ";
		}

		compileSource(vertexSource, fragmentSource);
	}
	void Shader::compileSource(const Tstr& vertexShSource, const Tstr& fragmentShSource) {
		create();

		const char * vertexShCStr = vertexShSource.c_str(),
			* fragmentShCStr = fragmentShSource.c_str();
		glShaderSource(m_vertexShId, 1, &vertexShCStr, NULL); //TODO
		glShaderSource(m_fragmentShId, 1, &fragmentShCStr, NULL);

		glCompileShader(m_vertexShId);
		glCompileShader(m_fragmentShId);

		glAttachShader(m_programId, m_vertexShId);
		glAttachShader(m_programId, m_fragmentShId);
		glLinkProgram(m_programId);
	}

	Shader::Step Shader::errors() {
		Shader::Step err{0};

		GLint vertexStatus, fragmentStatus, programStatus;
		glGetShaderiv(m_vertexShId, GL_COMPILE_STATUS, &vertexStatus);
		glGetShaderiv(m_fragmentShId, GL_COMPILE_STATUS, &fragmentStatus);
		glGetProgramiv(m_programId, GL_LINK_STATUS, &programStatus);

		if (!m_fileLog.empty())
			err |= file;
		if (vertexStatus == 0)
			err |= vertex;
		if (fragmentStatus == 0)
			err |= fragment;
		if (programStatus == 0)
			err |= program;
		return err;
	}
	Tstr Shader::getLog(Step step) {
		switch (step) {
		case file:
			return m_fileLog;
		case vertex: {
			char infoLog[maxInfoLogCharacters];
			glGetShaderInfoLog(m_vertexShId, maxInfoLogCharacters, nullptr, infoLog);
			return infoLog;
		}
		case fragment: {
			char infoLog[maxInfoLogCharacters];
			glGetShaderInfoLog(m_fragmentShId, maxInfoLogCharacters, nullptr, infoLog);
			return infoLog;
		}
		case program: {
			char infoLog[maxInfoLogCharacters];
			glGetProgramInfoLog(m_programId, maxInfoLogCharacters, nullptr, infoLog);
			return infoLog;
		}
		default:
			return "";
		}
	}
	Tstr Shader::debugInfo(const Tstr& name) {
		Tstr info{"Shader status id=" + std::to_string(m_programId) + (name.empty() ? "" : ",name=" + name) + ": "};
		
		if (Step status{errors()}; status) {
			info += "errors\n";
			if (status & file)
				info += "File: " + getLog(file) + ";\n";
			if (status & vertex)
				info += "Vertex: " + getLog(vertex) + ";\n";
			if (status & fragment)
				info += "Fragment: " + getLog(fragment) + ";\n";
			if (status & program)
				info += "Program: " + getLog(program) + ";\n";
		}
		else {
			info += "ok";
		}

		return info;
	}

	GLint Shader::getAttribLocation(const Tstr& attributeVariableName) {
		return glGetAttribLocation(m_programId, attributeVariableName.c_str());
	}


	//float
	template<> void Shader::uniform(const Tstr& uniformName, const GLfloat& v0) {
		bind();
		glUniform1f(glGetUniformLocation(m_programId, uniformName.c_str()), v0);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLfloat& v0, const GLfloat& v1) {
		bind();
		glUniform2f(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLfloat& v0, const GLfloat& v1, const GLfloat& v2) {
		bind();
		glUniform3f(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1, v2);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLfloat& v0, const GLfloat& v1, const GLfloat& v2, const GLfloat& v3) {
		bind();
		glUniform4f(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1, v2, v3);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const std::initializer_list<GLfloat>& v0) {
		bind();
		glUniform1fv(glGetUniformLocation(m_programId, uniformName.c_str()), v0.size(), v0.begin());
	}

	//int
	template<> void Shader::uniform(const Tstr& uniformName, const GLint& v0) {
		bind();
		glUniform1i(glGetUniformLocation(m_programId, uniformName.c_str()), v0);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLint& v0, const GLint& v1) {
		bind();
		glUniform2i(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLint& v0, const GLint& v1, const GLint& v2) {
		bind();
		glUniform3i(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1, v2);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLint& v0, const GLint& v1, const GLint& v2, const GLint& v3) {
		bind();
		glUniform4i(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1, v2, v3);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const std::initializer_list<GLint>& v0) {
		bind();
		glUniform1iv(glGetUniformLocation(m_programId, uniformName.c_str()), v0.size(), v0.begin());
	}

	//unsigned int
	template<> void Shader::uniform(const Tstr& uniformName, const GLuint& v0) {
		bind();
		glUniform1ui(glGetUniformLocation(m_programId, uniformName.c_str()), v0);	
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLuint& v0, const GLuint& v1) {
		bind();
		glUniform2ui(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLuint& v0, const GLuint& v1, const GLuint& v2) {
		bind();
		glUniform3ui(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1, v2);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const GLuint& v0, const GLuint& v1, const GLuint& v2, const GLuint& v3) {
		bind();
		glUniform4ui(glGetUniformLocation(m_programId, uniformName.c_str()), v0, v1, v2, v3);
	}
	template<> void Shader::uniform(const Tstr& uniformName, const std::initializer_list<GLuint>& v0) {
		bind();
		glUniform1uiv(glGetUniformLocation(m_programId, uniformName.c_str()), v0.size(), v0.begin());
	}

	//matrices (transpose disabled by default)
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat2& value) {
		bind();
		glUniformMatrix2fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat3& value) {
		bind();
		glUniformMatrix3fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat4& value) {
		bind();
		glUniformMatrix4fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat2x3& value) {
		bind();
		glUniformMatrix2x3fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat3x2& value) {
		bind();
		glUniformMatrix3x2fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat2x4& value) {
		bind();
		glUniformMatrix2x4fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat4x2& value) {
		bind();
		glUniformMatrix4x2fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat3x4& value) {
		bind();
		glUniformMatrix3x4fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat4x3& value) {
		bind();
		glUniformMatrix4x3fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, glm::value_ptr(value));
	}

	//matrices (transpose chosen manually)
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat2& value, GLboolean transpose) {
		bind();
		glUniformMatrix2fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat3& value, GLboolean transpose) {
		bind();
		glUniformMatrix3fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat4& value, GLboolean transpose) {
		bind();
		glUniformMatrix4fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat2x3& value, GLboolean transpose) {
		bind();
		glUniformMatrix2x3fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat3x2& value, GLboolean transpose) {
		bind();
		glUniformMatrix3x2fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat2x4& value, GLboolean transpose) {
		bind();
		glUniformMatrix2x4fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat4x2& value, GLboolean transpose) {
		bind();
		glUniformMatrix4x2fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat3x4& value, GLboolean transpose) {
		bind();
		glUniformMatrix3x4fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}
	template<> void Shader::uniform(const Tstr& uniformName, const glm::mat4x3& value, GLboolean transpose) {
		bind();
		glUniformMatrix4x3fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	}

	//single values
	template<> GLfloat Shader::getUniform(const Tstr& uniformName) {
		GLfloat uniform;
		glGetUniformfv(m_programId, glGetUniformLocation(m_programId, uniformName.c_str()), &uniform);
		return uniform;
	}
	template<> GLint Shader::getUniform(const Tstr& uniformName) {
		GLint uniform;
		glGetUniformiv(m_programId, glGetUniformLocation(m_programId, uniformName.c_str()), &uniform);
		return uniform;
	}
	template<> GLuint Shader::getUniform(const Tstr& uniformName) {
		GLuint uniform;
		glGetUniformuiv(m_programId, glGetUniformLocation(m_programId, uniformName.c_str()), &uniform);
		return uniform;
	}

	//GLfloat
	template<typename T, int N>
		typename std::enable_if<std::is_same<T, GLfloat>::value, std::array<T, N>>::type
		Shader::getUniform(const Tstr& uniformName)
	{
		static_assert(N > 0, "getUniform requires a number of values grater than 0");
		std::array<T, N> uniform;
		glGetUniformfv(m_programId, glGetUniformLocation(m_programId, uniformName.c_str()), uniform.data());
		return uniform;
	}
	//GLint
	template<typename T, int N>
		typename std::enable_if<std::is_same<T, GLint>::value, std::array<T, N>>::type
		Shader::getUniform(const Tstr& uniformName)
	{
		static_assert(N > 0, "getUniform requires a number of values grater than 0");
		std::array<T, N> uniform;
		glGetUniformiv(m_programId, glGetUniformLocation(m_programId, uniformName.c_str()), uniform.data());
		return uniform;
	}
	//GLuint
	template<typename T, int N>
		typename std::enable_if<std::is_same<T, GLuint>::value, std::array<T, N>>::type
		Shader::getUniform(const Tstr& uniformName)
	{
		static_assert(N > 0, "getUniform requires a number of values grater than 0");
		std::array<T, N> uniform;
		glGetUniformuiv(m_programId, glGetUniformLocation(m_programId, uniformName.c_str()), uniform.data());
		return uniform;
	}
	//neither GLfloat, GLint nor GLuint (error)
	template<typename T, int N>
		typename std::enable_if<!std::is_same<T, GLfloat>::value && !std::is_same<T, GLint>::value && !std::is_same<T, GLuint>::value, void>::type
		Shader::getUniform(const Tstr& uniformName)
	{
		static_assert(N > 0, "getUniform requires a number of values grater than 0");
		static_assert(std::is_same<T, GLfloat>::value || std::is_same<T, GLint>::value || std::is_same<T, GLuint>::value, "getUniform type must be either GLfloat, GLint or GLuint");
	}

	template<> glm::mat2 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat2(getUniform<GLfloat, 4>(uniformName).data());
	}
	template<> glm::mat3 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat3(getUniform<GLfloat, 9>(uniformName).data());
	}
	template<> glm::mat4 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat4(getUniform<GLfloat, 16>(uniformName).data());
	}
	template<> glm::mat2x3 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat2x3(getUniform<GLfloat, 6>(uniformName).data());
	}
	template<> glm::mat3x2 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat3x2(getUniform<GLfloat, 6>(uniformName).data());
	}
	template<> glm::mat2x4 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat2x4(getUniform<GLfloat, 8>(uniformName).data());
	}
	template<> glm::mat4x2 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat4x2(getUniform<GLfloat, 8>(uniformName).data());
	}
	template<> glm::mat3x4 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat3x4(getUniform<GLfloat, 12>(uniformName).data());
	}
	template<> glm::mat4x3 Shader::getUniform(const Tstr& uniformName) {
		return glm::make_mat4x3(getUniform<GLfloat, 12>(uniformName).data());
	}
}