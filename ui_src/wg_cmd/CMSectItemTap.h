#if !defined(AFX_CMSECTITEMTAP_H__673754E8_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMTAP_H__673754E8_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemTap.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CMSectItemBase.h"
#include "CMSectItemGrid.h"
#include "CMSectItemGrid4PscNew.h"
#include "CMSectItemGrid4Composite.h"
#include "CMSectItemGrid4Stlg.h"
#include "CMSectItmePSCView.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemTap dialog
class CCMSectItemStlgView;
class CCMSectItemTap : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemTap(CWnd* pParent = NULL);   // standard constructor
	~CCMSectItemTap();

//---------------------------------------------------
// Overrides
//---------------------------------------------------
public:
	BOOL OnOKPublic() override;
	BOOL OnApplyPublic() override;
	void ShowCalcResult() override;
	void DisplayOffsetPoint() override;
	void SetNewID(T_SECT_K nNewID) override;

	CCMSectItemStlgView* GetViewPointer() { return m_pStlgViewer; }
	void DrawStlgViewDlgSection();
//---------------------------------------------------
// Interface
//---------------------------------------------------
public:
	void ChangeMatlData(T_MATL_CONCRETE &ConcrData, T_MATL_STEEL &SteelData);
	void ChangeMatlDataCICT(T_MATL_CONCRETE &ConcreteDataSlab, T_MATL_CONCRETE &ConcreteDataGirder);

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	BOOL SaveData();
	BOOL Dlg2Data();

	void Dlg2DataMCellEtcCtrl();

	BOOL GetPeriAndStiff();
	void InitControls();
	void AlignControls();
	void ShowKey2Dlg();
	void ShowData2Dlg();

	void Data2DlgMCellJoint();

	void SetSectionShapeCombo();
	void SetDBNameList();
	void SetNameCombo();
	void SetSectData();
	void ChangeBitmap();
	void DoActionWhenDBChanged();
	int  GetShapeIndexFromData(T_SECT_D& rData);
	void ChangeCellNumber(int nCellNum);

	BOOL IsValueSect();      
	BOOL IsPscSect();
	BOOL IsCompositeSect();  
	BOOL IsStlgSect();
	BOOL IsPSCValue();
	BOOL IsGeneralSect();
	//BOOL IsShapeComposite(CString &Shape);
	//BOOL IsTypeComposite(int Stype);
	CString GetDB();
	void    SetDB(CString& csDBName, int nStype);

	int GetCurShapeIndex();
	void SetCurShapeByIndex(int nShapeIndex);
	int GetCobxIndexByShapeIndex(int nShapeIndex);

	void SetMatlSectData();
	void GetMatlSectData(T_SECT_D* pData);

	// MNET:XXXX-CJJEONG-20090316 (Tel : 2124) 
	void SetGeneralPartList();
	//BOOL SaveGeneralData();
	BOOL ConfirmBeforeImportCG();
	void CreateOrDestoryStlgView();  
	void ReDrawStlgViewer();
	void DestroyStlgView();
	void CreateStlgView();
protected:
	CArray<UINT, UINT> m_aCtrlDB;
	CArray<UINT, UINT> m_aCtrlUserDB;
	CArray<UINT, UINT> m_aCtrlValue;
	CArray<UINT, UINT> m_aCtrlFrame;
	CArray<UINT, UINT> m_aCtrlValuePSC;
	CArray<UINT, UINT> m_aCtrlROctagon;
	CArray<UINT, UINT> m_aCtrlMeshSize;
	CArray<UINT, UINT> m_aCtrlMaterial;
	CArray<UINT, UINT> m_aCtrlOption; // Calc Method
	CArray<UINT, UINT> m_aCtrlComposite;
	CArray<UINT, UINT> m_aCtrlCompositeMulti;
	CArray<UINT, UINT> m_aCtrlComposite2;
	CArray<UINT, UINT> m_aCtrlStlgBoxI;
	CArray<UINT, UINT> m_aCtrlStlgMCell;
	CArray<UINT, UINT> m_aCtrlView;

	// For changing section name by automatically
	BOOL    m_bAutoChangeName;

	// for User Section
	CCMSectItemGrid m_wndGridFirst;
	CCMSectItemGrid m_wndGridSecond;
	CCMSectItemGrid4PscNew m_wndGrid4Psc;
	CCMSectItemGrid4Composite m_wndGrid4Composite; 
	CCMSectItemGrid4Stlg m_wndGrid4StlG; 
	CCMSectItmePSCView* m_pViewer;
	CCMSectItemStlgView* m_pStlgViewer;

	// MNET:XXXX-CJJEONG-20090316 (Tel : 2124) 
	// Composite General Section Variable
	CCMSecViewWnd   m_wndSecViewCGI; // Section Preview
	CCMSecViewWnd   m_wndSecViewCGJ; // Section Preview
	CCMSectItemGrid m_wndGridComGenI;
	CCMSectItemGrid m_wndGridComGenJ;
	BOOL            m_bIsCGImport[2];

	BOOL m_bReCalc;

	T_SECP_D  m_PSCDesign;

	void CreatePSCView();
	void DestroyPSCView();
	BOOL IsViewWindow();
	void ShowHideEnableDisableStiffCalcMethod();
	void EnableDisableStlgMCell();
	void CalcRegGenLineTypeStiffAndUpdateGrid(int iOrj);
	void ShowHideComposite(BOOL bComposite);
	void ShowHidePSCWarpingCheck(BOOL bPSC);

