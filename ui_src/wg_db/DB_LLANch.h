#ifndef __DB_LLANch_DB_H__
#define __DB_LLANch_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LLANch
{
public:
	CDB_LLANch();
	virtual ~CDB_LLANch();
	CDBDoc* m_pDoc;

public:
	T_LLANch_K m_nStartNum;
	T_LLANch_K m_nLastNum;

public:
	void Add(T_LLANch_K Key,T_LLANch_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_LLANch_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_LLANch_K Key,T_LLANch_D& rData);
		//{return m_llanch.Lookup(Key,rData);}
	int GetCount();
		//{return m_llanch.GetCount();}
	POSITION GetStart();
		//{return m_llanch.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LLANch_K& rKey,T_LLANch_D& rData);
		//{m_llanch.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LLANch_K,T_LLANch_K,T_LLANch_D,T_LLANch_D&> m_llanch;
};

#endif

