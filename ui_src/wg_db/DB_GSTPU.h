#ifndef __DB_GSTPU_DB_H__
#define __DB_GSTPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GSTPU
{
public:
	CDB_GSTPU()
	{
		m_gstp.InitHashTable(HASHSIZEGSTP);
	}
	virtual ~CDB_GSTPU(){};

public:
	void Add(T_UDRD_KEY Key,T_GSTP_UDRD_D& rData)
		{m_gstp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gstp.RemoveKey(Key);}
	void DelAll()
		{m_gstp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GSTP_UDRD_D& rData)
		{return m_gstp.Lookup(Key,rData);}
	int GetCount()
		{return m_gstp.GetCount();}
	POSITION GetStart()
		{return m_gstp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GSTP_UDRD_D& rData)
		{m_gstp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GSTP_UDRD_D,T_GSTP_UDRD_D&>m_gstp;
};

#endif