#pragma once

extern enum VAO_IDs { Shapes, Indices, Colours, Tex, Normals, NumVAOs = 6 };
extern enum Buffer_IDs { ArrayBuffer, NumBuffers = 5 };
extern enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2, nPosition = 3 };
extern enum GameState { Game_Menu = 0, Game_Active, Game_End };

static GLuint  VAOs[NumVAOs];
static GLuint  Buffers[NumBuffers];
static GLuint  BufferBase[NumBuffers];
static GLfloat program;

static GLuint textureMenuStart, textureMenuExit, textureMenuEnd, texture1, textureBackground, textureMenu, playagain;
static GLuint zero, one, two, three, four, five, six, seven, eight, nine, userscore;

static const GLuint Base = 0;
static const GLuint  NumVertices = 36;
static GameState Game = Game_Menu;
static int Score = 0;
static glm::vec3 positionBase(0.0f);
static GLfloat timer = 0.0f;
static int tetrisBlocks[15][15] = {};
static int allRows[15] = {};
static GLfloat time = 1;
static GLfloat multiplier = 0.01f;
static glm::vec3 position(0.0f);
static glm::vec3 rotation(0.0f);

void gameTextures();

void initBackground(void);

void initScore(void);

void init(void);

void loadTexture(GLuint& texture, std::string texturepath);

void inputUpdate(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation);

void background(glm::vec3& rotation);

void displayButtons();

void displayMenu(GLfloat delta);

void score();

void tetrisPieces();

void currentPiece(glm::vec3& position, glm::vec3& rotation);

void display(GLfloat delta, glm::vec3& position, glm::vec3& rotation);
