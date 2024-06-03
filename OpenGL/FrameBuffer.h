#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

struct FrameBufferSpecification 
{
	unsigned int width = 0;
	unsigned int height = 0;
};

class FrameBuffer
{
public:
	FrameBuffer(const FrameBufferSpecification& specification);
	~FrameBuffer();

	void Invalidate();

	void Resize(unsigned int width, unsigned int height);
	void Bind();
	void Unbind();

	unsigned int colorAttachment() const;
	unsigned int depthAttachment() const;
	const FrameBufferSpecification& specification() const;

private:
	unsigned int handle_;
	unsigned int colorAttachement_;
	unsigned int depthAttachment_;

	FrameBufferSpecification specification_;
};

#endif