#pragma once

#include <random>

#include <glm/glm.hpp>

class Random
{
public:
	static void Init();
	static void Shutdown();

	// generate a random float between 0 and max
	static float RandomFloat(float max);
	// generate a random float between min and max
	static float RandomFloat(float min, float max);
	// generate a random float between 0 and 1
	static float RandomNormalizedFloat();

	static glm::vec2 RandomVector(const glm::vec2& min, const glm::vec2& max);
	static glm::vec3 RandomVector(const glm::vec3& min, const glm::vec3& max);
	static glm::vec4 RandomVector(const glm::vec4& min, const glm::vec4& max);
	static glm::vec2 RandomUnitVector2();
	static glm::vec3 RandomUnitVector3();
	static glm::vec4 RandomUnitVector4();

private:
	// the pseudo random number generator used
	static std::mt19937* s_RandomNumberGenerator;
	// normalized floats are always in the same range so we can pre-define a uniform distribution of them
	static std::uniform_real_distribution<float> s_NormamlizedFloatDistribution;
};
