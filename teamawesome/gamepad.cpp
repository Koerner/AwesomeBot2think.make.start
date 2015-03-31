#include "gamepad.h"

#include <QtCore>

const double Gamepad::m_speedScaling = 0.0025;
const double Gamepad::m_rotationScaling = 0.01;
const double Gamepad::m_interactionScaling = 0.3;
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

        double joystick_u = sf::Joystick::getAxisPosition(0,sf::Joystick::U);
        double joystick_v = sf::Joystick::getAxisPosition(0,sf::Joystick::V);

        if(fabs(joystick_x) < m_joystickThreshold) {joystick_x = 0;}
        if(fabs(joystick_y) < m_joystickThreshold) {joystick_y = 0;}

        if(fabs(joystick_u) < m_joystickThreshold) {joystick_u = 0;}
        if(fabs(joystick_v) < m_joystickThreshold) {joystick_v = 0;}


        //Q_SIGNAL setInteraction(-1*m_interactionScaling*joystick_u,-1*m_interactionScaling*joystick_v);
        Q_SIGNAL setCarLike(-1 * m_speedScaling * joystick_y, -1 * m_speedScaling * joystick_x, m_rotationScaling * (sf::Joystick::getAxisPosition(0,sf::Joystick::Z)- sf::Joystick::getAxisPosition(0,sf::Joystick::R)));
    }
    else
    {
        //Q_SIGNAL setInteraction(0,0);
        Q_SIGNAL setCarLike(0,0,0);
    }

    if(sf::Joystick::isButtonPressed(m_controllerIndex, BTN_RB) == true)
    {
        //std::cout << "SHOOT" << std::endl;
        Q_SIGNAL setTrig();
    }

    if(sf::Joystick::isButtonPressed(m_controllerIndex, BTN_X) == true)
    {
        Q_SIGNAL startNerf(100);
    }
    else
    {
        Q_SIGNAL startNerf(20);
    }

    Q_SIGNAL buttonB(sf::Joystick::isButtonPressed(m_controllerIndex,BTN_B));

    if(sf::Joystick::isButtonPressed(m_controllerIndex, BTN_AWESOME))
    {
        Q_SIGNAL buttonsAudio(2);
        qDebug() << "button awesome";
    } else if(sf::Joystick::isButtonPressed(m_controllerIndex, BTN_HELLO))
    {
        Q_SIGNAL buttonsAudio(1);
        qDebug() << "button hello";
    } else if(sf::Joystick::isButtonPressed(m_controllerIndex, BTN_BACK))
    {
        Q_SIGNAL buttonsAudio(3);
        qDebug() << "button back";
    } else if(sf::Joystick::isButtonPressed(m_controllerIndex, BTN_EAT) && sf::Joystick::isButtonPressed(m_controllerIndex, BTN_Y))
    {
        Q_SIGNAL buttonsAudio(4);
        qDebug() << "button eat";
    }

    return;
}
