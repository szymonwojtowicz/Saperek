#ifndef STANY_H
#define STANY_H

class Stany
{
public:
    Stany();

    int pobierzStan();
    void zmienStan();
    void zeruj();

    void zamroz();
private:
    int stan;

};

#endif // STANY_H
