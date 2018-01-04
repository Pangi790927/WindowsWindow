#ifndef TEXTIMPUTBOX_H_INCLUDED
#define TEXTIMPUTBOX_H_INCLUDED

#include "Window.h"
#include "String.h"

class TextInputBox{
public:
    TextBox *textBox;
    Log *commandLog;
    keyboard *keys;
    keyboard presedKeys;

    static const unsigned short int ASCII_COUNT = 256;

    bool reservedKeys[ASCII_COUNT];

    int waitingTime;
    int waitedTime[ASCII_COUNT];

    long commandNumber = 0;

    int commandBack = 0;

    TextInputBox() {}

    TextInputBox( TextBox *textBox, Log *commandLog, keyboard *keys, int waitingTime = 10 )
        : textBox(textBox), commandLog(commandLog), keys(keys), waitingTime(waitingTime) {

        for( int i = 0; i < ASCII_COUNT; i++ ){
            waitedTime[i] = waitingTime;
            reservedKeys[i] = false;
        }

        reservedKeys[VK_RETURN] = true;
        reservedKeys[VK_BACK] = true;
        reservedKeys[VK_SHIFT] = true;
        reservedKeys[VK_CONTROL] = true;
        reservedKeys[VK_UP] = true;
        reservedKeys[VK_DOWN] = true;
    }

    int scan2ascii(short int scancode, unsigned short* result) /// unclear function
    {
       static HKL layout=GetKeyboardLayout(0);
       static unsigned char State[ASCII_COUNT];

       if (GetKeyboardState(State)==FALSE){
            return 0;
       }

       //unsigned int vk = MapVirtualKeyEx(scancode,1,layout);
       return ToAsciiEx(scancode,scancode,State,result,0,layout);
    }

    bool isReserved( unsigned int key ){
        return reservedKeys[key];
    }

    bool DelayPressKey( unsigned char key ){
        if( (*keys)[key] == true && ( presedKeys[ key ] == false || !waitedTime[key] && (*keys)[key] == true) ){
            presedKeys[key] = true;
            return true;
        }else{
            waitedTime[key]--;
        }

        if( (*keys)[key] == false ){
            presedKeys[key] = false;
            waitedTime[key] = waitingTime;
        }
        return false;
    }

    String getCommandFromLog( int n = 1 ){
        int sizeLog = commandLog->lines.size();

        if( sizeLog - n >= 0 && sizeLog - n < sizeLog )
            return commandLog->lines[(commandLog->lines.size() - n)].line;
        else
            return textBox->buffer.back();
    }

    void pushStringBack( String str ){
        textBox->buffer.back() = str;
        textBox->pushStringBack("");            // this is for writing in it
    }

    bool listenToKeyBoard(){
        if( DelayPressKey( VK_RETURN ) ){
            commandLog->addToLog( commandNumber++, textBox->buffer.back() );
            textBox->pushStringBack( "" );
            commandBack = 0;

            return 1;   /// command has been issued and must be checked by interpretor
        }
        else if( DelayPressKey( VK_BACK ) ){
            textBox->buffer.back().popLastChar();
            commandBack = 0;

            return 0;
        }
        else if( DelayPressKey( VK_UP ) ){
            int logSize = commandLog->lines.size();

            do{
                commandBack = min( logSize, ++commandBack );
                textBox->buffer.back() = getCommandFromLog( commandBack );
            } while( commandBack < logSize && textBox->buffer.back() == "" );

            return 0;
        }
        else if( DelayPressKey( VK_DOWN ) ){
            commandBack = commandBack <= 1 ? 1 : --commandBack;
            textBox->buffer.back() = getCommandFromLog( commandBack );

            return 0;
        }
        else{
            for( int i = 0; i < ASCII_COUNT; i++ )
                if( !isReserved(i) && DelayPressKey(i) ){
                    commandBack = 0;

                    unsigned short result[5];
                    int numberResults = scan2ascii( i, result );

                    textBox->buffer.back() += (char)result[0];
                }

            return 0;
        }
        return 0;
    }
};

/// Known bug: windows key is breaking regular writing

#endif // TEXTIMPUTBOX_H_INCLUDED
