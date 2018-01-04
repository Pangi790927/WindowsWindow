#ifndef PLANETSMINIAPP_H_INCLUDED
#define PLANETSMINIAPP_H_INCLUDED

class Planet{
public:
    Point3f pos;
    Point3f vel;

    float mass;

    long long planetID;

    Point3f force;
    vector <Point3f> trace;

    static float gravitationalConstant;
    static long long lastID;

    Planet( Point3f pos, Point3f vel, float mass ) : pos(pos), vel(vel), mass(mass) {
        planetID = lastID++;
    }

    float field( float distance ){
        return mass * gravitationalConstant / distance / distance;
    }

    void calculateForces( vector <Planet> planets ){
        force = Point3f( 0, 0, 0 );
        for( int i = 0; i < planets.size(); i++ ){

            Point3f forceBase = (planets[i].pos - pos);

            if( forceBase.abs() != 0 )
                force += forceBase * ( field( forceBase.abs() ) * planets[i].mass / forceBase.abs() );
        }
    }

    void incrementPos( float timeIncrement ){
        trace.push_back( pos );

        vel = vel + (force / mass * timeIncrement);
        pos = pos + vel * timeIncrement;
    }

    void draw( Point3f transl, float massRadius ){
        glPushMatrix();
            glLoadIdentity();

            glScalef( 0.1, 0.1, 0.1 );
            glTranslatef( transl.x, transl.y, transl.z );

            glBegin(GL_POINTS);
                glColor3f( 1, 1, 1 );

                for( int i = 0; i < trace.size(); i++ )
                    glVertex2f( trace[i].x, trace[i].y );

            glEnd();

//            glBegin( GL_LINES );
//                glColor3f( 1, 0, 0 );
//                glVertex2f( pos.x + vel.x / 50.0f, pos.y + vel.y / 50.0f );
//                glVertex2f( pos.x, pos.y );
//            glEnd();
//
//            glBegin( GL_LINES );
//                glColor3f( 0, 0, 1 );
//                glVertex2f( pos.x + (force.x / force.abs() / 5.0f), pos.y + (force.y / force.abs() / 5.0f) );
//                glVertex2f( pos.x, pos.y );
//            glEnd();

            glBegin(GL_TRIANGLES);
                glColor3f( 1, 1, 1 );

                int n = 20;
                float Pi = 3.14159265359;
                float ratio = Pi * 2 / n;
                float radius = 0.03 * massRadius / 1000;

                for( float angle = 0; angle <= 0.000001 + Pi * 2;angle += ratio ){
                    glVertex2f( pos.x, pos.y );
                    glVertex2f( sin(angle) * radius + pos.x, cos(angle) * radius + pos.y );
                    glVertex2f( sin(angle + ratio) * radius + pos.x, cos(angle + ratio) * radius + pos.y );
                }

            glEnd();
        glPopMatrix();
    }
};

float Planet::gravitationalConstant = 12.53f;
long long Planet::lastID;

class Planets : public WindowCore{
public:
    int boo;

    float timeIncrement = 0.001f;

    Planets( int width, int height, HWND h = NULL, Options opt = Options() )
        : WindowCore( width, height, h, opt ) {}

    vector <Planet> planets;

    virtual void init(){
        planets.push_back( Planet( Point3f(), Point3f(), 100.00 ) );
        totalMomentum = Point3f();
    }

    Point3f totalMomentum;
    float energy = 0;

    virtual void draw(){

        if( window->ReSizeGLScene() ){
            width = window->Width;
            height = window->Height;
        }

        glClearColor( 0, 0, 0, 0 );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        for( int i = 0; i < planets.size(); i++ )
            planets[i].draw( -1 * planets[0].pos, planets[i].mass );

        glBegin( GL_LINES );
            glColor3f( 0, 0, 1 );
            glVertex2f( currentPoint.x, currentPoint.y );
            glVertex2f( lastPoint.x, lastPoint.y );
        glEnd();
    }

    float randFloat(){
        return (( rand() % 10000 ) / 10000.) * 2.0f - 1.0f;
    }

    void addPlanet( Point3f pos = Point3f( 0, 0.5, 0 ), Point3f vel = Point3f( -100, 0, 0 ), float mass = 370 ){
        planets.push_back( Planet( pos, vel, mass ) );
    }

    bool spawn = false;
    bool spawn2 = false;

    Point2f lastPoint;
    Point2f currentPoint;
    bool mouseDown = false;

    virtual void input(){
        if( window->eventMemory->KeyBoard[ VK_SPACE ] && !spawn ){
            addPlanet( Point3f( randFloat() * 8, randFloat() * 8 ) + planets[0].pos
                     , Point3f( randFloat() * 10, randFloat() * 10 )
                     , (1 + randFloat()) * 3.000f );
            totalMomentum += planets[planets.size() - 1].vel * planets[planets.size() - 1].mass;

            spawn = true;
        }
        else if( !window->eventMemory->KeyBoard[ VK_SPACE ] ){
            spawn = false;
        }

        if( window->eventMemory->Mouse.LMB ){
            currentPoint = getMousePos();
        }

        if( window->eventMemory->Mouse.LMB && !mouseDown ){
            lastPoint = getMousePos();
            mouseDown = true;
            spawn2 = true;
        }
        else if( !window->eventMemory->Mouse.LMB ){
            mouseDown = false;

            if( spawn2 ){
                addPlanet( (lastPoint ) * 10 + Point2f(planets[0].pos.x, planets[0].pos.y)
                         , (lastPoint - currentPoint ) * -10 * 2
                         , 3.000f );

                totalMomentum += planets[planets.size() - 1].vel * planets[planets.size() - 1].mass;

                spawn2 = false;
            }
        }

    }

    virtual void tick(){
        for( int i = 0; i < planets.size(); i++ )
            planets[i].calculateForces( planets );

        for( int i = 0; i < planets.size(); i++ )
            planets[i].incrementPos( timeIncrement );

        for( int i = 0; i < planets.size(); i++ )
            if( planets[i].trace.size() > 300 )
                planets[i].trace.erase(planets[i].trace.begin());

        Point3f currentMomentum;

        for( int i = 0; i < planets.size(); i++ ){
            currentMomentum += planets[i].vel * planets[i].mass;
        }

//        cout << "----------------------------------------------------------------------" << endl;
//        cout << "Momentum: current:" << currentMomentum << " normal: " << totalMomentum << endl;

//        for( int i = 0; i < planets.size(); i++ ){
//            if( currentMomentum.x != 0 )
//                planets[i].vel.x = planets[i].vel.x * totalMomentum.x / currentMomentum.x;
//
//            if( currentMomentum.y != 0 )
//                planets[i].vel.y = planets[i].vel.y * totalMomentum.y / currentMomentum.y;
//
//            if( currentMomentum.z != 0 )
//                planets[i].vel.z = planets[i].vel.z * totalMomentum.z / currentMomentum.z;
//        }
    }

    virtual void destroy(){
        ;
    }
};

#endif // PLANETSMINIAPP_H_INCLUDED
