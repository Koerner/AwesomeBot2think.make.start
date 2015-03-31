#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QObject>
#include <iostream>
#include <cmath>
#include <SFML/Window/Joystick.hpp>



class Gamepad : public QObject
{
    Q_OBJECT
private:
    int m_controllerIndex;
    static const double m_rotationScaling;
    static const double m_speedScaling;
    static const double m_joystickThreshold;
    static const double m_viewScaling;
    static const double m_interactionScaling;

public:
    explicit Gamepad(QObject *parent = 0);

    enum Buttons {BTN_A=0,BTN_B=1,BTN_X=2,BTN_Y=3, /*BTN_LB=4,*/ BTN_EAT=4, BTN_RB=5, BTN_BACK=6, BTN_AWESOME=8, BTN_HELLO=7};

signals:
    void buttonA(bool);
    void buttonB(bool);
    void buttonY(bool);
    void buttonX(bool);
    void buttonsAudio(int);

    void joystick_left(double horizontal, double vertical);
    void joystick_right(double horizontal, double vertical);
    void pedal(double left, double right);

    void setCarLike(double vx, double vy, double theta);
    void setInteraction(double yaw, double pitch);
    void setTrig();
    void startNerf(int);

public slots:
    void run();


};

#endif // GAMEPAD_H
