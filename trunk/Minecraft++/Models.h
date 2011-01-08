void InitModels();

//const float MT = MAX_TEXTURES;
#define MT(x) (float(x)+0.5)/MAX_TEXTURES

/* Row 1: side
 * Row 2: side
 * Row 3: side
 * Row 4: side
 * Row 5: bottom
 * Row 6: top    */


const GLfloat T_QUAD[8] =	{1,0,0,0,0,1,1,1};
const GLfloat T_STONE[6] =	{0,0,0,0,0,0};
const GLfloat T_DIRT[6] =	{1,1,1,1,1,1};
const GLfloat T_GRASS[6] =	{2,2,2,2,1,3};
const GLfloat T_COBBLE[6] = {4,4,4,4,4,4};
const GLfloat T_LEAVES[6] = {5,5,5,5,5,5};
const GLfloat T_LOGS[6] =	{6,6,6,6,7,7};
const GLfloat T_PLANKS[6] = {8,8,8,8,8,8};
const GLfloat T_GRAVEL[6] = {9,9,9,9,9,9};
const GLfloat T_SAND[6] =	{10,10,10,10,10,10};
const GLfloat T_GLASS[6] =	{11,11,11,11,11,11};
const GLshort M_XP[12] = {1,0,0,1,1,0,1,1,1,1,0,1};
const GLshort M_XN[12] = {0,1,0,0,0,0,0,0,1,0,1,1};
const GLshort M_YP[12] = {1,1,0,0,1,0,0,1,1,1,1,1};
const GLshort M_YN[12] = {0,0,0,1,0,0,1,0,1,0,0,1};
const GLshort M_ZP[12] = {0,0,1,1,0,1,1,1,1,0,1,1};
const GLshort M_ZN[12] = {0,1,0,1,1,0,1,0,0,0,0,0};