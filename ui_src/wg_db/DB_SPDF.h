#ifndef __DB_SPDF_DB_H__
#define __DB_SPDF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPDF
{
public:
	CDB_SPDF();
	virtual ~CDB_SPDF();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPDF_K Key,T_SPDF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SPDF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SPDF_K Key,T_SPDF_D& rData);
		//{return m_SPDF.Lookup(Key,rData);}
	int GetCount();
		//{return m_SPDF.GetCount();}
	POSITION GetStart();
		//{return m_SPDF.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPDF_K& rKey,T_SPDF_D& rData);
		//{m_SPDF.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPDF_K,T_SPDF_K,T_SPDF_D,T_SPDF_D&>m_spdf;
};

#endif

