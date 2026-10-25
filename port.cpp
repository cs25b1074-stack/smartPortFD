#include "port.h"
#include <iostream>
#include <cstring>
using namespace std;

Port::Port()
{
  berths = NULL;
  berthCount = 0;
  ships = NULL;
  shipCount = 0;
  stepNumber = 0;
}
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
void Port::setBerth(int index, Berth b)
{
  berths[index] = b;
}

int Port::findShip(int destination)
{
  for (int i = 0; i < shipCount; i++)
  {
    if (ships[i].getDestination() == destination)
    {
      return i;
    }
  }
  return -1;
}

int Port::findShipByID(int id)
{
  for (int i = 0; i < shipCount; i++)
  {
    if (ships[i].getShipID() == id)
    {
      return i;
    }
  }
  return -1;
}

void Port::addShip(Ship s)
{
  Ship *bigger = new Ship[shipCount + 1]; // one more place
  for (int i = 0; i < shipCount; i++)
  {
    bigger[i] = ships[i];
  }
  bigger[shipCount] = s;
  delete[] ships;
  ships = bigger;
  shipCount++;
}

int Port::totalContainers()
{
  int total = 0;
  for (int i = 0; i < shipCount; i++)
  {
    total = total + ships[i].getCount();
  }
  return total;
}

void Port::step()
{
  stepNumber++;
  cout << "\n--- Step " << stepNumber << " ---\n";

  // PART 1: every docked ship loads all its containers, then departs
  for (int b = 0; b < berthCount; b++)
  {
    if (berths[b].isFree())
    {
      continue; // no ship here
    }

    int shipIndex = findShipByID(berths[b].getDockedShipID());
    Ship &ship = ships[shipIndex];
    Crane &crane = berths[b].getCrane();

    for (int i = 0; i < ship.getCount(); i++)
    {
      Container &c = ship.getContainer(i);
      if (crane.canLift(c.getWeight()))
      {
        c.setStatus("Loaded");
        cout << "Berth " << b + 1 << ": loaded C" << c.getContainerID() << endl;
      }
      else
      {
        c.setStatus("Too heavy");
        cout << "Berth " << b + 1 << ": C" << c.getContainerID() << " is too heavy" << endl;
      }
    }

    ship.setStatus("Departed");
    berths[b].release();
    cout << "Ship " << ship.getShipID() << " departed" << endl;
  }

  // PART 2: every waiting ship docks at a free berth it fits in
  for (int i = 0; i < shipCount; i++)
  {
    if (strcmp(ships[i].getStatus(), "Waiting") != 0)
    {
      continue; // only waiting ships try to dock
    }

    // first check: does this ship fit in ANY berth at all?
    bool fits = false;
    for (int b = 0; b < berthCount; b++)
    {
      if (berths[b].canFit(ships[i].getLength(), ships[i].getDepth()))
      {
        fits = true;
      }
    }

    if (!fits)
    {
      ships[i].setStatus("No berth fits");
      cout << "Ship " << ships[i].getShipID() << " is too big for every berth" << endl;
      continue;
    }

    // it fits somewhere, so try to dock at a free berth
    for (int b = 0; b < berthCount; b++)
    {
      if (berths[b].canFit(ships[i].getLength(), ships[i].getDepth()) && berths[b].isFree())
      {
        berths[b].dock(ships[i].getShipID());
        ships[i].setStatus("Docked");
        cout << "Ship " << ships[i].getShipID() << " docked at Berth " << b + 1 << endl;
        break;
      }
    }
  }
}

bool Port::isFinished()
{
  for (int i = 0; i < shipCount; i++)
  {
    if (strcmp(ships[i].getStatus(), "Waiting") == 0)
    {
      return false;
    }
    if (strcmp(ships[i].getStatus(), "Docked") == 0)
    {
      return false;
    }
  }
  return true;
}

void Port::displayStatus()
{
  cout << "\n--- PORT STATUS ---\n";
  for (int b = 0; b < berthCount; b++)
  {
    berths[b].displayDetails();
  }
  if (shipCount == 0)
  {
    cout << "No ships yet.\n";
  }
  for (int i = 0; i < shipCount; i++)
  {
    ships[i].displayDetails();
  }
  cout << "Containers: " << totalContainers() << " of 15\n";
}
int Port::getBerthCount()
{
  return berthCount;
}
int Port::getShipCount()
{
  return shipCount;
}
Ship &Port::getShip(int index)
{
  return ships[index];
}
