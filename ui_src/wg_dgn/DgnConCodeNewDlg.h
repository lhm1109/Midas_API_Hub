#if !defined(__DGN_CON_CODE_NEW_DLG_H__)
#define __DGN_CON_CODE_NEW_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewDlg.h : header file
//
#include "..\MIT_frx\MComboBox.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "wg_dgnres2.h"
#include "HeaderPre.h"


class CDconChildDialog;

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewDlg dialog
class __MY_EXT_CLASS__ CDgnConCodeNewDlg : public CDialogMove
{
// Construction
public:
	CDgnConCodeNewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnConCodeNewDlg();   // standard destructor
// Dialog Data
	//{{AFX_DATA(CDgnConCodeNewDlg)
	enum { IDD = IDD_DGN_CON_CODE_NEW_DLG };
	mit::frx::MComboBox	m_DesignCode;
	
	enum EN_CHILD_DLG
	{
		EN_CHILD_KCI_ACI_1=1, EN_CHILD_KCI_ACI_2, EN_CHILD_ACI_3, EN_CHILD_NSR10_4, EN_CHILD_TWN_USD_5, EN_CHILD_IS456_6,
		EN_CHILD_AIJ_WSD_7,   EN_CHILD_GB50010_8, EN_CHILD_EURO204_9, EN_CHILD_ACI318_M_10, EN_CHILD_TWN_USD_112, EN_CHILD_ACI318_12, EN_CHILD_KDS2022_13,
		EN_CHILD_NTC_DCEC_2017, EN_CHILD_SP_63_2018, EN_CHILD_NTC_DCEC_2023,
	};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void ResizeDialog(CDconChildDialog* ChildDlg=nullptr);
    BOOL GetDataFromDlg(T_DCON_D& rData);

protected:
	int  GetChildDlgIndex(CString& strDesignCode);
protected:
	T_DCON_D m_Data;

	int m_nCurrentDlgIndex;
	CArray<UINT, UINT> m_aCtrlMethod;
	CMap <int,int,CDconChildDialog*,CDconChildDialog*> m_mapChildDlg;
	CArray<BOOL, BOOL> m_aCodeEnable;
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewDlg)
	virtual BOOL OnInitDialog();

	void InitCtrl();
	BOOL Dlg2Data();

	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeDgnCtrldataDesigncode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_DLG_H__)
