//////////////////////////////////////////////////////////////////////////////
//
//  Tetris3D.cpp
//
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm/glm.hpp> //includes GLM
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include<vector>
#include <string>  
#include<windows.h>

#include "Project.h"
#include "Shapes.h"
#include "Tetris.h"

Tetris piece;

#define BUFFER_OFFSET(a) ((void*)(a))

void gameTextures()
{
	//LOADS ALL IMAGES/TEXTURES FROM TEXTURE FOLDER
	loadTexture(textureBackground, "media/textures/tetris3d.png");
	loadTexture(textureMenu, "media/textures/background.png");
	loadTexture(textureMenuStart, "media/textures/start.png");
	loadTexture(texture1, "media/textures/greenFace.png");
	loadTexture(userscore, "media/textures/score.png");
	loadTexture(zero, "media/textures/0.png");
	loadTexture(one, "media/textures/1.png");
	loadTexture(two, "media/textures/2.png");
	loadTexture(three, "media/textures/3.png");
	loadTexture(four, "media/textures/4.png");
	loadTexture(five, "media/textures/5.png");
	loadTexture(six, "media/textures/6.png");
	loadTexture(seven, "media/textures/7.png");
	loadTexture(eight, "media/textures/8.png");
	loadTexture(nine, "media/textures/9.png");
	loadTexture(textureMenuExit, "media/textures/exit.png");
	loadTexture(playagain, "media/textures/playagain.png");
}

//////////////////////////////////////////////////////////////////////////////
//
//  LOADS ALL VAOS
//
//////////////////////////////////////////////////////////////////////////////

void initScore(void)
{
	//GENERATE VAO AND BIND
	glBindVertexArray(VAOs[1]);

	//GENERATE SHADER FOR OBJECT
	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);

	//COPYS INTO NEW OBJECTS
	GLfloat vertices[sizeof((Scorevertices))][3];
	memcpy(vertices, Scorevertices, sizeof(Scorevertices));

	GLuint indices[sizeof(Scoreindices)][3];
	memcpy(indices, Scoreindices, sizeof(Scoreindices));

	GLfloat colours[sizeof(Scorecolours)][4];
	memcpy(colours, Scorecolours, sizeof(Scorecolours));

	GLfloat texture_coords[sizeof(Scoretexture_coords)];
	memcpy(texture_coords, Scoretexture_coords, sizeof(Scoretexture_coords));

	GLfloat normals[sizeof(Scorenormals)];
	memcpy(normals, Scorenormals, sizeof(Scorenormals));

	//GENRATE BUFFER
	glGenBuffers(NumBuffers, Buffers);

	//BINDS ARRAYS TO BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Shapes]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//INDICES BINDING
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//COLOUR BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);
	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//NORMAL BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normals]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(normals), normals, 0);
	glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//TEXTURE BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Tex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//CREATING MODEL MATRIX
	glm::mat4 model = glm::mat4(1.0f);

	//CREATING VIEW MATRIX
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, -4.0f, -20.0f));

	//CREATING PROJECTION MATRIX
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
	glm::mat4 mv = model * view;

	int mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	int pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//AMBIENT LIGHT
	glm::vec4 ambient = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);//LEVEL OF AMBIENT LIGHT
	int aLoc = glGetUniformLocation(program, "ambient");	//ADDING UNIFORM TO SHADER
	glUniform4fv(aLoc, 1, glm::value_ptr(ambient));

	//LIGHT OBJECT
	glm::vec3 lightPos = glm::vec3(100.0f, 25.0f, 100.0f);//POSITION OF LIGHT SOURCE
	GLuint dLightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3fv(dLightPosLoc, 1, glm::value_ptr(lightPos));

	//DIFFUSE LIGHT
	glm::vec3 diffuseLight = glm::vec3(0.5f, 0.2f, 0.7f);
	GLuint dLightLoc = glGetUniformLocation(program, "dLight");
	glUniform3fv(dLightLoc, 1, glm::value_ptr(diffuseLight));

	//SPECULAR LIGHT 
	glm::vec3 specularLight = glm::vec3(0.7f);
	GLfloat shininess = 256; //128 is max value
	GLuint sLightLoc = glGetUniformLocation(program, "sLight");
	GLuint sShineLoc = glGetUniformLocation(program, "sShine");
	glUniform3fv(sLightLoc, 1, glm::value_ptr(specularLight));
	glUniform1fv(sShineLoc, 1, &shininess);

	//ENABLE VERTEX ATTRIBUTES
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition);
	glEnableVertexAttribArray(tPosition);
	glEnableVertexAttribArray(nPosition);

	//UNBINDS
	glBindVertexArray(0);
}

