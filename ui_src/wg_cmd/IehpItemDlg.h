#if !defined(__IEHPITEMDLG_H__)
#define __IEHPITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpItemDlg.h : header file
//

#include "..\MIT_frx\MenuBtnEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CIehpItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CIehpItemDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CIehpItemDlg(CWnd* pParent = NULL);   // standard constructor

//-----------------------------------------------------------------------
// Interfaces
//-----------------------------------------------------------------------
public:
	void SetParamData(T_IEHP_K Key, T_IEHP_D &data);
	void SetAssignHingeMode(BOOL bAssignMode, T_IEHG_K IehgKey);
	enum ELEMENT_TYPE   { HT_BEAM_COLM=0, HT_WALL_CRB, HT_SPRING, HT_TRUSS };
	enum DEFNITION_TYPE {MOMENT_ROTATION=0, MOMENT_CURVATURE=1};
	enum MATERIAL_TYPE  {RC_SRC_ENCASED=0, STEEL_SRC_FILLED=1};
	enum TABSTYLE  {LOCATION_NONE = 0,I_LOCATION=1,J_LOCATION=2, I_J_LOCATION=3, CENTER_LOCATION=4};

// Dialog Data
	//{{AFX_DATA(CIehpItemDlg)
	enum { IDD = IDD_ETC_IEHP_ITEM };

	int		m_nElementType;

	
	MComboBox	m_cmbSect;
	MComboBox m_cmbFibrName;

	MEdit	    m_edtName;
	CString	  m_strDesc;
	CEditUnit m_edtShearR;

	int		m_nInteractionType;
	int		m_nMatlType;
	int		m_nMembType;
	int		m_nAutoUserLocMemb;
	int		m_nLocMemb; // 0:I, 1:C, 2:J
	int   m_nNewHingeType;
	int   m_nDefinitionType;
	int		m_nWallType; // 0=Membrane, 1=Plate
	int   m_nFiberAutoUser; // 0:Auto, 1:User
	BOOL  m_bAddData4D2E;
	int    m_nAutoUserFiberOutpl;
	BOOL   m_bConsOutpl;
	int    m_nAutoUserShearR;
	double m_dShearR;
	
	int   m_nFlexStiff;
	BOOL  m_bUseNonlinearShearY;
	BOOL  m_bUseNonlinearShearZ;

	//}}AFX_DATA
	
	//CComboBox m_cmbMatl;
	//CComboBox m_cmbSteelCode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	CDBDoc*   m_pDoc;
	CArray<UINT, UINT> m_aAssignHinge;
	CArray<UINT, UINT> m_aHingeLocation;
	CArray<UINT, UINT> m_aNumofSection;
	CArray<UINT, UINT> m_aMembPos;
	CArray<UINT, UINT> m_aCtrlIDFiberShearR;
	CArray<UINT, UINT> m_aCtrlStiffnessCmb;

	BOOL      m_bDof[6];
	CString   m_strSec[6];
	MComboBox m_cmbHngLoc[6];
	MComboBox	m_cmbModel[6];
	MComboBox m_cmbHngStifLoc[6];
	mit::frx::CMenuBtnEx m_btnModel[6];
	T_IEHP_D  m_Data;
	T_IEHP_K  m_Key, m_OldKey;
	T_IEHG_K  m_AssignedIehgKey;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bPropMod[7];
	int       m_nMdlCombSel[6];

	BOOL		  m_bExistIJDataYield;				//yield Dlg에서 IJ단이 있는지
	BOOL			m_bExistIJPropDlg[7];				//Prop Dlg에서 IJ단이 있는지
	BOOL			m_bAssignHingeMode;
