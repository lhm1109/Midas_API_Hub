#if !defined(__CMDAUTOLOADCOMBCVLTMH7PAGE_H__)
#define __CMDAUTOLOADCOMBCVLTMH7PAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
//#include "..\wg_base\wg_base_DialogMove.h"
//
//#include "..\wg_db\wg_db_EditUnit.h"
//#include "..\wg_base\FoldGroupBox.h"


#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlTMH7Page dialog
enum LIST_IDX1 { IDX_NAME1 = 0, IDX_FACTOR1 };
class CCmdAutoLoadCombCvlDlg;
struct _LCOM_100_30_LC3;
class CCmdAutoLoadCombCvlTMH7Page : public CChildDialog
{
    // Construction
public:
    CCmdAutoLoadCombCvlTMH7Page(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    enum { IDD = IDD_CMD_LCOMBDEFAULT_TMH7_PAGE };

	MComboBox m_cmbBridgeType;
	int       m_nBD21D;
	int       m_nBD21DW;
    


    MComboBox m_cmbVEP;
    MComboBox m_cmbNVEP;
    int       m_nTMH7_Code_based;
    CEditUnit m_edtBD21D_Max;
    CEditUnit m_edtEPS_Max;
    CEditUnit m_edtBD21D_Min;
    int       m_nTMH7_User_Defined;
    CEditUnit m_edtBD21DW_Max;
    CEditUnit m_edtBD21DW_Min;
    int       m_nBD21DC;
    CEditUnit m_edtBD21DC_Max;
    CEditUnit m_edtBD21DC_Min;
    int		  m_nBD21EV;
    CEditUnit m_edtBD21EV_Max;
    CEditUnit m_edtBD21EV_Min;
    int       m_nBD21EH;
    CEditUnit m_edtBD21EH_Max;
    CEditUnit m_edtBD21EH_Min;
    int       m_nBD21ES;
    CEditUnit m_edtBD21ES_Max;
    CEditUnit m_edtBD21ES_Min;

    MComboBox m_MoveLcase;
    MComboBox m_BrkTrctv;
    MComboBox m_MoveType;
    CListCtrl m_MovingList;

	MComboBox m_cmbEffLoadCase;
	CListCtrl m_lstEffLoad;
	CArray<UINT, UINT> m_aEffLoadData;

    CEditUnit m_edtBD21Gamma_ULS;
    CEditUnit m_edtBD21Gamma_SLS;

    MButton   m_wndOthoChk;

	MButton	  m_LongBrkTrcChk;
	double    m_dEffLoadFactor;

	int       m_nOrthoCombType;  // 0 : Both(+-), 1: Combination Plus Only(++)
	CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&> m_a100_30_Load;

	double	  m_dUserDefDLFactor;
    double	  m_dUserDefDLFactor_Max;
    double	  m_dUserDefDLFactor_Min;
	double	  m_dUserDefSIDLFactor;
    double	  m_dUserDefSIDLFactor_Max;
    double	  m_dUserDefSIDLFactor_Min;

    CFoldGroupBox_Cross	m_chkLoadFactor;
	CFoldGroupBox_Cross m_chkLongBrkTrc;

	//For MVLD and its Type A,B,C
	CMap<T_MVLD_K, T_MVLD_K,int,int> m_aMvldType; // m_MovingList

	//For LongitudinalBraking Traction and its Factors
	CMap<T_STLD_K, T_STLD_K,double,double> m_aLongiBrakTra;//m_lstEffLoad
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCmdAutoLoadCombCvlAS51002017Page)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //	//}}AFX_VIRTUAL
    //
    //	// Implementation

public:
    void InitCombo();
    void SetDefault();

    void InitMVCombo();

    void InitMVListBox();
    BOOL CheckMovingCaseName(CString str,int& Index);
    void GetBD21LoadFactor();

    void Show_MoveComboBox(BOOL bCheck);

    void Initial_MVListBox();
    void OnCmdBD21Brdge();
    void OnCmdBD21MV();

    void OnCmdBD21EnableDisable();

    int m_nConstLoad;

    //
private:
	void SetListCtrlHeader(CListCtrl& list, const int nColNum, const CString* aTitle, const int* nColWidth);

protected:
    CDBDoc* m_pDoc;

    CCmdAutoLoadCombCvlDlg* m_pParent;
    //
    //	// Generated message map functions

	CArray<UINT, UINT> m_aCtrlCodeBased;
	CArray<UINT, UINT> m_aCtrlUserDefined;
	CArray<UINT, UINT> m_aCtrlLongBrkTra;

    virtual BOOL OnInitDialog();

    afx_msg void OnCmdBD21MVAddBtn();
    afx_msg void OnCmdBD21MVDelBtn();
    afx_msg void OnCmdBD21MVFactorBtn();
	afx_msg void OnCmdCodeOrUserRdo();
	afx_msg void OnCmdLongBrkTrc();

	afx_msg void OnEffLoadAddBtn();
	afx_msg void OnEffLoadModBtn();
	afx_msg void OnEffLoadDelBtn();

	afx_msg void OnCmdUserDefDL();
	afx_msg void OnCmdUserDefSIDL();

	afx_msg void OnCmdOthoChk();
	afx_msg void OnCmdSetOtholcBtn();

    afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
    //	//}}AFX_MSG
    //
    afx_msg void OnBnClickedLbTractionBtn();
    DECLARE_MESSAGE_MAP()

public:
    int m_nBD21BridgeType;

    int m_nBD21DNum;
    int m_nBD21DWNum;
    int m_nBD21DCNum;
    int m_nBD21EVNum;
    int m_nBD21EHNum;
    int m_nBD21BNum;

    double m_dBD21DMax;
    double m_dBD21DMin;
    double m_dBD21DWMax;
    double m_dBD21DWMin;
    double m_dBD21DCMax;
    double m_dBD21DCMin;
    double m_dBD21EHMax;
    double m_dBD21EHMin;
    double m_dBD21EVMax;
    double m_dBD21EVMin;
	double m_dBD21BMax;
	double m_dBD21BMin;

    double m_dBD21GF3ULS;
    double m_dBD21GF3SLS;
    
    int m_nBD21MoveCase;

    int m_nBD21MoveType;

    double m_dBD21MVFLULSC1;
    double m_dBD21MVFLSLSC1;
    double m_dBD21MVFLULSC23;
    double m_dBD21MVFLSLSC23;
	CString m_strMaxVal;
	CString m_strMinVal;
	CString m_strMaxVal2;
	CString m_strMinVal2;

    CArray<UINT, UINT> m_aCtrlBD21;

	int m_nCodeBasedOrUserDef;//0-Code Based;1-User Defined
	int m_nCodeBasedVerticalEarthPressure;//0-Method 1 ; 1 - Method 2& 3
	int m_nCodeBasedNonVerticalEarthPressure;//0-Approximate ; 1 - Accurate
	double m_dUserDefDL;//Max,Min,Both
    double m_dUserDefDL_Max;//Max,Min,Both
    double m_dUserDefDL_Min;//Max,Min,Both
	double m_dUserDefSIDL_Max;//Max,Min,Both
    double m_dUserDefSIDL_Min;//Max,Min,Both
	double m_dEPRelieving;//From Input Dlg
    int m_dUserRdo_Dmax;
    int m_dUserRdo_Dmin;
    int m_dUserRdo_Dboth;
    int m_dUserRdo_SIDLmax;
    int m_dUserRdo_SIDLmin;
    int m_dUserRdo_SIDLboth;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLBD21PAGE_H__)
