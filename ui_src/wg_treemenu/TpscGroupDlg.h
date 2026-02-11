#if !defined(__AFX_TPSCGROUPDLG_H__)
#define __AFX_TPSCGROUPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpscGroupDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
#include "..\wg_db\EditUnit.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CTpscGroupDlg dialog

class CTpscGroupDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CTpscGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {};
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CTpscGroupDlg)
	enum { IDD = IDD_TM_TPSC_GROUP };
	CTextUnit	m_ZAxisUnit;
	CTextUnit	m_YAxisUnit;
	MEdit	m_edtGroupName;
	CSelectEdit	m_edtElemList;
	CListCtrl	m_List;
	int		m_nYAxisSymPln;
	int		m_nZAxisSymPln;
	int		m_nYAxisVariType;
	int		m_nZAxisVariType;
	double	m_dYAxisDist;
	double	m_dZAxisDist;
	CEditUnit m_edtYQuad;
	CEditUnit m_edtZQuad;
	MillustViewer m_wndTpscPicture;
	//}}AFX_DATA


// Overrides
public:
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpscGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	
	T_TSGR_K m_TsgrKey;	
	T_TSGR_D m_TsgrData;
	T_TSGR_DT m_TsgrNodeData;
	T_TSGR_UNIT m_TsgrUnit;

	void InitListTitle();
	void InitTsgrData();
	

	int GetCurrentRow();
	int GetCurrentRows(CArray<int,int>& aSelectedRows);

	void UpdateBuffer();
	int GetCurrentTsgrKey();
	CString GetCurrentTsgrKeyName();
	void ZAxisVarCtrlMan();
	void YAxisVarCtrlMan();

	// Generated message map functions
	//{{AFX_MSG(CTpscGroupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmTpscGroupAdd();
	afx_msg void OnTmTpscGroupModify();
	afx_msg void OnTmTpscGroupDelete();
	afx_msg void OnItemchangedTmTpscGroupList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeTmTpscGroupLoadlistEdit();
	afx_msg void OnDestroy();
	afx_msg void OnTmTpscYaxisVarType();
	afx_msg void OnTmTpscZaxisVarType();
	afx_msg void OnTmTpscGroupConvert();
	afx_msg void OnDeltaposCmdYquadSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdZquadSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTLGROUPDLG_H__93312C02_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_)
