#if !defined(__CM_LOAD_DIRECTION_GROUP_DLG_H__)
#define      __CM_LOAD_DIRECTION_GROUP_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadDirectionGroupDlg.h : header file
//
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MCombobox.h"

#include "CMDlgBase.h"
#include "CMLoadDirectionGroupGrid.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadDirectionGroupDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLoadDirectionGroupDlg : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CCMLoadDirectionGroupDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CCMLoadDirectionGroupDlg)
	enum { IDD = IDD_CMD_SEIS_EVAL_LOAD_DIRECT_GRUP_DLG };

	//}}AFX_DATA

	CSelectLC	m_ComboLoadCase;
	mit::frx::MComboBox m_ComboAngle;
	mit::frx::MComboBox m_ComboSystem;
	mit::frx::MEdit     m_EditUserGroup;
	CSelectEdit m_EditElemList;

	BOOL Data2Dlg(T_EGLD_D& EgldD);
	BOOL Dlg2Data(T_EGLD_D& EgldD);

	void OnChangeCurrentEGLD(T_EGLD_K nKey);
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadDirectionGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CCMLoadDirectionGroupGrid m_wndGrid;

	void InitGrid();
	void SetGridTitle();
	void SetGridValue();
	void UpdateBuffer();
	BOOL GetEgldData(T_EGLD_D& data);
	void InitCtrlSetting();

	// Generated message map functions
	//{{AFX_MSG(CCMLoadDirectionGroupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdGrupAdd();
	afx_msg void OnCmdGrupClose();
	afx_msg void OnCmdGrupDelete();
	afx_msg void OnCmdGrupReplace();
	afx_msg void OnSelchangeSystemType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CM_LOAD_DIRECTION_GROUP_DLG_H__)
