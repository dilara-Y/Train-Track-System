#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal()
{
// TODO: Initialize each track in the departure yard.
// Each TrainTrack corresponds to one Destination.
for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
departureYard[i] = TrainTrack(static_cast<Destination>(i));
 }
}

RailMarshal::~RailMarshal()
{
    
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
        while (!departureYard[i].isEmpty()) {
            Train* departed = departureYard[i].departTrain();
            if (departed) delete departed;
        }
    }
    
    classificationYard.clear();
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
int idx = static_cast<int>(dest);
 return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
std::istringstream iss(line);
std::string cmd;
 iss >> cmd;

if (cmd == "ADD_WAGON") {
 int id, weight, maxCoupler;
std::string cargoStr, destStr;

if (iss >> id >> cargoStr >> destStr >> weight >> maxCoupler) {
CargoType cargo = parseCargo(cargoStr);
Destination dest = parseDestination(destStr);

Wagon* w = new Wagon(id, cargo, dest, weight, maxCoupler);
classificationYard.insertWagon(w);
std::cout << "Wagon " << *w << " added to yard." << std::endl;
} else {
 std::cout << "Error: Invalid ADD_WAGON parameters.\n";
}
}
else if (cmd == "REMOVE_WAGON") {
 int id;
if (iss >> id) {
bool found = false;
for (int i = 0; i < NUM_DESTINATIONS_INT && !found; i++) {
for (int j = 0; j < NUM_CARGOTYPES_INT && !found; j++) {
Wagon* removed = classificationYard.getBlockTrain(i, j).detachById(id);
if (removed != nullptr) {
delete removed;
found = true;
std::cout << "Wagon " << id << " removed." << std::endl;
}
}
}
 if (!found) {
 std::cout << "Error: Wagon " << id << " not found." << std::endl;
 }
} else {
 std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
 }
 }
else if (cmd == "ASSEMBLE_TRAIN") {
    std::string destStr;
    if (iss >> destStr) {
        Destination dest = parseDestination(destStr);
        TrainTrack& track = getDepartureYard(dest);
        std::string trainName = track.generateTrainName();
        
        
        Train* train = classificationYard.assembleTrain(dest, trainName);

        if (train) {
            int splitCounter = 1;
            
            Train* splitTrain = nullptr;
            
            
            while ((splitTrain = train->verifyCouplersAndSplit(splitCounter)) != nullptr) {
                
                
                track.addTrain(splitTrain);
                
                
                std::cout << "Train " << splitTrain->getName()
                          << " assembled after split with "
                          << splitTrain->getWagons() << " wagons." << std::endl;
                          
                splitCounter++;
            }

            
            track.addTrain(train);
            std::cout << "Train " << train->getName()
                      << " assembled with "
                      << train->getWagons() << " wagons." << std::endl;
        } else {
            std::cout << "No wagons to assemble for " << destStr << std::endl;
        }
    } else {
        std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    }
}
 else if (cmd == "DISPATCH_TRAIN") {
 std::string destStr;
 if (iss >> destStr) {
 Destination dest = parseDestination(destStr);
 dispatchFromTrack(dest);
 } else {
 std::cout << "Error: Invalid DISPATCH parameters.\n";
 }
 }
else if (cmd == "PRINT_YARD") {
 std::cout << "--- classification Yard ---\n";
classificationYard.print();
 }
 else if (cmd == "PRINT_TRACK") {
 std::string destStr;
 if (iss >> destStr) {
 Destination dest = parseDestination(destStr);
 departureYard[static_cast<int>(dest)].printTrack();
 } else {
std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
}
 }
 else if (cmd == "AUTO_DISPATCH") {
 std::string mode;
  if (iss >> mode) {
if (mode == "ON") {
TrainTrack::autoDispatch = true;
std::cout << "Auto dispatch enabled" << std::endl;
 } else if (mode == "OFF") {
 TrainTrack::autoDispatch = false;
 std::cout << "Auto dispatch disabled" << std::endl;
} else {
 std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
 }
} else {
std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
}
 }
 else if (cmd == "CLEAR") {
 classificationYard.clear();
 for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
 while (!departureYard[i].isEmpty()) {
Train* departed = departureYard[i].departTrain();
 if (departed) delete departed;
 }
 }
 std::cout << "System cleared." << std::endl;
 }
 else {
 std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
}
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    
    TrainTrack& t = getDepartureYard(track);
    
    
    if (t.isEmpty()) {
        
        std::cout << "Error: No trains to dispatch from track " << destinationToString(track) << ".\n";
    } else {
        
        Train* departed = t.departTrain();
        
        if (departed) {
            
            
            std::cout << "Dispatching " << departed->getName() 
                      << " (" << departed->getTotalWeight() << " tons)." << std::endl;
            
            
            delete departed;
        }
    }
}

void RailMarshal::printDepartureYard() const
{
for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
 {
 std::cout << "Track " << i << " ("
<< destinationToString(static_cast<Destination>(i)) << "):\n";
departureYard[i].printTrack();
}
}

// Debug helper functions
void RailMarshal::printStatus() const
{
 std::cout << "--- classification Yard ---\n";
 classificationYard.print();

 std::cout << "--- Departure Yard ---\n";
 for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
 {
 departureYard[i].printTrack();
}
}