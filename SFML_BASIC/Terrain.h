#pragma once
#include "SFML/OpenGL.hpp"
#include "SFML/Graphics.hpp" 
class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth,gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth,terrDepth;

	typedef  GLfloat vector[3];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector *texCoor;
	vector *vertices;
	vector *colors;

	sf::Image image;
	sf::Color color;

	float m_colorHeight[4];
	const float m_HEIGHT = 6;

	float getHeight(float x, float y);
	void setPoint(vector, float, float,float);
	
public:
	Terrain(void);
	~Terrain(void);

	void Init();
	void Draw();
};
