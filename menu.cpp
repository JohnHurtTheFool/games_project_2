
#include "menu.h"

Menu::Menu()
{
	selectedItem = -1;	//nothing return
	menuItemFont = new TextDX();
	menuHeadingFont = new TextDX();
}

void Menu::initialize(Graphics *g, Input *i)
{
	menuHeading ="SPACESHIP CONTROL:";
	menuItem1 = "PLAY";
	menuItem2 = "CHEAT CODES";
	menuItem3 = "OPTIONS";
	highlightColor = graphicsNS::BLUE;
	normalColor = graphicsNS::RED;
	menuAnchor = D3DXVECTOR2(GAME_WIDTH*.35,GAME_HEIGHT*.05);
	input = i;
	verticalOffset = GAME_HEIGHT/10;
	linePtr = 0;
	selectedItem = -1;
	graphics = g;
	menuItemFont = new TextDX();
	menuHeadingFont = new TextDX();
	menuItemFontHighlight = new TextDX();
	if(menuItemFont->initialize(graphics, 40, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuItem font"));
	if(menuItemFontHighlight->initialize(graphics, 50, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuItem font"));
	if(menuHeadingFont->initialize(graphics, 60, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuHeading font"));
	menuHeadingFont->setFontColor(normalColor);
	menuItemFont->setFontColor(normalColor);
	menuItemFontHighlight->setFontColor(highlightColor);
	upDepressedLastFrame = false;
	downDepressedLastFrame = false;
}

void Menu::update()
{
	if(!input->isKeyDown(VK_UP))
		upDepressedLastFrame = false;
	if(!input->isKeyDown(VK_DOWN))
		downDepressedLastFrame = false;
	if (input->isKeyDown(VK_UP)&&!upDepressedLastFrame)
	{
		linePtr--;
		upDepressedLastFrame=true;
	}

	if (input->isKeyDown(VK_DOWN)&&!downDepressedLastFrame)
	{
		linePtr++;
		downDepressedLastFrame=true;
	}
	if (linePtr > 2) linePtr = 0;
	if (linePtr < 0) linePtr = 2;

	if (input->isKeyDown(VK_RETURN))
		selectedItem = linePtr;
	else selectedItem = -1;
}

void Menu::displayMenu()
{
	menuHeadingFont->print(menuHeading, menuAnchor.x, menuAnchor.y);
	if (linePtr==0)
		menuItemFontHighlight->print(menuItem1, menuAnchor.x, menuAnchor.y+verticalOffset);
	else
		menuItemFont->print(menuItem1, menuAnchor.x, menuAnchor.y+verticalOffset);
	int foo = 2*verticalOffset;
	if (linePtr==1)
		menuItemFontHighlight->print(menuItem2, menuAnchor.x, menuAnchor.y+foo);
	else
		menuItemFont->print(menuItem2, menuAnchor.x, menuAnchor.y+foo);
	foo = 3*verticalOffset;
	if (linePtr==2)
		menuItemFontHighlight->print(menuItem3, menuAnchor.x, menuAnchor.y+foo);
	else
		menuItemFont->print(menuItem3, menuAnchor.x, menuAnchor.y+foo);
}