#pragma once

#include "Core/Scene.h"


class Week5Scene : public Scene
{
public:
	Week5Scene() = default;

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnRender() override;
};

