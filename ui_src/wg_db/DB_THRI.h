#ifndef __DB_THRI_DB_H__
#define __DB_THRI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THRI
{
public:
	CDB_THRI();
	virtual ~CDB_THRI();
	CDBDoc* m_pDoc;

public:
	T_THRI_K m_nStartNum;
	T_THRI_K m_nLastNum;

public:
	void Add(T_THRI_K Key,T_THRI_D& rData,CDB_THIS* pThis);
	BOOL Del(T_THRI_K Key,CDB_THIS* pThis);

public:
	BOOL Get(T_THRI_K Key,T_THRI_D& rData);
		//{return m_thri.Lookup(Key,rData);}
	int GetCount();
		//{return m_thri.GetCount();}
	POSITION GetStart();
		//{return m_thri.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THRI_K& rKey,T_THRI_D& rData);
		//{m_thri.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THRI_K,T_THRI_K,T_THRI_D,T_THRI_D&>m_thri;
};

#endif