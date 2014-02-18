#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    this->pierwszyRaz = false;
    roznicaCzasu = 0;
    muzyka = true;
    czasBlokady = 0;
    timerHandler.setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(czasomierz()));
     connect(&timerHandler,SIGNAL(timeout()),this,SLOT(kladzFlage()));  // zmien stan

    ui->stackedWidget->setCurrentIndex(1);
    rozmiar = 10;
    poziom = 3;
    miny = 10;
    player.setMedia(QUrl::fromLocalFile("/home/szymon/projekty/Qt/Saperek/music/Mark_Neil_-_11_strANGE_Ls.mp3"));
    player.play();
    blokadaCzasu = false;

    ikona.addPixmap(QPixmap(":/Ikony/Icon/Smile.png"));
    flaga.addPixmap(QPixmap(":/Ikony/Icon/flag.png"));
    usmiech.setIcon(ikona);

    ui->toolBar->hide();
    QLabel liczbaMin;
    QLabel czasPozostaly;
    liczbaMin.setText("Liczba Min");
    czasPozostaly.setText("Czas pozostały");
    QWidget *spacerWidget = new QWidget(this);
    QWidget *spacerWidget2 = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    ui->toolBar->addWidget(&liczbaMin);
    ui->toolBar->addWidget(&liczbaFlag);
    ui->toolBar->addWidget(spacerWidget);
    ui->toolBar->addWidget(&usmiech);
    ui->toolBar->addWidget(spacerWidget2);
    ui->toolBar->addWidget(&czasPozostaly);
    ui->toolBar->addWidget(&czas);



    background = "background: #1a1b1e;";
    ui->toolBar->setStyleSheet(background);

    hidePlansze();

    connect(&usmiech,SIGNAL(clicked()), this, SLOT(blokujPlansze()));

    ui->saper->setSpacing(0);
    ui->saper->setMargin(0);

    liczbaFlag.display(0);
    blokujPlansze(false);
    ui->saper->activate();
}

