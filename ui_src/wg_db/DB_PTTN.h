#ifndef __DB_PTTN_DB_H__
#define __DB_PTTN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_DESIGN.H"

class CDBDoc;
class CDB_PTTN
{
public:
	CDB_PTTN();
	virtual ~CDB_PTTN();
	CDBDoc* m_pDoc;

public:
	T_PTTN_K m_nStartNum;
	T_PTTN_K m_nLastNum;

public:
	void Add(T_PTTN_K Key, T_PTTN_D& rData);
	BOOL Del(T_PTTN_K Key);

public:
	BOOL Get(T_PTTN_K Key, T_PTTN_D& rData)
		{return m_pttn.Lookup(Key,rData);}
	int GetCount()
		{return m_pttn.GetCount();}
	POSITION GetStart()
		{return m_pttn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_PTTN_K& rKey, T_PTTN_D& rData)
		{m_pttn.GetNextAssoc(rNextPosition,rKey,rData);}

public:
	BOOL IsUsedTdnt(T_TDNT_K TdntKey);

protected:
	CMap<T_PTTN_K, T_PTTN_K, T_PTTN_D, T_PTTN_D&> m_pttn;
	CMap<T_TDNT_K, T_TDNT_K, int, int> m_tdntCount2;
};

#endif
