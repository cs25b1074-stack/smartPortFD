#include "port.h"
#include <iostream>
#include <cstring>
using namespace std;
Port::Port() : berths(0), berthCount(0), ships(0), shipCount(0), stepNumber(0) {}
Port::~Port()
{
  delete[] berths;
  delete[] ships;
}
void Port::setupBerths(int n)
{
  berths = new Berth[n];
  berthCount = n;
}
void Port::setBerth(int index, Berth b) { berths[index] = b; }

int Port::findShip(int destination)
{
  for (int i = 0; i < shipCount; i++)
    if (ships[i].getDestination() == destination)
      return i;
  return -1;
}

int Port::findShipByID(int id)
{
  for (int i = 0; i < shipCount; i++)
    if (ships[i].getShipID() == id)
      return i;
  return -1;
}

void Port::addShip(Ship s)
{
  Ship *bigger = new Ship[shipCount + 1]; // one more place
  for (int i = 0; i < shipCount; i++)
    bigger[i] = ships[i];
  bigger[shipCount] = s;
  delete[] ships;
  ships = bigger;
  shipCount++;
}

int Port::totalContainers()
{
  int total = 0;
  for (int i = 0; i < shipCount; i++)
    total += ships[i].getCount();
  return total;
}

void Port::step()
{
  cout << "\n--- Step " << ++stepNumber << " ---\n";
  // 1. Every docked ship: its crane handles the next waiting container, or the ship departs
  for (int b = 0; b < berthCount; b++)
  {
    if (berths[b].isFree())
      continue;
    Ship &s = ships[findShipByID(berths[b].getDockedShipID())];
    int i = 0;
    while (i < s.getCount() && strcmp(s.getContainer(i).getStatus(), "Waiting") != 0)
      i++;
    if (i == s.getCount())
    { // nothing left to load
      s.setStatus("Departed");
      berths[b].release();
      cout << "Ship " << s.getShipID() << " departed for " << Container::getPlaceName(s.getDestination()) << endl;
      continue;
    }
    Container &c = s.getContainer(i);
    if (berths[b].getCrane().canLift(c.getWeight()))
    {
      c.setStatus("Loaded");
      cout << "Berth " << b + 1 << ": loaded C" << c.getContainerID() << " (" << c.getWeight()
           << " t) onto Ship " << s.getShipID() << endl;
    }
    else
    {
      c.setStatus("Too heavy");
      cout << "Berth " << b + 1 << ": crane (max " << berths[b].getCrane().getMaxLift()
           << " t) cannot lift C" << c.getContainerID() << " (" << c.getWeight() << " t)\n";
    }
  }
  // 2. Every waiting ship: dock at the first free berth it fits
  for (int i = 0; i < shipCount; i++)
  {
    if (strcmp(ships[i].getStatus(), "Waiting") != 0)
      continue;
    bool fitsSomewhere = false;
    for (int b = 0; b < berthCount; b++)
    {
      if (!berths[b].canFit(ships[i].getLength(), ships[i].getDepth()))
        continue;
      fitsSomewhere = true;
      if (berths[b].dock(ships[i].getShipID()))
      {
        ships[i].setStatus("Docked");
        cout << "Ship " << ships[i].getShipID() << " docked at Berth " << b + 1 << endl;
        break;
      }
    }
    if (!fitsSomewhere)
    {
      ships[i].setStatus("No berth fits");
      cout << "Ship " << ships[i].getShipID() << " is too big for every berth\n";
    }
  }
}

bool Port::isFinished()
{
  for (int i = 0; i < shipCount; i++)
    if (strcmp(ships[i].getStatus(), "Waiting") == 0 || strcmp(ships[i].getStatus(), "Docked") == 0)
      return false;
  return true;
}

void Port::displayStatus()
{
  cout << "\n--- PORT STATUS ---\n";
  for (int b = 0; b < berthCount; b++)
    berths[b].displayDetails();
  if (shipCount == 0)
    cout << "No ships yet.\n";
  for (int i = 0; i < shipCount; i++)
    ships[i].displayDetails();
  cout << "Containers: " << totalContainers() << " of 15\n";
}
int Port::getBerthCount() { return berthCount; }
int Port::getShipCount() { return shipCount; }
Ship &Port::getShip(int index) { return ships[index]; }