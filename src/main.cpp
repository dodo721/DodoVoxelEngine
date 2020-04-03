#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>

#include <obj.hpp>
#include <cube.hpp>
#include <macros.hpp>
#include <shader.hpp>

//#include <GL/glew.h>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>

using namespace glm;
using namespace std;
using namespace obj3D;

void loadObj (Obj& obj);
GLuint loadPNG (string imagepath);

int main () {

	// Initialise GLFW
	//glewExperimental = true; // Needed for core profile
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow( 1920, 1080, "Dodo Voxel Engine", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	//glewExperimental=true; // Needed in core profile
	/*if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	
	// Dark blue background	
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/stdvert.vert", "shaders/stdfrag.frag" );

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint TextureID  = glGetUniformLocation(programID, "tex2d");
	
	float posX = 0;

	vector<Obj> objects (0);

	double lastTime;
	float horizontalAngle = 0;
	float verticalAngle = 0;
	float mouseSpeed = 0.1;
	vec3 position (10, 10, 10);

	char voxelData[10][10];

	srand( (unsigned)time(NULL) );

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			int add = (int)std::round(rand() % 2);
			cout << add << " ";
			if (add > 0.5) {
				Obj cube = createCube(vec3(i * 2,j * 2, 0));
				objects.push_back(cube);
			}
		}
		cout << endl;
	}

	foreach(Obj, obj, objects)
		cout << "Loading object " << obj->name << endl;
		loadObj(*obj);
		//cout << "Loaded " << obj->name << " at position " << obj->position.x << "," << obj->position.y << "," << obj->position.z << ", vert " << obj->vertexBuffer << ", uv " << obj->uvBuffer << endl;
	}

	do{
		
		// Delta time
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;
		// Window size
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);

		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		// Reset mouse position for next frame
		glfwSetCursorPos(window, width/2, height/2);

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(width/2 - xpos ) * deltaTime;
		verticalAngle   += mouseSpeed * float(height/2 - ypos ) * deltaTime;

		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f/2.0f),
			0,
			cos(horizontalAngle - 3.14f/2.0f)
		);
		glm::vec3 up = glm::cross( right, direction );

		// Camera matrix (view)
		glm::mat4 View       = glm::lookAt(
			position,           // Camera is here
			position+direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

		foreach(Obj, obj, objects)
			// Model matrix (translations)
			glm::mat4 Model = glm::translate(glm::mat4(), obj->position);
			if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS)
				obj->position.x += 0.005;
			if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS)
				obj->position.x -= 0.005;
			// Our ModelViewProjection : multiplication of our 3 matrices
			glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

			// Feed in MVP matrix
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			
			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, obj->texid);
			// Set our "myTextureSampler" sampler to use Texture Unit 0
			glUniform1i(TextureID, 0);
			
			glBindVertexArray(obj->VAO);
			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, obj->getVertSize() / 3);
			glBindVertexArray(0);
			/*glDrawElements(
				GL_TRIANGLES,      // mode
				obj->getIndicesSize(),    // count
				GL_UNSIGNED_INT,   // type
				(void*)0           // element array buffer offset
			);*/

		}
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 );

	foreach(Obj, obj, objects)
		// Cleanup VBO and shader
		glBindVertexArray(obj->VAO);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
		glDeleteBuffers(1, &obj->vertexBuffer);
		glDeleteBuffers(1, &obj->uvBuffer);
		glDeleteTextures(1, &obj->texid);
		glDeleteVertexArrays(1, &obj->VAO);
	}

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

}

void loadObj (Obj& obj) {

	glGenVertexArrays(1, &obj.VAO);
	glBindVertexArray(obj.VAO);

	glGenBuffers(1, &obj.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, obj.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, obj.getVertSize() * sizeof(obj.getVerts()[0]), obj.getVerts(), GL_STATIC_DRAW);

	glGenBuffers(1, &obj.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, obj.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, obj.getUVSize() * sizeof(obj.getUVs()[0]), obj.getUVs(), GL_STATIC_DRAW);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, obj.vertexBuffer);
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
	glBindBuffer(GL_ARRAY_BUFFER, obj.uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindVertexArray(0);
	//std::vector<unsigned int> indices;

	// fill "indices" as needed

	// Generate a buffer for the indices
	/*glGenBuffers(1, &obj.indiceBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.indiceBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getIndicesSize() * sizeof(unsigned int), obj.getIndices(), GL_STATIC_DRAW);*/

	try {
		obj.texid = loadPNG(obj.texpath);
	} catch (const char* e) {
		cout << "Error: " << e << endl;
		throw;
	}
}

GLuint loadPNG(string imagename){

	string texDir = "res/textures/";
	string imagepath = texDir + imagename;

    int w;
	int h;
	int comp;
	unsigned char* image = stbi_load(imagepath.c_str(), &w, &h, &comp, STBI_rgb);

	if(image == nullptr)
		throw("Failed to load texture");
	
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, image);

	// Nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}