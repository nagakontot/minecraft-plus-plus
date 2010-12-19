extern sf::Window Window;

void InitGraphics();

GLuint GetTexture(string name);
uint8_t* GetPixels(string name);
void BindTexture(GLuint tex);
const int TEXTURE_SIZE = 32;
const int MAX_TEXTURES = 32;

extern map<string, sf::Image> textures;
extern GLuint bound;
extern GLuint TEX;