void initBackgroundMenuStart(void)
{
	//GENERATE VAO AND BIND
	glBindVertexArray(VAOs[4]);

	//GENERATE SHADER FOR OBJECT
	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);


	//COPYS INTO NEW OBJECTS
	GLfloat vertices[sizeof((Bvertices))][3];
	memcpy(vertices, Bvertices, sizeof(Bvertices));

	GLuint indices[sizeof(Bindices)][3];
	memcpy(indices, Bindices, sizeof(Bindices));

	GLfloat colours[sizeof(Bcolours)][4];
	memcpy(colours, Bcolours, sizeof(Bcolours));

	GLfloat texture_coords[sizeof(Btexture_coords)];
	memcpy(texture_coords, Btexture_coords, sizeof(Btexture_coords));

	GLfloat normals[sizeof(Bnormals)];
	memcpy(normals, Bnormals, sizeof(Bnormals));


	//GENRATE BUFFER
	glGenBuffers(NumBuffers, Buffers);

	//BINDS ARRAYS TO BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Shapes]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//INDICES BINDING
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//COLOUR BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);
	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//NORMAL BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normals]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(normals), normals, 0);
	glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//TEXTURE BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Tex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//CREATING MODEL MATRIX
	glm::mat4 model = glm::mat4(1.0f);

	//CREATING VIEW MATRIX
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, -4.0f, -20.0f));

	//CREATING PROJECTION MATRIX
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	glm::mat4 mv = model * view;


	int mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	int pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//AMBIENT LIGHT
	glm::vec4 ambient = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);//LEVEL OF AMBIENT LIGHT
	int aLoc = glGetUniformLocation(program, "ambient");	//ADDING UNIFORM TO SHADER
	glUniform4fv(aLoc, 1, glm::value_ptr(ambient));

	//LIGHT OBJECT
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);//POSITION OF LIGHT SOURCE
	GLuint dLightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3fv(dLightPosLoc, 1, glm::value_ptr(lightPos));

	//DIFFUSE LIGHT
	glm::vec3 diffuseLight = glm::vec3(0.5f, 0.2f, 0.7f);
	GLuint dLightLoc = glGetUniformLocation(program, "dLight");
	glUniform3fv(dLightLoc, 1, glm::value_ptr(diffuseLight));

	//SPECULAR LIGHT 
	glm::vec3 specularLight = glm::vec3(0.7f);
	GLfloat shininess = 5; //128 is max value
	GLuint sLightLoc = glGetUniformLocation(program, "sLight");
	GLuint sShineLoc = glGetUniformLocation(program, "sShine");
	glUniform3fv(sLightLoc, 1, glm::value_ptr(specularLight));
	glUniform1fv(sShineLoc, 1, &shininess);



	//ENABLE VERTEX ATTRIBUTES
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition);
	glEnableVertexAttribArray(tPosition);
	glEnableVertexAttribArray(nPosition);

	//UNBINDS
	glBindVertexArray(0);
}

void initBackground(void)
{
	//GENERATE VAO AND BIND
	glBindVertexArray(VAOs[0]);

	//GENERATE SHADER FOR OBJECT
	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);

	//COPYS INTO NEW OBJECTS
	GLfloat vertices[sizeof((backgroundVertices))][3];
	memcpy(vertices, backgroundVertices, sizeof(backgroundVertices));

	GLuint indices[sizeof(backgroundIndices)][3];
	memcpy(indices, backgroundIndices, sizeof(backgroundIndices));

	GLfloat colours[sizeof(backgroundColours)][4];
	memcpy(colours, backgroundColours, sizeof(backgroundColours));

	GLfloat texture_coords[sizeof(backgroundTexture_coords)];
	memcpy(texture_coords, backgroundTexture_coords, sizeof(backgroundTexture_coords));

	GLfloat normals[sizeof(backgroundNormals)];
	memcpy(normals, backgroundNormals, sizeof(backgroundNormals));

	//GENRATE BUFFER
	glGenBuffers(NumBuffers, Buffers);

	//BINDS ARRAYS TO BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Shapes]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//INDICES BINDING
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//COLOUR BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);
	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//NORMAL BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normals]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(normals), normals, 0);
	glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//TEXTURE BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Tex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//CREATING MODEL MATRIX
	glm::mat4 model = glm::mat4(1.0f);

	//CREATING VIEW MATRIX
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, -4.0f, -20.0f));

	//CREATING PROJECTION MATRIX
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
	glm::mat4 mv = model * view;

	int mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	int pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//AMBIENT LIGHT
	glm::vec4 ambient = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);//LEVEL OF AMBIENT LIGHT
	int aLoc = glGetUniformLocation(program, "ambient");	//ADDING UNIFORM TO SHADER
	glUniform4fv(aLoc, 1, glm::value_ptr(ambient));

	//LIGHT OBJECT
	glm::vec3 lightPos = glm::vec3(100.0f, 25.0f, 100.0f);//POSITION OF LIGHT SOURCE
	GLuint dLightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3fv(dLightPosLoc, 1, glm::value_ptr(lightPos));

	//DIFFUSE LIGHT
	glm::vec3 diffuseLight = glm::vec3(0.5f, 0.2f, 0.7f);
	GLuint dLightLoc = glGetUniformLocation(program, "dLight");
	glUniform3fv(dLightLoc, 1, glm::value_ptr(diffuseLight));

	//SPECULAR LIGHT 
	glm::vec3 specularLight = glm::vec3(0.7f);
	GLfloat shininess = 256; //128 is max value
	GLuint sLightLoc = glGetUniformLocation(program, "sLight");
	GLuint sShineLoc = glGetUniformLocation(program, "sShine");
	glUniform3fv(sLightLoc, 1, glm::value_ptr(specularLight));
	glUniform1fv(sShineLoc, 1, &shininess);

	//ENABLE VERTEX ATTRIBUTES
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition);
	glEnableVertexAttribArray(tPosition);
	glEnableVertexAttribArray(nPosition);

	//UNBINDS
	glBindVertexArray(0);
}

