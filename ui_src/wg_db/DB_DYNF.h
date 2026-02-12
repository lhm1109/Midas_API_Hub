#ifndef __DB_DYNF_DB_H__
#define __DB_DYNF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DYNF
{
public:
	CDB_DYNF();
	virtual ~CDB_DYNF();
	CDBDoc* m_pDoc;

public:
	void Add(T_DYNF_K Key,T_DYNF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DYNF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DYNF_K Key,T_DYNF_D& rData);
		//{return m_dynf.Lookup(Key,rData);}
	int GetCount();
		//{return m_dynf.GetCount();}
	POSITION GetStart();
		//{return m_dynf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DYNF_K& rKey,T_DYNF_D& rData);
		//{m_dynf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DYNF_K,T_DYNF_K,T_DYNF_D,T_DYNF_D&>m_dynf;
};

#endif