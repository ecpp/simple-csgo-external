#include "hacks.h"
#include <thread>
#include "../../globals.h"
#include <iostream>
#include <WinUser.h>
#include "../utils/memory.h"
#include "../utils/vector.h"

constexpr Vector3 CalculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

void hax::aim(const MemoryEditor& memory) noexcept
{
	while (globals::runhax)
	{
		if (globals::isaim) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			// aimbot key
			if (!GetAsyncKeyState(0x58))
				continue;

			// get local player
			const auto localPlayer = memory.Read<std::uintptr_t>(globals::client + offset::dwLocalPlayer);
			const auto localTeam = memory.Read<std::int32_t>(localPlayer + offset::m_iTeamNum);
			const auto localPlayerFlags = memory.Read<std::uintptr_t>(localPlayer + offset::m_fFlags);


			// eye position = origin + viewOffset
			const auto localEyePosition = memory.Read<Vector3>(localPlayer + offset::m_vecOrigin) +
				memory.Read<Vector3>(localPlayer + offset::m_vecViewOffset);

			const auto clientState = memory.Read<std::uintptr_t>(globals::engine + offset::dwClientState);

			const auto localPlayerId =
				memory.Read<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);

			//bunny(localPlayer, localTeam, localPlayerFlags, memory, client);

			const auto viewAngles = memory.Read<Vector3>(clientState + offset::dwClientState_ViewAngles);
			const auto aimPunch = memory.Read<Vector3>(localPlayer + offset::m_aimPunchAngle) * 2;

			// aimbot fov
			auto bestFov = 50.f;
			auto bestAngle = Vector3{ };

			for (auto i = 1; i <= 32; ++i)
			{
				const auto player = memory.Read<std::uintptr_t>(globals::client + offset::dwEntityList + i * 0x10);

				if (memory.Read<std::int32_t>(player + offset::m_iTeamNum) == localTeam)
					continue;

				if (memory.Read<bool>(player + offset::m_bDormant))
					continue;

				if (memory.Read<std::int32_t>(player + offset::m_lifeState))
					continue;

				if (memory.Read<std::int32_t>(player + offset::m_bSpottedByMask) & (1 << localPlayerId))
				{
					const auto boneMatrix = memory.Read<std::uintptr_t>(player + offset::m_dwBoneMatrix);

					// pos of player head in 3d space
					// 8 is the head bone index :)
					const auto playerHeadPosition = Vector3{
						memory.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
						memory.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
						memory.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
					};

					const auto angle = CalculateAngle(
						localEyePosition,
						playerHeadPosition,
						viewAngles + aimPunch
					);

					const auto fov = std::hypot(angle.x, angle.y);

					if (fov < bestFov)
					{
						bestFov = fov;
						bestAngle = angle;
					}
				}
			}
			// if we have a best angle, do aimbot
			if (!bestAngle.IsZero()) {
				memory.Write<Vector3>(clientState + offset::dwClientState_ViewAngles, viewAngles + bestAngle); // smoothing
			}
		}


	}
}

void hax::bunny(const MemoryEditor& memory) noexcept
{
	while (globals::runhax)
	{
		if (globals::isbunny) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			// bunny key
			if (!GetAsyncKeyState(VK_SPACE))
				continue;
			// get local player
			const auto localPlayer = memory.Read<std::uintptr_t>(globals::client + offset::dwLocalPlayer);
			const auto localTeam = memory.Read<std::int32_t>(localPlayer + offset::m_iTeamNum);
			const auto localPlayerFlags = memory.Read<std::uintptr_t>(localPlayer + offset::m_fFlags);
			if (localPlayerFlags & (1 << 0))
			{
				memory.Write<std::int32_t>(globals::client + offset::dwForceJump, 5);
				std::this_thread::sleep_for(std::chrono::milliseconds(15));
				memory.Write<std::int32_t>(globals::client + offset::dwForceJump, 4);
			}
		}
	}
}

void hax::radar(const MemoryEditor& memory) noexcept {

	while (globals::runhax)
	{
		if (globals::isradar) {

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			const auto localPlayer = memory.Read<std::uintptr_t>(globals::client + offset::dwLocalPlayer);
			const auto localTeam = memory.Read<std::int32_t>(localPlayer + offset::m_iTeamNum);
			const auto localPlayerFlags = memory.Read<std::uintptr_t>(localPlayer + offset::m_fFlags);
			for (auto i = 1; i <= 32; ++i)
			{
				const auto player = memory.Read<std::uintptr_t>(globals::client + offset::dwEntityList + i * 0x10);

				if (memory.Read<std::int32_t>(player + offset::m_iTeamNum) == localTeam)
					continue;

				if (memory.Read<bool>(player + offset::m_bDormant))
					continue;

				if (memory.Read<std::int32_t>(player + offset::m_lifeState))
					continue;

				memory.Write<bool>(player + offset::m_bSpotted, true);
			}
		}
	}
}

