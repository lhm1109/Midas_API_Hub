#ifndef __DB_SINF_DB_H__
#define __DB_SINF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SINF
{
public:
	CDB_SINF();
	virtual ~CDB_SINF();
	CDBDoc* m_pDoc;

public:
	void Add(T_SINF_K Key,T_SINF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SINF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SINF_K Key,T_SINF_D& rData);
		//{return m_sinf.Lookup(Key,rData);}
	int GetCount();
		//{return m_sinf.GetCount();}
	POSITION GetStart();
		//{return m_sinf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SINF_K& rKey,T_SINF_D& rData);
		//{m_sinf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SINF_K,T_SINF_K,T_SINF_D,T_SINF_D&>m_sinf;
};

#endif