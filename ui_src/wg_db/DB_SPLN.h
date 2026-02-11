#ifndef __DB_SPLN_DB_H__
#define __DB_SPLN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPLN
{
public:
	CDB_SPLN();
	virtual ~CDB_SPLN();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPLN_K Key,T_SPLN_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SPLN_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SPLN_K Key,T_SPLN_D& rData);
		//{return m_spln.Lookup(Key,rData);}
	int GetCount();
		//{return m_spln.GetCount();}
	POSITION GetStart();
		//{return m_spln.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPLN_K& rKey,T_SPLN_D& rData);
		//{m_spln.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPLN_K,T_SPLN_K,T_SPLN_D,T_SPLN_D&>m_spln;
};

#endif

