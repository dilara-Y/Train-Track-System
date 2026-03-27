#include "Train.h"
#include <iostream>

Train::Train() : name(""), destination(Destination::OTHERS), totalWeight(0), nextLocomotive(nullptr) {}
Train::Train(const std::string &_name, Destination _dest) : name(_name), destination(_dest), totalWeight(0), nextLocomotive(nullptr) {}
Train::~Train() { clear(); }

// This function is given to you ready
void Train::appendWagonList(WagonList &wl)
{
    // Makes appendList use move semantics
    wagons.appendList(std::move(wl));
    totalWeight = wagons.getTotalWeight();
}

// This function is given to you ready
void Train::addWagonToRear(Wagon *w)
{
    wagons.addWagonToRear(w);
    totalWeight = wagons.getTotalWeight();
}

void Train::clear()
{ 
    //TODO: Do the cleaning as necesssary
    wagons.clear();
    totalWeight = 0;
}

// This function is given to you ready
void Train::print() const
{
    std::cout << "Train " << name << " (" << totalWeight << " tons): ";
    std::cout << wagons << std::endl;
}

Train *Train::verifyCouplersAndSplit(int splitCounter)
{
    
    Wagon *current = wagons.getRear();
    
    int weightPulled = 0; 

    
    while (current) {
        
        weightPulled += current->getWeight();

        Wagon *previous = current->getPrev();
        
        
        if (previous) {
            
            
            if (weightPulled > previous->getMaxCouplerLoad()) {
                
                
                int splitId = current->getID();
                WagonList newWagons = wagons.splitAtById(splitId);
                
                if (!newWagons.isEmpty()) {
                    std::string newName = name + "_split_" + std::to_string(splitCounter);
                    Train *newTrain = new Train(newName, destination);
                    
                    
                    newTrain->wagons = std::move(newWagons);
                    newTrain->totalWeight = newTrain->wagons.getTotalWeight();
                    
                    
                    totalWeight = wagons.getTotalWeight(); 
                    
                    std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
                    std::cout << newTrain->wagons << std::endl;
                    
                    return newTrain;
                }
            }
        }
        
         current = previous; 
           
    }
    return nullptr;
}
