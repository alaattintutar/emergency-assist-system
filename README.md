# Emergency Assist System

A C++ application simulating an emergency response logic system. It manages incoming distress or assistance requests and coordinates response teams through queues and stacks.

## Key Logic & Components
- **Quake Assist Controller (`QuakeAssistController.h`):** The main interface bridging requests to the response units.
- **Request Queue (`RequestQueue.h`):** Stores and schedules incoming distress calls.
- **Mission Stack (`MissionStack.h`):** Handles actively engaged or structured missions.
- **Response Teams (`Team.h`):** Models the available emergency rescue/assist groups.
- Includes a handy `run.sh` script to quickly compile and start the testing environment.
