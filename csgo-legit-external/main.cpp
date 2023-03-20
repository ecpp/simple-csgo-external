#include "src/utils/memory.h"
#include "src/utils/vector.h"
#include <atomic>
#include <thread>
#include <iostream>
#include "src/hacks/hacks.h"
#include "globals.h"

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
	std::thread(hax::noRecoil, memory).detach();

	std::thread getUserInput([&]() {
		while (globals::runhax) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			std::cout << "1) Rage Aim Status: " << globals::isaim << std::endl;
			std::cout << "2) Legit Aim Status: " << globals::islegitAim << std::endl;
			std::cout << "3) Radar Status: " << globals::isradar << std::endl;
			std::cout << "4) Bhop Status: " << globals::isbunny << std::endl;
			std::cout << "5) No Recoil Status: " << globals::isNoRecoil<< std::endl;
			std::cout << "6) Exit. " << std::endl;
			std::cout << "Enter the number you want to activate: ";
			std::string input;
			std::cin >> input;
			if (input == "1") {
				globals::islegitAim = false;
				globals::isaim = !globals::isaim;

			}
			else if (input == "2") {
				globals::isaim = false;
				globals::islegitAim = !globals::islegitAim;
			}
			else if (input == "3") {
				globals::isradar = !globals::isradar;
			}
			else if (input == "4") {
				globals::isbunny = !globals::isbunny;
			}
			else if (input == "5") {
				globals::isNoRecoil = !globals::isNoRecoil;
			}
			else {
				std::cout << "Exiting..." << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				globals::runhax = false;
				return 0;
			}
			system("CLS");

		}
		});

	getUserInput.join();

	return 0;
}
