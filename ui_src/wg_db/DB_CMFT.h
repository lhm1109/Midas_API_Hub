#ifndef __DB_CMFT_DB_H__
#define __DB_CMFT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CMFT
{
public:
	CDB_CMFT();
	virtual ~CDB_CMFT();
	CDBDoc* m_pDoc;

public:
	void Add(T_CMFT_K Key,T_CMFT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CMFT_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CMFT_K Key,T_CMFT_D& rData);
		//{return m_cmft.Lookup(Key,rData);}
	int GetCount();
		//{return m_cmft.GetCount();}
	POSITION GetStart();
		//{return m_cmft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CMFT_K& rKey,T_CMFT_D& rData);
		//{m_cmft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CMFT_K,T_CMFT_K,T_CMFT_D,T_CMFT_D&>m_cmft;
};

#endif

