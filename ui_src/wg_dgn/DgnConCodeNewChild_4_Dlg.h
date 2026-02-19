#if !defined(__DGN_CON_CODE_NEW_CHILD_4_DLG_H__)
#define __DGN_CON_CODE_NEW_CHILD_4_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewChild_4_Dlg.h : header file
//
#include "..\mit_frx\MComboBox.h"

#include "DconChildDialog.h"
#include "..\wg_base\FoldGroupBox.h"
#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild4Dlg dialog
class __MY_EXT_CLASS__ CDgnConCodeNewChild4Dlg : public CDconChildDialog
{
// Construction
public:
	CDgnConCodeNewChild4Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData=NULL, BOOL bExistData=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConCodeNewChild4Dlg)
	enum { IDD = IDD_DGN_CON_CODE_CHILD4_DLG };
	
	BOOL    m_bStrongColmWeakBeam;
	BOOL    m_bBeamDeflect;
	int     m_nSpecialWallMtd;  //0: DES, 1: DMO, 2: DMI
	int     m_nBndrElemMethod;
	int     m_iAlphaType1;
	double  m_dAlpha1;
	double  m_dAlpha2;
	double  m_dAlpha2_Colm;
	double  m_dConShearStrengthRatio;
	double  m_dMrft;

	BOOL    m_bSubBeam;
	BOOL    m_bCantilever;
	BOOL    m_bUnderBmColm;
	BOOL    m_bBCJointDgn;

	mit::frx::MComboBox m_CmbCd;
	mit::frx::MComboBox m_CmbIe;
	double    m_dCd;
	double    m_dIe;
	int		m_nPMCurveMethod;
	int		m_nCheckPos;
	int		m_nFsMethod;

	CFoldGroupBox_Cross	m_grpSeisParam;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewChild4Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual void ChangeDesignCode();
	virtual void UpdateChildData();
	virtual void ResizeChildDlg();
	virtual BOOL Dlg2Data();
protected:
	CArray<UINT, UINT> m_aSelectFrameType;
	CArray<UINT, UINT> m_aStrongColmWeakBeam;
	CArray<UINT, UINT> m_aShearWallType;
	CArray<UINT, UINT> m_aShearforDesign;
	CArray<UINT, UINT> m_aMembTypeSeisDgn;
	CArray<UINT, UINT> m_aTorsionDesign;
	CArray<UINT, UINT> m_aMomentRedistr;
	CArray<UINT, UINT> m_arCtrlSDR;	
	CArray<UINT, UINT> m_arCtrlAlphaColm;	
	CArray<UINT, UINT> m_aPMCurveMethod;
	CArray<UINT, UINT> m_aSelectPosGroup;
	CArray<UINT, UINT> m_aSCWBDesign;
	CArray<UINT, UINT> m_aRdoSCWBMethod;
	CArray<UINT, UINT> m_aFsMethod;

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewChild4Dlg)
	void InitCtrl();
	void SetComboBox();
	void SetComboCdAndIe();
	void GetInitValue(CString strCode);
	void EnableWindowBySpecialWall();
	void EnableWindowByBndrElemMethod();
	void EnableWindowByBeamColmJointDesign(BOOL bChangeValue=TRUE);

	void SetEQCtrl(const CString& strCode);
	void ReAlingShearDesignCtrl();
	
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcctrlSpecial();
	afx_msg void OnChkSpecialWall();
	afx_msg void OnChangeFrameType();
	afx_msg void OnChangeAlphaType();
	afx_msg void OnUpdateByCode();
	afx_msg void OnBCJoint();
	afx_msg void OnChangeBndrElemMethodType();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_CHILD_4_DLG_H__)
