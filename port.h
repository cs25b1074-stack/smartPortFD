#ifndef PORT_H
#define PORT_H
#include "berth.h"
#include "ship.h"
class Port
{
private:
  Berth *berths; // dynamic array, size chosen at setup
  int berthCount;
  Ship *ships; // dynamic array, grows by one per new destination
  int shipCount;
  int stepNumber;
  int findShipByID(int id);

public:
  Port();
  ~Port(); // frees both arrays
  void setupBerths(int n);
  void setBerth(int index, Berth b);
  int findShip(int destination); // index of the ship for that destination, or -1
  void addShip(Ship s);
  int totalContainers();
  void step();       // one simulation step
  bool isFinished(); // true when no ship is waiting or docked
  void displayStatus();
  int getBerthCount();
  int getShipCount();
  Ship &getShip(int index);
};
#endif