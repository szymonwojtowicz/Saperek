#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QLCDNumber>
#include <QTimer>
#include <QLabel>
#include <QtCore>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include "stany.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int rozmiar;
    int miny;
    int poziom;
    int roznicaCzasu;

    QMediaPlayer player;
    
    void setSpecialStyle(QString background, QString color, QString add_qss);
    bool pressed() const;
    void setPressed(bool arg);
    QString background;

private:
    void mouseReleaseEvent(QMouseEvent * ev);

 QStandardItemModel model;
    bool opened;
    int status;
    bool entered;
bool muzyka;

    Ui::MainWindow *ui;
    void losujMiny(int ile);
    QList <QPoint> minyLista;
    QList <QPoint> listaRekurencji;
    QToolButton *przycisk[14][14];
    Stany stany[14][14];
    bool poleRozminowane();
    int releaseX, releaseY;

    bool ifMina(int rzad, int kolumna);
    void odslaniajZera(int rzad, int kolumna);
    int ileMinObok(int rzad, int kolumna);
    QSpacerItem *spacer;
    QLabel liczbaMin;
    QLabel czasPozostaly;
    QLCDNumber czas;
    QLCDNumber liczbaFlag;
    QToolButton usmiech;
    QIcon ikona;
    QIcon flaga;
    QTimer *timer;
    int licznikCzasu;
    QLabel komunikat;
    bool pierwszyRaz;
    bool blokadaCzasu;
    int czasBlokady;

    void generujPlansze();
    void hidePlansze();
    QTimer timerHandler;

    void zmianaStany(int i, int j);
    void pokarzRanking();
public slots:
    void blokujPlansze(bool usuwaj = true);

    void noweRozdanie();
    void czasomierz();
    void ustalPoziom(int poziom);

    void kladzFlage();
protected slots:
    void pressField();
    void releasedField();
private slots:
    void on_subtractButton_clicked();
    void on_addSize_clicked();
    void on_pushButton_7_clicked();
    void on_powrotButton_clicked();
    void on_pushButton_2_clicked();
    void on_dzwiektb_clicked();
    void on_zwiekszPoziomB_clicked();
    void on_zmiejszPoziomB_clicked();

    void on_menub_clicked();
    void on_pushButton_clicked();
    void on_toolButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_11_clicked();
    void on_zatwierdzZwycieskieImie_clicked();
    void on_increaseRank_clicked();
    void on_decreseRank_clicked();
    void on_zwiekszPoziomRankingB_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_5_clicked();
};

#endif // MAINWINDOW_H
