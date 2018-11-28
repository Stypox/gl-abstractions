#ifndef _STYPOX_VBO_H_
#define _STYPOX_VBO_H_

#include <array>
#include <glad/glad.h>

namespace stypox::gl {
	class Vbo {
		GLuint m_id;
		bool m_idGenerated = false;
	public:
		Vbo();
		template <size_t N>
		Vbo(const std::array<GLfloat, N>& data, GLenum usage);
		~Vbo();

		Vbo(const Vbo& other) = delete;
		Vbo& operator= (const Vbo& other) = delete;
		Vbo(Vbo&& other);
		Vbo& operator= (Vbo&& other);

		void generate();
		void remove();

		template <size_t N>
		void data(const std::array<GLfloat, N>& data, GLenum usage);

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
		bind();
		glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), data.data(), usage);
	}
}
#endif