#include "Global.h"

sf::Window Window(sf::VideoMode(800,600,32),"Minecraft++",sf::Style::Close|sf::Style::Titlebar,sf::WindowSettings(32,0,0));

bool Game::Init() {
	InitBlocks();
	return true;
}
bool Game::Loop() {
	sf::Event e;
	while(Window.GetEvent(e)){
		switch(e.Type){
		case sf::Event::Closed:
			return false;
		}
	}
	return true;
}
void Game::Unload() {

}