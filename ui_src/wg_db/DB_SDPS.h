#ifndef __DB_SDPS_DB_H__
#define __DB_SDPS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDPS
{
public:
	CDB_SDPS();
	virtual ~CDB_SDPS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDPS_K Key,T_SDPS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDPS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDPS_K Key,T_SDPS_D& rData);
		//{return m_sdps.Lookup(Key,rData);}
	int GetCount();
		//{return m_sdps.GetCount();}
	POSITION GetStart();
		//{return m_sdps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDPS_K& rKey,T_SDPS_D& rData);
		//{m_sdps.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDPS_K,T_SDPS_K,T_SDPS_D,T_SDPS_D&>m_sdps;
};

#endif

