#ifndef TEXTURE_H
#define TEXTURE_H

#include "Asset.h"

class Texture : public Asset
{
public:
	Texture(const std::string& filePath);
	Texture(unsigned int width, unsigned int height);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind(unsigned int slot = 0) const;

	void SetDataRGBA(void* data, unsigned int size);

	unsigned int id() const;
	unsigned int width() const;
	unsigned int height() const;
	stbi_uc* data() const;

	bool operator==(const Texture& other);

private:
	unsigned int id_;

	int width_;
	int height_;
	int channels_;
	GLenum internalformat_;
	GLenum dataFormat_;
	stbi_uc* data_;
};

#endif // !TEXTURE_H