void init(void)
{

	//GENERATE VAO AND BIND
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[2]);

	//GENERATE SHADER FOR OBJECT
	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);


	//COPYS INTO NEW OBJECTS
	GLfloat vertices[sizeof((Bvertices))][3];
	memcpy(vertices, Bvertices, sizeof(Bvertices));

	GLuint indices[sizeof(Bindices)][3];
	memcpy(indices, Bindices, sizeof(Bindices));

	GLfloat colours[sizeof(Bcolours)][4];
	memcpy(colours, Bcolours, sizeof(Bcolours));

	GLfloat texture_coords[sizeof(Btexture_coords)];
	memcpy(texture_coords, Btexture_coords, sizeof(Btexture_coords));

	GLfloat normals[sizeof(Bnormals)];
	memcpy(normals, Bnormals, sizeof(Bnormals));


	//GENRATE BUFFER
	glGenBuffers(NumBuffers, Buffers);

	//BINDS ARRAYS TO BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Shapes]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//INDICES BINDING
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//COLOUR BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);
	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//NORMAL BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normals]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(normals), normals, 0);
	glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//TEXTURE BINDING
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Tex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//CREATING MODEL MATRIX
	glm::mat4 model = glm::mat4(1.0f);

	//CREATING VIEW MATRIX
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, -4.0f, -20.0f));

	//CREATING PROJECTION MATRIX
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	glm::mat4 mv = model * view;


	int mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	int pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//AMBIENT LIGHT
	glm::vec4 ambient = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);//LEVEL OF AMBIENT LIGHT
	int aLoc = glGetUniformLocation(program, "ambient");	//ADDING UNIFORM TO SHADER
	glUniform4fv(aLoc, 1, glm::value_ptr(ambient));

	//LIGHT OBJECT
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);//POSITION OF LIGHT SOURCE
	GLuint dLightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3fv(dLightPosLoc, 1, glm::value_ptr(lightPos));

	//DIFFUSE LIGHT
	glm::vec3 diffuseLight = glm::vec3(0.5f, 0.2f, 0.7f);
	GLuint dLightLoc = glGetUniformLocation(program, "dLight");
	glUniform3fv(dLightLoc, 1, glm::value_ptr(diffuseLight));

	//SPECULAR LIGHT 
	glm::vec3 specularLight = glm::vec3(0.7f);
	GLfloat shininess = 5; //128 is max value
	GLuint sLightLoc = glGetUniformLocation(program, "sLight");
	GLuint sShineLoc = glGetUniformLocation(program, "sShine");
	glUniform3fv(sLightLoc, 1, glm::value_ptr(specularLight));
	glUniform1fv(sShineLoc, 1, &shininess);



	//ENABLE VERTEX ATTRIBUTES
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition);
	glEnableVertexAttribArray(tPosition);
	glEnableVertexAttribArray(nPosition);

	//UNBINDS
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////
//
//  LOADS TEXTURE
//
//////////////////////////////////////////////////////////////////////////////

void loadTexture(GLuint& texture, std::string texturepath)
{
	// load and create a texture 
// -------------------------

// texture 1
// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(texturepath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}


//////////////////////////////////////////////////////////////////////////////
//
//  TETRIS PIECES CALULATIONS
//
//////////////////////////////////////////////////////////////////////////////
void reset() {

	//REMOVES ALL BLOCKS (1'S) FROM ARRAY
	for (size_t i = 0; i < 15; i++)
	{
		for (size_t j = 0; j < 15; j++)
		{
			tetrisBlocks[i][j] = 0;
		}
	}

	//RESETS CURRENT TETRIS PIECE AND SCORE 
	piece.setPosition(glm::vec3(0.f, 6.f, -19.f));
	position = piece.getPosition();
	Score = 0;
}

void inputUpdate(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation)
{
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && (Game == Game_Menu))
	{
		//GET CURSOR POSITION
		Sleep(100);
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//PLAY OPTION
		if ((xpos >= 310) && (xpos <= 490) && (ypos >= 120) && (ypos <= 300))
		{
			Game = Game_Active;
		};

		//EXIT OPTION
		if ((xpos >= 310) && (xpos <= 490) && (ypos >= 330) && (ypos <= 510))
		{
			glfwSetWindowShouldClose(window, true);//CLOSES WINDOW
		};
	}

	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && (Game == Game_End))
	{
		//GET CURSOR POSITION
		Sleep(100);
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//PLAY AGAIN OPTION
		if ((xpos >= 310) && (xpos <= 490) && (ypos >= 120) && (ypos <= 300))
		{
			reset();
			Game = Game_Active;
		};

		//EXIT OPTION
		if ((xpos >= 310) && (xpos <= 490) && (ypos >= 330) && (ypos <= 510))
		{
			glfwSetWindowShouldClose(window, true);//CLOSES WINDOW
		};
	}

	//CHANGES ROTATION AND POSITION VALUES
	if (Game == Game_Active)
	{
		if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (position.x > -7))
		{
			position.x -= 1.f;
			Sleep(150);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position.y -= 1.f;

			Sleep(100);
		}

		if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && (position.x < 7))
		{
			position.x += 1.f;
			Sleep(100);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			if (rotation.z == 270)
			{
				rotation.z = 0;
			}
			else
			{
				rotation.z += 90.f;
			}
			Sleep(150);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			if (rotation.z == -270)
			{
				rotation.z = 0;
			}
			else
			{
				rotation.z -= 90.f;
			}
			Sleep(150);
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);//CLOSES WINDOW
	}
}

void removeRows() 
{
	//CREATES NEW ARRAY
	int newTetris[15][15] = {};
	int row = 0;

	for (size_t i = 0; i < 15; i++)
	{
		for (size_t j = 0; j < 15; j++)
		{
			if (allRows[i] == 0)//ONLY INSERTS IF ROW ISNT FULL (1'S)
			{
				newTetris[row][j] = tetrisBlocks[i][j];
			}
		}

		if (allRows[i] == 0)
		{
			row++;
		}
	}

	//RESET LINE ARRAY AND COPYS NEW TETRIS ARRAY TO OLD TETRIS ARRAY
	memset(allRows, 0, sizeof(allRows));
	std::copy(&newTetris[0][0], &newTetris[0][0] + 15 * 15, &tetrisBlocks[0][0]);
}

void checkRow()
{
	int rows = 0;
	bool line = true;

	for (size_t i = 0; i < 15; i++)
	{
		for (size_t j = 0; j < 15; j++)
		{
			if (tetrisBlocks[i][j] == 0)
			{
				line = false;
			}
		}
		if (line == true) {
			//IF LINE IS FULL THEN ROWS & MUITPLYER INCREASES 
			rows++;
			allRows[i] = 1;
			multiplier += 0.01f;
		}
		line = true;
	}

	removeRows();//REMOVES ROWS FROM ARRAY

	//CHANGES SCORE
	switch (rows) {
	case 0:
		Score += 0;
		break;
	case 1:
		Score += 20;
		break;
	case 2:
		Score += 50;
		break;
	case 3:
		Score += 100;
		break;
	case 4:
		Score += 250;
		break;
	}
}

