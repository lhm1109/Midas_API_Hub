// CMGridElemStructTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridElemStructTypeDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ElemType    0
/////////////////////////////////////////////////////////////////////////////
// CCMGridElemStructTypeDlg dialog


CCMGridElemStructTypeDlg::CCMGridElemStructTypeDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMGridElemStructTypeDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMGridElemStructTypeDlg)
	m_nOption = 0;
	m_nStructType = 0;
	//}}AFX_DATA_INIT
	m_aRadio.Add(IDC_CMD_STRUCTTYPE_RDO1);
	m_aRadio.Add(IDC_CMD_STRUCTTYPE_RDO2);
	m_aRadio.Add(IDC_CMD_STRUCTTYPE_RDO3);
	m_aRadio.Add(IDC_CMD_STRUCTTYPE_RDO4);
}


void CCMGridElemStructTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridElemStructTypeDlg)
	DDX_Radio(pDX, IDC_CMD_CONS_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_CMD_STRUCTTYPE_RDO1, m_nStructType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridElemStructTypeDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMGridElemStructTypeDlg)
	ON_BN_CLICKED(IDC_CMD_CONS_OPT_ADD, OnCmdRadio)
	ON_BN_CLICKED(IDC_CMD_CONS_OPT_DELETE, OnCmdRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridElemStructTypeDlg message handlers

void CCMGridElemStructTypeDlg::OnTmExecute()
{
	UpdateData();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
	pIGM->GetSelectedElemKeyList(aSelKey);

	T_GSTY_D data;
	CArray<T_GSTY_D, T_GSTY_D&> aData;

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

		if (pDoc->m_pDataCtrl->AddGsty(aData))
			pIGM->UnselectAllElem();
	}
	else
	{
		//Delete
		T_GSTY_K key;
		CArray<T_GSTY_K, T_GSTY_K> raKey;
		for (int i=0; i<nSize; i++)
		{
			key = pDoc->m_pAttrCtrl->GetGstyKey(ElemType, aSelKey[i]);
			if (key != 0) raKey.Add(key);
		}
		if (raKey.GetSize() > 0)
		{
			if (pDoc->m_pDataCtrl->DelGsty(raKey))
				pIGM->UnselectAllElem();
		}
	}
}

void CCMGridElemStructTypeDlg::OnTmClose()
{
	CloseDlg();
}

void CCMGridElemStructTypeDlg::Data2Dlg(T_GSTY_D& data)
{
	m_nStructType = data.nElemType;
	UpdateData(FALSE);
}

void CCMGridElemStructTypeDlg::Dlg2Data(T_GSTY_D& data)
{
	UpdateData();
	data.nElemType = m_nStructType;
	data.nTgtType = ElemType;
}

BOOL CCMGridElemStructTypeDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) { ASSERT(0); return FALSE; }

	T_GSTY_D data;
	if (!pDoc->m_pAttrCtrl->GetGsty(key, data))
		data.Initialize();

	Data2Dlg(data);
	return TRUE;
}

void CCMGridElemStructTypeDlg::OnCmdRadio()
{
	UpdateData();
	CDlgUtil::CtrlEnableDisable(this, m_aRadio, m_nOption==0);
}
