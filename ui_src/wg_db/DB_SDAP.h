#ifndef __DB_SDAP_DB_H__
#define __DB_SDAP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDAP
{
public:
	CDB_SDAP();
	virtual ~CDB_SDAP();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDAP_K Key,T_SDAP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDAP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDAP_K Key,T_SDAP_D& rData);
		//{return m_sdap.Lookup(Key,rData);}
	int GetCount();
		//{return m_sdap.GetCount();}
	POSITION GetStart();
		//{return m_sdap.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDAP_K& rKey,T_SDAP_D& rData);
		//{m_sdap.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDAP_K,T_SDAP_K,T_SDAP_D,T_SDAP_D&>m_sdap;
};

#endif

