#pragma once
#include "urischeme.h"
#include <string>

namespace cortana {
	extern "C" { __declspec(dllexport)
		int handleUri(const char* uri);
	}

	int handleUri(std::string uri);
}