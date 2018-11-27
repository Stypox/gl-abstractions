#ifndef _STYPOX_EBO_H_
#define _STYPOX_EBO_H_

#include <array>
#include <GL/glew.h>

namespace stypox::gl {
	class Ebo {
		GLuint m_id;
		bool m_idGenerated = false;
	public:
		Ebo();
		template <size_t N>
		Ebo(const std::array<GLuint, N>& data, GLenum usage);
		~Ebo();

		Ebo(const Ebo& other) = delete;
		Ebo& operator= (const Ebo& other) = delete;
		Ebo(Ebo&& other);
		Ebo& operator= (Ebo&& other);

		void generate();
		void remove();

		template <size_t N>
		void data(const std::array<GLuint, N>& data, GLenum usage);

		inline void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }
		inline static void unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	};

	template <size_t N>
	inline Ebo::Ebo(const std::array<GLuint, N>& data, GLenum usage) {
		glGenBuffers(1, &m_id);
		this->data(data, usage);
	}
	template <size_t N>
	inline void Ebo::data(const std::array<GLuint, N>& data, GLenum usage) {
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, N * sizeof(GLuint), data.data(), usage);
	}
}
#endif