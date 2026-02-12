#ifndef __DB_GRUP_DB_H__
#define __DB_GRUP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NODE;
class CDB_ELEM;

class CDB_GRUP
{
public:
	CDB_GRUP();
	virtual ~CDB_GRUP();

public:
	T_GRUP_K m_nStartNum;
	T_GRUP_K m_nLastNum;

public:
	void Add(T_GRUP_K Key,T_GRUP_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_GRUP_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);
	void AddItem(T_GRUP_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL DelItem(T_GRUP_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_GRUP_K Key,T_GRUP_D& rData)
		{return m_grup.Lookup(Key,rData);}
	int GetCount()
		{return m_grup.GetCount();}
	POSITION GetStart()
		{return m_grup.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GRUP_K& rKey,T_GRUP_D& rData)
		{m_grup.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_GRUP_K Key);
	BOOL DelList(T_GRUP_K Key);
	void DelAllList();
	BOOL GetList(T_GRUP_K Key,GrupList*& rpList){return m_gruplist.Lookup(Key,rpList);}
	int GetCountList(){return m_gruplist.GetCount();}
	void AddListItem(T_GRUP_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_GRUP_K Key, int nCmd, int nKey);

protected:
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_D,T_GRUP_D&>m_grup;
	CMap<T_GRUP_K,T_GRUP_K,GrupList*,GrupList*>m_gruplist;
public:
	CMap<T_GRUP_K, T_GRUP_K, T_COLOR_D, T_COLOR_D&>m_grupcolor;
};

#endif