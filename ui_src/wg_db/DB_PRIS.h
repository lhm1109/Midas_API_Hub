#ifndef __DB_PRIS_DB_H__
#define __DB_PRIS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_PROS;

class CDB_PRIS
{
public:
	CDB_PRIS();
	virtual ~CDB_PRIS();
	CDBDoc* m_pDoc;

public:
	// 리스트 크기 조절 및 수정
	T_PRIS_K m_nStartNum;
	T_PRIS_K m_nLastNum;

public:
	void Add(T_PRIS_K Key, T_PRIS_D& rData);
	BOOL Del(T_PRIS_K Key);

public:
	BOOL Get(T_PRIS_K Key, T_PRIS_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PRIS_K& rKey, T_PRIS_D& rData);

	// DB 연계 참조 쿼리
	void AddList(T_PRIS_K Key);
	BOOL DelList(T_PRIS_K Key);
	void DelAllList();
	void AddListItem(T_PRIS_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_PRIS_K Key, int nCmd, int nKey);
	BOOL GetList(T_PRIS_K Key, DBDataList*& rpList);

protected:
	CMap<T_PRIS_K, T_PRIS_K, T_PRIS_D, T_PRIS_D&> m_pris;
	CMap<T_PRIS_K, T_PRIS_K, DBDataList*, DBDataList*>m_prislist;
};



#endif