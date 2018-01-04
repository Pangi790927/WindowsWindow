#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include "MathLib.h"
#include "String.h"
#include "Log.h"
#include "Font.h"

class TextBox{
public:
    Font font;
    vector <String> buffer;       ///    [line][column][character];

    int *width;                 ///     handle for width and height
    int *height;

    Point2f upLeftCorner;       ///     position inside window
    Point2f downLeftCorner;

    int fontHeight;             ///     font width to be deduced from this
    int lineSpacing;            ///     space between spaces

    int lineSize;               ///     characters on a line
    int bufferSize;             ///     max number of lines

    float scaleHeight;
    float scaleWidth;

    TextBox() {}

    void rescale(){
        scaleHeight = ( fontHeight ) / float(*height);
        scaleWidth = ( fontHeight ) / float(*width);
    }

    TextBox( int fontHeight, int *width, int *height, int bufferSize = 300 , int lineSize = 145, String fontName = String("basicFont")
           , Point2f downLeftCorner = Point2f( -1.0f, -1.0f )
           )
            : fontHeight(fontHeight), bufferSize(bufferSize), lineSize(lineSize),
              downLeftCorner(downLeftCorner), width(width), height(height)
    {
        font.load( fontName );
        rescale();

        buffer.resize(bufferSize);
    }

    String popStringFront(){
        String result = buffer[0];

        buffer.erase( buffer.begin() );

        return result;
    }

    void pushStringBack( String str ){
        if( str == "" ){
            buffer.push_back(str);
            return;
        }

        while( str.lenght() > lineSize ){
            buffer.push_back(str.getStringTo(str.begin() + lineSize - 1));
            str.eraseTo(str.begin() + lineSize - 1);
        }

        if( !str.empty() ){
            buffer.push_back(str);
        }
    }

    void drawBuffer(){
        for( int i = buffer.size() - 1; i >= 0; i-- ){
            float rightTranslate = 0.0;
            for( int j = 0; j < buffer[i].size(); j++ ){
                glPushMatrix();
                    glLoadIdentity();

                    glTranslatef( downLeftCorner.x, downLeftCorner.y, 0 );

                    glScalef( scaleWidth, scaleHeight, 1.0f );

                    glTranslatef( rightTranslate, buffer.size() - i, 0);
                    rightTranslate += font.horizontalScale[buffer[i][j]];

                    glCallList( font.character[ buffer[i][j] ] );
                glPopMatrix();
            }
        }
    }
};

#endif // TEXTBOX_H_INCLUDED
