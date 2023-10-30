#pragma once

namespace devue::common {
	class dv_basic_gui {
	public:
		virtual void prepare()	= 0;
		virtual void update()	= 0;
		virtual void shutdown() = 0;
	};
}