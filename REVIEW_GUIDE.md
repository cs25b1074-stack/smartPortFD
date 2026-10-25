# Smart Port Simulator: Review Guide

This guide splits the code review across four people: **Dharmik, Ritu, Mithunn and Tanish**.
The split follows how the classes depend on each other, so each of you can review your part
mostly on your own, without waiting for anyone else.

---

## 1. How the project fits together

```
Container   (depends on nothing)        Crane   (depends on nothing)
    ▲                                      ▲
    │ Ship holds an array of Containers    │ Berth has one Crane (composition)
    │                                      │
   Ship                                  Berth
    ▲                                      ▲
    └──────────────┐        ┌──────────────┘
                 Port  (holds an array of Ships and an array of Berths, runs the simulation)
                   ▲
                   │
               main.cpp  (menus and all keyboard input)
```

| File | What it is |
|---|---|
| `container.h/.cpp` | One 20-foot container: weight, type, destination, status |
| `crane.h/.cpp` | A crane with a maximum lift weight |
| `berth.h/.cpp` | A berth (length, depth) that **contains** one crane and can hold one docked ship |
| `ship.h/.cpp` | A ship going to one destination, with a **dynamic array** of containers |
| `port.h/.cpp` | The port: dynamic arrays of berths and ships, plus the step-by-step simulation |
| `main.cpp` | Welcome screen, main menu, input checking |

**Build and run:**
```bash
g++ main.cpp port.cpp ship.cpp berth.cpp crane.cpp container.cpp
./a.out          # on Windows: a.exe
```
`g++ *.cpp` works too. All 6 `.cpp` files must be listed; leaving one out gives an "undefined reference" error.
The code is plain C++98, so it builds on any g++ version with no extra flags.

---

## 2. Who reviews what

| Reviewer | Owns | Depends on (only through the public functions listed) |
|---|---|---|
| **Dharmik** | `Container` | nothing |
| **Ritu** | `Crane` + `Berth` | nothing outside her own files |
| **Mithunn** | `Ship` | `Container`: default constructor, copy, `isValid()`, `getDestination()`, `displayDetails()`, `getPlaceName()` |
| **Tanish** | `Port` + `main.cpp` | public functions of `Berth`, `Crane`, `Ship`, `Container` |

> **Rule for minimal dependency:** when your code uses someone else's class, treat it as a black box.
> Assume each public function does what its comment in the `.h` file says. If you think one doesn't,
> **write it down and send it to the owner**; don't review their `.cpp` yourself in the first pass.

---

## 3. Dharmik: `Container` (`container.h`, `container.cpp`)

### What the code does

| Piece | Purpose |
|---|---|
| `containerID` | Unique ID. **0 means invalid.** |
| `weight` | Tonnes, allowed 0.1–35 |
| `type[20]` | `"Hazardous"`, `"Reefer"` or `"Standard"` |
| `destination` | Number 1–15, an index into `places` |
| `status[20]` | `"Waiting"` → `"Loaded"` or `"Too heavy"` |
| `static int nextID` | Shared counter; starts at 1 (`container.cpp:5`) |
| `static const char *places[15]` | The 15 destination names, shared by every class (`container.cpp:6-9`) |
| `Container()` | Empty, invalid container (ID 0). Needed because `Ship` does `new Container[capacity]` |
| `Container(w, typeChoice, d)` | Checks all three values. If any is out of range it **returns early** and the container stays invalid. Only valid containers take an ID from `nextID` |
| `isValid()` | `containerID != 0` |
| `displayDetails()` | Prints `C<id> \| type \| weight t \| To: place \| status` |
| `getPlaceName(n)` | **Static.** Turns 1–15 into a city name, `"Unknown"` otherwise. Used by Ship, Port and main |
| getters / `setStatus` | Plain access. `setStatus` uses `strcpy` |

### OOP concepts to check
- Encapsulation (all data private)
- Static data members (`nextID`, `places`) and a static member function (`getPlaceName`)
- Constructor overloading (default + parameterised)

