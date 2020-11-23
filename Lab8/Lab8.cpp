// THIS LAB STARTS WITH CODE FROM TUTORIAL 1 FROM OPENGL-TUTORIAL.ORG CODE
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
GLFWwindow* window;



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
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

	// setup IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();




	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	// define your triangle data here.. don't be scared to experiment at this point
	// triangle has 3 floats per vertex and 3 vertices = 9 floats!! note: winding order is important!!
	static const GLfloat g_vertex_buffer_data[] = 
	{
		 1.0f,-1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};

	float g_colour_buffer_data[] =
	{
		// colour is one float each for RGBA
		1.0f,0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f
	};

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
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);	// sets this vertex array as the "current" one in OpenGL state machine
	
	// create a vertex buffer object id and fill it with our data
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	GLuint colourbuffer;
	glGenBuffers(1, &colourbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_colour_buffer_data), g_colour_buffer_data, GL_STATIC_DRAW);

	// 2nd attribute buffer : colours
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("Lab1VertexShader.vertexshader", "Lab1FragmentShader.fragmentshader");

	// Get a handle for our "MVP" (model * view * projection) uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 6), // Camera is at (0,0,6), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	float RotX = 0.0f;
	float RotY = 0.0f;
	float RotZ = 0.0f;
	float RotSpeed = 1.0f;
	
	double currentTime = glfwGetTime();
	double lastTime = currentTime;

	do{
		glClear(GL_COLOR_BUFFER_BIT);

		// calculate time delta and rotate triangle by that delta here... 
		currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		// key handlers
		if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) { RotY += deltaTime * RotSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) { RotY -= deltaTime * RotSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) { RotX += deltaTime * RotSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) { RotX -= deltaTime * RotSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) { RotZ += deltaTime * RotSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) { RotZ -= deltaTime * RotSpeed; }

		// Draw your coloured triangle here! 
		// Use our shader
		glUseProgram(programID);

		// Model matrix : changed over time by rotation euler angles
		glm::mat4 Model = glm::mat4_cast(glm::quat(glm::vec3(RotX,RotY,RotZ)));
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 1 triangle

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Text("Outside of Window");
		ImGui::ColorEdit4("VertexColour 1: ", &g_colour_buffer_data[0], ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("VertexColour 2: ", &g_colour_buffer_data[4], ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("VertexColour 3: ", &g_colour_buffer_data[8], ImGuiColorEditFlags_Float);
		
		glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_colour_buffer_data), g_colour_buffer_data, GL_STATIC_DRAW);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		


	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

