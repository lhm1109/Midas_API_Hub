#if !defined(__DGNCONWALLNEWDLG_H__)
#define __DGNCONWALLNEWDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConWallNewDlg.h : header file
//

#include "..\MIT_frx\MillustViewer.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnDlgBase.h"
#include "DgnDrawWnd.h"
#include "DgnConWallRebarGrid.h"
#include "wg_dgnRes2.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallNewDlg dialog

class __MY_EXT_CLASS__ CDgnConWallNewDlg : public CDgnDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CDgnConWallNewDlg(CWnd* pParent = NULL);
	virtual ~CDgnConWallNewDlg();

// Dialog Data
	//{{AFX_DATA(CDgnConWallNewDlg)
	enum { IDD = IDD_DGN_CON_WALL_DLG_NEW };
	CListCtrl m_WallIDList;
	mit::frx::MButton m_chkSubWallID;
	mit::frx::MEdit m_edtWallID;
	mit::frx::MEdit m_edtSubWallID;

	mit::frx::MComboBox m_cmbStartStory;
	mit::frx::MComboBox m_cmbEndStory;

	CEditUnit m_edtDw;
	CEditUnit m_edtDe;
	CTextUnit m_untDwDe;

	mit::frx::MButton m_chkUseModelThik;
	CEditUnit m_edtUseModelThik;
	CTextUnit m_untUseModelThik;

	mit::frx::MButton m_chkUseWeldedFabfic;

	CEditUnit m_edtBELength;
	CTextUnit m_untBELength;

	mit::frx::MillustViewer m_wndWallLayer1;
	mit::frx::MillustViewer m_wndWallLayer2;
	//}}AFX_DATA

protected:
	T_REBW_D m_RebwD;
	BOOL m_bInit;

	CDgnConWallRebarGrid m_RebarGrid;
	CMap<int,int,T_REBW_KEY,T_REBW_KEY> m_mIndex2RebwK;
	CMap<T_REBW_KEY,T_REBW_KEY,int,int> m_mRebwK2Index;

public:
	void ShowSelectedRebw(T_REBW_KEY RebwKey, BOOL bInit/*=TRUE*/);
	void ShowBitmap(BOOL bEndCheck);
	
protected:  
	double m_nD0,m_Hc,m_Bc,m_Hf,m_Bf;
	int m_nNqrb,m_nNrow;
	CString m_strShape;
	
protected:    
	void InitCtrls();
	void AlignCtrls();
	void EnableDisableCtrls();
	
	void InitStartStoryCombo();
	void InitEndStoryCombo(CString* strEndStory = NULL);

	void Data2Dlg();
	BOOL Dlg2Data(T_REBW_D* pRebwD);
	
	void SetListCtrlHeader();
	BOOL GetSelectedItem(T_REBW_KEY& RebwKey);
	int GetSelectedItems(CArray<T_REBW_KEY,T_REBW_KEY>& aRebwKey);
	void MakeItemEx();
	void SetItem(int nIndex, CString& strWallIDName, CString& strWallMark,
							 CString& strSttStory, CString& strEndStory, CString& strRebar);
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConWallNewDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConWallNewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkSubWallID();
	afx_msg void OnSelChangeStartStoryCmb();
	afx_msg void OnChkUseModelThickness();
	afx_msg void OnChkUseWeldedFabric();
	afx_msg void OnBtnUseWeldedFabric();
	afx_msg void OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnAddReplace();
	afx_msg void OnDgnDelete();
	afx_msg void OnDgnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONWALLNEWDLG_H__)
