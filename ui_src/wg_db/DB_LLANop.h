#ifndef __DB_LLANop_DB_H__
#define __DB_LLANop_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LLANop
{
public:
	CDB_LLANop();
	virtual ~CDB_LLANop();
	CDBDoc* m_pDoc;

public:
	T_LLANop_K m_nStartNum;
	T_LLANop_K m_nLastNum;

public:
	void Add(T_LLANop_K Key,T_LLANop_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_LLANop_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_LLANop_K Key,T_LLANop_D& rData);
		//{return m_llanop.Lookup(Key,rData);}
	int GetCount();
		//{return m_llanop.GetCount();}
	POSITION GetStart();
		//{return m_llanop.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LLANop_K& rKey,T_LLANop_D& rData);
		//{m_llanop.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LLANop_K,T_LLANop_K,T_LLANop_D,T_LLANop_D&>m_llanop;
};

#endif

