#if !defined(AFX_CMSECTITEMCON_H__B6EB45C1_79E9_11D4_92DF_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMCON_H__B6EB45C1_79E9_11D4_92DF_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemCon.h : header file
//
#include "CMSectItemBase.h"
#include "CMSecViewWnd.h"
#include "CMSecInputGrid.h"
#include "CMSectItemGrid4CI.h"
#include "CMSectItemGrid.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxSect.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCon dialog
class CCMSectItmePSCView;
class CCMSectItemCon : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemCon(CWnd* pParent = NULL);   // standard constructor

	void SetRcIDAr();

//---------------------------------------------------
// Overrides
//---------------------------------------------------
public:
	BOOL OnOKPublic() override;
	BOOL OnApplyPublic() override;
	void ShowCalcResult() override;
	void DisplayOffsetPoint() override;
	void SetNewID(T_SECT_K nNewID) override;

//---------------------------------------------------
// Interface
//---------------------------------------------------
public:
	void AlignControls();
	void AlignControls_7thDOF();
	void SaveIData();
	void SaveBoxData();
	void SaveCIData();
	void SaveCTData();
	void SavePCData();
	void SaveGeneralData();
	void SaveTubData();
	BOOL SaveStlGirderBoxData();
	BOOL SaveStlGirderIData();
	BOOL SaveStlGirderTubData();
	void ShowBoxData2Dlg();
	// 거더 갯수 미 설정방지..  ^^;;;  Control간 연계관계를 이유로 별도 처리 , 기존 Code 유지 
	void PostShowData2Dlg(); 
	void ShowIData2Dlg();
	void ShowCIData2Dlg();
	void ShowCTData2Dlg();
	void ShowPCData2Dlg(BOOL& bHumblyShow);
	void ShowUserData2Dlg();
	void ShowGeneralData2Dlg();
	void ShowTubData2Dlg();
	void ChangeMatlData(T_MATL_CONCRETE &ConcrData, T_MATL_STEEL &SteelData);
	void ChangeMatlDataCICTPC(T_MATL_CONCRETE &ConcrDataSlab, T_MATL_CONCRETE &ConcrDataGirder);
	void EnableDisable7DOTbyStiffener();

	BOOL IsViewWindow();
	void CreatePSCView();
	void DestroyPSCView();
	BOOL ChangeBitmap(int nType);

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	BOOL SaveData();
	BOOL Dlg2Data();
	BOOL GetPeriAndStiff();
	void SetTempMembValWithCbxAndView();
	void HideNotUsedCtrl();
	void InitSpinCtrl();

	void InitSecTypeCombo();
	void InitControls();  
	void InitGrid();
	void InitUnit();
	void InitView();   
	void InitImage();

	void ShowKey2Dlg();
	void ShowData2Dlg();

	void SetSectionNameList();
	void SetGeneralPartList();
	void ChangeSectType();
	
	void SetOffsetData(T_SECT_SECTION_D* pData1, T_SECT_SECTION_D* pData2);

	void EnableDisableStlGirder();
	void SymmetricSectAutoCalc();
			
	void ShowStlGirderBoxData2Dlg();  
	void ShowStlGirderIData2Dlg();
	void ShowStlGirderTubData2Dlg();

	void ChangeCtrlSize(BOOL bBigMatlGrp);
	void SaveDynamicSizeCtrlPos();

	void SetSectionATypeCombo();
	int  GetSectionAType();
	BOOL CheckData();
