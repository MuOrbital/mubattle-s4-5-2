#pragma once
#ifndef _GMACHERON_FIELD_H_
#define _GMACHERON_FIELD_H_

#include "w_BaseMap.h"


BoostSmartPointer(CGMAcheron);
class CGMAcheron : public BaseMap
{
public:
	static CGMAcheronPtr Make();

	CGMAcheron();
	virtual~CGMAcheron();
	virtual bool LoadMapData();

	virtual bool MoveObject(OBJECT* pObject);
	virtual bool CreateObject(OBJECT* pObject);
	virtual bool RenderObjectVisual(OBJECT* pObject, BMD* pModel);
	virtual bool RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon = 0);
	virtual void RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon = 0);
};
#endif