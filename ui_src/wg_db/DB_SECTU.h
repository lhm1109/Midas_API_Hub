#ifndef __DB_SECTU_DB_H__
#define __DB_SECTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECTU
{
public:
	CDB_SECTU()
	{
		m_sect.InitHashTable(HASHSIZESECT);
		m_sectdesign.InitHashTable(HASHSIZESECT);
	}
	virtual ~CDB_SECTU(){};

public:
	void Add(T_UDRD_KEY Key,T_SECT_UDRD_D& rData)
		{m_sect.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sect.RemoveKey(Key);}
	void DelAll()
		{m_sect.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SECT_UDRD_D& rData)
		{return m_sect.Lookup(Key,rData);}
	int GetCount()
		{return m_sect.GetCount();}
	POSITION GetStart()
		{return m_sect.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SECT_UDRD_D& rData)
		{m_sect.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddDesign(T_UDRD_KEY Key,T_SECT_UDRD_D& rData)
		{m_sectdesign.SetAt(Key,rData);}
	BOOL DelDesign(T_UDRD_KEY Key)
		{return m_sectdesign.RemoveKey(Key);}
	void DelAllDesign()
		{m_sectdesign.RemoveAll();}
	BOOL GetDesign(T_UDRD_KEY Key,T_SECT_UDRD_D& rData)
		{return m_sectdesign.Lookup(Key,rData);}
	POSITION GetStartDesign()
		{return m_sectdesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SECT_UDRD_D& rData)
		{m_sectdesign.GetNextAssoc(rNextPosition,rKey,rData);}
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SECT_UDRD_D,T_SECT_UDRD_D&>m_sect;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SECT_UDRD_D,T_SECT_UDRD_D&>m_sectdesign;
};

#endif