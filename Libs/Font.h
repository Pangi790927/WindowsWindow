#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "SimplexFont.h"

class Font{                     /// all fonts are display lists in 2d interval [0,0] - [1,1]
public:                         /// 0 -> 256 ASCII
    GLuint character[256];
    float horizontalScale[256];

    Font(){
        for( int i = 0; i < 256; i++ )
            horizontalScale[i] = 1.0f;
    }

    void load( String str ){
        if( str == "basicFont" )
        {
            cout << "Load Most Basic Font" << endl;

            for( int i = 32; i < 32 + 95; i++ ){
                character[i] = glGenLists( 1 );

                glNewList( character[i], GL_COMPILE );

                    glBegin( GL_LINES );
                        int last_x = -1;
                        int last_y = -1;

                        horizontalScale[i] = SimplexFontArray[i - 32][1] / 30.0f;

                        for( int j = 0; j < SimplexFontArray[i - 32][0]; j++ ){
                            if( last_x != -1 && SimplexFontArray[i - 32][j * 2 + 2] != -1 ){

                                glVertex2f( SimplexFontArray[i - 32][j * 2 + 2] / 30.0f
                                          , SimplexFontArray[i - 32][j * 2 + 3] / 30.0f );

                                glVertex2f( last_x / 30.0f
                                          , last_y / 30.0f );

                                last_x = SimplexFontArray[i - 32][j * 2 + 2];
                                last_y = SimplexFontArray[i - 32][j * 2 + 3];
                            }else{
                                last_x = SimplexFontArray[i - 32][j * 2 + 2];
                                last_y = SimplexFontArray[i - 32][j * 2 + 3];
                            }
                        }

                    glEnd();

                glEndList();
            }
        }
        else
        {
            /// load a real font
        }
    }
};

#endif // FONT_H_INCLUDED
