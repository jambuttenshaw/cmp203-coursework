#pragma once

#include <glm/glm.hpp>
#include "Core/Color.h"


class Material
{
public:
	Material() = default;
	Material(const Color& ambientAndDiffuse);

	void apply() const;

	inline void setAmbient(const Color& c) { ambient = c; }
	inline const Color& getAmbient() const { return ambient; }

	inline void setDiffuse(const Color& c) { diffuse = c; }
	inline const Color& getDiffuse() const { return diffuse; }

	inline void setAmbientAndDiffuse(const Color& c) { ambient = c; diffuse = c; }

	inline void setSpecular(const Color& c) { specular = c; }
	inline const Color& getSpecular() const { return specular; }

	inline void setEmission(const Color& c) { emmission = c; }
	inline const Color& getEmission() const { return emmission; }

	inline void setShininess(float s) { shininess = s; }
	inline float getShininess() const { return shininess; }

private:
	Color ambient = Color::White;
	Color diffuse = Color::White;
	Color specular = Color::Black;

	Color emmission = Color::Black;

	float shininess = 0.0f;

public:
	static const Material Default;
	static const Material Error;
};

