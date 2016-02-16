//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
 
#include "SFML/Graphics.hpp" 
#include "SFML/Graphics/Shader.hpp"
#include "SFML/OpenGL.hpp" 
#include <iostream> 
  
 
#include "Terrain.h"
#include "Camera.h"




int main() 
{ 
    // Create the main window 
	int width = 600;
	int height = 600;
	bool wireframe = true;

	sf::Texture sea; //texture object called car
	sf::Texture sand; //texture object called car
	sf::Texture grass; //texture object called car
	sf::Texture rock; //texture object called car
	sf::Texture snowrock; //texture object called car

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	sea.loadFromFile("../water.png");
	sand.loadFromFile("../sand.png");
	grass.loadFromFile("../grass.png");
	rock.loadFromFile("../rock.png");
	snowrock.loadFromFile("../snowyrock.png");

	// create the window
	sf::RenderWindow App(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(24));
    // Create a clock for measuring time elapsed     
    sf::Clock Clock; 

	aiVector3D position(0,10,-30);
	Camera camera;
    camera.Init(position); //create a camera

	//prepare OpenGL surface for HSR 
	glClearDepth(1.f);
	glClearColor(0.3f, 0.3f, 0.6f, 0.f); //background colour
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//// Setup a perspective projection & Camera position 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 5.0, 2.6, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color); // set color of diffuse component
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color); // set color of specular component
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // set position
	///////////////////////////setting material
	GLfloat materialAmbDiff[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create an array of RGBA values
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiff); // set the diffuse & ambient reflection colour for the front of faces
	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create an array of RGBA values (White)
	GLfloat materialShininess[] = { 128.0f }; // select value between 0-128, 128=shiniest
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular); // set the colour of specular reflection
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess); // set shininess of the material
   
	
	//set up a 3D Perspective View volume
    gluPerspective(90.f, (float)width/height, 1.f, 300.0f);//fov, aspect, zNear, zFar 

	//load & bind the shader
	sf::Shader shader;
	//all the lighting & texture blending code should  be put in 'fragment.glsl'
	if(!shader.loadFromFile("vertex.glsl","fragment.glsl")){
        exit(1);
    }

	shader.setParameter("sea", sea);
	shader.setParameter("sand", sand);
	shader.setParameter("grass", grass);
	shader.setParameter("rock", rock);
	shader.setParameter("snowrock", snowrock);


	//Create our Terrain
	Terrain terrain;
	terrain.Init();

    // Start game loop 
    while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::O))
			{
				// select the projection matrix and clear it out
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				
				// set up an orthographic projection with the same near clip plane
				glOrtho(-16.0, 16.0, -10.0, 16.0, 1.f, 300.0f);

				// select modelview matrix and clear it out
				glMatrixMode(GL_MODELVIEW);
			}
			else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::P))
			{	
				// select the projection matrix and clear it out
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				
				glFrustum(-5.0, 5.0, -5.0, 5.0, 5, 100);

				// select modelview matrix and clear it out
				glMatrixMode(GL_MODELVIEW);
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::I))
			{
				wireframe = !wireframe;

				if (wireframe == true)
				{
					glPolygonMode(GL_FRONT, GL_FILL);
					glPolygonMode(GL_BACK, GL_FILL);
				}
				else
				{
					glPolygonMode(GL_FRONT, GL_LINE);
					glPolygonMode(GL_BACK, GL_LINE);
				}				
			}			

			//update the camera
			camera.Update(Event);
        } 	

        //Prepare for drawing 
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		sf::Shader::bind(&shader);
		GLfloat light_position[] = { 5.0, 2, 0.5, 0.1 };

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // set position
        // Apply some transformations 
        //initialise the worldview matrix
		glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity(); 

		//get the viewing transform from the camera
		camera.ViewingTransform();

		//make the world spin
		//TODO:probably should remove this in final
		//static float ang=0.0;
		//ang+=0.01f;
		//glRotatef(ang*2,0,1,0);//spin about y-axis
		
		//draw the world
		terrain.Draw();

        // Finally, display rendered frame on screen 
        App.display(); 
    } 
   
    return EXIT_SUCCESS; 
}
