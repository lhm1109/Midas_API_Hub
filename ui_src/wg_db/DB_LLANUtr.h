#ifndef __DB_LLANUtr_DB_H__
#define __DB_LLANUtr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LLANUtr
{
public:
	CDB_LLANUtr()
	{
		m_llantr.InitHashTable(HASHSIZELLANtr);
	}
	virtual ~CDB_LLANUtr(){};

public:
	void Add(T_UDRD_KEY Key,T_LLANtr_UDRD_D& rData)
		{m_llantr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_llantr.RemoveKey(Key);}
	void DelAll()
		{m_llantr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLANtr_UDRD_D& rData)
		{return m_llantr.Lookup(Key,rData);}
	int GetCount()
		{return m_llantr.GetCount();}
	POSITION GetStart()
		{return m_llantr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LLANtr_UDRD_D& rData)
		{m_llantr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLANtr_UDRD_D,T_LLANtr_UDRD_D&>m_llantr;
};

#endif
