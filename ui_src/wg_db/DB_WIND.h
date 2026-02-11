#ifndef __DB_WIND_DB_H__
#define __DB_WIND_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_WIND
{
public:
	CDB_WIND();
	virtual ~CDB_WIND();
	CDBDoc* m_pDoc;

public:
	void Add(T_WIND_K Key,T_WIND_D& rData,CDB_STLD* pStld);
	BOOL Del(T_WIND_K Key,CDB_STLD* pStld);

public:
	BOOL Get(T_WIND_K Key,T_WIND_D& rData);
		//{return m_wind.Lookup(Key,rData);}
	int GetCount();
		//{return m_wind.GetCount();}
	POSITION GetStart();
		//{return m_wind.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WIND_K& rKey,T_WIND_D& rData);
		//{m_wind.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_WIND_K,T_WIND_K,T_WIND_D,T_WIND_D&>m_wind;
};

#endif