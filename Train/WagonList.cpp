#include "WagonList.h"
#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    Wagon* current = front;
    while (current != nullptr) {
        Wagon* next = current->getNext();
        delete current;
        current = next;
    }
    front = nullptr;
    rear = nullptr;
    
    totalWeight = 0;
}

WagonList::WagonList(WagonList&& other) noexcept
{
    front = other.front;
    rear = other.rear;
    totalWeight = other.totalWeight;
    
    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
}

WagonList& WagonList::operator=(WagonList&& other) noexcept
{
    if (this != &other) {
        clear();
        
        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;
        
        other.front = nullptr;
        other.rear = nullptr;
        other.totalWeight = 0;
    }
    return *this;
}

Wagon* WagonList::findById(int id)
{
    Wagon* current = front;
    while(current != nullptr){
        if(current->getID() == id){
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

void WagonList::addWagonToRear(Wagon* w)
{
    if(front == nullptr){
        rear = w;
        front = w;
        w->setNext(nullptr);
        w->setPrev(nullptr);
    }
    else{
        rear->setNext(w);
        w->setPrev(rear);
        w->setNext(nullptr);
        rear = w;
    }
    totalWeight += w->getWeight();
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    if(front == nullptr)
        return true;
    else
        return false;
}

void WagonList::insertSorted(Wagon* wagon)
{
    if(front == nullptr){
        front = wagon;
        rear = wagon;
        wagon->setNext(nullptr);
        wagon->setPrev(nullptr);
    }
    else if(wagon->getWeight() > front->getWeight()){
        wagon->setNext(front);
        front->setPrev(wagon);
        front = wagon;
        wagon->setPrev(nullptr);
    }
    else{
        Wagon* temp = front;
        while(temp->getNext() != nullptr && temp->getNext()->getWeight() >= wagon->getWeight()){
            temp = temp->getNext();
        }
        wagon->setNext(temp->getNext());
        wagon->setPrev(temp);

        if(temp->getNext() != nullptr){
            temp->getNext()->setPrev(wagon);
        }
        else{rear = wagon;}
        temp->setNext(wagon);
    }
    
    totalWeight += wagon->getWeight();
}

void WagonList::appendList(WagonList&& other)
{
    if (other.front == nullptr) {
        return;
    }
    if (front == nullptr) {
        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;
    } else {
        rear->setNext(other.front);
        other.front->setPrev(rear);
        rear = other.rear;
        totalWeight += other.totalWeight;
    }
    
    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
}

Wagon* WagonList::detachById(int id)
{
    if (front == nullptr) {
        return nullptr;
    }
    if (front->getID() == id) {
        Wagon* toRemove = front;
        front = front->getNext();
        
        if (front != nullptr) {
            front->setPrev(nullptr);
        } else {
            rear = nullptr;
        }
        
        toRemove->setNext(nullptr);
        toRemove->setPrev(nullptr);
        totalWeight -= toRemove->getWeight();
        std::cout << "Wagon " << toRemove->getID() << " detached from Wagon List. " << std::endl;
        return toRemove;
    }
    Wagon* current = front;
    while (current->getNext() != nullptr && current->getNext()->getID() != id) {
        current = current->getNext();}
    if (current->getNext() != nullptr) {
        Wagon* toRemove = current->getNext();
        current->setNext(toRemove->getNext());
        if (toRemove->getNext() != nullptr) {
            toRemove->getNext()->setPrev(current);
        }else{
            rear = current;}
        
        toRemove->setNext(nullptr);
        toRemove->setPrev(nullptr);
        totalWeight -= toRemove->getWeight();
        std::cout << "Wagon " << toRemove->getID() << " detached from Wagon List. " << std::endl;
        return toRemove;
    }
    return nullptr;
}

WagonList WagonList::splitAtById(int id)
{
    WagonList newList;
    if (front == nullptr){return newList;}
    if (front->getID() == id){
        newList.front = front;
        newList.rear = rear;
        newList.totalWeight = totalWeight;
        front = nullptr;
        rear = nullptr;
        totalWeight = 0;
        return newList;
    }
    Wagon* current = front; 
    while (current != nullptr && current->getID() != id) {
        current = current->getNext();
    }
    if (current == nullptr) {
        return newList;}
    
    Wagon* prevWagon = current->getPrev();
    newList.front = current;
    newList.rear = rear;
    current->setPrev(nullptr);
    
    if (prevWagon) {
        prevWagon->setNext(nullptr);
        rear = prevWagon;
    } 
    else { 
        front = nullptr;
        rear = nullptr;
    }
    int oldTotalWeight = totalWeight; 
    totalWeight = 0;
    Wagon* temp = front;
    while (temp != nullptr) {
        totalWeight += temp->getWeight();
        temp = temp->getNext();
    }
    newList.totalWeight = oldTotalWeight - totalWeight;
    return newList;
}

void WagonList::print() const
{
    std::cout << *this << std::endl;
    return;
}

std::ostream& operator<<(std::ostream& os, const WagonList& list)
{
    if (list.isEmpty())
        return os;

    Wagon* current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}