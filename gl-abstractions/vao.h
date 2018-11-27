#ifndef _STYPOX_VAO_H_
#define _STYPOX_VAO_H_

#include <GL/glew.h>

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

		void attribPointer(GLuint index, GLint sizeOfSet, GLenum type, GLsizei strideOfSet, GLsizei offsetOfSet, GLboolean normalize = GL_FALSE);

		inline void bind() { glBindVertexArray(m_id); }
		inline static void unbind() { glBindVertexArray(0); }
	};
}
#endif