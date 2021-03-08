#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>

enum class interface_type { 
	index, bruteforce
};

namespace utilities {
	namespace engine {

		template <typename ret, interface_type type>
		ret* get_interface(const std::string& module_name, const std::string& interface_name) {
			using create_interface_fn = void* (*)(const char*, int*);
			const auto fn = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));

			if (fn) {
				void* result = nullptr;

				switch (type) {
				case interface_type::index:
					result = fn(interface_name.c_str(), nullptr);

					break;
				case interface_type::bruteforce:
					char buf[128];

					for (uint32_t i = 0; i <= 100; ++i) {
						memset(static_cast<void*>(buf), 0, sizeof buf);

						result = fn(interface_name.c_str(), nullptr);

						if (result)
							break;
					}

					break;
				}

				if (!result)
					throw std::runtime_error(interface_name + " wasn't found in " + module_name);

				return static_cast<ret*>(result);
			}

			throw std::runtime_error(module_name + " wasn't found");
		}

		std::uint8_t* pattern_scan(const char* module_name, const char* signature_value) {
			const auto module_handle = GetModuleHandleA((LPCSTR)module_name);

			if (!module_handle)
				return nullptr;

			static auto pattern_to_byte = [](const char* pattern) {
				auto bytes = std::vector<int>{};
				auto start = const_cast<char*>(pattern);
				auto end = const_cast<char*>(pattern) + std::strlen(pattern);

				for (auto current = start; current < end; ++current) {
					if (*current == '?') {
						++current;

						if (*current == '?')
							++current;

						bytes.push_back(-1);
					}
					else {
						bytes.push_back(std::strtoul(current, &current, 16));
					}
				}
				return bytes;
			};

			auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
			auto nt_headers =
				reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

			auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
			auto pattern_bytes = pattern_to_byte(signature_value);
			auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

			auto s = pattern_bytes.size();
			auto d = pattern_bytes.data();

			for (auto i = 0ul; i < size_of_image - s; ++i) {
				bool found = true;

				for (auto j = 0ul; j < s; ++j) {
					if (scan_bytes[i + j] != d[j] && d[j] != -1) {
						found = false;
						break;
					}
				}
				if (found)
					return &scan_bytes[i];
			}
		}
	}

	namespace console {
		void message_console(const char* msg) {
			std::cout << msg;
		}
		void attach_console() {
			AllocConsole();
			freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
			freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		}
		void detach_console() {
			FreeConsole();
		}
	}

	namespace math {
		template <typename t> t clamp(t value, t min, t max) {
			if (value > max) {
				return max;
			}
			if (value < min) {
				return min;
			}
			return value;
		}
	}

	class memory {
	public:
		template <typename T>
		T& read(uintptr_t offset) {
			return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
		}

		template <typename T>
		void write(uintptr_t offset, T data) {
			*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
		}

	};

	template<typename T>
	__forceinline static T call_virtual(void* ppClass, int index) {
		int* table = *(int**)ppClass;
		int address = table[index];
		return (T)(address);
	}
}