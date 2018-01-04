#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

class Char{
public:
    char code;

    Char( char code = '\0' ) : code(code) {}

    Char operator + ( const Char& arg ){
        return code + arg.code;
    }

    Char operator - ( const Char& arg ){
        return code - arg.code;
    }

    operator char () {
        return code;
    }

    bool operator == ( const Char& arg ){
        return code == arg.code;
    }

    bool operator != ( const Char& arg ){
        return code != arg.code;
    }

    bool operator > ( const Char& arg ){
        return code > arg.code;
    }

    bool operator < ( const Char& arg ){
        return code < arg.code;
    }

    bool operator >= ( const Char& arg ){
        return code >= arg.code;
    }

    bool operator <= ( const Char& arg ){
        return code <= arg.code;
    }

    char toLower(){
        if( isAlpha() && isUpper() )
            return code = code + ( firstLower() - firstUpper() );
        else
            return code;
    }

    char toUpper(){
        if( isAlpha() && isLower() )
            return code = code + ( firstUpper() - firstLower() );
        else
            return code;
    }

    char getLower() const {
        if( isAlpha() && isUpper() )
            return code + ( firstLower() - firstUpper() );
        else
            return code;
    }

    char getUpper() const {
        if( isAlpha() && isLower() )
            return  code + ( firstUpper() - firstLower() );
        else
            return code;
    }

    bool isLower() const {
        return code > firstLower() && code < lastLower();
    }

    bool isUpper() const {
        return code > firstUpper() && code < lastUpper();
    }

    bool isDigit() const {
        return code > firstDigit() && code < lastDigit();
    }

    bool isAlpha() const {
        return isUpper() || isLower();
    }

    static char firstLower(){
        return 'a';
    }

    static char firstUpper(){
        return 'A';
    }

    static char firstDigit(){
        return '0';
    }

    static char lastLower(){
        return 'z';
    }

    static char lastUpper(){
        return 'Z';
    }

    static char lastDigit(){
        return '9';
    }

    static char toLower( Char& arg ){
        return arg.toLower();
    }

    static char toUpper( Char& arg ){
        return arg.toUpper();
    }

    static bool getLower( const Char& arg ){
        return arg.getLower();
    }

    static bool getUpper( const Char& arg ){
        return arg.getUpper();
    }

    static bool isLower( const Char& arg ){
        return arg.isLower();
    }

    static bool isUpper( const Char& arg ){
        return arg.isUpper();
    }

    static bool isDigit( const Char& arg ){
        return arg.isDigit();
    }

    static bool isAlpha( const Char& arg ){
        return arg.isAlpha();
    }

    friend istream& operator >> ( istream& stream, Char& arg ){
        stream.get( arg.code );

        return stream;
    }

    friend ostream& operator << ( ostream& stream, Char& arg ){
        stream << arg.code;

        return stream;
    }
};

#endif // CHAR_H_INCLUDED