protected:
	void InitCtrl();
	void AlignCtrl();
	void HideStiffnessMethodCtrl();
	void EnableDisableCtrls();
	void EnableDisableCtrls_WallType();
	void EnableDisableCtrls_MatlType();
	void EnableDisableCtrls_RefLoc();
	void EnableDisableCtrls_DefType();
	void EnableDisableCtrls_HingeType();
	void EnableDisableCtrls_Interact();
	void EnableDisableCtrls_Comp();       // Component 성분 별로 활성/비활성화
	void EnableDisableCtrls_YldSurf();
	void EnableDisableCtrls_FiberName();
	void Data2Dlg();
	BOOL Dlg2Data();
	void Dlg2CommonData();
	
	void InitSectComboCtrl();
	void InitSectComboCtrl_Sect();
	void InitSectComboCtrl_Thik();
	void HingeComboCtrl();
	void ModelComboCtrl();
	void MakeFibrCombo(BOOL bModify = FALSE);

	void SetKinemaData(int nSel);
	void SetOriginData(int nSel);
	void SetPeakData(int nSel);
	void SetCloughData(int nSel);
	void SetDegradData(int nSel);
	void SetTakedaData(int nSel);

	bool PropBtnProc(T_IEHP_D data, int n, int nSel, int nFg, int nTabStyle, bool bUpdateNeeded=false);
	void CtrlTypeManager();
	void CtrlManager();
	

	int GetTabStyle(int nIndex);
	/**/void CtrlElementType();
	/**/void CtrlNewHingeType();
	/**/void CtrlDefinition();
	/**/void CtrlMaterialType();
	/**/void CtrlInteractionType();
	/**/void CtrlWallType();
	/**/void CtrlLocation();
	
	void ChangeNumofSectionDx();
	void EnableDisableCtrlDofDx();
	void EnableDisableCtrlDofDy();
	void EnableDisableCtrlDofDz();
	void EnableDisableCtrlDofRx();
	void EnableDisableCtrlDofRy();
	void EnableDisableCtrlDofRz();
	void ChangebyCtrlDofRy();
	void ChangebyCtrlDofRz();

	CString GetHysModelName(int nHysModel);
	int GetCategory(int nHysModel);
	void OnSelchangeModelCmb(int nDof);
	void MakeBtnMenu(int ix);
	void SetCurSelBtnMenu(int ix);
	BOOL IsHysModel_Unable1stCtrl(int nHysModel);

    void InitCtrlTexts();

	bool UpdatePropData(int nDof);

// Implementation  
protected:

	// Generated message map functions
	//{{AFX_MSG(CIehpItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdElementType();
	afx_msg void OnCmdDefinitionType();
	afx_msg void OnCmdNewHingeType();
	afx_msg void OnInteractionType();
	afx_msg void OnMaterialType();
	afx_msg void OnCmdWallType();

	afx_msg void OnCmdApply();
	afx_msg void OnSelchangeIntrType();
	afx_msg void OnCmdDofDxChk();
	afx_msg void OnCmdDofRyChk();
	afx_msg void OnCmdDofRzChk();
	afx_msg void OnCmdPropDxBtn();
	afx_msg void OnCmdPropDyBtn();
	afx_msg void OnCmdPropDzBtn();
	afx_msg void OnCmdPropRxBtn();
	afx_msg void OnCmdPropRyBtn();
	afx_msg void OnCmdPropRzBtn();
	afx_msg void OnCmdPropYsBtn();
	afx_msg void OnChangeSectDxEdit();
	afx_msg void OnSelchangeLocDxCmb();
	virtual void OnOK();
	afx_msg void OnSelchangeModelDxCmb();
	afx_msg void OnSelchangeModelDyCmb();
	afx_msg void OnSelchangeModelDzCmb();
	afx_msg void OnSelchangeModelRxCmb();
	afx_msg void OnSelchangeModelRyCmb();
	afx_msg void OnSelchangeModelRzCmb();
	afx_msg void OnLocationChangedRyCmb();
	afx_msg void OnLocationChangedRzCmb();

	afx_msg void OnCmdDofDyChk();
	afx_msg void OnCmdDofDzChk();
	afx_msg void OnCmdDofRxChk();
	afx_msg void OnSelchangeSectCmb();
	afx_msg void OnAddDyna2E();  
	afx_msg void OnModelChangedDx();
	afx_msg void OnModelChangedDy();
	afx_msg void OnModelChangedDz();
	afx_msg void OnModelChangedRx();
	afx_msg void OnModelChangedRy();
	afx_msg void OnModelChangedRz();
	afx_msg void OnMphiLocBtn   ();
	afx_msg void OnFiberNameBtn ();
	afx_msg void OnFiberOutplBtn();
	afx_msg void OnShearRBtn    ();

	afx_msg void OnFlexStifType();
	afx_msg void OnStiffnessOptionBtn();
	afx_msg void OnSelchangeStiffLocDxCmb();
	afx_msg void OnSelchangeStiffLocDyCmb();
	afx_msg void OnSelchangeStiffLocDzCmb();
	afx_msg void OnSelchangeStiffLocRxCmb();
	afx_msg void OnSelchangeStiffLocRyCmb();
	afx_msg void OnSelchangeStiffLocRzCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__IEHPITEMDLG_H__)
