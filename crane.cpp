#include "crane.h"
Crane::Crane() : maxLift(0) {}
Crane::Crane(double lift) : maxLift(0)
{
  if (lift >= 1 && lift <= 100)
    maxLift = lift; // bad value: stays 0 (invalid)
}
bool Crane::isValid() { return maxLift != 0; }
bool Crane::canLift(double weight) { return weight <= maxLift; }
double Crane::getMaxLift() { return maxLift; }