#ifndef __DB_REBR_DB_H__
#define __DB_REBR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_REBR
{
public:
	CDB_REBR();
	CDB_REBR(CDB_REBR& src);
	virtual ~CDB_REBR();
	CDB_REBR& operator=(CDB_REBR& src);
	CDBDoc* m_pDoc;

public:
	void Add(T_REBR_K Key,T_REBR_D& rData,CDB_SECT* pSect);
	BOOL Del(T_REBR_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_REBR_K Key,T_REBR_D& rData);
		//{return m_rebr.Lookup(Key,rData);}
	int GetCount();
		//{return m_rebr.GetCount();}
	POSITION GetStart();
		//{return m_rebr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_REBR_K& rKey,T_REBR_D& rData);
		//{m_rebr.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_REBR_KEY Key);
	BOOL DelList(T_REBR_KEY Key);
	void DelAllList();
	BOOL GetList(T_REBR_K Key,RebrList*& rpList){return m_rebrlist.Lookup(Key.keymap,rpList);}
	int GetCountList(){return m_rebrlist.GetCount();}
	void AddListItem(T_REBR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_REBR_K Key, int nCmd, int nKey);

protected:
	CMap<T_REBR_KEY,T_REBR_KEY,T_REBR_D,T_REBR_D&>m_rebr;
	CMap<T_REBR_KEY,T_REBR_KEY,RebrList*,RebrList*> m_rebrlist;
};

#endif

