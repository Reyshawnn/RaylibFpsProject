#include "UI.h"

void UI::drawAmmoUI(const Vector2 data) const
{
	DrawRectangle(data.x, data.y, 330, 100, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(data.x, data.y,330, 100,BLUE);
}

void UI::drawMapUI(const Vector2 data) const
{
	DrawCircle(data.x, data.y, 90, Fade(PURPLE, 0.5f));
	DrawCircleLines(data.x, data.y, 90, Fade(PURPLE, 0.5f));
}

void UI::drawExtraHudUI(const Vector2 data) const
{
	DrawRectangle(data.x, data.y, 330, 100, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(data.x, data.y, 330, 100, BLUE);
}
