#ifndef __DB_BREV_DB_H__
#define __DB_BREV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_BREV
{
public:
	CDB_BREV();
	virtual ~CDB_BREV();
	CDBDoc* m_pDoc;

public:
	T_BREV_K m_nStartNum;
	T_BREV_K m_nLastNum;

public:
	void Add(T_BREV_K Key,T_BREV_D& rData);
	BOOL Del(T_BREV_K Key);

public:
	BOOL Get(T_BREV_K Key,T_BREV_D& rData);
	//{return m_ssps.Lookup(Key.keymap,rData);}
	int GetCount();
	//{return m_ssps.GetCount();}
	POSITION GetStart();
	//{return m_ssps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BREV_K& rKey,T_BREV_D& rData);
	//{m_ssps.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_BREV_K,T_BREV_K,T_BREV_D,T_BREV_D&>m_brev;
};

#endif