void oldPiece()
{
	if (piece.getType() == "L")
	{
		if (piece.getOldRotation().z == 0)
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y + 1][x] = 1;
			tetrisBlocks[y - 1][x] = 1;
			tetrisBlocks[y - 1][x - 1] = 1;
		}

		if ((piece.getOldRotation().z == -90) || (piece.getOldRotation().z == 270))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y][x - 1] = 1;
			tetrisBlocks[y + 1][x - 1] = 1;
			tetrisBlocks[y][x + 1] = 1;
		}

		if ((piece.getOldRotation().z == 90) || (piece.getOldRotation().z == -270))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y][x - 1] = 1;
			tetrisBlocks[y][x + 1] = 1;
			tetrisBlocks[y - 1][x + 1] = 1;
		}

		if ((piece.getOldRotation().z == -180) || (piece.getOldRotation().z == 180))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y + 1][x] = 1;
			tetrisBlocks[y + 1][x + 1] = 1;
			tetrisBlocks[y - 1][x] = 1;
		}
	}

	if (piece.getType() == "S")
	{
		int x = piece.getOldPosition().x + 7;
		int y = piece.getOldPosition().y + 7;
		tetrisBlocks[y][x] = 1;
		tetrisBlocks[y + 1][x] = 1;
		tetrisBlocks[y + 1][x + 1] = 1;
		tetrisBlocks[y][x + 1] = 1;
	}

	if (piece.getType() == "Z")
	{
		if (piece.getOldRotation().z == 0)
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y][x + 1] = 1;
			tetrisBlocks[y - 1][x] = 1;
			tetrisBlocks[y - 1][x - 1] = 1;
		}

		if ((piece.getOldRotation().z == -90) || (piece.getOldRotation().z == 270))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y][x - 1] = 1;
			tetrisBlocks[y + 1][x - 1] = 1;
			tetrisBlocks[y - 1][x] = 1;
		}

		if ((piece.getOldRotation().z == 90) || (piece.getOldRotation().z == -270))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y + 1][x] = 1;
			tetrisBlocks[y][x + 1] = 1;
			tetrisBlocks[y - 1][x + 1] = 1;
		}

		if ((piece.getOldRotation().z == -180) || (piece.getOldRotation().z == 180))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y + 1][x] = 1;
			tetrisBlocks[y + 1][x + 1] = 1;
			tetrisBlocks[y][x - 1] = 1;
		}
	}

	if (piece.getType() == "I")
	{
		if (piece.getOldRotation().z == 0)
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y + 1][x] = 1;
			tetrisBlocks[y - 1][x] = 1;
			tetrisBlocks[y - 2][x] = 1;
		}

		if ((piece.getOldRotation().z == -90) || (piece.getOldRotation().z == 270))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y][x + 1] = 1;
			tetrisBlocks[y][x - 1] = 1;
			tetrisBlocks[y][x - 2] = 1;
		}

		if ((piece.getOldRotation().z == 90) || (piece.getOldRotation().z == -270))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y][x + 1] = 1;
			tetrisBlocks[y][x + 2] = 1;
			tetrisBlocks[y][x - 1] = 1;
		}

		if ((piece.getOldRotation().z == -180) || (piece.getOldRotation().z == 180))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y + 1][x] = 1;
			tetrisBlocks[y + 2][x] = 1;
			tetrisBlocks[y - 1][x] = 1;
		}
	}

	if (piece.getType() == "T")
	{
		if (piece.getOldRotation().z == 0)
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y - 1][x] = 1;
			tetrisBlocks[y][x - 1] = 1;
			tetrisBlocks[y][x + 1] = 1;
		}

		if ((piece.getOldRotation().z == -90) || (piece.getOldRotation().z == 270))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y - 1][x] = 1;
			tetrisBlocks[y][x - 1] = 1;
			tetrisBlocks[y + 1][x] = 1;
		}

		if ((piece.getOldRotation().z == 90) || (piece.getOldRotation().z == -270))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y - 1][x] = 1;
			tetrisBlocks[y][x + 1] = 1;
			tetrisBlocks[y + 1][x] = 1;
		}

		if ((piece.getOldRotation().z == -180) || (piece.getOldRotation().z == 180))
		{
			int x = piece.getOldPosition().x + 7;
			int y = piece.getOldPosition().y + 7;
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y + 1][x] = 1;
			tetrisBlocks[y][x - 1] = 1;
			tetrisBlocks[y][x + 1] = 1;
		}
	}

}

void resetPiece()
{
	//RESETS POSITION
	piece.setPosition(glm::vec3(0.f, 6.f, -19.f));
	position = piece.getPosition();
	rotation = glm::vec3(0.f, 0.f, 0.f);

	//CHECK IF PEICE WAS AT TOP OF PLAY AREA
	if (piece.getOldPosition().y >= 5) { Game = Game_End; }

	//SETS OLD PIECE AND CHECK FOR ROWS
	oldPiece();
	checkRow();

	//RESETS OLD POSITION
	piece.setOldPosition(piece.getPosition());
	piece.setOldRotation(piece.getRotation());

	//CHANGES BLOCK TYPE
	piece.setType();
}

