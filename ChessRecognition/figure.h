#ifndef FIGURE_H
#define FIGURE_H

enum PieceName{
    queen = 0,
    king = 1,
    pown = 2,
    rug = 3,
    bishop = 4,
    horse = 5,
    empty = 6,
    unidentified = 7
};

enum Team{
    black = 0,
    white = 1,
    none
};

class Figure
{
public:
    Figure(PieceName name, Team team);
private:
    PieceName name;
    Team team;
};

#endif // FIGURE_H
