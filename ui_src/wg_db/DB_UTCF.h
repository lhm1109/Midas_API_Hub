#ifndef __DB_UTCF_DB_H__
#define __DB_UTCF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_UTCF
{
public:
	CDB_UTCF();
	virtual ~CDB_UTCF();
	CDBDoc* m_pDoc;

public:
	void Add(T_UTCF_K Key,T_UTCF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_UTCF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_UTCF_K Key,T_UTCF_D& rData);
		//{return m_utcf.Lookup(Key,rData);}
	int GetCount();
		//{return m_utcf.GetCount();}
	POSITION GetStart();
		//{return m_utcf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UTCF_K& rKey,T_UTCF_D& rData);
		//{m_utcf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UTCF_K,T_UTCF_K,T_UTCF_D,T_UTCF_D&>m_utcf;
};

#endif

