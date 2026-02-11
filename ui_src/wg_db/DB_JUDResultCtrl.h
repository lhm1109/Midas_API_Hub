// DB_JUDResultCtrl.h: interface for the CDB_JUDResultCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DB_JUDRESULTCTRL_H__)
#define __DB_JUDRESULTCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DB_ST_DT.h"

#define D_FRAME_X_AXIS  1
#define D_FRAME_Y_AXIS  2

#include "HeaderPre.h"
class CDBDoc;
class __MY_EXT_CLASS__ CDB_JUDResultCtrl
{
public:	
	CDB_JUDResultCtrl(CWnd* pParent=NULL);
	virtual ~CDB_JUDResultCtrl();

public:
	BOOL MakeElemList();
	BOOL MakeLcomList();
	
	int  GetFrameCount();  
	int  GetFrameKeyList(CArray<UINT,UINT>& aFrameKey);
	int  GetFrameAxisList(CArray<UINT2,UINT2>& aFrameKey, int nAxis);
	BOOL GetFrameName(UINT FrameK, CString& strName);
	BOOL GetFrameItem(UINT2 FrameK, CString& strItem, int nAxis);
	BOOL GetFrameElemList(UINT2 FrameK, CArray<T_ELEM_K,T_ELEM_K>& aElemK);  
	
	int  GetStorKeyList(CArray<UINT,UINT>& aStoryKey);
	BOOL GetStorItem(UINT StorK, CString& strItem);
	BOOL GetStoryElemList(UINT StorK , CArray<T_ELEM_K,T_ELEM_K>& aElemK);    

	int  GetElemKeyList(int nMode, CArray<UINT,UINT>& aElemK);
	 int GetNodeKeyList(int nMode, CArray<UINT,UINT>& aNodeK);
	int  GetLcomKeyList(CArray<UINT,UINT>& aLcomKey, int nType);
	BOOL GetLcomItem(UINT LcomK, CString& strItem, int nType);   

protected:
	CDBDoc* m_pDoc;

	// Frame & Story    
	CMapEx<UINT,UINT,CString,LPCSTR>   m_mapFrameName  ;   
	CMapEx<UINT2,UINT2,CString,LPCSTR> m_mapFrameItem_X;   
	CMapEx<UINT2,UINT2,CString,LPCSTR> m_mapFrameItem_Y;   
	CMapEx<UINT,UINT,CString,LPCTSTR>  m_mapStoryItem  ;   

	// Elem   
	CMapEx<UINT2,UINT2,CArray<T_ELEM_K,T_ELEM_K>*,CArray<T_ELEM_K,T_ELEM_K>*> m_mapElemKList_Frame;
	CMapEx<UINT,UINT,CArray<T_ELEM_K,T_ELEM_K>*,CArray<T_ELEM_K,T_ELEM_K>*>   m_mapElemKList_Story;      
	void DeleteElemList();
	BOOL MakeFrameElemList();  
	BOOL MakeStoryElemList();  	

	// Lcom
	CMapEx<UINT,UINT,CString,LPCSTR> m_mapLcom_Anal;
	CMapEx<UINT,UINT,CString,LPCSTR> m_mapLcom_STL ;
	CMapEx<UINT,UINT,CString,LPCSTR> m_mapLcom_RC  ;
	CMapEx<UINT,UINT,CString,LPCSTR> m_mapLcom_DS  ;
	CMapEx<UINT,UINT,CString,LPCSTR> m_mapLcom_Hor ; 
	BOOL MakeLcomList_Anal();
	BOOL MakeLcomList_Sect();
	BOOL MakeLcomList_DS();
	BOOL MakeLcomList_Hor();    
};
#include "HeaderPost.h"
#endif // !defined(AFX__DB_JUDRESULTCTRL_H__)
