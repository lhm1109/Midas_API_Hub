#if !defined(__CMMVHLITEMSTDPOLANDDLG_H__)
#define __CMMVHLITEMSTDPOLANDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdPolandDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlItemStdPolandDlg : public CDialogMove
{
public:
	CCMMvhlItemStdPolandDlg(CWnd* pParent = NULL);
	~CCMMvhlItemStdPolandDlg();

	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_POLAND };

	MComboBox	  m_cmbCodeName;  
	CEditUnit   m_edtVehicleName;
	MComboBox	  m_cmbVehicleType;
	MComboBox	  m_cmbSelVihicle;
	CListCtrl   m_List;
	CEditUnit   m_edtQ;
	CTextUnit   m_untQ;
	CEditUnit   m_edtA;
	CTextUnit   m_untA;
	MButton     m_chkDynamic;
	CEditUnit   m_edtPhi;
	CEditUnit   m_edtWheelSpacing1;
	CTextUnit   m_untWheelSpacing1;
	CEditUnit   m_edtNoseTailDist1;
	CTextUnit   m_untNoseTailDist1;
	CEditUnit   m_edtNumVehicle1;
	CEditUnit   m_edtTotalLoad;
	CTextUnit   m_untTotalLoad;
	CEditUnit   m_edtTrackedLength;
	CTextUnit   m_untTrackedLength;
	CEditUnit   m_edtWheelSpacing2;
	CTextUnit   m_untWheelSpacing2;
	CEditUnit   m_edtNoseTailDist2;
	CTextUnit   m_untNoseTailDist2;
	CEditUnit   m_edtNumVehicle2;	

public:
	void SetParamData(T_MVHL_D &ParamData) 
	{ m_csOldName = ParamData.VehicleLoadName; m_Data = ParamData; m_bModify = TRUE; }

public:
	void InitUnit();
	void InitCombo();
	void SetSelVehicleList();
	void AlignControl();
	void ControlsShowHide();
	void EnableDisableCtrl();

	void SetHeaderTitle();
	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	void MakeItemEx();
	int  GetLoadCount();
	int  GetDistCount();

	void ChangeBitmap();
	void Data2Dlg();
	BOOL Dlg2Data();  
	BOOL ChangeData();
	BOOL ApplyOrOK();

	void SetLoadTypeList(int nIndex);
	void GetIndex(CString &csName, int *sitem=NULL);  
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdMvhlCodeCmb();
	afx_msg void OnCmdMvhlTypeCmb();
	afx_msg void OnCmdSelVehicleCmb();
	afx_msg void OnCmdDynamicChk();
	afx_msg void OnCmdAutoUserRdo();
	afx_msg void OnCmdApply();
	DECLARE_MESSAGE_MAP()
	void SetCbxCurSelByStr(CComboBox& Cbx, CString str);

protected:  
	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;

	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer	m_wndPicture;

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
#endif // !defined(__CMMVHLITEMSTDPOLANDDLG_H__)