### Review checklist
- [ ] Do the range checks in the constructor match the comments in `container.h`?
- [ ] Is the invalid-container case safe? (ID 0, empty strings, weight 0)
- [ ] Is an ID used up by an **invalid** container? (It should not be.)
- [ ] `type` and `status` are `char[20]`: is every string copied in short enough? (Longest is `"Hazardous"`, 9 chars; `"Too heavy"`, 9 chars)
- [ ] `setStatus` has no length check. Is that acceptable given who calls it?
- [ ] `getPlaceName(0)` and `getPlaceName(16)` both return `"Unknown"`?
- [ ] Default copy constructor and `=` are used (by `Ship`). Is that safe here? (Yes: no pointers, only fixed arrays.)

### Test ideas
- `Container(36, 1, 1)`, `Container(10, 4, 1)`, `Container(10, 1, 0)` → all invalid, no ID used
- Make 3 valid containers → IDs 1, 2, 3

---

## 4. Ritu: `Crane` and `Berth` (`crane.h/.cpp`, `berth.h/.cpp`)

### What `Crane` does

| Piece | Purpose |
|---|---|
| `maxLift` | Tonnes, allowed 1–100. **0 means invalid.** |
| `Crane()` | Empty crane, `maxLift = 0` |
| `Crane(lift)` | Stores `lift` only if it is 1–100; otherwise stays 0 |
| `isValid()` | `maxLift != 0` |
| `canLift(weight)` | `weight <= maxLift` |

### What `Berth` does

| Piece | Purpose |
|---|---|
| `berthID` | Unique ID, 0 = invalid |
| `length`, `depth` | Metres, 20–500 and 2–30 |
| `Crane crane` | **Composition**: the crane is part of the berth and is created and destroyed with it |
| `dockedShipID` | ID of the ship at this berth, **0 = free** |
| `Berth(len, dep, lift)` | Starts empty, builds its own crane with `crane = Crane(lift);`, then checks length, depth and that the crane is valid |
| `isFree()` | `dockedShipID == 0` |
| `canFit(shipLength, shipDepth)` | Ship must be no longer than the berth and need no more depth than it has |
| `dock(shipID)` | Fails (returns `false`) if the berth is busy; otherwise records the ship |
| `release()` | Ship leaves; berth becomes free |
| `getCrane()` | Returns a **reference** so Port can ask the real crane, not a copy |
| `displayDetails()` | Prints size, crane limit and Free/Docked |

### OOP concepts to check
- Composition (Berth **has a** Crane)
- Object as a data member (the crane is first made empty by `Crane()`, then replaced with `Crane(lift)`)
- Returning a reference (`Crane &getCrane()`)

### Review checklist
- [ ] Does every member get a starting value in **both** constructors? (`crane` is set by its own `Crane()` constructor in the default case.)
- [ ] If the crane is invalid, is the berth invalid too?
- [ ] `dock()` on a busy berth returns `false` and doesn't overwrite the ship?
- [ ] `canFit` uses `<=`: is a ship of exactly equal length or depth allowed? Is that intended?
- [ ] `Crane::isValid()` uses `maxLift != 0` on a `double`. Fine here, since 0 is assigned exactly, but worth noting
- [ ] Default copy and assignment of `Berth` are used by `Port::setBerth`. Safe? (Yes: no pointers.)
- [ ] An invalid berth still has `berthID 0`. Can one ever get into the port? (Check with Tanish: `main.cpp` validates first.)

### Test ideas
- `Berth(100, 10, 150)` → invalid, because the crane is invalid
- `dock(1)` then `dock(2)` → second call returns `false`; `release()` then `dock(2)` → `true`

---

## 5. Mithunn: `Ship` (`ship.h`, `ship.cpp`)

This is the class with **dynamic memory**, so it gets the most careful review.

### What the code does

