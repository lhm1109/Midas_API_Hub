#ifndef __DB_PMOS_DB_H__
#define __DB_PMOS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_PROS;
class CDB_PMOS
{
public:
	CDB_PMOS();
	virtual ~CDB_PMOS();
	CDBDoc* m_pDoc;

public:
	// 리스트 크기 조절 및 수정
	T_PMOS_K m_nStartNum;
	T_PMOS_K m_nLastNum;

public:
	void Add(T_PMOS_K Key, T_PMOS_D& rData);
	BOOL Del(T_PMOS_K Key);

public:
	BOOL Get(T_PMOS_K Key, T_PMOS_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PMOS_K& rKey, T_PMOS_D& rData);
protected:
	CMap<T_PMOS_K, T_PMOS_K, T_PMOS_D, T_PMOS_D&> m_pmos;
};



#endif