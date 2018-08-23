#ifndef _STYPOX_VBO_H_
#define _STYPOX_VBO_H_

#include <array>
#include <GL/glew.h>
#include <sp/basicTypes.h>
#include <sp/declarations.h>

namespace stypox::gl {
	class Vbo {
		GLuint m_id;
		bool m_idGenerated = false;
	public:
		Vbo();
		template <size_t N>
		Vbo(const std::array<GLfloat, N>& data, GLenum usage);
		~Vbo();

		template <size_t N>
		void data(const std::array<GLfloat, N>& data, GLenum usage);

		void generate();
		void remove();

		inline void bind() { glBindBuffer(GL_ARRAY_BUFFER, m_id); }
		inline static void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	};

	template <size_t N>
	inline Vbo::Vbo(const std::array<GLfloat, N>& data, GLenum usage) {
		glGenBuffers(1, &m_id);
		this->data(data, usage);
	}
	template <size_t N>
	inline void Vbo::data(const std::array<GLfloat, N>& data, GLenum usage) {
		std::cout << N;
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), data.data(), usage);
	}
}

#include "vbo.cpp"
#endif