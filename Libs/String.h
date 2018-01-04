#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

#include "Char.h"
#include "SeparatorList.h"

class String{
public:
    vector <Char> buffer;

    String() {}

    String( char *str ){
        if( str != NULL )
            for( int i = 0; str[i] != '\0'; i++ )
                buffer.push_back( str[i] );
    }

    String( string str ){
        for( int i = 0; i != str.size(); i++ )
            buffer.push_back( str[i] );
    }

    String( vector <Char> str ){
        for( int i = 0; i != str.size(); i++ )
            buffer.push_back( str[i] );
    }

    String( vector <char> str ){
        for( int i = 0; i != str.size(); i++ )
            buffer.push_back( str[i] );
    }

    String( const String& str ){
        buffer = str.buffer;
    }

    String( Char Character ){
        buffer.push_back( Character );
    }

    String( char Character ){
        buffer.push_back( Character );
    }

    String( int num ){
        (*this) = toString( num );
    }

    String( float num ){
        (*this) = toString( num );
    }

    String( double num ){
        (*this) = toString( num );
    }

    String( long double num ){
        (*this) = toString( num );
    }

    typedef std::vector<Char>::iterator iterator;
    typedef std::vector<Char>::const_iterator const_iterator;

    iterator begin() {
        return buffer.begin();
    }

    const_iterator begin() const {
        return buffer.begin();
    }

    iterator end() {
        return buffer.end();
    }

    const_iterator end() const {
        return buffer.end();
    }

    size_t size() const {
        return buffer.size();
    }

    int lenght(){
        return size();
    }

    bool empty(){
        return !lenght();
    }

    bool outOfBounds( iterator it ){
        if( distance(begin(), it) > size() )
            return true;
        return false;
    }

    Char& operator [] ( int i ){
        return buffer[i];
    }

    String operator + ( String str ) {
        String result(*this);

        for( int i = 0; i < str.size(); i++ )
            result.buffer.push_back( str[i] );

        return result;
    }

    String operator += ( String str ){
        return *this = *this + str;
    }

    char* toCharString() {
        char *result = new char[size() + 1];

        for( int i = 0; i < size(); i++ )
            result[i] = buffer[i];

        result[size()] = '\0';

        return result;
    }

    template <typename type>
    String toString( type number ){
        stringstream stream;
        stream << number;

        string str;
        stream >> str;

        return String( str );
    }

    String getFirstAsWord() {
        getFirstAsWord( getTopSeparator() );
    }

    int getFirstAsInt() {
        getFirstAsInt( getTopSeparator() );
    }

    float getFirstAsFloat() {
        getFirstAsFloat( getTopSeparator() );
    }

    float getFirstAsDouble() {
        getFirstAsDouble( getTopSeparator() );
    }

    iterator getFirstNonSeparator( SeparatorList list ) {
        iterator it = begin();

        while( it != end() && list.contains(*it) )
            it++;

        return it;
    }

    iterator getFirstWordEnd( SeparatorList list ) {
        iterator it = begin();

        while( it != end() && !list.contains(*it) )
            it++;

        return it;
    }

    void erase( iterator first, iterator last ){
        if( outOfBounds(first) )
            first = end();

        if( outOfBounds(last) )
            last = end();

        buffer.erase( first, last );
    }

    void erase( iterator pos ){
        if( outOfBounds(pos) )
            pos = end();

        buffer.erase( pos );
    }

    void eraseTo( iterator last ){
        erase( begin(), last );
    }

    void erasefrom( iterator first ){
        erase( first, end() );
    }

    void deleteFirstNonSeparator( SeparatorList list ){
        iterator it = getFirstNonSeparator( list );

        erase( begin(), it );
    }

    void deleteFirstWord( SeparatorList list ){
        iterator it = getFirstWordEnd( list );

        erase( begin(), it );
    }

    void popLastChar(){
        if( !empty() )
            buffer.pop_back();
    }

    String getFirstAsWord( SeparatorList list ) {
        String result = "";

        iterator it = begin();
        while( it != end() && !(list.contains(*it))  ){
            result += *(it++);
        }

        return result;
    }

    long double getNumber( SeparatorList list ){
        String numberString = getFirstAsWord( list );

        stringstream stream;
        stream << numberString;

        long double number = 0;
        stream >> number;

        return number;
    }

    int getFirstAsInt( SeparatorList list ) {
        return getNumber(list);
    }

    float getFirstAsFloat( SeparatorList list ) {
        return getNumber(list);
    }

    double getFirstAsDouble( SeparatorList list ) {
        return getNumber(list);
    }

    String toUpper(){
        for( int i = 0; i < size(); i++ )
            buffer[i].toUpper();
    }

    String tolower(){
        for( int i = 0; i < size(); i++ )
            buffer[i].toLower();
    }

    String getStringFrom( iterator first ){
        return getStringBetween( first, end() );
    }

    String getStringTo( iterator last ){
        return getStringBetween( begin(), last );
    }

    String getStringBetween( iterator first, iterator last ){
        String result;

        if( outOfBounds(first) )
            first = end();

        if( outOfBounds(last) )
            last = end();

        for( iterator it = first; it != last && it != end(); it++ )
            result += *it;

        return result;
    }

    bool operator == ( String str ) {
        if( size() != str.size() )
            return 0;

        for( int i = 0; i < size(); i++ )
            if( buffer[i] != str[i] )
                return false;

        return true;
    }

    bool operator != ( String str ) {
        return !((*this) == str);
    }

    static vector<SeparatorList> separatorStack;

    static void pushSeparator() {
        separatorStack.push_back( separatorStack.back() );
    }

    static SeparatorList popSeparator() {
        SeparatorList separatorList = separatorStack.back();

        vector<SeparatorList>::iterator it = prev(separatorStack.end());
        separatorStack.erase( it );

        return separatorList;
    }

    static void addSeparator( SeparatorList list ) {
        separatorStack.back().addSeparators(list);
    }

    static void deleteSeparator( SeparatorList list ) {
        separatorStack.back().deleteSeparators(list);
    }

    static bool isSeparator( Char arg ) {
        return separatorStack.back().contains( arg );
    }

    static SeparatorList getTopSeparator() {
        return separatorStack.back();
    }

    template <typename type>
    friend String operator + ( type number, String& str ){
        return String ( number ) + str;
    }

    friend istream& operator >> ( istream& stream, String& arg ){
        string str;
        getline( stream, str );

        arg = str;

        return stream;
    }

    friend ostream& operator << ( ostream& stream, String arg ){
        for( vector<Char>::iterator it = arg.buffer.begin(); it != arg.buffer.end(); it++ )
            stream << ( *it );

        return stream;
    }
};

#endif // STRING_H_INCLUDED
