// gputest1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <Windows.h>
#include <winbase.h>
#include <sys/stat.h>
#include <windows.h>
#include <iostream>
#include <glad/gl.h>
//#include "glfw3.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <random>
#include "storage.cpp"
#include <glfw3.h>
#include <iostream>



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
float angleadjust = 0;
float fb = 0;
float xpos = 0;
float ypos = 20;
float zpos = -3;
// function to create text from glerror code
void glerrorstring(GLenum error) {
	switch (error) {
	case GL_NO_ERROR:
		cout << "no error";
		break;
	case GL_INVALID_ENUM:
		cout << "invalid enum";
		break;
	case GL_INVALID_VALUE:
		cout << "invalid value";
		break;
	case GL_INVALID_OPERATION:
		cout << "invalid operation";
		break;
	case GL_STACK_OVERFLOW:
		cout << "stack overflow";
		break;
	case GL_STACK_UNDERFLOW:
		cout << "stack underflow";
		break;
	case GL_OUT_OF_MEMORY:
		cout << "out of memory";
		break;
	};
};
//function to take orientation and movement input from user
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
	angleadjust += 0.1;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
	angleadjust -= 0.1;
	}
	if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
		fb -= 0.3;
	}
	if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
		fb += 0.3;
	}
}
void printMap(store<int> map) {
	map.length1 = 16;
	map.length2 = 16;
	map.length3 = 16;
	for (int i = 1; i < 16; i++) {
		for (int e = 1; e < 16; e++) {
			for (int z = 0; z < 1; z++) {
				cout << map.get(i,e,0) << " ";
			}
		}
		cout << "\n";
	}
};
// function to generate random 16x16x16 grid of cubes of certain colors (represented by assigned number)
store<int> genMap() {
	store<int> map = store<int>(3,16,16,16);
	for (int i = 0; i < 16; i++) {
		for (int e = 0; e < 16; e++) {
			for (int z = 0; z < 16; z++) {
				map.add(i, e, z, 0);
				if (z == 0) {
					map.add(i,e,z,rand()%3 + 1);
				}
				else if (rand() % 10 >8) {
					map.add(i, e,z, rand() % 3 + 1);
				}
			}
		}
	}

	return map;

}
//generates data for a buffer that will be used to store high level data for a face3 in this voxel case a face is just a face of a cube and the data stored is just used to determine an efficent number of samples
store<int> initFaceBuffer() {
	store<int> faces = store<int>(1, 0);
	for (int i = 0; i < 4*10000 +1; i++) {
		//currently only needs to be zeroed
		faces.add(i,0);
	}
	return faces;
}
// generate data for an rgb texel array with depth paramter which is set to 300
store<int> initexels() {
	store<GLint> texels = store<GLint>(3,1920,1080,4);
	for(int i = 0; i < 1920; i++) {
		for (int e = 0; e < 1080; e++) {
			texels.add(i, e, 0,0);
			texels.add(i, e, 1,0);
			texels.add(i, e, 2,0);
			texels.add(i, e, 3, 300);

		}
	
	}
	return texels;
}
// need to generate data for a buffer that will contain values like fov and use position/orientation
store<float> genusefulfl(float angle) {
	store<GLfloat> ufl = store<GLfloat>(1, 0);
	ufl.add(1,cos(angle));
	ufl.add(0,sin(angle));
	xpos += -sin(angle) * fb;
	ypos += cos(angle) * fb;
	fb = 0;
	ufl.add(2,xpos);
	ufl.add(3, ypos);
	ufl.add(4, zpos);
	return ufl;
}
//function to retrieve data from opengl buffers mostly used for debug
store<int> readbuffer(void* point, int length) {
	GLuint* pnt = (GLuint*)point;
	store<int> buf = store<int>(1, length);
	for (int i = 0; i < length; i++) {
		buf.add(i, int(*(pnt + i)));

	}
	return buf;

}
int main(void)
{
	string Address = "C:\\Users\\johnm\\Desktop\\perc_data\\";
	store<string> folders = readDir(Address);
	string command = "md " + Address + to_string(folders.length1 + 1);
	system(command.c_str());
	// standard opengl initiation code follows
if (!glfwInit())
{
	fprintf(stderr, "Failed to initialize GLFW\n");
	getchar();
	return -1;
}

GLFWwindow* window;
glfwWindowHint(GLFW_SAMPLES, 4);
glfwWindowHintString(GLFW_MAXIMIZED, GLFW_FALSE);
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

window = glfwCreateWindow(1920, 1080, "Render system prototype v1(voxel)", NULL, NULL);
if (window == NULL) {
	getchar();
	glfwTerminate();
	return -1;
}
glfwMakeContextCurrent(window);
int version = gladLoadGL(glfwGetProcAddress);

if (version == 0) {
	printf("Failed to initialize OpenGL context\n");
	return -1;
}
else {
	cout << version << "\n";
}
//glad is used not glew
//glewExperimental = true;
//if (glewInit() != GLEW_OK) {
	//fprintf(stderr, "Failed to initialize GLEW\n");
	//getchar();
	//glfwTerminate();
	//return -1;
//}
//set input mode for user control
glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

// this code uses a pair of triangles with the texel array output to render to the screen the buffer generated below are used for the triangle vertices
GLuint VertexArrayID;
glGenVertexArrays(1, &VertexArrayID);
glBindVertexArray(VertexArrayID);



GLuint vertexbuffer;

glGenBuffers(1, &vertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
// buff data;
//store<GLint> texels = initLatticeState(size)-

//initialize texel data and generate texture binding
GLuint texadd;

store<GLint>  texels = initexels();
cout << texels.Array[500];
glGenTextures(1, &texadd);

cout << texadd;
glerrorstring(glGetError());


//initiate map and face data
store<int> map = genMap();
store<GLint> faces = initFaceBuffer();
//cout << "hey";
//vGLint> sizer = { 1,1,1,1 };
//store<GLint> lattice = latticeData(size);
GLuint shaderdata[3];
glGenBuffers(3, shaderdata);
// the map is static and consequantly is only sent to the gpu once on startup so this step is placed outside the main loop
// the map is sent to the gpu using the below buffer operations
glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderdata[0]);
//cout << lattice.get(50,50);
glBufferData(GL_SHADER_STORAGE_BUFFER, 16*16*16* sizeof(int), &map.Array[0], GL_STATIC_READ);
glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, shaderdata[0],0, 16*16*16* sizeof(GLint));
// this rendering engine uses 4 shaders to compute shaders which loosely speaking replace the vertex and fragment shaders and 2 normal vertex and fragment shaders which are used to conveinienttly output
//the results of the custom shaders to the screen
// the two normal shaders can be compiled and linked together whilst the 2 compute shaders must be comiled and linked separately 
GLuint programID = compile_shader();
GLuint compstage1 = createcompshader(1);
GLuint compstage2 = createcompshader(2);
glLinkProgram(programID);
glLinkProgram(compstage1);
glLinkProgram(compstage2);

