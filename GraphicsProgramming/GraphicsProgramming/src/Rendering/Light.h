#pragma once

#include <glm/glm.hpp>
#include "Core/Color.h"


class Light
{
public:
	enum class LightType
	{
		Invalid,
		Point,
		Directional,
		Spot
	};

	Light() : type(LightType::Invalid) {}
	Light(LightType t) { setType(t); }

	void render(unsigned int lightID, bool debugSphere = false);

	// getters/setters
	void setType(LightType t);
	inline LightType getType() const { return type; }

	inline void setPosition(const glm::vec3& pos) { homogeneousPos.xyz = pos; }
	inline const glm::vec3& getPosition() { return homogeneousPos.xyz; }

	inline void setDiffuseColor(const Color& color) { diffuseColor = color; }
	inline const Color& getDiffuseColor() { return diffuseColor; }

	inline void setAmbientColor(const Color& color) { ambientColor = color; }
	inline const Color& getAmbientColor() { return ambientColor; }

	inline void setSpecularColor(const Color& color) { specularColor = color; }
	inline const Color& getSpecularColor() { return specularColor; }

	inline void setSpotDirection(const glm::vec3& dir) { spotDirection = dir; }
	inline const glm::vec3& getSpotDirection() { return spotDirection; }

	inline void setSpotExponent(float e) { spotExponent = e; }
	inline float getSpotExponent() { return spotExponent; }

	inline void setSpotCutoff(float cutoff) { spotCutoff = cutoff; }
	inline float getSpotCutoff() { return spotCutoff; }

	inline void setAttenuation(const glm::vec3& a) { attentuation = a; }
	inline const glm::vec3& getAttenuation() { return attentuation; }

public:

	static void DisplayDebugSpheresForAllLights(bool value) { debugSpheresOnAllLights = value; }

private:
	// type
	LightType type = LightType::Point;

	// position
	struct
	{
		glm::vec3 xyz{ 0.0f };
		float w = 1.0f;

		float const* ptr() const { return reinterpret_cast<float const*>(this); }
	} homogeneousPos;

	// color
	Color diffuseColor;
	Color ambientColor;
	Color specularColor;

	// spotlight
	glm::vec3 spotDirection{ 0.0f };
	float spotExponent = 0.0f;
	float spotCutoff = 0.0f;

	// attenuation
	glm::vec3 attentuation = { 1, 0, 0 };

private:

	static bool debugSpheresOnAllLights;
};

