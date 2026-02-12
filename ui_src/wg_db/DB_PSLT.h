#ifndef __DB_PSLT_DB_H__
#define __DB_PSLT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSLT
{
public:
	CDB_PSLT();
	virtual ~CDB_PSLT();

public:
	T_PSLT_K m_nStartNum;
	T_PSLT_K m_nLastNum;

public:
	void Add(T_PSLT_K Key, T_PSLT_D& rData);
	BOOL Del(T_PSLT_K Key);

public:
	BOOL Get(T_PSLT_K Key,T_PSLT_D& rData)
	{return m_pslt.Lookup(Key,rData);}
	int GetCount()
	{return m_pslt.GetCount();}
	POSITION GetStart()
	{return m_pslt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PSLT_K& rKey,T_PSLT_D& rData)
	{m_pslt.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_PSLT_K Key);
	BOOL DelList(T_PSLT_K Key);
	void DelAllList();
	BOOL GetList(T_PSLT_K Key,PsltList*& rpList){return m_psltlist.Lookup(Key,rpList);}
	void AddListItem(T_PSLT_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_PSLT_K Key, int nCmd, int nKey,BOOL bOneElemAllPres = FALSE);
protected:
	CMap<T_PSLT_K,T_PSLT_K,T_PSLT_D,T_PSLT_D&>m_pslt;
	CMap<T_PSLT_K,T_PSLT_K,PsltList*,PsltList*>m_psltlist;
};

#endif