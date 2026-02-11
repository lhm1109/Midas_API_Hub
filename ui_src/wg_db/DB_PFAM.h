#ifndef __DB_PFAM_DB_H__
#define __DB_PFAM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT_PFAM.h"

class CDB_PFAM
{
public:
	CDB_PFAM();
	virtual ~CDB_PFAM();

public:
	T_PFAM_K m_nStartNum;
	T_PFAM_K m_nLastNum;

public:
	void Add(T_PFAM_K Key, T_PFAM_D& rData);
	BOOL Del(T_PFAM_K Key);

public:
	BOOL Get(T_PFAM_K Key, T_PFAM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PFAM_K& rKey, T_PFAM_D& rData);

	void AddList(T_PFAM_K Key);
	BOOL DelList(T_PFAM_K Key);
	void DelAllList(void);
	BOOL GetList(T_PFAM_K Key, PfamList*& rpList);
	int GetCountList(void);
	void AddListItem(T_PFAM_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_PFAM_K Key, int nCmd, int nKey);

protected:
	CMap<T_PFAM_K, T_PFAM_K, T_PFAM_D, T_PFAM_D&>  m_mapData;
	CMap<T_PFAM_K, T_PFAM_K, PfamList*, PfamList*> m_mapList;
};

#endif
