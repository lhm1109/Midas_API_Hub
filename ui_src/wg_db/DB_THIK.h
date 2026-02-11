#ifndef __DB_THIK_DB_H__
#define __DB_THIK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;

class CDB_THIK
{
public:
	CDB_THIK();
	virtual ~CDB_THIK();

public:
	T_THIK_K m_nLastNum;
	T_THIK_K m_nStartNum;

public:
	void Add(T_THIK_K Key,T_THIK_D& rData);
	BOOL Del(T_THIK_K Key);

public:
	BOOL Get(T_THIK_K Key,T_THIK_D& rData)
		{return m_thik.Lookup(Key,rData);}
	int GetCount()
		{return m_thik.GetCount();}
	POSITION GetStart()
		{return m_thik.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THIK_K& rKey,T_THIK_D& rData)
		{m_thik.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_THIK_K Key);
	BOOL DelList(T_THIK_K Key);
	void DelAllList();
	BOOL GetList(T_THIK_K Key,ThikList*& rpList){return m_thiklist.Lookup(Key,rpList);}
	int GetCountList(){return m_thiklist.GetCount();}
	void AddListItem(T_THIK_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_THIK_K Key, int nCmd, int nKey);

protected:
	CMap<T_THIK_K,T_THIK_K,T_THIK_D,T_THIK_D&>m_thik;
	CMap<T_THIK_K,T_THIK_K,ThikList*,ThikList*>m_thiklist;
public:
	CMap<T_THIK_K,T_THIK_K,T_COLOR_D,T_COLOR_D&>m_thikcolor;
};

#endif
