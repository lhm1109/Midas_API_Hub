#ifndef __DB_SDDO_DB_H__
#define __DB_SDDO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDDO
{
public:
	CDB_SDDO();
	virtual ~CDB_SDDO();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDDO_K Key,T_SDDO_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDDO_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDDO_K Key,T_SDDO_D& rData);
		//{return m_sddo.Lookup(Key,rData);}
	int GetCount();
		//{return m_sddo.GetCount();}
	POSITION GetStart();
		//{return m_sddo.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDDO_K& rKey,T_SDDO_D& rData);
		//{m_sddo.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDDO_K,T_SDDO_K,T_SDDO_D,T_SDDO_D&>m_sddo;
};

#endif

