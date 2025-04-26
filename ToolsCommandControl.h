#pragma once

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {  
namespace fusion {  
	class ToolsCommandControl :  
		public CommandControl
	{  
	public:
		bool initialize();
	};  
}  
}
