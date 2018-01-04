#ifndef SEPARATORLIST_H_INCLUDED
#define SEPARATORLIST_H_INCLUDED

#include <vector>

#include "Char.h"

class SeparatorList{
public:
    vector <Char> separators;

    SeparatorList( char separator ) {
        addSeparator( separator );
        addSeparator('\0');
    }

    SeparatorList( char* separator ) {
        if(separator != NULL)
            for( int i = 0; separator[i] != '\0'; i++ )
                addSeparator( separator[i] );

        addSeparator('\0');
    }

    SeparatorList( const SeparatorList& list ) {
        for( int i = 0; i < list.separators.size(); i++ )
            addSeparator( list.separators[i] );

        addSeparator('\0');
    }

    SeparatorList() {
        addSeparator('\0');
    }

    Char& operator [] (int i){
        return separators[i];
    }

    size_t size(){
        return separators.size();
    }

    bool contains( Char separator ){
        for( int i = 0; i < separators.size(); i++ )
            if( separator == separators[i] )
                return true;

        return false;
    }

    void addSeparator( Char separator ){
        if( !contains(separator) )
            separators.push_back(separator);
    }

    void deleteSeparator( Char separator ){
        while( contains( separator ) ){
            separators.erase( find( separators.begin(), separators.end(), separator ));
        }
    }

    bool empty() const {
        return !separators.size();
    }

    void addSeparators( SeparatorList separator ){
        while( !separator.empty() ){
            addSeparator( separator.separators.front() );
            separator.deleteSeparator( separator.separators.front() );
        }
    }

    void deleteSeparators( SeparatorList separator ){
        while( !separator.empty() ){
            deleteSeparator( separator.separators.front() );
            separator.deleteSeparator( separator.separators.front() );
        }
    }
};

#endif // SEPARATORLIST_H_INCLUDED
