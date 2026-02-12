#ifndef __DB_LLANid_DB_H__
#define __DB_LLANid_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LLANid
{
public:
	CDB_LLANid();
	virtual ~CDB_LLANid();
	CDBDoc* m_pDoc;

public:
	T_LLANid_K m_nStartNum;
	T_LLANid_K m_nLastNum;

public:
	void Add(T_LLANid_K Key,T_LLANid_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_LLANid_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_LLANid_K Key,T_LLANid_D& rData);
		//{return m_llanid.Lookup(Key,rData);}
	int GetCount();
		//{return m_llanid.GetCount();}
	POSITION GetStart();
		//{return m_llanid.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LLANid_K& rKey,T_LLANid_D& rData);
		//{m_llanid.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LLANid_K,T_LLANid_K,T_LLANid_D,T_LLANid_D&> m_llanid;
};

#endif

