#include "Global.h"



bool Game::Init() {
	InitBlocks();
	Window.Create(sf::VideoMode(800,600,32),"Minecraft++",sf::Style::Close|sf::Style::Titlebar,sf::ContextSettings(24,0,0));
	srand(clock());
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
	uint64_t n1, n2;
	cin >> n1 >> n2;
	cout << random(n1, n2) << endl;
	return true;
}
void Game::Unload() {

}