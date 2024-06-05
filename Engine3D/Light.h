#ifndef LIGHT_H
#define LIGHT_H

struct Light
{
	glm::vec3 color_;

	float ambientIntensity_;
};

struct DirectionalLIght : public Light
{
	glm::vec3 direction_;

	float diffuseIntensity_;

	void CalculateLocalDirection(const glm::mat4& world);

private:
	glm::vec3 localDirection_;
};

struct PointLight : public Light
{

};

struct SpotLight : public Light
{

};

#endif // !LIGHT_H
