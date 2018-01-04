#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <vector>
#include <iostream>
#include <fstream>

#include "String.h"
#include "Char.h"

class LogLine{
public:
    String line;
    int id_number;

    LogLine( int id_number, String line ) : id_number(id_number), line(line) {}
};

class Log{
public:
    vector <LogLine> lines;

    Log() {}

    Log( const Log& log ){
        for( int i = 0; i < lines.size(); i++ )
            lines = log.lines;
    }

    Log( String file ){
        ifstream f( file.toCharString() );

        for( String line; f >> line; ){
            line.deleteFirstNonSeparator( " \t\n" );

            int id = line.getFirstAsInt( " \t\n" );

            line.deleteFirstWord( " \t\n" );
            line.deleteFirstNonSeparator( " \t\n" );

            if( id && !line.empty() )
                lines.push_back( LogLine( id, line ) );
        }
    }

    void addToLog( int id_number, String line ){
        lines.push_back( LogLine( id_number, line ) );
    }

    void SaveLog( String file ){
        ofstream g( file.toCharString() );

        for( int i = 0; i < lines.size(); i++ )
            g << lines[i].id_number << " " << lines[i].line << endl;

        g.close();
    }

    friend ostream& operator << ( ostream& stream, const Log& arg ){
        for( int i = 0; i < arg.lines.size(); i++ )
            stream << arg.lines[i].id_number << " " << arg.lines[i].line << endl;

        return stream;
    }

    friend istream& operator >> ( istream& stream, Log& arg ){

        for( String line; stream >> line; ){
            line.deleteFirstNonSeparator( " \t\n" );

            int id = line.getFirstAsInt( " \t\n" );

            line.deleteFirstWord( " \t\n" );
            line.deleteFirstNonSeparator( " \t\n" );

            if( id && !line.empty() )
                arg.lines.push_back( LogLine( id, line ) );
        }

        return stream;
    }
};

#endif // LOG_H_INCLUDED
