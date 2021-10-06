#include "SolarSystem.h"

#include "Rendering/Transform.h"
#include "Rendering/RenderHelper.h"

void SolarSystem::update(float dt)
{
	rotation += 10 * dt;
}

void SolarSystem::render()
{
	// draw sun in the centre
	RenderHelper::drawSphere(0.3f, 1, 1, 0);
	{ // first planet
		// the first transform rotates the planet around the sun
		Transform t1({ 0, 0, 0 }, { 0, 0, 10 + 0.5f * rotation }, { 1, 1, 1 });
		{
			// the second transform places the planet in its orbit and rotates the planet
			Transform t2({ 0.7f, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
			RenderHelper::drawSphere(0.1f, 0, 0, 1);
		}
	}
	{ // second planet
		// the first transform rotates the planet around the sun
		Transform t1({ 0, 0, 0 }, { 0, 0, 70 + rotation }, { 1, 1, 1 });
		{
			// the second transform places the planet in its orbit and rotates the planet
			Transform t2({ 1.4f, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
			RenderHelper::drawSphere(0.1f, 1.0f, 0, 0.6f);

			// the second planet has a moon
			{
				Transform t3({ 0, 0, 0 }, { 0, 0, 160 + 2 * rotation }, { 1, 1, 1 });
				{
					Transform t4({ 0.25f, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
					RenderHelper::drawSphere(0.05f, 0.7f, 0.7f, 0.7f);

					// this moon has a moon
					{
						Transform t5({ 0, 0, 0 }, { 0, 0, 45 + 6 * rotation }, { 1, 1, 1 });
						{
							Transform t6({ 0.1f, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
							RenderHelper::drawSphere(0.03f, 0.8f, 0.2f, 0.3f);
						}
					}
				}
			}
		}
	}
	{ // third
		// the first transform rotates the planet around the sun
		Transform t1({ 0, 0, 0 }, { 0, 0, 220 + 3 * rotation }, { 1, 1, 1 });
		{
			// the second transform places the planet in its orbit and rotates the planet
			Transform t2({ 2.3f, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
			RenderHelper::drawSphere(0.1f, 0, 0.9f, 0.3f);

			// the third planet has two moons
			{
				Transform t3({ 0, 0, 0 }, { 0, 0, 90 + 3.0f * rotation }, { 1, 1, 1 });
				{
					Transform t4({ 0.2f, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
					RenderHelper::drawSphere(0.07f, 0.5f, 0.7f, 0.9f);
				}
			}
			{
				Transform t3({ 0, 0, 0 }, { 0, 210 + 2.5f * rotation, 45 + 6 * rotation }, { 1, 1, 1 });
				{
					Transform t4({ 0.5f, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
					RenderHelper::drawSphere(0.03f, 1.0f, 0.0f, 0.7f);
				}
			}
		}
	}
}
