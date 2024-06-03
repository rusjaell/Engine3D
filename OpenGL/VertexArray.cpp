#include "pch.h"
#include "VertexArray.h"

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type)
	{
	case ShaderDataType::Float:
		return GL_FLOAT;
	case ShaderDataType::Float2:
		return GL_FLOAT;
	case ShaderDataType::Float3:
		return GL_FLOAT;
	case ShaderDataType::Float4:
		return GL_FLOAT;
	case ShaderDataType::Mat3:
		return GL_FLOAT;
	case ShaderDataType::Mat4:
		return GL_FLOAT;
	case ShaderDataType::Int:
		return GL_INT;
	case ShaderDataType::Int2:
		return GL_INT;
	case ShaderDataType::Int3:
		return GL_INT;
	case ShaderDataType::Int4:
		return GL_INT;
	case ShaderDataType::Bool:
		return GL_BOOL;
	}

	// todo make my own one
	assert(false, "Unknown ShaderDataType!");

	return 0;
}

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &id_);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id_);
}

void VertexArray::Bind()
{
	glBindVertexArray(id_);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(id_);
	vertexBuffer->Bind();

	const BufferLayout& layout = vertexBuffer->layout();
	unsigned int index = 0;
	for (const auto& element : layout) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.componentCount(), ShaderDataTypeToOpenGLBaseType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.stride(), (const void*)element.offset);
		index++;
	}

	vertexBuffers_.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(id_);
	indexBuffer->Bind();

	indexBuffer_ = indexBuffer;
}

const std::vector<Shared<VertexBuffer>>& VertexArray::vertexBuffers() const {
	return vertexBuffers_;
}

const Shared<IndexBuffer> VertexArray::indexBuffer() const {
	return indexBuffer_;
}