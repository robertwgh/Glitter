#ifndef LEARNGL_UTIL_HPP
#define LEARNGL_UTIL_HPP

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #include <limits.h>
    #define GetCurrentDir getcwd
 #endif

#include <string>

std::string get_work_dir()
{
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        std::cerr << "Failed to get the current path." << std::endl;
        exit(-1);
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
    return std::string(cCurrentPath);
}


std::string get_exe_path()
{
  char result[1024];
  
#ifdef _WIN32
  return std::string( result, GetModuleFileName( NULL, result, 1024 ) );
#else
  ssize_t count = readlink( "/proc/self/exe", result, 1024 );
  return std::string( result, (count > 0) ? count : 0 );
#endif
}

std::string get_exe_dir()
{
    std::string path = get_exe_path();

#ifdef _WIN32
    return path.substr(0, path.rfind('\\'));
#else
    return path.substr(0, path.rfind('/'));
#endif
}


#endif

