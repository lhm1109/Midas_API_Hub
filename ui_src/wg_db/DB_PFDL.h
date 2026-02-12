#ifndef __DB_PFDL_DB_H__
#define __DB_PFDL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PFDL
{
public:
	CDB_PFDL();
	virtual ~CDB_PFDL();
	CDBDoc* m_pDoc;

public:
	void Add(T_PFDL_K Key,T_PFDL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PFDL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PFDL_K Key,T_PFDL_D& rData);
		//{return m_PFDL.Lookup(Key,rData);}
	int GetCount();
		//{return m_PFDL.GetCount();}
	POSITION GetStart();
		//{return m_PFDL.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PFDL_K& rKey,T_PFDL_D& rData);
		//{m_PFDL.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PFDL_K,T_PFDL_K,T_PFDL_D,T_PFDL_D&>m_PFDL;
};

#endif

