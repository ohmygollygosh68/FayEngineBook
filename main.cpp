#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"

extern "C"
{
#include "lua536/include/lua.h"
#include "lua536/include/lauxlib.h"
#include "lua536/include/lualib.h"
}

#include <iostream>
#include <string>

#ifdef _WIN64
#pragma comment(lib, "lua536/liblua53.a")
#endif // _WIN64

bool CheckLua(lua_State* L, int r)
{
    if (r != LUA_OK)
    {
        std::string errormsg = lua_tostring(L, -1);
        std::cout << errormsg << std::endl;
        return false;
    }
    return true;
}

int lua_HostFunction(lua_State* L)
{
    // int n = lua_gettop(L) returns the number of arguments. Not used here.
    float a = (float)lua_tonumber(L, 1); // Fresh stack so not necessary to use -1 although this does work
    float b = (float)lua_tonumber(L, 2);
    std::cout << "[C++] HostFunction(" << a << ", " << b << ") called" << std::endl;
    float c = a * b;
    lua_pushnumber(L, c);
    return 1; // This is just the number of arguments passed back to Lua. In this case the value used for variable c. It does not have to be just 1.

}





#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
    Example()
    {
        // Name your application
        sAppName = "Example";
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Called once per frame, draws random coloured pixels
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
        return true;
    }
};


















int main()
{

    Example demo;
    if (demo.Construct(256, 240, 4, 4))
        demo.Start();



    struct Player
    {
        std::string title;
        std::string name;
        std::string family;
        int level;
    } player;

    // Create Lua State
    lua_State* L = luaL_newstate();

    // Add standard libraries to Lua Virtual Machine
    luaL_openlibs(L);
    // Link the C++ function to the virtual machine
    lua_register(L, "HostFunction", lua_HostFunction);

    if (CheckLua(L, luaL_dofile(L, "VideoExample.lua")))
    {
        lua_getglobal(L, "DoAThing");
        if (lua_isfunction(L, -1))
        {
            lua_pushnumber(L, 5.0f);
            lua_pushnumber(L, 6.0f);
            if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
            {
                std::cout << "[C++] Called in Lua 'DoAThing(5.0f, 6.0f)', got "
                    << (float)lua_tonumber(L, -1) << std::endl;
            }
        }
    }


    system("pause");
    lua_close(L);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Window Title");
    ImGui::SFML::Init(window);

    bool circleExists = true;
    float circleRadius = 200.0f;
    int circleSegments = 100;
    float circleColor[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
    sf::CircleShape shape(circleRadius, circleSegments);
    shape.setFillColor(sf::Color
    (
        (int)(circleColor[0] * 255),
        (int)(circleColor[1] * 255),
        (int)(circleColor[2] * 255)
    )); // Color circle
    shape.setOrigin(circleRadius, circleRadius);
    shape.setPosition(400, 400); // Center circle

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Window title");
        ImGui::Text("Window text!");
        ImGui::Checkbox("Circle", &circleExists);
        ImGui::SliderFloat("Radius", &circleRadius, 100.0f, 300.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 150);
        ImGui::ColorEdit3("Color Circle", circleColor);
        ImGui::End();

        shape.setRadius(circleRadius);
        shape.setOrigin(circleRadius, circleRadius);
        shape.setPointCount(circleSegments);
        shape.setFillColor(sf::Color
        (
            (int)(circleColor[0] * 255),
            (int)(circleColor[1] * 255),
            (int)(circleColor[2] * 255)
        )); // Color circle

        window.clear(sf::Color(18, 33, 43)); // Color background
        if (circleExists)
            window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}