// THIS LAB STARTS WITH CODE FROM TUTORIAL 1 FROM OPENGL-TUTORIAL.ORG CODE

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	// for value_ptr used in lights array uniform
using namespace glm;

#include "common/model.h"
#include "common/shader.h"
#include "common/GLError.h"
#include "common/skybox.h"
#include "common/controls.hpp"

void GLAPIENTRY
MessageCallback(GLenum source,
				 GLenum type,
				 GLuint id,
				 GLenum severity,
				 GLsizei length,
				 const GLchar* message,
				 const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			  type, severity, message);
}

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
	window = glfwCreateWindow( 1024, 768, "Lab4 - Phong with specular+ambient occlusion maps", NULL, NULL);
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

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	check_gl_error();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	check_gl_error();

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	check_gl_error();
	
	// shader for mat (which has ambient occlusion texture in the ambient channel that is multiplied by diffuse colour texture)
	// and specularity colour map
	Shader StandardShaderWithAO("StandardShadingWithAOSpecmaps.vertexshader", "StandardShadingWithAOSpecmaps.fragmentshader");
	check_gl_error();

	// shader for our standard models with 1 diffuse texture
	Shader StandardShader("StandardShading.vertexshader", "StandardShading.fragmentshader");
	check_gl_error();

	// used to draw a full white light sphere!
	Shader lightShader("LightShading.vertexshader", "LightShading.fragmentshader");
	check_gl_error();

	// used by the skybox, basically uses cubemap texture to draw the correct albedo (diffuse colour)
	Shader skyboxShader("skybox.vertexshader", "skybox.fragmentshader");
	check_gl_error();

	//Skybox sky("xp.jpg", "xn.jpg", "yp.jpg", "yn.jpg", "zp.jpg", "zn.jpg");
	Skybox sky("../3dcontent/skybox/tropicalsunnyday/","TropicalSunnyDayRight.png", "TropicalSunnyDayLeft.png", "TropicalSunnyDayUp.png", "TropicalSunnyDayDown.png", "TropicalSunnyDayBack.png", "TropicalSunnyDayFront.png");

	Model matModel("../3dcontent/models/mat_scaled/mat_scaled.obj");

	// left hand guy with no spec+AO for comparison
	Model matModel2("../3dcontent/models/mat_scaled/mat_scaled.obj");
	matModel2.SetPosition(glm::vec3(-1, 0, 0));

	// the "ground" mesh, in this case a cylinder with a slight tapered base
	Model baseModel("../3dcontent/models/base/base.obj");

	// basically a sphere, but with a shader to make it entirely white
	Model lightModel("../3dcontent/models/sphere/sphere.obj");
	// naughty, should check if getmesh returns null!!!
	lightModel.GetMesh(0)->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	check_gl_error();

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 7, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	check_gl_error();

	glm::vec4 light(0, 0, 0, 25);
	glm::vec3 lightColour(1, 1, 1);

	// how much time has accumulaed?
	static float timeElapsed = 0.0f;

	double currentTime = glfwGetTime();
	double lastTime = currentTime;

	bool IsAnimatingTime = true;

	bool bShowNormalMap = false;

	do{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
		// Compute time difference between current and last frame
		currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		if(IsAnimatingTime)
			timeElapsed += deltaTime;
		// update light position
		float lightdistance = 4.0f;
		light.y = lightdistance * cosf(timeElapsed);
		light.z = lightdistance * sinf(timeElapsed);

		glm::vec3 lightpos(light);
		lightModel.GetMesh(0)->SetPosition(lightpos);
		//check_gl_error();
		computeMatricesFromInputs(deltaTime);
		Projection = getProjectionMatrix();
		View = getViewMatrix();

		// skybox code causes skyboxShader to be bound.. might want to leave this as another step?
		// skybox always comes first, so other models can overwrite colour buffer
		sky.Draw(View,Projection, skyboxShader);

		// draw the "fancy" mat first, he's got AO!
		StandardShaderWithAO.use();
		StandardShaderWithAO.setFloat("ambientlevel", 0.2f);
		StandardShaderWithAO.setVec3("lightColour", lightColour);
		StandardShaderWithAO.setVec4("light", light);
		matModel.Render(View, Projection, StandardShaderWithAO);
	

		// now bind and use our "Standard phong lighting shader for both "normal" mat and the base mesh
		StandardShader.use();
		// global ambient light level for everything rendered with this shader
		StandardShader.setFloat("ambientlevel", 0.2f);
		StandardShader.setVec3("lightColour", lightColour);
		StandardShader.setVec4("light", light);

		matModel2.Render(View, Projection, StandardShader);

		// Draw your model here!
		baseModel.Render(View, Projection, StandardShader);

		// using a sphere for a light so we can vizualize the light position
		// it has a custom shader that basically emits solid white
		lightShader.use();
		lightModel.Render(View, Projection, lightShader);

		// Swap buffers
		glfwSwapBuffers(window);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;

		glfwPollEvents();

		// FIXME - this could be a lot better to control, think about how to improve it! e.g. orbit some position with light?
		// toggle the status of time animation
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			IsAnimatingTime = !IsAnimatingTime;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
		{
			IsAnimatingTime = true;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
		{
			IsAnimatingTime = false;
		}

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		{
			bShowNormalMap = true;
		}

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			bShowNormalMap = false;
		}


	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

