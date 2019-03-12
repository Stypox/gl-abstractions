#ifndef _STYPOX_GL_VAO_HPP_
#define _STYPOX_GL_VAO_HPP_

#include <glad/glad.h>

namespace stypox::gl {
	class Vao {
		GLuint m_id;
		bool m_idGenerated = false;
	public:
		Vao();
		~Vao();

		Vao(const Vao& other) = delete;
		Vao& operator= (const Vao& other) = delete;
		Vao(Vao&& other);
		Vao& operator= (Vao&& other);

		void generate();
		void remove();

		static void enableAttrib(GLuint index);
		static void attribPointer(GLuint index, GLint sizeOfSet, GLenum type, GLsizei strideOfSet, GLsizei offsetOfSet, GLboolean normalize = GL_FALSE);
		static void attribDivisor(GLuint index, GLuint divisor);

		inline void bind() { glBindVertexArray(m_id); }
		inline static void unbind() { glBindVertexArray(0); }
	};
}
#endif