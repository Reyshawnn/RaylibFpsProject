#include "UI.h"

void UI::drawAmmoUI(std::string& string, std::string& string2,std::string& string3) const
{
    DrawRectangle(5, 475, 330, 100, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(5, 475, 330, 100, BLUE);
    DrawText(string3.c_str(), 5, 475, 20, BLACK);
    DrawText(string.c_str(), 10, 500, 20, BLACK);
    DrawText(" / ", 15, 500, 20, BLACK);
    DrawText(string2.c_str(), 40, 500, 20, BLACK);
    
   
}

void UI::drawMapUI() const
{
    DrawRectangle(600, 20, 150, 150, Fade(PURPLE, 0.5f));
    DrawRectangle(600, 20, 150, 150, Fade(PURPLE, 0.5f));
}

void UI::drawExtraHudUI(Vector3 velData,std::string& hitString) const
{
    DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(5, 5, 330, 100, BLUE);

    DrawText("Camera controls:", 15, 15, 10, BLACK);
    DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
    DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
    DrawText(TextFormat("- Velocity Len: (%06.3f)", Vector2Length(Vector2{ velData.x, velData.z })), 15, 60, 10, BLACK);
    DrawText("Last Target Hit: ", 15, 75, 10, BLACK);
    if (hitString.empty())
    {
        DrawText("N/A", 100, 75, 10, BLACK);
    }
    else
    {

        DrawText(hitString.c_str(), 100, 75, 10, BLACK);
    }
}
