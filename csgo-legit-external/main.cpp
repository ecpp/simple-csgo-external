#include "src/utils/memory.h"
#include "src/utils/vector.h"
#include <atomic>
#include <thread>
#include <iostream>
#include "src/hacks/hacks.h"
#include "globals.h"


constexpr Vector3 CalculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

__declspec(align(16)) struct Color
{
	constexpr Color(const float r, const float g, const float b, const float a = 1.f) noexcept :
		r(r), g(g), b(b), a(a) { }

	float r, g, b, a;
};

int main()
{
	MemoryEditor memory{ "csgo.exe" };
	while (!memory.IsProcessOpen()) {
		std::cout << "Failed to find csgo.exe" << std::endl;
		
	}
	std::cout << "ID (csgo.exe):" << memory.GetProcessId() << std::endl;

	while (!memory.GetModuleAddress("serverbrowser.dll")) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << "Waiting for serverbrowser.dll" << std::endl;
	}

	globals::client = memory.GetModuleAddress("client.dll");
	globals::engine = memory.GetModuleAddress("engine.dll");

	if (!globals::client || !globals::engine) {
		std::cout << "Failed to find client.dll or engine.dll" << std::endl;
		return 0;
	}

	std::cout << "ID (client.dll):" << globals::client << std::endl;
	std::cout << "ID (engine.dll):" << globals::engine << std::endl;

	std::thread{ hax::aim, memory }.detach();
	std::thread(hax::bunny, memory).detach();
	std::thread(hax::radar, memory).detach();
	std::thread(hax::legitAim, memory).detach();

	std::thread getUserInput([&]() {
		while (globals::runhax) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			std::cout << "1) Rage Aim Status: " << globals::isaim << std::endl;
			std::cout << "2) Legit Aim Status: " << globals::islegitAim << std::endl;
			std::cout << "3) Radar Status: " << globals::isradar << std::endl;
			std::cout << "4) Bhop: " << globals::isbunny << std::endl;
			std::cout << "Enter the number you want to activate." << std::endl;
			std::string input;
			std::cin >> input;
			if (input == "1") {
				globals::islegitAim = false;
				globals::isaim = !globals::isaim;
				std::cout << "aim: " << globals::isaim << std::endl;

			}
			else if (input == "2") {
				globals::isaim = false;
				globals::islegitAim = !globals::islegitAim;
				std::cout << "legitAim: " << globals::islegitAim << std::endl;
			}
			else if (input == "3") {
				globals::isradar = !globals::isradar;
				std::cout << "radar: " << globals::isradar << std::endl;
			}
			else if (input == "4") {
				globals::isbunny = !globals::isbunny;
				std::cout << "bhop: " << globals::isbunny << std::endl;
			}
			else {
				globals::runhax = false;
				std::cout << "Exiting..." << std::endl;
				return 0;
			}
			system("CLS");

		}
		});

	getUserInput.join();

	return 0;
}
