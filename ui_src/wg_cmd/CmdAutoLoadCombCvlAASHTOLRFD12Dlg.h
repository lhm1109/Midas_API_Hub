#if !defined(__CMDAUTOLOADCOMBCVLAASHTOLRFD12DLG_H__)
#define __CMDAUTOLOADCOMBCVLAASHTOLRFD12DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlAASHTOLRFD12Dlg.h : header file
//
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAASHTOLRFD12Dlg dialog
enum LIST_IDX { IDX_NAME=0, IDX_FACTOR };

struct _LCOM_100_30_LC3;
struct _LCOM_GRAV_EFF_MV;
struct _LCOM_GRAV_EFF;
class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlAASHTOLRFD12Dlg : public CChildDialog
{
// Construction
public:
	CCmdAutoLoadCombCvlAASHTOLRFD12Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlAASHTOLRFD12Dlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_AASHTO_LRFD12_DLG };

	CEditUnit m_edtLrfd12Modifier;
	int       m_nLrfd12DC;
	int       m_nLrfd12DDType;
	int       m_nLrfd12DD1;
	int       m_nLrfd12DD2;
	int       m_nLrfd12DD3;
	int       m_nLrfd12DW;
	int       m_nLrfd12EHType;
	int       m_nLrfd12EH1;
	int       m_nLrfd12EH2;
	int       m_nLrfd12EVType;
	int       m_nLrfd12EV2;
	int       m_nLrfd12EV3;
	int       m_nLrfd12EV4;
	int       m_nLrfd12EV5;
	int       m_nLrfd12EV6;
	int       m_nLrfd12EV7;
	int       m_nLrfd12ES;
	mit::frx::MComboBox m_cmbPSCRSHType;
	CEditUnit m_edtLrfd12PS;
	CEditUnit m_edtLrfd12CRSH;
	CEditUnit m_edtLrfd12Settlement;
	CEditUnit m_edtLrfd16_GammaLL;
	BOOL  	  m_bLrfd12Culvert;
	int   	  m_nLrfd12Deformation;
	mit::frx::MButton    m_wndSeisChk;
	mit::frx::MButton   m_wndOthoChk;
    CListCtrl m_lstEffLoad;
	mit::frx::MComboBox m_cmbEffLoadCase;
    double    m_dEffLoadFactor;

	CFoldGroupBox_Cross	m_chkLoadFactor;
    //CFoldGroupBox_Cross	m_chkSeisLoad;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlAASHTOLRFD12Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitCombo();
	void GetAASHTOLRFD12LoadFactor();
	void OnCmdAASHTOLRFDEnableDisable(BOOL bAlignCtrl=TRUE);
	BOOL GetAASHTOLrfd16(){return m_bAASHTOLrfd16;}
	void SetAASHTOLrfd16(BOOL bAASHTOLrfd16){ m_bAASHTOLrfd16 = bAASHTOLrfd16;}
    BOOL GetAASHTOLrfd18(){return m_bAASHTOLrfd18;}
    void SetAASHTOLrfd18(BOOL bAASHTOLrfd18){ m_bAASHTOLrfd18 = bAASHTOLrfd18;}
    BOOL GetAASHTOLrfd20(){return m_bAASHTOLrfd20;}
    void SetAASHTOLrfd20(BOOL bAASHTOLrfd20){ m_bAASHTOLrfd20 = bAASHTOLrfd20;}
	BOOL GetAASHTOLrfd24() { return m_bAASHTOLrfd24; }
	void SetAASHTOLrfd24(BOOL bAASHTOLrfd24){ m_bAASHTOLrfd24 = bAASHTOLrfd24;}
	int m_nConstLoad;
    void Get_LiveLoadData(CArray<_LCOM_GRAV_EFF_MV, _LCOM_GRAV_EFF_MV&>& aEffectiveMVLoad);
    void Get_LiveLoadData(CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&>& aEffectiveLoad);

protected:
	CDBDoc* m_pDoc;
	CString m_strCodeName;

	CCmdAutoLoadCombCvlDlg* m_pParent;
	BOOL      m_bAASHTOLrfd16;
    BOOL      m_bAASHTOLrfd18;
	BOOL      m_bAASHTOLrfd20;
	BOOL      m_bAASHTOLrfd24;
	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlAASHTOLRFD12Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAASHTOLRFD12DDTypeRdo();
	afx_msg void OnCmdAASHTOLRFD12EHTypeRdo();
	afx_msg void OnCmdAASHTOLRFD12EVTypeRdo();
	afx_msg void OnSelchangeCmdAASHTOLRFD12PSCRSHCmb();
	afx_msg void OnCmdAASHTOLRFD12PSCRSHBtn();
    afx_msg void OnCmdSeisChk();
    afx_msg void OnCmdOthoChk();
    afx_msg void OnCmdSetOtholcBtn();
    afx_msg void OnEffLoadAddBtn();
    afx_msg void OnEffLoadModBtn();
    afx_msg void OnEffLoadDelBtn();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:    
    void AlignControl();
    void OnCmdAASHTOLRFD12EnableDisable();

    void SetListCtrlHeader(CListCtrl& list, const int nColNum, const CString* aTitle, const int* nColWidth);
    void InitMVCombo();
    BOOL CheckMovingCaseName(CString str, int& Index);
    BOOL IsEff_MV_Type(CString str);
	BOOL IsEff_LL_Type(CString str);
    
private:
    CStringArray m_arrEff_MVData;
    CStringArray m_arrEff_LLData;

public:
	int m_nAshLr12DcType;
	int m_nAshLr12DdType;
	int m_nAshLr12DwType;
	int m_nAshLr12EhType;
	int m_nAshLr12ElType;
	int m_nAshLr12EvType;
	int m_nAshLr12EsType;
	int m_nAshLr12PsType;
	int m_nAshLr12CrType;
	int m_nAshLr12ShType;
	double m_dAshLr12Dc1;
	double m_dAshLr12Dc2;
	double m_dAshLr12Dd1;
	double m_dAshLr12Dd2;
	double m_dAshLr12Dw1;
	double m_dAshLr12Dw2;
	double m_dAshLr12Eh1;
	double m_dAshLr12Eh2;
	double m_dAshLr12El1;
	double m_dAshLr12El2;
	double m_dAshLr12Ev1;
	double m_dAshLr12Ev2;
	double m_dAshLr12Es1;
	double m_dAshLr12Es2;
	double m_dAshLr12Ps1;
	double m_dAshLr12Ps2;
	double m_dAshLr12Cr1;
	double m_dAshLr12Cr2;
	double m_dAshLr12Sh1;
	double m_dAshLr12Sh2;
	double m_dFacGammaLL; //AASHTO LRFD 2016, Load factor for live load

    int       m_nOrthoCombType;  // 0 : Both(+-), 1: Combination Plus Only(++)
    CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&> m_a100_30_Load;        
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLAASHTOLRFD12DLG_H__)
