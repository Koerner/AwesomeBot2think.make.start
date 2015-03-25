#ifndef DXLTEST_H
#define DXLTEST_H

#include <QObject>

class DxlCon;

class DxlTest : public QObject
{
    Q_OBJECT
public:
    explicit DxlTest(DxlCon* dxlCon);

public slots:
    void run();

private:
    DxlCon* dxlCon;
};
#endif // DXLTEST_H
