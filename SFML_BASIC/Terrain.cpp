#include "stdafx.h"
#include "SFML/OpenGL.hpp"
#include "Terrain.h"
#include <cmath>

Terrain::Terrain(void)
{
	gridWidth=100;
	gridDepth=100;

	terrWidth=50; //size of terrain in world units
	terrDepth=50;
	vertices=NULL;
	colors=NULL;	
	texCoor = NULL;
	
	//num squares in grid will be width*height, two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth*gridWidth*2*3;

	 image.loadFromFile("../test.jpg");
}


Terrain::~Terrain(void)
{
	delete [] vertices;
	delete [] colors;
	delete[] texCoor;
}

//interpolate between two values
float lerp(float start, float end, float t){
	return start+(end-start)*t;
}


void Terrain::setPoint(vector v,float x, float y, float z){

		v[0]=x;
		v[1]=y;
		v[2]=z;
}

//helper function to calculate height of terrain at a given point in space
//you will need to modify this significantly to pull height from a map
float  Terrain::getHeight(int x, int y){
	//for the sample we will calculate height based on distance form origin
	float dist = pixelArray[y][x] * m_HEIGHT * m_HEIGHT;
	return dist;
}

void Terrain::Init(){

	delete[] vertices;//just in case we've called init before
	vertices = new vector[numVerts];
	delete[] colors;
	colors = new vector[numVerts];
	delete[] texCoor;
	texCoor = new vector[numVerts];

	for (int i = 0; i < gridWidth; i++){ //iterate left to right
		for (int j = 0; j < gridDepth; j++){//iterate front to back
			color = image.getPixel(j, i);
			pixelArray[j][i] = ((color.r / 255.0f));
		}



		//interpolate along the edges to generate interior points
		for (int i = 0; i < gridWidth - 1; i++){ //iterate left to right	
			for (int j = 0; j < gridDepth - 1; j++){//iterate front to back
				int sqNum = (j + i*gridDepth);
				int vertexNum = sqNum * 3 * 2; //6 vertices per square (2 tris)
				float front = lerp(-terrDepth / 2, terrDepth / 2, (float)j / gridDepth);
				float back = lerp(-terrDepth / 2, terrDepth / 2, (float)(j + 1) / gridDepth);
				float left = lerp(-terrWidth / 2, terrWidth / 2, (float)i / gridDepth);
				float right = lerp(-terrDepth / 2, terrDepth / 2, (float)(i + 1) / gridDepth);

				/*
				back   +-----+	looking from above, the grid is made up of regular squares
				|tri1/|	'left & 'right' are the x cooded of the edges of the square
				|   / |	'back' & 'front' are the y coords of the square
				|  /  |	each square is made of two trianlges (1 & 2)
				| /   |
				|/tri2|
				front  +-----+
				left   right
				*/

				//tri1
				setPoint(colors[vertexNum], 0.f, 0.f, 0.f);
				setPoint(vertices[vertexNum++], left, getHeight(j, i), front);
				setPoint(colors[vertexNum], 0.f, 1.f, 0.f);
				setPoint(vertices[vertexNum++], right, getHeight(j, i + 1), front);
				setPoint(colors[vertexNum], 0.f, 0.f, 1.f);
				setPoint(vertices[vertexNum++], right, getHeight(j + 1, i + 1), back);

				//tri2
				setPoint(colors[vertexNum], 1.f, 0.f, 0.f);
				setPoint(vertices[vertexNum++], right, getHeight(j + 1, i + 1), back);
				setPoint(colors[vertexNum], 0.f, 1.f, 0.f);
				setPoint(vertices[vertexNum++], left, getHeight(j, i), front);
				setPoint(colors[vertexNum], 0.f, 0.f, 1.f);
				setPoint(vertices[vertexNum++], left, getHeight(j + 1, i), back);
			}
		}
	}
}


void Terrain::Draw(){
	glBegin(GL_TRIANGLES);
	for(int i =0;i<numVerts;i++){
			glColor3fv(colors[i]);
			glVertex3fv(vertices[i]);
			glTexCoord2fv(texCoor[i]);
	}
	glEnd();
}