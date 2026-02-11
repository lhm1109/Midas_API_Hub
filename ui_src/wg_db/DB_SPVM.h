#ifndef __DB_SPVM_DB_H__
#define __DB_SPVM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPVM
{
public:
	CDB_SPVM();
	virtual ~CDB_SPVM();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPVM_K Key,T_SPVM_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SPVM_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SPVM_K Key,T_SPVM_D& rData);
		//{return m_mbtp.Lookup(Key,rData);}
	int GetCount();
		//{return m_mbtp.GetCount();}
	POSITION GetStart();
		//{return m_mbtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPVM_K& rKey,T_SPVM_D& rData);
		//{m_mbtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPVM_K, T_SPVM_K,T_SPVM_D,T_SPVM_D&>m_spvm;//Special Provision of Elem for AREMA 
};

#endif

