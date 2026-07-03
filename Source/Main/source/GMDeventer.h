#pragma once
#ifndef _GMDEVENTER_FIELD_H_
#define _GMDEVENTER_FIELD_H_

#include "w_BaseMap.h"

BoostSmartPointer(CGMDeventer);

class CGMDeventer : public BaseMap
{
public:
	static CGMDeventerPtr Make();
	CGMDeventer();
	virtual~CGMDeventer();
public:
	virtual bool LoadMapData();

	virtual bool MoveObject(OBJECT* pObject);
	virtual bool CreateObject(OBJECT* pObject);
	virtual bool RenderObjectVisual(OBJECT* pObject, BMD* pModel);
	virtual bool RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon = 0);
	virtual void RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon = 0);
};

#endif