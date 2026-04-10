#include "stdafx.h"
#include "LuaObject.h"
#include "LuaStack.hpp"

class ObjectClass {
public:
	ObjectClass() : m_Struct(0) {

	};

	ObjectClass(DWORD Struct) {
		m_Struct = (OBJECT*)Struct;
	};

	~ObjectClass() {

	};

	float Alpha() const {
		return m_Struct->Alpha;
	};

	DWORD Mesh() const {
		return m_Struct->BlendMesh;
	};

	float Light() const {
		return m_Struct->BlendMeshLight;
	};

	float TexCoordU() const {
		return m_Struct->BlendMeshTexCoordU;
	};

	float TexCoordV() const {
		return m_Struct->BlendMeshTexCoordV;
	};

	DWORD Hidden() const {
		return m_Struct->HiddenMesh;
	};

	int getAction() const {
		return m_Struct->CurrentAction;
	};

	float getTime() const {
		return m_Struct->Timer;
	};

	void setTime(float value) const {
		m_Struct->Timer = value;
	};

private:
	OBJECT* m_Struct;
};

float worldTimes() {
	return WorldTime;
}

float GetDoubleRender(float a1, float a2) 
{
	float Return;
	float InitValue;
	
	InitValue = (float)((int32_t)(a2 * 0.01745f * 1000.0f / a1 + WorldTime) % (int32_t)(6283.185546875f / a1)) * 0.001f * a1;
	
	if (InitValue >= 3.14f)
	{
		Return = cos(InitValue);
	}
	else
	{
		Return = -cos(InitValue);
	}

	return (float)((Return + 1.0f) * 0.5f);
}

void RegisterClassObject(lua_State * lua)
{
	luaaa::LuaClass<ObjectClass> luaObject(lua, "Object");
	luaObject.ctor<DWORD>();
	luaObject.fun("Alpha", &ObjectClass::Alpha);
	luaObject.fun("Mesh", &ObjectClass::Mesh);
	luaObject.fun("Light", &ObjectClass::Light);
	luaObject.fun("TexCoordU", &ObjectClass::TexCoordU);
	luaObject.fun("TexCoordV", &ObjectClass::TexCoordV);
	luaObject.fun("Hidden", &ObjectClass::Hidden);
	luaObject.fun("getAction", &ObjectClass::getAction);
	luaObject.fun("getTime", &ObjectClass::getTime);
	luaObject.fun("setTime", &ObjectClass::setTime);

	luaaa::LuaModule(lua).fun("worldTime", &worldTimes);
	luaaa::LuaModule(lua).fun("GetDoubleRender", &GetDoubleRender);
}
