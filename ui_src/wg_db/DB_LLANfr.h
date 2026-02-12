#ifndef __DB_LLANfr_DB_H__
#define __DB_LLANfr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LLANfr
{
public:
	CDB_LLANfr();
	virtual ~CDB_LLANfr();
	CDBDoc* m_pDoc;

public:
	T_LLANfr_K m_nStartNum;
	T_LLANfr_K m_nLastNum;

public:
	void Add(T_LLANfr_K Key,T_LLANfr_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_LLANfr_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_LLANfr_K Key,T_LLANfr_D& rData);
		//{return m_llanfr.Lookup(Key,rData);}
	int GetCount();
		//{return m_llanfr.GetCount();}
	POSITION GetStart();
		//{return m_llanfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LLANfr_K& rKey,T_LLANfr_D& rData);
		//{m_llanfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LLANfr_K,T_LLANfr_K,T_LLANfr_D,T_LLANfr_D&> m_llanfr;
};

#endif

