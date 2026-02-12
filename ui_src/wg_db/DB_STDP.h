#ifndef __DB_STDP_DB_H__
#define __DB_STDP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_STDP
{
public:
	CDB_STDP();
	virtual ~CDB_STDP();
	CDBDoc* m_pDoc;

public:
	void Add(T_STDP_K Key,T_STDP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_STDP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_STDP_K Key,T_STDP_D& rData);
		//{return m_stdp.Lookup(Key,rData);}
	int GetCount();
		//{return m_stdp.GetCount();}
	POSITION GetStart();
		//{return m_stdp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STDP_K& rKey,T_STDP_D& rData);
		//{m_stdp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STDP_K,T_STDP_K,T_STDP_D,T_STDP_D&>m_stdp;
};

#endif