| Piece | Purpose |
|---|---|
| `shipID` | Unique ID, 0 = invalid |
| `length`, `depth` | Metres, 20–400 and 2–25 (depth the ship **needs**) |
| `destination` | 1–15, same list as containers |
| `Container *cargo` | **Dynamic array**, size = `capacity`, created once in the constructor |
| `count` / `capacity` | Containers on board / maximum (1–15) |
| `status[20]` | `"Waiting"` → `"Docked"` → `"Departed"`, or `"No berth fits"` |
| `Ship()` | Empty ship, `cargo = 0` (null) |
| `Ship(len, dep, dest, cap)` | Checks values; on success allocates `new Container[capacity]`, sets status `"Waiting"`, takes an ID |
| `copyFrom(other)` | **Private helper: deep copy.** Allocates its own array and copies the containers one by one. Keeps the **same ID** (a copy is the same ship) |
| `Ship(const Ship&)` | Copy constructor → `copyFrom` |
| `operator=` | Self-assignment check, `delete[]` old array, then `copyFrom` |
| `~Ship()` | `delete[] cargo` |
| `operator+=(Container c)` | Adds `c` **only if** the ship is valid, not full, `c` is valid **and** going to the same destination. Otherwise it silently does nothing |
| `isFull()` | `count == capacity` |
| `getContainer(i)` | Returns a **reference** so Port can change the real container's status |
| `displayDetails()` | Prints the ship line, then each container indented |

### OOP concepts to check
- Rule of Three (copy constructor, assignment operator, destructor)
- Deep copy compared with shallow copy
- Operator overloading (`+=`, `=`)
- Dynamic memory (`new[]` / `delete[]`)

### Review checklist
- [ ] **Rule of Three complete?** Every path that sets `cargo` is matched by a `delete[]`
- [ ] Copy constructor: `cargo` isn't initialised before `copyFrom`. Is that OK? (Yes, `copyFrom` sets it first.)
- [ ] Copying a **default** `Ship` (capacity 0) → `new Container[0]`. Legal C++, and `delete[]` on it is fine
- [ ] `operator=` returns `*this` by reference, so `a = b = c` works
- [ ] `operator+=` fails **silently**. Should it return `bool` or print? Check how `main.cpp` relies on it (Tanish)
- [ ] `getContainer(index)` has **no bounds check**. Who guarantees `index < count`?
- [ ] Copying a ship keeps the same ID. Confirm this is wanted (Port copies ships when its array grows)
- [ ] `count == capacity` for an invalid ship is `0 == 0` → "full". Harmless?

### Test ideas
- Make a ship, add 2 containers, copy it, change a container's status in the copy → the original must **not** change (proves deep copy)
- `s = s;` must not crash
- `s += Container(5, 1, otherDestination)` → count unchanged

---

## 6. Tanish: `Port` and `main.cpp`

### What `Port` does

| Piece | Purpose |
|---|---|
| `Berth *berths`, `berthCount` | Dynamic array, size picked once in `setupBerths(n)` |
| `Ship *ships`, `shipCount` | Dynamic array that **grows by one** each time a new destination appears |
| `stepNumber` | Counter shown as `--- Step N ---` |
| `~Port()` | `delete[]` both arrays |
| `setupBerths(n)` / `setBerth(i, b)` | Allocate the berths, then fill slot `i` with a copy of `b` |
| `findShip(destination)` | Index of the ship going there, or -1. **One ship per destination.** |
| `findShipByID(id)` | Private; used inside `step()` to find the docked ship |
| `addShip(s)` | Makes a new array one bigger, copies the old ships across (uses `Ship::operator=`), adds `s`, deletes the old array |
| `totalContainers()` | Sum of `count` over all ships (main caps this at 15) |
| `step()` | **The simulation, see below** |
| `isFinished()` | `true` when no ship is `"Waiting"` or `"Docked"` |
| `displayStatus()` | All berths, all ships with their containers, and the total |

