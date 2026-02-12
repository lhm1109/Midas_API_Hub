#ifndef __DB_SDTS_DB_H__
#define __DB_SDTS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDTS
{
public:
	CDB_SDTS();
	virtual ~CDB_SDTS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDTS_K Key,T_SDTS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDTS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDTS_K Key,T_SDTS_D& rData);
		//{return m_sdts.Lookup(Key,rData);}
	int GetCount();
		//{return m_sdts.GetCount();}
	POSITION GetStart();
		//{return m_sdts.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDTS_K& rKey,T_SDTS_D& rData);
		//{m_sdts.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDTS_K,T_SDTS_K,T_SDTS_D,T_SDTS_D&>m_sdts;
};

#endif

