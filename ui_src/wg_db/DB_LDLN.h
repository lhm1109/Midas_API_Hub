#ifndef __DB_LDLN_DB_H__
#define __DB_LDLN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_LDLN
{
public:
	CDB_LDLN();
	virtual ~CDB_LDLN();
	CDBDoc* m_pDoc;

public:
	T_LDLN_K m_nStartNum;
	T_LDLN_K m_nLastNum;

public:
	void Add(T_LDLN_K Key,T_LDLN_D& rData);
	BOOL Del(T_LDLN_K Key);

public:
	BOOL Get(T_LDLN_K Key,T_LDLN_D& rData);
		//{return m_ldln.Lookup(Key,rData);}
	int GetCount();
		//{return m_ldln.GetCount();}
	POSITION GetStart();
		//{return m_ldln.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LDLN_K& rKey,T_LDLN_D& rData);
		//{m_ldln.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LDLN_K,T_LDLN_K,T_LDLN_D,T_LDLN_D&>m_ldln;
};

#endif

