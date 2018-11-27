#ifndef _STYPOX_SHADER_H_
#define _STYPOX_SHADER_H_
//TODO functions like getAttribLocation should save errors in a log. glGetnUniform gives errors and could be useful.

#include <string>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../libs/file-management/file-management/file_management.h"

namespace stypox::gl {
	constexpr int maxInfoLogCharacters{512};

	class Shader {
		GLuint m_vertexShId, m_fragmentShId, m_programId;
		std::string m_fileLog;
		bool m_idsCreated = false;

		void create();
	public:
		enum Step : char {
			file = 1,
			vertex = 2,
			fragment = 4,
			program = 8
		};


		Shader();
		Shader(const std::filesystem::path& vertexShFilename, const std::filesystem::path& fragmentShFilename);
		~Shader();

		Shader(const Shader& other) = delete;
		Shader& operator= (const Shader& other) = delete;
		Shader(Shader&& other);
		Shader& operator= (Shader&& other);

		void remove();
		
		void compileFile(const std::filesystem::path& vertexShFilename, const std::filesystem::path& fragmentShFilename);
		void compileSource(const std::string& vertexShSource, const std::string& fragmentShSource);

		Step errors();
		std::string getLog(Step step);
		std::string debugInfo(std::string name = "");

		inline void bind() { glUseProgram(m_programId); }
		inline static void unbind() { glUseProgram(0); }

		GLint getAttribLocation(const std::string& attributeVariableName);

		template<typename T> void uniform(const std::string& uniformName, const T& v0);
		template<typename T> void uniform(const std::string& uniformName, const T& v0, const T& v1);
		template<typename T> void uniform(const std::string& uniformName, const T& v0, const T& v1, const T& v2);
		template<typename T> void uniform(const std::string& uniformName, const T& v0, const T& v1, const T& v2, const T& v3);
		template<typename T> void uniform(const std::string& uniformName, const T& v0, GLboolean transpose);

		//single values
		template<typename T> T getUniform(const std::string& uniformName);
		//GLfloat
		template<typename T, int N>
			typename std::enable_if<std::is_same<T, GLfloat>::value, std::array<T, N>>::type
			getUniform(const std::string& uniformName);
		//GLint
		template<typename T, int N>
			typename std::enable_if<std::is_same<T, GLint>::value, std::array<T, N>>::type
			getUniform(const std::string& uniformName);
		//GLuint
		template<typename T, int N>
			typename std::enable_if<std::is_same<T, GLuint>::value, std::array<T, N>>::type
			getUniform(const std::string& uniformName);
		//neither GLfloat, GLint nor GLuint (error)
		template<typename T, int N>
			typename std::enable_if<!std::is_same<T, GLfloat>::value && !std::is_same<T, GLint>::value && !std::is_same<T, GLuint>::value, void>::type
			getUniform(const std::string& uniformName);
	};

	inline void operator|=(Shader::Step& a, Shader::Step b) {
		a = static_cast<Shader::Step>(static_cast<char>(a)|static_cast<char>(b));
	}
	inline void operator&=(Shader::Step& a, Shader::Step b) {
		a = static_cast<Shader::Step>(static_cast<char>(a)&static_cast<char>(b));
	}
}
#endif
