#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

struct FrameBufferSpecification 
{
	unsigned int width = 1;
	unsigned int height = 1;
	glm::vec4 clearColor;
	unsigned int clearBit;
};

class FrameBuffer
{
public:
	FrameBuffer(const FrameBufferSpecification& specification);
	~FrameBuffer();

	void Invalidate();

	void Bind();
	void Unbind();

	void Resize(unsigned int width, unsigned int height);
	
	unsigned int id() const;
	unsigned int colorAttachment() const;
	unsigned int depthAttachment() const;
	FrameBufferSpecification& specification();

private:
	unsigned int handle_;
	unsigned int colorAttachement_;
	unsigned int depthAttachment_;

	FrameBufferSpecification specification_;
};

#endif