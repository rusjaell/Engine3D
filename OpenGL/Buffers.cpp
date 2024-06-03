#include "pch.h"
#include "Buffers.h"

BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
    :name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized)
{
}

unsigned int BufferElement::componentCount() const {
	switch (type)
	{
	case ShaderDataType::Float:
		return 1;
	case ShaderDataType::Float2:
		return 2;
	case ShaderDataType::Float3:
		return 3;
	case ShaderDataType::Float4:
		return 4;
	case ShaderDataType::Mat3:
		return 3 * 3;
	case ShaderDataType::Mat4:
		return 4 * 4;
	case ShaderDataType::Int:
		return 1;
	case ShaderDataType::Int2:
		return 2;
	case ShaderDataType::Int3:
		return 3;
	case ShaderDataType::Int4:
		return 4;
	case ShaderDataType::Bool:
		return 1;
	}

	// todo make my own one
	assert(false, "Unknown ShaderDataType!");

	return 0;
}

BufferLayout::BufferLayout()
{
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : elements_(elements), stride_(0)
{
    CalculateOffsetsAndStride();
}

const std::vector<BufferElement>& BufferLayout::elements() const
{
    return elements_;
}

unsigned int BufferLayout::stride() const {
	return stride_;
}

std::vector<BufferElement>::iterator BufferLayout::begin() {
    return elements_.begin();
}

std::vector<BufferElement>::iterator BufferLayout::end() {
    return elements_.end();
}

std::vector<BufferElement>::const_iterator BufferLayout::begin() const
{
	return elements_.begin();
}

std::vector<BufferElement>::const_iterator BufferLayout::end() const
{
	return elements_.end();
}

void BufferLayout::CalculateOffsetsAndStride()
{
    unsigned offset = 0;
    stride_ = 0;
    for (auto& element : elements_) {
        element.offset = offset;
        offset += element.size;
        stride_ += element.size;
    }
}

VertexBuffer::VertexBuffer(unsigned int size)
{
	glCreateBuffers(1, &id_);
	glBindBuffer(GL_ARRAY_BUFFER, id_);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(float* data, unsigned int size)
{
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &id_);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id_);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(const void* data, unsigned int size)
{
	glBindBuffer(GL_ARRAY_BUFFER, id_);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const BufferLayout& VertexBuffer::layout() const
{
	return layout_;
}

void VertexBuffer::SetLayout(const BufferLayout& layout)
{
	layout_ = layout;
}

IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count)
    : count_(count)
{
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &id_);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::count() const
{
    return count_;
}

unsigned int ShaderDataTypeSize(ShaderDataType type) {
	switch (type)
	{
	case ShaderDataType::Float:
		return 4;
	case ShaderDataType::Float2:
		return 4 * 2;
	case ShaderDataType::Float3:
		return 4 * 3;
	case ShaderDataType::Float4:
		return 4 * 4;
	case ShaderDataType::Mat3:
		return 4 * 3 * 3;
	case ShaderDataType::Mat4:
		return 4 * 4 * 4;
	case ShaderDataType::Int:
		return 4;
	case ShaderDataType::Int2:
		return 4 * 2;
	case ShaderDataType::Int3:
		return 4 * 3;
	case ShaderDataType::Int4:
		return 4 * 4;
	case ShaderDataType::Bool:
		return 1;
	}

	// todo make my own one
	assert(false, "Unknown ShaderDataType!");

	return 0;
}
