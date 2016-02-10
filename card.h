#ifndef CARD_H
#define CARD_H


class Card
{
public:
    Card();
    int id;
    int elementNumber;
    int amount;
    enum type {
        ADD,
        REMOVE,
        STOP
    };
};

#endif // CARD_H
