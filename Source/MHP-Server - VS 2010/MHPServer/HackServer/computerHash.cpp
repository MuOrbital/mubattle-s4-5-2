#include "stdafx.h"
#include "computerHash.h"
#include <sstream>
#include <string>

#define WIN32_LEAN_AND_MEAN



// we just need this for purposes of unique machine id. So any one or two mac's is
// fine.
unsigned short cComputerHash::hashMacAddress(PIP_ADAPTER_INFO info) {
	unsigned short hash = 0;
	for (unsigned int i = 0; i < info->AddressLength; i++) {
		hash += (info->Address[i] << ((i & 1) * 8));
	}
	return hash;
}

void cComputerHash::getMacHash(unsigned short &mac1, unsigned short &mac2) {
	IP_ADAPTER_INFO AdapterInfo[32];
	DWORD dwBufLen = sizeof(AdapterInfo);

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (dwStatus != ERROR_SUCCESS)
		return; // no adapters.

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	mac1 = hashMacAddress(pAdapterInfo);
	if (pAdapterInfo->Next)
		mac2 = hashMacAddress(pAdapterInfo->Next);

	// sort the mac addresses. We don't want to invalidate
	// both macs if they just change order.
	if (mac1 > mac2) {
		unsigned short tmp = mac2;
		mac2 = mac1;
		mac1 = tmp;
	}
}

unsigned short cComputerHash::getVolumeHash() {
	DWORD serialNum = 0;

	// Determine if this volume uses an NTFS file system.
	GetVolumeInformation("c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
	unsigned short hash = (unsigned short)((serialNum + (serialNum >> 16)) & 0xFFFF);

	return hash;
}

unsigned short cComputerHash::getCpuHash() {
	int cpuinfo[4] = {0, 0, 0, 0};
	__cpuid(cpuinfo, 0);
	unsigned short hash = 0;
	unsigned short *ptr = (unsigned short * )(&cpuinfo[0]);
	for (unsigned int i = 0; i < 8; i++)
		hash += ptr[i];

	return hash;
}

const char *cComputerHash::getMachineName() {
	static char computerName[1024];
	DWORD size = 1024;
	GetComputerName(computerName, &size);
	return &(computerName[0]);
}

std::string cComputerHash::generateHash(const std::string &bytes) {
	static char chars[] = "0123456789ABCDEF";
	std::stringstream stream;

	auto size = bytes.size();
	for (unsigned long i = 0; i < size; ++i) {
		unsigned char ch = ~((unsigned char)((unsigned short)bytes[i] +
			(unsigned short)bytes[(i + 1) % size] +
			(unsigned short)bytes[(i + 2) % size] +
			(unsigned short)bytes[(i + 3) % size])) * (i + 1);

		stream << chars[(ch >> 4) & 0x0F] << chars[ch & 0x0F];
	}

	return stream.str();
}


//	static std::string *cachedHash = nullptr;

std::string cComputerHash::machineHash() {
	static const unsigned long TargetLength = 64;

	//if (cachedHash != nullptr) {
	//	return *cachedHash;
	//}

	std::stringstream stream;

	stream << getMachineName();
	stream << getCpuHash();
	stream << getVolumeHash();

	UUID uuid;

	UuidCreateSequential(&uuid);

	DWORD ComputerHardwareId2 = *(DWORD*)(&uuid.Data4[2]) ^ 0x13B686FE;

	DWORD ComputerHardwareId3 = ((*(WORD*)(&uuid.Data4[6]) & 0xFFFF));

	stream << ComputerHardwareId2;

	stream << ComputerHardwareId3;

	auto string = stream.str();

	while (string.size() < TargetLength) {
		string = string + string;
	}

	if (string.size() > TargetLength) {
		string = string.substr(0, TargetLength);
	}

	return generateHash(string);
}