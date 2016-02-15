#include "stdafx.h"
#include "SFML/OpenGL.hpp"
#include "Terrain.h"
#include <cmath>
#include <iostream>

Terrain::Terrain(void)
{
	gridWidth=100;
	gridDepth=100;

	terrWidth=50; //size of terrain in world units
	terrDepth=50;
	normals = NULL;
	vertices=NULL;
	colors=NULL;	
	texCoords = NULL;
	
	//num squares in grid will be width*height, two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth*gridWidth*2*3;

	 image.loadFromFile("../test.jpg");
}

Terrain::~Terrain(void)
{
	delete[] normals;
	delete [] vertices;
	delete [] colors;
	delete[] texCoords;
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

void Terrain::setTexCoords(vector v, float x, float y){

	v[0] = x;
	v[1] = y;	
}

void Terrain::calculateNormal(vector normalsVec, vector point1, vector point2, vector point3){

	vector u, v, normal = {};

	float sum = FLT_MIN;
	sum += (point2[0] - point1[0]) * (point2[1] - point1[1]) * (point2[2] - point1[2]);
	sum += (point3[0] - point2[0]) * (point3[1] - point2[1]) * (point3[2] - point2[2]);
	sum += (point1[0] - point3[0]) * (point1[1] - point3[1]) * (point1[2] - point3[2]);

	int direction = 1;
	if (sum > 0)
		direction = -1;

	u[0] = point2[0] - point1[0];
	u[1] = point2[1] - point1[1];
	u[2] = point2[2] - point1[2];
	
	v[0] = point3[0] - point1[0];	
	v[1] = point3[1] - point1[1];
	v[2] = point3[2] - point1[2];

	normal[0] = (u[1] * v[2]) - (u[2] * v[1]);
	normal[1] = (u[2] * v[0]) - (u[0] * v[2]);
	normal[2] = (u[0] * v[1]) - (u[1] * v[0]);

	//normalise the vector by dividing by its length
	double length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

	normalsVec[0] = direction * (normal[0] / length);
	normalsVec[1] = direction * (normal[1] / length);
	normalsVec[2] = direction * (normal[2] / length);
}

//helper function to calculate height of terrain at a given point in space
//you will need to modify this significantly to pull height from a map
float  Terrain::getHeight(int x, int y){
	//for the sample we will calculate height based on distance form origin
	float dist = pixelArray[y][x] * m_heightMultiplier;
	return dist;
}

void Terrain::NormalVector(GLfloat p1[], GLfloat p2[], GLfloat p3[], GLfloat n[])
{

	GLfloat v1[3], v2[3]; // two vectors

	//calculate two vectors lying on the surface
	// v1=p2-p1
	// v2=p3-p2

	for (int i = 0; i < 3; i++){
		v1[i] = p2[i] - p1[i];
		v2[i] = p3[i] - p2[i];
	}

	// calculate cross product of two vectors ( n= v1 x v2)
	n[0] = v1[1] * v2[2] - v2[1] * v1[2];
	n[1] = v1[2] * v2[0] - v2[2] * v1[0];
	n[2] = v1[0] * v2[1] - v2[0] * v1[1];



}

void Terrain::Draw()
{

	glBegin(GL_TRIANGLES);
	int counter = 0;
	bool once = true;
	for (int i = 0; i < numVerts; i++){
		if (i == currentTris){
			NormalVector(vertices[i], vertices[i + 2], vertices[i + 1], normal);
			currentTris += 3;
			glNormal3fv(normal);
			counter++;

		}
		glColor3fv(colors[i]);
		glTexCoord2fv(texCoords[i]);

		glVertex3fv(vertices[i]);


	}
	currentTris = 0;
	int i = counter;
	glEnd();

}

void Terrain::Init()
{
	delete[] normals;
	normals = new vector[numVerts];
	delete[] vertices;//just in case we've called init before
	vertices = new vector[numVerts];
	delete[] colors;
	colors = new vector[numVerts];
	delete[] texCoords;
	texCoords = new vector[numVerts];

	for (int i = 0; i < gridWidth; i++)
	{ //iterate left to right
		for (int j = 0; j < gridDepth; j++)
		{//iterate front to back
			color = image.getPixel(j, i);
			pixelArray[j][i] = ((color.r / 255.0f));
		}
	}

	//interpolate along the edges to generate interior points
	for (int i = 0; i < gridWidth - 1; i++)
	{ //iterate left to right	
		for (int j = 0; j < gridDepth - 1; j++)
		{//iterate front to back
			int sqNum = (j + i*gridDepth);
			int vertexNum = sqNum * 3 * 2; //6 vertices per square (2 tris)
			int firstVertexNum = vertexNum;
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
			setPoint(colors[vertexNum], 1.f, 1.f, 1.f);
			setTexCoords(texCoords[vertexNum], (float)i / (gridWidth / 4 - 2), (float)j / (gridDepth / 4 - 2));
			setPoint(vertices[vertexNum++], left, getHeight(j, i), front);
			//setPoint(vertices[vertexNum++], left, getHeight(left, front), front);


			setPoint(colors[vertexNum], 0.f, 1.f, 0.f);
			setTexCoords(texCoords[vertexNum], (float)(i + 1) / (gridWidth / 4 - 2), (float)j / (gridDepth / 4 - 2));
			setPoint(vertices[vertexNum++], right, getHeight(j, i + 1), front);
			//setPoint(vertices[vertexNum++], right, getHeight(right, front), front);//bottom right

			setPoint(colors[vertexNum], 0.f, 0.f, 1.f);
			setTexCoords(texCoords[vertexNum], (float)(i + 1) / (gridWidth / 4 - 2), (float)(j + 1) / (gridDepth / 4 - 2));
			setPoint(vertices[vertexNum++], right, getHeight(j + 1, i + 1), back);
			//setPoint(vertices[vertexNum++], right, getHeight(right, back), back); //top right

			//tri2
			setPoint(colors[vertexNum], 1.f, 0.f, 0.f);
			setTexCoords(texCoords[vertexNum], (float)(i + 1) / (gridWidth / 4 - 2), (float)(j + 1) / (gridDepth / 4 - 2));
			setPoint(vertices[vertexNum++], right, getHeight(j + 1, i + 1), back);
			//setPoint(vertices[vertexNum++], right, getHeight(right, back), back);

			setPoint(colors[vertexNum], 0.f, 0.f, 1.f);
			setTexCoords(texCoords[vertexNum], (float)i / (gridWidth / 4 - 2), (float)(j + 1) / (gridDepth / 4 - 2));
			setPoint(vertices[vertexNum++], left, getHeight(j + 1, i), back);
			//setPoint(vertices[vertexNum++], left, getHeight(left, back), back); //top left

			setPoint(colors[vertexNum], 0.f, 1.f, 0.f);
			setTexCoords(texCoords[vertexNum], (float)i / (gridWidth / 4 - 2), (float)j / (gridDepth / 4 - 2));
			setPoint(vertices[vertexNum++], left, getHeight(j, i), front);
			//setPoint(vertices[vertexNum++], left, getHeight(left, front), front); //bottom left 
		}
	}
}
