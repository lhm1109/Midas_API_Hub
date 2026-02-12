#ifndef __DB_RCPL_DB_H__
#define __DB_RCPL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RCPL
{
public:
	CDB_RCPL();
	virtual ~CDB_RCPL();
	CDBDoc* m_pDoc;

public:
	T_RCPL_K m_nStartNum;
	T_RCPL_K m_nLastNum;

public:
	void Add(T_RCPL_K Key,T_RCPL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RCPL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RCPL_K Key,T_RCPL_D& rData);
		//{return m_rcpl.Lookup(Key,rData);}
	int GetCount();
		//{return m_rcpl.GetCount();}
	POSITION GetStart();
		//{return m_rcpl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RCPL_K& rKey,T_RCPL_D& rData);
		//{m_rcpl.GetNextAssoc(rNextPosition,rKey,rData);}
	BOOL GetRcplAssigned(T_ELEM_K KeyElem, T_RCPL_K& rKeyRcpl);   // element가 속한 Rcpl 의 key를 찾아준다.
		//{return m_elemtorcpl.Lookup(KeyElem, rKeyRcpl);}
protected:
	CMap<T_RCPL_K,T_RCPL_K,T_RCPL_D,T_RCPL_D&>m_rcpl;
	CMap<T_ELEM_K,T_ELEM_K,T_RCPL_K,T_RCPL_K>  m_elemtorcpl;
};

#endif

