#ifndef __DB_RLCC_DB_H__
#define __DB_RLCC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RLCC
{
public:
	CDB_RLCC();
	virtual ~CDB_RLCC();
	CDBDoc* m_pDoc;

public:
	T_RLCC_K m_nStartNum;
	T_RLCC_K m_nLastNum;

public:
	void Add(T_RLCC_K Key,T_RLCC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RLCC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RLCC_K Key,T_RLCC_D& rData);
		//{return m_rlcc.Lookup(Key,rData);}
	int GetCount();
		//{return m_rlcc.GetCount();}
	POSITION GetStart();
		//{return m_rlcc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RLCC_K& rKey,T_RLCC_D& rData);
		//{m_rlcc.GetNextAssoc(rNextPosition,rKey,rData);}
	BOOL GetRcplAssigned(T_ELEM_K KeyElem, T_RLCC_K& rKeyRcpl);   // element가 속한 Rcpl 의 key를 찾아준다.
		//{return m_elemtorlcc.Lookup(KeyElem, rKeyRcpl);}
protected:
	CMap<T_RLCC_K,T_RLCC_K,T_RLCC_D,T_RLCC_D&>m_rlcc;
	CMap<T_ELEM_K,T_ELEM_K,T_RLCC_K,T_RLCC_K>  m_elemtorlcc;
};

#endif

