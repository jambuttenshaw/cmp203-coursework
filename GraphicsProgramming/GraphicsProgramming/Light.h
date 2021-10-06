#pragma once

#include "Vector3.h"
#include "Color.h"


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

	inline void setPosition(const Vector3& pos) { homogeneousPos.xyz = pos; }
	inline const Vector3& getPosition() { return homogeneousPos.xyz; }

	inline void setDiffuseColor(const Color& color) { diffuseColor = color; }
	inline const Color& getDiffuseColor() { return diffuseColor; }

	inline void setAmbientColor(const Color& color) { ambientColor = color; }
	inline const Color& getAmbientColor() { return ambientColor; }

	inline void setSpotDirection(const Vector3& dir) { spotDirection = dir; }
	inline const Vector3& getSpotDirection() { return spotDirection; }

	inline void setSpotExponent(float e) { spotExponent = e; }
	inline float getSpotExponent() { return spotExponent; }

	inline void setSpotCutoff(float cutoff) { spotCutoff = cutoff; }
	inline float getSpotCutoff() { return spotCutoff; }

private:
	// type
	LightType type = LightType::Point;

	// position
	struct
	{
		Vector3 xyz;
		float w = 1.0f;

		float* ptr() { return reinterpret_cast<float*>(this); }
	} homogeneousPos;

	// color
	Color diffuseColor;
	Color ambientColor;

	// spotlight
	Vector3 spotDirection;
	float spotExponent = 0.0f;
	float spotCutoff = 0.0f;
};

