#include "pch.h"
#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &id_);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &id_);
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(id_);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArrayObject::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
	Bind();
	(static_cast<VertexBuffer>(vertexBuffer)).Bind();

	unsigned int offset = 0;
	const auto& elements = layout.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count_, element.type_, element.normalized_, layout.GetStride(), (const void*)offset);
		offset += element.count_ * VertexBufferElement::GetSizeOfType(element.type_);
	}
}
