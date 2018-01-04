#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include "Window.h"
#include "MathLib.h"

#include "String.h"
#include "Log.h"

#include "TextBox.h"
#include "TextImputBox.h"

#include "CommandParser.h"

class Console : public WindowCore{
public:
    Log pastCommands;
    Log commandLog;

    TextBox textBox;
    TextInputBox inputListener;

    CommandParser commandParser;

    Console( int width, int height, HWND h = NULL, Options opt = Options() )
            : WindowCore( width, height, h, opt ){
    }

    ~Console(){}

    void getManagerLink( WindowManager *link ){
        commandParser.managerLink = link;
    }

    void getWindowLink( WindowLink *link ){
        commandParser.windowLink = link;
    }

    virtual void init(){
        textBox = TextBox( 35, &width, &height, 300, 100, "basicFont", Point2f( -1.0f, -( 1.0f - 0.01 ) ) );
        inputListener = TextInputBox( &textBox, &commandLog, &(window->eventMemory->KeyBoard) );
        commandParser = CommandParser( &options, &inputListener, window );
    }

    virtual void draw(){

        if( window->ReSizeGLScene() ){
            width = window->Width;
            height = window->Height;

            textBox.rescale();
        }

        glClearColor( 0, 0, 0, 0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        textBox.drawBuffer();
    }

    virtual void input(){
        if( inputListener.listenToKeyBoard() ){
            //inputListener.pushStringBack();
            commandParser.parseCommand( inputListener.getCommandFromLog() );
        }
    }

    int commandCount = 0;
    virtual void tick(){
//        if( commandCount++ % 2000 == 0 )
//            inputListener.pushStringBack( String() + "1232323" + 0 + 0 + 0 );
    }

    virtual void destroy(){
        cout << "destructing window ..." << endl;
        //commandLog.SaveLog( "ConsoleLog.txt" );
    }
};


#endif // CONSOLE_H_INCLUDED
