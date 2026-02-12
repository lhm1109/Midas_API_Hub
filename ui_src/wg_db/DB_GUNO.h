#ifndef __DB_GUNO_DB_H__
#define __DB_GUNO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_GUNO
{
public:
	CDB_GUNO();
	virtual ~CDB_GUNO();
	CDBDoc* m_pDoc;

public:
	T_GUNO_K m_nStartNum;
	T_GUNO_K m_nLastNum;

public:
	void Add(T_GUNO_K Key, T_GUNO_D& rData, CDB_NODE* pNode);
	BOOL Del(T_GUNO_K Key, CDB_NODE* pNode);

public:
	BOOL Get(T_GUNO_K Key, T_GUNO_D& rData);
	//{return m_guno.Lookup(Key,rData);}
	int GetCount();
	//{return m_guno.GetCount();}
	POSITION GetStart();
	//{return m_guno.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUNO_K& rKey, T_GUNO_D& rData);
	//{m_guno.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUNO_K,T_GUNO_K,T_GUNO_D,T_GUNO_D&>m_guno;
};

#endif