#ifndef __DB_DCBW_DB_H__
#define __DB_DCBW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STOR;

class CDB_DCBW
{
public:
	CDB_DCBW();
	virtual ~CDB_DCBW();
	CDBDoc* m_pDoc;

public:
	void Add(T_DCBW_K Key,T_DCBW_D& rData,CDB_STOR* pStor);
	BOOL Del(T_DCBW_K Key,CDB_STOR* pStor);

public:
	BOOL Get(T_DCBW_K Key,T_DCBW_D& rData);
		//{return m_dcbw.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_dcbw.GetCount();}
	POSITION GetStart();
		//{return m_dcbw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DCBW_K& rKey,T_DCBW_D& rData);
		//{m_dcbw.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_DCBW_KEY,T_DCBW_KEY,T_DCBW_D,T_DCBW_D&>m_dcbw;
};

#endif
