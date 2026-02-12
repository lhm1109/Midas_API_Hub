#ifndef __DB_IELC_DB_H__
#define __DB_IELC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_STLD;

class CDB_IELC
{
public:
	CDB_IELC();
	virtual ~CDB_IELC();
	CDBDoc* m_pDoc;

public:
	void Add(T_IELC_K Key,T_IELC_D& rData,CDB_ELEM* pElem, CDB_STLD* pStld);
	BOOL Del(T_IELC_K Key,CDB_ELEM* pElem, CDB_STLD* pStld);

public:
	BOOL Get(T_IELC_K Key,T_IELC_D& rData);
	//{return m_ielc.Lookup(Key,rData);}
	int GetCount();
	//{return m_ielc.GetCount();}
	POSITION GetStart();
	//{return m_ielc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_IELC_K& rKey,T_IELC_D& rData);
	//{m_ielc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_IELC_KEY,T_IELC_KEY,T_IELC_D,T_IELC_D&>m_ielc;
};

#endif
