#if !defined(__CMMVHLITEMSTDINDIADLG_H__)
#define __CMMVHLITEMSTDINDIADLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdIndiaDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdIndiaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdIndiaDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemStdIndiaDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvhlItemStdIndiaDlg();

//----------------------------------------------------------------------
// Implementation
public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:
	void InitControl();
	void InitUnit();
	void InitCombo();
	void ChangeCodeCombo();
	void ChangeVehlTypeCombo();
	void ChangeText();

	void AlignControl();
	void ControlsShowHide();

	void GetIndex(int nCodeType, int &nVehlType, int &nSelVehi);  
	void ChangeBitmap(int nCodeType, int nVehlType, int nSelVehi);

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nListType, int nPos, int nNo, double dLoad, double dDist);
	int  GetLoadCount(int nListType);
	int  GetDistCount(int nListType);

	void Data2Dlg();
	BOOL Dlg2Data();  
	void ChangeData();

	void GetIRSVehlLoadType(BOOL& bPointLoad, BOOL& bDistLoad);

	BOOL ApplyOrOK();
	
protected:
	
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;

	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;

	CArray<UINT, UINT> m_aNoseTail;
	CArray<UINT, UINT> m_aNoseTail70R;
	CArray<UINT, UINT> m_aSelVehicle;
	CArray<UINT, UINT> m_aPicture;
	CArray<UINT, UINT> m_aIRC;
	CArray<UINT, UINT> m_aIRS_Point1;
	CArray<UINT, UINT> m_aIRS_Point2;
	CArray<UINT, UINT> m_aIRS_Dist;
	CArray<UINT, UINT> m_aFooyWay;
	CArray<UINT, UINT> m_aFooyPath;
	CArray<UINT, UINT> m_aLongiForce;
	CArray<UINT, UINT> m_aBrakeLOCORatio;
	CArray<UINT, UINT> m_aBrakeTrainRatio;
	CArray<UINT, UINT> m_aBrakeLOCO;
	CArray<UINT, UINT> m_aIRCFatigue;
	CArray<UINT, UINT> m_aOKCancel;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdIndiaDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_INDIA };

	MComboBox	m_cmbCodeName;
	CEditUnit m_edtVehicleName;
	MComboBox	m_cmbVehicleType;
	CEditUnit m_edtNoseTailDist;
	CTextUnit m_untNoseTailDist;
	CListCtrl	m_lstIRC;
	CEditUnit m_edtIRCdD1;
	CTextUnit m_untIRCdD1;
	CEditUnit m_edtIRCdD2;
	CTextUnit m_untIRCdD2;
	CEditUnit m_edtIRCP;
	CTextUnit m_untIRCP;
	CEditUnit m_edtIRCD;
	CTextUnit m_untIRCD;
	CEditUnit m_edtIRCPb;
	CTextUnit m_untIRCPb;
	CEditUnit m_edtIRCDb;
	CTextUnit m_untIRCDb;
	CEditUnit m_edtIRCW;
	CTextUnit m_untIRCW;
	CEditUnit m_edtIRCFW;
	CTextUnit m_untIRCFW;
	MComboBox	m_cmbSelVehicle;
	CListCtrl	m_lstIRS1;
	CListCtrl	m_lstIRS2;
	CListCtrl	m_lstI12T;
	CListCtrl	m_lstI40T;
	CEditUnit m_edtIRSdD12;
	CTextUnit m_untIRSdD12;
	CEditUnit m_edtIRSdD22;
	CTextUnit m_untIRSdD22;
	CEditUnit m_edtIRSdD3;
	CTextUnit m_untIRSdD3;
	CEditUnit m_edtIRSdW;
	CTextUnit m_untIRSdW;
	CEditUnit m_edtIRSdD11;
	CTextUnit m_untIRSdD11;
	CEditUnit m_edtIRSdD21;
	CTextUnit m_untIRSdD21;
	CEditUnit m_edtTractive;
	CTextUnit m_untTractive;
	CEditUnit m_edtBrakeLOCORatio;
	CEditUnit m_edtBrakeTrainRatio;
	CEditUnit m_edtBrakeLOCO;
	CTextUnit m_untBrakeLOCO;
	CEditUnit m_edtIRSL;
	CTextUnit m_untIRSL;
	CEditUnit m_edtIRSFW;
	CTextUnit m_untIRSFW;
	CEditUnit m_edtNoseTailDistW;
	CTextUnit m_untNoseTailDistW;
	CEditUnit m_edtNoseTailDistT;
	CTextUnit m_untNoseTailDistT;

	CEditUnit m_edtIRCMinD;
	CTextUnit m_untIRCMinD;
	CEditUnit m_edtRoughFactor;

	int       m_nFactorType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdIndiaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdIndiaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdMvhlSelVehlCmb();
	afx_msg void OnCmdMvhlApply();

	afx_msg void OnCmdFactorTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const;
	CString ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const;
	CString ConvSelVihicleStrRaw2Trans(const CString& strRaw) const;
	CString ConvSelVihicleStrTrans2Raw(const CString& strTrans) const;
	CString ConvVehicleNameStrRaw2Trans(const CString& strRaw) const;
	CString ConvVehicleNameStrTrans2Raw(const CString& strTrans) const;

	CString GetVehicleTypeStr() const;
	CString GetSelVihicleStr() const;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHLITEMSTDINDIADLG_H__)
