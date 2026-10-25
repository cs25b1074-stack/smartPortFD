#ifndef CRANE_H
#define CRANE_H
class Crane
{
private:
  double maxLift; // tonnes, 1 to 100
public:
  Crane();                     // empty crane, max lift 0
  Crane(double lift);          // checks the value
  bool isValid();              // false if the lift value was rejected
  bool canLift(double weight); // true if weight <= maxLift
  double getMaxLift();
};
#endif