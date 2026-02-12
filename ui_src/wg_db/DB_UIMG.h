#ifndef __DB_UIMG_DB_H__
#define __DB_UIMG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_UIMG
{
public:
	CDB_UIMG();
	virtual ~CDB_UIMG();
	CDBDoc* m_pDoc;

public:
	T_UIMG_K m_nLastNum;
	T_UIMG_K m_nStartNum;

public:
	void Add(T_UIMG_K Key,T_UIMG_D& rData);
	BOOL Del(T_UIMG_K Key);

public:
	BOOL Get(T_UIMG_K Key,T_UIMG_D& rData)
		{return m_uimg.Lookup(Key,rData);}
	int GetCount()
		{return m_uimg.GetCount();}
	POSITION GetStart()
		{return m_uimg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UIMG_K& rKey,T_UIMG_D& rData)
		{m_uimg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UIMG_K,T_UIMG_K,T_UIMG_D,T_UIMG_D&>m_uimg;
};

#endif
