#include "pch.h"

void Main()
{
    AllocConsole();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w+", stdout);
    SDK::Init();
    Log("Initalization Started \n");
    Log("EngineVersion: %f \n", SDK::UE::GetEngineVersion());
    Log("FortniteVersion: %f \n", SDK::UE::GetFortniteVersion());
    Log("ImageBase: %p \n", GetModuleHandle(0));
    FindAll();

    BeginHeaderFile();
    Append("GetNetMode", Offsets::WorldGetNetMode - SDK::UE::Memory::GetBaseAddress());
    Append("GIsClient", Offsets::GIsClient - SDK::UE::Memory::GetBaseAddress());
    Append("GIsServer", (Offsets::GIsClient + 1) - SDK::UE::Memory::GetBaseAddress());
    Append("KickPlayer", Offsets::KickPlayer - SDK::UE::Memory::GetBaseAddress());
    Append("InitListen", Offsets::NetDriverInitListen - SDK::UE::Memory::GetBaseAddress());
    Append("SetWorld", Offsets::NetDriverSetWorld - SDK::UE::Memory::GetBaseAddress());
    Append("ActorNetMode", Offsets::ActorNetMode - SDK::UE::Memory::GetBaseAddress(), true);
    Append("ChangeGameSessionId", Offsets::GameSessionIdPatch - SDK::UE::Memory::GetBaseAddress(), true);

    if (Offsets::EngineCreateNetDriver != -1) {
        Append("CreateNetDriver", Offsets::EngineCreateNetDriver - SDK::UE::Memory::GetBaseAddress());
    }
    else {
        Append("CreateNetDriver_Local", Offsets::CreateNetDriverLocal - SDK::UE::Memory::GetBaseAddress());
        Append("GetWorldContextFromObject", Offsets::GetWorldContextFromObject - SDK::UE::Memory::GetBaseAddress());
    }
    Append("ServerReplicateActors", Offsets::RepDriverServerReplicateActors - SDK::UE::Memory::GetBaseAddress());
    Append("TickFlush", Offsets::NetDriverTickFlush - SDK::UE::Memory::GetBaseAddress());
    Append("ReadyToStartMatch", GetFunctionAddress("AFortGameModeAthena", "ReadyToStartMatch"));
    Append("SpawnDefaultPawnFor", GetFunctionAddress("AFortGameModeAthena", "SpawnDefaultPawnFor"));
    Append("ServerAcknowledgePossesion", GetFunctionAddress("AFortPlayerControllerAthena", "ServerAcknowledgePossession"));
    endHeader();

    BeginFuncsFile();
    AppendFunc("CreateNetDriver", "UNetDriver*", "SDK::UEngine*, SDK::UWorld*, SDK::FName", "Addresses::CreateNetDriver");
    AppendFunc("InitListen", "bool", "SDK::UNetDriver*, SDK::UWorld*, SDK::FURL&, bool, FString", "Addresses::InitListen");
    AppendFunc("ServerReplicateActors", "void", "SDK::UReplicationDriver*, float", "Addresses::ServerReplicateActors");
    AppendFunc("SetWorld", "void", "SDK::UNetDriver*, SDK::UWorld*", "Addresses::SetWorld");
    EndFuncHeader();

    Log("Finished Generating Headers.");
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::thread(Main).detach();
        break;
    }
    return TRUE;
}

