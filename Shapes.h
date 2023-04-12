#pragma once

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

#include<vector>

static GLfloat  Bcolours[][4] = {
	{ 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 0.1f },
	{ 0.0f, 1.0f, 0.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 0.1f },
	{ 0.0f, 1.0f, 0.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 0.1f },
};

static GLfloat Bvertices[][3] = {
	{0.5f,  0.5f, -0.5f},  //0 top right
	{0.5f, -0.5f, -0.5f},  //1 bottom right
	{-0.5f, -0.5f, -0.5f}, //2 bottom left
	{-0.5f,  0.5f, -0.5f},  //3 top left

	{0.5f,  0.5f, 0.5f},  //4 top right
	{0.5f, -0.5f, 0.5f},  //5 bottom right
	{-0.5f, -0.5f, 0.5f}, //6 bottom left
	{-0.5f,  0.5f, 0.5f}  //7 top left 
};

static GLuint Bindices[][3] = {  // note that we start from 0!
	{0, 3, 1},  // first Triangle front
	{3, 2, 1},   // second Triangle

	{4, 7, 0 },
	{7, 3, 0 },

	{1, 2, 5 },
	{2, 6, 5 },

	{5, 4, 0 },
	{0, 1, 5 },

	{2, 3, 7 },
	{7, 6, 2 },

	{4, 5, 7 },  // first Triangle back
	{7, 5, 6 }   // second Triangle
};

static GLfloat  Btexture_coords[] = {
		 1.0f, 1.0f,
		 1.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 1.0f,

		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
};

static GLfloat Bnormals[]{
0.0f,  0.0f, -1.0f,//point down
0.0f,  0.0f, -1.0f,
0.0f,  0.0f, -1.0f,
0.0f,  0.0f, -1.0f,
0.0f,  0.0f, -1.0f,
0.0f,  0.0f, -1.0f,

0.0f,  0.0f, 1.0f,//point up
0.0f,  0.0f, 1.0f,
0.0f,  0.0f, 1.0f,
0.0f,  0.0f, 1.0f,
0.0f,  0.0f, 1.0f,
0.0f,  0.0f, 1.0f,

-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,

1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,

0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,

0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f };

static GLfloat  Scorecolours[][4] = {
	{ 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 0.1f },
	{ 0.0f, 1.0f, 0.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 0.1f },
	{ 0.0f, 1.0f, 0.0f, 0.1f }, { 0.0f, 1.0f, 0.0f, 0.1f },
};

static GLfloat Scorevertices[][3] = {
	{0.5f,  0.5f, -0.5f},  //0 top right
	{0.5f, -0.5f, -0.5f},  //1 bottom right
	{-0.5f, -0.5f, -0.5f}, //2 bottom left
	{-0.5f,  0.5f, -0.5f},  //3 top left

	{0.5f,  0.5f, 0.5f},  //4 top right
	{0.5f, -0.5f, 0.5f},  //5 bottom right
	{-0.5f, -0.5f, 0.5f}, //6 bottom left
	{-0.5f,  0.5f, 0.5f}  //7 top left 
};

static GLuint Scoreindices[][3] = {  // note that we start from 0!
	{4, 5, 7 },  // first Triangle back
	{7, 5, 6 } ,  // second Triangle

};

static GLuint Scoreindices2[][3] = {  // note that we start from 0!
	{0, 3, 1},  // first Triangle front
	{3, 2, 1},   // second Triangle

	{4, 7, 0 },
	{7, 3, 0 },

	{1, 2, 5 },
	{2, 6, 5 },

	{5, 4, 0 },
	{0, 1, 5 },

	{2, 3, 7 },
	{7, 6, 2 },

	{4, 5, 7 },  // first Triangle back
	{7, 5, 6 }   // second Triangle
};

static GLfloat  Scoretexture_coords[] = {
		 1.0f, 1.0f,
		 1.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 1.0f,

		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
};

static GLfloat Scorenormals[]{

0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f };

static GLfloat  backgroundColours[][4] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f },
};

static GLfloat backgroundVertices[][3] = {

	{5.5f,  5.5f, -5.5f},  //0 top right
	{5.5f,  -5.5f, -5.5f},  //1 bottom right
	{-5.5f,  -5.5f, -5.5f},  //2 bottom left
	{-5.5f,  5.5f, -5.5f},  //3 top left

	{5.5f,  5.5f, 5.5f},  //4 top right
	{5.5f,  -5.5f, 5.5f},  //5 bottom right
	{-5.5f,  -5.5f, 5.5f},  //6 bottom left
	{-5.5f,  5.5f, 5.5f},  //7 top left

};

static GLuint backgroundIndices[][3] = {  // note that we start from 0!
	{0, 3, 1},  // first Triangle front
	{3, 2, 1},   // second Triangle

	{4, 7, 0 },
	{7, 3, 0 },

	{1, 2, 5 },
	{2, 6, 5 },

	{5, 4, 0 },
	{0, 1, 5 },

	{2, 3, 7 },
	{7, 6, 2 },

	{4, 5, 7 },  // first Triangle back
	{7, 5, 6 }   // second Triangle
};

static GLfloat  backgroundTexture_coords[] = {
		 1.0f, 1.0f,
		 1.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 1.0f,

		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
};

static GLfloat backgroundNormals[]{
0.0f,  0.0f, -1.0f,//point down
0.0f,  0.0f, -1.0f,
0.0f,  0.0f, -1.0f,
0.0f,  0.0f, -1.0f,
0.0f,  0.0f, -1.0f,
0.0f,  0.0f, -1.0f,

0.0f,  0.0f, 1.0f,//point up
0.0f,  0.0f, 1.0f,
0.0f,  0.0f, 1.0f,
0.0f,  0.0f, 1.0f,
0.0f,  0.0f, 1.0f,
0.0f,  0.0f, 1.0f,

-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,
-1.0f,  0.0f,  0.0f,

1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,
1.0f,  0.0f,  0.0f,

0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,
0.0f, -1.0f,  0.0f,

0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f,
0.0f,  1.0f,  0.0f };

