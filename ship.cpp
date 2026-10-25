#include "ship.h"
#include <iostream>
#include <cstring>
using namespace std;
int Ship::nextID = 1;
// Empty (invalid) ship: ID 0, no cargo array
Ship::Ship()
{
  shipID = 0;
  length = 0;
  depth = 0;
  destination = 0;
  cargo = NULL;
  count = 0;
  capacity = 0;
  status[0] = '\0';
}

// Starts empty (invalid). Only if all four values are fine does it become a real ship.
Ship::Ship(double len, double dep, int dest, int cap)
{
  shipID = 0;
  length = 0;
  depth = 0;
  destination = 0;
  cargo = NULL;
  count = 0;
  capacity = 0;
  status[0] = '\0';

  bool lengthOk = false;
  if (len >= 20 && len <= 400)
  {
    lengthOk = true;
  }

  bool depthOk = false;
  if (dep >= 2 && dep <= 25)
  {
    depthOk = true;
  }

  bool destinationOk = false;
  if (dest >= 1 && dest <= 15)
  {
    destinationOk = true;
  }

  bool capacityOk = false;
  if (cap >= 1 && cap <= 15)
  {
    capacityOk = true;
  }

  if (lengthOk && depthOk && destinationOk && capacityOk)
  {
    length = len;
    depth = dep;
    destination = dest;
    capacity = cap;
    cargo = new Container[capacity]; // made once, exactly the right size
    strcpy(status, "Waiting");
    shipID = nextID; // only valid ships get an ID
    nextID++;
  }
}
void Ship::copyFrom(const Ship &other)
{
  shipID = other.shipID; // a copy is the same ship: same ID
  length = other.length;
  depth = other.depth;
  destination = other.destination;
  count = other.count;
  capacity = other.capacity;
  strcpy(status, other.status);
  cargo = new Container[capacity]; // its own array, not shared
  for (int i = 0; i < count; i++)
  {
    cargo[i] = other.cargo[i];
  }
}
Ship::Ship(const Ship &other)
{
  copyFrom(other);
}
Ship &Ship::operator=(const Ship &other)
{
  if (this == &other)
  {
    return *this; // ship = ship; nothing to do
  }
  delete[] cargo; // free the old array first
  copyFrom(other);
  return *this;
}

Ship &Ship::operator+=(Container c)
{
  if (!isValid() || isFull())
  {
    return *this;
  }
  if (!c.isValid() || c.getDestination() != destination)
  {
    return *this;
  }
  cargo[count] = c;
  count++;
  return *this;
}
bool Ship::isValid()
{
  if (shipID != 0)
  {
    return true;
  }
  return false;
}
bool Ship::isFull()
{
  if (count == capacity)
  {
    return true;
  }
  return false;
}
void Ship::displayDetails()
{
  cout << "Ship " << shipID << " (" << Container::getPlaceName(destination) << ") -- ";
  cout << length << " m -- ";
  cout << "needs " << depth << " m depth -- ";
  cout << count << " of " << capacity << " containers -- ";
  cout << status << endl;

  for (int i = 0; i < count; i++)
  {
    cout << "    ";
    cargo[i].displayDetails();
  }
}
int Ship::getShipID()
{
  return shipID;
}
double Ship::getLength()
{
  return length;
}
double Ship::getDepth()
{
  return depth;
}
int Ship::getDestination()
{
  return destination;
}
int Ship::getCount()
{
  return count;
}
int Ship::getCapacity()
{
  return capacity;
}
const char *Ship::getStatus()
{
  return status;
}
void Ship::setStatus(const char s[])
{
  strcpy(status, s);
}
Container &Ship::getContainer(int index)
{
  return cargo[index];
}
Ship::~Ship()
{
  delete[] cargo;
}
