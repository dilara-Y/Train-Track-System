#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {
    // Arrays are automatically initialized
}

ClassificationYard::~ClassificationYard() { 
    clear(); 
}

WagonList& ClassificationYard::getBlockTrain(int destination, int cargoType) {
    return blockTrains[destination][cargoType];
}

WagonList* ClassificationYard::getBlocksFor(Destination dest) {
    return blockTrains[static_cast<int>(dest)];
}

void ClassificationYard::insertWagon(Wagon* w) {
    if (!w)
        return;
        
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

Train* ClassificationYard::assembleTrain(Destination dest, const std::string& trainName) {
    int destIndex = static_cast<int>(dest);
    bool hasBlock[NUM_CARGOTYPES_INT];
    int hazardousIndex = -1;
    bool hasAnyWagons = false;
   for (int j = 0; j < NUM_CARGOTYPES_INT; j++){
        hasBlock[j] = !blockTrains[destIndex][j].isEmpty();
        if (hasBlock[j]) {
            hasAnyWagons = true;
            CargoType cargoType = static_cast<CargoType>(j);
            if (cargoType == CargoType::HAZARDOUS) {
                hazardousIndex = j;
            }
        }}
   if (!hasAnyWagons) {return nullptr;}

   Train* train = new Train(trainName, dest);

   bool used[NUM_CARGOTYPES_INT] = {false};
    if (hazardousIndex != -1) {used[hazardousIndex] = true; }
   
    for (int round = 0; round < NUM_CARGOTYPES_INT; round++){
        int maxIndex = -1;
        double maxWeight = -1;
       for (int j = 0; j < NUM_CARGOTYPES_INT; j++) {
            if (!used[j] && hasBlock[j]) {
                double frontWeight = blockTrains[destIndex][j].getFront()->getWeight();
                if (frontWeight > maxWeight) {
                    maxWeight = frontWeight;
                    maxIndex = j;
                }
            }
        }
       if (maxIndex != -1){
            train->appendWagonList(blockTrains[destIndex][maxIndex]);
            used[maxIndex] = true;
        } else {
            break; 
        }
    }
   if (hazardousIndex != -1 && !blockTrains[destIndex][hazardousIndex].isEmpty()){
        
        Wagon* hazardousWagon = blockTrains[destIndex][hazardousIndex].detachById(
            blockTrains[destIndex][hazardousIndex].getFront()->getID()
        );
       if (hazardousWagon) {
            train->addWagonToRear(hazardousWagon);
        }
    }
    return train;
}

bool ClassificationYard::isEmpty() const {
    for (int i = 0; i < NUM_DESTINATIONS_INT; i++) {
        for (int j = 0; j < NUM_CARGOTYPES_INT; j++) {
            if (!blockTrains[i][j].isEmpty()) {
                return false;
            }
        }
    }
    return true;
}

void ClassificationYard::clear() {
    for (int i = 0; i < NUM_DESTINATIONS_INT; i++) {
        for (int j = 0; j < NUM_CARGOTYPES_INT; j++) {
            blockTrains[i][j].clear();
        }
    }
}

void ClassificationYard::print() const {
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i) {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j) {
            if (!blockTrains[i][j].isEmpty()) {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}