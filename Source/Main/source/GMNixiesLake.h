#pragma once
#ifndef _GMNIXIES_LAKE_FIELD_H_
#define _GMNIXIES_LAKE_FIELD_H_
#include "w_BaseMap.h"


BoostSmartPointer(CGMNixiesLake);
class CGMNixiesLake : public BaseMap
{
public:
	static CGMNixiesLakePtr Make();
	CGMNixiesLake();
	virtual~CGMNixiesLake();
public:
	virtual bool LoadMapData();
	virtual bool MoveObject(OBJECT* pObject);
	virtual bool CreateObject(OBJECT* pObject);
	virtual bool RenderObjectVisual(OBJECT* pObject, BMD* pModel);
	virtual bool RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon = 0);
	virtual void RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon = 0);
	virtual void RenderFrontSideVisual();
	virtual bool CreateNixiesSnow(PARTICLE* o);
};

#endif