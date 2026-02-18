#if !defined(__DGN_CON_CODE_NEW_CHILD_10_DLG_H__)
#define __DGN_CON_CODE_NEW_CHILD_10_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewDlg.h : header file
//
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"

#include "DconChildDialog.h"
#include "..\wg_base\FoldGroupBox.h"
#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewDlg dialog
class __MY_EXT_CLASS__ CDgnConCodeNewChild10Dlg : public CDconChildDialog
{
// Construction
public:
	CDgnConCodeNewChild10Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData=NULL, BOOL bExistData=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConCodeNewDlg)
	enum { IDD = IDD_DGN_CON_CODE_CHILD10_DLG };
	
	BOOL    m_bStrongColmWeakBeam;
	BOOL    m_bSpecialWall;
	BOOL    m_bBeamDeflect;
	int     m_nBndrElemMethod;
	int     m_iAlphaType1;
	double  m_dAlpha1;
	double  m_dAlpha2;
	double  m_dAlpha2_Colm;
	double  m_dConShearStrengthRatio;
	double  m_dMrft;
	double  m_dTrft;

	BOOL    m_bSubBeam;
	BOOL    m_bCantilever;
	BOOL    m_bUnderBmColm;
	BOOL	  m_bTorsionDesign;
	BOOL    m_bBCJointDgn;

	int       m_nExposure; // 0:Dry, 1:etc

	mit::frx::MButton	  m_btnTorsion;
	mit::frx::MComboBox m_CmbCd;
	mit::frx::MComboBox m_CmbIe;
	double    m_dCd;
	double    m_dIe;

	CFoldGroupBox_Cross	m_grpSeisParam;
	int		m_nPMCurveMethod;
	int		m_nCheckPos;
	int		m_nFsMethod;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewDlg)
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
	CArray<UINT, UINT> m_aExposureCond;
	CArray<UINT, UINT> m_arCtrlSDR;	
	CArray<UINT, UINT> m_arCtrlAlphaColm;	
	CArray<UINT, UINT> m_aPMCurveMethod;
	CArray<UINT, UINT> m_aSelectPosGroup;
	CArray<UINT, UINT> m_aSCWBDesign;
	CArray<UINT, UINT> m_aRdoSCWBMethod;
	CArray<UINT, UINT> m_aFsMethod;

	BOOL m_bShowExposure;
	BOOL m_bShowAlpha2Colm;
	BOOL m_bShowBeamDeflect;
 	int  m_nBeamDeflectSpan;
	BOOL m_bShowPMCurveMethod;
	//   T_DCON_D* m_pData;
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewDlg)
	void InitCtrl();
	void SetComboBox();
	void SetComboCdAndIe();
	void SetComboCdAngle_ACI318();
	void GetInitValue(CString strCode);
	void EnableWindowBySpecialWall();
	void EnableWindowByBndrElemMethod();
	void EnableWindowByBeamColmJointDesign(const CString& strCode, BOOL bChangeValue=TRUE);
	void EnableWindowByNTC_LDS();
	void SetEQCtrl(const CString& strCode);
	void ReAlingShearDesignCtrl();
	void SetShowExposure(const CString& strCode);
 	void SetShowBeamDeflection(const CString& strCode);
	void SetAlpha2Colm(const CString& strCode);
	BOOL UseAlpha2Colm(const CString& strCode);
	BOOL UseBeamColmJointDesign(const CString& strCode);
	void AlignControl();
	void SetShowPMCurveMethod(const CString& strCode);
	void SetControlText(const CString& strCode);
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcctrlSpecial();
	afx_msg void OnChangeFrameType();
	afx_msg void OnChangeAlphaType();
	afx_msg void OnChangeTorsion();
	afx_msg void OnUpdateByCode();
	afx_msg void OnChkSpecialWall();
	afx_msg void OnBCJoint();
	afx_msg void OnChangeBndrElemMethodType();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_CHILD_10_DLG_H__)
