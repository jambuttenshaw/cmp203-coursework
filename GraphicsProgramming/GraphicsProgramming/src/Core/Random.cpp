#include "Random.h"


std::mt19937* Random::s_RandomNumberGenerator = nullptr;
std::uniform_real_distribution<float> Random::s_NormamlizedFloatDistribution(0, 1);

void Random::Init()
{
	// initialize the random number generator with a random seed
	// std::random_device is too slow to use other than in the constructor,
	// but it provides a very good seed
	std::random_device rd;
	s_RandomNumberGenerator = new std::mt19937(rd());
}

void Random::Shutdown()
{
	delete s_RandomNumberGenerator;
}

float Random::RandomFloat(float max)
{
	return RandomFloat(0, max);
}
float Random::RandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(*s_RandomNumberGenerator);
}
float Random::RandomNormalizedFloat()
{
	return s_NormamlizedFloatDistribution(*s_RandomNumberGenerator);
}

glm::vec2 Random::RandomVector(const glm::vec2& min, const glm::vec2& max)
{
	return glm::vec2{ RandomFloat(min.x, max.x), RandomFloat(min.y, max.y) };
}
glm::vec3 Random::RandomVector(const glm::vec3& min, const glm::vec3& max)
{
	return glm::vec3{ RandomFloat(min.x, max.x), RandomFloat(min.y, max.y), RandomFloat(min.z, max.z) };
}
glm::vec4 Random::RandomVector(const glm::vec4& min, const glm::vec4& max)
{
	return glm::vec4{ RandomFloat(min.x, max.x), RandomFloat(min.y, max.y), RandomFloat(min.z, max.z), RandomFloat(min.w, max.w) };
}

glm::vec2 Random::RandomUnitVector2()
{
	float phi1 = RandomFloat(3.1416f);
	// point on 2D circle
	return glm::vec2{
		cosf(phi1),
		sinf(phi1)
	};
}
glm::vec3 Random::RandomUnitVector3()
{
	float phi1 = RandomFloat(3.1416f);
	float phi2 = RandomFloat(3.1416f);
	// point on 3D hypersphere
	return glm::vec3{
		cosf(phi1),
		sinf(phi1) * cosf(phi2),
		sinf(phi1) * sinf(phi2)
	};
}
glm::vec4 Random::RandomUnitVector4()
{
	float phi1 = RandomFloat(3.1416f);
	float phi2 = RandomFloat(3.1416f);
	float phi3 = RandomFloat(6.2831f);
	// point on 4D hypersphere
	return glm::vec4{
		cosf(phi1),
		sinf(phi1) * cosf(phi2),
		sinf(phi1) * sinf(phi2) * cosf(phi3),
		sinf(phi1) * sinf(phi2) * sinf(phi3)
	};
}
