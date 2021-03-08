#pragma once
#include "utilities.hpp"
#include "vector.hpp"
#include "view_matrix.hpp"

class c_client_class;

struct player_info_t {
	int64_t __pad0;
	union {
		int64_t xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];
	int userid;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

class iv_engine_client {
public:
	int get_local_player() {
		using original_fn = int(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[12](this);
	}
	int get_player_for_user_id(int user_id) {
		using original_fn = int(__thiscall*)(iv_engine_client*, int);
		return (*(original_fn**)this)[9](this, user_id);
	}
	unsigned int get_engine_build_number()
	{
		using original_fn = unsigned int(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[104](this);
	}
	unsigned int get_client_version(int build) {
		using original_fn = unsigned int(__thiscall*)(iv_engine_client*, unsigned int);
		return (*(original_fn**)this)[219](this, build);
	}
	void get_player_info(int index, player_info_t* info) {
		using original_fn = void(__thiscall*)(iv_engine_client*, int, player_info_t*);
		return (*(original_fn**)this)[8](this, index, info);
	}
	void get_screen_size(int& width, int& height) {
		using original_fn = void(__thiscall*)(iv_engine_client*, int&, int&);
		return (*(original_fn**)this)[5](this, width, height);
	}
	bool is_in_game() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[26](this);
	}
	bool is_connected() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[27](this);
	}
	void set_view_angles(vec3_t& angles) {
		using original_fn = void(__thiscall*)(iv_engine_client*, vec3_t&);
		return (*(original_fn**)this)[19](this, angles);
	}
	vec3_t get_view_angles() {
		vec3_t temp;
		using original_fn = void(__thiscall*)(iv_engine_client*, vec3_t&);
		(*(original_fn**)this)[18](this, temp);
		return temp;
	}
	void get_view_angles_alternative(vec3_t& angles) {
		return utilities::call_virtual<void(__thiscall*)(void*, vec3_t&)>(this, 18)(this, angles);
	}
	view_matrix_t& world_to_screen_matrix() {
		view_matrix_t temp;
		using original_fn = view_matrix_t & (__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[37](this);
	}
	bool is_taking_screenshot() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[92](this);
	}
};

class i_client_entity_list {
public:
	void* get_client_entity(int index) {
		using original_fn = void*(__thiscall*)(i_client_entity_list*, int);
		return (*(original_fn**)this)[3](this, index);
	}
	void* get_client_entity_handle(uintptr_t handle) {
		using original_fn = void*(__thiscall*)(i_client_entity_list*, uintptr_t);
		return (*(original_fn**)this)[4](this, handle);
	}
	int get_highest_index() {
		using original_fn = int(__thiscall*)(i_client_entity_list*);
		return (*(original_fn**)this)[6](this);
	}
};

class i_base_client_dll {
public:
	c_client_class* get_client_classes() {
		using original_fn = c_client_class * (__thiscall*)(i_base_client_dll*);
		return (*(original_fn**)this)[8](this);
	}
};