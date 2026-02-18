#if !defined(AFX_DgnRCRatingPlateRebarDlg_H__INCLUDED_)
#define AFX_DgnRCRatingPlateRebarDlg_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCRatingPlateRebarDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "DgnBeamGridWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateRebarDlg dialog
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"

class CDgnBeamGridWndCIVIL;

class __MY_EXT_CLASS__ CDgnRCRatingPlateRebarDlg : public CDialogMove
{
	// Construction
public:
	CDgnRCRatingPlateRebarDlg(CWnd* pParent = NULL);   
	~CDgnRCRatingPlateRebarDlg();  

// Dialog Data
	//{{AFX_DATA(CDgnRCRatingPlateRebarDlg)
	enum { IDD = IDD_DGN_RATING_RC_PLATE_REBAR_DLG};

	MComboBox m_cmbTopLayer;
	CEditUnit	m_edtAsTop;
	CTextUnit	m_untAsTop;
	MComboBox m_cmbBotLayer;
	CEditUnit	m_edtAsBot;
	CTextUnit	m_untAsBot;
	BOOL      m_bSRebar;
	MComboBox	m_cmbSSize;
	CEditUnit	m_edtSSpace;
	CTextUnit	m_untSSpace;
	CEditUnit	m_edtSNum;
	//}}AFX_DATA


// Overrides
	//{{AFX_VIRTUAL(CDgnRCRatingPlateRebarDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetData(const int iThik, T_RRPL_SUB_D& rData);
	void GetData(T_RRPL_SUB_D& rData);

protected:
	CDgnBeamGridWndCIVIL*  m_TopGrid;
	CDgnBeamGridWndCIVIL*  m_BotGrid;

	CDBDoc* m_pDoc;
	T_RRPL_SUB_D m_Data; // I만 사용, Layer 최대갯수 3개
	int m_iThik;
	
	void InitCombo();
	void InitGrid();
	void InitUnit();
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetDataToGrid();
	void GetDataFromGrid();
	void ControlsEnableDisable();
	BOOL CheckRebarData(T_RRPL_SUB_D & CPBE);
	BOOL CheckRebarData_MainRebar01(T_RRPL_SUB_D &rData);
	BOOL CheckRebarData_MainRebar02(T_RRPL_SUB_D &rData);
	BOOL CheckPosiInputData_New(T_RRPL_SUB_D &rData);
	BOOL CheckRebarData_Stirrup(T_RRPL_SUB_D &rData);

	void CalcAs(T_RRPL_SUB_D& RchkBeamD, double& dAsTop, double& dAsBot);
	
	// Generated message map functions
	//{{AFX_MSG(CDgnRCRatingPlateRebarDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeDgnRcTopLayerCmb();
	afx_msg void OnSelchangeDgnRcBotLayerCmb();
	afx_msg void OnClickedShearChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnRCRatingPlateRebarDlg_H__INCLUDED_)