void currentPiece(glm::vec3& position, glm::vec3& rotation) {

	piece.setPosition(position);
	piece.setRotation(rotation);

	//CHECKS FOR PIECE TYPE
	if (piece.getType() == "L")
	{
		//SETS X AND Y POSITIONS 
		int x = piece.getOldPosition().x + 7;
		int y = piece.getOldPosition().y + 7;

		//CHECKS FOR ROATATION 
		if (piece.getOldRotation().z == 0)
		{
			//REMOVES OLD POSITION FROM ARRAY
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y + 1][x] = 0;
			tetrisBlocks[y - 1][x] = 0;
			tetrisBlocks[y - 1][x - 1] = 0;
		}

		if ((piece.getOldRotation().z == -90) || (piece.getOldRotation().z == 270))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y][x - 1] = 0;
			tetrisBlocks[y + 1][x - 1] = 0;
			tetrisBlocks[y][x + 1] = 0;
		}

		if ((piece.getOldRotation().z == 90) || (piece.getOldRotation().z == -270))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y][x - 1] = 0;
			tetrisBlocks[y][x + 1] = 0;
			tetrisBlocks[y - 1][x + 1] = 0;
		}

		if ((piece.getOldRotation().z == -180) || (piece.getOldRotation().z == 180))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y + 1][x] = 0;
			tetrisBlocks[y + 1][x + 1] = 0;
			tetrisBlocks[y - 1][x] = 0;
		}

		x = piece.getPosition().x + 7;
		y = piece.getPosition().y + 7;

		if (piece.getRotation().z == 0)
		{
			//CHECKS IF NEW POSITION IS EMPTY OR NOT 
			if ((y < 1) || (tetrisBlocks[y - 1][x] == 1) || (tetrisBlocks[y - 1][x - 1] == 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x] == 1))
			{
				//RESETS CURRENT PIECE 
				resetPiece();
			}
			else {
				//PUTS CURRENT PIECE INTO TETRIS ARRAY
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y + 1][x] = 1;
				tetrisBlocks[y - 1][x] = 1;
				tetrisBlocks[y - 1][x - 1] = 1;
			}
		}

		if ((piece.getRotation().z == -90) || (piece.getRotation().z == 270))
		{
			if ((y < 0) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y][x - 1] == 1) || (tetrisBlocks[y + 1][x - 1] == 1) || (tetrisBlocks[y][x + 1] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y][x - 1] = 1;
				tetrisBlocks[y + 1][x - 1] = 1;
				tetrisBlocks[y][x + 1] = 1;
			}		
		}

		if ((piece.getRotation().z == 90) || (piece.getRotation().z == -270))
		{
			if ((y < 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y][x - 1] == 1) || (tetrisBlocks[y][x + 1] == 1) || (tetrisBlocks[y - 1][x + 1] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y][x - 1] = 1;
				tetrisBlocks[y][x + 1] = 1;
				tetrisBlocks[y - 1][x + 1] = 1;
			}
		}

		if ((piece.getRotation().z == -180) || (piece.getRotation().z == 180))
		{
			if ((y < 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x] == 1) || (tetrisBlocks[y + 1][x + 1] == 1) || (tetrisBlocks[y - 1][x] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y + 1][x] = 1;
				tetrisBlocks[y + 1][x + 1] = 1;
				tetrisBlocks[y - 1][x] = 1;
			}
		}
	}

	if (piece.getType() == "Z")
	{
		int x = piece.getOldPosition().x + 7;
		int y = piece.getOldPosition().y + 7;

		if (piece.getOldRotation().z == 0)
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y][x + 1] = 0;
			tetrisBlocks[y - 1][x] = 0;
			tetrisBlocks[y - 1][x - 1] = 0;
		}

		if ((piece.getOldRotation().z == -90) || (piece.getOldRotation().z == 270))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y][x - 1] = 0;
			tetrisBlocks[y + 1][x - 1] = 0;
			tetrisBlocks[y - 1][x] = 0;
		}

		if ((piece.getOldRotation().z == 90) || (piece.getOldRotation().z == -270))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y + 1][x] = 0;
			tetrisBlocks[y][x + 1] = 0;
			tetrisBlocks[y - 1][x + 1] = 0;
		}

		if ((piece.getOldRotation().z == -180) || (piece.getOldRotation().z == 180))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y + 1][x] = 0;
			tetrisBlocks[y + 1][x + 1] = 0;
			tetrisBlocks[y][x - 1] = 0;
		}

		x = piece.getPosition().x + 7;
		y = piece.getPosition().y + 7;

		if (piece.getRotation().z == 0)
		{
			if ((y < 1) || (tetrisBlocks[y - 1][x] == 1) || (tetrisBlocks[y - 1][x - 1] == 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y][x + 1] == 1))
			{
				resetPiece();
			}
			else {
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y][x + 1] = 1;
				tetrisBlocks[y - 1][x] = 1;
				tetrisBlocks[y - 1][x - 1] = 1;
			}
		}

		if ((piece.getRotation().z == -90) || (piece.getRotation().z == 270))
		{
			if ((y < 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y][x - 1] == 1) || (tetrisBlocks[y + 1][x - 1] == 1) || (tetrisBlocks[y - 1][x] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y][x - 1] = 1;
				tetrisBlocks[y + 1][x - 1] = 1;
				tetrisBlocks[y - 1][x] = 1;
			}
		}

		if ((piece.getRotation().z == 90) || (piece.getRotation().z == -270))
		{
			if ((y < 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x] == 1) || (tetrisBlocks[y][x + 1] == 1) || (tetrisBlocks[y - 1][x + 1] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y + 1][x] = 1;
				tetrisBlocks[y][x + 1] = 1;
				tetrisBlocks[y - 1][x + 1] = 1;
			}
		}

		if ((piece.getRotation().z == -180) || (piece.getRotation().z == 180))
		{
			if ((y < 0) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x] == 1) || (tetrisBlocks[y + 1][x + 1] == 1) || (tetrisBlocks[y][x - 1] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y + 1][x] = 1;
				tetrisBlocks[y + 1][x + 1] = 1;
				tetrisBlocks[y][x - 1] = 1;
			}
		}
	}

	if (piece.getType() == "I")
	{
		int x = piece.getOldPosition().x + 7;
		int y = piece.getOldPosition().y + 7;

		if (piece.getOldRotation().z == 0)
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y + 1][x] = 0;
			tetrisBlocks[y - 1][x] = 0;
			tetrisBlocks[y - 2][x] = 0;
		}

		if ((piece.getOldRotation().z == -90) || (piece.getOldRotation().z == 270))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y][x + 1] = 0;
			tetrisBlocks[y][x - 1] = 0;
			tetrisBlocks[y][x - 2] = 0;
		}

		if ((piece.getOldRotation().z == 90) || (piece.getOldRotation().z == -270))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y][x + 1] = 0;
			tetrisBlocks[y][x + 2] = 0;
			tetrisBlocks[y][x - 1] = 0;
		}

		if ((piece.getOldRotation().z == -180) || (piece.getOldRotation().z == 180))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y + 1][x] = 0;
			tetrisBlocks[y + 2][x] = 0;
			tetrisBlocks[y - 1][x] = 0;
		}

		x = piece.getPosition().x + 7;
		y = piece.getPosition().y + 7;

		if (piece.getRotation().z == 0)
		{
			if ((y < 2) || (tetrisBlocks[y - 1][x] == 1) || (tetrisBlocks[y - 2][x] == 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x] == 1))
			{
				resetPiece();
			}
			else {
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y + 1][x] = 1;
				tetrisBlocks[y - 1][x] = 1;
				tetrisBlocks[y - 2][x] = 1;
			}
		}

		if ((piece.getRotation().z == -90) || (piece.getRotation().z == 270))
		{
			if ((y < 0) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y][x - 1] == 1) || (tetrisBlocks[y][x - 2] == 1) || (tetrisBlocks[y][x + 1] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y][x + 1] = 1;
				tetrisBlocks[y][x - 1] = 1;
				tetrisBlocks[y][x - 2] = 1;
			}
		}

		if ((piece.getRotation().z == 90) || (piece.getRotation().z == -270))
		{
			if ((y < 0) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y][x + 1] == 1) || (tetrisBlocks[y][x + 2] == 1) || (tetrisBlocks[y][x - 1] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y][x + 1] = 1;
				tetrisBlocks[y][x + 2] = 1;
				tetrisBlocks[y][x - 1] = 1;
			}
		}

		if ((piece.getRotation().z == -180) || (piece.getRotation().z == 180))
		{
			if ((y < 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x] == 1) || (tetrisBlocks[y + 2][x] == 1) || (tetrisBlocks[y - 1][x] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y + 1][x] = 1;
				tetrisBlocks[y + 2][x] = 1;
				tetrisBlocks[y - 1][x] = 1;
			}
		}
	}

	if (piece.getType() == "T")
	{
		int x = piece.getOldPosition().x + 7;
		int y = piece.getOldPosition().y + 7;

		if (piece.getOldRotation().z == 0)
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y - 1][x] = 0;
			tetrisBlocks[y][x - 1] = 0;
			tetrisBlocks[y][x + 1] = 0;
		}

		if ((piece.getOldRotation().z == -90) || (piece.getOldRotation().z == 270))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y - 1][x] = 0;
			tetrisBlocks[y][x - 1] = 0;
			tetrisBlocks[y + 1][x] = 0;
		}

		if ((piece.getOldRotation().z == 90) || (piece.getOldRotation().z == -270))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y - 1][x] = 0;
			tetrisBlocks[y][x + 1] = 0;
			tetrisBlocks[y + 1][x] = 0;
		}

		if ((piece.getOldRotation().z == -180) || (piece.getOldRotation().z == 180))
		{
			tetrisBlocks[y][x] = 0;
			tetrisBlocks[y + 1][x] = 0;
			tetrisBlocks[y][x - 1] = 0;
			tetrisBlocks[y][x + 1] = 0;
		}

		x = piece.getPosition().x + 7;
		y = piece.getPosition().y + 7;

		if (piece.getRotation().z == 0)
		{
			if ((y < 1) || (tetrisBlocks[y - 1][x] == 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y][x - 1] == 1) || (tetrisBlocks[y][x + 1] == 1))
			{
				piece.setPosition(glm::vec3(0.f, 6.f, -19.f));
				position = piece.getPosition();
				rotation = glm::vec3(0.f, 0.f, 0.f);

				if (piece.getOldPosition().y >= 5) { Game = Game_End; }
				oldPiece();
				checkRow();
				
				piece.setOldPosition(piece.getPosition());
				piece.setOldRotation(piece.getRotation());

				piece.setType();
			}
			else {
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y - 1][x] = 1;
				tetrisBlocks[y][x - 1] = 1;
				tetrisBlocks[y][x + 1] = 1;
			}
		}

		if ((piece.getRotation().z == -90) || (piece.getRotation().z == 270))
		{
			if ((y < 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y][x - 1] == 1) || (tetrisBlocks[y - 1][x] == 1) || (tetrisBlocks[y + 1][x] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y - 1][x] = 1;
				tetrisBlocks[y][x - 1] = 1;
				tetrisBlocks[y + 1][x] = 1;
			}
		}

		if ((piece.getRotation().z == 90) || (piece.getRotation().z == -270))
		{
			if ((y < 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y - 1][x] == 1) || (tetrisBlocks[y + 1][x] == 1) || (tetrisBlocks[y][x + 1] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y - 1][x] = 1;
				tetrisBlocks[y][x + 1] = 1;
				tetrisBlocks[y + 1][x] = 1;
			}
		}

		if ((piece.getRotation().z == -180) || (piece.getRotation().z == 180))
		{
			if ((y < 0) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x] == 1) || (tetrisBlocks[y][x - 1] == 1) || (tetrisBlocks[y][x + 1] == 1))
			{
				resetPiece();
			}
			else
			{
				tetrisBlocks[y][x] = 1;
				tetrisBlocks[y + 1][x] = 1;
				tetrisBlocks[y][x - 1] = 1;
				tetrisBlocks[y][x + 1] = 1;
			}
		}
	}

	if (piece.getType() == "S")
	{
		int x = piece.getOldPosition().x + 7;
		int y = piece.getOldPosition().y + 7;

		tetrisBlocks[y][x] = 0;
		tetrisBlocks[y + 1][x] = 0;
		tetrisBlocks[y + 1][x + 1] = 0;
		tetrisBlocks[y][x + 1] = 0;

		 x = piece.getPosition().x + 7;
		 y = piece.getPosition().y + 7;

		if ((y < 0) || (tetrisBlocks[y + 1][x] == 1) || (tetrisBlocks[y ][x + 1] == 1) || (tetrisBlocks[y][x] == 1) || (tetrisBlocks[y + 1][x + 1] == 1))
		{
			resetPiece();
		}
		else {
			tetrisBlocks[y][x] = 1;
			tetrisBlocks[y + 1][x] = 1;
			tetrisBlocks[y + 1][x + 1] = 1;
			tetrisBlocks[y][x + 1] = 1;
		}
	}

	piece.setOldPosition(piece.getPosition());
	piece.setOldRotation(piece.getRotation());
}

