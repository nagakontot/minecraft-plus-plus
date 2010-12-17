void InitModels();

const GLshort T_QUAD[8] = {1,0,0,0,0,1,1,1};
const GLshort M_XP[12] = {1,0,0,1,1,0,1,1,1,1,0,1};
const GLshort M_XN[12] = {0,1,0,0,0,0,0,0,1,0,1,1};
const GLshort M_YP[12] = {1,1,0,0,1,0,0,1,1,1,1,1};
const GLshort M_YN[12] = {0,0,0,1,0,0,1,0,1,0,0,1};
const GLshort M_ZP[12] = {0,0,1,1,0,1,1,1,1,0,1,1};
const GLshort M_ZN[12] = {0,0,0,0,1,0,1,1,0,1,0,0};