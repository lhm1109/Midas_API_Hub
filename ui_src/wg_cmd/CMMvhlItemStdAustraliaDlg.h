#if !defined(__CMMVHLITEMSTDAUSTRALIADLG_H__)
#define __CMMVHLITEMSTDAUSTRALIADLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdAustraliaDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdAustraliaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdAustraliaDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdAustraliaDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdAustraliaDlg();

//----------------------------------------------------------------------
// Implementation
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:

	void InitUnit();
	void InitCombo();
	void AlignControl();
	void ControlsShowHide();

	void SetHeaderTitle();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	void MakeItemEx();
	int  GetLoadCount();
	int  GetDistCount();

	void ChangeBitmap(int nIndex, int nBitmap);
	void Data2Dlg();
	BOOL Dlg2Data();  
	BOOL ApplyOrOK();
	BOOL ChangeData();
	
	void SetLoadTypeList(int nIndex);
	void  GetIndex(CString &csName, int *sitem=NULL);  
	void SetBDoubleFactor();

protected:  

	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;
	double m_dIncreLength;
	
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;
	
	CArray<UINT, UINT> m_aDynaAllow;   // Dynamic Load Allowance
	CArray<UINT, UINT> m_aDynaAllow2;  // Dynamic Load Allowance - 2
	CArray<UINT, UINT> m_aPicture;     // Picture
	CArray<UINT, UINT> m_aList;        // List
	CArray<UINT, UINT> m_aW;           // W
	CArray<UINT, UINT> m_aDW1DD1;      // dW1, dD1
	CArray<UINT, UINT> m_aFatigue;     // Fatigue
	CArray<UINT, UINT> m_aPedestrian;  // Pedestrian Live Load 관련 Controls
	CArray<UINT, UINT> m_aEtc;         // OK, Cancel, Apply
	CArray<UINT, UINT> m_aHeavy;       // Heavy Load
	CArray<UINT, UINT> m_aRoad;        // Road Load
	CArray<UINT, UINT> m_aRail;        // Rail Load
	CArray<UINT, UINT> m_aRailSub;     // Rail Load의 일부
	CArray<UINT, UINT> m_aBDouble;     // B_Double
	CArray<UINT, UINT> m_aLateralEcc;  // Lateral Eccentricity
public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdAustraliaDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_AUSTRALIA };
	
	MComboBox	m_cmbCodeName;
	CEditUnit m_edtVehicleName;
	MComboBox	m_cmbVehicleType;  
	CEditUnit m_edtDynaAllow;
	CEditUnit m_edtDynaAllow_Bend;
	CEditUnit m_edtDynaAllow_AllOther;
// 	CEditUnit m_edtDynaAllow_Single;
// 	CEditUnit m_edtDynaAllow_Other;

	CEditUnit m_edtW;
	CTextUnit m_untW;
	CEditUnit m_edtW1;
	CTextUnit m_untW1;
	CEditUnit m_edtD1;
	CTextUnit m_untD1;
	BOOL      m_bFatigue;

	CEditUnit m_edtPedW1;
	CTextUnit m_untPedW1;
	CEditUnit m_edtPedW3;
	CTextUnit m_untPedW3;
	
	CEditUnit m_edtPedA1;
	CTextUnit m_untPedA1;
	CEditUnit m_edtPedA2_min;
	CEditUnit m_edtPedA2_max;
	CTextUnit m_untPedA2;
	CEditUnit m_edtPedA3;
	CTextUnit m_untPedA3;

	CEditUnit m_edtHeavyP;
	CTextUnit m_untHeavyP;
	CEditUnit m_edtHeavyD;
	CTextUnit m_untHeavyD;
	CEditUnit m_edtRoadW1;
	CTextUnit m_untRoadW1;
	CEditUnit m_edtRoadW2;
	CTextUnit m_untRoadW2;
	CEditUnit m_edtRoadP;
	CTextUnit m_untRoadP;
	CEditUnit m_edtRailInc;
	CTextUnit m_untRailInc;

	CEditUnit m_edtLateralEcc;
	CTextUnit m_untLateralEcc;

	int m_nDistType;
	int m_nDLAType;
	CEditUnit m_edtDynaAllowDouble;

	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdAustraliaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdAustraliaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdDistTypeRdo();
	afx_msg void OnCmdDLATypeRdo();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMSTDAUSTRALIADLG_H__)
