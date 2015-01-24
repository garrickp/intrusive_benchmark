/*
	Microbenchmarks for intrusively linked lists compared to std::vector in C++

*/

#include <iostream>
#include <time.h>
#include <numeric>
#include <algorithm>
#include <vector>

using namespace std;

const auto GRAVITY = -9.086;
const auto PROJECTILE_LIMIT = 50000;

class Projectile {
	public:
		float x;
		float y; // +y is up
		float z;
		float vel_x;
		float vel_y; // +y is up
		float vel_z;

		Projectile(float x, float y, float z, float vel_x, float vel_y, float vel_z);
		~Projectile();

		void ApplyGravity(double elapsedSec);
		void ApplyVelocity(double elapsedSec);
		bool DidImpactGround();
		void Render();
};

Projectile::Projectile(float x, float y, float z, float vel_x, float vel_y, float vel_z): x(x), y(y), z(z), vel_x(vel_x), vel_y(vel_y), vel_z(vel_z) { }

Projectile::~Projectile() { }

void Projectile::ApplyGravity(double elapsedSec) {
	vel_y += (GRAVITY * elapsedSec);
}

void Projectile::ApplyVelocity(double elapsedSec) {
	x += (vel_x * elapsedSec);
	y += (vel_y * elapsedSec);
	z += (vel_z * elapsedSec);
}

void Projectile::Render() {
	// Empty body for benchmark case
}

bool Projectile::DidImpactGround() {
	if (y <= 0) {
		return true;
	}
	return false;
}

int main() {
	vector<Projectile *> allItems;
	vector<Projectile *> renderables;
	vector<Projectile *> projectiles;

	vector<double> frameTimings;

	static const auto bulletInterval = 0.002 * float(CLOCKS_PER_SEC);

	auto start = clock();
	auto lastLoopTime = start;

	{
		auto projectile = new Projectile(0, 2, 0, 100, 0, 0);
		allItems.insert(allItems.begin(), projectile);
		renderables.insert(renderables.begin(), projectile);
		projectiles.insert(projectiles.begin(), projectile);
	}

	int projectilesGenerated = 1;

	while (!projectiles.empty()) {
		auto renderLoopStart = clock();

		auto elapsedSec = (renderLoopStart - lastLoopTime) / float(CLOCKS_PER_SEC);
		auto sinceStart = renderLoopStart - start;

		// Add a projectile to the stack every 50th of a second
		if ((projectilesGenerated < PROJECTILE_LIMIT) && (double(1 + projectilesGenerated) * bulletInterval <= sinceStart)) {
			auto projectile = new Projectile(0, 2, 0, 100, 0, 0);
			allItems.insert(allItems.begin(), projectile);
			renderables.insert(renderables.begin(), projectile);
			projectiles.insert(projectiles.begin(), projectile);
			projectilesGenerated += 1;
		}

		auto phyItem = projectiles.begin();
		while (phyItem != projectiles.end()) {
			(*phyItem)->ApplyGravity(elapsedSec);
			(*phyItem)->ApplyVelocity(elapsedSec);

			if ((*phyItem)->DidImpactGround()) {
				phyItem = projectiles.erase(phyItem);
				for (auto item = allItems.begin(); item != allItems.end(); item++) {
					if (item == phyItem) {
						allItems.erase(item);
						break;
					}
				}
				for (auto item = renderables.begin(); item != renderables.end(); item++) {
					if (item == phyItem) {
						renderables.erase(item);
						break;
					}
				}
			} else {
				phyItem++;
			}
		}

		for (auto rendItem = renderables.begin(); rendItem != renderables.end(); rendItem++) {
			(*rendItem)->Render();
		}

		auto renderLoopEnd = clock();
		lastLoopTime = renderLoopStart;

		frameTimings.push_back((renderLoopEnd - renderLoopStart) / float(CLOCKS_PER_SEC));
	}
	auto end = clock();

	auto totalRunTime = (end - start) / float(CLOCKS_PER_SEC);

	// Sort frameTimings & print out low, high, avg and median timings.
	sort(frameTimings.begin(), frameTimings.end());
	auto sum = accumulate(frameTimings.begin(), frameTimings.end(), float(0));

	cout << fixed;
	cout << "Minimum: " << frameTimings[0] << endl;
	cout << "Maximum: " << frameTimings[frameTimings.size() - 1] << endl;
	cout << "Average: " << (sum / frameTimings.size()) << endl;
	cout << "Median: " << frameTimings[int(frameTimings.size() / 2)] << endl;
	cout << "Total Frames Rendered: " << frameTimings.size() << endl;
	cout << "Total Run Time: " << totalRunTime << endl;
}

