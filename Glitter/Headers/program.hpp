#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "glitter.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace ogl{
    
class Program{
public:

    enum mode{
        STRING_MODE, 
        FILE_MODE
    };
    
    Program()
    {
        program = glCreateProgram();
    }
    /*
    Program(const GLchar * vsSource, const GLchar * fsSource, const mode m)
    {
        program = glCreateProgram();
        
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vsSource, NULL);
        glCompileShader(vertexShader);
        GLint err;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &err);
        if(!err)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "Error: failed to compile vertex shader. " << std::endl << infoLog << std::endl;
            return;
        }
        
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fsSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &err);
        if(!err)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "Error: failed to compile fragment shader. " << std::endl << infoLog << std::endl;
            return;
        }
        
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &err);
        if(!err)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "Error: failed to link shaders. " << std::endl << infoLog << std::endl;
            return;
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    */
    
    Program(const GLchar * vertexStr, const GLchar * fragmentStr, const mode m)
    {
        if(vertexStr == NULL || fragmentStr == NULL) return;
        const GLchar * vsSource = vertexStr;
        const GLchar * fsSource = fragmentStr;
        
        if(m == FILE_MODE)
        {
            std::ifstream vsFile;
            std::ifstream fsFile;
            
            vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            
            try{
                vsFile.open(vertexStr);
                fsFile.open(fragmentStr);
                std::stringstream vStream, fStream;
                
                vStream << vsFile.rdbuf();
                fStream << fsFile.rdbuf();
                vsFile.close();
                fsFile.close();
                
                vsSource = vStream.str().c_str();
                fsSource = fStream.str().c_str();
                
            }catch(std::ifstream::failure e){
                
                std::cout << "Error: failed to read shader files" << std::endl;
            }
        }

        program = glCreateProgram();
        
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vsSource, NULL);
        glCompileShader(vertexShader);
        GLint err;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &err);
        if(!err)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "Error: failed to compile vertex shader. " << std::endl << infoLog << std::endl;
            return;
        }
        
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fsSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &err);
        if(!err)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "Error: failed to compile fragment shader. " << std::endl << infoLog << std::endl;
            return;
        }
        
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &err);
        if(!err)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "Error: failed to link shaders. " << std::endl << infoLog << std::endl;
            return;
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    
    
    GLuint get()
    {
        return program;
    }
    
    void use()
    {
        glUseProgram(program);
    }
    
    GLint getAttribLocation(const GLchar * attrib)
    {
        if(attrib == NULL) return -1;
        return glGetAttribLocation(program, attrib);
    }
    
    GLint getUniformLocation(const GLchar * uniform)
    {
        if(uniform == NULL) return -1;
        return glGetUniformLocation(program, uniform);
    }
    
private:
    GLuint program;
};

}

#endif
