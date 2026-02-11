#if !defined(__CMMVHLITEMSTDSOUTHAFRICADLG_H__)
#define __CMMVHLITEMSTDSOUTHAFRICADLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdSouthAfricaDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "CMMvhlItemSouthAfricaLoadGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdSouthAfricaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdSouthAfricaDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdSouthAfricaDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdSouthAfricaDlg();

//----------------------------------------------------------------------
// Implementation
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:
	void InitUnit();
	void InitCombo();
	void InitControls();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();

	void ChangeBitmap(int nBitmap);

	void Data2Dlg();
	BOOL Dlg2Data();  
	BOOL ApplyOrOK();

	void  GetIndex(CString &csName, int *sitem=NULL);  

protected:
	
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;

	CArray<UINT, UINT> m_aCtrl_NA;
	CArray<UINT, UINT> m_aCtrl_NB;
	CArray<UINT, UINT> m_aCtrl_NC;
	CArray<UINT, UINT> m_aCtrl_OKCancel;

	CCMMvhlItemSouthAfricaLoadGrid m_NCGrid[3];

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdSouthAfricaDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_SOUTH_AFRICA };

	MComboBox	m_cmbCodeName;
	CEditUnit m_edtVehicleName;
	MComboBox	m_cmbVehicleType;
	BOOL      m_bIncreLength;
	CEditUnit m_edtIncreLength;
	CTextUnit m_untIncreLength;
	CEditUnit m_edtPb;
	CTextUnit m_untPb;
	MComboBox m_cmbNumUnit;
	CEditUnit m_edtd;
	CTextUnit m_untd;
	CEditUnit m_edtD1;
	CTextUnit m_untD1;
	CEditUnit m_edtD2;
	CTextUnit m_untD2;
	CEditUnit m_edtD3;
	CTextUnit m_untD3;
	CEditUnit m_edtD4;
	CTextUnit m_untD4;
	CEditUnit m_edtD5;
	CTextUnit m_untD5;
	CEditUnit m_edtD6;
	CTextUnit m_untD6;
	CEditUnit m_edtW;
	CTextUnit m_untW;
	BOOL      m_bOpposite;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdSouthAfricaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdSouthAfricaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdMvhlIncreLengthChk();
	afx_msg void OnCmdMvhlApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMSTDSOUTHAFRICADLG_H__)
