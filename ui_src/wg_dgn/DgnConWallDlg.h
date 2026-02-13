#if !defined(AFX_DGNCONWALLDLG_H__EFAC9C01_4038_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONWALLDLG_H__EFAC9C01_4038_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConWallDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_baseMouseEdit2.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConWallDlg dialog

class CDgnConWallDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConWallDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {  }

// Dialog Data
	//{{AFX_DATA(CDgnConWallDlg)
	enum { IDD = IDD_DGN_CON_WALL_DLG };
	CEditUnit	m_VRebarSpace;
	CTextUnit	m_VRebarUnit;
	CEditUnit	m_HRebarSpace;
	CComboBox	m_HRebarSize;
	CTextUnit	m_HRebarUnit;
	CListCtrl m_WallCheckList;
	CMouseEdit	m_IdList;
	CComboBox	m_EndRebarSize;
	CSpinButtonCtrl	m_EndSpin;
	CComboBox	m_VRebarSize;
	CEditUnit m_ESpace;
	CTextUnit m_ESpaceUnit;
	CEditUnit	m_wT;
	CEditUnit	m_wDw;
	CEditUnit	m_wDe;
	CTextUnit	m_wDeUnit;
	CTextUnit	m_wDwUnit;
	CTextUnit	m_wTUnit;
	CComboBox	m_wEstory;
	CComboBox	m_wSstory;
	BOOL	m_bEndCheck;
	BOOL	m_bUseMt;
	int		m_EndRebarNo;
	CString	m_strWallMark;
	//}}AFX_DATA

public:
	BOOL m_bOption,m_bWallMarkFocus;
	int m_SpinNo,m_ENo;
	CString m_WallIDs,m_wSstoryName,m_wEstoryName,m_VSize,m_ESize,m_HSize;
	double m_Thickness,m_De,m_Dw,m_VSpace,m_HSpace,m_EndSpace;

public:
	void UnSelected_ListBoxItem();
	void Check_EndRebarData();
	void Initial_SelectionData();
	void Initial_SstoryCombo();
	void Initial_EstoryCombo();
	void Initial_Units();
	void Initial_MouseEdit();
	void Initial_Data();
	void Show_EndRebarData();
	void Show_UseModelThickness();
	void Search_SelStoryKey(CString strSName,CString strEName,CArray<int,int>& KeyList);
	void Initial_ListCtrlBox();
	void Write_ListCtrlBox(int Index,CString strID,CString strStory);
	void Write_AllListCtrl();
	void Select_ListCtrlItem(int nItem);
	void Show_WallTypeBitmap();

	BOOL AddErrorCheck();
	BOOL DelErrorCheck();
	BOOL CheckSelectOption();
	BOOL CheckEQWallMark(CString MarkName);
	BOOL CheckEQWallID(CString strID,CString strIDs);

	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);

	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();

	// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
	void Enable_EndBarEdit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConWallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConWallDlg)
	afx_msg void OnDgnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDgnRcWsstory();
	afx_msg void OnDgnEndCheck();
	afx_msg void OnDgnRcUsemt();
	afx_msg void OnDeltaposDgnWallEndspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDgnWallcheckList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnWallAdd();
	afx_msg void OnDgnWallDelete();
	afx_msg void OnDgnWallModify();
	afx_msg void OnSetfocusDgnRcWsstory();
	afx_msg void OnSetfocusDgnRcWestory();
	afx_msg void OnSetfocusDgnRcWallid();
	afx_msg void OnDgnFabric();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONWALLDLG_H__EFAC9C01_4038_11D4_888F_0000C0F30D4D__INCLUDED_)
