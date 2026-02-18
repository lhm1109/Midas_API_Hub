#if !defined(__DGNSTLCODEDLG_H__)
#define __DGNSTLCODEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlCodeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "wg_dgnRes.h"

#include "HeaderPre.h"

const int iDgn_StlID2 = 4;
const int iDgn_StlID3 = 5;

using namespace mit::frx;

class CDgnDataCtrl;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlCodeDlg dialog
class __MY_EXT_CLASS__ CDgnStlCodeDlg : public CDialogMove
{
// Construction
public:
	CDgnStlCodeDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDgnStlCodeDlg();

// Dialog Data
	//{{AFX_DATA(CDgnStlCodeDlg)
	enum { IDD = IDD_DGN_STL_CODE_DLG };
	MComboBox	m_DesignCode;
	MComboBox	m_DesignSubCode;
	MComboBox m_DesignAnnexAASHTO;
	BOOL	m_bNotUseBLu;
	BOOL  m_bCheckDeflect;
	BOOL	m_bSpecialEQ;
	BOOL  m_bStrongColmWeakBeam;
	int		m_iBDEnvelope;//add xuezc bending envelope(2018/3/30)GB50017-17
	int		m_iSafeLevel;
	int		m_iEqGrade;
	int   m_iWtrRatio;
	int   m_iBuckResist;    // Biaxial moments for buckling resistance
	int m_iRatioCalMethod;
	MComboBox m_cmbStructureType;
	MComboBox m_cmbStructImportCoef;//add by maxiao(2015-10-26)GB50017-15
	double m_dNetGrossRatio;
	int m_nSeismicGrade;
	BOOL m_bWeldBuch;
	BOOL m_bStifRib;
	double m_dStifRibSpace;

	double m_dBendModulus[2];	//0:y Dir  1:z Dir
	BOOL    m_bUseUGLcom4UGMemb;

	int   m_nInteractionKijType;		// define kij type.
	int   m_nLoadPositionMcr;			// Point of load application for Mcr
	BOOL m_bCombinedRatioLinearSum;	//consider EQ(6.2) or not.

	MComboBox m_cmbFrameType;
	MComboBox m_cmbNonSeisMemb;
	CEditUnit m_edtBehaviourQ;
	CEditUnit m_edtBehaviourOV;
	//}}AFX_DATA
	int m_iID2[iDgn_StlID2];
	int m_iID3[iDgn_StlID3];
	T_KEY m_GroupKey;

	BOOL      m_bBendingCoeff;
	BOOL      m_bWebBeam;
	BOOL      m_bWebColm;
protected:
	CDgnDataCtrl* m_pDgnDataCtrl;
	//The other dialog is the parent of this dialog. and the parent is responsible for passing in the modified data(T_DSTL_D* m_pDstlData).
	T_DSTL_D* m_pDstlData;
public:
	void Initial_ExistData();
	void Initial_Data();
	CRect MoveCtrl(int iKind, int* pID);
	void SelchangeDgnCtrldataDesigncode(BOOL bUpdateDef);
	void InitStructureTypeCombo();
	void InitStructureTypeComboIS();
	BOOL GetEnableDeflection(const CString& strDgnCode);
	//After setting this data, the data modified by this dialog box will not be added to the database
	void SetUserData(T_DSTL_D* pDstlData);
protected:
	BOOL IsAISCCodeSeries(const CString &strCodeName);
	BOOL IsSpecialEQCode(const CString &strCodeName);
	void OnCodeChange4CH(const CString &strCodeName);
	void SetGrupComboBox(MComboBox& cobx);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	int m_nFrameType;
	CArray<UINT,UINT> m_aSeisDgnParaCtrl;
	CArray<UINT,UINT> m_aWidthThickCtrl;
	CArray<UINT,UINT> m_aBuckResistCtrl;    // MNET:XXXX-HSSHIM-20110613
	CArray<UINT,UINT> m_aNationalAnnexCtrl;
	CArray<UINT,UINT> m_aAnnexAASHTOCtrls;
	CArray<UINT,UINT> m_aDefaultCtrl;
	CArray<UINT,UINT> m_aSafetyCtrl;
	CArray<UINT,UINT> m_aStrucSectCtrl;//add by maxiao(2015-10-26)GB50017-15
	CArray<UINT,UINT> m_aSeismiCtrl;
	CArray<UINT,UINT> m_aBeamBuckCrl;
	CArray<UINT,UINT> m_aBendingCtrl;
	CArray<UINT,UINT> m_aSmspCtrl;
	CArray<UINT,UINT> m_aBendModulusCtrl;
	CArray<UINT,UINT> m_aUGMemberCtrl;
	CArray<UINT,UINT> m_aEC3AddCtrl;
	CArray<UINT,UINT> m_aIRC24AddCtrl;
	CArray<UINT,UINT> m_aEC3SeisCtrl;
	CArray<UINT,UINT> m_aAIJASD02Ctrl;

	CArray<BOOL, BOOL> m_aCodeEnable;
	int m_nOldCodeIndex;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlCodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDgnCtrlDataAdvBtn();
	afx_msg void OnDgnStlctrlSpecial();
	afx_msg void OnSelchangeDgnCtrldataDesigncode();
	afx_msg void OnSelchangeDgnCtrldataNationalAnnex();
	afx_msg void OnSelchangeDgnCtrldataDesignAnnexAashto();
	afx_msg void OnDgnStlctrlStifRib();//add by maxiao(2015-10-26)GB50017-15
	afx_msg void OnBtnGrup();
	afx_msg void OnSetGrupCombo();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif 