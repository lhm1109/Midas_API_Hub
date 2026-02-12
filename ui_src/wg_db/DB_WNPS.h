#ifndef __DB_WNPS_DB_H__
#define __DB_WNPS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_WNPS
{
public:
	CDB_WNPS();
	virtual ~CDB_WNPS();
	CDBDoc* m_pDoc;

public:
	void Add(T_WNPS_K Key, T_WNPS_D& rData, CDB_STLD* pStld);
	BOOL Del(T_WNPS_K Key, CDB_STLD* pStld);

public:
	BOOL Get(T_WNPS_K Key, T_WNPS_D& rData);
	//{return m_wnps.Lookup(Key,rData);}
	int GetCount();
	//{return m_wnps.GetCount();}
	POSITION GetStart();
	//{return m_wnps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_WNPS_K& rKey, T_WNPS_D& rData);
	//{m_wnps.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_WNPS_K, T_WNPS_K, T_WNPS_D, T_WNPS_D&>m_wnps;
};

#endif