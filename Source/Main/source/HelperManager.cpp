#include "StdAfx.h"
#include "HelperManager.h"
#include "HelperSystem.h"
#include "HelperView.h"
#include "Console.h"
#include "ZzzCharacter.h"

CHelperManager gHelperManager;

CHelperManager::CHelperManager()
{
	this->m_HelperList.clear();
}

CHelperManager::~CHelperManager()
{
	//this->Relese();
}

void CHelperManager::Relese()
{
	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();) 
	{
		std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator tempIT = it;

		++it;

		BoostWeak_Ptr(HELPER_VIEW) element = *tempIT;

		if (element.expired() == false)
		{
			element.lock()->Relese();
		}

		//m_HelperList.erase(tempIT);
	}

	m_HelperList.clear();
}

void CHelperManager::Init()
{
	//SetCompleteHook(0xE8, 0x004D7348, &this->RenderPetMain);
	//SetCompleteHook(0xE8, 0x004D7A98, &this->RenderPetMain);
	//SetCompleteHook(0xE8, 0x004D9A65, &this->RenderPetMain);
	//
	//SetCompleteHook(0xE8, 0x004D6EA5, &this->UpdatePetMain);
	//SetCompleteHook(0xE8, 0x004D7787, &this->UpdatePetMain);
	//SetCompleteHook(0xE8, 0x004D9568, &this->UpdatePetMain);
}

void CHelperManager::RenderPetMain()
{
	//((void(*)())0x503950)();
	//
}

void CHelperManager::UpdatePetMain()
{
	//((void(*)())0x503720)();
	//
}

bool CHelperManager::CreateHelper(int Index, int ModelIndex, vec3_t Position, CHARACTER * m_Owner, int SubType)
{
	if (m_Owner == nullptr)
	{
		return 0;
	}

	if (CheckExistsHelper(m_Owner))
	{
		return 1;
	}

	HELPER_VIEWPtr element = HELPER_VIEW::Make();

	if (element == nullptr)
	{
		return 0;
	}

	if (element->Create(Index, ModelIndex, Position, m_Owner, SubType))
	{
		auto elementInfo = gHelperSystem.GetHelper(ModelIndex);

		if (elementInfo == 0) 
		{
			return 0;
		}

		element->m_Type = elementInfo->Type;

		element->m_HeightFloor = elementInfo->HeightFloor;

		element->m_Movement = elementInfo->Movement;

		if (SceneFlag == 4)
		{
			element->SetScale(elementInfo->SizeCharList);
		}
		else 
		{
			element->SetScale(elementInfo->Size);
		}

		element->SetMiniature(elementInfo->Miniature, elementInfo->SizeMiniature, elementInfo->VelocityMiniature);

		this->Register(element);
		return 1;
	}

	return 0;
}

void CHelperManager::DeleteHelper(CHARACTER* Owner, int itemType, bool allDelete)
{
	if (Owner == nullptr)
	{
		return;
	}

	UnRegister(Owner, itemType, allDelete);
}

void CHelperManager::Register(BoostSmart_Ptr(HELPER_VIEW) info)
{
	this->m_HelperList.push_back(info);
}

void CHelperManager::UnRegister(CHARACTER* Owner, int itemType, bool isUnregistAll)
{
	if (Owner == nullptr)
	{
		return;
	}

	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();) 
	{
		std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator tempIT = it;

		++it;

		BoostWeak_Ptr(HELPER_VIEW) element = *tempIT;

		if (Hero == Owner)
		{
			if (element.lock()->IsSameOwner(&Owner->Object)) 
			{
				element.lock()->Relese();
				m_HelperList.erase(tempIT);
				break;
			}
		}
		else
		{
			if ((-1 == itemType && element.lock()->IsSameOwner(&Owner->Object))	|| element.lock()->IsSameObject(&Owner->Object, itemType))
			{
				element.lock()->Relese();

				m_HelperList.erase(tempIT);

				if (-1 == itemType || !isUnregistAll)
				{
					return;
				}
			}
		}
	}
}

bool CHelperManager::CheckExistsHelper(CHARACTER* Owner)
{
	if (Owner == nullptr) 
	{
		return 0;
	}

	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();) 
	{
		std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator tempIT = it;

		++it;

		BoostWeak_Ptr(HELPER_VIEW) element = *tempIT;

		if (element.expired() == false) 
		{
			HELPER_VIEWPtr helper = element.lock();

			if (helper->IsSameOwner(&Owner->Object) && helper->IsSameObject(&Owner->Object, Owner->Helper.Type))
			{
				if (helper->IsLive())
				{
					return 1;
				}

				helper->Relese();
				m_HelperList.erase(tempIT);
			}
		}
	}

	return 0;
}

void CHelperManager::RenderHelpers()
{
	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();)
	{
		std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator tempIT = it;

		++it;

		BoostWeak_Ptr(HELPER_VIEW) element = *tempIT;

		if (element.expired() == false) 
		{
			element.lock()->Render();
		}
	}

}

void CHelperManager::UpdateHelpers()
{
	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();)
	{
		std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator tempIT = it;

		++it;

		BoostWeak_Ptr(HELPER_VIEW) element = *tempIT;

		if (element.expired() == false)
		{
			element.lock()->Update();
		}
	}

}
