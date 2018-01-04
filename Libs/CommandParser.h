#ifndef COMMANDPARSER_H_INCLUDED
#define COMMANDPARSER_H_INCLUDED

#include "PlanetsMiniApp.h"

class CommandParser{
public:
    Options *options;
    WindowManager *managerLink;
    WindowLink *windowLink;

    TextInputBox *textInputBox;

    OpenGLWindow *window;

    CommandParser() {}
    CommandParser( Options *options, TextInputBox *textInputBox = NULL
                  , OpenGLWindow *consoleWindow = NULL )
        : options(options), textInputBox(textInputBox), window(consoleWindow) {}

    WindowLink planetLink;

    void parseCommand( String command ){
        String separators = " \n\t";

        command.deleteFirstNonSeparator( separators.toCharString() );

        if( command.getFirstAsWord( separators.toCharString() ) == "exit" ){
            (*options)["Closed"] = true;
        }

        if( command.getFirstAsWord( separators.toCharString() ) == "cmdPrint" ){
            command.deleteFirstWord( separators.toCharString() );
            cout << "This was printed by the command \"cmdPrint\" :" << command << endl;
        }

        if( command.getFirstAsWord( separators.toCharString() ) == "planets" ){
            planetLink = managerLink->addWindow( new Planets( 640, 640, *windowLink ) );
            window->eventMemory->KeyBoard[ VK_RETURN ] = false;
        }

        if( command.getFirstAsWord( separators.toCharString() ) == "planets_put" ){
             if( planetLink.linked() )
                (static_cast<Planets*>(planetLink.window->windowCore))->addPlanet();
        }

        if( command.getFirstAsWord( separators.toCharString() ) == "clear" ){
            for( int i = 0; i < 100; i++ )
                textInputBox->pushStringBack(" ");
        }

        if( command.getFirstAsWord( separators.toCharString() ) == "help" ){

            command.deleteFirstWord( separators.toCharString() );
            command.deleteFirstNonSeparator( separators.toCharString() );

            if( command.getFirstAsWord( separators.toCharString() ) == "" ){
                textInputBox->pushStringBack(" > help <command>    - prints command help");
                textInputBox->pushStringBack(" > planets           - starts planets program");
                textInputBox->pushStringBack(" > cmdPrint <text>   - prints in command prompt");
                textInputBox->pushStringBack(" > exit              - quits console");
                textInputBox->pushStringBack(" > clear             - clears console");
                textInputBox->pushStringBack(" > planets_put       - puts planet near origin in the most");
                textInputBox->pushStringBack(" >                     recent planets window");
            }
            else if( command.getFirstAsWord( separators.toCharString() ) == "planets" ){
                textInputBox->pushStringBack(" - this command starts a demo program");
                textInputBox->pushStringBack(" - that implements a very basic");
                textInputBox->pushStringBack(" - physics iterator ");
                textInputBox->pushStringBack(" - usage: planets ");
            }
            else if( command.getFirstAsWord( separators.toCharString() ) == "cmdPrint" ){
                textInputBox->pushStringBack(" - this command prints text ");
                textInputBox->pushStringBack(" - in the main console ");
                textInputBox->pushStringBack(" - usage: cmdPrint <text> ");
            }
            else if( command.getFirstAsWord( separators.toCharString() ) == "exit" ){
                textInputBox->pushStringBack(" - this command quits this console ");
                textInputBox->pushStringBack(" - usage: exit ");
            }
            else if( command.getFirstAsWord( separators.toCharString() ) == "help" ){
                textInputBox->pushStringBack(" - this command prints a list of commands ");
                textInputBox->pushStringBack(" - or the help for a specific command ");
                textInputBox->pushStringBack(" - usage: help ");
                textInputBox->pushStringBack(" - usage: help <command> ");
            }
            else if( command.getFirstAsWord( separators.toCharString() ) == "clear" ){
                textInputBox->pushStringBack(" - clears console by adding 100 ");
                textInputBox->pushStringBack(" - empty lines into this console ");
                textInputBox->pushStringBack(" - usage: clear ");
            }
            else{
                textInputBox->pushStringBack(" - command does not have help or it ");
                textInputBox->pushStringBack(" - does not exist ");
            }
        }
    }
};

#endif // COMMANDPARSER_H_INCLUDED
