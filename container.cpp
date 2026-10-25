#include "container.h"
#include <iostream>
#include <cstring>
using namespace std;

int Container::nextID = 1;
const char *Container::places[15] = {
    "New York", "San Francisco", "Los Angeles", "London", "Rotterdam",
    "Hamburg", "Dubai", "Mumbai", "Chennai", "Singapore",
    "Shanghai", "Busan", "Tokyo", "Sydney", "Cape Town"};
Container::Container() : containerID(0), weight(0), destination(0)
{
  type[0] = '\0';
  status[0] = '\0';
}
Container::Container(double w, int typeChoice, int d) : containerID(0), weight(0), destination(0)
{
  type[0] = '\0';
  status[0] = '\0';

  bool weightOk = false;
  if (w >= 0.1 && w <= 35)
  {
    weightOk = true;
  }

  bool typeOk = false;
  if (typeChoice >= 1 && typeChoice <= 3)
  {
    typeOk = true;
  }

  bool destinationOk = false;
  if (d >= 1 && d <= 15)
  {
    destinationOk = true;
  }

  if (weightOk && typeOk && destinationOk)
  {
    const char *typeNames[3] = {"Hazardous", "Reefer", "Standard"};
    strcpy(type, typeNames[typeChoice - 1]);
    weight = w;
    destination = d;
    strcpy(status, "Waiting"); // waits at the port to be loaded
    containerID = nextID;      // only valid containers get an ID
    nextID++;
  }
}
bool Container::isValid()
{
  if (containerID != 0)
  {
    return true;
  }
  return false;
}
void Container::displayDetails()
{
  cout << "C" << containerID << " -- ";
  cout << type << " -- ";
  cout << weight << " t -- ";
  cout << "To: " << getPlaceName(destination) << " -- ";
  cout << status << endl;
}
const char *Container::getPlaceName(int number)
{
  if (number < 1 || number > 15)
  {
    return "Unknown";
  }
  return places[number - 1];
}
int Container::getContainerID()
{
  return containerID;
}
double Container::getWeight()
{
  return weight;
}
const char *Container::getType()
{
  return type;
}
int Container::getDestination()
{
  return destination;
}
const char *Container::getStatus()
{
  return status;
}
void Container::setStatus(const char s[])
{
  strcpy(status, s);
}
