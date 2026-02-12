#if !defined(AFX_CMLLANEJPDLG_H__21FC4334_33F7_4A24_8AA2_A79863D65B25__INCLUDED_)
#define AFX_CMLLANEJPDLG_H__21FC4334_33F7_4A24_8AA2_A79863D65B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLLaneJPDlg.h : header file
//
#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_base\wg_base_NotifyListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLLaneJPDlg dialog
#include "HeaderPre.h"
class CCMLLaneJPInfoDlg;
class __MY_EXT_CLASS__ CCMLLaneJPDlg : public CCMDlgBase
{
// Construction
	// Construction
	T_LLANjp_D_OLD m_LLANjpD;
	int m_nOP; // (0) Add (1) Modify (2) Else
public:
	CCMLLaneJPInfoDlg*  m_pInfoDlg;
	CCMLLaneJPDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL SetByInfoDlg(T_LLANjp_BASE_OLD* pLLANDBase);
	void Dlg2Data();
	void Data2Dlg();
	BOOL AddOp(T_LLANjp_BASE_OLD* pLlanBaseD);
	void DelOp();
	BOOL ModOp(T_LLANjp_BASE_OLD* pLlanBaseD);
	void CopyOp();
	void MakeInfoList();
	BOOL IsExistInfoName(CString& InfoName,CString ToBeExcludeName);
	void UpdateUnit();
	void ShowInfoData();
	//BOOL IsRelevantCode(int nCode);
	//void ShowHideByCode();

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CCMLLaneJPDlg)
	enum { IDD = IDD_CMD_ML_LLAN_JAPAN_OLD };
	CTextUnit	m_wndGLoadWUnit;
	CTextUnit	m_wndMLoadWUnit;
	CTextUnit	m_wndLLoadWUnit;
	CEditUnit	m_wndMLoadW    ;
	CEditUnit	m_wndLLoadW    ;
	CEditUnit	m_wndGLoadW    ;
	CNotifyListCtrl	m_List         ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLLaneJPDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void SetHeaderTitle();
	void ButtonCtrl();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMLLaneJPDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRemoveButton();
	afx_msg void OnModButton();
	afx_msg void OnDelButton();
	afx_msg void OnCopyButton();
	afx_msg void OnAddButton();
	afx_msg void OnUpdateRemove(CCmdUI*pCmdUI);
	afx_msg void OnUpdateAdd(CCmdUI*pCmdUI);
	afx_msg void OnUpdateDel(CCmdUI*pCmdUI);
	afx_msg void OnUpdateMod(CCmdUI*pCmdUI);
	afx_msg void OnUpdateCopy(CCmdUI*pCmdUI);
	afx_msg void OnUpdateOk(CCmdUI*pCmdUI);
	afx_msg void OnUpdateLLaneList(CCmdUI*pCmdUI);
	afx_msg void OnUpdateRemoveBtn(CCmdUI*pCmdUI);
	afx_msg void OnClickLlaneList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLlaneList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedLlaneList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturnLlaneList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg LRESULT OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLLANEJPDLG_H__21FC4334_33F7_4A24_8AA2_A79863D65B25__INCLUDED_)