//////////////////////////////////////////////////////////////////////////////
//
//   DISPLAY FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////////

void background(glm::vec3& rotation)
{
	//BACKGROUND OBJECT
	glBindVertexArray(VAOs[0]);

	//SETTING POSITION
	glm::vec3 backposition(0.0f);
	backposition.z = -20.f;
	backposition.y = 0.f;

	//CREATING MODEL MATRIX
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 backmodel = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	backmodel = glm::translate(backmodel, backposition);
	glm::mat4 mv = backmodel * view;

	int mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	int pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//DRAWS BACKGROUND SHAPE
	glBindTexture(GL_TEXTURE_2D, textureBackground);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

}

void tetrisPieces()
{
	glBindVertexArray(VAOs[2]);

	//RENDERSS ARRAY IN FOUR SECTIONS
	for (float i = 0; i < 7; i++)
	{
		for (float j = 0; j < 8; j++)
		{
			if (tetrisBlocks[std::atomic_uint(i)][std::atomic_uint(j)] == 1) //IF PIECE IS AT POSITION DISPLAY CUBE
			{
				//SETS POSITION OF ARRAY
				positionBase.y = (i - 7.f);
				positionBase.x = (j - 7.f);

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, positionBase);

				//CREATING VIEW MATRIX
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
				glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
				glm::mat4 mv = model * view;

				int mvLoc = glGetUniformLocation(program, "mv_matrix");
				glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

				int pLoc = glGetUniformLocation(program, "p_matrix");
				glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

				//DRAWS OBJECT IN WINDOW
				glBindVertexArray(VAOs[2]);
				glBindTexture(GL_TEXTURE_2D, texture1);
				glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
			}
		}
	}

	for (float i = 14; i >= 7; i--)
	{
		for (float j = 0; j < 8; j++)
		{
			if (tetrisBlocks[std::atomic_uint(i)][std::atomic_uint(j)] == 1)//IF PIECE IS AT POSITION DISPLAY CUBE
			{
				//SETS POSITION OF ARRAY
				positionBase.y = (i - 7.f);
				positionBase.x = (j - 7.f);

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, positionBase);

				//CREATING VIEW MATRIX
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
				glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
				glm::mat4 mv = model * view;

				int mvLoc = glGetUniformLocation(program, "mv_matrix");
				glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

				int pLoc = glGetUniformLocation(program, "p_matrix");
				glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

				//DRAWS OBJECT IN WINDOW
				glBindVertexArray(VAOs[2]);
				glBindTexture(GL_TEXTURE_2D, texture1);
				glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

			}
		}
	}

	for (float i = 0; i < 7; i++)
	{
		for (float j = 14; j > 7; j--)
		{
			if (tetrisBlocks[std::atomic_uint(i)][std::atomic_uint(j)] == 1)//IF PIECE IS AT POSITION DISPLAY CUBE
			{
				//SETS POSITION OF ARRAY
				positionBase.y = (i - 7.f);
				positionBase.x = (j - 7.f);

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, positionBase);

				//CREATING VIEW MATRIX
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
				glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
				glm::mat4 mv = model * view;

				int mvLoc = glGetUniformLocation(program, "mv_matrix");
				glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

				int pLoc = glGetUniformLocation(program, "p_matrix");
				glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

				//DRAWS OBJECT IN WINDOW
				glBindVertexArray(VAOs[2]);
				glBindTexture(GL_TEXTURE_2D, texture1);
				glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
			}
		}
	}

	for (float i = 14; i >= 7; i--)
	{
		for (float j = 14; j > 7; j--)
		{
			if (tetrisBlocks[std::atomic_uint(i)][std::atomic_uint(j)] == 1)
			{
				//SETS POSITION OF ARRAY
				positionBase.y = (i - 7.f);
				positionBase.x = (j - 7.f);

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, positionBase);

				//CREATING VIEW MATRIX
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
				glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
				glm::mat4 mv = model * view;

				int mvLoc = glGetUniformLocation(program, "mv_matrix");
				glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

				int pLoc = glGetUniformLocation(program, "p_matrix");
				glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

				//DRAWS OBJECT IN WINDOW
				glBindVertexArray(VAOs[2]);
				glBindTexture(GL_TEXTURE_2D, texture1);
				glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

			}
		}
	}
}

