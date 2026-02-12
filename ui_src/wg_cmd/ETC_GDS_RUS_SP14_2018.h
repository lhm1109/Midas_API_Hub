#pragma once
#include "MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\MIT_frx\MButton.h"

class CSpfcSP14_2018Util;
class CETC_GDS_RUS_SP14_2018 : public CMyChildDialog
{
public:
	CETC_GDS_RUS_SP14_2018(T_SPFC_D* pParamData, BOOL bModify, CWnd* pParent = nullptr);
	enum { IDD = IDD_ETC_GDS_RUS_SP14_2018 };

public:
	virtual void Data2Dlg();

public:
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit = TRUE);
	void Dlg2UtilData() override;
	void End();
	void EndNotClose() {};
	BOOL ISVALID();

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void InitCombo();
	void InitEditUnit();
	void GetFinalValue();
	bool Data2Dlg(const T_SPFC_RUS_SP14_2018& cData);
	bool Dlg2Data(T_SPFC_RUS_SP14_2018& rData);

private:
	BOOL m_bInit;
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcSP14_2018Util> m_pUtil;

private:
	CComboBox m_cmbSoil;
	CComboBox m_cmbK0;
	CComboBox m_cmbK1;
	CComboBox m_cmbKPsi;
	CEditUnit m_edtAcce;
	CTextUnit m_untAcce;
	mit::frx::MButton   m_chkNLDS;
	CEditUnit m_edtMaxT;
};
