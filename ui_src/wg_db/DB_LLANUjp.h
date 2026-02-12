#ifndef __DB_LLANUjp_DB_H__
#define __DB_LLANUjp_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LLANUjp
{
public:
	CDB_LLANUjp()
	{
		m_llanjp.InitHashTable(HASHSIZELLANjp);
	}
	virtual ~CDB_LLANUjp(){};

public:
	void Add(T_UDRD_KEY Key,T_LLANjp_UDRD_D& rData)
		{m_llanjp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_llanjp.RemoveKey(Key);}
	void DelAll()
		{m_llanjp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLANjp_UDRD_D& rData)
		{return m_llanjp.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLANjp_UDRD_D,T_LLANjp_UDRD_D&>m_llanjp;
};

#endif