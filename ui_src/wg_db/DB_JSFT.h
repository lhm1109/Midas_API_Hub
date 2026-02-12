#ifndef __DB_JSFT_DB_H__
#define __DB_JSFT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_JSFT
{
public:
	CDB_JSFT();
	virtual ~CDB_JSFT();
	CDBDoc* m_pDoc;

public:
	void Add(T_JSFT_K Key,T_JSFT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_JSFT_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_JSFT_K Key,T_JSFT_D& rData);
		//{return m_jsft.Lookup(Key,rData);}
	int GetCount();
		//{return m_jsft.GetCount();}
	POSITION GetStart();
		//{return m_jsft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_JSFT_K& rKey,T_JSFT_D& rData);
		//{m_jsft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_JSFT_K,T_JSFT_K,T_JSFT_D,T_JSFT_D&>m_jsft;
};

#endif

