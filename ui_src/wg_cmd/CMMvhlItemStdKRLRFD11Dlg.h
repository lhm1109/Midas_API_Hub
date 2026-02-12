#if !defined(__CMMVHLITEMSTDKRLRFD11DLG_H__)
#define __CMMVHLITEMSTDKRLRFD11DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "CMMvhlItemStdKRLSD15LoadGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdKRLRFD11Dlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdKRLRFD11Dlg : public CDialogMove
{
public:
	CCMMvhlItemStdKRLRFD11Dlg(CWnd* pParent = NULL);
	~CCMMvhlItemStdKRLRFD11Dlg();

	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_KRLRFD11 };

	MComboBox	m_cmbCodeName;
	CEditUnit   m_edtVehicleName;
	MComboBox	m_cmbVehicleType;  
	CEditUnit   m_edtDynaAllow;
	BOOL        m_bConvertDistLoad;
	int         m_nDgnLaneType;
	int         m_nLengthLane;
	CEditUnit   m_edt1stModel;
	CTextUnit   m_unt1stModel;
	CListCtrl   m_List;

	MButton	m_chkPatch;

public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:
	void InitCombo();
	void InitControls();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableCtrl();

	void SetHeaderTitle();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	void MakeItemEx();
	int  GetLoadCount();
	int  GetDistCount();

	void ChangeBitmap(int nBitmap);
	void Data2Dlg();
	BOOL Dlg2Data();  
	BOOL ApplyOrOK();
	BOOL ChangeData();
	
	void SetLoadTypeList(int nIndex);
	void  GetIndex(CString &csName, int *sitem=NULL);  

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdMvhlConvertDistLoadChk();
	afx_msg void OnCmdMvhlDgnLaneTypeRdo();
	afx_msg void OnCmdMvhlDgnLengthLaneRdo();
	afx_msg void OnChkPatch();
	afx_msg void OnCmdApply();
	DECLARE_MESSAGE_MAP()

protected:  
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	int       m_nStndCode;

	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer m_wndPicture;

	CArray<UINT, UINT> m_aDgnLaneType;
	CArray<UINT, UINT> m_aLengthLane;
	CArray<UINT, UINT> m_aEtc;
	CArray<UINT, UINT> m_aCtrlPatch;

	CCMMvhlItemStdKRLSD15LoadGrid m_Grid;
};

#include "HeaderPost.h"
#endif // !defined(__CMMVHLITEMSTDKRLRFD11DLG_H__)
