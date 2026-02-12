#ifndef __DB_FMAG_DB_H__
#define __DB_FMAG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_FMAG
{
public:
	CDB_FMAG();
	virtual ~CDB_FMAG();
	CDBDoc* m_pDoc;

public:
	void Add(T_FMAG_K Key,T_FMAG_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_FMAG_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_FMAG_K Key,T_FMAG_D& rData);
		//{return m_fmag.Lookup(Key,rData);}
	int GetCount();
		//{return m_fmag.GetCount();}
	POSITION GetStart();
		//{return m_fmag.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FMAG_K& rKey,T_FMAG_D& rData);
		//{m_fmag.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_FMAG_K,T_FMAG_K,T_FMAG_D,T_FMAG_D&>m_fmag;
};

#endif

