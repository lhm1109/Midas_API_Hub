#ifndef __DB_CHRPU_DB_H__
#define __DB_CHRPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CHRPU
{
public:
	CDB_CHRPU()
	{
		m_chrp.InitHashTable(HASHSIZECHRP);
	}
	virtual ~CDB_CHRPU(){};

public:
	void Add(T_UDRD_KEY Key,T_CHRP_UDRD_D& rData)
		{m_chrp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_chrp.RemoveKey(Key);}
	void DelAll()
		{m_chrp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CHRP_UDRD_D& rData)
		{return m_chrp.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CHRP_UDRD_D,T_CHRP_UDRD_D&>m_chrp;
};

#endif