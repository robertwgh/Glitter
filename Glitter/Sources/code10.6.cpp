// Chapter 10
// Experiment with LookAt matrix and camera position. 
// use keyboard to control the camera movement.
// w: up, s down; a, left; d right;
// Using delta time to control the camera speed.
// Add mouse movement control.

#include <string>
#include "learngl.hpp"
#include "learngl_util.hpp"

#define NUM_CUBE    3000
#define WIDTH 2560
#define HEIGHT 1440
#define FULL_SCREEN 0

bool keys[1024];
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
double lastX, lastY;
float yaw = -90.0f; // if yaw ==0, the camera front is towards x axis. natually, we want the camera points to the negtive Z axis, so we set it to -90 degree.
float pitch = 0.0f;
float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GL_TRUE);
        
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    double xoffset = xpos - lastX;
    double yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    
    // process mouse event;
    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
        
    yaw += xoffset;
    pitch -= yoffset; // Here I use the different setting as in the tutorial. since I think this is more like in a game, the mouse movement is opposite to the look at direction.
    
    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 89.0f)
        fov -= yoffset;
    
    if(fov <= 1.0f) fov = 1.0f;
    if(fov >= 89.0f) fov = 89.0f;
}


// Camera control function.
void do_movement()
{         
    // process keyboard event;
    float time = glfwGetTime();
    deltaTime = time - lastFrame;
    lastFrame = time;
    float cameraSpeed = 5.0f * deltaTime;
    
    if(keys[GLFW_KEY_W])
        //*cameraPos += *cameraUp * cameraSpeed;
        cameraPos += cameraSpeed * cameraFront;
    
    if(keys[GLFW_KEY_S])
        //*cameraPos -= *cameraUp * cameraSpeed;
        cameraPos -= cameraSpeed * cameraFront;
    
    if(keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if(keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        
        
    //std::cout << " fov=" << fov << ", cameraPos=("  << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")"
    //<< ", cameraFront=("  << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")"
    //<< ", cameraUp=("  << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << ")" << std::endl;
}


void window_size_callback(GLFWwindow* window, int width, int height)
{
    glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);
}


void showFPS(GLFWwindow * window, double & lastTime, int & frame)
{
    double currentTime = glfwGetTime();
    double time = currentTime - lastTime;
    frame ++;
    if(time >= 0.5)
    {
        std::stringstream ss;
        ss << "Robert learns OpenGL.  FPS "<< 1.0f / time * frame;
        frame = 0;
        lastTime = currentTime;
        glfwSetWindowTitle(window, ss.str().c_str());
    }
}

GLFWwindow * initTest(int width, int height)
{
    // Create OpenGL window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow * window;
    if(FULL_SCREEN)
        window = glfwCreateWindow(width, height, "Robert learns OpenGL", glfwGetPrimaryMonitor(), nullptr);
    else
        window = glfwCreateWindow(width, height, "Robert learns OpenGL", nullptr, nullptr);
    
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glfwSetScrollCallback(window, scroll_callback);
    
    // Glad library init.
    if(!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return nullptr;
    }
    
    // Print OpenGL version.
    std::cout << "OpenGL version " << GLVersion.major << "." << GLVersion.minor << std::endl;
    // Set Viewport.
    glViewport(0, 0, width, height);
    
    return window;
}

const GLchar * vsSource = SHADER_SOURCE(
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 color;
    layout(location = 2) in vec2 texCoord;
    
    out vec3 myColor;
    out vec2 myTexCoord;
    
    uniform mat4 transform;
    
    void main()
    {
        gl_Position = transform * vec4(position.xyz, 1.0f);
        myColor = color;
        myTexCoord = texCoord;
    }
);

const GLchar * fsSource = SHADER_SOURCE(
    in vec3 myColor;
    in vec2 myTexCoord;
    out vec4 color;
    uniform sampler2D sampler;
    void main()
    {
        // Solve the image up-side down issue.
        color = texture(sampler, vec2(myTexCoord.x, 1.0f - myTexCoord.y));// * vec4(myColor, 0.5f);
    }
);
 
int main()
{
    int width = WIDTH, height = HEIGHT;
    GLFWwindow * window = initTest(width, height);
    ogl::Program program(vsSource, fsSource, ogl::Program::STRING_MODE);
    
    // Prepare texture data
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int imgw, imgh;
    std::string file_path = get_exe_dir() + "/challenger.jpg";
    unsigned char * image = SOIL_load_image(file_path.c_str(), &imgw, &imgh, 0, SOIL_LOAD_RGB);
    if(image == NULL) { std::cout << "Failed to load image from " << file_path << std::endl; return -1; }
    std::cout << "Image loaded, size: " << imgw << " X " << imgh << std::endl;
    
    // Transfer texture data to texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgw, imgh, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    // This is my cube.
    /* 
     *   _________
     *  /________/|
     * |        | |
     * |        | |
     * |        | |
     * |________|/  
     * 
     */
     
    //Prepare data    
    GLfloat vertices[] = {
        // front plane
        0.5f, 0.5f, 0.5f,       1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 1.0f,
        // back plane
        0.5f, 0.5f, -0.5f,      1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 0.0f,
        // top
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        // bottom
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
        // left
        -0.5f, 0.5f, 0.5f,      1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,      1.0f, 1.0f,
        
        // right
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f
    };
    
    // define the position and rotation speed for each cube.
    glm::vec3 cube_pos[NUM_CUBE];
    float cube_rot[NUM_CUBE];
    for(int i = 0; i < NUM_CUBE; i ++)
    {
        cube_pos[i] = glm::vec3((float)std::rand()/RAND_MAX * 3.0f - 1.5f, (float)std::rand()/RAND_MAX * 3.0f - 1.5f, (float)std::rand()/RAND_MAX*10.0f - 10.0f);
        cube_rot[i] = ((float)std::rand()/RAND_MAX - 0.5f) * 5.0f; // Rotation speed.
    }
    
    // Define VBO, EBO and VAO.
    GLuint vbo, ebo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    GLuint vbo_tex_coords;
    glGenBuffers(1, &vbo_tex_coords);
    
    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        GLint posAttribPtrLoc = program.getAttribLocation("position"); 
        glVertexAttribPointer(posAttribPtrLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttribPtrLoc);
        
        GLint texAttribPtrLoc = program.getAttribLocation("texCoord");
        glVertexAttribPointer(texAttribPtrLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texAttribPtrLoc);
    }
    glBindVertexArray(0);
    
    program.use();
    
    // Wireframe mode.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Create a game loop.
    double lastTime = glfwGetTime();
    int frame = 0;
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        do_movement();
        
        // Rendering
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.80f, 0.80f, 0.80f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        
        for(GLuint i = 0; i < NUM_CUBE; i ++)
        {
            glm::mat4 model, view, projection;
            model = glm::rotate(model, glm::radians(10.0f * i), glm::vec3(0.5f, 0.6f, 0.4f));
            float scale = .5f;
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            model = glm::translate(model, cube_pos[i]);
            model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f * cube_rot[i], glm::vec3(1.0f, 0.8f, 0.5f));
            
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
            projection = glm::perspective(glm::radians(fov), (float)width/(float)height, 0.1f, 100.0f);
            glm::mat4 transform = projection * view * model;
            GLuint transformLoc = program.getUniformLocation("transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            
            //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Finish rendering
        glfwSwapBuffers(window);
        showFPS(window, lastTime, frame);
    }
    
    glDeleteBuffers(1, &vbo); 
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &texture);
    glfwTerminate();
    return 0;
}
