#ifndef __DB_RRPL_DB_H__
#define __DB_RRPL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RRPL
{
public:
	CDB_RRPL();
	virtual ~CDB_RRPL();
	CDBDoc* m_pDoc;

public:
	T_RRPL_K m_nStartNum;
	T_RRPL_K m_nLastNum;

public:
	void Add(T_RRPL_K Key,T_RRPL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RRPL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RRPL_K Key,T_RRPL_D& rData);
	//{return m_rrpl.Lookup(Key,rData);}
	int GetCount();
	//{return m_rrpl.GetCount();}
	POSITION GetStart();
	//{return m_rrpl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RRPL_K& rKey,T_RRPL_D& rData);
	//{m_rrpl.GetNextAssoc(rNextPosition,rKey,rData);}
	BOOL GetRrplAssigned(T_ELEM_K KeyElem, T_RRPL_K& rKeyRrpl);   // element가 속한 Rrpl 의 key를 찾아준다.
	//{return m_elemtorrpl.Lookup(KeyElem, rKeyRrpl);}
protected:
	CMap<T_RRPL_K,T_RRPL_K,T_RRPL_D,T_RRPL_D&>m_rrpl;
	CMap<T_ELEM_K,T_ELEM_K,T_RRPL_K,T_RRPL_K>  m_elemtorrpl;
};

#endif

