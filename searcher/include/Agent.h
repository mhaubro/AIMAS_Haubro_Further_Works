#ifndef AGENT_H
#define AGENT_H

#include <utility>
#include <string>

#include "Task.h"
#include "Node.h"
#include "Plan.h"
#include "Entity.h"
#include "CentralPlanner.h"
#include "RequestFreeSpaceTask.h"

class Plan;
class Task;
class RequestFreeSpaceTask;
class CentralPlanner;

class Agent : public Entity
{
  public:
	//Is this the number, or an actual rank. I assume actual rank.
    int rank;
    int number;

    void removeFreeSpaceTaskMessage(RequestFreeSpaceTask * toBeRemoved);

    Command * getAction(Node * startstate, Node * tempstate);



    Task * task;
    std::list<Node *> search(Node * state);
    std::list<Node *> Nakedsearch(Node * state);
    std::list<Node *> noBoxesOrAgents(Node * state, Box * box);

    Agent(char chr, int rank, Location location, Entity::COLOR color, int region, int id);
    Agent(char chr, Location location, Entity::COLOR color, int region, int id);
    Agent(char chr, Location location, int region, int id);
    Agent(const Agent * agt);

    void removeFreeSpaceTask();

    int hashCode();

    bool equals(const Agent * o) const;



    void setMyPlanner(CentralPlanner * planner);
    CentralPlanner * getPlanner() const;

private:
    Command * getAwayFromGoal(Node * startstate);
    void maybeSleep(int sleep);

    void cleanTasks();
    void replanTask(Node * state);
    void noTask(Node * startstate);
    void gettingJob(Node * startstate);

    CentralPlanner * myPlanner;
    RequestFreeSpaceTask * freeSpaceTask;//Can at max request one at a time
    bool workingOnTask = false;//Only set to true, if an agent has started carrying out a task
    int skipNextIte = 0;
    Plan * plan;
    void noPlan(Node * startstate);
    Command * handleConflict(Node * state);
    bool checkForHelp(Node * state);


};
#endif
