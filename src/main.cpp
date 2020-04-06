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
#include <glm/gtx/string_cast.hpp>

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

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window;

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 05 - Textured Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/stdvert.vert", "shaders/stdfrag.frag" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// Load the texture using any two methods
	//GLuint Texture = loadBMP_custom("uvtemplate.bmp");
	GLuint Texture = loadPNG("grass.png");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "tex2d");

	vector<Obj*> objects (0);
	Obj* cube = createCube();
	objects.push_back(cube);

	foreach (Obj*, obj, objects)
		cout << "Loading " << (*obj)->name << endl;
		loadObj(**obj);
	}

	do{
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		npforeach(Obj*, obj, objects)
			//cout << obj->position.x << ", " << obj->position.y << ", " << obj->position.z << endl;
			// Model matrix : an identity matrix (model will be at the origin)
			glm::mat4 Model = glm::translate(glm::mat4(1.0f), obj->position);
			// Our ModelViewProjection : multiplication of our 3 matrices
			glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glBindVertexArray(obj->VAO);

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBuffer);
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
			glBindBuffer(GL_ARRAY_BUFFER, obj->uvBuffer);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				2,                                // size : U+V => 2
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, obj->getVertSize()); // 12*3 indices starting at 0 -> 12 triangles

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

		}
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	npforeach (Obj*, obj, objects)
		glDeleteBuffers(1, &obj->vertexBuffer);
		glDeleteBuffers(1, &obj->uvBuffer);
		glDeleteVertexArrays(1, &obj->VAO);
		delete obj;
	}

	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
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