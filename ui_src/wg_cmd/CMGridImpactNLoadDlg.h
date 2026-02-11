#if !defined(AFX_CMGRIDIMPACTNLOADDLG_H__5081ACB9_7B69_4EDC_918F_203DF3569903__INCLUDED_)
#define AFX_CMGRIDIMPACTNLOADDLG_H__5081ACB9_7B69_4EDC_918F_203DF3569903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridImpactNLoadDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxP1lt.h"

/////////////////////////////////////////////////////////////////////////////
// CCMGridImpactNLoadDlg dialog
#include "CMChildBarBase.h"
class CCMGridImpactNLoadDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMGridImpactNLoadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CCMGridImpactNLoadDlg)
	enum { IDD = IDD_CMD_GRID_LNKF_DLG };
	CCobxP1lt	m_comboLoadType;
	MEdit	m_wndLoadItem;
	CTextUnit	m_dSpanLengthUnit;
	CEditUnit	m_wndImpactFactor;
	CEditUnit	m_wndSpanLength;
	CListCtrl	m_GridList;
	CListBox	m_ctrlSelectLink;
	CListBox	m_ctrlList;
	int		m_nFactorType;
	int		m_nImpactFactor;
	int		m_nStructType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridImpactNLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> arImpactFactor;
	CArray<UINT, UINT> arP1LoadFactor;
	CArray<UINT, UINT> arStructType;
	void AlignCtrl();
	void SetListCtrlHeader();
	void InitLinkList();
	void MakeItemEx();
	void SetItem(int nIndex, T_LNKF_K key, T_LNKF_D &data);
	CString DataToStr(int i, T_LNKF_K key, T_LNKF_D &data);
	BOOL GetSelectedData(int &nIndex);
	BOOL ValidData(T_LNKF_D& data);
	BOOL Dlg2Data(T_LNKF_D& data, int linkType, UINT key);
	void Data2Dlg(T_LNKF_D& data);
	void UnselectLink();
	void SelectLink();

	// Generated message map functions
	//{{AFX_MSG(CCMGridImpactNLoadDlg)
	virtual BOOL OnInitDialog();
	virtual void OnTmClose();
	afx_msg void OnTmConsImpact();
	afx_msg void OnTmImpactRdo();
	afx_msg void OnCmdMoreButton();
	afx_msg void OnSelchangeLoadtypeCombo();
	afx_msg void OnCmdGridLeft();
	afx_msg void OnCmdGridRight();
	afx_msg void OnTmExecute();
	afx_msg void OnTmDelete();
	afx_msg void OnItemChangedGridList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCmdLink();
	afx_msg void OnDblclkCmdSelLink();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGRIDIMPACTNLOADDLG_H__5081ACB9_7B69_4EDC_918F_203DF3569903__INCLUDED_)
