#include "gamepad.h"

#include <QtCore>

const double Gamepad::m_speedScaling = 0.0025;
const double Gamepad::m_rotationScaling = 0.01;
const double Gamepad::m_joystickThreshold = 15;
const double Gamepad::m_viewScaling = 1;

Gamepad::Gamepad(QObject *parent) :
    QObject(parent)
{

    m_controllerIndex = 0;
    sf::Joystick::update();
    QTimer *timer = new QTimer(this);
    timer->start(20);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));

}

void Gamepad::run()
{

    QCoreApplication::processEvents();
    sf::Joystick::update();

    if(sf::Joystick::isButtonPressed(m_controllerIndex,BTN_A) == true)
    {

        double joystick_y = sf::Joystick::getAxisPosition(0,sf::Joystick::Y);
        double joystick_x = sf::Joystick::getAxisPosition(0,sf::Joystick::X);

        if(fabs(joystick_x) < m_joystickThreshold) {joystick_x = 0;}
        if(fabs(joystick_y) < m_joystickThreshold) {joystick_y = 0;}

        Q_SIGNAL setCarLike(m_speedScaling * -1 * joystick_y, -1 * m_speedScaling * joystick_x, m_rotationScaling * (sf::Joystick::getAxisPosition(0,sf::Joystick::Z)- sf::Joystick::getAxisPosition(0,sf::Joystick::R)));
    }
    else
    {
        Q_SIGNAL setCarLike(0,0,0);
    }

    if(sf::Joystick::isButtonPressed(m_controllerIndex,BTN_B) == true)
    {
        double joystick_u = sf::Joystick::getAxisPosition(0,sf::Joystick::U);
        double joystick_v = sf::Joystick::getAxisPosition(0,sf::Joystick::V);

        if(fabs(joystick_u) < m_joystickThreshold) {joystick_u = 0;}
        if(fabs(joystick_v) < m_joystickThreshold) {joystick_v = 0;}


        Q_SIGNAL setView(joystick_u,0,joystick_v);
        //Q_SIGNAL setView(-1*m_viewScaling*joystick_u,-1*m_viewScaling*joystick_v,0);
    }
    else
    {
        //Q_SIGNAL setView(0,0,0);
    }

    //        Q_SIGNAL joystick_left(sf::Joystick::getAxisPosition(0,sf::Joystick::X), -1 * sf::Joystick::getAxisPosition(0,sf::Joystick::Y));
    //        Q_SIGNAL joystick_left(sf::Joystick::getAxisPosition(0,sf::Joystick::U), -1 * sf::Joystick::getAxisPosition(0,sf::Joystick::V));
    //        Q_SIGNAL pedal(sf::Joystick::getAxisPosition(0,sf::Joystick::Z),sf::Joystick::getAxisPosition(0,sf::Joystick::R));


    //QThread::msleep(20);

    //}

    return;
}
