#ifndef __DB_MCMB_DB_H__
#define __DB_MCMB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_MCMB
{
public:
	CDB_MCMB();
	virtual ~CDB_MCMB();
	CDBDoc* m_pDoc;

public:
	void Add(T_MCMB_K Key,T_MCMB_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MCMB_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_MCMB_K Key,T_MCMB_D& rData);
		//{return m_mcmb.Lookup(Key,rData);}
	int GetCount();
		//{return m_mcmb.GetCount();}
	POSITION GetStart();
		//{return m_mcmb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MCMB_K& rKey,T_MCMB_D& rData);
		//{m_mcmb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MCMB_K,T_MCMB_K,T_MCMB_D,T_MCMB_D&>m_mcmb;
};

#endif

