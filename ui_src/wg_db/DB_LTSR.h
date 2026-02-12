#ifndef __DB_LTSR_DB_H__
#define __DB_LTSR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LTSR
{
public:
	CDB_LTSR();
	virtual ~CDB_LTSR();
	CDBDoc* m_pDoc;

public:
	void Add(T_LTSR_K Key,T_LTSR_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_LTSR_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_LTSR_K Key,T_LTSR_D& rData);
		//{return m_ltsr.Lookup(Key,rData);}
	int GetCount();
		//{return m_ltsr.GetCount();}
	POSITION GetStart();
		//{return m_ltsr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LTSR_K& rKey,T_LTSR_D& rData);
		//{m_ltsr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LTSR_K,T_LTSR_K,T_LTSR_D,T_LTSR_D&>m_ltsr;
};

#endif

