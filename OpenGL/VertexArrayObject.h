#ifndef VERTEX_ARRAY_OBJECT_H
#define	VERTEX_ARRAY_OBJECT_H

#include "Buffer.h"
#include "VertexBufferLayout.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

private:
	unsigned int id_;
	
};

#endif // !VERTEX_ARRAY_OBJECT_H
