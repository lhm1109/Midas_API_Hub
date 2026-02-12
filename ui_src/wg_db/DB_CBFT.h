#ifndef __DB_CBFT_DB_H__
#define __DB_CBFT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CBFT
{
public:
	CDB_CBFT();
	virtual ~CDB_CBFT();
	CDBDoc* m_pDoc;

public:
	void Add(T_CBFT_K Key,T_CBFT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CBFT_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CBFT_K Key,T_CBFT_D& rData);
		//{return m_cbft.Lookup(Key,rData);}
	int GetCount();
		//{return m_cbft.GetCount();}
	POSITION GetStart();
		//{return m_cbft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CBFT_K& rKey,T_CBFT_D& rData);
		//{m_cbft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CBFT_K,T_CBFT_K,T_CBFT_D,T_CBFT_D&>m_cbft;
};

#endif

