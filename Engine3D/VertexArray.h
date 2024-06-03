#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "Buffers.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void Unbind();

	void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const Shared<IndexBuffer>& vertexBuffer);

	const std::vector<Shared<VertexBuffer>>& vertexBuffers() const;
	const Shared<IndexBuffer> indexBuffer() const;

private:
	unsigned int id_;

	std::vector<Shared<VertexBuffer>> vertexBuffers_;
	Shared<IndexBuffer> indexBuffer_;
};

#endif // !VERTEX_ARRAY_H
