#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <vector>
#include <assert.h>

struct VertexBufferElement {
	unsigned int type_;
	unsigned int count_;
	unsigned char normalized_;

	static unsigned int GetSizeOfType(unsigned int type) {

		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		assert(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout()
		: stride_(0)
	{
	}
	
	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		elements_.push_back({ GL_FLOAT, count, GL_FALSE });
		stride_ += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const {
		return elements_;
	}

	inline unsigned int GetStride() const {
		return stride_;
	}

private:
	unsigned int stride_;

	std::vector<VertexBufferElement> elements_;
};

#endif // !VERTEX_BUFFER_LAYOUT_H

