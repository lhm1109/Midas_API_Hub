#if !defined(AFX_DGNRATINGPARAMDLG_H__)
#define AFX_DGNRATINGPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlRatingParamDlg.h : header file
//
#include "../wg_base/DialogMove.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/EditUnit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "HeaderPre.h"
 
class CDBDoc;

class __MY_EXT_CLASS__ CDgnStlRatingParamDlg : public CDialogMove
{
	// Construction
public:
	CDgnStlRatingParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnStlRatingParamDlg();
	
	// Dialog Data
	//{{AFX_DATA(CDgnStlRatingParamDlg)
	enum { IDD = IDD_DGN_STL_RATING_PARAM };	
	
	//}}AFX_DATA
	
	CArray<UINT,UINT> m_aLSDCtrl;
	CArray<UINT,UINT> m_aOutputCtrl;
	//CArray<UINT,UINT> m_aOutputCtrlForce;
	CArray<UINT,UINT> m_aOutputCtrlDetail;
	CArray<UINT,UINT> m_aOutputCtrlASD;
	
	MButton			m_chkUserInput;
	
	CArray<UINT, UINT> m_aSurveyMethodRdo;

	CEditUnit		m_EdtLiveLoadCriLeft;
	CEditUnit		m_EdtLiveLoadCriRight;

	MButton			m_chkTotalSectionForce;
	MButton			m_chkAllowStressCalc;
	MButton			m_chkSectionInputInfo;

	MButton			m_chkDetailReport;

	CString			m_strLiveLoadType;

protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlRatingParamDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnClickModDgnParam();
	afx_msg void OnCheckUserInputData();
	//}}AFX_VIRTUAL
	
	
	// Implementation
	
	T_RPAS_D		m_Data;

	void SetControl();

	void Data2Dlg();
	BOOL Dlg2Data();
	/*void Execute();*/

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSelAll();
	afx_msg void OnBtnUnSelAll();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include "HeaderPost.h"

#endif // !defined(AFX_DGNRATINGPARAMDLG_H__)
