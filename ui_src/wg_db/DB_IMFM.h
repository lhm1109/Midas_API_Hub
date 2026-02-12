#ifndef __DB_IMFM_DB_H__
#define __DB_IMFM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATL;

class CDB_IMFM
{
public:
	CDB_IMFM();
	virtual ~CDB_IMFM();

public:
	void Add(T_IMFM_K Key,T_IMFM_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_IMFM_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_IMFM_K Key,T_IMFM_D& rData)
		{return m_imfm.Lookup(Key,rData);}
	int GetCount()
		{return m_imfm.GetCount();}
	POSITION GetStart()
		{return m_imfm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_IMFM_K& rKey,T_IMFM_D& rData)
		{m_imfm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_IMFM_K,T_IMFM_K,T_IMFM_D,T_IMFM_D&>m_imfm;
};

#endif