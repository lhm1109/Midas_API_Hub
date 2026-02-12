#if !defined(__CMTLGTDLG_H__)
#define __CMTLGTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_base\MSExcel.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "CMGXGridWnd.h"
#include "CMTlgtDataStore.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTlgtDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMTlgtDlg : public CCMDlgBase
{
	// Construction
public:
	CCMTlgtDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMTlgtDlg();
	enum { IDD = IDD_ETC_TLGT_DLG };

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateUnit();

public:
	int				m_nSelectionMethod;
	CMouseEdit		m_wndPickingEdit;
	CTextUnit		m_2PointUnit1;
	CTextUnit		m_2PointUnit2;
	CMouseEdit		m_wnd2PointsEdit1;
	CMouseEdit		m_wnd2PointsEdit2;
	MEdit			m_wndNodeSelectEdit;

	CCMGXGridWnd	m_wndGrid;
	
	CSelectLC		m_cmbLoadCase;
	MEdit			m_edtName;

	MComboBox		m_cmbTrainCode;
	MComboBox		m_cmbTrainType;
	CEditUnit		m_edtNumWheels;
	CEditUnit		m_edtNumWVehicle;

	CEditUnit		m_edtSpeed;
	CString			m_untSpeed;
	int				m_nScale;
	CEditUnit		m_edtScale;
	CEditUnit		m_edtMax;

	CEditUnit		m_edtTime;
	CTextUnit		m_untTime;

	MComboBox		m_cmbDir;

	CListCtrl		m_List;

	CEditUnit		m_edtLength;
	CEditUnit		m_edtForce;

	int m_nTrainCode;
	int m_nTrainType;
	int m_nVehcleNum;

	BOOL bInit;

	CArray<UINT, UINT> m_aWheelCtrl;
	CArray<UINT, UINT> m_aVehiclCtrl;

	CArray<TLGT_ITEM, TLGT_ITEM&> m_aItem;
	CArray<TLGT_ITEM, TLGT_ITEM&> m_aUserItem;

	CMSExcel*        m_pXL;

protected:

	void SelectMethodMan();
	void SelectNode();
	void InitGrid();
	void SetGridTitle();
	void SetText();
	void SetControl();
	void InitItemDataVar();
	void ShowHideByCode();
	BOOL GetDistanceFromPrevNode(ROWCOL nRow, TLGT_NODE_ITEM& NodeItem);
	void Data2Dlg();
	BOOL Dlg2Data();

	void SetHeaderTitle(BOOL bInit);
	void OnUpdateList();
	void SetVehicleTypeList(int nCode);

	BOOL Grid_IsExistKey(int nKey);
	void Grid_InsertItem(int nPos, TLGT_NODE_ITEM &item);
	void InsertBaseItem(int nPos, int nType);
	BOOL GenerateData();
	BOOL ExcelImport(CString csFileName, CArray<double>& arValLength, CArray<double>& aValForce);
	void AutoAdjustComboWidth(CComboBox& Combo);

    void EnableTrainTypeOptBtn(BOOL bEnable);
    void SetFocusTrainTypeOptBtn(BOOL bFocus);
    void GetDataDefault(int nCode, int nType, int nVehcleNum, ARR_TLGT_ITEM& raUser, ARR_TLGT_ITEM& raItem);
    bool GetDataEUTrainTypeB(ARR_TLGT_ITEM& raItem);

    TRAIN_CODE GetCurTrainCode();
    TRAIN_TYPE GetCurTrainType();

protected:
	afx_msg void OnCmdBtnOk();
	afx_msg void OnCmdChangeDefineTracks();
	afx_msg void OnCmdChangeScaling();
	afx_msg void OnCmdOpAdd();
	afx_msg void OnCmdOpInsert();
	afx_msg void OnCmdOpDelete();
	afx_msg void OnCmdTrainloadAdd();
	afx_msg void OnCmdTrainloadModify();
	afx_msg void OnCmdTrainloadDelete();
	afx_msg void OnCmdTrainloadInsert();
	afx_msg void OnCmdOpenBtn();
	afx_msg void OnCmdSaveBtn();
	afx_msg void OnCmdPreviewBtn();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdChangedTrainCode();
	afx_msg void OnCmdChangedTrainType();
    afx_msg void OnCmdClickTrainTypeOptBtn();
	afx_msg void OnCmdVehicleBtn();
	afx_msg void OnCmdExcelImport();
	afx_msg void OnClickedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBtnClose();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc* m_pDoc;
	TLGT_INFO m_tlgtInfo;
	CArray<T_THFC_D, T_THFC_D&> arThfcData;
	CArray<T_THFC_K, T_THFC_K>  arThfcKey;
	CArray<T_THNL_D, T_THNL_D&> arThnlData;
	CArray<T_THNL_K, T_THNL_K>	arThnlKey;
	CCMTlgtDataStore* m_pDStore;

	CArray<UINT, UINT> m_aCtrlSelectNode;
	CArray<UINT, UINT> m_aCtrlVehicleType;
	CArray<UINT, UINT> m_aCtrlScale;
};


#include "HeaderPost.h"
#endif