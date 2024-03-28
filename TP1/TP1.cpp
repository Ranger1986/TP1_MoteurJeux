// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;

#include <common/objloader.hpp>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/vboindexer.hpp>

#include "src/Plan.h"
#include "src/Scene.h"
#include "src/Transform.h"

void processInput(GLFWwindow* window);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int vertices_cote = 16;

// camera
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
bool camlibre = true;
bool camfixe = false;
bool camorbitale = false;
// timing
float deltaTime = 0.0f;  // time between current frame and last frame
float lastFrame = 0.0f;

// rotation
float angle = 0.;
float angle_coef = 1.;
float zoom = 1.;

/*******************************************************************************/
// declaration des matrices composante de MVP
mat4 modelMatrix;
mat4 viewMatrix;
mat4 projectionMatrix;
// donnees 3d
vector<unsigned short> indices;
vector<vec3> indexed_vertices;
vector<vec3> indexed_color;
vector<float> texCoords;
// buffers
GLuint vertexbuffer;
GLuint elementbuffer;
GLuint uv;
GLuint colorbuffer;

int main(void) {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "TP - GLFW", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;  // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    // glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");

    // Get a handle for our "Model View Projection" matrices uniforms
    modelMatrix = mat4(1.f);
    viewMatrix = mat4(1.f);
    projectionMatrix = mat4(1.f);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    //begin create scene
    Scene* solarSystem = new Scene;
    std::string filename("sphere.off");

    Scene* sun = new Scene;
    sun->transform.rotz(23.44);
    loadOFF(filename, sun->indexed_vertices, sun->indices);
    sun->color=vec3(1.f,1.f,0.f);

    Scene* earth_node = new Scene;
    earth_node->transform.scale(0.5f);
    earth_node->transform.translate(vec3(5.0,0.0,0.0));
    earth_node->transform.rotz(5.14f);
    Scene* earth_mesh = new Scene;
    loadOFF(filename, earth_mesh->indexed_vertices, earth_mesh->indices);
    earth_mesh->color=vec3(0.2,0.2,0.8);

    Scene* moon_node = new Scene;
    moon_node->transform.scale(static_cast<float>(1.738/6.378));
    moon_node->transform.translate(vec3(3.0,0.0,0.0));
    moon_node->transform.rotz(6.68f);
    Scene* moon_mesh = new Scene;
    loadOFF(filename, moon_mesh->indexed_vertices, moon_mesh->indices);
    moon_mesh->color=vec3(0.5,0.5,0.5);

    solarSystem->addChild(sun);
    sun->addChild(earth_node);
    earth_node->addChild(earth_mesh);
    earth_node->addChild(moon_node);
    moon_node->addChild(moon_mesh);
    //end create scene
    do {
        indexed_vertices.clear();
        indices.clear();
        texCoords.clear();
        indexed_color.clear();
        // Plan
        sun->transform.roty(360.f/365/2);
        earth_node->transform.roty(360.f/24/2);
        moon_node->transform.roty(1);
        indexed_vertices = solarSystem->getVertices();
        indexed_color = solarSystem->getColor();
        indices = solarSystem->getIndices();
        texCoords = solarSystem->getTexCoords();
        // Load it into a VBO
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        // Generate a buffer for the indices as well
        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &uv);
        glBindBuffer(GL_ARRAY_BUFFER, uv);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(unsigned short), &texCoords[0], GL_STATIC_DRAW);
        //std::cout << indexed_color[0][0] << std::endl;
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_color.size() * sizeof(glm::vec3), &indexed_color[0], GL_STATIC_DRAW);
        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        viewMatrix = lookAt(camera_position, camera_target, camera_up);
        projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        mat4 tempViewMat = rotate(viewMatrix, radians(45.0f), vec3(1.0, 0.0, 0.0));
        mat4 MVP;
        if (camorbitale) {
            tempViewMat = rotate(tempViewMat, radians(angle / 10), camera_up);
            angle += angle_coef;
            MVP = projectionMatrix * tempViewMat * modelMatrix;
        } else {
            MVP = projectionMatrix * viewMatrix * modelMatrix;
        }
        GLuint MVPlocation = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, &MVP[0][0]);
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,         // attribute
            3,         // size
            GL_FLOAT,  // type
            GL_FALSE,  // normalized?
            0,         // stride
            (void*)0   // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uv);
        glVertexAttribPointer(
            1,         // attribute
            2,         // size
            GL_FLOAT,  // type
            GL_FALSE,  // normalized?
            0,         // stride
            (void*)0   // array buffer offset
        );
        
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            2,         // attribute
            3,         // size
            GL_FLOAT,  // type
            GL_FALSE,  // normalized?
            0,         // stride
            (void*)0   // array buffer offset
        );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glDrawElements(
            GL_TRIANGLES,       // mode
            indices.size(),     // count
            GL_UNSIGNED_SHORT,  // type
            (void*)0            // element array buffer offset
        );

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }  // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteBuffers(1, &colorbuffer);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Vitesse de déplacement de la caméra
    float cameraSpeed = 2.5f * deltaTime;
    /*
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        camlibre = true;
        camfixe = false;
        camorbitale = false;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        camlibre = false;
        camfixe = true;
        camorbitale = false;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camlibre = false;
        camfixe = false;
        camorbitale = true;
    }
    */
    if (camlibre) {
        // Déplacer la caméra vers l'avant
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera_position -= cameraSpeed * normalize(camera_position - camera_target);
        }

        // Déplacer la caméra vers l'arrière
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera_position += cameraSpeed * normalize(camera_position - camera_target);
        }

        // Déplacer la caméra vers la droite
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_position += vec3{1, 0, 0} * cameraSpeed;
            camera_target += vec3{1, 0, 0} * cameraSpeed;
        }

        // Déplacer la caméra vers la gauche
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_position -= vec3{1, 0, 0} * cameraSpeed;
            camera_target -= vec3{1, 0, 0} * cameraSpeed;
        }

        // Déplacer la caméra vers le haut
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_position += vec3{0, 1, 0} * cameraSpeed;
            camera_target += vec3{0, 1, 0} * cameraSpeed;
        }

        // Déplacer la caméra vers le bas
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_position -= vec3{0, 1, 0} * cameraSpeed;
            camera_target -= vec3{0, 1, 0} * cameraSpeed;
        }
    }
    if (camorbitale) {
        // Déplacer la caméra vers l'avant
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera_position += cameraSpeed * camera_front;
        }

        // Déplacer la caméra vers l'arrière
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera_position -= cameraSpeed * camera_front;
        }
        // Déplacer la caméra vers l'avant
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            if (angle_coef > 0) {
                angle_coef -= 1;
            }
        }

        // Déplacer la caméra vers l'arrière
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            angle_coef += 1;
        }
    }
    // Déplacer la caméra vers l'avant
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        if (vertices_cote > 0) {
            vertices_cote -= 1;
        }
    }

    // Déplacer la caméra vers l'arrière
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        vertices_cote += 1;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}