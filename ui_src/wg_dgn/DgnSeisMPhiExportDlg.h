#if !defined(__DgnSeisMPhiExportDlg_H__)
#define __DgnSeisMPhiExportDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisMPhiExportDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_dgn\BrdgSeisEval.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;
struct DGN_SEIS_MPHI_EXPORT
{
	CString strName;
	BOOL bIehp;
	BOOL bPhgt;

	BOOL    bMyIehp;
	BOOL    bMzIehp;
	double  dMyIehpLp;
	double  dMzIehpLp;

	BOOL    bMyPhgt;
	int     nMyLocPhgt;  // Hinge Location 0:I 1:J 2:I&J
	double  dMyPhgt_I;   // Hinge Length Ratio(Lp/L) at I-end
	double  dMyPhgt_J;

	BOOL    bMzPhgt;
	int     nMzLocPhgt; // Hinge Location 0:I 1:J 2:I&J
	double  dMzPhgt_I;
	double  dMzPhgt_J;

	double  dMMy;  // Residual Resistance After Failure (M/My)
	double  dDDy;  // Residual Resistance (D/Dy)

	void Initialize()
	{
		strName = _T("");
		bIehp = TRUE;
		bPhgt = TRUE;

		bMyIehp = TRUE;
		bMzIehp = TRUE;
		dMyIehpLp = 1.0; //m
		dMzIehpLp = 1.0; //m

		bMyPhgt = TRUE;
		nMyLocPhgt = 2; // IJ
		dMyPhgt_I = 0.1;
		dMyPhgt_J = 0.1;

		bMzPhgt = TRUE;
		nMzLocPhgt = 2; //IJ
		dMzPhgt_I = 0.1;
		dMzPhgt_J = 0.1;

		dMMy = 0.0;
		dDDy = 15.0;
	}

	DGN_SEIS_MPHI_EXPORT() {}
	DGN_SEIS_MPHI_EXPORT(DGN_SEIS_MPHI_EXPORT& rData) { *this = rData; }
	DGN_SEIS_MPHI_EXPORT& operator = (DGN_SEIS_MPHI_EXPORT& rData)
	{
		strName = rData.strName;
		bIehp = rData.bIehp;
		bPhgt = rData.bPhgt;
		bMyIehp = rData.bMyIehp;
		bMzIehp = rData.bMzIehp;
		dMyIehpLp = rData.dMyIehpLp;
		dMzIehpLp = rData.dMzIehpLp;
		bMyPhgt = rData.bMyPhgt;
		nMyLocPhgt = rData.nMyLocPhgt;
		dMyPhgt_I = rData.dMyPhgt_I;
		dMyPhgt_J = rData.dMyPhgt_J;
		bMzPhgt = rData.bMzPhgt;
		nMzLocPhgt = rData.nMzLocPhgt;
		dMzPhgt_I = rData.dMzPhgt_I;
		dMzPhgt_J = rData.dMzPhgt_J;
		dMMy = rData.dMMy;
		dDDy = rData.dDDy;

		return *this;
	}
};


// FEMA Property의 Primary Curve를 그리는데 사용되는 구조체
const int MPHI_POINT_NUM = 9;
struct DGN_SEiS_MPHI_FEMA
{
	double dAxialForce; // From Define Axial Force Dialog
	double dMMy[2][9]; // [0]:Y-Axis,          / [1]:Z-Axis
	double dDDy[2][9]; // [0-3]:180', [5-8]:0' / [0-3]:270', [5-8]:180'
	double dMY[2][2]; // Yield Strength [Y,Z-Axis][+,-]
	double dDY[2][2]; // Yield Rotation [Y,Z-Axis][+,-]

	void Initialize()
	{
		dAxialForce = 0.;
		for (int i = 0; i < 2; i++)
		{
			dMMy[i][0] = -0.2; dDDy[i][0] = -15.0;
			dMMy[i][1] = -0.2; dDDy[i][1] = -6;
			dMMy[i][2] = -1.25; dDDy[i][2] = -6;
			dMMy[i][3] = -1; dDDy[i][3] = -1;
			dMMy[i][4] = 0; dDDy[i][4] = 0;
			dMMy[i][5] = 1; dDDy[i][5] = 1;
			dMMy[i][6] = 1.25; dDDy[i][6] = 6;
			dMMy[i][7] = 0.2; dDDy[i][7] = 6;
			dMMy[i][8] = 0.2; dDDy[i][8] = 15.0;
			dMY[i][0] = 0.;
			dMY[i][1] = 0.;
			dDY[i][0] = 0.;
			dDY[i][1] = 0.;
		}
	}

