#ifndef __DB_GIMPU_DB_H__
#define __DB_GIMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GIMPU
{
public:
	CDB_GIMPU()
	{
		m_gimp.InitHashTable(HASHSIZEGIMP);
	}
	virtual ~CDB_GIMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_GIMP_UDRD_D& rData)
		{m_gimp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gimp.RemoveKey(Key);}
	void DelAll()
		{m_gimp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GIMP_UDRD_D& rData)
		{return m_gimp.Lookup(Key,rData);}
	int GetCount()
		{return m_gimp.GetCount();}
	POSITION GetStart()
		{return m_gimp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GIMP_UDRD_D& rData)
		{m_gimp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GIMP_UDRD_D,T_GIMP_UDRD_D&>m_gimp;
};

#endif