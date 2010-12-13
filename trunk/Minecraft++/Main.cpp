#include "Global.h"

int main() {
	if(!Game::Init()){return 1;}
	while(Game::Loop()){}
	Game::Unload();
	return 0;
}