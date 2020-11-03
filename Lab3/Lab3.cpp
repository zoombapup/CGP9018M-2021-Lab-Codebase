// THIS LAB STARTS WITH CODE FROM TUTORIAL 1 FROM OPENGL-TUTORIAL.ORG CODE

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// the code for both of these is adapted from examples on LearnOpenGL.com
#include "common/model.h"
#include "common/shader.h"
// can't remember where this comes from.. there are lots of examples on the internet of similar code
#include "common/GLError.h"

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Lab 3 - Phong Lighting", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	check_gl_error();

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	check_gl_error();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	check_gl_error();

	double currentTime = glfwGetTime();
	double lastTime = currentTime;

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	
	check_gl_error();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	
	check_gl_error();

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	check_gl_error();

	// TODO: you should create a PhongSpec lighting model in these shaders
	Shader StandardShader("StandardShading.vertexshader", "StandardShading.fragmentshader");
	check_gl_error();

	// TODO: Try loading different models here.. also, maybe add more models? check out the model class code
	Model matModel("../3dcontent/models/mat_scaled/mat_scaled.obj");
	//ourModel.GetMesh(0)->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
	
	check_gl_error();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera is at (2,2,1), in World Space
	glm::vec3 cameraPosition(2, 2, 1);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		cameraPosition, 
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	do{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	
		// update view matrix with new camera position
		View = glm::lookAt(cameraPosition,glm::vec3(0, 0.5f, 0),glm::vec3(0, 1, 0));

		// binds the shader as the current one in use
		StandardShader.use();

		// bind shader uniforms here.. have a look at the Shader class for helper function names
		// eg StandardShader.setFloat("name",0.1f);

		// you probably want to update any uniforms you need to before calling render!

	
		// Draw your model here! note: we pass in View and Projection matrices, but Model matrix is held within mesh class inside Model class
		matModel.Render(View, Projection, StandardShader);

	

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// check keys and modify variables here.. these will then get passed to the shaders via uniforms you set above


	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

