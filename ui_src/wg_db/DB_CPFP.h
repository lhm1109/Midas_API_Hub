#ifndef __DB_CPFP_DB_H__
#define __DB_CPFP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPFP
{
public:
	CDB_CPFP();
	virtual ~CDB_CPFP();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPFP_K Key,T_CPFP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPFP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPFP_K Key,T_CPFP_D& rData);
		//{return m_cpfp.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpfp.GetCount();}
	POSITION GetStart();
		//{return m_cpfp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPFP_K& rKey,T_CPFP_D& rData);
		//{m_cpfp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPFP_K,T_CPFP_K,T_CPFP_D,T_CPFP_D&>m_cpfp;
};

#endif