void displayButtons() {

	glBindVertexArray(VAOs[2]);

	//CREATING MODEL MATRIX
	glm::vec3 position(0.0f);
	position.z = -3.5f;
	position.y = .5f;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	//CREATING VIEW MATRIX
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
	glm::mat4 mv = model * view;

	int mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	int pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//DRAWS OBJECT IN WINDOW
	if (Game == Game_End)
	{
		glBindTexture(GL_TEXTURE_2D, playagain);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, textureMenuStart);
	}

	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

	//SETS POSITION FOR EXIT BUTTON
	position.z = 0;
	position.y = -1.2;

	model = glm::translate(model, position);

	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

	mv = model * view;

	mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//DRAWS OBJECT IN WINDOW
	glBindVertexArray(VAOs[2]);
	glBindTexture(GL_TEXTURE_2D, textureMenuExit);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

}

void score()
{
	glBindVertexArray(VAOs[1]);

	glm::vec3 backposition(0.0f);

	//SETS POSITION DEPENDING ON GAME STATE
	if (Game == Game_End)
	{
		backposition.z = -6.1f;
		backposition.y = 2.8f;
		backposition.x = -1.5f;
	}
	else
	{
		backposition.z = -6.1f;
		backposition.y = -2.8f;
		backposition.x = 1.f;
	}
	//CREATING MODEL MATRIX
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, backposition);

	//CREATING VIEW MATRIX
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
	glm::mat4 mv = model * view;

	int mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	int pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//DRAWS OBJECT IN WINDOW
	glBindTexture(GL_TEXTURE_2D, userscore);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

	//CHANGES POSITION AND SCALE OF NUMBERS
	backposition.z = 0.f;
	backposition.y = -0.4f;
	backposition.x = 1.7f;
	model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
	int pos = 0;

	//DISPLAYS THE SCORE
	for (size_t i = 6; i > 0; i--)
	{
		model = glm::translate(model, backposition);

		mv = model * view;

		mvLoc = glGetUniformLocation(program, "mv_matrix");
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

		pLoc = glGetUniformLocation(program, "p_matrix");
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

		std::string currentScore = std::to_string(Score);

		if (currentScore.size() < i) {
			glBindTexture(GL_TEXTURE_2D, zero);
		}
		else
		{
			char ch = currentScore.at(pos);
			pos++;
			int num = (int)ch - 48;

			//DEPENDING ON NUMBER CHANGE TEXTURE
			switch (num) {
			case 1:
				glBindTexture(GL_TEXTURE_2D, one);
				break;
			case 2:
				glBindTexture(GL_TEXTURE_2D, two);
				break;
			case 3:
				glBindTexture(GL_TEXTURE_2D, three);
				break;
			case 4:
				glBindTexture(GL_TEXTURE_2D, four);
				break;
			case 5:
				glBindTexture(GL_TEXTURE_2D, five);
				break;
			case 6:
				glBindTexture(GL_TEXTURE_2D, six);
				break;
			case 7:
				glBindTexture(GL_TEXTURE_2D, seven);
				break;
			case 8:
				glBindTexture(GL_TEXTURE_2D, eight);
				break;
			case 9:
				glBindTexture(GL_TEXTURE_2D, nine);
				break;
			case 0:
				glBindTexture(GL_TEXTURE_2D, zero);
				break;
			default:
				glBindTexture(GL_TEXTURE_2D, zero);
				break;
			}

		}
		//DRAWS OBJECT IN WINDOW
		glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

		backposition.y = 0.f;
		backposition.x = 0.9f;
	}
}

