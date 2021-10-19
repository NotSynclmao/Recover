#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "offsets.h"
#include "ue4.h"
#include "memory.h"
#include "util.h"

namespace core
{
	UE4::GObjects* GlobalObjects;
	UE4::UObject** GWorld;

	UE4::UObject* CheatManager;
	UE4::UObject* GameMode;
	UE4::UObject* Controller;
	UE4::UObject* Pawn;

	bool bInFrontend;
	bool bIsLoading;
	bool bIsInGame;
	bool bHasAlreadyTriggeredEvent;
	bool bEnableKeybinds;


	static DWORD WINAPI RiftPlayerPawnTick(LPVOID lpParam)
	{
		while (true)
		{
			if (bEnableKeybinds)
			{
				bool StopHoldingKey = false;
				if (GetKeyState(VK_SPACE) & 0x8000)
				{
					auto Jump_Object = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Character.Jump");
					if (StopHoldingKey == false)
					{
						StopHoldingKey = true;

						UE4::ProcessEvent(Pawn, Jump_Object, nullptr, 0);
					}
				}

				


				else if (GetKeyState(VK_F2) & 0x8000)
				{
					if (StopHoldingKey == false)
					{
						StopHoldingKey = true;
						auto func = GlobalObjects->FindObjectByFullName("Function /Script/Engine.PlayerController.SwitchLevel");
						if (func)
						{
							bInFrontend = true;
							bIsLoading = false;
							bIsInGame = false;
							bEnableKeybinds = false;
							bHasAlreadyTriggeredEvent = false;

							const char* CharMapName = ("Frontend");
							std::string StrMapName(CharMapName);
							std::wstring FStrMapName = std::wstring(StrMapName.begin(), StrMapName.end());
							UE4::FString MapName = FStrMapName.c_str();
							UE4::ProcessEvent(UE4::GetFirstPlayerController(*GWorld), func, &MapName, 0);
						}
					}
				}

				else
					StopHoldingKey = false;

			}
			else
			{
				if (GetKeyState(VK_F3) & 0x8000)
				{
					bool StopHoldingKey = false;
					if (StopHoldingKey == false)
					{
						StopHoldingKey = true;

						auto func = GlobalObjects->FindObjectByFullName("Function /Script/Engine.PlayerController.SwitchLevel");
						if (func)
						{
							const char* CharMapName = ("Athena_Terrain");
							std::string StrMapName(CharMapName);
							std::wstring WStrMapName = std::wstring(StrMapName.begin(), StrMapName.end());
							UE4::FString MapName = WStrMapName.c_str();
							UE4::ProcessEvent(UE4::GetFirstPlayerController(*GWorld), func, &MapName, 0);
							bInFrontend = false;
							bIsLoading = true;
						}
					}
				}
			}

			Sleep(1000 / 60);
		}

		return 0;
	}


