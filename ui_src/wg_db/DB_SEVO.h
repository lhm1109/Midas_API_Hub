#ifndef __DB_SEVO_DB_H__
#define __DB_SEVO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_VBEM;

class CDB_SEVO
{
public:
	CDB_SEVO();
	virtual ~CDB_SEVO();

public:
	T_SEVO_K m_nStartNum;
	T_SEVO_K m_nLastNum;

public:
	void Add(T_SEVO_K Key,T_SEVO_D& rData, CDB_VBEM* pVbem);
	BOOL Del(T_SEVO_K Key, CDB_VBEM* pVbem);

public:
	BOOL Get(T_SEVO_K Key,T_SEVO_D& rData)
		{return m_sevo.Lookup(Key,rData);}
	int GetCount()
		{return m_sevo.GetCount();}
	POSITION GetStart()
		{return m_sevo.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SEVO_K& rKey,T_SEVO_D& rData)
		{m_sevo.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_SEVO_K Key);
	BOOL DelList(T_SEVO_K Key);
	void DelAllList();
	BOOL GetList(T_SEVO_K Key,SevoList*& rpList){return m_sevolist.Lookup(Key,rpList);}
	int GetCountList(){return m_sevolist.GetCount();}
	void AddListItem(T_SEVO_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SEVO_K Key, int nCmd, int nKey);

protected:
	CMap<T_SEVO_K,T_SEVO_K,T_SEVO_D,T_SEVO_D&>m_sevo;
	CMap<T_SEVO_K,T_SEVO_K,SevoList*,SevoList*>m_sevolist;
};

#endif