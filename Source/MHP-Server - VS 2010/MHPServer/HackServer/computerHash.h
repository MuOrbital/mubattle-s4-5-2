#pragma once

#include <intrin.h>
#include <iphlpapi.h>

class cComputerHash
{
public:
	static unsigned short hashMacAddress(PIP_ADAPTER_INFO info);
	static void getMacHash(unsigned short &mac1, unsigned short &mac2);
	static unsigned short getVolumeHash();
	static unsigned short getCpuHash();
	static const char *getMachineName();
	static std::string generateHash(const std::string &bytes);
	static std::string machineHash();
};