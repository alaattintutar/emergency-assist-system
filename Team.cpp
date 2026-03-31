#include "Team.h"

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    //Implement tryAssignRequest function as explained in the PDF.
    if (currentWorkload + req.computeWorkloadContribution() > maxLoadCapacity) {
        return false;
    }
    currentWorkload += req.computeWorkloadContribution();
    missionStack.push(req);
    return true;
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //Implement rollbackMission function as explained in the PDF.

    for (int i = 0; i < missionStack.size(); i++) {
        if (missionStack.getData()[i].getType() == "SUPPLY") {
            supplyQueue.enqueue(missionStack.getData()[i]);
        }
        else if (missionStack.getData()[i].getType() == "RESCUE") {
            rescueQueue.enqueue(missionStack.getData()[i]);
        }
    }
    clearMission();
}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}
