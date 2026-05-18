#pragma once
#include <SFML/System/String.hpp>

// SCREEN SIZE (moving from engine)
// 
// helpers that i use across projects.

using namespace std;

#define Message(input) std::cout << input << std::endl;
#define MessageNE(input) std::cout << input;
#define InputResponse(input) std::cin >> input;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const int PLAYER_LIVES = 20;

const int PLAYER_HP = 20; // if we have time maybe.

// font path TODO: Playtest on Linux to see if we need to add macros depending on platform.
const string FONT_PATH = "content/fonts/burbankbigcondensed_bold.otf";

// ICON PATH FOR PROGRAM
const string ICON_PATH = "content/textures/spr_placeholder.png";