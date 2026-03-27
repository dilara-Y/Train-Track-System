#include "TrainTrack.h"
#include <iostream>

bool TrainTrack::autoDispatch = false;

TrainTrack::TrainTrack()
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(Destination::OTHERS),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::TrainTrack(Destination _dest)
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(_dest),
      totalWeight(0), trainCounter(0)
{
}


    TrainTrack::~TrainTrack()
{
    while (firstLocomotive != nullptr) {
        Train* departed = departTrain();
        delete departed;
    }
}


std::string TrainTrack::generateTrainName()
{
    ++trainCounter;
    return "Train_" + destinationToString(destination) + "_" + std::to_string(trainCounter);
}

void TrainTrack::addTrain(Train *train)
{
    if (!train) return;
    
    if (isEmpty()) {
        firstLocomotive = train;
        lastLocomotive = train;
        train->setNext(nullptr);
    } else {
        lastLocomotive->setNext(train);
        train->setNext(nullptr);
        lastLocomotive = train;
    }
    
    totalWeight += train->getTotalWeight();
    
    if (autoDispatch && totalWeight > AUTO_DISPATCH_LIMIT) {
        int maxIterations = 100;
        int iterations = 0;
        
        while (totalWeight > AUTO_DISPATCH_LIMIT && 
               firstLocomotive != lastLocomotive &&
               iterations < maxIterations) {
            
            Train* departed = departTrain();
            if (departed) {
                std::cout << "Auto-dispatch: departing " << departed->getName() << " to make room.\n";
                delete departed;
            } else {
                break;
            }
            iterations++;
        }
        
        if (iterations >= maxIterations) {
            std::cerr << "ERROR: Auto-dispatch max iterations reached!\n";
        }
    }
}

Train *TrainTrack::departTrain()
{
    if (firstLocomotive == nullptr) {
        return nullptr;
    }
    
    Train* removed = firstLocomotive;
    firstLocomotive = firstLocomotive->getNext();
    
    if (firstLocomotive == nullptr) {
        lastLocomotive = nullptr;
    }
    
    totalWeight -= removed->getTotalWeight();
    removed->setNext(nullptr);
    
    std::cout << "Train " << removed->getName() << " departed from Track " << destinationToString(destination) << "." << std::endl;
    return removed;
}

bool TrainTrack::isEmpty() const
{
    return firstLocomotive == nullptr;
}

Train *TrainTrack::findTrain(const std::string &name) const
{
    Train* current = firstLocomotive;
    while (current != nullptr) {
        if (current->getName() == name) {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

void TrainTrack::printTrack() const
{
    if (isEmpty())
        return;

    Train *current = firstLocomotive;

    std::cout << "[Track " << static_cast<int>(firstLocomotive->destination) << "] ";
    while (current)
    {
        std::cout << current->getName() << "(" << current->getTotalWeight() << "ton)-" << current->wagons << " -> ";
        current = current->getNext();
    }
    std::cout << std::endl;
    return;
}