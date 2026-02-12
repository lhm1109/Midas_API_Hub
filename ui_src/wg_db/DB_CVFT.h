#ifndef __DB_CVFT_DB_H__
#define __DB_CVFT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CVFT
{
public:
	CDB_CVFT();
	virtual ~CDB_CVFT();
	CDBDoc* m_pDoc;

public:
	void Add(T_CVFT_K Key,T_CVFT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CVFT_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CVFT_K Key,T_CVFT_D& rData);
		//{return m_cvft.Lookup(Key,rData);}
	int GetCount();
		//{return m_cvft.GetCount();}
	POSITION GetStart();
		//{return m_cvft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CVFT_K& rKey,T_CVFT_D& rData);
		//{m_cvft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CVFT_K,T_CVFT_K,T_CVFT_D,T_CVFT_D&>m_cvft;
};

#endif

