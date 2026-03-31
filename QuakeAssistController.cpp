#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {

    if (line.empty()) return true;

    std::stringstream sline(line);
    std::string command;
    sline >> command;

    if (command == "INIT_TEAMS") {
        int numTeams;
        sline >> numTeams;
        initializeTeams(numTeams);
    }
    else if (command == "SET_TEAM_CAPACITY") {
        int teamId, capacity;
        sline >> teamId >> capacity;
        handleSetTeamCapacity(teamId, capacity);
    }
    else if (command == "ADD_SUPPLY") {
        std::string id, city, supplyType;
        int amount, emergencyLevel;
        sline >> id >> city >> supplyType >> amount >> emergencyLevel;
        handleAddSupply(id, city, supplyType, amount, emergencyLevel);
    }
    else if (command == "ADD_RESCUE") {
        std::string id, city, risk;
        int numPeople, emergencyLevel;
        sline >> id >> city >> numPeople >> risk >> emergencyLevel;
        handleAddRescue(id, city, numPeople, risk, emergencyLevel);
    }
    else if (command == "REMOVE_REQUEST") {
        std::string id;
        sline >> id;
        handleRemoveRequest(id);
    }
    else if (command == "HANDLE_EMERGENCY") {
        int teamId, k;
        sline >> teamId >> k;
        handleHandleEmergency(teamId, k);
    }
    else if (command == "DISPATCH_TEAM") {
        int teamId;
        sline >> teamId;
        handleDispatchTeam(teamId);
    }
    else if (command == "PRINT_QUEUES") {
        printQueues();
    }
    else if (command == "PRINT_TEAM") {
        int teamId;
        sline >> teamId;
        printTeam(teamId);
    }
    else if (command == "CLEAR") {
        clear();
        return false;
    }
    else {
        std::cout << "Error: Unknown command '" << command << "'." << std::endl;
        return false;
    }

    return true;
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
        if (teams != nullptr) {
            delete[] teams;
        }
        teamCount = numTeams;
        teams = new (std::nothrow) Team[numTeams];

        if (teams == nullptr) {
            return false;
        }
        for (int i = 0; i < numTeams; i++) {
            teams[i].setId(i);
        }
    std::cout << "Initialized " << numTeams << " teams." << std::endl;

        return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.
    if (teams != nullptr && teamId >= 0 && teamId < teamCount) {
        return teamId;
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    teams[teamId].setMaxLoadCapacity(capacity);
    std::cout << "Team " << teamId << " capacity set to " << capacity << "." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    Request req(id, cityStr, supplyTypeStr, amount, emergencyLevel);
    supplyQueue.enqueue(req);
    std::cout << "Request " << id << " added to SUPPLY queue." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request req(id, cityStr, numPeople, riskStr, emergencyLevel);
    rescueQueue.enqueue(req);
    std::cout << "Request " << id << " added to RESCUE queue." << std::endl;
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    bool removed = false;

    if (id[0] == 'S') {
        removed = supplyQueue.removeById(id);
    }
    else if (id[0] == 'R') {
        removed = rescueQueue.removeById(id);
    }
    if (removed) {
        std::cout << "Request " << id << " removed from queues." << std::endl;
    } else {
        std::cout << "Error: Request " << id << " not found." << std::endl;
    }
    return true;
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.
    Team& team = teams[teamId];
    int sCount = 0, rCount = 0, workload = 0;
    bool overload = false;
    for (int i = 0; i < k; i++) {
        if (supplyQueue.isEmpty() && !rescueQueue.isEmpty()) {
            Request req = rescueQueue.getData()[rescueQueue.getFrontIndex()];
            overload = !team.tryAssignRequest(req);
            if (overload) {break;}
            rescueQueue.dequeue(req);
            rCount++;
            workload += req.computeWorkloadContribution();
        }
        else if (rescueQueue.isEmpty() && !supplyQueue.isEmpty()) {
        Request req = supplyQueue.getData()[supplyQueue.getFrontIndex()];
        overload = !team.tryAssignRequest(req);
        if (overload) {break;}
        supplyQueue.dequeue(req);
        sCount++;
        workload += req.computeWorkloadContribution();
        }
        else if (rescueQueue.isEmpty() && supplyQueue.isEmpty()) {
            break;
        }
        else {
            Request sFirst = supplyQueue.getData()[supplyQueue.getFrontIndex()];
            Request rFirst = rescueQueue.getData()[rescueQueue.getFrontIndex()];
            if (sFirst.computeEmergencyScore() > rFirst.computeEmergencyScore()) {
                overload = !team.tryAssignRequest(sFirst);
                if (overload) {break;}
                supplyQueue.dequeue(sFirst);
                sCount++;
                workload += sFirst.computeWorkloadContribution();
            }
            else{
                overload = !team.tryAssignRequest(rFirst);
                if (overload){break;}
                rescueQueue.dequeue(rFirst);
                rCount++;
                workload += rFirst.computeWorkloadContribution();
            }
        }

    }
    if (!overload) {
        std::cout << "Team " << teamId << " assigned " << k << " requests (" <<
            sCount << " SUPPLY, " << rCount << " RESCUE), total workload " << workload << "." << std::endl;
    }
    else{
        teams[teamId].rollbackMission(supplyQueue, rescueQueue);
        std::cout << "Overload on Team " << teamId << ": rolling back mission." << std::endl;
    }
    return true;
}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.
    std::cout << "SUPPLY QUEUE:" << std::endl;
    if (!supplyQueue.isEmpty()){
        for (int i = 0; i < supplyQueue.getCount(); i++) {
            int j = (supplyQueue.getFrontIndex() + i) % supplyQueue.getCapacity();
            std::cout << supplyQueue.getData()[j].getId() << " "
                <<supplyQueue.getData()[j].getCity() << " "
                    <<supplyQueue.getData()[j].getSupplyType() << " "
                        <<supplyQueue.getData()[j].getAmount() << " "
                         <<supplyQueue.getData()[j].getEmergencyLevel() << std::endl;
        }
    }
    std::cout << "RESCUE QUEUE:" << std::endl;
    if (!rescueQueue.isEmpty()) {
        for (int i = 0; i < rescueQueue.getCount(); i++) {
            int j = (rescueQueue.getFrontIndex() + i) % rescueQueue.getCapacity();
            std::cout << rescueQueue.getData()[j].getId() << " "
                <<rescueQueue.getData()[j].getCity() << " "
                    <<rescueQueue.getData()[j].getNumPeople() << " "
                        <<rescueQueue.getData()[j].getRisk() << " "
                            <<rescueQueue.getData()[j].getEmergencyLevel() << std::endl;
        }
    }
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.
    std::cout << "TEAM " << teamId << " STACK:" << std::endl;
    if (findTeamIndexById(teamId) != -1){
        for (int i = teams[teamId].getMissionStack().getTopIndex(); 0 <= i; i-- ) {
            Request r = teams[teamId].getMissionStack().getData()[i];
            if (r.getType() == "RESCUE") {
                std::cout << r.getId() << " " << r.getCity() << " " << r.getNumPeople() << " " <<
                    r.getRisk() << " " << r.getEmergencyLevel() << std::endl;
            }
            else if (r.getType() == "SUPPLY") {
                std::cout << r.getId() << " " << r.getCity() << " " << r.getSupplyType() << " " <<
                    r.getAmount() << " " << r.getEmergencyLevel() << std::endl;
            }
        }
    }
}

void QuakeAssistController::clear() {
    //Clear data.
    if (teams != nullptr) {
        for (int i = 0; i < teamCount; i++) {
            teams[i].clearMission();
        }
    }
    supplyQueue.clear();
    rescueQueue.clear();
    std::cout << "System cleared." << std::endl;
}
