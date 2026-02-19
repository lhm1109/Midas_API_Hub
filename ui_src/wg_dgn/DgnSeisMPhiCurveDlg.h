#if !defined(__DGNSEISMPHICURVEDLG_H__)
#define __DGNSEISMPHICURVEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisMPhiCurveDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "DgnSeisMPhiCurveGrid.h"
#include "DgnSeisMPhiCurveGraph.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiCurveDlg dialog
#include "HeaderPre.h"
//class CBrdgSeisEval;
class __MY_EXT_CLASS__ CDgnSeisMPhiCurveDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisMPhiCurveDlg(CWnd* pParent = NULL);   // standard constructor  
	virtual ~CDgnSeisMPhiCurveDlg();

	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_MPCC_D &ParamData)  { m_Data = ParamData; }
	void MakeBmpAndClose(CString strPath);
	//void SetAngleDirChange(int nAngleDir);
	//----------------------------------------------------------------------
	// Interface
public:
	// Dialog Data
	//{{AFX_DATA(CDgnSeisMPhiCurveDlg)
	enum { IDD = IDD_DGN_SEIS_MPHI_CURVE_DLG };
	
	int       m_nAngleDir;
	MComboBox m_cmbCurve;
	CEditUnit m_edtIyL;
	CEditUnit m_edtIyT;	
	MEdit     m_edtFileName;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisMPhiCurveDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

//----------------------------------------------------------------------
// Implementation
protected:
	void InitUnit();
	void InitCombo();
	void InitGrid();
	void InitView();
	void InitControl();

	void EnableDisableControls();

	void Data2Dlg();
	void GetCurveOpt(); 
	BOOL Get_MPhiResult(const T_MPCC_D& MpccD, T_BRDGEVAL_MPHI_RES mphiResD[2], BOOL bRecal, double dAddAngle=0.0);

	void GetTempData();
	
	CDBDoc* m_pDoc;

	CDgnSeisMPhiCurveGrid* m_pGrid;
	CDgnSeisMPhiCurveGraph* m_pGraph;

	T_MPCC_D m_Data;

	T_BRDGEVAL_MPHI_RES m_MphiResD[2];
	T_CurveOpt_D m_CurveOpt;	

	CBrdgSeisEval* m_pSeisEval;
	
	BOOL m_bSetBmp;
	CString m_strBmpPath;


	// Generated message map functions
	//{{AFX_MSG(CDgnSeisMPhiCurveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSeisMPhiAngleDirRdo();
	afx_msg void OnDgnSeisMPhiCurveCmb();
	afx_msg void OnDgnSeisMPhiSectEffStiffBtn();
	afx_msg void OnDgnSeisMPhiBrowserBtn();
	afx_msg void OnDgnSeisMPhiOutputBtn();
	afx_msg void OnDgnSeisMPhiCloseBtn();
	afx_msg void OnDgnSeisMPhiExportBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISMPHICURVEDLG_H__)
