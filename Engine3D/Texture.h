#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:
	Texture(const std::string& type, const std::string& filePath);
	Texture(std::string& type, unsigned int width, unsigned int height);
	~Texture();

	void Bind(unsigned int slot = 0) const;

	void SetDataRGBA(void* data, unsigned int size);

	unsigned int id() const;
	unsigned int width() const;
	unsigned int height() const;
	stbi_uc* data() const;
	const std::string& type() const;

	bool operator==(const Texture& other);

private:
	unsigned int id_;

	int width_;
	int height_;
	int channels_;
	GLenum internalformat_;
	GLenum dataFormat_;
	stbi_uc* data_;
	std::string type_;
};

#endif // !TEXTURE_H
