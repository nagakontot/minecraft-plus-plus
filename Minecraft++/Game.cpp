#include "Global.h"



bool Game::Init() {
	InitBlocks();
	Window.Create(sf::VideoMode(800,600,32),"Minecraft++",sf::Style::Close|sf::Style::Titlebar,sf::ContextSettings(24,0,0));
	InitGraphics();
	srand(clock());
	return true;
}
bool Game::Loop() {
	//Event handling
	sf::Event e;
	while(Window.GetEvent(e)){
		switch(e.Type){
		case sf::Event::Closed:
			return false;
		}
	}
	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	//Draw everything
	//Display the screen
	Window.Display();
	return Window.IsOpened();
}
void Game::Unload() {

}