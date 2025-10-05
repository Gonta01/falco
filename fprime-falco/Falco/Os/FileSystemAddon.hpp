#ifndef _FileSystemAddon_hpp_
#define _FileSystemAddon_hpp_

#include <FpConfig.hpp>

namespace Os {
	// This namespace encapsulates a very simple file system interface that has the most often-used features.
	namespace FileSystem {
		Status initFileSystem(); //!<  initalize file system
	}
}

#endif
