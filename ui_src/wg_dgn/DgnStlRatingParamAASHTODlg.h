#if !defined(AFX_DGNRATINGPARAMAASHTODLG_H__)
#define AFX_DGNRATINGPARAMAASHTODLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlRatingParamAASHTODlg.h : header file
//
#include "..\wg_base\DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"

#include "HeaderPre.h"
 
class __MY_EXT_CLASS__ CDgnStlRatingParamAASHTODlg : public CDialogMove
{
	// Construction
public:
	CDgnStlRatingParamAASHTODlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnStlRatingParamAASHTODlg();
	
	// Dialog Data
	//{{AFX_DATA(CDgnStlRatingParamAASHTODlg)
	enum { IDD = IDD_DGN_STL_RATING_PARAM_AASHTO_DLG };

	T_RCDS_D m_RcdsD; 
	CEditUnit m_edtSysTemFact;

	BOOL      m_bStre;
	CEditUnit m_edtStrePhiy;
	CEditUnit m_edtStrePhiu;
	CEditUnit m_edtStrePhic;
	CEditUnit m_edtStrePhif;
	CEditUnit m_edtStrePhiv;
	CEditUnit m_edtStrePhise;
	CEditUnit m_edtStrePhib;
	int       m_nGirderType;
	BOOL      m_bTorsionType;
	int       m_nBridgeType;
	BOOL      m_bStreOption1;
	BOOL      m_bStreOption2;
	BOOL      m_bStreOption3;
	BOOL      m_bStreOption4;
	
	BOOL      m_bServ;
	int       m_nCalcType;
	CEditUnit m_edtDgnComp;
	CTextUnit m_untDgnComp; 
	CEditUnit m_edtDgnTens;
	CTextUnit m_untDgnTens;
	CEditUnit m_edtLegalComp;
	CTextUnit m_untLegalComp;
	CEditUnit m_edtLegalTens;
	CTextUnit m_untLegalTens;

	CEditUnit m_edtDetailA;
	CEditUnit m_edtADTTSL_G;
	CEditUnit m_edtADTTSL_P;
	CEditUnit m_edtADTTSL_0;
	CEditUnit m_edtADTTSL_Lim;
	CFoldGroupBox_Cross	m_chkBmpFat;
	CFoldGroupBox_Cross	m_chkBmpSer;

	BOOL      m_bFati;

	int       m_nMeasurement;

	int       m_nFatigueLife;
	int       m_nFatServ_G;
	int       m_nFatServ_R;
	int       m_nFatServ_I;
	//}}AFX_DATA
	
protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlRatingParamAASHTODlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support		
	//}}AFX_VIRTUAL
		
	// Implementation
	void InitCtrl();
	void AlignControl();
	void ResizeDialog();

	void InitUnit();
	void ControlsEnableDisable();

	void Data2Dlg();
	BOOL Dlg2Data();

	T_RPAS_D m_Data;

	CArray<UINT, UINT> m_aFatCtrls;
	CArray<UINT, UINT> m_aTestCtrls;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingParamAASHTODlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();	
	afx_msg void OnDgnUpdateCodeBtn();
	afx_msg void OnDgnStreChk();
	afx_msg void OnDgnServChk();
	afx_msg void OnDgnFatiChk();
	afx_msg void OnDgnCalcTypeRdo();
	afx_msg void OnDgnGirderTypeRdo();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include "HeaderPost.h"

#endif // !defined(AFX_DGNRATINGPARAMAASHTODLG_H__)
