#ifndef __dapps__
#define __dapps__

#include "dapps-common.h"
#include "utilities/ArgsProcessor.h"
#include "registry/RegistryServer.h"
#include "app/DappsServer.h"
#include "commons/utilities/LoggingUtil.h"

namespace dapps
{
	// Satisfy compiler
	class Dapps;

	class Dapps
	{
		public:
			ArgsProcessor* args;
			RegistryServer* registry;
			DappsServer* appServer;
			LoggingUtil* loggingUtil;
	};
}
#endif //__dapps__
