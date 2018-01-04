#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "SOIL/SOIL.h"

class Texture{
public:
    GLuint texture;

    int width;
    int height;
    int channels;

    unsigned char *data;

    Texture(){
        texture = 0;
    }

    void loadTexture( String textureName ){
        glEnable( GL_TEXTURE_2D );

        bind();

        texture = SOIL_load_OGL_texture( textureName.toCharString(),
                                         SOIL_LOAD_AUTO,
                                         SOIL_CREATE_NEW_ID,
                                         SOIL_FLAG_INVERT_Y );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLfloat fLargest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

        loadDataTexture(textureName);
    }

    void bind(){
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glEnable( GL_TEXTURE_2D );
    }

    void loadDataTexture( String textureName ){
        data = SOIL_load_image( textureName.toCharString()
                                , &width, &height, &channels, SOIL_LOAD_AUTO );
    }

    void sendDataToTexture(){
        bind();

        GLenum format = GL_RGB;
        switch( channels ){
            case 1: format = GL_ALPHA;               break;
            case 2: format = GL_LUMINANCE_ALPHA ;    break;
            case 3: format = GL_RGB;                 break;
            case 4: format = GL_RGBA;                break;
        }

        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data );
    }

    Color4f getPixel( int x, int y ){
        if( x < 0 || y < 0 )
            return Point3f(0, 0, 0);

        if( x >= width || y >= height )
            return Point3f(0, 0, 0);

        if( channels <= 2 || channels >= 5 )
            return Point3f(0, 0, 0);

        if( channels == 3 )
            return Color4f( data[(y * width + x) * channels + 0]
                           ,data[(y * width + x) * channels + 1]
                           ,data[(y * width + x) * channels + 2]
                           , 0 );

        if( channels == 4 )
            return Color4f( data[(y * width + x) * channels + 0]
                           ,data[(y * width + x) * channels + 1]
                           ,data[(y * width + x) * channels + 2]
                           ,data[(y * width + x) * channels + 3] );
    }

    void setPixel( int x, int y, Color4f color ){
        if( x < 0 || y < 0 )
            return ;

        if( x >= width || y >= height )
            return ;

        if( channels <= 2 || channels >= 5 )
            return ;

        data[(y * width + x) * channels + 0] = color.r;
        data[(y * width + x) * channels + 1] = color.g;
        data[(y * width + x) * channels + 2] = color.b;

        if( channels == 4 )
            data[(y * width + x) * channels + 3] = color.a;
    }

    void drawRawData(){
        glRotatef( 180, 0, 0, 1 );
        for( int i = 0; i < width * height; i++ ){
            glBegin(GL_POINTS);
                glColor3f( data[ i * channels + 0 ] / 255.
                          ,data[ i * channels + 1 ] / 255.
                          ,data[ i * channels + 2 ] / 255. );

                glVertex2f( float( i % width) / width, float( i / width) / width);
            glEnd();
        }
        glRotatef( -180, 0, 0, 1 );
    }

};

#endif
