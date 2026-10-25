#ifndef SHIP_H
#define SHIP_H
#include "container.h"
class Ship
{
private:
  int shipID;
  double length;    // metres, 20 to 400
  double depth;     // metres needed, 2 to 25
  int destination;  // 1 to 15, same list as containers
  Container *cargo; // dynamic array, size = capacity
  int count;        // containers on board
  int capacity;     // max containers, 1 to 15, fixed
  char status[20];  // "Waiting", "Docked", "Departed" or "No berth fits"
  static int nextID;
  void copyFrom(const Ship &other); // deep copy, used by copy constructor and =
public:
  Ship();
  Ship(double len, double dep, int dest, int cap); // checks all values
  Ship(const Ship &other);                         // copy constructor
  Ship &operator=(const Ship &other);              // assignment
  ~Ship();                                         // frees the cargo array
  Ship &operator+=(Container c);                   // add a container if there is room
  bool isValid();
  bool isFull();
  void displayDetails();
  int getShipID();
  double getLength();
  double getDepth();
  int getDestination();
  int getCount();
  int getCapacity();
  const char *getStatus();
  void setStatus(const char s[]);
  Container &getContainer(int index);
};
#endif