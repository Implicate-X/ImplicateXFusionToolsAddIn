#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;
using namespace icu;

namespace implicatex {
	namespace fusion {
		class ToolsBar
		{
		public:
			bool initialize();
			void terminate();
		};
	}
}
