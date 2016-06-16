// Chapter 7
// Element buffer objects
// Texture. Start to learn to use texture.

#include "learngl.hpp"

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GL_TRUE);
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
    
    GLFWwindow * window = glfwCreateWindow(width, height, "Robert learns OpenGL", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    
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
        color = texture(sampler, vec2(myTexCoord.x, 1.0f - myTexCoord.y)) * vec4(myColor, 0.5f);
    }
);
 
int main()
{
    GLFWwindow * window = initTest(800, 600);
    ogl::Program program(vsSource, fsSource, ogl::Program::STRING_MODE);
    
    // Prepare texture data
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height;
    unsigned char * image = SOIL_load_image("/home/kn/workspace/robertwgh/Glitter/image/challenger.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if(image == NULL) { std::cout << "Failed to load image" << std::endl; return -1; }
    std::cout << "Image loaded, size: " << width << " X " << height << std::endl;
    
    // Transfer texture data to texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //Prepare data
    /*
    GLfloat vertices[] = {
        // position             colors          tex coords
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   .01f, .01f,// top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   .01f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,// bottom left
        -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, .01f  // top left
    };
    * */
    
    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,// top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,// bottom left
        -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    
    GLuint indices [] = {
        0, 1, 2, 
        2, 3, 0
    };
    

    // Define VBO, EBO and VAO.
    GLuint vbo, ebo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
 
    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        GLint posAttribPtrLoc = program.getAttribLocation("position"); //glGetAttribLocation(program, "position");
        glVertexAttribPointer(posAttribPtrLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttribPtrLoc);
        
        GLint colorAttribPtrLoc = program.getAttribLocation("color"); //glGetAttribLocation(program, "color");
        glVertexAttribPointer(colorAttribPtrLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(colorAttribPtrLoc);
        
        GLint texAttribPtrLoc = program.getAttribLocation("texCoord"); //glGetAttribLocation(program, "color");
        glVertexAttribPointer(texAttribPtrLoc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
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
        
        // Rendering
        glClearColor(0.80f, 0.80f, 0.80f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // This uniform should be defined here. Not outside the loop.
        // Define my transform matrix.
        glm::mat4 model, view, projection;
        model = glm::rotate(model, 45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.8f, 0.5f, 1.0f));
        //transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
        projection = glm::perspective(45.0f, 800.0f/600, 0.1f, 100.0f);
        glm::mat4 transform = projection * view * model;
        GLuint transformLoc = program.getUniformLocation("transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
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
