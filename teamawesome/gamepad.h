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

public:
    explicit Gamepad(QObject *parent = 0);

signals:
    void buttonA(bool);
    void buttonB(bool);
    void buttonY(bool);
    void buttonX(bool);

    void joystick_left(double horizontal, double vertical);
    void joystick_right(double horizontal, double vertical);
    void pedal(double left, double right);

    void setCarLike(double vx, double vy, double theta);


public slots:
    void run();


};

#endif // GAMEPAD_H
