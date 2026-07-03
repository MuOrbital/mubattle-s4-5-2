#pragma once
#ifndef _GMNARSE_FIELD_H_
#define _GMNARSE_FIELD_H_

#include "w_BaseMap.h"

BoostSmartPointer(CGMZoneNars);

class CGMZoneNars : public BaseMap
{
public:
	static CGMZoneNarsPtr Make();
	CGMZoneNars();
	virtual ~CGMZoneNars();
public:
	virtual bool MoveObject(OBJECT* pObject);
	virtual bool CreateObject(OBJECT* pObject);
	virtual bool RenderObjectVisual(OBJECT* pObject, BMD* pModel);
	virtual bool RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon = 0);
	virtual void RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon = 0);
	virtual void RenderFrontSideVisual();
};

#endif