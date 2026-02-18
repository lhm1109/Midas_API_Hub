#if !defined(__DGNCONBARRATIODLG_H__)
#define __DGNCONBARRATIODLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBarRatioDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"



const int iDgn_RbarRatioID1 = 7; // Except EC2-2:05
const int iDgn_RbarRatioID2 = 3; // Only EC2-2:05
const int iDgn_RbarRatioID3 = 3; // Only EC2-2:05
const int iDgn_RbarRatioID4 = 13; // Only Eurocode2:04, Italy

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConBarRatioDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnConBarRatioDlg : public CDialogMove
{
// Construction
public:
	CDgnConBarRatioDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConBarRatioDlg)
	enum { IDD = IDD_DGN_CON_BARR_DLG };
	double	m_Rhoc;
	double	m_Rhor;
	double	m_Rhow;
	CString	m_strCode;
	CString m_strCode2;
	
	BOOL m_bBeam;
	BOOL m_bColm;
	BOOL m_bBrce;
	BOOL m_bWall;
	BOOL m_bSlab;
	BOOL m_bMat;
	BOOL m_bShell;  

	CEditUnit m_edtShellHorz;	
	CEditUnit m_edtShellVert;	

	//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 기존 방식으로 적용한다.
	/*
	double  m_Rhoc_max;
	double  m_Rhoc_min;
	*/

	//}}AFX_DATA
	int m_iRBARRATIOID1[iDgn_RbarRatioID1];
	int m_iRBARRATIOID2[iDgn_RbarRatioID2];
	int m_iRBARRATIOID3[iDgn_RbarRatioID3];
	int m_iRBARRATIOID4[iDgn_RbarRatioID4];

public:
	BOOL ErrorCheck();

	void Initial_DefaultData();
	void Initial_ExistData();

	void Update_InitDataByCode(CString strCodeName);
	CRect MoveCtrl(int* pID, int iArSize, CRect Rect0);
	void InitDlgCtrls();
	int GetDataStatus(CString strCode);
	void ResizeDialog();
	void EnableCtrlDynamic();
	
	//
	int m_nNationalAnnex;
	CString m_strNationalAnnex;
	void SetDatabyNationalAnnex(int iStatus, int nNationalAnnex, double& dMax, double& dMin, double& dShellDir1, double& dShellDir2);
	//

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBarRatioDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
		CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConBarRatioDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnChkShell();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBARRATIODLG_H__B1565525_E5DB_11D3_888F_0000C0F30D4D__INCLUDED_)
