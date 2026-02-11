#ifndef __DB_SIFA_H29_DB_H__
#define __DB_SIFA_H29_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SIFA_H29
{
public:
	CDB_SIFA_H29();
	virtual ~CDB_SIFA_H29();
	CDBDoc* m_pDoc;

public:
	T_SIFA_H29_K m_nStartNum;
	T_SIFA_H29_K m_nLastNum;

public:
	void Add(T_SIFA_H29_K Key, T_SIFA_H29_D& rData);
	BOOL Del(T_SIFA_H29_K Key);

public:
	BOOL Get(T_SIFA_H29_D& rData);
	BOOL Get(T_SIFA_H29_K Key, T_SIFA_H29_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIFA_H29_K& rKey, T_SIFA_H29_D& rData);

protected:
	CMap<T_SIFA_H29_K, T_SIFA_H29_K, T_SIFA_H29_D, T_SIFA_H29_D&> m_dataMap;
};

#endif