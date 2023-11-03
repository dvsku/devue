#pragma once

#include <cstdint>

namespace devue::core {
	class dv_util_diag {
	public:
		static inline uint16_t fps = 0U;
		static inline double dtime = 0.0;

	public:
		static void init();
		static void update();

	private:
		static inline uint16_t m_frames		   = 0U;
		static inline double   m_previous_time = 0.0;
		static inline double   m_elapsed_time  = 0.0;
	};
}