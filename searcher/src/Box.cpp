
#include "Box.h"
#include <string>
#include <utility>

Box::Box(char chr, Location location, Entity::COLOR color, int region, int id) :
Entity(chr, location, color, region, id)
{
}

Box::Box(char chr, Location location, int region, int id) :
Entity(chr, location, Entity::BLUE, region, id)
{
}

Box::Box(Box * box) :
Entity(box->getChar(), box->getLocation(), box->getColor(), box->getRegion(), box->getID())
{
}

int Box::hashCode()
{
    int result = 17;
    result = 31 * result + (int) (chr);
    result = 31 * result + location.getX();
    result = 31 * result + location.getY();
    result = 31 * result + color;
    return result;
}

bool Box::equals(const Box * box) const
{
    if (box == this) return true;
    return box->chr == chr && box->color == color && box->location == location;
}
