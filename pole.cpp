#include "pole.h"
#include "ui_pole.h"

Pole::Pole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pole)
{
    ui->setupUi(this);
    rozmiar = 15;
    ui->plansza->setSpacing(0);
    ui->plansza->setVerticalSpacing(0);
    ui->plansza->setHorizontalSpacing(0);
    ui->plansza->setMargin(0);

    losujMiny(10);

    for(int i=1;i < rozmiar; i++)
        for(int j=0; j < rozmiar; j++)
        {
            przycisk[i][j] = new QToolButton;
            //  przycisk[i][j]->setStyleSheet("margin:0px;");
            przycisk[i][j]->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

            connect(przycisk[i][j],SIGNAL(clicked()), this, SLOT(wcisniety()));
            ui->plansza->addWidget(przycisk[i][j],i,j);

        }
    QRect wymiar( przycisk[rozmiar][rozmiar]->geometry());
   // wymiar.setWidth( przycisk[rozmiar][rozmiar]->pos().rx());
  //   emit wymiarujPole(wymiar);
}

void Pole::wcisniety()
{
    int iloscMin = 0;
    for(int i=0; i < rozmiar; i++)
        for(int j=0; j < rozmiar; j++)
            if(sender() == przycisk[i][j])
            {
                qDebug() << "Wcisłeś" << i <<" "<< j << "Długość listy wynosi" << minyLista.length();
                if(ifMina(i,j))
                {
                    przycisk[i][j]->setText("#");
                    // przycisk[i][j]->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
                    przycisk[i][j]->setStyleSheet("background: red");
                    blokujPlansze();
                    return;
                }
                else
                {
                    iloscMin = ileMinObok(i,j);
                    przycisk[i][j]->setStyleSheet("background: white");
                    przycisk[i][j]->setText(QString::number(iloscMin));
                }
                if(iloscMin == 0)
                    odslaniajZera(i,j);
                break;
            }
}

Pole::~Pole()
{
    delete ui;
}

void Pole::losujMiny(int ile)
{
    if(!minyLista.isEmpty())
        minyLista.clear();
    bool wystapil;
    QPoint tempPoint;
    while(minyLista.length()<ile)
    {
        wystapil = true;
        qsrand(QTime::currentTime().msec());

        tempPoint.setX(qrand()%this->rozmiar);
        tempPoint.setY(qrand()%this->rozmiar);
        if(minyLista.contains(tempPoint))
            wystapil = false;

        if(wystapil)
            minyLista.append(tempPoint);
    }
}

bool Pole::ifMina(int rzad, int kolumna)
{
    QPoint tempPoint;
    tempPoint.setX(rzad);
    tempPoint.setY(kolumna);
    if(minyLista.contains(tempPoint))
        return true;
    return false;
}

void Pole::odslaniajZera(int rzad, int kolumna)
{
    QPoint tempPoint;
    tempPoint.setX(rzad);
    tempPoint.setY(kolumna);
    if(!listaRekurencji.contains(tempPoint))
        listaRekurencji.append(tempPoint);
    else
        return;
    for(int w = rzad-1; w < rzad+2; w++)
        for(int h = kolumna-1; h < kolumna+2; h++)
        {
            if(w < rozmiar && h < rozmiar && w>0 && h>=0)
            {
                if(rzad != w && kolumna != h)
                    if(ileMinObok(w,h) == 0)
                    {
                        qDebug() << "Odsłania" << w << h;
                        odslaniajZera(w, h);
                    }
                przycisk[w][h]->click();
            }
        }
}


int Pole::ileMinObok(int rzad, int kolumna)
{
    int iloscMin = 0;
    for(int w = rzad-1; w < rzad+2; w++)
        for(int h = kolumna-1; h < kolumna+2; h++)
            if(w < rozmiar && h < rozmiar && w >=0 && h >= 0)
                if(ifMina(w,h))
                    iloscMin++;
    return iloscMin;
}


void Pole::blokujPlansze()
{
    //   for(int i=0; i < rozmiar; i++)
    //   for(int j=0; j < rozmiar; j++)
    //      if(!przycisk[i][j]->text().isEmpty())
    //     disconnect(przycisk[i][j],SIGNAL(clicked()), this, SLOT(wcisniety()));
}


