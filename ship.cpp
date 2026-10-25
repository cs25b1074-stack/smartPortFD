#include "ship.h"
#include <iostream>
#include <cstring>
using namespace std;
int Ship::nextID = 1;
Ship::Ship() : shipID(0), length(0), depth(0), destination(0), cargo(0), count(0), capacity(0)
{
  status[0] = '\0';
}
Ship::Ship(double len, double dep, int dest, int cap)
    : shipID(0), length(0), depth(0), destination(0), cargo(0), count(0), capacity(0)
{
  status[0] = '\0';
  if (len < 20 || len > 400 || dep < 2 || dep > 25 || dest < 1 || dest > 15 || cap < 1 || cap > 15)
    return; // bad value: ship stays invalid
  length = len;
  depth = dep;
  destination = dest;
  capacity = cap;
  cargo = new Container[capacity]; // made once, exactly the right size
  strcpy(status, "Waiting");
  shipID = nextID++; // only valid ships get an ID
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
    cargo[i] = other.cargo[i];
}
Ship::Ship(const Ship &other) { copyFrom(other); }
Ship &Ship::operator=(const Ship &other)
{
  if (this == &other)
    return *this; // ship = ship; nothing to do
  delete[] cargo; // free the old array first
  copyFrom(other);
  return *this;
}
Ship::~Ship() { delete[] cargo; }
Ship &Ship::operator+=(Container c)
{
  if (!isValid() || isFull() || !c.isValid() || c.getDestination() != destination)
    return *this;
  cargo[count] = c;
  count++;
  return *this;
}
bool Ship::isValid() { return shipID != 0; }
bool Ship::isFull() { return count == capacity; }
void Ship::displayDetails()
{
  cout << "Ship " << shipID << " (" << Container::getPlaceName(destination) << ") | "
       << length << " m | needs " << depth << " m depth | " << count << " of "
       << capacity << " containers | " << status << endl;
  for (int i = 0; i < count; i++)
  {
    cout << "    ";
    cargo[i].displayDetails();
  }
}
int Ship::getShipID() { return shipID; }
double Ship::getLength() { return length; }
double Ship::getDepth() { return depth; }
int Ship::getDestination() { return destination; }
int Ship::getCount() { return count; }
int Ship::getCapacity() { return capacity; }
const char *Ship::getStatus() { return status; }
void Ship::setStatus(const char s[]) { strcpy(status, s); }
Container &Ship::getContainer(int index) { return cargo[index]; }