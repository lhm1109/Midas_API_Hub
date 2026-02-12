#ifndef __DB_MATLU_DB_H__
#define __DB_MATLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATLU
{
public:
	CDB_MATLU()
	{
		m_matl.InitHashTable(HASHSIZEMATL);
	}
	virtual ~CDB_MATLU(){};

public:
	void Add(T_UDRD_KEY Key,T_MATL_UDRD_D& rData)
		{m_matl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_matl.RemoveKey(Key);}
	void DelAll()
		{m_matl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MATL_UDRD_D& rData)
		{return m_matl.Lookup(Key,rData);}
	int GetCount()
		{return m_matl.GetCount();}
	POSITION GetStart()
		{return m_matl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MATL_UDRD_D& rData)
		{m_matl.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddDesign(T_UDRD_KEY Key,T_MATD_UDRD_D& rData)
		{m_matldesign.SetAt(Key,rData);}
	BOOL DelDesign(T_UDRD_KEY Key)
		{return m_matldesign.RemoveKey(Key);}
	void DelAllDesign()
		{m_matldesign.RemoveAll();}
	BOOL GetDesign(T_UDRD_KEY Key,T_MATD_UDRD_D& rData)
		{return m_matldesign.Lookup(Key,rData);}
	POSITION GetStartDesign()
		{return m_matldesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MATD_UDRD_D& rData)
		{m_matldesign.GetNextAssoc(rNextPosition,rKey,rData);}
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MATL_UDRD_D,T_MATL_UDRD_D&>m_matl;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MATD_UDRD_D,T_MATD_UDRD_D&>m_matldesign;
};

#endif