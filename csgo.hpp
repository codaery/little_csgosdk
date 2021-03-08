#pragma once
#include "csgo_classes.hpp"
#include "utilities.hpp"

namespace interfaces {
	extern i_base_client_dll* client = nullptr;
	extern i_client_entity_list* entity_list = nullptr;
	extern iv_engine_client* engine = nullptr;

	void initialize() {
		client = utilities::engine::get_interface<i_base_client_dll, interface_type::index>("client.dll", "VClient018");
		entity_list = utilities::engine::get_interface<i_client_entity_list, interface_type::index>("client.dll", "VClientEntityList003");
		engine = utilities::engine::get_interface<iv_engine_client, interface_type::index>("engine.dll", "VEngineClient014");
	}
}

namespace entity {
	class renderer {
	public:
		bool world_to_screen(const vec3_t& origin, vec3_t& screen) {
			auto matrix = interfaces::engine->world_to_screen_matrix();

			auto find_point = [](vec3_t& point, int screen_w, int screen_h, int degrees) -> void {
				float x2 = screen_w * 0.5f;
				float y2 = screen_h * 0.5f;

				float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2)));
				float r = degrees / d;

				point.x = r * point.x + (1 - r) * x2;
				point.y = r * point.y + (1 - r) * y2;
			};

			float w = matrix[3][0] * origin.x + matrix[3][1] * origin.y + matrix[3][2] * origin.z + matrix[3][3];

			int screen_width, screen_height;
			interfaces::engine->get_screen_size(screen_width, screen_height);

			float inverse_width = -1.0f / w;
			bool behind = true;

			if (w > 0.01) {
				inverse_width = 1.0f / w;
				behind = false;
			}

			screen.x = (float)((screen_width / 2) + (0.5 * ((matrix[0][0] * origin.x
				+ matrix[0][1] * origin.y
				+ matrix[0][2] * origin.z
				+ matrix[0][3]) * inverse_width) * screen_width + 0.5));

			screen.y = (float)((screen_height / 2) - (0.5 * ((matrix[1][0] * origin.x
				+ matrix[1][1] * origin.y
				+ matrix[1][2] * origin.z
				+ matrix[1][3]) * inverse_width) * screen_height + 0.5));

			if (screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind) {
				find_point(screen, screen_width, screen_height, screen_height / 2);
				return false;
			}

			return !(behind);
		}
	};
	extern renderer render;
}