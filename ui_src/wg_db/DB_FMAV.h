#ifndef __DB_FMAV_DB_H__
#define __DB_FMAV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_FMAV
{
public:
	CDB_FMAV();
	virtual ~CDB_FMAV();
	CDBDoc* m_pDoc;

public:
	void Add(T_FMAV_K Key, T_FMAV_D& rData);
	BOOL Del(T_FMAV_K Key);

public:
	BOOL Get(T_FMAV_K Key, T_FMAV_D& rData);
	//{return m_fmav.Lookup(Key,rData);}
	int GetCount();
	//{return m_fmav.GetCount();}
	POSITION GetStart();
	//{return m_fmav.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_FMAV_K& rKey, T_FMAV_D& rData);
	//{m_fmav.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_FMAV_K, T_FMAV_K, T_FMAV_D, T_FMAV_D&>m_fmav;
};

#endif

