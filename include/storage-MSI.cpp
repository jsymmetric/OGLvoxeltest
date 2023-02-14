
template<typename Type>
class store {
	//vector<Type>Array;
public:
	vector <Type> Array;
	int dimension;
	int length1;
	int length2;
	int length3;
	store() {
		int length1 = 0;
		int length2 = 0;
		int length3 = 0;
	};
	store(int dim, int len) {
		Array.resize(len);
		dimension = dim;
		length1 = len;
	};
	store(int dim, int len, int len2) {
		Array.resize(len*len2);
		dimension = dim;
		length1 = len;
		length2 = len2;
	};
	store(int dim, int len, int len2, int len3) {
		Array.resize(len*len2*len3);
		dimension = dim;
		length1 = len;
		length2 = len2;
		length3 = len3;
	};
	void add(Type value) {
		if (length1 < 0) {
			length1 = 0;
		};
		Array.resize(length1+1);
		Array[length1] = value;
		length1++;
	};
	void add(int pos, Type value) {
		if (pos >= length1) {
			length1 = pos + 1;
			Array.resize(length1);
		}
		Array[pos] = value;
	};
	void add(int pos, int pos2, Type value) {
		Array[pos*length2 + pos2] = value;
	};
	void add(int pos, int pos2, int pos3, Type value) {
		Array[pos*length2*length3 + pos2*length3 + pos3] = value;
	};
	Type get(int pos) {
		return Array[pos];
	};
	Type get(int pos, int pos2) {
		return Array[pos*length2 + pos2];
	};
	Type get(int pos, int pos2, int pos3) {
		return Array[pos*length2*length3 + pos2*length3 + pos3];
	};
};

class block {
	int x = 0;
	int y = 0;
	int z = 0;
	Cube box;


public:
	string texture;
	int texcoord;
	void setCoords(int xx, int yy, int zz) {
		z = zz;
		y = yy;
		x = xx;
	};
	float* getTexcoords() {
		float texcods[108];
		for (int i = 0; i < 36; i++) {
			texcods[i] = box.vertexs[i];
			texcods[i + 1] = box.vertexs[i + 1];
			texcods[i + 2] = texcoord;
		}
		return texcods;

	};
	float* getVertexs() {
		int count = 0;
		//cout << "start" << endl;
		float chunkVer[108];
		//cout << "end" << endl;
		for (int yy = 0; yy < 108; yy++) {

			if (count == 0) {
				int Y = box.vertexs[yy];
				chunkVer[yy] = Y + x;
			};
			if (count == 1) {
				int X = box.vertexs[yy] + z - 2;
				chunkVer[yy] = X;
			};
			if (count == 2) {
				int Z = box.vertexs[yy];
				chunkVer[yy] = Z + y;
			};
			if (count == 2) {
				count = 0;
			}
			else {
				count++;
			}
		}
		return chunkVer;
	}

};

class picture{
public:
	store<GLubyte> texture = store<GLubyte>(2,16,16);
	string name;
};

class imsearch {
	store<picture> list;
public:
	imsearch(store<picture> in){
		list = in;
	}
int	find(string tofind) {
	int index = -1;
	for(int i = 0; i < list.length1; i++) {
		if(tofind == list.get(i).name) {
			index = i;
	}
	}
	return index;
	}
};
class fragment {
public:
	fragment() {};
	int num_of_tex = 0;
	store<int> tex_grid;
	store<float> mesh = store<float>(1,36);
	//int cornercod[3];
	float xlen;
	float ylen;
	int ybound;
	int xbound;
	fragment(store<int> to_convert,int x,int y,int z,int w,int level) {
		//cornercod[0] = x;
		//cornercod[1] = y;
		//cornercod[2] = z;
		xbound = to_convert.length1;
		ybound = to_convert.length2;
		xlen = (float)to_convert.length1/xbound;
		ylen = (float)to_convert.length2/ybound;
		//cout << level << endl;
		// Check later
		if (w  ==  0 || w == 1) {
			float coordinates[36] = {x,y,z,
			0,0,level,
			x + to_convert.length1,y,z,
			xlen,0,level,
			x,y + to_convert.length2,z,
			0,ylen,level,
			x + to_convert.length1,y + to_convert.length2,z,
			xlen,ylen,level,
			x + to_convert.length1,y,z,
			xlen,0,level,
			x,y + to_convert.length2,z,
			0,ylen,level};
			mesh.Array.assign(coordinates,coordinates+36);
		}
		if (2 == w || w== 3) {
			float coordinates[36] = { x,y,z,
			0,0,level,
			x + to_convert.length1,y,z,
			xlen,0,level,
			x,y,z + to_convert.length2,
			0,ylen,level,
			x + to_convert.length1,y,z + to_convert.length2,
			xlen,ylen,level,
			x + to_convert.length1,y,z,
			xlen,0,level,
			x,y,z + to_convert.length2,
			0,ylen,level};
			mesh.Array.assign(coordinates, coordinates + 36);
		}
		if (4 == w || w == 5) {
			float coordinates[36] = {x,y,z,
			0,0,level,
			x,y + to_convert.length1,z,
			xlen,0,level,
			x,y,z + to_convert.length2,
			0,ylen,level,
			x,y+to_convert.length1,z+to_convert.length2,
			xlen,ylen,level,
			x,y + to_convert.length1,z,
			xlen,0,level,
			x,y,z + to_convert.length2,
			0,ylen,level};
			mesh.Array.assign(coordinates, coordinates + 36);
		}
		tex_grid = to_convert;
	}
};