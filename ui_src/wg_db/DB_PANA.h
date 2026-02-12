#ifndef __DB_PANA_DB_H__
#define __DB_PANA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_STLD;
class CDB_SPLC;
class CDB_THIS;
class CDB_POLC;

class CDB_PANA
{
public:
	CDB_PANA();
	virtual ~CDB_PANA();

public:
	T_PANA_K m_nStartNum;
	T_SMLC_K m_nLastNum;
public:
//	void Add(T_PANA_K Key, T_PANA_D& rData, CDB_STLD* pStld, CDB_SPLC* pSplc, CDB_THIS* pThis, CDB_POLC* pPolc);
//	BOOL Del(T_PANA_K Key, CDB_STLD* pStld, CDB_SPLC* pSplc, CDB_THIS* pThis, CDB_POLC* pPolc);

public:
	//BOOL Get(T_PANA_K Key, T_PANA_D& rData)
	//{
	//	return m_pana.Lookup(Key, rData);
	//}
	//int GetCount()
	//{
	//	return m_pana.GetCount();
	//}
	//POSITION GetStart()
	//{
	//	return m_pana.GetStartPosition();
	//}
	//void GetNext(POSITION& rNextPosition, T_PANA_K& rKey, T_PANA_D& rData)
	//{
	//	m_pana.GetNextAssoc(rNextPosition, rKey, rData);
	//}

// 	void AddList(T_PANA_K Key);
// 	BOOL DelList(T_PANA_K Key);
// 	void DelAllList();
// 
// 	void AddListItem(T_PANA_K Key, int nCmd, int nKey);
// 	BOOL DelListItem(T_PANA_K Key, int nCmd, int nKey);

protected:
	CMap<T_PANA_K, T_PANA_K, T_PANA_D, T_PANA_D&>m_pana;
};

#endif
