// Chapter 6, exercise 6.3
// Output the vertex position to the fragment shader using the out keyword and 
// set the fragment's color equal to this vertex position (see how even the 
// vertex position values are interpolated across the triangle). Once you 
// managed to do this; try to answer the following question: 
// why is the bottom-left side of our triangle black

#include "learngl.hpp"

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GL_TRUE);
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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow * window = glfwCreateWindow(width, height, "Robert learns OpenGL", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
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

const GLchar * vsSource = STRINGIZE_SOURCE(
    \#version 330 core \n
    layout (location = 0) in vec3 position;
    out vec3 myColor;
    void main()
    {
        gl_Position = vec4(position.xyz, 1.0f);
        //myColor = position.xyz; // Initial version.
        myColor = vec3(position.x + 0.5f, position.y + 0.5f, position.z); // Improved version, make it prettier.
    }
);

const GLchar * fsSource = STRINGIZE_SOURCE(
    \#version 330 core \n
    out vec4 color;
    in vec3 myColor;
    void main()
    {
        color = vec4(myColor, 1.0f);
    }
);
 
int main()
{
    GLFWwindow * window = initTest(800, 600);
    ogl::Program program(vsSource, fsSource, ogl::Program::STRING_MODE);
 
    //Prepare data
    GLfloat triangle[] = {
        -0.5f, -0.5f, 0.0f, /* coordinate*/ 1.0f, 0.0f, 0.0f, /* color */
        0.5f, -0.5f, 0.0f,                  0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,                   0.0f, 0.0f, 1.0f
    };
    
    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
 
    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
        
        GLint posAttribPtrLoc = program.getAttribLocation("position"); //glGetAttribLocation(program, "position");
        glVertexAttribPointer(posAttribPtrLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttribPtrLoc);
    }
    glBindVertexArray(0);
    
    program.use();
    
    // Create a game loop.
    double lastTime = glfwGetTime();
    int frame = 0;
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        // Rendering
        glClearColor(0.80f, 0.80f, 0.80f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
              
        // Finish rendering
        glfwSwapBuffers(window);
        showFPS(window, lastTime, frame);
    }
    
    glDeleteBuffers(1, &vbo); 
    glfwTerminate();
    return 0;
}
