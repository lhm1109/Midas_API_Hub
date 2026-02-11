#ifndef __DB_PFSL_DB_H__
#define __DB_PFSL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT_PFSL.h"

class CDB_PFSL
{
public:
	CDB_PFSL();
	virtual ~CDB_PFSL();

public:
	T_PFSL_K m_nStartNum;
	T_PFSL_K m_nLastNum ;

public:
	void Add(T_PFSL_K Key, T_PFSL_D& rData);
	BOOL Del(T_PFSL_K Key);

public:
	BOOL Get(T_PFSL_K Key, T_PFSL_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PFSL_K& rKey, T_PFSL_D& rData);

	void AddList(T_PFSL_K Key);
	BOOL DelList(T_PFSL_K Key);
	void DelAllList(void);
	BOOL GetList(T_PFSL_K Key, PfslList*& rpList);
	int GetCountList(void);
	void AddListItem(T_PFSL_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_PFSL_K Key, int nCmd, int nKey);

protected:
	CMap<T_PFSL_K, T_PFSL_K, T_PFSL_D, T_PFSL_D&>  m_mapData;
	CMap<T_PFSL_K, T_PFSL_K, PfslList*, PfslList*> m_mapList;
};

#endif
