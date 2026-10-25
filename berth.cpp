#include "berth.h"
#include <iostream>
using namespace std;
int Berth::nextID = 1;
// Empty (invalid) berth: ID 0, no ship docked
Berth::Berth()
{
  berthID = 0;
  length = 0;
  depth = 0;
  dockedShipID = 0;
}

// Starts empty (invalid). Only if the length, depth and crane are all fine does it become a real berth.
Berth::Berth(double len, double dep, double lift)
{
  berthID = 0;
  length = 0;
  depth = 0;
  dockedShipID = 0;
  crane = Crane(lift); // COMPOSITION: the berth makes its own crane

  bool lengthOk = false;
  if (len >= 20 && len <= 500)
  {
    lengthOk = true;
  }

  bool depthOk = false;
  if (dep >= 2 && dep <= 30)
  {
    depthOk = true;
  }

  if (lengthOk && depthOk && crane.isValid())
  {
    length = len;
    depth = dep;
    berthID = nextID; // only valid berths get an ID
    nextID++;
  }
}
bool Berth::isValid()
{
  if (berthID != 0)
  {
    return true;
  }
  return false;
}
bool Berth::isFree()
{
  if (dockedShipID == 0)
  {
    return true;
  }
  return false;
}
bool Berth::canFit(double shipLength, double shipDepth)
{
  if (shipLength <= length && shipDepth <= depth)
  {
    return true;
  }
  return false;
}
bool Berth::dock(int shipID)
{
  if (!isFree())
  {
    return false;
  }
  dockedShipID = shipID;
  return true;
}
void Berth::release()
{
  dockedShipID = 0;
}
void Berth::displayDetails()
{
  cout << "Berth " << berthID << " -- ";
  cout << length << " m long -- ";
  cout << depth << " m deep -- ";
  cout << "Crane max " << crane.getMaxLift() << " t -- ";

  if (isFree())
  {
    cout << "Free" << endl;
  }
  else
  {
    cout << "Ship " << dockedShipID << " docked" << endl;
  }
}
int Berth::getBerthID()
{
  return berthID;
}
int Berth::getDockedShipID()
{
  return dockedShipID;
}
Crane &Berth::getCrane()
{
  return crane;
}