void displayMenu(GLfloat delta)
{
	//RESET WINDOW TO BLACK
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);

	//BIND TEXTURES ON TEXTURE UNITS
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(VAOs[0]);

	// CREATING MODEL MATRIX
	glm::vec3 position(0.0f);
	position.z = -12.5f;
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);

	// CREATING VIEW MATRIX
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
	glm::mat4 mv = model * view;

	int mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));

	int pLoc = glGetUniformLocation(program, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//DRAWS OBJECT IN WINDOW
	glBindTexture(GL_TEXTURE_2D, textureMenu);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

	//DISPLAYS BUTTONS OVER MENU BACKGROUND
	glBindVertexArray(0);
	displayButtons();

}

void displayEnd()
{
	//RESET WINDOW TO BLACK
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);

	//BIND TEXTURES ON TEXTURE UNITS
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//DISPLAYS BUTTONS AND END SCORE
	displayButtons();
	score();

}

void display(GLfloat delta, glm::vec3& position, glm::vec3& rotation)
{
	//RESET WINDOW TO BLACK
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind textures on corresponding texture units
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//MOVES BLOCK DOWN AUTOMATICALLY
	if (delta > time)
	{
		position.y -= 1.f;
		time = time + 1.f;
		Sleep(20);
	}

	//CREATES TETRIS GAME SPACE
	background(rotation);
	score();
	currentPiece(position, rotation);
	tetrisPieces();
}

//////////////////////////////////////////////////////////////////////////////
//
//   MAIN
//
//////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	//INIT GLFW
	glfwInit();

	//CHANGE WINDOW SIZE HERE
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	//REMOVES RESIZING WINDOW FEATURE
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//CREATES WINDOW
	GLFWwindow* window = glfwCreateWindow(800, 600, "3D Tetris", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	//CREATES OBJECT
	gameTextures();
	init();
	initBackground();
	initScore();
	initBackgroundMenuStart();

	//CREATES CURRENT PEICE
	piece = Tetris();
	
	//SETS POSITION 
	position.z = -19.f;
	positionBase.z = -19.f;
	position = piece.getPosition();

	while (!glfwWindowShouldClose(window))
	{
		//CHECKS FOR INPUT
		inputUpdate(window, position, rotation);

		//DISPLAYS GAME
		if (Game == Game_Menu) {
			displayMenu(timer);
		}
		else if (Game == Game_Active) {
			display(timer, position, rotation);
			timer += multiplier; //WILL INCREASE SPEED AT MUTIPLAYER INCREASES
		}
		else if (Game == Game_End) {
			timer = 0.0f;
			time = 1.f;
			multiplier = 0.01f;
			displayEnd();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();	
	}

	//USER EXITS WINDOW
	glfwDestroyWindow(window);
	glfwTerminate();
}