#ifndef _STYPOX_VBO_H_
#define _STYPOX_VBO_H_

#include <glad/glad.h>

namespace stypox::gl {
	class Vbo {
		GLuint m_id;
		bool m_idGenerated = false;
	public:
		Vbo();
		template <class T>
		Vbo(T* pointer, size_t count, GLenum usage);
		~Vbo();

		Vbo(const Vbo& other) = delete;
		Vbo& operator= (const Vbo& other) = delete;
		Vbo(Vbo&& other);
		Vbo& operator= (Vbo&& other);

		void generate();
		void remove();

		template <class T>
		void data(T* pointer, size_t count, GLenum usage) { bind(); glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), pointer, usage); }

		inline void bind() { glBindBuffer(GL_ARRAY_BUFFER, m_id); }
		inline static void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	};

	template <class T>
	inline Vbo::Vbo(T* pointer, size_t count, GLenum usage) :
		m_idGenerated{true} {
		glGenBuffers(1, &m_id);
		this->data(pointer, count, usage);
	}
}
#endif