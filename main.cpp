#include <iostream>

/// math libs
#include "Libs/MathLib.h"

/// window libs
#include "Libs/Window.h"
#include "Libs/Console.h"

/// Texture Libs
#include "Libs/Texture.h"

/// Internet connection
#include "Libs/IPClientServerConnection.h"

/// For fun
#include "Libs/PlanetsMiniApp.h"

using namespace std;

/**
    Bug List:
        -- Pressing Windows key does weird stuff in console
**/

class foo : public WindowCore{
public:
    int boo;

    Texture neptun;

    foo( int width, int height, HWND h = NULL, Options opt = Options() )
        : WindowCore( width, height, h, opt ) {}

    virtual void init(){
        neptun.loadTexture( "Data/Texture/Uranus.png" );
    }

    virtual void draw(){

        if( window->ReSizeGLScene() ){
            width = window->Width;
            height = window->Height;
        }

        glClearColor( 0, 0, 0, 0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

        glEnable( GL_TEXTURE_2D );
        neptun.bind();

        glPushMatrix();
            glLoadIdentity();

            glScalef( 0.5, 0.5, 0.5 );

            glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 1.0f);

                glVertex2f( -1, -1);
                    glTexCoord2f( 0, 0 );

                glVertex2f( -1, 1);
                    glTexCoord2f( 0, 1 );

                glVertex2f( 1, 1);
                    glTexCoord2f( 1, 1 );

                glVertex2f( 1, -1);
                    glTexCoord2f( 1, 0 );
            glEnd();
        glPopMatrix();
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

int main()
{
    WindowManager testMain;

    WindowLink newLink =     testMain.addWindow( new foo(800, 800) );
//    WindowLink fooLink =     testMain.addWindow( new foo( 512, 512, newLink ) );
    WindowLink consoleLink = testMain.addWindow( new Console( 640, 480, newLink ) );
//    WindowLink planetsLink = testMain.addWindow( new Planets( 640, 640, newLink ) );

    (static_cast<Console*>(consoleLink.window->windowCore))->getManagerLink( &testMain );
    (static_cast<Console*>(consoleLink.window->windowCore))->getWindowLink( &newLink );

//    cout << (static_cast<foo*>(newLink.window->windowCore))->boo << endl;

    testMain.Loop();

    return 0;
}
