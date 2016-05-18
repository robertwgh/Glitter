To be able to find OpenGL library and include folder, the following things have been done:
1. install nvidia-dev package
    sudo apt-get install nvidia-364-dev
2. modify the cmake FindOpenGL.cmake module at
    /usr/share/cmake-3.2/Module/FindOpenGL.cmake
    add path for OPENGL_gl_LIBRARY
                 OPENGL_INCLUDE_DIR

Original Glitter should be fine without the above change. 
But after I insert the SOIL submodule, we need GL/gl.h to build. otherwise cmake won't build.
