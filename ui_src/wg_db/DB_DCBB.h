#ifndef __DB_DCBB_DB_H__
#define __DB_DCBB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DCBB
{
public:
	CDB_DCBB();
	virtual ~CDB_DCBB();
	CDBDoc* m_pDoc;

public:
	void Add(T_DCBB_K Key,T_DCBB_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DCBB_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DCBB_K Key,T_DCBB_D& rData);
		//{return m_dcbb.Lookup(Key,rData);}
	int GetCount();
		//{return m_dcbb.GetCount();}
	POSITION GetStart();
		//{return m_dcbb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DCBB_K& rKey,T_DCBB_D& rData);
		//{m_dcbb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DCBB_K,T_DCBB_K,T_DCBB_D,T_DCBB_D&>m_dcbb;
};

#endif

