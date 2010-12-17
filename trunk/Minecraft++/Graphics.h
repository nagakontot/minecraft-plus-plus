extern sf::Window Window;

void InitGraphics();

GLuint GetTexture(string name);
void BindTexture(GLuint tex);

extern map<string, sf::Image> textures;
extern GLuint bound;