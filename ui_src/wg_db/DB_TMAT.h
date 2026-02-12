#ifndef __DB_TMAT_DB_H__
#define __DB_TMAT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATL;

class CDB_TMAT
{
public:
	CDB_TMAT();
	CDB_TMAT(CDB_TMAT& src);
	CDB_TMAT& operator=(CDB_TMAT& src);
	virtual ~CDB_TMAT();

public:
	void Add(T_TMAT_K Key,T_TMAT_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_TMAT_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_TMAT_K Key,T_TMAT_D& rData)
		{return m_tmat.Lookup(Key,rData);}
	int GetCount()
		{return (int)m_tmat.GetCount();}
	POSITION GetStart()
		{return m_tmat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TMAT_K& rKey,T_TMAT_D& rData)
		{m_tmat.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_TMAT_K,T_TMAT_K,T_TMAT_D,T_TMAT_D&>m_tmat;
};

#endif