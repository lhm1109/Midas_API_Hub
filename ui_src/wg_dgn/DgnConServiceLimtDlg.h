#if !defined(AFX_DGNCONSERVICELIMTDLG_H__62F3DCBA_BBF3_408B_A5A4_315D2FDC46DF__INCLUDED_)
#define AFX_DGNCONSERVICELIMTDLG_H__62F3DCBA_BBF3_408B_A5A4_315D2FDC46DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Coded by Seungjun MNet:No.2393 ('20060829)
// DgnConServiceLimtDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

// Coded by Seungjun MNet:No.2393 ('20060915)
// For Eurocode2 - Exposure Class.
enum { X0=0, XC1, XC2, XC3, XC4, XD1, XD2, XD3, XS1, XS2, XS3, XF1, XF2, XF3, XF4, XA1, XA2, XA3, X_NUM};
enum {IRC_Moderate=101,IRC_Severe=102,IRC_Very_Severe=103,IRC_Extreme=104};
using namespace mit::frx;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConServiceLimtDlg dialog

class CDgnConServiceLimtDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:


protected:
	int m_iCode;
	int m_iSubCode;
	int m_bIsCsgDesign;
	int m_bSlabService;

// Construction
public:
	CDgnConServiceLimtDlg(BOOL bSlabServ=FALSE, CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConServiceLimtDlg)
	enum { IDD = IDD_DGN_CON_SERVICE_DLG };
	MComboBox	m_ctrEC2ExpComb;
	int       m_nOption;
	int       m_iCrackEnv;
	int       m_iDeflectEnv;
	CEditUnit    m_dUserCrack;
	CEditUnit    m_dCharCrack;
	CEditUnit    m_dFreqCrack;
	CEditUnit    m_dQuasCrack;
	double    m_dUserDeflect;
	CTextUnit m_dCrackCurUnit1;
	CTextUnit m_dCrackCurUnit2;
	CTextUnit m_dCrackCurUnit3;
	CTextUnit m_dCrackCurUnit4;
 	double    m_dk1;
	double    m_dk2;
	double    m_dk3;
	double    m_dk4;
	double    m_dRH;
	double	  m_dDAF;
	int		    m_iSeletType;
	double	m_dCharacLim;
	BOOL	m_bCharCrack;
	BOOL	m_bFreqCrack;
	BOOL	m_bQuasCrack;
	
	int       m_iServLife;
	MButton   m_RdoServLife100;      // National Annex : Sweden 에서만 사용.
	MButton   m_RdoServLife50;
	MButton   m_RdoServLife20;
	BOOL      m_bCorrosionHigh;
	MButton   m_RdoCorrosionHigh;
	MButton   m_RdoCorrosionLow;
	
	BOOL      m_bCrackW;
	CEditUnit m_edtCrackWLimit;
	CTextUnit m_untCrackWLimit;

	CEditUnit m_edtReductFactor;
	CTextUnit m_untReductFactor;
	MComboBox	m_cmbTimeDep;
	CEditUnit m_edtTimeDep;
	int       m_iDeflectEnv_Long;
	double    m_dUserDeflect_Long;
	double    m_dReductFactor;

	double m_dStrainLimit;
	double m_dStrainGrowthRate;
	double m_dK;
	//}}AFX_DATA


public:
	void Initial_SelectItem();
	void SetIsCpgDesign(BOOL bIsCpgDesign);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConServiceLimtDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL m_bShowChar, m_bShowQuas;

	CMap<int,int,int,int> m_arBeamData;   // Element 중 Concrete Beam의 Data 저장 

	CArray<UINT, UINT> m_arExpMoveCtrl;
	CArray<UINT, UINT> m_arStressMoveCtrl;
	CArray<UINT, UINT> m_arCrack1MoveCtrl;  // Eurocode2.
	CArray<UINT, UINT> m_arCrack2MoveCtrl;
	CArray<UINT, UINT> m_arCrack2UserMoveCtrl;
	CArray<UINT, UINT> m_arDeflectMoveCtrl;
	CArray<UINT, UINT> m_arDeflectUserMoveCtrl;
	CArray<UINT, UINT> m_arCharDeflectUserMoveCtrl;
	CArray<UINT, UINT> m_arBtnMoveCtrl;
	CArray<UINT, UINT> m_arSwedenMoveCtrl;
	CArray<UINT, UINT> m_arCrackWidth;  // Russia
	CArray<UINT, UINT> m_arLongDeflect;  // Russia
	CArray<UINT, UINT> m_arAIJ_WSD99_MoveCtrl;
	CArray<UINT, UINT> m_arServiceLifeCtrl;
	CArray<UINT, UINT> m_arCorrosionCtrl;

	void Initial_Unit();
	void Initial_Data();
	void EnableItems(BOOL bEnable);
	void Update_InitDlgItems();
	void UpdateBuffer();
	void ChangeDlgItemInfo();
	void AlignControls(int bWhich, int iEnvNum=0);
	void ResizeDlg();

	BOOL GetDesignCsgCodeNo();

	void GetCrackLimByEC2(int iExClass);
	void SetCrackcItem();
	void GetCrackLimByIS456();
	void GetCrackLimByIRC112();

	double SetCrackWidth_CorrosionLow(int ServLife, int iExClass);
	double SetCrackWidth_CorrosionHigh(int ServLife, int iExClass);

	BOOL IsCodeForMeshedDesignAIJ_JP();
	
	void Initial_StressFactor_EN04(int iIdx);
	
	// Generated message map functions
	//{{AFX_MSG(CDgnConServiceLimtDlg)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnConServAr();
	afx_msg void OnDgnConServDel();
	afx_msg void OnDgnConCrackUser();
	afx_msg void OnDgnConDeflectUser();
	afx_msg void OnDgnConCrackcCheck();
	afx_msg void OnSelchangeDgnConcExp();
	afx_msg void OnDgnConServLife();
	afx_msg void OnDgnConCorrosion();
	afx_msg void OnDgnCrackWidthChk();
	afx_msg void OnDgnConDeflectUserLong();
	afx_msg void OnSelChangeTimeCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONSERVICELIMTDLG_H__62F3DCBA_BBF3_408B_A5A4_315D2FDC46DF__INCLUDED_)
