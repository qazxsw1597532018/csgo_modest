#include "csgo.hpp"

namespace ctx
{
	csgo_t csgo = csgo_t{};
	client_t client = client_t{};
	mem_t mem = mem_t{};
	config_t config = config_t{};
}

BOOL WINAPI detach()
{
	shared::input::undo();

	hooks::undo();

	shared::log::detach();

	return TRUE;
}

DWORD WINAPI entry( LPVOID lpThreadParameter )
{
	if ( !shared::input::init( "Valve001" ) )
		goto DETACH;

	LOG( "Initialized Input!" );

	menu::init();

	hooks::init();

	LOG( "Initialized Hooks!" );

	netvars::init();

	LOG( "Initialized Netvars!" );

	LOG( "Cheat Attached!" );

	while ( !shared::input::get_key_info( VK_END ).m_state )
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

DETACH:
	LOG( "Cheat Detached!" );

	std::this_thread::sleep_for( std::chrono::milliseconds( 150 ) );

	detach();

	FreeLibraryAndExitThread( static_cast< HMODULE >( lpThreadParameter ), EXIT_SUCCESS );
}

BOOL APIENTRY DllMain( _In_ HINSTANCE hinstDLL,
					   _In_ DWORD     fdwReason,
					   _In_ LPVOID    lpvReserved )
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( hinstDLL );

		if ( auto handle = CreateThread( nullptr, NULL, entry, hinstDLL, NULL, nullptr ) )
		{
			CloseHandle( handle );
		}
	}
	else if ( fdwReason == DLL_PROCESS_DETACH &&
			  !lpvReserved )
	{
		return detach();
	}

	return TRUE;
}