#pragma once

#include "Core/Vector3.h"
#include "Core/Color.h"


class Material
{
public:
	Material() = default;

	void apply();

	inline void setAmbient(const Color& c) { ambient = c; }
	inline const Color& getAmbient() { return ambient; }
	inline void setDiffuse(const Color& c) { diffuse = c; }
	inline const Color& getDiffuse() { return diffuse; }
	inline void setSpecular(const Color& c) { specular = c; }
	inline const Color& getSpecular() { return specular; }
	inline void setEmission(const Color& c) { emmission = c; }
	inline const Color& getEmission() { return emmission; }
	inline void setShininess(float s) { shininess = s; }
	inline float getShininess() { return shininess; }

private:
	Color ambient = Color::black;
	Color diffuse = Color::white;
	Color specular = Color::black;

	Color emmission = Color::black;

	float shininess = 0.0f;
};

