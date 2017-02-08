#include <cassert>
#include <iostream>
#include "Entity.hpp"
#include "Moving.hpp"
#include "AxisMoving.hpp"
#include "FreeMoving.hpp"
#include "AxisSighted.hpp"

using namespace lif;
using namespace std;

int main() {
	Entity e;
	e.addComponent(new AxisMoving(e, 10));
	e.addComponent(new FreeMoving(e, 10));
	e.addComponent(new AxisSighted(e, 10));
	cout << "init entity\n";
	e.init();
	cout << "done init entity\n";

	cout << e.get<Moving>() << endl;
	cout << e.get<AxisMoving>() << endl;
	cout << e.get<Sighted>() << endl;
}
