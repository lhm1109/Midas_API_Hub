#ifndef __DB_PROS_DB_H__
#define __DB_PROS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_PRIS;

class CDB_PROS
{
public:
	CDB_PROS();
	virtual ~CDB_PROS();
	CDBDoc* m_pDoc;

public:
	// 리스트 크기 조절 및 수정
	T_PROS_K m_nStartNum;
	T_PROS_K m_nLastNum;

public:
	void Add(T_PROS_K Key, T_PROS_D& rData, CDB_PRIS *pPris);
	BOOL Del(T_PROS_K Key, CDB_PRIS* pPris);

public:
	BOOL Get(T_PROS_K Key, T_PROS_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PROS_K& rKey, T_PROS_D& rData);


protected:
	CMap<T_PROS_K, T_PROS_K, T_PROS_D, T_PROS_D&> m_pros;

};



#endif