#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#define GLEW_STATIC

#include <gl/glew.h>
#include "WindowClass.h"
#include "options.h"
#include <map>
#include <vector>

using namespace std;

class WindowCore{
public:
    int width;
    int height;
    HWND hwndParent;
    Options options;
    OpenGLWindow *window;
    EventMemory *eventMemory;

    WindowCore( int width = 128, int height = 128, HWND hwndParent = NULL
        , Options options = Options(), EventMemory *eventMemory = new EventMemory() )

        : options(options), width(width), height(height), hwndParent(hwndParent), eventMemory(eventMemory)
    {}

    int& operator [] ( string name ){
        return options[name];
    }

    Point2f getMousePos(){
        Point2f pos = window->eventMemory->Mouse.getPos();

        pos.x /= window->Width;
        pos.y /= window->Height;
        pos *= 2;
        pos -= Point2f( 1, 1 );
        pos.y = -pos.y;

        return pos;
    }

    /// user stuff

    virtual void init(){
        ;
    }

    virtual void draw(){
        ;
    }

    virtual void input(){
        ;
    }

    virtual void tick(){
        ;
    }

    virtual void destroy(){
        ;
    }
};

class WindowFrame{
public:
    OpenGLWindow window;
    WindowCore *windowCore;

    WindowFrame( WindowCore *windowCore = new WindowCore() )
            : window( (*windowCore)["perspective"], windowCore->width, windowCore->height
                , windowCore->hwndParent, windowCore->eventMemory
              )
            , windowCore(windowCore)
    {
        windowCore->window = &window;
        (*windowCore)["Closed"] = false;

        window.setVSync( (*windowCore)["vSync"] );
        glewInit();
        window.ReSizeGLScene();

        windowCore->init();
    }

    void draw(){
        if( !(*windowCore)["Closed"] ){
            wglMakeCurrent( window.hDC, window.hRC );

            windowCore->draw();

            SwapBuffers( window.hDC );
            wglMakeCurrent(NULL, NULL);
        }
    }

    void input(){
        windowCore->input();
    }

    void tick(){
        windowCore->tick();
    }

    void destroy(){
        windowCore->destroy();
    }
};

class WindowLink{
public:
    WindowFrame* window;
    HWND hwnd;

    WindowLink( WindowFrame* window = NULL, HWND hwnd = NULL ) : window(window), hwnd(hwnd) {}

    bool linked(){
        return window != NULL;
    }

    operator HWND() const{
        return hwnd;
    }
};

class WindowManager{
public:
    vector <WindowFrame*> windows;
    map <HWND, WindowFrame*> handleInput;
    WindowFrame* currentWindow;

    WindowLink addWindow( WindowCore *windowCore = new WindowCore() ){

        WindowFrame* windowFrame = new WindowFrame( windowCore );

        windows.push_back( windowFrame );
        handleInput.insert( pair <HWND, WindowFrame*> ( windows[ windows.size() - 1 ]->window.hwnd,
                                                         windows[ windows.size() - 1 ] ) );

        return WindowLink( windowFrame, windowFrame->window.hwnd );
    }

    void Loop(){
        while(true){
            currentWindow = handleInput[GetActiveWindow()];

            if (PeekMessage(&currentWindow->window.msg, NULL, 0, 0, PM_REMOVE)){
                if (currentWindow->window.msg.message == WM_QUIT){
                    (*(currentWindow->windowCore))["Closed"] = true;
                }
                else{
                    TranslateMessage(&currentWindow->window.msg);
                    DispatchMessage(&currentWindow->window.msg);

                    currentWindow->input();
                }
            }
            else
            for( int i = 0 ; i < windows.size() ; i++ ){
                if( i >= 0 && i < windows.size() && (*(windows[i]->windowCore))["Closed"] == true ){
                    windows[i]->destroy();
                    delete windows[i];

                    windows.erase( windows.begin() + i );
                    i--;
                }

                if( i >= 0 && i < windows.size() )
                    windows[i]->tick();

                if( i >= 0 && i < windows.size() )
                    windows[i]->draw();
            }
        }
    }
};

#endif // WINDOW_H_INCLUDED
