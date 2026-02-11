#ifndef __DB_ASGB_DB_H__
#define __DB_ASGB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_ASGB
{
public:
	CDB_ASGB();
	virtual ~CDB_ASGB();
	CDBDoc* m_pDoc;

public:
	T_ASGB_K m_nStartNum;
	T_ASGB_K m_nLastNum;

public:
	void Add(T_ASGB_K Key,T_ASGB_D& rData);
	BOOL Del(T_ASGB_K Key);

public:
	BOOL GetListFromBaseDB(T_ASGB_K Key,AsgbList*& rpList){return m_asgblist.Lookup(Key,rpList);}

	BOOL Get(T_ASGB_K Key,T_ASGB_D& rData)
		{return m_asgb.Lookup(Key,rData);}
	BOOL GetSeqKey(int nSeq, T_ASGB_K& Key)
		{return m_asgbSeq.Lookup(nSeq, Key);}
	int GetCount()
		{return m_asgb.GetCount();}
	POSITION GetStart()
		{return m_asgb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ASGB_K& rKey,T_ASGB_D& rData)
		{m_asgb.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_ASGB_K Key);
	BOOL DelList(T_ASGB_K Key);
	void DelAllList();
	BOOL GetList(T_ASGB_K Key,AsgbList*& rpList);//{return m_asgblist.Lookup(Key,rpList);}
	int GetCountList();//{return m_asgblist.GetCount();}
	void AddListItem(T_ASGB_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_ASGB_K Key, int nCmd, int nKey);

protected:
	CMap<T_ASGB_K,T_ASGB_K,T_ASGB_D,T_ASGB_D&>m_asgb;
	CMap<int,int,T_ASGB_K,T_ASGB_K&>m_asgbSeq;
	CMap<T_ASGB_K,T_ASGB_K,AsgbList*,AsgbList*>m_asgblist;
};

#endif