protected:
	CArray<UINT, UINT> m_aCtrlBox, m_aCtrlUser, m_aCtrlI, m_aCtrlCI, m_aCtrlCT, m_aCtrlGirder, m_aCtrlMat, m_aCtrlMulti, m_aCtrlPC, m_aCtrlGeneral, m_aCtrlTub, m_aCtrlStlGirder;
	CArray<UINT, UINT> m_PreViewBox, m_PreViewI, m_PreViewUser, m_PreViewCI, m_PreViewCT, m_PreViewPC, m_PreViewGeneral, m_PreViewTub, m_PreViewStlGirderBox, m_PreViewStlGirderI, m_PreViewStlGirderTub;
	CArray<UINT, UINT> m_aCtrSlabInfo, m_aCtrSlabGROUP;
	CArray<UINT, UINT> m_aCtrlAType;

	CCMSecViewWnd m_wndSecViewFirst;
	CCMSecViewWnd m_wndSecViewSecond;
	CCMSecViewWnd m_wndSecViewBox;
	CCMSecViewWnd m_wndSecViewI;
	CCMSecViewWnd m_wndSecViewCI;
	CCMSecViewWnd m_wndSecViewCT;
	CCMSecViewWnd m_wndSecViewPC;
	CCMSecViewWnd m_wndSecViewGeneral;
	CCMSecViewWnd m_wndSecViewTub;
	CCMSecViewWnd m_wndSecViewStlGirderBox;
	CCMSecViewWnd m_wndSecViewStlGirderI;
	CCMSecViewWnd m_wndSecViewStlGirderTub;
	CCMSecInputGrid m_wndGridFirst;
	CCMSecInputGrid m_wndGridSecond;
	CCMSectItemGrid4CI m_wndGridCI;
	CCMSectItemGrid4CI m_wndGridCT;
	CCMSectItemGrid    m_wndGridGeneral;  
	CCMSectItmePSCView* m_pViewer;
	MillustViewer m_ImgViewerPREVIEW3;
	MillustViewer m_ImgViewerPREVIEW4;
	MillustViewer m_ImgViewerCI_PREVIEW1;
	MillustViewer m_ImgViewerCT_PREVIEW1;
	MillustViewer m_ImgViewerPSC_PREVIEW;
	MillustViewer m_ImgViewerPREVIEW7;
	MillustViewer m_ImgViewerTUB_PREVIEW1;
	MillustViewer m_ImgViewerB_GUIDE_BMP;
	MillustViewer m_ImgViewerI_GUIDE_BMP;
	MillustViewer m_ImgViewerTUB_GUIDE_BMP;




	DWORD m_nCurType; // 1 = Steel Box, 2 = Steel-I, 3 = Composite-I, 4 = Composite-T, 6 = Composite-T, 7 = Composite General, 8 = Steel Tub,
										// 9 = Comp Steel Girder Box, 10 = Comp Steel Girder I, 11 = Comp Steel Girder Tub
										// hslee : 이 변수값 T_SECT_D::nStype과 맞춰주는 리팩토링 필요함.. 단... 이 리팩토링을 하고 나면 Gen, Civil모두 테스트 해야 합니다.
	CStringArray m_aOffsetName;

	T_SECT_D  m_DataBox;
	T_SECT_D  m_DataI;
	T_SECT_D  m_DataCI;
	T_SECT_D  m_DataCT;
	T_SECT_D  m_DataUser;
	T_SECT_D  m_DataPC;
	T_SECT_K  m_PscValueSectK;
	T_SECT_D  m_DataGeneral;
	T_SECT_D  m_DataTub;
	T_SECT_D  m_DataStlGirderBox;
	T_SECT_D  m_DataStlGirderI;
	T_SECT_D  m_DataStlGirderTub;
	T_SECP_D  m_PSCDesign;
	
	BOOL m_bReCalc;

	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT>          m_aImportID;
	CArray<T_SECT_D, T_SECT_D&> m_aImportSect;
	UINT      m_CurSectID;

	CRect m_rectMatlGrpSize[2]; // 0 : SmallSize, 1 : BigSize

