#include "pch.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
	: specification_(specification)
{
	Invalidate();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &handle_);
}

void FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	specification_.width = width;
	specification_.height = height;
	Invalidate();
}

void FrameBuffer::Invalidate()
{
	glCreateFramebuffers(1, &handle_);
	glBindFramebuffer(GL_FRAMEBUFFER, handle_);
	
	glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachement_);
	glBindTexture(GL_TEXTURE_2D, colorAttachement_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification_.width, specification_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachement_, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment_);
	glBindTexture(GL_TEXTURE_2D, depthAttachment_);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specification_.width, specification_.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment_, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[FrameBuffer] framebuffer is incomplete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle_);

	glClearColor(specification_.clearColor.r, specification_.clearColor.g, specification_.clearColor.b, specification_.clearColor.a);
	glClear(specification_.clearBit);

	glViewport(0, 0, specification_.width, specification_.height);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::id() const
{
	return handle_;
}

unsigned int FrameBuffer::colorAttachment() const
{
	return colorAttachement_;
}

unsigned int FrameBuffer::depthAttachment() const
{
	return depthAttachment_;
}

FrameBufferSpecification& FrameBuffer::specification() 
{
	return specification_;
}