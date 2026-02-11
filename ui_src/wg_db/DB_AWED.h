#ifndef __DB_AWED_DB_H__
#define __DB_AWED_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_AWED
{
public:
	CDB_AWED();
	virtual ~CDB_AWED();
	CDBDoc* m_pDoc;

public:
	void Add(T_AWED_K Key,T_AWED_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_AWED_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_AWED_K Key,T_AWED_D& rData);
		//{return m_awed.Lookup(Key,rData);}
	int GetCount();
		//{return m_awed.GetCount();}
	POSITION GetStart();
		//{return m_awed.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_AWED_K& rKey,T_AWED_D& rData);
		//{m_awed.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_AWED_K,T_AWED_K,T_AWED_D,T_AWED_D&>m_awed;
};

#endif

