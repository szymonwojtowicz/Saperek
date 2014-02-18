#ifndef POLE_H
#define POLE_H

#include <QWidget>
#include <QToolButton>
#include <QSpacerItem>
#include <QDebug>
#include <QTime>

namespace Ui {
class Pole;
}

class Pole : public QWidget
{
    Q_OBJECT
    
public:
    explicit Pole(QWidget *parent = 0);
    ~Pole();
public slots:
    void wcisniety();
private:
    int rozmiar;
    Ui::Pole *ui;

    void losujMiny(int ile);
    QList <QPoint> minyLista;
    QList <QPoint> listaRekurencji;
    QToolButton *przycisk[16][30];
    bool ifMina(int rzad, int kolumna);
    void odslaniajZera(int rzad, int kolumna);
    int ileMinObok(int rzad, int kolumna);
    QSpacerItem *spacer;
    void blokujPlansze();
signals:
    void wymiarujPole(QRect);
};

#endif // POLE_H