public:
// Dialog Data
	//{{AFX_DATA(CCMSectItemCon)
	enum { IDD = IDD_CMD_SECT_ITEM_CON };
	CBCGPStatic	m_txtOffset;
	MComboBox	m_cboSecType;
	CTextUnit	m_unit9;
	CTextUnit	m_unit8;
	CTextUnit	m_unit7;
	CTextUnit	m_unit6;
	CTextUnit	m_unit5;
	CTextUnit	m_unit4;
	CTextUnit	m_unit3;
	CTextUnit	m_unit2;
	CTextUnit	m_unit12;
	CTextUnit	m_unit11;
	CTextUnit	m_unit1;
	CTextUnit	m_untBf3;
	CTextUnit m_untStlGirderSlab;
	CTextUnit	m_untStlGirderBot;
	CTextUnit	m_untStlGirderB3;
	CTextUnit	m_untStlGirderB6;
	CTextUnit	m_untStlGirdert2;
	CTextUnit	m_untStlGirdertBf1;
	CTextUnit	m_untStlGirderBf2;
	CTextUnit	m_untStlGirderSWidth;
	CTextUnit	m_untStlGirderCTC;
	CEditUnit	m_tw;
	CEditUnit	m_Tr2;
	CEditUnit	m_Tr1;
	CEditUnit	m_Tf2;
	CEditUnit	m_Tf1;
	CEditUnit	m_Tc;
	CEditUnit	m_StlGirderTc;
	CEditUnit	m_swidth;
	CEditUnit	m_Hw;
	CEditUnit	m_Hr2;
	CEditUnit	m_Hr1;
	CEditUnit	m_Hh;
	CEditUnit	m_StlGirderHh;
	CEditUnit	m_ESEC;
	CEditUnit	m_DSDC;
	CEditUnit	m_PS;
	CEditUnit	m_PC;
	CEditUnit	m_TSTC;
	CEditUnit	m_CTC;
	CEditUnit	m_Bf2;
	CEditUnit m_Bf3;
	CEditUnit m_tfp;
	CEditUnit	m_Bc;
	CEditUnit	m_StlGirderBc;
	CEditUnit	m_Bf1;
	CEditUnit	m_B2;
	CEditUnit	m_B1;  
	CEditUnit	m_edtStlGirderSg;
	CEditUnit	m_edtStlGirderTop;
	CEditUnit	m_edtStlGirderBot;
	CEditUnit	m_edtStlGirderB1;
	CEditUnit	m_edtStlGirderB2;
	CEditUnit	m_edtStlGirderB3;
	CEditUnit	m_edtStlGirderB4;
	CEditUnit	m_edtStlGirderB5;
	CEditUnit	m_edtStlGirderB6;
	CEditUnit	m_edtStlGirderH ;
	CEditUnit	m_edtStlGirdert1;
	CEditUnit	m_edtStlGirdert2;
	CEditUnit	m_edtStlGirdertw1;
	CEditUnit	m_edtStlGirdertw2;
	CEditUnit	m_edtStlGirderbf1;
	CEditUnit	m_edtStlGirderbf2;
	CEditUnit m_edtStlGirdertfp;
	CEditUnit m_Sg;

	//CSpinButtonCtrl	m_gnum_spin;
	//CSpinButtonCtrl	m_N1_spin;
	//CSpinButtonCtrl	m_N2_spin;
	MComboBox	m_wndSecondName;
	MComboBox	m_wndFirstName;
	MComboBox	m_cmbPart;
	MComboBox	m_cmbBeforePart;
	MEdit	m_wndID;
	MEdit	m_wndName;
	CFormulaEditSpin		m_gnum;
	CFormulaEditSpin		m_N1;
	CFormulaEditSpin		m_N2;
	MButton m_chkShearDeform;
	MButton m_chkWarpingEffect;
	MButton m_chkHumbly;
	MButton m_chkHumblyBefore;
	MButton m_chkHumblyAfter;
	MButton m_chkPSCDesign;
	CEditUnit m_edtPSCShear;
	CTextUnit m_untPSCShear;
	CEditUnit m_edtMat1; // MNET:2582 20061129 mylee
	CEditUnit m_edtMat2;
	MButton m_chkMat;
	MEdit	    m_editPath;
	CCobxSect m_cmbPSC;
	BOOL      m_bSymAutoCalc;
	MComboBox m_cbxAType;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemCon)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemCon)
	virtual BOOL OnInitDialog();
	
	afx_msg void OnChangeFirstName();
	afx_msg void OnChangeSecondName();
	afx_msg void OnChangeCmdSpIdB1();
	afx_msg void OnChangeCmdSpIdB2();
	afx_msg void OnChangeCmdSpIdBc();
	afx_msg void OnChangeCmdSpIdBf1();
	afx_msg void OnChangeCmdSpIdBf2();
	afx_msg void OnChangeCmdSpIdBf3();
	afx_msg void OnChangeCmdSpIdtfp();
	afx_msg void OnChangeCmdSpIdHh();
	afx_msg void OnChangeCmdSpIdHr1();
	afx_msg void OnChangeCmdSpIdHr2();
	afx_msg void OnChangeCmdSpIdHw();
	afx_msg void OnChangeCmdSpIdTc();
	afx_msg void OnChangeCmdSpIdTf1();
	afx_msg void OnChangeCmdSpIdTf2();
	afx_msg void OnChangeCmdSpIdTr1();
	afx_msg void OnChangeCmdSpIdTr2();
	afx_msg void OnChangeCmdSpIdTw();
	afx_msg void OnChangeCmdN1();
	afx_msg void OnChangeCmdSpIdN2();
	afx_msg void OnMaterialBtn();
	afx_msg void OnSelchangeCmdSpSectypeCombo();
	afx_msg void OnSelchangePscCombo();
	afx_msg void OnCmdSectCentroidBtn();
	afx_msg void OnChangeCmdSpIdSwidth();
	afx_msg void OnChangeCmdSpIdEsec();
	afx_msg void OnChangeCmdSpIdDsdc();
	afx_msg void OnChangeCmdSpIdPs();
	afx_msg void OnChangeCmdSpIdPc();
	afx_msg void OnChangeCmdSpIdTstc();
	afx_msg void OnChangeCmdSpIdCtc();
	afx_msg void OnChangeCmdSpIdGnum();
	afx_msg void OnCmdChangeOffsetBtn();
	afx_msg void OnCmdMultiModulusCheck();
	afx_msg void OnCmdPSCImportBtn();
	afx_msg void OnCmdImportBtn();
	afx_msg void OnCmdCalcSectPropBtn();
	afx_msg void OnCmdSelectBtn();
	afx_msg void OnCmdFemRdo();
	afx_msg void OnSelchangeCmdPartSel();
	afx_msg void OnChkWarpingEffect();
	afx_msg void OnChkhumbly();
	afx_msg void OnChkhumblyBefore();
	afx_msg void OnChkhumblyAfter();
	afx_msg void OnChkPSCDesign();
	afx_msg void OnChangeCmdSpIdSg();
	
	afx_msg void OnCmdStiffenerBtn();

	afx_msg void OnChangeBc();
	afx_msg void OnChangetc();
	afx_msg void OnChangeHh();
	afx_msg void OnSymmetricAutoCalcBtn();
	afx_msg void OnChangeSg();
	afx_msg void OnChangeTop();
	afx_msg void OnChangeBot();
	afx_msg void OnChangeB1();
	afx_msg void OnChangeB2();
	afx_msg void OnChangeB3();
	afx_msg void OnChangeB4();
	afx_msg void OnChangeB5();
	afx_msg void OnChangeB6();
	afx_msg void OnChangeH();
	afx_msg void OnChanget1();
	afx_msg void OnChanget2();
	afx_msg void OnChangetw1();
	afx_msg void OnChangetw2();
	afx_msg void OnChangeBf1();
	afx_msg void OnChangeBf2();
	afx_msg void OnChangetfp();
	afx_msg void OnStiffenerBtn();

	afx_msg void OnChangeSelAType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMCON_H__B6EB45C1_79E9_11D4_92DF_0000C0B0E6B3__INCLUDED_)