	DGN_SEiS_MPHI_FEMA() { Initialize(); }
	DGN_SEiS_MPHI_FEMA(DGN_SEiS_MPHI_FEMA& rData) { *this = rData; }
	DGN_SEiS_MPHI_FEMA& operator = (DGN_SEiS_MPHI_FEMA& rData)
	{
		dAxialForce = rData.dAxialForce;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < MPHI_POINT_NUM; j++)
			{
				dDDy[i][j] = rData.dDDy[i][j];
				dMMy[i][j] = rData.dMMy[i][j];
			}
			dMY[i][0] = rData.dMY[i][0];
			dDY[i][0] = rData.dDY[i][0];
			dMY[i][1] = rData.dMY[i][1];
			dDY[i][1] = rData.dDY[i][1];
		}
		return *this;
	}
};

const int MPHI_I_end = 0;
const int MPHI_J_end = 1;
const int MPHI_IJ_end= 2;

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiExportDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisMPhiExportDlg : public CDialogMove //, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisMPhiExportDlg(CWnd* pParent = NULL);   // standard constructor
	
	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_MPCC_D &MpccData, T_BRDGEVAL_MPHI_RES MphiResD1[2], T_BRDGEVAL_MPHI_RES MphiResD2[2]);
 

public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisMPhiExportDlg)
	enum { IDD = IDD_DGN_SEIS_MPHI_EXPORT_DLG };
	
	CEdit m_edtName;
	BOOL  m_bPhgt;
	BOOL  m_bIehp;

	BOOL  m_bMyIehp;
	BOOL  m_bMzIehp;
	CEditUnit m_edtMyIehp;
	CEditUnit m_edtMzIehp;
	CTextUnit m_untMyIehp;
	CTextUnit m_untMzIehp;

	BOOL  m_bMyPhgt;
	BOOL  m_bMzPhgt;
	MComboBox m_CmbMyPhgtLoc;
	MComboBox m_CmbMzPhgtLoc;
	CEditUnit m_edtPhgtMy_I;
	CEditUnit m_edtPhgtMy_J;
	CEditUnit m_edtPhgtMz_I;
	CEditUnit m_edtPhgtMz_J;

	CEditUnit m_edtMMy;
	CEditUnit m_edtDDy;

	DGN_SEIS_MPHI_EXPORT m_Data;
	T_MPCC_D m_MpccData;
	T_BRDGEVAL_MPHI_RES m_MphiResD1[2];
	T_BRDGEVAL_MPHI_RES m_MphiResD2[2];

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisMPhiExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitUnit();
	void InitCombo();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckData();
	void EnableDisableControls();

	void MakePhgtData(T_PHGT_D& PhgtD);

	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisMPhiExportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOKBtn();
	afx_msg void OnCancelBtn();
	afx_msg void OnApplyBtn();
	
	afx_msg void OnIehpChkBtn();
	afx_msg void OnPhgtChkBtn();
	afx_msg void OnMyIehpChkBtn();
	afx_msg void OnMzIehpChkBtn();
	afx_msg void OnMyPhgtChkBtn();
	afx_msg void OnMzPhgtChkBtn();
	afx_msg void OnMyPhgtLocCmb();
	afx_msg void OnMzPhgtLocCmb();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CArray<UINT, UINT> m_aCtrlMyIehp;
	CArray<UINT, UINT> m_aCtrlMzIehp;
	CArray<UINT, UINT> m_aCtrlMyPhgt;
	CArray<UINT, UINT> m_aCtrlMzPhgt;

// 	int MPHI_I_end ;
// 	int MPHI_J_end ;
// 	int MPHI_IJ_end ;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisMPhiExportDlg_H__)
