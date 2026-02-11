#ifndef __DB_HMAT_DB_H__
#define __DB_HMAT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_MATL;

class CDB_HMAT
{
public:
	CDB_HMAT();
	virtual ~CDB_HMAT();
	CDBDoc* m_pDoc;

public:
	void Add(T_HMAT_K Key,T_HMAT_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_HMAT_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_HMAT_K Key,T_HMAT_D& rData);
		//{return m_hmat.Lookup(Key,rData);}
	int GetCount();
		//{return m_hmat.GetCount();}
	POSITION GetStart();
		//{return m_hmat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HMAT_K& rKey,T_HMAT_D& rData);
		//{m_hmat.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_HMAT_K,T_HMAT_K,T_HMAT_D,T_HMAT_D&>m_hmat;
};

#endif