void MainWindow::kladzFlage()
{
    czasBlokady++;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::losujMiny(int ile)
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

bool MainWindow::ifMina(int rzad, int kolumna)
{
    QPoint tempPoint;
    tempPoint.setX(rzad);
    tempPoint.setY(kolumna);
    if(minyLista.contains(tempPoint))
        return true;
    return false;
}

void MainWindow::odslaniajZera(int rzad, int kolumna)
{
    QPoint tempPoint;
    tempPoint.setX(rzad);
    tempPoint.setY(kolumna);
    if(listaRekurencji.isEmpty())
        listaRekurencji.append(tempPoint);
    else if((!listaRekurencji.contains(tempPoint)))
        listaRekurencji.append(tempPoint);
    else
        return;
    for(int w = rzad-1; w < rzad+2; w++)
        for(int h = kolumna-1; h < kolumna+2; h++)
            if(w < rozmiar && h < rozmiar && w>=0 && h>=0)
            {
                if(rzad != w && kolumna != h)
                    if(ileMinObok(w,h) == 0)
                        odslaniajZera(w, h);

              //  qDebug() << "zginął przy: " << w << " " << h;
                przycisk[w][h]->click();
            }
}

int MainWindow::ileMinObok(int rzad, int kolumna)
{
    int iloscMin = 0;
    for(int w = rzad-1; w < rzad+2; w++)
        for(int h = kolumna-1; h < kolumna+2; h++)
            if(w < rozmiar && h < rozmiar && w >=0 && h >= 0)
                if(ifMina(w,h))
                    iloscMin++;
    return iloscMin;
}


void MainWindow::blokujPlansze(bool usuwaj)
{
    ui->stackedWidget->setCurrentIndex(1);
    timer->stop();

    if(usuwaj)
        for(int i=0; i < rozmiar; i++)
            for(int j=0; j < rozmiar; j++)
                delete przycisk[i][j];

    this->pierwszyRaz = false;
}

void MainWindow::noweRozdanie()
{
    listaRekurencji.clear();
    losujMiny(miny);
    generujPlansze();
    timer->stop();
    czas.display(0);
}

void MainWindow::generujPlansze()
{
    for(int i=0;i < rozmiar; i++)
        for(int j=0; j < rozmiar; j++)
        {
            if(this->pierwszyRaz)
                delete przycisk[i][j];

            przycisk[i][j] = new QToolButton;
            przycisk[i][j]->setStyleSheet("background: blue; border: 1px solid black; border-radius: 5px; background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #0adb0f, stop: 1 #09801b);");
            przycisk[i][j]->setGeometry(0,0,ui->centralWidget->width()/rozmiar,(ui->centralWidget->height()-ui->toolBar->height())/rozmiar);

            stany[i][j].zeruj();
            przycisk[i][j]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            connect(przycisk[i][j],SIGNAL(pressed()), this, SLOT(pressField()));
            connect(przycisk[i][j],SIGNAL(released()), this, SLOT(releasedField()));

            ui->saper->addWidget(przycisk[i][j],i,j);

        }
    this->pierwszyRaz = true;
    licznikCzasu = 0;
}

void MainWindow::czasomierz()
{
    czas.display(licznikCzasu++);
}

void MainWindow::hidePlansze()
{
    if(!listaRekurencji.isEmpty())
        listaRekurencji.clear();
    timer->start(1000);
}

void MainWindow::pressField()
{
    timerHandler.stop();
     timerHandler.start();
}

void MainWindow::releasedField()
{
qDebug() << "Release" << czasBlokady;
timer->start(1000);
int iloscMin = 0;
for(int i=0; i < rozmiar; i++)
    for(int j=0; j < rozmiar; j++)
        if(sender() == przycisk[i][j])
        {
            qDebug() << "Czas" << timerHandler.remainingTime() << QString::number(czasBlokady);

           if(czasBlokady > 10)
            {
                zmianaStany(i,j);
                czasBlokady =0;
                liczbaFlag.display(--miny);
        //        timerHandler.stop();
                return;
            }
             czasBlokady = 0;

            if(stany[i][j].pobierzStan()==1)  // jeżeli pole jest oznaczone flagą to nic nie rób
                return;
            else if(stany[i][j].pobierzStan()==2)   // jeżeli pytajnik to usuń ikone pytajnika i zmieniaj stan
            {
                stany[i][j].zmienStan();
                QIcon ikona;
                przycisk[i][j]->setIcon(ikona);
            }
          //  qDebug() << "Wcisłeś" << i <<" "<< j << "Długość listy wynosi" << minyLista.length();
            if(ifMina(i,j))
            {
                timer->stop();
                QIcon bomba;
                bomba.addFile(":/Ikony/Icon/mina.png");

                foreach(QPoint punkt, minyLista)
                {
                    przycisk[punkt.x()][punkt.y()]->setIcon(bomba);
                    przycisk[punkt.x()][punkt.y()]->setStyleSheet("background: red");
                }
                for(int i=0; i < rozmiar; i++)
                    for(int j=0; j < rozmiar; j++)
                        przycisk[i][j]->blockSignals(true);
                czasBlokady =0;
                timerHandler.stop();
                return;
            }
            else
            {
                iloscMin = ileMinObok(i,j);

                if(iloscMin == 1)
                    przycisk[i][j]->setStyleSheet(background + "color: yellow; font-weight:bold;");
                else if(iloscMin == 2)
                    przycisk[i][j]->setStyleSheet(background +"color: green; font-weight:bold;");
                else if(iloscMin >= 3)
                    przycisk[i][j]->setStyleSheet(background +"color: red; font-weight:bold;");
                else
                    przycisk[i][j]->setStyleSheet(background);

                if(iloscMin > 0)
                    przycisk[i][j]->setText(QString::number(iloscMin));

                przycisk[i][j]->blockSignals(true);
                if(poleRozminowane())
                    ui->stackedWidget->setCurrentIndex(5);
                czasBlokady =0;
                timerHandler.stop();
            }
            if(iloscMin == 0)
                odslaniajZera(i,j);
            break;
        }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev )
{
    timerHandler.stop();
    czasBlokady = 0;
    if(ev->button() == Qt::RightButton)
    {
        int x = ev->x();
        int y = ev->y();

        for(int i=0; i < rozmiar; i++)
            for(int j=0; j < rozmiar; j++)
            {
                if(!przycisk[i][j]->signalsBlocked())
                    if( przycisk[i][j]->x() <= x  && (przycisk[i][j]->x() + przycisk[i][j]->width()) >= x)
                        if( przycisk[i][j]->y() <= y  &&  (przycisk[i][j]->y()  + przycisk[i][j]->height() + ui->toolBar->height())  >= y)
                        {

                            zmianaStany(i,j);
                            if(stany[i][j].pobierzStan() == 1)
                                liczbaFlag.display(--miny);
                            else if(stany[i][j].pobierzStan() == 2)
                                liczbaFlag.display(++miny);

                            if(poleRozminowane())
                                ui->stackedWidget->setCurrentIndex(5);
                            return;
                        }
            }
        return;
    }
}

bool MainWindow::poleRozminowane()
{
    foreach(QPoint punkt, minyLista)
    {
        if(stany[punkt.x()][punkt.y()].pobierzStan() != 1)
            return false;
    }
    for(int i =0 ; i < rozmiar; i++)
        for(int j = 0 ; j < rozmiar; j++)
            if(!przycisk[i][j]->signalsBlocked() && stany[i][j].pobierzStan() != 1)
                return false;

    timer->stop();
    return true;
}

void MainWindow::zmianaStany(int i, int j)
{
    czasBlokady = 0;
    timerHandler.stop();
    QIcon flaga;
    stany[i][j].zmienStan();

    if(stany[i][j].pobierzStan()==1)
        flaga.addFile(":/Ikony/Icon/flag.png");
    else if(stany[i][j].pobierzStan()==2)
        flaga.addFile(":/Ikony/Icon/pytajnik.png");

    przycisk[i][j]->setIcon(flaga);
    if( stany[i][j].pobierzStan()==1 || stany[i][j].pobierzStan()==2 )
        przycisk[i][j]->setStyleSheet("background: yellow; border: 1px solid black; border-radius: 5px; ");
    else
        przycisk[i][j]->setStyleSheet("background: blue; border: 1px solid black; border-radius: 5px; background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #0adb0f, stop: 1 #09801b);");
}

void MainWindow::on_subtractButton_clicked()
{
    if(rozmiar > 8)
        rozmiar--;
    ui->siezButton->setText(QString::number(rozmiar) + "x" + QString::number(rozmiar));
}

void MainWindow::on_addSize_clicked()
{
    if(rozmiar < 14)
        rozmiar++;
    ui->siezButton->setText(QString::number(rozmiar) + "x" + QString::number(rozmiar));
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_powrotButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_dzwiektb_clicked()
{
    player.setVolume(100);

    QIcon ikonaMuzyka;
    if(muzyka)
    {
        ikonaMuzyka.addFile(":/Ikony/Icon/mute.png");
        muzyka =false;
        player.stop();
    }
    else
    {
        ikonaMuzyka.addFile(":/Ikony/Icon/sounds.png");
        muzyka = true;
        player.play();
    }
    ui->dzwiektb->setIcon(ikonaMuzyka);
}


void MainWindow::on_zwiekszPoziomB_clicked()
{
    if(poziom<5)
        ustalPoziom(++poziom);
}

void MainWindow::on_zmiejszPoziomB_clicked()
{
    if(poziom>1)
        ustalPoziom(--poziom);
}

void MainWindow::ustalPoziom(int poziom)
{
    if(poziom==1)
        ui->poziomB->setText("prosty");
    else if(poziom == 2)
        ui->poziomB->setText("łatwy");
    else if(poziom == 3)
        ui->poziomB->setText("średni");
    else if(poziom == 4)
        ui->poziomB->setText("trudny");
    else if(poziom == 5)
        ui->poziomB->setText("wojenny");
    ui->poziomRankingB->setText(ui->poziomB->text());
}

void MainWindow::on_menub_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    pokarzRanking();
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_toolButton_clicked()
{

    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_3_clicked()
{
     ui->toolBar->show();
    ui->stackedWidget->setCurrentIndex(0);
    miny = rozmiar + 5 * poziom;
    liczbaFlag.display(miny);
    noweRozdanie();
}

void MainWindow::on_pushButton_4_clicked()
{
    close();
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_zatwierdzZwycieskieImie_clicked()
{
    if(ui->imieZwyciezcyLE->text().isEmpty())
    {
        ui->imieZwyciezcyL->setText("Przed zatwierdzeniem podaj imię zwycięzcy");
        ui->imieZwyciezcyL->setStyleSheet("color: red;");
        return;
    }

    QFile file("./wyniki.txt");
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Błąd zapisu";
        return;
    }

    QTextStream out(&file);

    out << poziom << endl;
    out << ui->imieZwyciezcyLE->text() << endl;
    out << rozmiar << endl;
    out << licznikCzasu << endl;

    file.close();
    ui->stackedWidget->setCurrentIndex(2);
    pokarzRanking();
}

void MainWindow::pokarzRanking()
{
    QFile file("./wyniki.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Błąd odczytu";
        return;
    }

    model.clear();

    QTextStream in(&file);

    QList <QStandardItem *> poziomy;
    QList <QStandardItem *> imiona;
    QList <QStandardItem *> rozmiary;
    QList <QStandardItem *> liczniki;

    QStringList poziomyS, imionaS, rozmiaryS, licznikiS;

    int i = 0;
    while(!in.atEnd())
    {
        i++;
         poziomyS.append(in.readLine());
         imionaS.append(in.readLine());
         rozmiaryS.append(in.readLine());
         licznikiS.append(in.readLine());
         if(!licznikiS.isEmpty())
         qDebug() << QString::number(i) << " " << licznikiS.last() ;
    }
 qDebug() << "poziomy: "<< licznikiS.size();

    for(int i =0; i < licznikiS.size() -1; i++)
            for(int j =0; j < licznikiS.size() -1; j++)
        if(licznikiS.at(i).toInt() > licznikiS.at(i+1).toInt())
        {

            poziomyS.swap(i, i+1);
            imionaS.swap(i, i+1);
            rozmiaryS.swap(i, i+1);

             licznikiS.swap(i, i+1);
            qDebug() << "zamiana";
        }
        for(int i =0; i < licznikiS.size(); i++)
            if(rozmiar == rozmiaryS.at(i).toInt() && poziom == poziomyS.at(i).toInt())
            {
                imiona.append(new QStandardItem(imionaS.at(i)));
                liczniki.append(new QStandardItem(licznikiS.at(i)));
            }

    model.appendColumn(imiona);
    model.appendColumn(liczniki);
    ui->rankingPoziom->setText(QString::number(rozmiar) + "x" + QString::number(rozmiar));
    QStringList horzHeaders;
    horzHeaders << "Imie Gracza" << "Czas";
    model.setHorizontalHeaderLabels(horzHeaders);
    model.sort(1);
    ui->tableView->setModel(&model);

    file.close();
}

void MainWindow::on_increaseRank_clicked()
{
    if(rozmiar < 14)
        rozmiar++;
    ui->rankingPoziom->setText(QString::number(rozmiar) + "x" + QString::number(rozmiar));
    ui->siezButton->setText(QString::number(rozmiar) + "x" + QString::number(rozmiar));
    pokarzRanking();
}

void MainWindow::on_decreseRank_clicked()
{
    if(rozmiar > 8)
        rozmiar--;
    ui->rankingPoziom->setText(QString::number(rozmiar) + "x" + QString::number(rozmiar));
    ui->siezButton->setText(QString::number(rozmiar) + "x" + QString::number(rozmiar));
    pokarzRanking();
}

void MainWindow::on_zwiekszPoziomRankingB_clicked()
{
    this->on_zwiekszPoziomB_clicked();
    pokarzRanking();
}

void MainWindow::on_pushButton_13_clicked()
{
    this->on_zmiejszPoziomB_clicked();
    pokarzRanking();
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_pushButton_10_clicked()
{
    QFile file("./wyniki.txt");
    if(ui->pin->text() == "1234")
        if(file.remove())
            ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
