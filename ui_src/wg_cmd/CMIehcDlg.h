#pragma once

/////////////////////////////////////////////////////////////////////////////
// CCMIehcDlg 
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\SpinBtnExCtrl.h"
#include "wg_cmdres.h"

using namespace mit::frx;

#include "HeaderPre.h"
struct T_IEHC_D;
class __MY_EXT_CLASS__ CCMIehcDlg : public CDialogMove
{
public:
	CCMIehcDlg(CWnd* pParent = NULL);   // standard constructor

public:
#if defined(_MGEN)
	enum {	IDD = IDD_CMD_IEHC_DLG_GEN	};
#else
	enum { IDD = IDD_CMD_IEHC_DLG };
#endif

public:	
	void SetData(int nAssignType, int nBeamLoc, int nBeamDivNumNyCore, int nBeamDivNumNzCore, int nBeamDivNumNyCover, int nBeamDivNumNzCover, BOOL bWallConsOut, int nWallDivNumZCore, int nWallDivNumYCore, int nWallDivNumZCover, int nWallDivNumYCover, double dR, int nFAreaSizeCore,int nFAreaSizeCover,int nWAreaSizeCore,int nWAreaSizeCover, BOOL bConsiderRebarArea1D, BOOL bConsiderRebarAreaWall);
	void GetData(int& nBeamLoc, int& nBeamDivNumNyCore, int& nBeamDivNumNzCore, int& nBeamDivNumNyCover, int& nBeamDivNumNzCover, BOOL& bWallConsOut, int& nWallDivNumZCore, int& nWallDivNumYCore, int& nWallDivNumZCover, int& nWallDivNumYCover, double& dR, int& nFAreaSizeCore,int& nFAreaSizeCover,int& nWAreaSizeCore,int& nWAreaSizeCover, BOOL& bConsiderRebarArea1D, BOOL& bConsiderRebarAreaWall);
protected:
	int    m_nBeamLoc     ;
	BOOL   m_bWallConsOut ;	
	BOOL   m_bBeamConsiderRebar;
	BOOL   m_bWallConsiderRebar;
	CFormulaEditSpin m_edtBeamDivNumNyCore;
	CFormulaEditSpin m_edtBeamDivNumNzCore;
	CFormulaEditSpin m_edtBeamDivNumNyCover;
	CFormulaEditSpin m_edtBeamDivNumNzCover;
	CFormulaEditSpin m_edtWallDivNumZCore;
	CFormulaEditSpin m_edtWallDivNumYCore;
	CFormulaEditSpin m_edtWallDivNumZCover;
	CFormulaEditSpin m_edtWallDivNumYCover;
	CEditUnit m_edtShearR;
	int m_nFAreaSizeCore;	
	int m_nWAreaSizeCore;
	int m_nFAreaSizeCover;
	int m_nWAreaSizeCover;

	//T_IEHC_D m_IehcD;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	void InitCtrl();
	void AlignCtrls();
	void Data2Dlg();
	void Dlg2Data();
	void EnableDisableCtrls();
	// Generated message map functions
	//{{AFX_MSG(CCMIehcDlg)
	afx_msg void OnShearRBtn();
	afx_msg void OnSpinBeamNyCore(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinBeamNzCore(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinWallZCore(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinWallYCore(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinBeamNyCover(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinBeamNzCover(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinWallZCover(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinWallYCover(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOnConsOutplBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int    m_nAssignType  ; // D_FIBR_TYPE. 0:ºñÅº¼ºÈùÁö, 1:Pushover
	int    m_nBeamDivNumNyCore;
	int    m_nBeamDivNumNzCore;
	int    m_nWallDivNumZCore ;
	int    m_nWallDivNumYCore ;
	double m_dR           ;
	int    m_nBeamDivNumNyCover;
	int    m_nBeamDivNumNzCover;
	int    m_nWallDivNumZCover ;
	int    m_nWallDivNumYCover ;

};
#include "HeaderPost.h"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

