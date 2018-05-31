#ifndef BOX_H
#define BOX_H

#include <utility>
#include <string>
#include "Entity.h"

class Box : public Entity
{
public:
    Box(char chr, Location location, Entity::COLOR color, int region, int id);
    Box(char chr, Location location, int region, int id);
    Box(Box * box);
    int hashCode();
    bool equals(const Box * box) const;


    bool workInProgress = false;

  private:

};
#endif
