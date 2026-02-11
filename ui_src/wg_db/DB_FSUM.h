#ifndef __DB_FSUM_DB_H__
#define __DB_FSUM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_FSUM
{
public:
	CDB_FSUM();
	virtual ~CDB_FSUM();
	CDBDoc* m_pDoc;

public:
	T_FSUM_K m_nStartNum;
	T_FSUM_K m_nLastNum;

public:
	void Add(T_FSUM_K Key, T_FSUM_D& rData, CDB_ELEM* pElem);  
	BOOL Del(T_FSUM_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_FSUM_K Key,T_FSUM_D& rData);
	//  {return m_fsum.Lookup(Key,rData);}
	int GetCount();
	//  {return m_fsum.GetCount();}
	POSITION GetStart();
	//  {return m_fsum.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FSUM_K& rKey,T_FSUM_D& rData);
	//  {m_fsum.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_FSUM_K,T_FSUM_K,T_FSUM_D,T_FSUM_D&>m_fsum;
};

#endif
