#ifndef __DB_ASOP_DB_H__
#define __DB_ASOP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_ASOP
{
public:
	CDB_ASOP();
	virtual ~CDB_ASOP();
	CDBDoc* m_pDoc;

public:
	void Add(T_ASOP_K Key,T_ASOP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_ASOP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_ASOP_K Key,T_ASOP_D& rData);
		//{return m_asop.Lookup(Key,rData);}
	int GetCount();
		//{return m_asop.GetCount();}
	POSITION GetStart();
		//{return m_asop.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ASOP_K& rKey,T_ASOP_D& rData);
		//{m_asop.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ASOP_K,T_ASOP_K,T_ASOP_D,T_ASOP_D&>m_asop;
};

#endif

