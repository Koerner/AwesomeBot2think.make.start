#include "gamepad.h"

const double Gamepad::m_speedScaling = 0.0025;
const double Gamepad::m_rotationScaling = 0.01;
const double Gamepad::m_joystickThreshold = 15;


Gamepad::Gamepad(QObject *parent) :
    QObject(parent)
{
    for(int i=0; i<sf::Joystick::ButtonCount;i++)
    {
        if(sf::Joystick::isConnected(i) == true)
        {
            m_controllerIndex = i;
            break;
        }
    }

    m_controllerIndex = 0;
    sf::Joystick::update();


}

void Gamepad::publish()
{
    while(true)
    {
        Q_SIGNAL buttonA(sf::Joystick::isButtonPressed(m_controllerIndex,0));
        Q_SIGNAL buttonB(sf::Joystick::isButtonPressed(m_controllerIndex,1));
        Q_SIGNAL buttonY(sf::Joystick::isButtonPressed(m_controllerIndex,3));
        Q_SIGNAL buttonX(sf::Joystick::isButtonPressed(m_controllerIndex,2));


        if(sf::Joystick::isButtonPressed(m_controllerIndex,0) == true)
        {
            std::cout << "Button A Pressed" << std::endl;

            double joystick_y = sf::Joystick::getAxisPosition(0,sf::Joystick::Y);
            double joystick_x = sf::Joystick::getAxisPosition(0,sf::Joystick::X);

            if(fabs(joystick_x) < m_joystickThreshold) {joystick_x = 0;}
            if(fabs(joystick_y) < m_joystickThreshold) {joystick_y = 0;}

            std::cout << "Set Car Like (" <<  m_speedScaling * -1 * joystick_y << "mm/s , "<< m_speedScaling * joystick_x  << "mm/s , "<< m_rotationScaling * (sf::Joystick::getAxisPosition(0,sf::Joystick::Z)- sf::Joystick::getAxisPosition(0,sf::Joystick::R))  << "deg/s )" << std::endl;
            Q_SIGNAL setCarLike(m_speedScaling * -1 * joystick_y, -1 * m_speedScaling * joystick_x, m_rotationScaling * (sf::Joystick::getAxisPosition(0,sf::Joystick::Z)- sf::Joystick::getAxisPosition(0,sf::Joystick::R)));
        }
        else
        {
            Q_SIGNAL setCarLike(0,0,0);
        }

//        if(sf::Joystick::isButtonPressed(m_controllerIndex,1) == true)
//        {
//            //std::cout << "Button B Pressed" << std::endl;
//        }
//        if(sf::Joystick::isButtonPressed(m_controllerIndex,3) == true)
//        {
//            //std::cout << "Button Y Pressed" << std::endl;
//        }
//        if(sf::Joystick::isButtonPressed(m_controllerIndex,2) == true)
//        {
//            //std::cout << "Button X Pressed" << std::endl;
//        }

        Q_SIGNAL joystick_left(sf::Joystick::getAxisPosition(0,sf::Joystick::X), -1 * sf::Joystick::getAxisPosition(0,sf::Joystick::Y));
        Q_SIGNAL joystick_left(sf::Joystick::getAxisPosition(0,sf::Joystick::U), -1 * sf::Joystick::getAxisPosition(0,sf::Joystick::V));
        Q_SIGNAL pedal(sf::Joystick::getAxisPosition(0,sf::Joystick::Z),sf::Joystick::getAxisPosition(0,sf::Joystick::R));

        sf::Joystick::update();
    }

    return;
}