int count = 0;
printMap(map);
do {
	void* buf = glMapNamedBuffer(shaderdata[0], GL_READ_ONLY);
	store<int> but = readbuffer(buf, 16 * 16 * 16);
	//cout << "\n";
	//printMap(but);
	glUnmapNamedBuffer(shaderdata[0]);
	// the non static buffers need to be reinitialzed every frame so are setup here.
	glBindTexture(GL_TEXTURE_2D, texadd);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32I, 1920, 1080, 0, GL_RGBA_INTEGER, GL_INT, &texels.Array[0]);
	GLuint* point = NULL;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindImageTexture(0, texadd, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32I);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderdata[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (100 * 100 * 4 + 1) * sizeof(GLint), &faces.Array[0], GL_DYNAMIC_READ);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, shaderdata[1], 0, (100 * 100 * 4   + 1)*sizeof(GLint));
	store<float> usefulFloats = genusefulfl(angleadjust);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderdata[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 5 * sizeof(GLfloat), &usefulFloats.Array[0], GL_DYNAMIC_READ);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, shaderdata[2], 0, 5 * sizeof(GLfloat));
	//glerrorstring(glGetError());
	
	// the first compute shader is dispatched. This compute shader samples larger areas of the map to determine how many pixels are in a given region etc 
	//in this voxel cube case this shader analyizes individual faces in the map to determine if they should be culled or how many samples do they need to render them on screen 
	//without holes,other artifacts or inefficient static sampling rates. 
	// this data is stored in the faces buffer which will be accessed by the next compute shader

	glUseProgram(compstage1);
	glDispatchCompute(1, 1, 1);
	//glerrorstring(glGetError());
	//in order to dispatch the right number of compute shader instances are need for the second compute shader it is neccessary to read back data from the results of the first shader
	// as this is only dynamic on the cpu side
	void* bufmap = glMapNamedBuffer(shaderdata[1], GL_READ_ONLY);
	store<int> bu = readbuffer(bufmap, 200);
	float nosF = bu.get(0);
	//cout << "\n";
	//printMap(bu);
	for (int op = 0; op < 0; op++) {
		//if (bu.get(op) < -3) {
			cout << bu.get(op) << " " << (op-1)%4 << "\n";
		//}
	}
	glUnmapNamedBuffer(shaderdata[1]);
	glUseProgram(compstage2);
	glDispatchCompute(nosF, 1, 1);
	GLint g_vertex_buffer_data[] = { -1,-1,0,   0, 0,   1,-1,0,   1, 0,  -1,1,0,  0, 1,  1,1,0,   1, 1,   1,-1,0,  1, 0,   -1,1,0,   0, 1 };
	//glerrorstring(glGetError());
	glUseProgram(programID);

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLint), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 5 * sizeof(GLint), (void*)0);
	int siz = 3 * sizeof(GLint);
	glVertexAttribPointer(1, 2, GL_INT, GL_FALSE, sizeof(GLint) * 5, (GLvoid*)siz);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glfwSwapBuffers(window);
	glfwPollEvents();
	glfwSetKeyCallback(window, key_callback);
}
while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && 
	glfwWindowShouldClose(window) == 0);
glDeleteBuffers(1, &vertexbuffer);
glDeleteBuffers(4, &shaderdata[0]);
glDeleteBuffers(2, &texadd);
glDeleteVertexArrays(1, &VertexArrayID);
glDeleteProgram(programID);
glfwTerminate();
}
// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
