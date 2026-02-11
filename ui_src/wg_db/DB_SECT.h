#ifndef __DB_SECT_DB_H__
#define __DB_SECT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;

class CDB_SECT
{
public:
	CDB_SECT();
	CDB_SECT(CDB_SECT& src);
	CDB_SECT& operator=(CDB_SECT& src);
	virtual ~CDB_SECT();

public:
	T_SECT_K m_nLastNum;
	T_SECT_K m_nStartNum;

public:
	void Add(T_SECT_K Key,T_SECT_D& rData);
	BOOL Del(T_SECT_K Key);
	void AddDesign(T_SECT_K Key,T_SECT_D& rData);
	BOOL DelDesign(T_SECT_K Key);

public:
	//inline CMap<T_SECT_K, T_SECT_K, T_SECT_D, T_SECT_D&>::CPair * GetP(T_SECT_K Key)
	inline auto GetP(T_SECT_K Key)
	{
		return m_sect.PLookup(Key);
	}
	inline BOOL Get(T_SECT_K Key,T_SECT_D& rData)
		{return m_sect.Lookup(Key,rData);}
	int GetCount()
		{return (int)m_sect.GetCount();}
	POSITION GetStart()
		{return m_sect.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SECT_K& rKey,T_SECT_D& rData)
		{m_sect.GetNextAssoc(rNextPosition,rKey,rData);}

	BOOL GetDesign(T_SECT_K Key,T_SECT_D& rData)
		{return m_sectdesign.Lookup(Key,rData);}
	POSITION GetStartDesign()
		{return m_sectdesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_SECT_K& rKey,T_SECT_D& rData)
		{m_sectdesign.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_SECT_K Key);
	BOOL DelList(T_SECT_K Key);
	void DelAllList();
	BOOL GetList(T_SECT_K Key,SectList*& rpList){return m_sectlist.Lookup(Key,rpList);}
	int GetCountList(){return (int)m_sectlist.GetCount();}
	void AddListItem(T_SECT_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SECT_K Key, int nCmd, int nKey);

protected:
	CMap<T_SECT_K,T_SECT_K,T_SECT_D,T_SECT_D&>m_sect;
	CMap<T_SECT_K,T_SECT_K,T_SECT_D,T_SECT_D&>m_sectdesign;
	CMap<T_SECT_K,T_SECT_K,SectList*,SectList*>m_sectlist;
public:
	CMap<T_SECT_K,T_SECT_K,T_COLOR_D,T_COLOR_D&>m_sectcolor;
};

#endif
