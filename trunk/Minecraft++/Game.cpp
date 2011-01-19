#include "Global.h"

bool Game::Active = true;
bool Game::Done = false;
double fps = 0;
double tdelta = 0;
double delta = 0;
uint64_t ticks = 10;
uint16_t Game::Range = 10;
bool Game::Online = false;

bool Game::Init() {
	if(!bf::exists("save")){
		bf::create_directory("save");
	}
	//Load settings
	po::options_description desc("Allowed options");
	desc.add_options()
    ("range", po::value<uint16_t>(&Range)->default_value(10), "Change view range")
	("online", po::value<bool>(&Online)->default_value(false), "Change whether the game is online or not")
    ("genspeed", po::value<uint16_t>(&GenSpeed)->default_value(10), "Change generation speed");
	po::variables_map vm;
	po::store(po::parse_config_file<char>("config.ini", desc, false), vm);
	po::notify(vm);
	cout << "View Range is " << Range << endl;
	cout << "Chunk Generation Speed is " << GenSpeed << endl;
	InitBlocks();
	InitGen();
	SkyInit();
	Window.Create(sf::VideoMode(800,600,32),"Infinity Cubed",sf::Style::Close|sf::Style::Titlebar,sf::ContextSettings(24,0,0,3,3));
	Window.ShowMouseCursor(false);
	//Window.SetFramerateLimit(30);
	InitGraphics();
	Window.SetActive();
	srand(clock());
	return true;
}
bool Game::Loop() {
	//Event handling
	sf::Event e;
	int mbut = 0;
	while(Window.GetEvent(e)){
		switch(e.Type){
		case sf::Event::Closed:
			Window.Close();
			break;
		case sf::Event::KeyPressed:
			switch(e.Key.Code){
			case sf::Key::Escape:
				Window.Close();
				break;
			case sf::Key::Equal:
				Range++;
				break;
			case sf::Key::Subtract:
				Range--;
				break;
			}
		case sf::Event::GainedFocus:
			Active = true;
			break;
		case sf::Event::LostFocus:
			Active = false;
			break;
		case sf::Event::MouseButtonPressed:
			switch(e.MouseButton.Button){
			case sf::Mouse::Left:
				mbut = 1;
				break;
			case sf::Mouse::Right:
				mbut = 2;
				break;
			}
			break;
		case sf::Event::MouseWheelMoved:
			Inv.Selected = ((Inv.Selected-1+e.MouseWheel.Delta)%Inv.Total+Inv.Total)%Inv.Total+1;
			break;
		}
	}
	//Update stuff
	player.Step();
	GenChunks();
	UpdateChunks();
	UnloadChunks();
	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	//Projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(double)Window.GetWidth()/Window.GetHeight(),0.01,1000);
	gluLookAt(0,0,0,ldx(1,player.rot.d)*ldx(1,player.rot.p),ldy(1,player.rot.d)*ldx(1,player.rot.p),ldy(1,player.rot.p),0,0,-1);
	glMatrixMode(GL_MODELVIEW);
	SkyDraw();
	glMatrixMode(GL_PROJECTION);
	glTranslated(-player.pos.x,-player.pos.y,-player.pos.z+player.eyeh);
	glMatrixMode(GL_MODELVIEW);
	//Draw everything
	if(ticks%30==0){//Every so often run through and clean up the chunk list
		Chunks.clear();
		for(uint8_t a=0;a<=Range*2;a++){
			for(uint8_t b=0;b<=Range*2;b++){
				for(uint8_t c=0;c<=Range*2;c++){
					GetChunk(player.pos.cx+a-Range,player.pos.cy+b-Range,player.pos.cz+c-Range,true);
				}
			}
		}
		for(auto x=ChunkPos.begin();x!=ChunkPos.end();x++){
			for(auto y=x->second.begin();y!=x->second.end();y++){
				for(auto z=y->second.begin();z!=y->second.end();z++){
					Chunk* c=z->second;
					if(c!=0){
						double d = pdis(player.pos.cx,player.pos.cy,player.pos.cz,c->x,c->y,c->z);
						if(d<Range){
							if(c->verts>0){
								Chunks.insert(c);
							}
						} else if(d>Range*2){
							AddChunkUnload(c);
						}
					}
				}
			}
		}
		sort(ChunksToGen.begin(),ChunksToGen.end(),ChunkComp);
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_3D,TEX);
	for(auto i=Chunks.begin(); i!=Chunks.end(); i++){
		(*i)->Draw();
	}
	glBindTexture(GL_TEXTURE_3D,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_DEPTH_TEST);
	player.EditBlocks(mbut);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1,1,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Draw the interface
	glBegin(GL_LINES);
	glVertex2f(0.49,0.5);
	glVertex2f(0.51,0.5);
	glVertex2f(0.5,0.49);
	glVertex2f(0.5,0.51);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	//Display the screen
	Window.Display();
	fps = fps*0.8+0.2/Window.GetFrameTime();
	tdelta += Window.GetFrameTime();
	delta = min(max((double)Window.GetFrameTime(),0.0001),0.1);
	ticks++;
	if(ticks%10==0){
		//cout << fps << endl;
	}
	GLCHECK();
	return Window.IsOpened();
}
void Game::Unload() {
	Done = true;
	for(auto x=ChunkPos.begin();x!=ChunkPos.end();x++){
		for(auto y=x->second.begin();y!=x->second.end();y++){
			for(auto z=y->second.begin();z!=y->second.end();z++){
				Chunk* c=z->second;
				if(c!=0){
					AddChunkUnload(c);
				}
			}
		}
	}
	UnloadChunks();
}