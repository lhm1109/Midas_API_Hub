#if !defined(AFX_STLDLAPLDLG_H__INCLUDED_)
#define AFX_STLDLAPLDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldLaplDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLagr.h"
#include "..\wg_db\wg_db_selectctrl.h"
/////////////////////////////////////////////////////////////////////////////
// CStldLaplDlg dialog

class CStldLaplDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CStldLaplDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

	void SetLaplDisplay();

	enum { IDD = IDD_TM_STLD_LAPL };

	int		      m_nOption;       //0:Add       1:Replace 2:Delete
	CCobxLagr	  m_wndLagrCobx;
	CSelectEdit m_edElemList;
	CListCtrl	  m_List;

protected:
	CDBDoc* m_pDoc;

	BOOL    m_bIgnoreMsg; //¼¿ º¯ÇÒ¶§...

	//DlgData----------------------------------------
	CArray<T_ELEM_K, T_ELEM_K> m_arKeyElem;
	CArray<T_LAPL_K, T_LAPL_K> m_arKeyLapl;
	T_LAGR_K m_LagrK;
	//DlgData----------------------------------------

protected:  
	void SetListCtrlHeader();
	void MakeItemList();
	int  GetSelectedLapl(CArray<T_LAPL_K, T_LAPL_K>& arKeyLapl);
	int  MakeLapl(CArray<T_LAPL_D, T_LAPL_D&>& arLaplData, BOOL& bIsRemoved);
	int  MakeLapl4Planar(CArray<T_LAPL_D, T_LAPL_D&>& arLaplData, BOOL& bIsRemoved);
	BOOL CheckLaplData_Dupl(const T_LAPL_D& rData);
	BOOL Dlg2Data();
	BOOL SetDataInDlg();
	int  GetElementType();

	BOOL OnCmdAdd();
	BOOL OnCmdReplace();
	BOOL OnCmdDelete();
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeElemType();
	afx_msg void OnChangeAssignType();
	afx_msg void OnTmDefineLagrBtn();
	afx_msg void OnChangedItemSelection(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#endif
