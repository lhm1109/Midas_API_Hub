#ifndef __DB_THJI_DB_H__
#define __DB_THJI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THJI
{
public:
	CDB_THJI();
	virtual ~CDB_THJI();
	CDBDoc* m_pDoc;

public:
	T_THJI_K m_nStartNum;
	T_THJI_K m_nLastNum;

public:
	void Add(T_THJI_K Key, T_THJI_D& rData);
	BOOL Del(T_THJI_K Key);

public:
	BOOL Get(T_THJI_K Key, T_THJI_D& rData);
	//{return m_thji.Lookup(Key,rData);}
	int GetCount();
	//{return m_thji.GetCount();}
	POSITION GetStart();
	//{return m_thji.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_THJI_K& rKey, T_THJI_D& rData);
	//{m_thji.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THJI_K, T_THJI_K, T_THJI_D, T_THJI_D&>m_thji;
};

#endif