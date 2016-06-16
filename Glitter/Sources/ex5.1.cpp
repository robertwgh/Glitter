// Chapter 5, ex5.1
// Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data

#include "learngl.hpp"

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void showFPS(GLFWwindow * window, double & lastTime, int & frame)
{
    double currentTime = glfwGetTime();
    double time = currentTime - lastTime;
    frame++;
    if (time >= 0.5)
    {
        std::stringstream ss;
        ss << "Robert learns OpenGL.  FPS " << 1.0f / time * frame;
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
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Glad library init.
    if (!gladLoadGL()) {
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
    void main()
    {
        gl_Position = vec4(position.x, position.y, position.z, 1.0f);
    }
);

const GLchar * fsSource = SHADER_SOURCE(
    out vec4 color;
    void main()
    {
        color = vec4(1.0f, 0.5f, 0.2f, 0.1f);
    }
);

int main()
{
    GLFWwindow * window = initTest(800, 600);

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsSource, NULL);
    glCompileShader(vertexShader);
    GLint err;
    GLchar errLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errLog);
        std::cout << "Vertex shader compile failed. Error log: " << errLog << std::endl;
        return -1;
    }

    // Compiler fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errLog);
        std::cout << "Fragment shader compile filed. Error log: " << errLog << std::endl;
        return -1;
    }

    // Create program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &err);
    if (!err)
    {
        glGetProgramInfoLog(program, 512, NULL, errLog);
        std::cout << "Link program failed, error log: " << errLog << std::endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Prepare data
    GLfloat vertices[] = {
        -0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -0.25f, 0.25f, 0.0f,
        0.0f, -0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.25f, 0.25f, 0.0f
    };

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLint posAttribPtrLoc = glGetAttribLocation(program, "position");
        glVertexAttribPointer(posAttribPtrLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttribPtrLoc);
    }
    glBindVertexArray(0);

    glUseProgram(program);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Create a game loop.
    double lastTime = glfwGetTime();
    int frame = 0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Rendering
        glClearColor(0.80f, 0.80f, 0.80f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(GLfloat)/3);
        glBindVertexArray(0);

        // Finish rendering
        glfwSwapBuffers(window);
        showFPS(window, lastTime, frame);
    }

    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}
