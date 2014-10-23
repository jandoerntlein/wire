/**
 *  Very simple test.
 */
#include <iostream>
// #define WIRE_USE_EXCEPTIONS
#include "wire.h"

class Button
{
public:
    Wire<>              toggle;
    Wire<int>           changeColor;
    Wire<float,float>   range;

    DirectWire<>    direct_toggle;
    DirectWire<int> direct_ChangeColor;
};

class Light
{
public:
    Light() 
    {
        m_color     = 0;
        m_isEnabled = false;
        m_idx       = s_idx++;
    }

    void Toggle()
    {
        m_isEnabled = !m_isEnabled;
        std::cout << "Light " << m_idx << " is set " << ( m_isEnabled ? "ON" : "OFF" ) << " (color = " << m_color << ")" << std::endl; 
    }

    void setColor( int col )
    {
        m_color = col;
        std::cout << "Light " << m_idx << " changed color to " << m_color << std::endl; 
    }

    void setRange( float from, float to )
    {
        std::cout << "Light " << m_idx << " got range [" << from << ", " << to << "]" << std::endl;         
    }

private: 
    static int  s_idx;
    int  m_idx;
    int  m_color;
    bool m_isEnabled;
};

int Light::s_idx = 0;

int main( int argc, char** argv )
{
    std::cout << "--- Wire Test ---" << std::endl;

    Button B;
    Light  L1;
    Light  L2;

    B.toggle.connect( &Light::Toggle, &L1 );
    B.toggle.connect( &Light::Toggle, &L2 );
    B.changeColor.connect( &Light::setColor, &L1 );
    B.changeColor.connect( &Light::setColor, &L2 );
    B.range.connect( &Light::setRange, &L1 );
    B.toggle(); // toggles all connected lights
    B.changeColor( 17 );
    B.toggle();
    B.range( 17.f, 19.f );

    std::cout << "--- Disconnect Test ---" << std::endl;

    B.toggle.disconnect();
    B.toggle.connect( &Light::Toggle, &L2 );
    B.toggle();    

    std::cout << "--- DirectWire Test ---" << std::endl;

    Light  L3;
    Light  L4;
    enum { PORT0, PORT1 };

    B.direct_toggle.connect( PORT0, &Light::Toggle, &L3 );
    B.direct_toggle.connect( PORT1, &Light::Toggle, &L4 );
    B.direct_ChangeColor.connect( PORT0, &Light::setColor, &L3 );
    B.direct_ChangeColor.connect( PORT1, &Light::setColor, &L4 );
    B.direct_toggle( PORT0 ); // toggles only light connected to PORT0
    B.direct_toggle( PORT1 );
    B.direct_ChangeColor( PORT0, 1337 );
    B.direct_ChangeColor( PORT1, 42 );
    B.direct_toggle( PORT0 );
    B.direct_toggle( PORT1 );

    std::cout << "--- End Test ---" << std::endl;

    return 0;
}