public:
	UINT GetBitmapIndexFromInfoPSC(int nParam=0);
	UINT GetBitmapIndexFromInfoComposite(int nParam=0);
	BOOL ChangeBitmapForViewer(UINT nIndex);

	void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6]);
	void SectWapingCheckPSC();

	void OnCmdCalcSectPscValPropI();
	void OnCmdCalcSectPscValPropJ();

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemTap)
	enum { IDD = IDD_CMD_SECT_ITEM_TAP };
	CTextUnit	m_unitMeshSize;
	CEditUnit	m_editMeshSize;
	MButton	m_chkMeshSize;
	CBCGPStatic	m_txtOffset;
	MEdit	m_editCellNum;
	MComboBox	m_wndVar1; // for PSC
	MComboBox	m_wndVar2;
	MComboBox	m_wndDB;
	CCobxAutoFill	m_wndSecondName;
	CCobxAutoFill	m_wndFirstName;
	MComboBox	m_cboType;
	MEdit	m_wndName;
	MEdit	m_wndID;
	MButton m_chkShearDeform;
	MButton m_chkWarpingEffect;
	MButton m_chkHumbly;
	MButton m_chkHumblyBefore;
	MButton m_chkHumblyAfter;
	MButton m_chkPSCDesign;
	CEditUnit m_edtPSCShear_I;
	CEditUnit m_edtPSCShear_J;
	CTextUnit m_untPSCShear;
	
	CBCGPStatic   m_wndFirstTitle[8];
	CEditUnit	m_wndFirstSize[8];
	CTextUnit m_wndFirstUnit[8];
	CBCGPStatic   m_wndSecondTitle[8];
	CEditUnit	m_wndSecondSize[8];
	CTextUnit m_wndSecondUnit[8];
	CEditUnit m_wndMatlValue[4];

	CEditUnit m_wndMatlValueCompo[2]; // Add by mylee 20070221
	CEditUnit m_edtMatPs; 
	CEditUnit m_edtMatPc;
	CEditUnit m_edtMatTstc;
	//CEditUnit	m_ESEC; 
	//CEditUnit	m_DSDC;
	CEditUnit m_edtMat1; 
	CEditUnit m_edtMat2;
	MButton m_chkMat;
	MComboBox	m_cmbPart;
	MComboBox	m_cmbBeforePart;
	
	int   m_nWarpingCheck;

	MButton m_chkMCellSym;

	CSpinButtonCtrl	m_spnMCellNoCell;
	CEditUnit	m_edtMCellNoCell;

	MButton m_chkMCellJ1;
	MButton m_chkMCellJ2;
	MButton m_chkMCellJr1;
	MButton m_chkMCellJr2;

	CArray<UINT, UINT> m_aRdoMCellLorR;
	CArray<UINT, UINT> m_aRdoMCellFShape;


	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemTap)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemTap)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeShape();
	afx_msg void OnChangeFirstName();
	afx_msg void OnChangeSecondName();
	afx_msg void OnChangeDB();
	afx_msg void OnChangeFirstSize();
	afx_msg void OnChangeSecondSize();
	afx_msg void OnChangeUseDB();
	afx_msg void OnChangeVar1();
	afx_msg void OnChangeVar2();
	afx_msg void OnSetfocusCmdSpIdName();
	afx_msg void OnUpdateCmdSpIdName();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnChangeCmdEditCellNum();
	afx_msg void OnDeltaposCmdSpinCellNum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdChangeOffsetBtn();
	afx_msg void OnCmdSelMaterialBtn();
	afx_msg void OnCmdMeshSizeChk2();
	afx_msg void OnChangeMaterial();
	afx_msg void OnCmdFemRdo();  
	afx_msg void OnMaterialBtn();
	afx_msg void OnCmdMultiModulusCheck();
	afx_msg void OnChangeCmdSpIdEsec();
	afx_msg void OnChangeCmdSpIdDsdc();
	afx_msg void OnChangeCmdSpIdPs();
	afx_msg void OnChangeCmdSpIdPc();
	afx_msg void OnChangeCmdSpIdTstc();
	afx_msg void OnComGenImportSPCI();
	afx_msg void OnComGenImportSPCJ();
	afx_msg void OnSelchangeCmdPartSel();
	afx_msg void OnCmdCalcSectPropIBtn();
	afx_msg void OnCmdCalcSectPropJBtn();
	afx_msg void OnChkWarpingEffect();
	afx_msg void OnChkhumbly();
	afx_msg void OnChkhumblyBefore();
	afx_msg void OnChkhumblyAfter();
	afx_msg void OnChkPSCDesign();
	afx_msg void OnCmdSectWapingCheckBtn();
	afx_msg void OnChangeMCellFShale();
	afx_msg void OnChangeMCellConnectSize();
	afx_msg void OnCmdSectWapingCheckUser();
	afx_msg void OnSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdSectChangeBtn();
	
	//}}AFX_MSG
	afx_msg LRESULT OnGridDataChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMTAP_H__673754E8_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
