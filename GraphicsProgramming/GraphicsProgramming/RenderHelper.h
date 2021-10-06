#pragma once


class RenderHelper
{
public:
	RenderHelper() = delete;

	static void drawSphere(float size, int slices = 20, int stacks = 20);
	static void drawSphere(float size, float r, float g, float b, int slices = 20, int stacks = 20);

};

