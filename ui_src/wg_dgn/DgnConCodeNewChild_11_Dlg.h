#if !defined(__DGN_CON_CODE_NEW_CHILD_11_DLG_H__)
#define __DGN_CON_CODE_NEW_CHILD_11_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewChild_5_Dlg.h.h : header file
//
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"

#include "DconChildDialog.h"
#include "..\wg_base\FoldGroupBox.h"
#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild11Dlg dialog
class CDgnDataCtrl;
class __MY_EXT_CLASS__ CDgnConCodeNewChild11Dlg : public CDconChildDialog
{
    // Construction
public:
    CDgnConCodeNewChild11Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData = NULL, BOOL bExistData = FALSE);   // standard constructor
    virtual ~CDgnConCodeNewChild11Dlg();

    // Dialog Data
        //{{AFX_DATA(CDgnConCodeNewChild11Dlg)
    enum { IDD = IDD_DGN_CON_CODE_CHILD11_DLG };

    BOOL    m_bStrongColmWeakBeam;
	BOOL    m_bSpecialWall;
	BOOL    m_bBeamDeflect;

    BOOL	  m_bTorsionDesign;
    int     m_nBndrElemMethod;
    int     m_iAlphaType1;
    int     m_nCheckPos;
    double  m_dAlpha1;
    double  m_dAlpha2;
    double  m_dAlpha2_Colm;
    double  m_dConShearStrengthRatio;
    double  m_dMrft;
    double  m_dTrft;

    mit::frx::MButton	  m_btnTorsion;

    int		m_nPMCurveMethod;

    BOOL    m_bSubBeam;
    BOOL    m_bCantilever;
    BOOL    m_bUnderBmColm;
	int 	m_nBeamRebarConsiderOpt; // 0: Equivalent Rebar, 1: Each Rebar
	BOOL	m_bConsiderBeamElemForce;
    BOOL    m_bBCJointDgn;
    BOOL    m_bNotConsiderk1;
    double  m_dPhic;
    BOOL    m_bConsiderInteractionShear;
	int		m_nFsMethod;
	
	mit::frx::MComboBox m_CmbCd;
	mit::frx::MComboBox m_CmbIe;
	double    m_dCd;
	double    m_dIe;
    CFoldGroupBox_Cross	m_grpSeisParam;

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDgnConCodeNewChild11Dlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
public:
    virtual void ChangeDesignCode();
    virtual void UpdateChildData();
    virtual void ResizeChildDlg();
    virtual BOOL Dlg2Data();
protected:
    CArray<UINT, UINT> m_aShearforDesign;
    CArray<UINT, UINT> m_aStrongColmWeakBeam;
    CArray<UINT, UINT> m_aShearWallType;
    CArray<UINT, UINT> m_arCtrlAlphaColm;
    CArray<UINT, UINT> m_aSCWBDesign;
    CArray<UINT, UINT> m_aTorsionDesign;
    CArray<UINT, UINT> m_aMomentRedistr;
    CArray<UINT, UINT> m_aRdoSCWBMethod;
    CArray<UINT, UINT> m_aOptSCWBDesign;
    CArray<UINT, UINT> m_aSelectPosGroup;
    CArray<UINT, UINT> m_aPMCurveMethod;
    CArray<UINT, UINT> m_aMembTypeSeisDgn;
    CArray<UINT, UINT> m_aInteractionShear;
	CArray<UINT, UINT> m_aBeamCalcMethod;
	CArray<UINT, UINT> m_aFsMethod;

    CDgnDataCtrl* m_pDgnDataCtrl;

    //   T_DCON_D* m_pData;
    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDgnConCodeNewDlg)
    void InitCtrl();
    void GetInitValue(CString strCode);
    void SetEQCtrl(const CString& strCode);
    void SetComboCdAndIe();
	void EnableWindowBySpecialWall();
    void EnableWindowByBndrElemMethod();
protected:
    virtual BOOL OnInitDialog();
    afx_msg void OnDgnRcctrlSpecial();
    afx_msg void OnChangeAlphaType();
    afx_msg void OnChangeTorsion();
    afx_msg void OnUpdateByCode();
    afx_msg void OnChkSpecialWall();
    afx_msg void OnChangeSCWBMethod();
    afx_msg void OnChangeBndrElemMethodType();
    afx_msg void OnBCJoint();
    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_CHILD_5_DLG_H__)
