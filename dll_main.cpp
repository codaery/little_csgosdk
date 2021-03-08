#include <iostream>
#include <Windows.h>
#include <memory>
#include <vector>
#include "csgo.hpp"

#define csgo_version 13783

unsigned long __stdcall dll_thread() {

	utilities::console::attach_console(); 
	utilities::console::message_console("\n");
	utilities::console::message_console(" credits: github.com/carterrrj");
	return true;
}

unsigned long __stdcall deinitialize() {

	utilities::console::detach_console();
	Sleep(100);
	ExitProcess(0);

	return false;
}

unsigned long __stdcall initialize() {

	interfaces::initialize();

	if (interfaces::engine->get_engine_build_number() == csgo_version) {
		dll_thread();
	}
	else {
		utilities::console::attach_console(); utilities::console::message_console("\n");

		utilities::console::message_console(" [-] counter-strike: global offensive is updated\n");

		Sleep(500);

		utilities::console::message_console(" [!] enter the new version of the game in the 'csgo_version' parameter\n");

		Sleep(5000);

		utilities::console::message_console(" [-] wait for shutdown\n");

		Sleep(1000);

		deinitialize();
	}

	return true;
}

BOOL __stdcall DllMain(const HMODULE instance, const unsigned long reason, const void* reserved) {

	switch (reason)
	{
	   case DLL_PROCESS_ATTACH: {
	  	   CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)initialize, instance, NULL, nullptr);
		   break;
	   }

	   case DLL_PROCESS_DETACH: {
		   deinitialize();
		   break;
	   }
	}
	return true;
}