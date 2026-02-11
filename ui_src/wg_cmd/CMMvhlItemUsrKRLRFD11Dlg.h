#if !defined(__CMMVHLITEMUSRKRLRFD11DLG_H__)
#define __CMMVHLITEMUSRKRLRFD11DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrKRLRFD11Dlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "MvhlItemGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrKRLRFD11Dlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvhlItemUsrKRLRFD11Dlg : public CDialogMove
{
public:
	CCMMvhlItemUsrKRLRFD11Dlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_KRLRFD11 };

	int         m_nLoadType;

	CEditUnit   m_edtVehicleName;
	BOOL        m_bConvertDistLoad;
	CEditUnit	  m_edtTrkP;
	CEditUnit	  m_edtTrkD;
	CEditUnit	  m_edtTrkL;
	CListCtrl	  m_List;
	CEditUnit   m_edtLaneL1;
	CTextUnit   m_untLaneL1;
	CEditUnit   m_edtLaneW;
	CTextUnit   m_untLaneW;
	CEditUnit   m_edtLaneL2;
	CTextUnit   m_untLaneL2;
	CEditUnit   m_edtLaneW2;
	CEditUnit   m_edtLaneW2L;
	CEditUnit   m_edtLaneW2Exp;
	CTextUnit   m_untLaneW2;

	CEditUnit   m_edtTrainW1;
	CTextUnit   m_untTrainW1;
	CEditUnit   m_edtTrainD1;
	CTextUnit   m_untTrainD1;
	CEditUnit   m_edtTrainW2;
	CTextUnit   m_untTrainW2;
	CEditUnit   m_edtTrainD2;
	CTextUnit   m_untTrainD2;
	CMvhlItemGrid m_TrainGrid;

	MComboBox   m_cmbLaneNum;
	CEditUnit   m_edtLaneDist;
	CTextUnit   m_untLaneDist;
	CEditUnit   m_edtLoadW;
	CTextUnit   m_untLoadW;
	CEditUnit   m_edtLaneL;
	CTextUnit   m_untLaneL;

	CEditUnit   m_edtDynaAllow;
	int         m_nLengthLane;
	CEditUnit   m_edt1stModel;
	CTextUnit   m_unt1stModel;

	MButton	m_chkPatch;

public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

protected:
	void InitUnit();
	void InitCombo();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableCtrl();

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist, double dLength);
	BOOL DeleteItem(int nPos);
	BOOL ModifyItem(int nPos, int nNo, double dLoad, double dDist, double dLength);

	void ChangeBitMap();	

	void Data2Dlg();
	BOOL Dlg2Data();

	int  GetLoadCount();
	int  GetDistCount();
	BOOL GetLoadAndDistValue(double &dLoad, double &dDist, double &dLength);

	BOOL ApplyOrOK();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlLoadTypeRdo();
	afx_msg void OnCmdMvhlConvertDistLoadChk();
	afx_msg void OnCmdMlMvhlLaneNumCmb();
	afx_msg void OnCmdMvhlAddBtn();
	afx_msg void OnCmdMvhlInsBtn();
	afx_msg void OnCmdMvhlModBtn();
	afx_msg void OnCmdMvhlDelBtn();
	afx_msg void OnCmdMvhlApplyBtn();
	afx_msg void OnCmdMvhlChangeL1Edt();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdMvhlDgnLengthLaneRdo();
	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;

	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer  m_wndPicture;

	CArray<UINT, UINT> m_aTruck;
	CArray<UINT, UINT> m_aTrain;
	CArray<UINT, UINT> m_aLane;	
	CArray<UINT, UINT> m_aEtc;
};

#include "HeaderPost.h"
#endif // !defined(__CMMVHLITEMUSRKRLRFD11DLG_H__)
