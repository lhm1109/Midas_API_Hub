#ifndef __DB_THGA_DB_H__
#define __DB_THGA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THGA
{
public:
	CDB_THGA();
	virtual ~CDB_THGA();
	CDBDoc* m_pDoc;

public:
	void Add(T_THGA_K Key,T_THGA_D& rData,CDB_THIS* pThis);
	BOOL Del(T_THGA_K Key,CDB_THIS* pThis);

public:
	BOOL Get(T_THGA_K Key,T_THGA_D& rData);
		//{return m_thga.Lookup(Key,rData);}
	int GetCount();
		//{return m_thga.GetCount();}
	POSITION GetStart();
		//{return m_thga.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THGA_K& rKey,T_THGA_D& rData);
		//{m_thga.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THGA_K,T_THGA_K,T_THGA_D,T_THGA_D&>m_thga;
};

#endif