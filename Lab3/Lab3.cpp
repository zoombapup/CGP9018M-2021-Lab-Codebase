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
	window = glfwCreateWindow( 1024, 768, "Phong Lighting", NULL, NULL);
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
	Model sphereModel("../3dcontent/models/mat_scaled/mat_scaled.obj");
	check_gl_error();
	//sphereModel.GetMesh(0)->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera is at (4,3,3), in World Space
	glm::vec3 cameraPosition(2, 2, 1);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		cameraPosition, 
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::vec3 lightPosition(6, 5, 3);
	glm::vec3 lightColour(1, 1, 1);
	float ambientIntensity = 0.1f;
	float specularIntensity = 0.5f;
	float specularPower = 32.0;

	double currentTime = glfwGetTime();
	double lastTime = currentTime;

	glm::quat rotquat(glm::vec3(0.0f,glm::radians(0.2f), 0.0f));

	float LightMoveSpeed = 0.3f;
	float CameraMoveSpeed = 0.001f;

	do{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// Compute time difference between current and last frame
		currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// key handlers
		if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) { lightPosition.y += deltaTime * LightMoveSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) { lightPosition.y -= deltaTime * LightMoveSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) { lightPosition.x += deltaTime * LightMoveSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) { lightPosition.x -= deltaTime * LightMoveSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) { lightPosition.z += deltaTime * LightMoveSpeed; }
		if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) { lightPosition.z -= deltaTime * LightMoveSpeed; }

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { cameraPosition.y += deltaTime * CameraMoveSpeed; }
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { cameraPosition.y -= deltaTime * CameraMoveSpeed; }
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { cameraPosition.x -= deltaTime * CameraMoveSpeed; }
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { cameraPosition.x += deltaTime * CameraMoveSpeed; }
		//if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) { cameraPosition.z += deltaTime * LightMoveSpeed; }
		//if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) { cameraPosition.z -= deltaTime * LightMoveSpeed; }


		if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) { specularIntensity += (deltaTime * 0.002f); }
		if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) { specularIntensity -= (deltaTime * 0.002f); }
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) { specularPower += (deltaTime * 0.002f); }
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { specularPower -= (deltaTime * 0.002f); }
		// update view matrix with new camera position
		View = glm::lookAt(cameraPosition,glm::vec3(0, 0.5f, 0),glm::vec3(0, 1, 0));

		// binds the shader as the current one in use
		StandardShader.use();

		StandardShader.setFloat("ambientIntensity", ambientIntensity);
		StandardShader.setFloat("specularIntensity", specularIntensity);
		StandardShader.setVec3("lightPosition", lightPosition);
		StandardShader.setVec3("lightColour", lightColour);
		StandardShader.setVec3("cameraPosition", cameraPosition);
		StandardShader.setFloat("specularPower", specularPower);

		// Draw your model here! note: we pass in View and Projection matrices, but Model matrix is held within mesh class inside Model class
		sphereModel.Render(View, Projection, StandardShader);

		glm::quat rot = sphereModel.GetMesh(0)->GetRotation();
		sphereModel.GetMesh(0)->SetRotation(rot * rotquat);
		sphereModel.GetMesh(1)->SetRotation(rot * rotquat);
		sphereModel.GetMesh(2)->SetRotation(rot * rotquat);

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

