/*
Type your name and student ID here
	- Name: Chu Kai Chun
	- Student ID: 1155093260
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <math.h>
#include <iostream>
#include <fstream>

// some constants for view, projection and transformations
GLint programID;
float x_delta = 0.5f;
int x_press_num = 0;
int z_press_num = 0;
float z_delta = 0.5f;
float y_delta = 0.5f;
int scale_press_num = 0;
float scale_delta = 0.5f;
int rotation_press_num = 0;
float rotation_delta = 2.0f;
int view_rotation = 0;
float FoV = 45.0f;
int cloudX_press_num = 0, cloudY_press_num = 0, cloudScale_press_num = 10;


void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		std::cout << "File failed to load ... " << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	std::string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

GLuint vaoID;
GLuint vertexBuffer;

GLuint vaoID2;
GLuint vertexBuffer2, indexBuffer2;

GLuint vaoID3;
GLuint vertexBuffer3, indexBuffer3;

GLuint vaoID4;
GLuint vertexBuffer4, indexBuffer4;

GLuint vaoID5;
GLuint vertexBuffer5, indexBuffer5;

void sendDataToOpenGL() {
    // TODO:
    // create 2D objects and 3D objects and/or lines (points) here and bind to VAOs & VBOs
      //#declare Brown = color red 0.647059 green 0.164706 blue 0.164706
      //declare SkyBlue = color red 0.196078 green 0.6 blue 0.8

    const GLfloat landAndSky[] = {

    -40.0f, -0.0f , -40.0f,      //Land
    +0.761f, +0.698f , +0.502f,

    -40.0f, -0.0f , +40.0f,
     +0.761f, +0.698f , +0.502f,

    +40.0f, +0.0f , +40.0f,
    +0.761f, +0.698f , +0.502f,

    -40.0f, -0.0f , -40.0f,
    +0.761f, +0.698f , +0.502f,
    +40.0f, +0.0f , +40.0f,
     +0.761f, +0.698f , +0.502f,

    +40.0f, +0.0f , -40.0f,
    +0.761f, +0.698f , +0.502f,

    -40.0f, +50.0f, -40.0f, //Front Sky
    +0.196078f, 0.6f, 1.0f,

    -40.0f, +0.0f , -40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +0.0f , -40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +50.0f, -40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +0.0f , -40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +50.0f, -40.0f,
    +0.196078f, 0.6f, 0.9f,


    -40.0f, +50.0f, +40.0f, //Left Sky
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +0.0f , +40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +0.0f , -40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +50.0f, +40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +0.0f , -40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +50.0f, -40.0f,
    +0.196078f, 0.6f, 0.8f,


    +40.0f, +50.0f, -40.0f, //Right Sky
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +0.0f , -40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +0.0f , +40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +50.0f, -40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +0.0f , +40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +50.0f, +40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +50.0f, +40.0f, //Back Sky
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +0.0f , +40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +0.0f , +40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +50.0f, +40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +0.0f , +40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +50.0f, +40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +50.0f, +40.0f, //Top Sky
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +50.0f, +40.0f,
    +0.196078f, 0.6f, 0.8f,


    +40.0f, +50.0f, -40.0f,
    +0.196078f, 0.6f, 0.8f,

    +40.0f, +50.0f, -40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +50.0f, +40.0f,
    +0.196078f, 0.6f, 0.8f,

    -40.0f, +50.0f, -40.0f, 
    +0.196078f, 0.6f, 0.8f,

    };



    //First VAO for the landscape made with 2D Objects
    glGenVertexArrays(1, &vaoID); 
    glBindVertexArray(vaoID);  


    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(landAndSky), landAndSky, GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


    const GLfloat pyramids[] = {

        +5.0f, +4.0f, -8.0f,   //Pyramid 1 pos 0 
        +0.761f, +0.698f , +0.502f,

        +3.0f, +0.0f, -10.0f,  //pos 1 
        +0.661f, +0.598f , +0.402f,

        +3.0f, +0.0f, -6.0f,   //pos 2 
        +0.661f, +0.598f , +0.402f,

        +7.0f, +0.0f, -6.0f,   //pos 3 
        +0.661f, +0.598f , +0.402f,

        +7.0f, +0.0f, -10.0f,  //pos 4
        +0.661f, +0.598f , +0.402f,

        +12.5f, +6.0f, -12.5f,   //Pyramid 2 pos 5
        +0.761f, +0.698f , +0.502f,

        +10.0f, +0.0f,  -15.0f, //pos 6
        +0.661f, +0.598f , +0.402f,

        +10.0f, +0.0f, -10.0f,   //pos 7 
        +0.661f, +0.598f , +0.402f,

        +15.0f, +0.0f, -10.0f,   //pos 8
        +0.661f, +0.598f , +0.402f,

        +15.0f, +0.0f, -15.0f,  //pos 9
        +0.661f, +0.598f , +0.402f,



    };

    const GLushort pyramidsIndices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        5, 6, 7,
        5, 7, 8,
        5, 8, 9,
        5, 9, 6,
    };

    //Second VAO for the pyramids with Indexing
    glGenVertexArrays(1, &vaoID2); 
    glBindVertexArray(vaoID2);


    glGenBuffers(1, &vertexBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramids), pyramids, GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


    glGenBuffers(1, &indexBuffer2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidsIndices), pyramidsIndices, GL_STATIC_DRAW);

    const GLfloat boxMan[] = {

        +0.0f, +1.0f, +0.0f,  //pos 0
        +1.0f, +0.0f, +1.0f,

        +1.0f, +1.0f, +0.0f,  //pos 1
        +0.0f, +1.0f, +1.0f,

        +1.0f, +1.0f, -1.0f, //pos 2
         +1.0f, +1.0f, +0.0f,

        +0.0f, +1.0f, -1.0f, //pos 3
         +1.0f, +0.0f, +0.0f,

        +0.0f, +0.0f, +0.0f, //pos 4
         +0.647059f, +0.164706f, +0.164706f,

        +1.0f, +0.0f, +0.0f, //pos 5
         +0.647059f, +0.164706f, +0.164706f,

        +1.0f, +0.0f, -1.0f, //pos 6
         +0.0f, +0.0f, +1.0f,

        +0.0f, +0.0f, -1.0f, //pos 7
         +0.0f, +1.0f, +0.0f,

    };

    const GLushort boxManIndices[] = {
        0,1,2,
        0,2,3,
        0,4,5,
        0,5,1,
        1,5,6,
        1,6,2,
        0,4,7,
        0,7,3,
        3,7,6,
        3,6,2,
        7,4,5,
        7,5,1,
    };

    //Third VAO for the BoxMan with Indexing
    glGenVertexArrays(1, &vaoID3);   
    glBindVertexArray(vaoID3); 


    glGenBuffers(1, &vertexBuffer3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxMan), boxMan, GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    glGenBuffers(1, &indexBuffer3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxManIndices), boxManIndices, GL_STATIC_DRAW);

    const GLfloat clouds[] = {

        +3.0f, +11.0f, -39.0f,  //pos 0   Main body
        +1.0f, +1.0f,  +1.0f,

        +3.0f, +8.0f, -39.0f,    //pos 1
        +1.0f, +1.0f,  +1.0f,

        +11.5f,+8.0f, -39.0f,    //pos 2
        +1.0f, +1.0f,  +1.0f,

        +11.5f, +11.0f, -39.0f,  //pos 3
        +1.0f, +1.0f,  +1.0f,

        +2.4f, +10.6f, -39.0f,    //pos 4  left round end
        +1.0f, +1.0f,  +1.0f,

        +3.0f, +10.6f, -39.0f,   //pos 5
        +1.0f, +1.0f,  +1.0f,

        +2.0f, +9.8f, -39.0f,    //pos 6
        +1.0f, +1.0f,  +1.0f,

        +2.4f, +9.8f, -39.0f,    //pos 7
        +1.0f, +1.0f,  +1.0f,

        +3.0f, +9.8f, -39.0f,    //pos 8
        +1.0f, +1.0f,  +1.0f,

        +2.0f, +9.2f, -39.0f,    //pos 9
        +1.0f, +1.0f,  +1.0f,

        +2.4f, +9.2f, -39.0f,   //pos 10
        +1.0f, +1.0f,  +1.0f,

        +3.0f, +9.2f, -39.0f,   //pos 11
        +1.0f, +1.0f,  +1.0f,

        +2.4f, +8.4f, -39.0f,   //pos 12
        +1.0f, +1.0f,  +1.0f,

        +3.0f, +8.4f, -39.0f,   //pos 13
        +1.0f, +1.0f,  +1.0f,

        +11.5f, +10.6f, -39.0f,   //pos 14 right round end
        +1.0f, +1.0f,  +1.0f,

        +12.1f, +10.6f, -39.0f,   //pos 15
        +1.0f, +1.0f,  +1.0f,

        +11.5f, +9.8f, -39.0f,   //pos  16
        +1.0f, +1.0f,  +1.0f,

        +12.1f, +9.8f, -39.0f,    //pos 17
        +1.0f, +1.0f,  +1.0f,

        +12.5f, +9.8f, -39.0f,    //pos 18
        +1.0f, +1.0f,  +1.0f,

        +1.5f, +9.2f, -39.0f,    //pos 19
        +1.0f, +1.0f,  +1.0f,

        +12.1f, +9.2f, -39.0f,   //pos 20
        +1.0f, +1.0f,  +1.0f,

        +12.5f, +9.2f, -39.0f,   //pos 21
        +1.0f, +1.0f,  +1.0f,

        +11.5f, +8.4f, -39.0f,   //pos 22
        +1.0f, +1.0f,  +1.0f,

        +12.1f, +8.4f, -39.0f,   //pos 23
        +1.0f, +1.0f,  +1.0f,


        +4.0f, +11.0f, -39.0f,    //pos 24 top-left-round-end
        +1.0f, +1.0f,  +1.0f,

        +4.4f, +11.6f, -39.0f,      //pos 25
        +1.0f, +1.0f,  +1.0f,

        +4.4f, +11.0f, -39.0f,    //pos  26
        +1.0f, +1.0f,  +1.0f,

        +5.2f, 12.0f, -39.0f,    //pos 27
        +1.0f, +1.0f,  +1.0f,

        +5.2f, +11.6f, -39.0f,    //pos 28
        +1.0f, +1.0f,  +1.0f,

        +5.8f, +12.0f, -39.0f,    //pos 29
        +1.0f, +1.0f,  +1.0f,

        +5.8f, +11.6f, -39.0f,    //pos 30
        +1.0f, +1.0f,  +1.0f,

        +6.6f, +11.6f, -39.0f,    //pos 31
        +1.0f, +1.0f,  +1.0f,

        +6.6f, +11.0f, -39.0f,    //pos 32
        +1.0f, +1.0f,  +1.0f,

        +7.0f, +11.0f, -39.0f,    //pos 33
        +1.0f, +1.0f,  +1.0f,

            +7.5f, +11.0f, -39.0f,    //pos 34 top-left-round-end
            +1.0f, +1.0f, +1.0f,

            +7.9f, +11.6f, -39.0f,    //pos 35
            +1.0f, +1.0f, +1.0f,

            +7.9f, +11.0f, -39.0f,    //pos  36
            +1.0f, +1.0f, +1.0f,

            +8.7f, 12.0f, -39.0f,     //pos 37
            +1.0f, +1.0f, +1.0f,

            +8.7f, +11.6f, -39.0f,    //pos 38
            +1.0f, +1.0f, +1.0f,

            +9.3f, +12.0f, -39.0f,    //pos 39
            +1.0f, +1.0f, +1.0f,

            +9.3f, +11.6f, -39.0f,    //pos 40
            +1.0f, +1.0f, +1.0f,

            +10.1f, +11.6f, -39.0f,    //pos 41
            +1.0f, +1.0f, +1.0f,

            +10.1f, +11.0f, -39.0f,    //pos 42
            +1.0f, +1.0f, +1.0f,

            +10.5f, +11.0f, -39.0f,    //pos 43
            +1.0f, +1.0f, +1.0f,



    };


    const GLushort cloudsIndices[] = {
        0,1,2,
        0,2,3,
        0,4,5,
        4,6,7,
        4,7,8,
        4,8,5,
        6,9,11,
        6,11,8,
        9,10,12,
        10,12,13,
        10,13,11,
        12,1,13,
        3,14,15,
        14,16,17,
        14,17,15,
        15,17,18,
        16,19,21,
        16,21,18,
        19,22,23,
        19,23,20,
        20,21,23,
        22,2,23,
        24,25,26,
        25,26,32,
        25,32,31,
        25,28,27,
        27,28,30,
        27,30,29,
        29,30,31,
        31,32,33,
        34,35,36,
        35,36,42,
        35,42,41,
        35,38,37,
        37,38,40,
        37,40,39,
        39,40,41,
        41,42,43,

    };

    //Fourth VAO for 2D cloud with indexing 
    glGenVertexArrays(1, &vaoID4);  
    glBindVertexArray(vaoID4);   


    glGenBuffers(1, &vertexBuffer4);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(clouds), clouds, GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


    glGenBuffers(1, &indexBuffer4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer4);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 120 * sizeof(unsigned short), cloudsIndices, GL_STATIC_DRAW);

    const GLfloat sun[] = {

        -6.0f, +13.0f, -39.5f,  //pos 0
        +1.0f, +0.9f, +0.0f,

        -6.6f, +12.4f, -39.5f,  //pos 1
        +1.0f, +0.9f, +0.0f,

        -6.0f, +12.4f, -39.5f,  //pos 2
        +1.0f, +0.9f, +0.0f,

        -6.6f, +11.2f, -39.5f,  //pos 3
        +1.0f, +0.9f, +0.0f,

        -6.0f, +11.2f, -39.5f,  //pos 4
        +1.0f, +0.9f, +0.0f,

        -6.0f, +10.6f, -39.5f,  //pos 5
        +1.0f, +0.9f, +0.0f,

        -4.8f, +13.0f, -39.5f,  //pos 6
        +1.0f, +0.9f, +0.0f,

        -6.6f, +12.4f, -39.5f,  //pos 7
        +1.0f, +0.9f, +0.0f,

        -4.2f, +12.4f, -39.5f,  //pos 8
        +1.0f, +0.9f, +0.0f,

        -4.8f, +11.2f, -39.5f,  //pos 9
        +1.0f, +0.9f, +0.0f,

        -4.2f, +11.2f, -39.5f,  //pos 10
        +1.0f, +0.9f, +0.0f,

        -4.8f, +10.6f, -39.5f,  //pos 11
        +1.0f, +0.9f, +0.0f,

    };

    const GLushort sunIndices[] = {
        0,1,2,
        1,3,4,
        1,4,2,
        3,4,5,
        0,5,11,
        0,11,6,
        6,7,8,
        7,9,10,
        7,10,8,
        9,11,10,
    };

    //Fifth VAO for 2D sun with indexing 
    glGenVertexArrays(1, &vaoID5);  
    glBindVertexArray(vaoID5); 


    glGenBuffers(1, &vertexBuffer5);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sun), sun, GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


    glGenBuffers(1, &indexBuffer5);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer5);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 30 * sizeof(unsigned short), sunIndices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


}




void paintGL() {
	// always run
	// TODO:
	// render your objects and control the transformation here
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  //specify the background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

  

    glUseProgram(programID);
    

    //The view can rotate with Key H and J ,   
    //make use of sin and cos function to move the camera
    glm::mat4 viewMatrix(1.0f);                       
    viewMatrix =                  
    glm::lookAt(glm::vec3(5.0f * sin(view_rotation * glm::degrees(0.005f)), 2.0f, 5.0f * cos(view_rotation * glm::degrees(0.005f))), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    //Field of view can be adjust by scrolling
    glm::mat4 projectionMatrix =    
    glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);  

    

    GLint viewMatrixUniformLocation = glGetUniformLocation(programID, "viewMatrix");

    GLint projectionMatrixUniformLocation = glGetUniformLocation(programID, "projectionMatrix");

    GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
        
    glUseProgram(programID);

    {
        //Bind the first VAO
        glBindVertexArray(vaoID);  

        glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
    
        //Send the matrices of first VAO to shader
        glUniformMatrix4fv(projectionMatrixUniformLocation, 1,  
            GL_FALSE, &projectionMatrix[0][0]);


        glUniformMatrix4fv(viewMatrixUniformLocation, 1,
            GL_FALSE, &viewMatrix[0][0]);

        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
            GL_FALSE, &modelTransformMatrix[0][0]);

        //Draw the landscapes
        glDrawArrays(GL_TRIANGLES, 0, 36);   

    }

    {
        //bind second VAO
        glBindVertexArray(vaoID2); 

        // No special transformations for the pyramids
        glm::mat4 modelTransformMatrix = glm::mat4(1.0f);  
   

        glUniformMatrix4fv(projectionMatrixUniformLocation, 1,
            GL_FALSE, &projectionMatrix[0][0]);


        glUniformMatrix4fv(viewMatrixUniformLocation, 1,
            GL_FALSE, &viewMatrix[0][0]);

        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
            GL_FALSE, &modelTransformMatrix[0][0]);

        //Draw pyramids
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, 0);  
         
    }

    {   
        //Bind third VAO
        glBindVertexArray(vaoID3);  

        glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
        
        //Rotate BoxMan along y-axis
        modelTransformMatrix = 
        glm::rotate(modelTransformMatrix, glm::radians(rotation_press_num * rotation_delta), glm::vec3(0.0f, 1.0f, 0.0f));
        
        //Translation depends on how many times the keys "w" , "a" , "s" , "d" have been clicked
        modelTransformMatrix = 
        glm::translate(modelTransformMatrix, glm::vec3(x_press_num * x_delta, 0.0f, z_press_num * z_delta));

        glUniformMatrix4fv(projectionMatrixUniformLocation, 1,
            GL_FALSE, &projectionMatrix[0][0]);


        glUniformMatrix4fv(viewMatrixUniformLocation, 1,
            GL_FALSE, &viewMatrix[0][0]);

        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
            GL_FALSE, &modelTransformMatrix[0][0]);

        //Draw the BoxMan
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);  
    }

    {   
        //Bind fourth VAO
        glBindVertexArray(vaoID4);   

        glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
        
        //Scaling of the cloud
        modelTransformMatrix =   
        glm::scale(modelTransformMatrix, glm::vec3(cloudScale_press_num * 0.1, cloudScale_press_num * 0.1, 1.0f));
        
        //Translatino of the cloud depends on how many times the keys 
        //"up" , "left", "down", "right" have been clicked
        modelTransformMatrix = 
        glm::translate(modelTransformMatrix, glm::vec3(cloudX_press_num * x_delta, cloudY_press_num * y_delta, 0.0f));

        glUniformMatrix4fv(projectionMatrixUniformLocation, 1,
            GL_FALSE, &projectionMatrix[0][0]);


        glUniformMatrix4fv(viewMatrixUniformLocation, 1,
            GL_FALSE, &viewMatrix[0][0]);

        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
            GL_FALSE, &modelTransformMatrix[0][0]);

        //Draw the cloud
        glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_SHORT, 0); 

    }

    {   
        //Bind the fifth VAO
        glBindVertexArray(vaoID5);

        //No transformations for the sun
        glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
   

        glUniformMatrix4fv(projectionMatrixUniformLocation, 1,
            GL_FALSE, &projectionMatrix[0][0]);


        glUniformMatrix4fv(viewMatrixUniformLocation, 1,
            GL_FALSE, &viewMatrix[0][0]);

        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
            GL_FALSE, &modelTransformMatrix[0][0]);

        //Draw the sun
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, 0);

    }
    

    //Unbind VAO
    glBindVertexArray(0);
    glUseProgram(0);


}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        x_press_num -= 1;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        x_press_num += 1;
    }


    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        z_press_num += 1;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        z_press_num -= 1;
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {

        rotation_press_num += 1;
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        rotation_press_num -= 1;
    }


    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        scale_press_num += 1;
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        scale_press_num -= 1;
    }

    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        view_rotation += 1;
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        view_rotation -= 1;
    }


    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        cloudX_press_num -= 1;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        cloudX_press_num += 1;
    }


    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        cloudY_press_num += 1;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        cloudY_press_num -= 1;
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        cloudScale_press_num += 1;
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        cloudScale_press_num -= 1;
    }



}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    //Scroll to adjust the FoV between 1.0f and 90.0f
    if (FoV >= 1.0f && FoV <= 90.0f) 
        FoV -= yoffset;
    if (FoV <= 1.0f)
        FoV = 1.0f;
    if (FoV >= 90.0f)
        FoV = 90.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void initializedGL(void) {
	// run only once
	// TODO:
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char* argv[]) {
	GLFWwindow* window;
 

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Assignment 1", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

	/* Initialize the glew */
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	get_OpenGL_info();
	initializedGL();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
         
       
		paintGL();
        

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
