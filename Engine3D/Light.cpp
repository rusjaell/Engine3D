#include "pch.h"
#include "Light.h"

void DirectionalLIght::CalculateLocalDirection(const glm::mat4& world)
{
	glm::mat3 worldToLocal = glm::inverse(glm::mat3(world));
	localDirection_ = worldToLocal * direction_;
	
	glm::normalize(localDirection_);
}
