#ifndef BERTH_H
#define BERTH_H
#include "crane.h"
class Berth
{
private:
  int berthID;
  double length;     // metres, 20 to 500
  double depth;      // metres, 2 to 30
  Crane crane;       // COMPOSITION: every berth has one crane
  int dockedShipID;  // 0 means the berth is free
  static int nextID; // shared ID counter
public:
  Berth();                                    // empty berth, everything 0
  Berth(double len, double dep, double lift); // checks all three values
  bool isValid();
  bool isFree();
  bool canFit(double shipLength, double shipDepth);
  bool dock(int shipID); // false if the berth is busy
  void release();        // ship leaves, berth becomes free
  void displayDetails();
  int getBerthID();
  int getDockedShipID();
  Crane &getCrane(); // reference to this berth's crane
};
#endif