#include "crane.h"

Crane::Crane()
{
  maxLift = 0;
}

Crane::Crane(double lift)
{
  maxLift = 0;

  if (lift >= 1 && lift <= 100)
  {
    maxLift = lift;
  }
}
bool Crane::isValid()
{
  if (maxLift != 0)
  {
    return true;
  }
  return false;
}

bool Crane::canLift(double weight)
{
  if (weight <= maxLift)
  {
    return true;
  }
  return false;
}

double Crane::getMaxLift()
{
  return maxLift;
}