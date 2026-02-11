#ifndef __DB_UFMT_DB_H__
#define __DB_UFMT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_UFMT
{
public:
	CDB_UFMT();
	virtual ~CDB_UFMT();
	CDBDoc* m_pDoc;

public:
	void Add(T_UFMT_K Key,T_UFMT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_UFMT_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_UFMT_K Key,T_UFMT_D& rData);
		//{return m_ufmt.Lookup(Key,rData);}
	int GetCount();
		//{return m_ufmt.GetCount();}
	POSITION GetStart();
		//{return m_ufmt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UFMT_K& rKey,T_UFMT_D& rData);
		//{m_ufmt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UFMT_K,T_UFMT_K,T_UFMT_D,T_UFMT_D&>m_ufmt;
};

#endif

