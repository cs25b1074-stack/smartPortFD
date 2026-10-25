#include <iostream>
#include "port.h"
using namespace std;

// Input helpers: all keyboard input lives in main.cpp.
// readValue is OVERLOADED: one version for int, one for double.
int readValue(const char prompt[], int min, int max)
{
  int v;
  cout << prompt;
  cin >> v;
  while (v < min || v > max)
  {
    cout << "  Enter a number from " << min << " to " << max << ": ";
    cin >> v;
  }
  return v;
}

double readValue(const char prompt[], double min, double max)
{
  double v;
  cout << prompt;
  cin >> v;
  while (v < min || v > max)
  {
    cout << "  Enter a number from " << min << " to " << max << ": ";
    cin >> v;
  }
  return v;
}

bool readYesNo(const char prompt[])
{
  char c;
  cout << prompt;
  cin >> c;
  while (c != 'y' && c != 'n')
  {
    cout << "  Type y or n: ";
    cin >> c;
  }
  return c == 'y';
}

void showWelcome()
{
  cout << "==========================================\n";
  cout << "   WELCOME TO THE SMART PORT SIMULATOR\n";
  cout << "==========================================\n";
  cout << " Manage ships, berths, cranes and containers\n";
  cout << " and watch your port in action.\n";
  cout << "==========================================\n";
}

void setupPort(Port &port)
{
  if (port.getBerthCount() > 0)
  {
    cout << "\nThe port is already set up.\n";
    return;
  }
  cout << "\n--- SET UP THE PORT ---\n";
  int n = readValue("How many berths? (1-5): ", 1, 5);
  port.setupBerths(n);
  for (int i = 0; i < n; i++)
  {
    cout << "\nBerth " << i + 1 << endl;
    double len = readValue("  Length (m, 20-500): ", 20.0, 500.0);
    double dep = readValue("  Water depth (m, 2-30): ", 2.0, 30.0);
    double lift = readValue("  Crane max lift (t, 1-100): ", 1.0, 100.0);
    port.setBerth(i, Berth(len, dep, lift));
    cout << "  Berth " << i + 1 << " created with its crane.\n";
  }
  cout << "\nPort is ready: " << n << " berths, " << n << " cranes.\n";
}

void addContainers(Port &port)
{
  if (port.getBerthCount() == 0)
  {
    cout << "\nPlease set up the port first (option 1).\n";
    return;
  }
  cout << "\n--- ADD CONTAINERS --- (max 15 in the port)\n";
  while (port.totalContainers() < 15 && readYesNo("\nAdd a container? (y/n): "))
  {
    cout << "Type: 1) Hazardous  2) Reefer  3) Standard\n";
    int t = readValue("Choose type: ", 1, 3);
    double w = readValue("Weight (t, 0.1-35): ", 0.1, 35.0);
    cout << "Destination:\n";
    for (int i = 1; i <= 15; i++)
      cout << "  " << i << ") " << Container::getPlaceName(i) << endl;
    int d = readValue("Choose destination: ", 1, 15);
    int s = port.findShip(d);
    if (s == -1)
    { // first container for this place
      cout << "New ship needed for " << Container::getPlaceName(d) << ".\n";
      double len = readValue("  Ship length (m, 20-400): ", 20.0, 400.0);
      double dep = readValue("  Depth needed (m, 2-25): ", 2.0, 25.0);
      int cap = readValue("  Max containers (1-15): ", 1, 15);
      port.addShip(Ship(len, dep, d, cap));
      s = port.getShipCount() - 1;
      cout << "  Ship " << port.getShip(s).getShipID() << " created.\n";
    }
    Ship &ship = port.getShip(s);
    if (ship.isFull())
    {
      cout << "Ship " << ship.getShipID() << " is full. Container not added.\n";
    }
    else
    {
      ship += Container(w, t, d);
      cout << "Added to Ship " << ship.getShipID() << " (" << port.totalContainers() << " of 15)\n";
    }
  }
  if (port.totalContainers() == 15)
    cout << "Limit reached: 15 containers.\n";
}

void runSimulation(Port &port)
{
  if (port.getShipCount() == 0)
  {
    cout << "\nAdd containers first (option 2).\n";
    return;
  }
  while (!port.isFinished())
  {
    int c = readValue("\n1) Next step  2) Run to end  3) Back to menu\nChoose: ", 1, 3);
    if (c == 3)
      return;
    if (c == 1)
      port.step();
    if (c == 2)
      while (!port.isFinished())
        port.step();
  }
  cout << "\nAll ships have left. Simulation complete.\n";
  port.displayStatus();
}

int main()
{
  Port port;
  showWelcome();
  int choice = 0;
  while (choice != 5)
  {
    cout << "\nMAIN MENU\n  1) Set up the port\n  2) Add containers\n  3) View port status\n";
    choice = readValue("  4) Run simulation\n  5) Exit\nChoose: ", 1, 5);
    if (choice == 1)
      setupPort(port);
    if (choice == 2)
      addContainers(port);
    if (choice == 3)
      port.displayStatus();
    if (choice == 4)
      runSimulation(port);
  }
  cout << "\nThank you for using the Smart Port Simulator. Goodbye!\n";
  return 0;
}