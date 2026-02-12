#ifndef __DB_NLLP_DB_H__
#define __DB_NLLP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_NLLP
{
public:
	CDB_NLLP();
	virtual ~CDB_NLLP();
	CDBDoc* m_pDoc;

public:
	T_NLLP_K m_nStartNum;
	T_NLLP_K m_nLastNum;

public:
	void Add(T_NLLP_K Key,T_NLLP_D& rData);
	BOOL Del(T_NLLP_K Key);

public:
	BOOL GetListFromBaseDB(T_NLLP_K Key,NllpList*& rpList){return m_nllplist.Lookup(Key,rpList);}

	BOOL Get(T_NLLP_K Key,T_NLLP_D& rData)
		{return m_nllp.Lookup(Key,rData);}
	BOOL GetSeqKey(int nSeq, T_NLLP_K& Key)
		{return m_nllpSeq.Lookup(nSeq, Key);}
	int GetCount()
		{return m_nllp.GetCount();}
	POSITION GetStart()
		{return m_nllp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NLLP_K& rKey,T_NLLP_D& rData)
		{m_nllp.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_NLLP_K Key);
	BOOL DelList(T_NLLP_K Key);
	void DelAllList();
	BOOL GetList(T_NLLP_K Key,NllpList*& rpList);//{return m_nllplist.Lookup(Key,rpList);}
	int GetCountList();//{return m_nllplist.GetCount();}
	void AddListItem(T_NLLP_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_NLLP_K Key, int nCmd, int nKey);

protected:
	CMap<T_NLLP_K,T_NLLP_K,T_NLLP_D,T_NLLP_D&>m_nllp;
	CMap<int,int,T_NLLP_K,T_NLLP_K&>m_nllpSeq;
	CMap<T_NLLP_K,T_NLLP_K,NllpList*,NllpList*>m_nllplist;
};

#endif
