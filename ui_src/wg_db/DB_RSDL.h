#ifndef __DB_RSDL_DB_H__
#define __DB_RSDL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RSDL
{
public:
	CDB_RSDL();
	virtual ~CDB_RSDL();
	CDBDoc* m_pDoc;

public:
	T_RSDL_K m_nStartNum;
	T_RSDL_K m_nLastNum;

public:
	void Add(T_RSDL_K Key,T_RSDL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RSDL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RSDL_K Key,T_RSDL_D& rData);
		//{return m_rsdl.Lookup(Key,rData);}
	int GetCount();
		//{return m_rsdl.GetCount();}
	POSITION GetStart();
		//{return m_rsdl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RSDL_K& rKey,T_RSDL_D& rData);
		//{m_rsdl.GetNextAssoc(rNextPosition,rKey,rData);}
	BOOL GetRsdlAssigned(T_ELEM_K KeyElem, T_RSDL_K& rKeyRsdl);   // element가 속한 Rsdl 의 key를 찾아준다.
		//{return m_elemtorsdl.Lookup(KeyElem, rKeyRsdl);}
protected:
	CMap<T_RSDL_K,T_RSDL_K,T_RSDL_D,T_RSDL_D&>m_rsdl;
	CMap<T_ELEM_K,T_ELEM_K,T_RSDL_K,T_RSDL_K>  m_elemtorsdl;
};

#endif

