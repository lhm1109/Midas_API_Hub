#ifndef __DB_PFSB_DB_H__
#define __DB_PFSB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT_PFSB.h"

class CDB_PFSB
{
public:
	CDB_PFSB();
	virtual ~CDB_PFSB();

public:
	T_PFSB_K m_nStartNum;
	T_PFSB_K m_nLastNum;

public:
	void Add(T_PFSB_K Key, T_PFSB_D& rData);
	BOOL Del(T_PFSB_K Key);

public:
	BOOL Get(T_PFSB_K Key, T_PFSB_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PFSB_K& rKey, T_PFSB_D& rData);

	void AddList(T_PFSB_K Key);
	BOOL DelList(T_PFSB_K Key);
	void DelAllList(void);
	BOOL GetList(T_PFSB_K Key, PfsbList*& rpList);
	int GetCountList(void);
	void AddListItem(T_PFSB_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_PFSB_K Key, int nCmd, int nKey);

protected:
	CMap<T_PFSB_K, T_PFSB_K, T_PFSB_D, T_PFSB_D&>  m_mapData;
	CMap<T_PFSB_K, T_PFSB_K, PfsbList*, PfsbList*> m_mapList;
};

#endif
