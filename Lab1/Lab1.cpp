// THIS LAB STARTS WITH CODE FROM TUTORIAL 1 FROM OPENGL-TUTORIAL.ORG CODE

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// we're using shaders so need this for the "LoadShaders" function
#include <common/shader.hpp>

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
	window = glfwCreateWindow( 1024, 768, "Lab 1 - Triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	// define your triangle data here.. don't be scared to experiment at this point
	// triangle has 3 floats per vertex and 3 vertices = 9 floats!! note: winding order is important!!


	// define vertex colour data here (same as above, but 4 floats per vertex (RGBA values in range 0..1)


	// now we need to let OpenGL know about the vertex data.. 
	// the order is...
	// 1) define a vertex array object (using glGenVertexArrays)
	// 2) bind it as the active VAO (glBindVertexArray)
	// 3) define a vertex buffer object (glGenBuffers)
	// 4) bind it as the active buffer object (glBindBuffer)
	// 5) fill the buffer with data (glBufferData)
	// 6) enable the correct "vertex attribute array" (glEnableVertexAttribArray)
	// 7) tell OpenGL where in the vertex buffer the attibute is found (glVertexAttribPointer)
	// we then can draw the content by calling:
	// 1) glDrawArrays and telling it to draw triangles! (check the opengl spec for parameters to all these calls)

	// create a vertex array ID so we can bind our buffer to it
	
	// create a vertex buffer object id and fill it with our data

	// 1st attribute buffer : vertices

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("Lab1VertexShader.vertexshader", "Lab1FragmentShader.fragmentshader");

	// Get a handle for our "MVP" (model * view * projection) uniform

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 6), // Camera is at (0,0,6), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


	do{
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw your coloured triangle here! 
		// Use our shader

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform

		// Draw the triangle !

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

