#ifndef __DB_FGRP_DB_H__
#define __DB_FGRP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NODE;
class CDB_ELEM;

class CDB_FGRP
{
public:
	CDB_FGRP();
	virtual ~CDB_FGRP();
	
public:
	T_FGRP_K m_nStartNum;
	T_FGRP_K m_nLastNum;
	
public:
	void Add(T_FGRP_K Key,T_FGRP_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_FGRP_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);
	
public:
	BOOL Get(T_FGRP_K Key,T_FGRP_D& rData)
	{return m_fgrp.Lookup(Key,rData);}
	int GetCount()
	{return m_fgrp.GetCount();}
	POSITION GetStart()
	{return m_fgrp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FGRP_K& rKey,T_FGRP_D& rData)
	{m_fgrp.GetNextAssoc(rNextPosition,rKey,rData);}
	
	void AddList(T_FGRP_K Key);
	BOOL DelList(T_FGRP_K Key);
	void DelAllList();
	BOOL GetList(T_FGRP_K Key,FgrpList*& rpList){return m_fgrplist.Lookup(Key,rpList);}
	int GetCountList(){return m_fgrplist.GetCount();}
	void AddListItem(T_FGRP_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_FGRP_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_FGRP_K,T_FGRP_K,T_FGRP_D,T_FGRP_D&>m_fgrp;
	CMap<T_FGRP_K,T_FGRP_K,FgrpList*,FgrpList*>m_fgrplist;
};

#endif