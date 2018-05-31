#include "Task.h"
#include "Agent.h"
#include "Box.h"
#include <iostream>

bool Task::isCompleted(Agent * a, Node * n)
{
  return false;
}
bool Task::seemsCompleted(Agent * a, Node * n)
{
  return false;
}

int Task::h(Agent * a, Node * n)
{
  return 0;
}
