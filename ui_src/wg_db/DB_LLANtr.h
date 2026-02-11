#ifndef __DB_LLANtr_DB_H__
#define __DB_LLANtr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LLANtr
{
public:
	CDB_LLANtr();
	virtual ~CDB_LLANtr();
	CDBDoc* m_pDoc;

public:
	T_LLANtr_K m_nStartNum;
	T_LLANtr_K m_nLastNum;

public:
	void Add(T_LLANtr_K Key,T_LLANtr_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_LLANtr_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_LLANtr_K Key,T_LLANtr_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_LLANtr_K& rKey,T_LLANtr_D& rData);

protected:
	CMap<T_LLANtr_K,T_LLANtr_K,T_LLANtr_D,T_LLANtr_D&> m_llantr;
};

#endif

