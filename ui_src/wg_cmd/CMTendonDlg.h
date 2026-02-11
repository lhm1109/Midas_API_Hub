#if !defined(__CMTENDONDLG_H__)
#define      __CMTENDONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTendonDlg.h : header file
//
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CMDlgBase.h"
#include "MyListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTendonDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMTendonDlg : public CCMDlgBase
{
protected:
	BOOL m_bShowProfDlg;
	// Construction
public:
	BOOL IsShowProfDlg();
	void SetProfDlgFlag(BOOL bShowProfDlg);

	CMap<T_TDNA_K, T_TDNA_K,T_TDNA_K,T_TDNA_K> m_CurSelTdnaK;
	CCMTendonDlg(CWnd* pParent = NULL);   // standard constructor
	void MakeListHeader();
	void UpdateTdnaList();
	void OnChangeCurrentElem(int nKey);


	void    DisplaySelectedTendon();
	void GetSelectedTendon(CArray<T_TDNA_K,T_TDNA_K>& arSelected);
	void    DisplayCurrentTendon(T_TDNA_K TendonKey);
	LRESULT OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnListCtrlLButtonUp(WPARAM wParam, LPARAM lParam);
	void SaveCurSelectedStagKeys();
	void ReselectCurStageKeys();
	
	
// Dialog Data
	//{{AFX_DATA(CCMTendonDlg)
	enum { IDD = IDD_CMD_DEF_TENDON_PROP_DLG };
	CMyListCtrl	m_wndTendonList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTendonDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	void ShowItemDlg(T_TDNA_K key);

	// Generated message map functions
	//{{AFX_MSG(CCMTendonDlg)
	afx_msg void OnCmdClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdModify();
	afx_msg void OnCmdCopy();
	afx_msg void OnDblclkCmdTendonList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdTendonList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCmdTendonList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCmdChangeProperty();
	afx_msg void OnCmdExportToDXF();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Size Á¶Àý
protected:
	void SaveWindowSizeData();
	void AlignControl(const CRect& rect);
	void AlignControlForCivil();

	CRect m_MinimumRect;
	int m_nDlgWidth;
	int m_nDlgHeight;
	CArray<UINT, UINT> m_aControls;
	CArray<float, float> m_fHRatio;
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTENDONDLG_H__17CEA006_533C_41EB_B796_2FCE17611292__INCLUDED_)
