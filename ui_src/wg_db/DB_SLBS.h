#ifndef __DB_SLBS_DB_H__
#define __DB_SLBS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_NODE;
class CDB_ELEM;

class CDB_SLBS
{
public:
	CDB_SLBS();
	virtual ~CDB_SLBS();

public:
	T_SLBS_K m_nStartNum;
	T_SLBS_K m_nLastNum;

public:
	void Add(T_SLBS_K Key,T_SLBS_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_SLBS_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);
	void AddItem(T_SLBS_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL DelItem(T_SLBS_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem);

public:
	BOOL Get(T_SLBS_K Key,T_SLBS_D& rData)
	{return m_slbs.Lookup(Key,rData);}
	int GetCount()
	{return m_slbs.GetCount();}
	POSITION GetStart()
	{return m_slbs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SLBS_K& rKey,T_SLBS_D& rData)
	{m_slbs.GetNextAssoc(rNextPosition,rKey,rData);}
	
	void AddList(T_SLBS_K Key);
	BOOL DelList(T_SLBS_K Key);
	void DelAllList();
	BOOL GetList(T_SLBS_K Key,SlbsList*& rpList){return m_slbslist.Lookup(Key,rpList);}
	int GetCountList(){return m_slbslist.GetCount();}
	void AddListItem(T_SLBS_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SLBS_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_SLBS_K,T_SLBS_K,T_SLBS_D,T_SLBS_D&>m_slbs;
	CMap<T_SLBS_K,T_SLBS_K,SlbsList*,SlbsList*>m_slbslist;
};

#endif