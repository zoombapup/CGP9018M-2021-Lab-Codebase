// THIS LAB STARTS WITH CODE FROM TUTORIAL 5 FROM OPENGL-TUTORIAL.ORG CODE


// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// we're using shaders so need this for the "LoadShaders" function
#include <common/shader.hpp>

// soil2 image loading library.. so we can use JPG/TGA instead of the horrible BMP!
#include "SOIL2.h"

// add your rotation code here....
void RotateCube(glm::mat4 & ModelMat, float timedelta)
{
	// TODO actually do the rotation.. note the ModelMat is passed by ref so you can change it without returning.. is that a bad idea?
}

// function to calculate vertex normals by taking three vertices of triangle and using two edges to get the normal via cross product
// WARNING THIS IS BAD CODE!! (doesn't check input ranges, doesn't check for null pointer inputs, writes into a buffer unguarded.. EVIL EVIL EVIL)
// but who cares as this is only practice code!! :)
void CalculateNormals(const GLfloat * vertices, float * normalbuffer)
{
	unsigned int output = 0;
	unsigned int input = 0;

	for (unsigned int i = 0; i < 12; i++)
	{
		// first vertex
		glm::vec3 vertex1(vertices[input + 0], vertices[input + 1], vertices[input + 2]);
		// second vertex
		glm::vec3 vertex2(vertices[input + 3], vertices[input + 4], vertices[input + 5]);
		// third vertex
		glm::vec3 vertex3(vertices[input + 6], vertices[input + 7], vertices[input + 8]);
		// the "stride" of the input.. 3 floats each for 3 vertices = 9 floats total
		input += 9;

		// line between our first vertex and our second
		glm::vec3 v1tov2(glm::normalize(vertex2 - vertex1));
		// line between our first vertex and our third
		glm::vec3 v1tov3(glm::normalize(vertex3 - vertex1));
		// cross product to give us the normal (note we normalize the two input vectors first)
		glm::vec3 normal(glm::cross(v1tov2, v1tov3));
		// normalize the resultant normal (shouldn't be needed, but I'm paranoid)
		normal = normalize(normal);
		// write our three vertices worth of normal with the same value (i.e. a flat face with all vertex normals pointing the same way)
		normalbuffer[output + 0] = normal.x;
		normalbuffer[output + 1] = normal.y;
		normalbuffer[output + 2] = normal.z;
		normalbuffer[output + 3] = normal.x;
		normalbuffer[output + 4] = normal.y;
		normalbuffer[output + 5] = normal.z;
		normalbuffer[output + 6] = normal.x;
		normalbuffer[output + 7] = normal.y;
		normalbuffer[output + 8] = normal.z;
		// stride of the output, 3x3 = 9 same as input as it happens!!
		output += 9;

	}
}



int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Lab 2 - Textured Spinning Lit Cube", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	double currentTime = glfwGetTime();
	double lastTime = currentTime;

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" (model * view * projection) uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// get the "model" matrix which is eventually part of our MVP so we can use it in shaders for calculating worldspace positions
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin) you will want to play with this to move the
	// model around!! try different things with it!
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

   // Load the texture here
	GLuint Texture = SOIL_load_OGL_texture("..//3dcontent//textures//texture_diffuse1.jpg", SOIL_LOAD_RGB,SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);

	// Get a handle for our texture sampler uniform 
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	// get a handle for the light position uniform
	GLuint LightPosID = glGetUniformLocation(programID, "LightPositionWorldSpace");
	// get a handle for the light direction uniform
	GLuint LightDirID = glGetUniformLocation(programID, "LightDirectionWorldSpace");

	// generate a vertex array object ID and bind it as the current VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	// note we don't want this const as we're going to calculate it!!
	static float g_normal_buffer_data[36 * 3];

	// calculate the normals per face and store them in vertex order (one normal per vertex)
	CalculateNormals(g_vertex_buffer_data, g_normal_buffer_data);

	// Two UV coordinates for each vertex. They were created with Blender. <- from opengl-tutorials.com
	// incidentally these UV's are kind of dumb :)
	static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};

	// generate a vertex buffer object
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	// bind it to the currently active vertex array object (VertexArrayID defined above)
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// and fill it with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// now do the same for UV coordinates
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	// now do the same for normals
	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : Normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 2, but must match the layout in the shader.
		3,                                // size : Normal = 3 floats
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// counter for elapsed time.. calculate the time delta each update and increase this value!
	float elapsed = 0.0f;
	float deltaTime = 0.0f;	// temp, you want to calculate this inside the Do..While loop!
	// our light position (might be useful to animate this to view your lighting?)
	glm::vec3 LightPos(4.0f, 3.0f, 3.0f);
	glm::vec3 LightDir(0.0f, -1.0f, 0.0f);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		// set our "M" (lazy name for "Model") uniform
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);

		
		// make light direction always point to origin
		LightDir = glm::normalize(LightPos - glm::vec3(0.0f, 0.0f, 0.0f));
		
		// bind our light position uniform
		glUniform3f(LightDirID, LightDir.x, LightDir.y, LightDir.z);
		// and our light position uniform
		glUniform3f(LightPosID, LightPos.x, LightPos.y, LightPos.z);

		// Bind our texture in Texture Unit 0 (glActiveTexture)

		// Bind the texture (glBindTexture) using the handle of our texture loaded via SOIL

		// Set our sampler uniform in the shader to use Texture Unit 0 (glUniform1i)


		// remember that OpenGL is stateful, so if we call glDrawArrays now, it will try and draw with the currently bound VAO data
		// which means it will use our vertex array + vertex buffers + vertex attributes

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// calculate time delta and rotate cube by that delta here... this will then be drawn in the next update
		// note: we could do this at the start of the do loop.. works either way as long as the Model matrix is changed
		// every update and then written into the MVP matrix (which then gets set for our MVP uniform)

		// call the function to get the new rotation (by filling in the Model matrix
		RotateCube(Model, deltaTime);
		// recompute the MVP matrix so the object rotates
		MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
