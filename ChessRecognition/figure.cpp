#include "figure.h"

Figure::Figure(PieceName name, Team team)
{
    this->name = name;
    this->team = team;
}

void Figure::setName(PieceName name)
{
    this->name = name;
}

bool Figure::isFigureInThisField()
{
    //std::cout<<name<<std::endl;
    if(name != empty) return true;
    else return false;
}
