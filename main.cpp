#include <iostream>
#include <string>
#include <exception>
#include <SDL.h>
#include <GL/glew.h>


void init()
{
    int _major=0;
    int _minor=0;
    int _renderpath=0;
    SDL_Window* _sdlWindow;
    SDL_GLContext _glcontext;
    bool npot;

    std::string message =  "SDL_CreateWindow - ";

    uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

    _sdlWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, flags);
    if (!_sdlWindow)
    {
        throw std::runtime_error(message + "[FAIL]");
    }

    std::cout << message + "[OK]" << std::endl;

    message =  "Init OpenGL - ";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    _glcontext = SDL_GL_CreateContext(_sdlWindow);

    if (!_glcontext)
    {
        throw std::runtime_error(message + "[FAIL]");
    }

    std::cout << message + "[OK]" << std::endl;


    char *version_string = (char*)glGetString(GL_VERSION);
    if (version_string[0]-'0' >=3) //we have atleast gl 3.0
    {
        glGetIntegerv(GL_MAJOR_VERSION, &_major);
        glGetIntegerv(GL_MINOR_VERSION, &_minor);
        _renderpath = 1;
    }
    else
    {
        _major = version_string[0]-'0';
        _minor = version_string[2]-'0';
        _renderpath = 0;
    }

    std::cout << "Using OpenGL " << _major << "." << _minor << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    switch (_renderpath)
    {
        case 0:
            std::cout << "Render path: OpenGL 2.1"  << std::endl;
            break;
        case 1:
            std::cout << "Render path: OpenGL 3.0+" << std::endl;
            break;
        default:
            break;
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    glGetError(); // glew sometimes throws bad enum, so clean it
    if (GLEW_OK != err)
    {
        throw std::runtime_error(message + "[FAIL]: " + std::string((char*)glewGetErrorString(err)));
    }

    npot = GL_ARB_texture_rectangle;
    std::cout << (npot ? "TexRec" : "No TexRec")  << std::endl;

    if (_renderpath==1)
    {
        GLint count;
        glGetIntegerv(GL_NUM_EXTENSIONS, &count);

        GLint i;
        for (i = 0; i < count; i++)
        {
            std::cout << (const char *) glGetStringi(GL_EXTENSIONS, i) << std::endl;
        }
    }
    else
    {
        std::cout << (const char *) glGetString(GL_EXTENSIONS) << std::endl;
    }
}

int main(void)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    init();
}
