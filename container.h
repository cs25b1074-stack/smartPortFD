#ifndef CONTAINER_H
#define CONTAINER_H
// All containers are the standard 20-foot size (1 TEU); only weight and type vary.
class Container
{
private:
  int containerID;
  double weight;                 // tonnes, 0.1 to 35
  char type[20];                 // "Hazardous", "Reefer" or "Standard"
  int destination;               // 1 to 15, from the places list
  char status[20];               // "Waiting", "Loaded" or "Too heavy"
  static int nextID;             // shared ID counter
  static const char *places[15]; // the 15 destinations shown as MCQ
public:
  Container();                                // empty container, everything 0
  Container(double w, int typeChoice, int d); // checks all values
  bool isValid();
  void displayDetails();
  int getContainerID();
  double getWeight();
  const char *getType();
  int getDestination();
  const char *getStatus();
  void setStatus(const char s[]);
  static const char *getPlaceName(int number); // number 1 to 15
};
#endif