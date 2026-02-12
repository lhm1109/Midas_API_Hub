#if !defined(__CSTLDBTMPDEFCODEPROVDLG_H__)
#define __CSTLDBTMPDEFCODEPROVDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CStldBtmpDefCodeProvDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "StldBtmpDefCodeProvUtil.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;
class CDBDoc;
class CStldBtmpDefCodeProvDlg: public CDialogMove
{
public:
	CStldBtmpDefCodeProvDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldBtmpDefCodeProvDlg();

	enum { IDD = IDD_TM_STLD_BTMP_DEF_CODE_PROV};

	BTMP_DEF_CODE_PROV_D& GetGenData() {return m_Data;}
	void SetDlgData(BTMP_DEF_CODE_PROV_D& data) { m_Data = data; }
protected: 
	// 함수(컨트럴)
	afx_msg void OnOk();
	afx_msg void OnSelchangeModelTypCbx();
	afx_msg void OnSelchangeTypeCbx();	

	afx_msg void OnClickGradRdo();	
	afx_msg void OnClickDumbRdo();

	afx_msg void OnClickAASHTO_SmalT_Rdo();	
	afx_msg void OnClickAASHTO_A_Rdo();	

	afx_msg void OnClickEuro_Procedure_Rdo();	
	afx_msg void OnClickEuro_H0_Rdo();	
	afx_msg void OnClickEuro_H1_Rdo();	
	afx_msg void OnClickEuro_H2_Rdo();	
	afx_msg void OnClickEuro_H3_Rdo();	
	afx_msg void OnClickEuro_H4_Rdo();	

	afx_msg void OnClickAS_SmalT_Rdo();	
// 	afx_msg void OnClickAS_H1_Rdo();	
// 	afx_msg void OnClickAS_H2_Rdo();	

	DECLARE_MESSAGE_MAP()

	// 함수(상속)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// 함수(일반)
	void SetControl();
	void ResetSectTypeContents();
	void SetInitData();

	BOOL CheckData();
	BOOL CheckDataAASHTO();
	BOOL CheckDataEuro();
	BOOL CheckDataAs();

	BOOL Dlg2Data();
	BOOL Dlg2DataSnip();
	BOOL Dlg2DataJTG();
    BOOL Dlg2DataIRC6();
	BOOL Dlg2DataAASHTO();
	BOOL Dlg2DataEuro();
	BOOL Dlg2DataAS();
	void Data2Dlg();
	void Data2DlgSnip();
	void Data2DlgJTG();
    void Data2DlgIRC();
	void Data2DlgAASHTO();
	void Data2DlgAEuro();
	void Data2DlgAS();

	void SetData2Dlg(BOOL bCodeChange=FALSE);
	void SetData2DlgAASHTO(BOOL bCodeChange=FALSE);
	void SetData2DlgAEuro(BOOL bCodeChange=FALSE);
	void SetData2DlgAS(BOOL bCodeChange=FALSE);

	void ShowHideCtrl();
	void AlignCtrl();
	void ResizeDialog();
	void ChangeBitmap();

	// 변수(컨트럴)
	MComboBox m_cbxDefCode;
	MComboBox m_cbxSectType;

	// Snip,Sp
	CEditUnit m_edtSnipTMax;
	CArray<UINT, UINT> m_aRdoSnipTempGrad;
	
	// JTG
	CEditUnit m_edtJTGT1;
	//CTextUnit m_untJTGT1;
	CEditUnit m_edtJTGT2;
	//CTextUnit m_untJTGT2;
	CEditUnit m_edtJTGSmallT;
	CEditUnit m_edtJTGTDumb1;
	CEditUnit m_edtJTGTDumb2;

	CArray<UINT, UINT> m_aRdoDumbType;

	// AASHTO
	CEditUnit m_edtAASHTOT1;
	CEditUnit m_edtAASHTOT2;
	CEditUnit m_edtAASHTOT3;
	CEditUnit m_edtAASHTOTA;
	CEditUnit m_edtAASHTOTSmallT;
	CArray<UINT, UINT> m_aRdoAASHTOSmallT;
	CArray<UINT, UINT> m_aRdoAASHTO_A;

	// EURO
	CEditUnit m_edtEuroT1;
	CEditUnit m_edtEuroT2;
	CEditUnit m_edtEuroT3;
	CEditUnit m_edtEuroT4;
	CEditUnit m_edtEuroH0;
	CEditUnit m_edtEuroH1;
	CEditUnit m_edtEuroH2;
	CEditUnit m_edtEuroH3;
	CEditUnit m_edtEuroH4;
	CArray<UINT, UINT> m_aRdoEuroPro;
	CArray<UINT, UINT> m_aRdoEuroH0;
	CArray<UINT, UINT> m_aRdoEuroH1;
	CArray<UINT, UINT> m_aRdoEuroH2;
	CArray<UINT, UINT> m_aRdoEuroH3;
	CArray<UINT, UINT> m_aRdoEuroH4;

	// AS
	CEditUnit m_edtAsSmallT;
	CEditUnit m_edtAsT1;
	CEditUnit m_edtAsT2;
	CEditUnit m_edtAsH1;
	CEditUnit m_edtAsH2;
	CArray<UINT, UINT> m_aRdoAsSmallT;
	CArray<UINT, UINT> m_aRdoAsH1;
	CArray<UINT, UINT> m_aRdoAsH2;

	// control
	CArray<UINT, UINT> m_aControlsOrg;
	CArray<UINT, UINT> m_aControlsAASHTO;
	CArray<UINT, UINT> m_aControlsEuro;
	CArray<UINT, UINT> m_aControlsEuro_Proc;
	CArray<UINT, UINT> m_aControlsEuro_T1;
	CArray<UINT, UINT> m_aControlsEuro_T2;
	CArray<UINT, UINT> m_aControlsEuro_T3;
	CArray<UINT, UINT> m_aControlsEuro_T4;
	CArray<UINT, UINT> m_aControlsEuro_H0;
	CArray<UINT, UINT> m_aControlsEuro_H1;
	CArray<UINT, UINT> m_aControlsEuro_H2;
	CArray<UINT, UINT> m_aControlsEuro_H3;
	CArray<UINT, UINT> m_aControlsEuro_H4;

	CArray<UINT, UINT> m_aControlsAS;
	CArray<UINT, UINT> m_aControlsAS_T1;
	CArray<UINT, UINT> m_aControlsAS_T2;
	CArray<UINT, UINT> m_aControlsAS_Small_T;
	CArray<UINT, UINT> m_aControlsAS_H1;
	CArray<UINT, UINT> m_aControlsAS_H2;

	// 변수(일반)
	CDBDoc* m_pDoc;
	BTMP_DEF_CODE_PROV_D m_Data;
	CBitmap* m_pBitmap;
	MillustViewer m_mvPicture;


};

#endif // !defined(__CSTLDBTMPDEFCODEPROVDLG_H__)


