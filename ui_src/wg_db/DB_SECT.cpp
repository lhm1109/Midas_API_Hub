#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SECT.h"
#include "DBDoc.h"
#include "DispOptColorObj.h"
#include "DispOptMgr.h"

#include "..\wg_base\RandomColorUtil.h"   // for CRandomColorUtil

CDB_SECT::CDB_SECT()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sect.InitHashTable(HASHSIZESECT);
	m_sectdesign.InitHashTable(HASHSIZESECT);
	m_sectlist.InitHashTable(HASHSIZELISTSECT);
	m_sectcolor.InitHashTable(HASHSIZELISTSECT);
}

CDB_SECT::~CDB_SECT()
{
	DelAllList();
}

CDB_SECT::CDB_SECT(CDB_SECT& src)
{
	*this = src;
}

CDB_SECT& CDB_SECT::operator= (CDB_SECT& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_SECT_K KeySect;
	T_SECT_D DataSect;
	T_COLOR_D Color;
	m_sect.RemoveAll();
	pos=src.m_sect.GetStartPosition();
	while(pos != NULL)
	{
		src.m_sect.GetNextAssoc(pos,KeySect,DataSect);
		m_sect.SetAt(KeySect, DataSect);
	}
	pos=src.m_sectdesign.GetStartPosition();
	while(pos != NULL)
	{
		src.m_sectdesign.GetNextAssoc(pos,KeySect,DataSect);
		m_sectdesign.SetAt(KeySect, DataSect);
	}
	//CMap<T_SECT_K,T_SECT_K,T_COLOR_D,T_COLOR_D&>m_sectcolor;
	pos=src.m_sectcolor.GetStartPosition();
	while(pos != NULL)
	{
		src.m_sectcolor.GetNextAssoc(pos,KeySect,Color);
		m_sectcolor.SetAt(KeySect, Color);
	}

	DelAllList();
	SectList* pList;
	pos=src.m_sectlist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_sectlist.GetNextAssoc(pos,KeySect,pList);
		SectList* pListNew = new SectList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_sectlist.SetAt(KeySect, pListNew);
	}

	return *this;
}

void CDB_SECT::Add(T_SECT_K Key,T_SECT_D& rData)
{
	T_SECT_D data;
	if(!m_sect.Lookup(Key, data))
	{
		T_COLOR_D DataColor;
		DataColor.Initialize();
		
		CRandomColorUtil randomColorUtil;
		
		if (CDispOptMgr::GetFlagAssignRandomColor())
		{
			//       DataColor.WireFrame  = randomColorUtil.GetRandomColor();
			//       DataColor.HiddenFill = randomColorUtil.GetRandomColor();
			//       DataColor.HiddenEdge = randomColorUtil.GetRandomColor();
			randomColorUtil.GetRandomColor(DataColor.HiddenFill, DataColor.HiddenEdge, DataColor.WireFrame);
		}
		else
		{
			COptColorObjNames COCN;
			/*DataColor.WireFrame = CDBDoc::GetDocPoint()->m_pDispOpt->GetColor(COCN.m_ElemGlobalWire)->m_DefVal;
			DataColor.HiddenFill = CDBDoc::GetDocPoint()->m_pDispOpt->GetColor(COCN.m_ElemGlobalHidd)->m_DefVal;
			DataColor.HiddenEdge = CDBDoc::GetDocPoint()->m_pDispOpt->GetColor(COCN.m_ElemGlobalHideEdge)->m_DefVal;*/

			DataColor.WireFrame = RGB(111, 142, 91);
			DataColor.HiddenFill = RGB(159, 205, 131);
			DataColor.HiddenEdge = RGB(111, 142, 91);
		}
		m_sectcolor.SetAt(Key, DataColor);
	}
	m_sect.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SECT::Del(T_SECT_K Key)
{
	BOOL ret=m_sect.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(m_sectcolor.RemoveKey(Key));
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SECT_K key;
			T_SECT_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
	return ret;
}

void CDB_SECT::AddDesign(T_SECT_K Key,T_SECT_D& rData)
{
	T_SECT_D data;
	m_sectdesign.SetAt(Key,rData);
}

BOOL CDB_SECT::DelDesign(T_SECT_K Key)
{
	return m_sectdesign.RemoveKey(Key);
}

void CDB_SECT::AddList(T_SECT_K Key)
{
	SectList* pList = new SectList;
	m_sectlist.SetAt(Key, pList);
}

BOOL CDB_SECT::DelList(T_SECT_K Key)
{
	SectList* pList;
	BOOL bExist=m_sectlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_sectlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_SECT::DelAllList()
{
	T_SECT_K Key;
	SectList* pList;
	POSITION pos=m_sectlist.GetStartPosition();
	while(pos != NULL)
	{
		m_sectlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_sectlist.RemoveAll();
}

void CDB_SECT::AddListItem(T_SECT_K Key, int nCmd, int nKey)
{
	SectList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_SECT::DelListItem(T_SECT_K Key, int nCmd, int nKey)
{
	SectList* pList;
	if(!GetList(Key, pList))return TRUE;

	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}
