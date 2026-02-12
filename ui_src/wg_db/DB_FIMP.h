#ifndef __DB_FIMP_DB_H__
#define __DB_FIMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_FIMP
{
public:
	CDB_FIMP();
	virtual ~CDB_FIMP();
	CDBDoc* m_pDoc;

public:
	T_FIMP_K m_nStartNum;
	T_FIMP_K m_nLastNum;

public:
	void Add(T_FIMP_K Key,T_FIMP_D& rData);
	BOOL Del(T_FIMP_K Key);

public:
	BOOL Get(T_FIMP_K Key,T_FIMP_D& rData);
		//{return m_fimp.Lookup(Key,rData);}
	int GetCount();
		//{return m_fimp.GetCount();}
	POSITION GetStart();
		//{return m_fimp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FIMP_K& rKey,T_FIMP_D& rData);
		//{m_fimp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_FIMP_K,T_FIMP_K,T_FIMP_D,T_FIMP_D&>m_fimp;
};

#endif
