#ifndef __DB_DSPS_DB_H__
#define __DB_DSPS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_DESIGN.H"

class CDB_DSPS
{
public:
	CDB_DSPS();
	virtual ~CDB_DSPS();

public:
	T_DSPS_K m_nStartNum;
	T_DSPS_K m_nLastNum;

public:
	void Add(T_DSPS_K Key, T_DSPS_D& rData);
	BOOL Del(T_DSPS_K Key);

public:
	BOOL Get(T_DSPS_K Key, T_DSPS_D& rData)
		{return m_dsps.Lookup(Key,rData);}
	int GetCount()
		{return m_dsps.GetCount();}
	POSITION GetStart()
		{return m_dsps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_DSPS_K& rKey, T_DSPS_D& rData)
		{m_dsps.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DSPS_K, T_DSPS_K, T_DSPS_D, T_DSPS_D&> m_dsps;
};

#endif
