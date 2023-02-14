
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <Windows.h>
#include <winbase.h>
#include <glfw3.h>
#include <sys/stat.h>
#include <windows.h>
#define UNICODE
using namespace std;
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

GLuint compile_shader() {
	string dir = "C:\\Users\\johnm\\Desktop\\gpshader";
	//wstring direct = wstring(dir);
	//cout << dir;
	std::string dirV = dir + "\\vertexshader.txt";
	std::string dirC1 = dir + "\\compshaderstage1.txt";
	std::string dirC2 = dir + "\\compshaderstage2.txt";
	std::string dirF = dir + "\\fragmentshader.txt";

	ifstream vertsh;
	vertsh.open(dirV, ios::in);
	string vert;
	string vert1;
	while (getline(vertsh, vert1)) {
		//cout << vert;
		vert += "\n" + vert1;

	}
	ifstream fragmtsh;
	fragmtsh.open(dirF, ios::in);
	string frag;
	string frag1;
	while (getline(fragmtsh, frag1)) {
		frag += "\n" + frag1;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	GLuint vertshader;
	const char* vertsourp = vert.c_str();
	vertshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertshader, 1, &vertsourp, NULL);
	glCompileShader(vertshader); 

	glGetShaderiv(vertshader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertshader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> vertshaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vertshader, InfoLogLength, NULL, &vertshaderErrorMessage[0]);
		printf("%s\n", &vertshaderErrorMessage[0]);
	}

	GLuint fragshader;
	const char* fragsourp = frag.c_str();
	fragshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragshader, 1, &fragsourp, NULL);
	glCompileShader(fragshader);

	glGetShaderiv(fragshader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragshader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> fragshaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fragshader, InfoLogLength, NULL, &fragshaderErrorMessage[0]);
		printf("%s\n", &fragshaderErrorMessage[0]);
	}

	GLuint percolationShaders;
	percolationShaders = glCreateProgram();
	glAttachShader(percolationShaders, vertshader);
	glAttachShader(percolationShaders, fragshader);

	return percolationShaders;

}
GLuint createcompshader(int nos) {
	string dir = "C:\\Users\\johnm\\Desktop\\gpshader";
	std::string dirC = dir + "\\compshaderstage" + to_string (nos) +".txt";
	//cout << dirC;
	ifstream computsh(dirC, ios::in);
	string comp = "";
	if (computsh.is_open()) {
		string comp1;
		//cout << "he";
		while (getline(computsh, comp1)) {
			//cout << comp1 << "\n";
			comp += "\n" + comp1;

		}
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	GLuint compshader;
	const char* compsourp = comp.c_str();
	compshader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compshader, 1, &compsourp, NULL);
	glCompileShader(compshader);

	glGetShaderiv(compshader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(compshader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> compshaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(compshader, InfoLogLength, NULL, &compshaderErrorMessage[0]);
		printf("%s\n", &compshaderErrorMessage[0]);
	}

	GLuint percolationShaders;
	percolationShaders = glCreateProgram();
	glAttachShader(percolationShaders, compshader);
	return percolationShaders;


}
string parseLine(string line) {
	string Filename;
	Filename[0] = '\\';
	int pos = 1;
	const char* letters = line.data();
	int count = 0;
	bool space = false;
	unsigned char bar = ' ';
	for (int i = 0; i < line.length(); i++) {
		if (letters[i] == bar && space == 0) {
			space = true;
			count++;
		}
		if (letters[i] != ' ' && space == 1) {
			space = false;
		}
		if (letters[i] != ' ' && count == 3) {
			Filename.append(letters, i, 1);
			pos++;
		}
	}
	Filename.erase(remove(Filename.begin(), Filename.end(), '\n'), Filename.end());
	return Filename;
}

store<string> readDir(string Address) {
	string prompt = "cd " + Address + " & dir";
	const char* request = prompt.data();
	store<string> File = store<string>(1, 0);
	char line[100];
	int count = 0;
	FILE* command = _popen(request, "r");
	while (fgets(line, 100, command)) {
		if (count > 7) {
			File.add(count - 8, parseLine(line));
		}
		count++;
	}
	File.length1 += -2;
	return File;
}
template<typename gen>
void save_data(store<int> bridged,store<int> new_points,store<float> ratioed,store<gen> maximum,store<float> ensemble,store<int> setup_data) {
	string Address = "C:\\Users\\johnm\\Desktop\\perc_data\\";
	store<string> folders = readDir(Address);
	string command = "md " + Address + to_string(folders.length1+0);
	system(command.c_str());
	Address += to_string(folders.length1 +0) + "\\";
	for (int i = 0; i < bridged.length1; i++) {
		ofstream bridge;
		string brid = Address + "bridges.txt";
		bridge.open(brid, ios::app);
		string br = to_string(bridged.get(i)) + "\n";
		bridge << br.data();
		bridge.close();
	}
	for (int i = 0; i < ratioed.length1; i++) {
		ofstream ratio;
		string rat = Address + "ratios.txt";
		ratio.open(rat, ios::app);
		string rt = to_string(ratioed.get(i))+ "\n";
		ratio << rt.data();
		ratio.close();
	}
for (int i = 0; i < maximum.length1; i++) {
	ofstream max;
	string maxi = Address + "max.txt";
	max.open(maxi, ios::app);
	string mx = to_string(maximum.get(i)) + "\n";
	max << mx.data();
	max.close();
}
for (int i = 0; i < setup_data.length1; i++) {
	ofstream set;
	string seti = Address + "setup.txt";
	set.open(seti, ios::app);
	string setup = to_string(setup_data.get(i)) + "\n";
	set << setup.data();
	set.close();
}
for (int i = 0; i < ensemble.length1; i++) {
	ofstream ens;
	string en = Address + "ensemble.txt";
	ens.open(en, ios::app);
	string ensi = to_string(ensemble.get(i)) + "\n";
	ens << ensi.data();
	ens.close();
}
	int count = 0;
	string pointer = "new_points0.txt";
	for (int i = 0; i < new_points.length1; i++) {
		ofstream newpoints;
		string points = Address + pointer;
		newpoints.open(points, ios::app);
		newpoints.flush();
		if (new_points.get(i) != -1) {
		newpoints << new_points.get(i) << "\n";
		}
		else {
			count++;
			pointer = "new_points" + to_string(count) + ".txt";
		}
	}






}

