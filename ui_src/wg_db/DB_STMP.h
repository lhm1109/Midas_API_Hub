#ifndef __DB_STMP_DB_H__
#define __DB_STMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LDGR;

class CDB_STMP
{
public:
	CDB_STMP();
	virtual ~CDB_STMP();
	CDBDoc* m_pDoc;

public:
	T_STMP_K m_nStartNum;
	T_STMP_K m_nLastNum;

public:
	void Add(T_STMP_K Key,T_STMP_D& rData,CDB_STLD* pStld,CDB_LDGR* pLdgr);
	BOOL Del(T_STMP_K Key,CDB_STLD* pStld,CDB_LDGR* pLdgr);

public:
	BOOL Get(T_STMP_K Key,T_STMP_D& rData);
		//{return m_stmp.Lookup(Key,rData);}
	int GetCount();
		//{return m_stmp.GetCount();}
	POSITION GetStart();
		//{return m_stmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STMP_K& rKey,T_STMP_D& rData);
		//{m_stmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STMP_K,T_STMP_K,T_STMP_D,T_STMP_D&>m_stmp;
};

#endif