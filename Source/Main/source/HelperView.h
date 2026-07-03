#pragma once

#include <list>

BoostSmartPointer(HELPER_VIEW);

class HELPER_VIEW
{
public:
	static HELPER_VIEWPtr Make();

	HELPER_VIEW() : m_pOwner(nullptr), m_itemType(-1), m_Obj(nullptr), m_HeightFloor(0.0f), m_Movement(0), m_Type(0)
	{
		this->Init();
	};

	virtual ~HELPER_VIEW()
	{
		this->Relese();
	};

	void Init()
	{
		if (m_Obj == nullptr)
		{
			m_Obj = new OBJECT();
		}
	};

	bool Create(int Index, int ModelIndex, vec3_t Position, CHARACTER* m_Owner, int SubType);

	void Render();
	void Update();
	void UpdateModel(OBJECT* model);
	void UpdateMove(OBJECT* model);

	void WorkMoviment(OBJECT* ObjectStruct, float MaxPos, int type, int movement, int state);
	void WorkdMovimentMiniature(OBJECT* ObjectStruct, float MaxPos);

	bool IsLive() { return (m_Obj != nullptr && m_Obj->Live); };
	bool IsSameOwner(OBJECT*Owner) { return (m_Obj != nullptr && Owner == m_Obj->Owner) ? 1 : 0; };
	bool IsSameObject(OBJECT*Owner, int itemType) { return (m_Obj != nullptr && Owner == m_Obj->Owner && itemType == m_itemType) ? 1 : 0; };
	bool IsSameObject(OBJECT*Owner) { return (m_Obj != nullptr && Owner == m_Obj->Owner) ? 1 : 0; };
	void SetScale(float scale) 
	{
		m_Obj->Scale = scale; 
		this->PetOriginalSize = scale;
	};

	void SetMiniature(int miniature, float sizeminiature, float velocity) 
	{
		this->Miniature = miniature;
		this->SizeMiniature = sizeminiature;
		this->VelocityMiniature = velocity;
	}

	void Relese() 
	{
		m_itemType = -1;
		ModelIndex = -1;

		if (m_pOwner) 
		{
			m_pOwner = nullptr;
		}

		if (m_Obj) 
		{
			m_Obj->Live = false;
			m_Obj->Owner = nullptr;

			delete m_Obj;
			m_Obj = nullptr;
		}
	}

	int m_itemType;
	int ModelIndex;
	OBJECT* m_Obj;
	CHARACTER* m_pOwner;

	DWORD m_startTick;

	int m_Type;
	int m_Movement;
	float m_HeightFloor;
	float PetOriginalSize;

	int Miniature;
	float SizeMiniature;
	float VelocityMiniature;
};
