// CMGridImpactFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridImpactFactorDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGridImpactFactorDlg dialog


CCMGridImpactFactorDlg::CCMGridImpactFactorDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMGridImpactFactorDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMGridImpactFactorDlg)
	m_nOption = 0;
	m_nFactorType = 0;
	m_nTargetType = 0;
	//}}AFX_DATA_INIT
	m_arFactorType.Add(IDC_CMD_GIMP_TYPERDO1);
	m_arFactorType.Add(IDC_CMD_GIMP_TYPERDO2);
	m_arFactorType.Add(IDC_CMD_GIMP_TYPERDO3);
	m_arFactorType.Add(IDC_CMD_GIMP_TYPERDO4);
	m_arFactorType.Add(IDC_CMD_STATIC);
}


void CCMGridImpactFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridImpactFactorDlg)
	DDX_Radio(pDX, IDC_CMD_CONS_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_CMD_GIMP_TYPERDO1, m_nFactorType);
	DDX_Radio(pDX, IDC_CMD_TARGET_RDO1, m_nTargetType);
	DDX_Control(pDX, IDC_CMD_GIMP_UNIT, m_dSpanLengthUnit);
	DDX_Control(pDX, IDC_CMD_GIMP_LENGTH_EDT, m_dSpanLength);
	DDX_Control(pDX, IDC_CMD_GIMP_FACTOR_EDT, m_dImpactFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridImpactFactorDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMGridImpactFactorDlg)
	ON_BN_CLICKED(IDC_CMD_GIMP_TYPERDO1, OnTmGimpType)
	ON_BN_CLICKED(IDC_CMD_GIMP_TYPERDO2, OnTmGimpType)
	ON_BN_CLICKED(IDC_CMD_GIMP_TYPERDO3, OnTmGimpType)
	ON_BN_CLICKED(IDC_CMD_GIMP_TYPERDO4, OnTmGimpType)
	ON_BN_CLICKED(IDC_CMD_CONS_OPT_ADD, OnTmConsAddDelRdo)
	ON_BN_CLICKED(IDC_CMD_CONS_OPT_DELETE, OnTmConsAddDelRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridImpactFactorDlg message handlers

void CCMGridImpactFactorDlg::Data2Dlg(T_GIMP_D& data)
{
	CDlgUtil::CtrlRadioSetCheck(this, m_arFactorType, data.nFactorType);
	m_nFactorType = data.nFactorType;
	m_nTargetType = data.nTgtType;
	switch(data.nFactorType)
	{
		case 0:
		case 1:
			break;
		case 2:
			m_dSpanLength.SetEditUnit(data.dSpanLength);
			break;
		case 3:
			m_dImpactFactor.SetEditUnit(data.dImpactFactor);
			break;
	}

	UpdateData(FALSE);
}

void CCMGridImpactFactorDlg::Dlg2Data(T_GIMP_D& data)
{
	UpdateData();

	CDlgUtil::CtrlRadioGetCheck(this, m_arFactorType, data.nFactorType);
	data.nTgtType = m_nTargetType;
	switch(data.nFactorType)
	{
		case 0:
		case 1:
			break;
		case 2:
			data.dSpanLength = m_dSpanLength.GetEditValue();
			break;
		case 3:
			data.dImpactFactor = m_dImpactFactor.GetEditValue();
			break;
	}
}

void CCMGridImpactFactorDlg::OnTmGimpType()
{
	UpdateData(TRUE);
	m_dSpanLength.EnableWindow(m_nFactorType == 2);
	m_dImpactFactor.EnableWindow(m_nFactorType == 3);
}

void CCMGridImpactFactorDlg::OnTmExecute()
{  
	UpdateData();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
	CArray<UINT, UINT> aSelKey;
	
	if (m_nTargetType == 0)
		pIGM->GetSelectedElemKeyList(aSelKey);
	else
		pIGM->GetSelectedNodeKeyList(aSelKey);
	
	T_GIMP_D data;
	CArray<T_GIMP_D, T_GIMP_D&> aData;

	data.Initialize();
	Dlg2Data(data);

	int nSize = aSelKey.GetSize();
	if (nSize < 1)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___IMPACT_FACTOR_NO_SELECTED));
		return;
	}

	if (m_nOption == 0)
	{
		//Add
		for (int i=0; i<nSize; i++)
		{
			data.nTgtKey = aSelKey[i];
			aData.Add(data);
		}

		if (pDoc->m_pDataCtrl->AddGimp(aData))
			pIGM->UnselectAllElem();
	}
	else
	{
		//Delete
		T_GIMP_K key;
		CArray<T_GIMP_K, T_GIMP_K> raKey;
		for (int i=0; i<nSize; i++)
		{
			key = pDoc->m_pAttrCtrl->GetGimpKey(m_nTargetType, aSelKey[i]);
			raKey.Add(key);
		}
		if (pDoc->m_pDataCtrl->DelGimp(raKey))
			pIGM->UnselectAllElem();
	}
}

void CCMGridImpactFactorDlg::OnTmClose()
{
	CloseDlg();
}

BOOL CCMGridImpactFactorDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dSpanLength.EnableWindow(FALSE);
	m_dImpactFactor.EnableWindow(FALSE);
	m_dSpanLengthUnit.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	GetDlgItem(IDC_CMD_EXECUTE)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridImpactFactorDlg::Execute()
{
	OnTmExecute();
}

BOOL CCMGridImpactFactorDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) { ASSERT(0); return FALSE; }

	T_GIMP_D data;
	if (!pDoc->m_pAttrCtrl->GetGimp(key, data))
		data.Initialize();

	Data2Dlg(data);

	OnTmGimpType();

	return TRUE;
}

void CCMGridImpactFactorDlg::OnTmConsAddDelRdo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_arFactorType, m_nOption == 0);
	if (m_nOption == 1)
	{
		m_nFactorType = 0;
		UpdateData(FALSE);
		OnTmGimpType();
	}
}
