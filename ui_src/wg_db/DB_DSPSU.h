#ifndef __DB_DSPSU_DB_H__
#define __DB_DSPSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_DESIGN.H"

class CDB_DSPSU
{
public:
	CDB_DSPSU()
	{
		m_dsps.InitHashTable(HASHSIZEDSPS);
	}
	virtual ~CDB_DSPSU(){};

public:
	void Add(T_UDRD_KEY Key, T_DSPS_UDRD_D& rData)
		{m_dsps.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dsps.RemoveKey(Key);}
	void DelAll()
		{m_dsps.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key, T_DSPS_UDRD_D& rData)
		{return m_dsps.Lookup(Key,rData);}
	int GetCount()
		{return m_dsps.GetCount();}
	POSITION GetStart()
		{return m_dsps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_DSPS_UDRD_D& rData)
		{m_dsps.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_DSPS_UDRD_D, T_DSPS_UDRD_D&> m_dsps;
};

#endif