**`step()` in two phases (`port.cpp:53-110`):**
1. **Each busy berth:** find the docked ship's first `"Waiting"` container.
   - None left → ship becomes `"Departed"`, berth is released.
   - Otherwise the berth's crane tries it: `canLift` → `"Loaded"`, else `"Too heavy"`.
   - **One container per berth per step.**
2. **Each waiting ship:** try berths in order; dock at the first one that fits **and** is free.
   - It fits somewhere but every fitting berth is busy → stays `"Waiting"`.
   - It fits nowhere → `"No berth fits"` (counts as finished).

### What `main.cpp` does

| Function | Purpose |
|---|---|
| `readValue(prompt, int, int)` / `readValue(prompt, double, double)` | **Function overloading.** Repeats the question until the value is in range |
| `readYesNo` | Accepts only `y` / `n` |
| `showWelcome` | Banner |
| `setupPort` | Only once: asks for 1–5 berths, each with length, depth and crane lift |
| `addContainers` | Up to 15 containers in total. For a new destination it asks for the ship's details and creates the ship; then adds the container with `ship += Container(...)` |
| `runSimulation` | Step by step, run to the end, or back to the menu; shows final status when done |
| `main` | Menu loop 1–5 |

### OOP concepts to check
- Composition / aggregation (Port owns arrays of Berths and Ships)
- Dynamic array resizing
- Function overloading (`readValue`)
- Separation of concerns (all `cin` lives in `main.cpp`; classes only print)

### Review checklist
- [ ] **Port has a destructor but no copy constructor or `operator=`** (Rule of Three). Safe only because `Port` is always passed by reference. Should copying be disabled or implemented?
- [ ] `ships[findShipByID(...)]` in `step()`: if it ever returned -1 this would read `ships[-1]`. Can that happen?
- [ ] `setBerth(index, b)` has no bounds check
- [ ] `addShip`: does it leak memory or double-free? (Old array is deleted after copying; `Ship::operator=` does the deep copy)
- [ ] A ship that docks in phase 2 starts loading on the **next** step. Intended?
- [ ] A ship departs even if some containers are `"Too heavy"`: they stay listed on the ship with that status. Is that the behaviour we want to present?
- [ ] `isFinished()`: can the loop in `runSimulation` ever run forever? (Think: a ship that fits a berth that is always busy)
- [ ] **`readValue` with letters** (e.g. typing `abc`): `cin` goes into a fail state and the `while` loop never ends. Needs `cin.clear()` + `cin.ignore(...)`
- [ ] `addContainers`: "Added to Ship …" is printed without checking that `+=` really added it. Is that always true here? (Destination matches and full was checked, so yes)
- [ ] The 15-container limit is checked before each container, never in the middle of adding one

### Test ideas
- 1 berth, 2 destinations → second ship waits until the first departs
- Berth crane 10 t, container 20 t → `"Too heavy"`, ship still departs
- Ship length 300, every berth 200 → `"No berth fits"`, simulation still finishes
- Type a letter at the main menu → shows the input bug

---

## 7. Review order and cross-check

**Pass 1: own area (in parallel).** Everyone starts at the same time; nobody blocks anyone.

**Pass 2: cross-review (15–20 min each).** Read the next person's section of this guide and skim their code:

| Reviewer | Cross-reviews |
|---|---|
| Dharmik | Ship (Mithunn's): how containers are stored and copied |
| Ritu | Port `step()` (Tanish's): how berths and cranes are used |
| Mithunn | Container (Dharmik's): what a copied container looks like |
| Tanish | Berth/Crane (Ritu's): `canFit`, `dock`, `release` |

By the end of pass 2, **every class has been read by at least two people**, and together the four of you have covered all of them.

---

## 8. How to report findings

Use one line per finding so they are easy to merge:

```
[File:line] [Bug / Risk / Style / Question] Short description, plus a suggested fix
```

Example:
```
[main.cpp:11] [Bug] Typing a letter makes readValue loop forever. Add cin.clear(); cin.ignore(1000, '\n');
```

Send items that belong to another person's class to that person rather than fixing them yourself.
