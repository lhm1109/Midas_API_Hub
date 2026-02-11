#ifndef __DB_DMGB_DB_H__
#define __DB_DMGB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DMGB
{
public:
	CDB_DMGB();
	virtual ~CDB_DMGB();
	CDBDoc* m_pDoc;

public:
	T_DMGB_K m_nStartNum;
	T_DMGB_K m_nLastNum;

public:
	void Add(T_DMGB_K Key,T_DMGB_D& rData);
	BOOL Del(T_DMGB_K Key);

public:
	BOOL GetListFromBaseDB(T_DMGB_K Key,DmgbList*& rpList){return m_dmgblist.Lookup(Key,rpList);}

	BOOL Get(T_DMGB_K Key,T_DMGB_D& rData)
		{return m_dmgb.Lookup(Key,rData);}
	BOOL GetSeqKey(int nSeq, T_DMGB_K& Key)
		{return m_dmgbSeq.Lookup(nSeq, Key);}
	int GetCount()
		{return m_dmgb.GetCount();}
	POSITION GetStart()
		{return m_dmgb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DMGB_K& rKey,T_DMGB_D& rData)
		{m_dmgb.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_DMGB_K Key);
	BOOL DelList(T_DMGB_K Key);
	void DelAllList();
	BOOL GetList(T_DMGB_K Key,DmgbList*& rpList);//{return m_dmgblist.Lookup(Key,rpList);}
	int GetCountList();//{return m_dmgblist.GetCount();}
	void AddListItem(T_DMGB_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_DMGB_K Key, int nCmd, int nKey);

protected:
	CMap<T_DMGB_K,T_DMGB_K,T_DMGB_D,T_DMGB_D&>m_dmgb;
	CMap<int,int,T_DMGB_K,T_DMGB_K&>m_dmgbSeq;
	CMap<T_DMGB_K,T_DMGB_K,DmgbList*,DmgbList*>m_dmgblist;
};

#endif