	static void LoadAthena()
	{
		Controller = (UE4::UObject*)UE4::GetFirstPlayerController(*GWorld);
		std::cout << Controller << std::endl;
		// -------------
		// Cheat Manager
		void* cheatManager = UE4::StaticConstructObject_Internal
		(
			GlobalObjects->FindObjectByFullName("Class /Script/Engine.CheatManager")
			, Controller
			, nullptr
			, 0
			, 0
			, nullptr
			, false
			, nullptr
			, false
		);
		// -------------
		// Cheat Manager
		std::cout << cheatManager << std::endl;

		// -------------
		// Summon
		auto Summon_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.Summon");
		std::cout << "SUMMON OFFSET:???!?" << Summon_offset << std::endl;

		if (Summon_offset)
		{
			struct
			{
				UE4::FString ClassName;
			} SummonParams;

			SummonParams.ClassName = L"PlayerPawn_Athena_C";

			UE4::ProcessEvent(cheatManager, Summon_offset, &SummonParams, 0);

			std::cout << "Summoned!" << std::endl;
		}
		// -------------
		// Summon


		Pawn = GlobalObjects->FindObject("PlayerPawn_Athena_C_");

		// Possess
		// -------------
		auto Possess_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Controller.Possess");

		if (Possess_offset)
		{
			if (Pawn)
			{
				struct
				{
					void* InPawn;
				} PossessParams;

				PossessParams.InPawn = Pawn;

				UE4::ProcessEvent(Controller, Possess_offset, &PossessParams, 0);

				std::cout << "Possessed!" << std::endl;
			}
		}
		// -------------
		// Possess


		// ServerReadyToStartMatch
		// -------------
		auto ServerReadyToStartMatch_offset = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerController.ServerReadyToStartMatch");

		if (ServerReadyToStartMatch_offset)
		{
			UE4::ProcessEvent(Controller, ServerReadyToStartMatch_offset, nullptr, 0);

			std::cout << "Controller is ready to start match!" << std::endl;
		}
		// -------------
		// ServerReadyToStartMatch


		// -------------
		// StartMatch
		auto StartMatch_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.GameMode.StartMatch");

		if (StartMatch_offset)
		{
			auto mode = GlobalObjects->FindObject("Athena_GameMode_C_");

			if (mode)
			{
				UE4::ProcessEvent(mode, StartMatch_offset, nullptr, 0);

				std::cout << "Started match!" << std::endl;
			}
		}
		// -------------
		// StartMatch

		std::cout << "Done!" << std::endl;

		// -------------
		// Extras
		auto K2_TeleportTo_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Actor.K2_TeleportTo");

		if (K2_TeleportTo_offset)
		{
			struct
			{
				UE4::FVector DestLocation;
				UE4::FRotator DestRotation;
			} TeleportParams;

			TeleportParams.DestLocation = UE4::FVector{ 4024.503662, -631.651001, 3533.149902 }; // Salty Springs
			TeleportParams.DestRotation = UE4::FRotator{ 1,1,1 };

			UE4::ProcessEvent(Pawn, K2_TeleportTo_offset, &TeleportParams, 0);

			std::cout << "Teleported to Salty Springs!" << std::endl;
		}


		auto DestroyAll_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.DestroyAll");

		if (DestroyAll_offset)
		{
			struct
			{
				void* aClass;
			} DestroyParams;

			DestroyParams.aClass = GlobalObjects->FindObject("FortHLODSMActor");

			UE4::ProcessEvent(cheatManager, DestroyAll_offset, &DestroyParams, 0);

			std::cout << "Destroyed all instances of FortHLODSMActor!" << std::endl;
		}
		// -------------
		// Extras

		// Character
		auto PlayerState = ReadPointer(Controller, 0x220);
		if (PlayerState)
		{
			auto head = GlobalObjects->FindObjectByFullName("CustomCharacterPart /Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
			auto body = GlobalObjects->FindObjectByFullName("CustomCharacterPart /Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01"); 

			struct FCustomCharacterData
			{
				unsigned char WasPartReplicatedFlags;
				unsigned char UnknownData00[0x3];
				int RequiredVariantPartFlags;
				void* Parts[0x6];
			};

			class AFortPlayerState
			{
			public:
				unsigned char UnknownData00[0x4D0];
				FCustomCharacterData CharacterData;
			};


			AFortPlayerState* MyFortPlayerState = reinterpret_cast<AFortPlayerState*>(PlayerState);

			MyFortPlayerState->CharacterData.Parts[0] = head;
			MyFortPlayerState->CharacterData.Parts[1] = body;

			auto CharacterData_offset = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerState.OnRep_CharacterData");

			if (CharacterData_offset)
			{
				UE4::ProcessEvent(PlayerState, CharacterData_offset, nullptr, 0);
			}
		}

		// Hide HUD for event
		auto hud = ReadPointer(Controller, 0x2A8);
		UE4::ProcessEvent(hud, GlobalObjects->FindObjectByFullName("Function /Script/Engine.HUD.ShowHUD"), nullptr, 0);

		// Start NightNight
		auto obj = GlobalObjects->FindObjectByFullName("BP_NightNight_Scripting_C /Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.BP_NightNight_Scripting_2");
		auto func = GlobalObjects->FindObjectByFullName("Function /Game/Athena/Prototype/Blueprints/NightNight/BP_NightNight_Scripting.BP_NightNight_Scripting_C.LoadNightNightLevel");
		if (obj && func)
		{
			struct
			{
				bool Condition;
			} params;
			params.Condition = true;
			UE4::ProcessEvent(obj, func, &params, 0);

			// Play Level Sequence
			UE4::ProcessEvent(GlobalObjects->FindObjectByFullName("LevelSequencePlayer /Game/Athena/Maps/Test/S10/NightNightSequenceMap.NightNightSequenceMap.PersistentLevel.NightNight.AnimationPlayer"), GlobalObjects->FindObjectByFullName("Function /Script/MovieScene.MovieSceneSequencePlayer.Play"), nullptr, 0);
		}
	};

	static void Setup()
	{
		GWorld = reinterpret_cast<UE4::UObject**>(Memory::FindPattern(GWORLD_PATTERN, true, 3));
		UE4::GObjectsAddr = Memory::FindPattern(GOBJECT_PATTERN, true, 10);
		UE4::FreeAddr = Memory::FindPattern(FMEMORYFREE_PATTERN);
		UE4::GetObjNameAddr = Memory::FindPattern(GETOBJNAME_PATTERN);
		UE4::GetFirstPlayerControllerAddr = Memory::FindPattern(GETFIRSTPLAYERCONTROLLER_PATTERN);
		UE4::ProcessEventAddr = Memory::FindPattern(PROCESSEVENT_PATTERN);
		UE4::StaticConstructObject_InternalAddr = Memory::FindPattern(STATICCONSTRUCTOBJECTINTERNAL_PATTERN);

		GlobalObjects = reinterpret_cast<UE4::GObjects*>(UE4::GObjectsAddr);
		LoadAthena();
	};
}