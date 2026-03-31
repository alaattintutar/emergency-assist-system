<div align="center">
  
# 🚨 Emergency Assist System

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Data Structures](https://img.shields.io/badge/Queue-Stack-ff69b4?style=for-the-badge)
![System](https://img.shields.io/badge/Simulation-Controller-orange?style=for-the-badge)

*A disaster response algorithm modeling real-time crisis coordination.*

</div>

## 📌 Overview
The **Emergency Assist System** is a highly logical C++ backend designed to manage distress signals, coordinate search-and-rescue teams, and deploy emergency supply lines effectively. Built around customized `Stack` and `Queue` data structures, it prioritizes crises based on calculated *Emergency Scores* and ensures response teams are optimally dispatched without breaching their maximum workload capacities.

## ✨ Core Features
- 🆘 **Priority Queuing:** Independent dynamic queues for standard `SUPPLY` and critical `RESCUE` requests (`RequestQueue`).
- 🚁 **Mission Stacks (LIFO):** Each rescue team operates on a strict `MissionStack`, enabling immediate rollback operations if a crisis overloads a team.
- 📊 **Algorithmic Workload Resolution:** A `QuakeAssistController` algorithmically calculates which emergency call to handle first using mathematical `computeEmergencyScore()` logic.
- 🛑 **Capacity Protection:** Prevents team overloading during disasters; automatically re-queues tasks upon reaching maximum stress indices.

## ⚙️ Technical Architecture
- **Language:** C++
- **Design Pattern:** Controller Pattern (`QuakeAssistController` acts as the single source of truth routing external string commands to internal actions).
- **String Parsing:** Utilizes efficient `std::stringstream` to interpret fast-paced realtime disaster commands flawlessly.

## 🚀 Getting Started
To boot up the emergency operations center:

```bash
# Easy-run using the provided script
sh run.sh

# Or compile manually:
g++ -std=c++11 *.cpp -o EmergencySystem
./EmergencySystem
```
