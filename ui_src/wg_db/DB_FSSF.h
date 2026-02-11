#ifndef __DB_FSSF_DB_H__
#define __DB_FSSF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_FSSF
{
public:
	CDB_FSSF();
	virtual ~CDB_FSSF();
	CDBDoc* m_pDoc;

public:
	void Add(T_FSSF_K Key,T_FSSF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_FSSF_K Key,CDB_ELEM* pElem);
	void Initialize();

public:
	BOOL Get(T_FSSF_K Key,T_FSSF_D& rData);
		//{return m_fssf.Lookup(Key,rData);}
	int GetCount();
		//{return m_fssf.GetCount();}
	POSITION GetStart();
		//{return m_fssf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FSSF_K& rKey,T_FSSF_D& rData);
		//{m_fssf.GetNextAssoc(rNextPosition,rKey,rData);}
	void AddList(T_FSSF_K Key);
	BOOL DelList(T_FSSF_K Key);
	void DelAllList();
	BOOL GetList(T_FSSF_K Key,FssfList*& rpList){return m_fssflist.Lookup(Key,rpList);}
	void AddListItem(T_FSSF_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_FSSF_K Key, int nCmd, int nKey);
protected:
	CMap<T_FSSF_K,T_FSSF_K,FssfList*,FssfList*>m_fssflist;
	CMap<T_FSSF_K,T_FSSF_K,T_FSSF_D,T_FSSF_D&>m_fssf;
};

#endif
