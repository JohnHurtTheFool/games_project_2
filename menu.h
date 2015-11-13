
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef MENU_H                 // Prevent multiple definitions if this 
#define MENU_H 

class Menu;

#include "graphics.h"
#include "constants.h"
#include "testDX.h"
#include <string>
#include <sstream>
#include "input.h"

namespace menuNS
{ }

// inherits from Entity class
class Menu 
{
private:
   TextDX *menuItemFont;
   TextDX *menuItemFontHighlight;
   TextDX *menuHeadingFont;
   Input   *input;         // pointer to the input system
   Graphics *graphics;
   int selectedItem;
   std::string menuHeading;
   std::string menuItem1;
   std::string menuItem2;
   std::string menuItem3;
   D3DXVECTOR2 menuAnchor;
   int verticalOffset;
   int linePtr;
   COLOR_ARGB highlightColor ;
   COLOR_ARGB normalColor;
   bool upDepressedLastFrame;
   bool downDepressedLastFrame;

public:
    // constructor
    Menu();
	void initialize(Graphics *g, Input *i);
	void update();
	int getSelectedItem() {return selectedItem;}
	void displayMenu();
};
#endif

