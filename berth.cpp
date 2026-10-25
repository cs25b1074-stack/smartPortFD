#include "berth.h"
#include <iostream>
using namespace std;
int Berth::nextID = 1;
Berth::Berth() : berthID(0), length(0), depth(0), dockedShipID(0) {}
// The crane is built in the initializer list, before the body runs.
Berth::Berth(double len, double dep, double lift)
    : berthID(0), length(0), depth(0), crane(lift), dockedShipID(0)
{
  if (len < 20 || len > 500 || dep < 2 || dep > 30 || !crane.isValid())
    return;
  length = len;
  depth = dep;
  berthID = nextID++; // only valid berths get an ID
}
bool Berth::isValid() { return berthID != 0; }
bool Berth::isFree() { return dockedShipID == 0; }
bool Berth::canFit(double shipLength, double shipDepth)
{
  return shipLength <= length && shipDepth <= depth;
}
bool Berth::dock(int shipID)
{
  if (!isFree())
    return false;
  dockedShipID = shipID;
  return true;
}
void Berth::release() { dockedShipID = 0; }
void Berth::displayDetails()
{
  cout << "Berth " << berthID << " | " << length << " m long | " << depth
       << " m deep | Crane max " << crane.getMaxLift() << " t | ";
  if (isFree())
    cout << "Free" << endl;
  else
    cout << "Ship " << dockedShipID << " docked" << endl;
}
int Berth::getBerthID() { return berthID; }
int Berth::getDockedShipID() { return dockedShipID; }
Crane &Berth::getCrane() { return crane; }