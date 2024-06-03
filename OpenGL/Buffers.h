#ifndef BUFFER_H
#define BUFFER_H

enum class ShaderDataType
{
	None,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

static unsigned int ShaderDataTypeSize(ShaderDataType type);

struct BufferElement {

	std::string name;
	ShaderDataType type;
	unsigned int size;
	unsigned int offset;
	bool normalized;
	 
	BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

	unsigned int componentCount() const;
};

class BufferLayout {

public:
	BufferLayout();
	BufferLayout(const std::initializer_list<BufferElement>& elements);

	const std::vector<BufferElement>& elements() const;

	unsigned int stride() const;

	std::vector<BufferElement>::iterator begin();
	std::vector<BufferElement>::iterator end();
	std::vector<BufferElement>::const_iterator begin() const;
	std::vector<BufferElement>::const_iterator end() const;

private:
	void CalculateOffsetsAndStride();

private:
	std::vector<BufferElement> elements_;
	unsigned int stride_;
};

class VertexBuffer
{
public:
	VertexBuffer(unsigned int size);
	VertexBuffer(float* data, unsigned int size);
	~VertexBuffer();

	void Bind();
	void Unbind();

	void SetData(const void* data, unsigned int size);

	const BufferLayout& layout() const;
	void SetLayout(const BufferLayout& layout);

private:
	unsigned int id_;
	BufferLayout layout_;
};

class IndexBuffer
{
public:
	IndexBuffer(unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();

	unsigned int count() const;

private:
	unsigned int id_;
	unsigned int count_;
};

#endif