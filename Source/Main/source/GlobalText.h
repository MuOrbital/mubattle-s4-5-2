#pragma once

struct TEXT_INFO
{
	int Index;
	char Message[256];
};

class CGlobalText
{
public:

	CGlobalText();

	virtual ~CGlobalText();

	char* operator[](int index) 
{
		return GlobalText(index);
	}
	void Load(char* path);

	char* GlobalText(int index);

	std::map<int, TEXT_INFO> m_MessageInfo;

private:

	char m_DefaultMessage[128];
};

extern CGlobalText GlobalText;