void hax::legitAim(const MemoryEditor& memory) noexcept
{
	while (globals::runhax)
	{
		if (globals::islegitAim) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			// aimbot key
			if (!GetAsyncKeyState(0x58))
				continue;

			// get local player
			const auto localPlayer = memory.Read<std::uintptr_t>(globals::client + offset::dwLocalPlayer);
			const auto localTeam = memory.Read<std::int32_t>(localPlayer + offset::m_iTeamNum);
			const auto localPlayerFlags = memory.Read<std::uintptr_t>(localPlayer + offset::m_fFlags);


			// eye position = origin + viewOffset
			const auto localEyePosition = memory.Read<Vector3>(localPlayer + offset::m_vecOrigin) +
				memory.Read<Vector3>(localPlayer + offset::m_vecViewOffset);

			const auto clientState = memory.Read<std::uintptr_t>(globals::engine + offset::dwClientState);

			const auto localPlayerId =
				memory.Read<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);

			const auto viewAngles = memory.Read<Vector3>(clientState + offset::dwClientState_ViewAngles);
			const auto aimPunch = memory.Read<Vector3>(localPlayer + offset::m_aimPunchAngle) * 2;

			// aimbot fov
			auto bestFov = 5.f;
			auto bestAngle = Vector3{};

			for (auto i = 1; i <= 32; ++i)
			{
				const auto player = memory.Read<std::uintptr_t>(globals::client + offset::dwEntityList + i * 0x10);

				if (memory.Read<std::int32_t>(player + offset::m_iTeamNum) == localTeam)
					continue;

				if (memory.Read<bool>(player + offset::m_bDormant))
					continue;

				if (memory.Read<std::int32_t>(player + offset::m_lifeState))
					continue;

				if (memory.Read<std::int32_t>(player + offset::m_bSpottedByMask) & (1 << localPlayerId))
				{
					const auto boneMatrix = memory.Read<std::uintptr_t>(player + offset::m_dwBoneMatrix);

					// pos of player head in 3d space
					// 8 is the head bone index :)
					const auto playerHeadPosition = Vector3{
						memory.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
						memory.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
						memory.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
					};

					const auto angle = CalculateAngle(
						localEyePosition,
						playerHeadPosition,
						viewAngles + aimPunch
					);

					const auto fov = std::hypot(angle.x, angle.y);

					if (fov < bestFov)
					{
						bestFov = fov;
						bestAngle = angle;
					}
				}
			}

			// if we have a best angle, do aimbot
			if (!bestAngle.IsZero())
			{
				auto targetAngle = viewAngles + bestAngle;

				// calculate delta angle
				const auto deltaAngle = targetAngle - viewAngles;

				// calculate new angle
				const auto newAngle = viewAngles + deltaAngle * globals::legitAimSmooth;

				// write new angle
				memory.Write<Vector3>(clientState + offset::dwClientState_ViewAngles, newAngle);
			}
		}
	}
}

void hax::noRecoil(const MemoryEditor& memory) noexcept
{
	auto oldPunch = Vector3{};
	while (globals::runhax)
	{
		if (globals::isNoRecoil) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			const auto localPlayer = memory.Read<std::uintptr_t>(globals::client + offset::dwLocalPlayer);
			const auto localTeam = memory.Read<std::int32_t>(localPlayer + offset::m_iTeamNum);
			const auto localPlayerFlags = memory.Read<std::uintptr_t>(localPlayer + offset::m_fFlags);
			const auto clientState = memory.Read<std::uintptr_t>(globals::engine + offset::dwClientState);
			const auto localPlayerId =
				memory.Read<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);
			const auto shotsFired = memory.Read<std::int32_t>(localPlayer + offset::m_iShotsFired);
			if (shotsFired > 1) {
				const auto aimPunch = memory.Read<Vector3>(localPlayer + offset::m_aimPunchAngle);
				const auto viewAngles = memory.Read<Vector3>(clientState + offset::dwClientState_ViewAngles);
				auto newAngle = viewAngles + oldPunch - aimPunch * 2;
				
				//angle limit check
				if (newAngle.x > 89.0f)
					newAngle.x = 89.0f;

				if (newAngle.x < -89.0f)
					newAngle.x = -89.0f;

				if (newAngle.y > 180.0f)
					newAngle.y = 180.0f;

				if (newAngle.y < -180.0f)
					newAngle.y = -180.0f;


				memory.Write<Vector3>(clientState + offset::dwClientState_ViewAngles, newAngle);
				oldPunch = aimPunch * 2;
			}
			else {
				oldPunch = 0;
			}
			

		}
	}
}