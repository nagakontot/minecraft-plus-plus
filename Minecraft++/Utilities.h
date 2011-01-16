#define pi 3.1415926535897932384626433832795
#define degtorad 0.01745329251994329576923690768489
#define radtodeg 57.295779513082320876798154814105

inline void GLCHECK(){
	GLenum e = glGetError();
	switch(e){
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		cout << "GL_INVALID_ENUM" << endl;
		break;
	case GL_INVALID_VALUE:
		cout << "GL_INVALID_VALUE" << endl;
		break;
	case GL_INVALID_OPERATION:
		cout << "GL_INVALID_OPERATION" << endl;
		break;
	case GL_STACK_OVERFLOW:
		cout << "GL_STACK_OVERFLOW" << endl;
		break;
	case GL_STACK_UNDERFLOW:
		cout << "GL_STACK_UNDERFLOW" << endl;
		break;
	case GL_OUT_OF_MEMORY:
		cout << "GL_OUT_OF_MEMORY" << endl;
		break;
	case GL_TABLE_TOO_LARGE:
		cout << "GL_TABLE_TOO_LARGE" << endl;
		break;
	}
}

template <class T>
const inline std::string tostring (const T& t) {
	std::stringstream ss;
	ss << t;
	return ss.str();
}
const inline std::string tostring(const int& t) {
	char buffer[15];
	sprintf(buffer,"%i",t);
	return (string)buffer;
}
const inline std::string tostring(const double& t) {
	char buffer[15];
	sprintf(buffer,"%f",t);
	return (string)buffer;
}
template <class T>
const inline double toreal (const T& t) {
	std::stringstream ss;
	ss << t;
	double d;
	ss >> d;
	return d;
}
const inline double sqr (double x) {
	return x*x;
}
const inline double ldx (double len, double dir) {
	return cos(dir*degtorad)*len;
}
const inline double ldy (double len, double dir) {
	return sin(dir*degtorad)*len;
}
const inline uint64_t dif(uint64_t a, uint64_t b){
	if(a>b){
		return a-b;
	} else {
		return b-a;
	}
}
const inline double pdis (double x1, double y1, double x2, double y2) {
	return sqrt(sqr(x1-x2)+sqr(y1-y2));
}
const inline double pdis (uint64_t x1, uint64_t y1, uint64_t z1, uint64_t x2, uint64_t y2, uint64_t z2) {
	return sqrt(sqr(dif(x1,x2))+sqr(dif(y1,y2))+sqr(dif(z1,z2)));
}
const inline double pdir (double x1, double y1, double x2, double y2) {
	return radtodeg*atan2(y2-y1,x2-x1);
}
const inline double angdif (double angle1, double angle2) {
	return fmod(fmod(angle1-angle2,360)+540,360)-180;
}
template <class T>
const inline int8_t sign(T x) {
	return (x>0)-(x<0);
}
template <class T>
const inline T closer(T x, T y, T c){
	if(abs(x-c)<abs(y-c)){
		return x;
	} else {
		return y;
	}
}
template <class T>
const inline T farther(T x, T y, T c){
	if(abs(x-c)<abs(y-c)){
		return y;
	} else {
		return x;
	}
}
const inline double psqdis (double x1, double y1, double x2, double y2) {
	return sqr(x1-x2)+sqr(y1-y2);
}
inline double random(double x) {
	return double(rand())/RAND_MAX*x;
}
const inline uint32_t rhash(uint8_t* x, uint32_t len) {
	uint32_t hash, i;
	for(hash = i = 0; i < len; ++i){
		hash += x[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}
template <class T1>
const inline uint32_t random(T1 s1) {
	uint8_t x[sizeof(T1)];
	uint32_t s = 0;
	*(T1*)(x+s) = s1;
	s += sizeof(T1);
	return max(uint32_t(1),rhash(x, s));
}
template <class T1, class T2>
const inline uint32_t random(T1 s1, T2 s2) {
	uint8_t x[sizeof(T1)+sizeof(T2)];
	uint32_t s = 0;
	*(T1*)(x+s) = s1;
	s += sizeof(T1);
	*(T2*)(x+s) = s2;
	s += sizeof(T2);
	return rhash(x, s);
}
template <class T1, class T2, class T3>
const inline uint32_t random(T1 s1, T2 s2, T3 s3) {
	uint8_t x[sizeof(T1)+sizeof(T2)+sizeof(T3)];
	uint32_t s = 0;
	*(T1*)(x+s) = s1;
	s += sizeof(T1);
	*(T2*)(x+s) = s2;
	s += sizeof(T2);
	*(T3*)(x+s) = s3;
	s += sizeof(T3);
	return rhash(x, s);
}
template <class T1, class T2, class T3, class T4>
const inline uint32_t random(T1 s1, T2 s2, T3 s3, T4 s4) {
	uint8_t x[sizeof(T1)+sizeof(T2)+sizeof(T3)+sizeof(T4)];
	uint32_t s = 0;
	*(T1*)(x+s) = s1;
	s += sizeof(T1);
	*(T2*)(x+s) = s2;
	s += sizeof(T2);
	*(T3*)(x+s) = s3;
	s += sizeof(T3);
	*(T4*)(x+s) = s4;
	s += sizeof(T4);
	return rhash(x, s);
}
template <class T1, class T2, class T3, class T4, class T5>
const inline uint32_t random(T1 s1, T2 s2, T3 s3, T4 s4, T5 s5) {
	uint8_t x[sizeof(T1)+sizeof(T2)+sizeof(T3)+sizeof(T4)+sizeof(T5)];
	uint32_t s = 0;
	*(T1*)(x+s) = s1;
	s += sizeof(T1);
	*(T2*)(x+s) = s2;
	s += sizeof(T2);
	*(T3*)(x+s) = s3;
	s += sizeof(T3);
	*(T4*)(x+s) = s4;
	s += sizeof(T4);
	*(T5*)(x+s) = s5;
	s += sizeof(T5);
	return rhash(x, s);
}
template <class T1, class T2, class T3, class T4, class T5, class T6>
const inline uint32_t random(T1 s1, T2 s2, T3 s3, T4 s4, T5 s5, T6 s6) {
	uint8_t x[sizeof(T1)+sizeof(T2)+sizeof(T3)+sizeof(T4)+sizeof(T5)+sizeof(T6)];
	uint32_t s = 0;
	*(T1*)(x+s) = s1;
	s += sizeof(T1);
	*(T2*)(x+s) = s2;
	s += sizeof(T2);
	*(T3*)(x+s) = s3;
	s += sizeof(T3);
	*(T4*)(x+s) = s4;
	s += sizeof(T4);
	*(T5*)(x+s) = s5;
	s += sizeof(T5);
	*(T6*)(x+s) = s6;
	s += sizeof(T6);
	return rhash(x, s);
}
inline bool file_exists(string filename) {
	ifstream file(filename, ios_base::binary);
	bool answer = file.is_open();
	file.close();
	return answer;
}