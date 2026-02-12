#ifndef __DB_ELNK_DB_H__
#define __DB_ELNK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELNK;
class CDB_BNGR;

class CDB_ELNK
{
public:
	CDB_ELNK();
	virtual ~CDB_ELNK();
	CDBDoc* m_pDoc;

public:
	T_ELNK_K m_nStartNum;
	T_ELNK_K m_nLastNum;

public:
	void Add(T_ELNK_K Key,T_ELNK_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr);
	BOOL Del(T_ELNK_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr);

public:
	BOOL Get(T_ELNK_K Key,T_ELNK_D& rData);
		//{return m_elnk.Lookup(Key,rData);}
	int GetCount();
		//{return m_elnk.GetCount();}
	POSITION GetStart();
		//{return m_elnk.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ELNK_K& rKey,T_ELNK_D& rData);
		//{m_elnk.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_ELNK_K Key);
	BOOL DelList(T_ELNK_K Key);
	void DelAllList();
	BOOL GetList(T_ELNK_K Key, ElnkList*& rpList);//{return m_elnklist.Lookup(Key,rpList);}
	int GetCountList();//{return m_elnklist.GetCount();}
	void AddListItem(T_ELNK_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_ELNK_K Key, int nCmd, int nKey);

protected:
	CMap<T_ELNK_K,T_ELNK_K,T_ELNK_D,T_ELNK_D&>m_elnk;
	CMap<T_ELNK_K,T_ELNK_K, ElnkList*,ElnkList*>m_elnklist;
};

#endif