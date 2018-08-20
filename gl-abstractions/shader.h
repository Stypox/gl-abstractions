#ifndef _SP_SHADER_H_
#define _SP_SHADER_H_
//TODO functions like getAttribLocation should save errors in a log. glGetnUniform gives errors and could be useful.

#include <string>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sp/fileManagement.h>
#include <sp/basicTypes.h>
#include <sp/declarations.h>

namespace stypox::gl {
	constexpr int maxInfoLogCharacters{512};

	class Shader {
	public:
		GLuint m_vertexShId, m_fragmentShId, m_programId;
		Tstr m_fileLog;
		bool m_idsCreated = false;

		void deleteShaders();
		void createShaders();
	public:
		enum Step : char {
			file = 1,
			vertex = 2,
			fragment = 4,
			program = 8
		};


		Shader();
		Shader(const Tpath& vertexShFilename, const Tpath& fragmentShFilename);
		~Shader();
		
		void compileFile(const Tpath& vertexShFilename, const Tpath& fragmentShFilename);
		void compileSource(const Tstr& vertexShSource, const Tstr& fragmentShSource);

		Step errors();
		Tstr getLog(Step step);
		Tstr debugInfo(Tstr name = "");

		void bind();
		static void unbind();

		GLint getAttribLocation(const Tstr& attributeVariableName);

		template<typename T> void uniform(const Tstr& uniformName, const T& v0);
		template<typename T> void uniform(const Tstr& uniformName, const T& v0, const T& v1);
		template<typename T> void uniform(const Tstr& uniformName, const T& v0, const T& v1, const T& v2);
		template<typename T> void uniform(const Tstr& uniformName, const T& v0, const T& v1, const T& v2, const T& v3);
		template<typename T> void uniform(const Tstr& uniformName, const T& v0, GLboolean transpose);

		//single values
		template<typename T> T getUniform(const Tstr& uniformName);
		//GLfloat
		template<typename T, int N>
			typename std::enable_if<std::is_same<T, GLfloat>::value, std::array<T, N>>::type
			getUniform(const Tstr& uniformName);
		//GLint
		template<typename T, int N>
			typename std::enable_if<std::is_same<T, GLint>::value, std::array<T, N>>::type
			getUniform(const Tstr& uniformName);
		//GLuint
		template<typename T, int N>
			typename std::enable_if<std::is_same<T, GLuint>::value, std::array<T, N>>::type
			getUniform(const Tstr& uniformName);
		//neither GLfloat, GLint nor GLuint (error)
		template<typename T, int N>
			typename std::enable_if<!std::is_same<T, GLfloat>::value && !std::is_same<T, GLint>::value && !std::is_same<T, GLuint>::value, void>::type
			getUniform(const Tstr& uniformName);
	};

	inline void operator|=(Shader::Step& a, Shader::Step b) {
		a = static_cast<Shader::Step>(static_cast<char>(a)|static_cast<char>(b));
	}
	inline void operator&=(Shader::Step& a, Shader::Step b) {
		a = static_cast<Shader::Step>(static_cast<char>(a)&static_cast<char>(b));
	}
}

#include "shader.cpp"
#endif
