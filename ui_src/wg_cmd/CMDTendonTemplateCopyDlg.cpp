// CMDTendonTemplateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMDTendonTemplateCopyDlg.h"
#include "CMDTendonTemplateDlg.h"
#include "DrawEditBtn_TndnTemplate.h"

#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMDTendonTemplateCopyDlg::CCMDTendonTemplateCopyDlg(CDgnTendonTemplateCtrl* pCtrl, CDrawEditBtn_TndnTemplate* pParentDraw, CWnd* pParent /* = NULL*/)
	:CCMDlgBase(CCMDTendonTemplateCopyDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pTemplateCtrl = pCtrl;
	m_pParentDlg = (CCMDTendonTemplateDlg*) pParent;
	m_pParentDraw = pParentDraw;
	m_pParentDraw->SetShowPreview(TRUE);
}

CCMDTendonTemplateCopyDlg::~CCMDTendonTemplateCopyDlg()
{
	if(m_pParentDlg)
	{
		m_pParentDlg->closeCopyDlg();
		m_pParentDlg->SetElemsCbxInit();
	}

	if(m_pParentDraw)
	{
		m_pParentDraw->SetShowPreview(FALSE);
		m_pParentDraw->MakeDrawUnit(FALSE);
	}
}

BEGIN_MESSAGE_MAP(CCMDTendonTemplateCopyDlg, CCMDlgBase)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_COPYDLG_RDO_COPY,				    OnChangeRdoButton)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_COPYDLG_RDO_MOVE,			    	OnChangeRdoButton)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_COPYDLG_OFFY,					      OnChangeInfo)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_COPYDLG_OFFZ,					      OnChangeInfo)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_COPYDLG_COPYNUM,				      OnChangeInfo)
	///ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TENDON_TEMPLATE_COPYDLG_SPIN_NUM, OnDeltaposCmdCopyNum)     //Span Num
END_MESSAGE_MAP()

void CCMDTendonTemplateCopyDlg::DoDataExchange( CDataExchange* pDX )
{
	CCMDlgBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_COPYDLG_RDO_COPY,	m_rdoCopy);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_COPYDLG_RDO_MOVE,	m_rdoMove);	
	//DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_COPYDLG_SPIN_NUM,	m_spnCopyNum);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_COPYDLG_OFFY,	  	m_edtOffY);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_COPYDLG_OFFZ,	  	m_edtOffZ);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_COPYDLG_OFFY_UNIT,	m_stcOffY);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_COPYDLG_OFFZ_UNIT,	m_stcOffZ);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_COPYDLG_COPYNUM,	  m_edtCopyNum);	  
}

void CCMDTendonTemplateCopyDlg::OnCancel()
{
	DestroyWindow();
}

void CCMDTendonTemplateCopyDlg::OnOK()
{
	if(!Dlg2Data())
		return;

	int i;
	int nCnt = (int)m_vecKeyList.size();
	int nSizeTndn;
	if(nCnt <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_TDNA));
		return;
	}

	if(m_bCopy)
	{
		m_pTemplateCtrl->CopyTemplateNU(m_vecKeyList, m_dOffsetZ, m_dOffsetY, m_nCopyNum, TRUE);
	}
	else
	{
		vecTndnTemp vecTndn;
		for(i = 0 ; i < nCnt; i++)
		{
			nSizeTndn = m_pTemplateCtrl->CopyTemplateNU(m_vecKeyList[i], m_dOffsetZ, m_dOffsetY, 1, vecTndn, FALSE);
			if(nSizeTndn == 1)
			{
				m_pTemplateCtrl->ModTemplateNU(m_vecKeyList[i], vecTndn[0]);
			}
		}

	}
	
	InitData(FALSE);

	m_pParentDlg->UpdateListDraw(FALSE);
}

BOOL CCMDTendonTemplateCopyDlg::OnInitDialog()
{
	CCMDlgBase::OnInitDialog();

	SetControl();

	InitData();
	
	return TRUE;
}

void CCMDTendonTemplateCopyDlg::SetControl()
{
	m_edtOffY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtOffZ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_stcOffY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_stcOffZ.SetUnitType(D_UNITSYS_BASE_LENGTH);  

	m_edtCopyNum.SetRange(1, 1000);
	m_edtCopyNum.SetInteger(TRUE);
}

BOOL CCMDTendonTemplateCopyDlg::SetPreviewDataDraw()
{	
	if(!Dlg2Data())
	{
		return FALSE;
	}

	int i;
	int nSizeKey = (int)m_vecKeyList.size();
	int nSizeTndn;
	
	vecTndnTemp vecTndn;
	vecTndnTemp vecTndnTmp;
	vecTndn.clear();

	for(i = 0 ; i < nSizeKey; i++)
	{
		nSizeTndn = m_pTemplateCtrl->CopyTemplateNU(m_vecKeyList[i], m_dOffsetZ, m_dOffsetY, m_nCopyNum, vecTndnTmp);

		if(nSizeTndn <= 0)
			return FALSE;

		vecTndn.insert(vecTndn.end(), vecTndnTmp.begin(), vecTndnTmp.end());
	}

	m_pParentDraw->SetPreviewData(vecTndn, FALSE);
	m_pParentDraw->SetSelectedLinkKeyListByTndnKey(m_vecKeyList);

	return TRUE;
}

BOOL CCMDTendonTemplateCopyDlg::Dlg2Data()
{
	m_pParentDlg->GetSelectedKeyList(m_vecKeyList);
	if(m_vecKeyList.size() <= 0)
		return FALSE;
	
	m_dOffsetZ = m_edtOffZ.GetEditValue();
	m_dOffsetY = m_edtOffY.GetEditValue();
	
	CUnitCtrl* pUnitCtrl = m_pDoc->m_pUnitCtrl;
	m_dOffsetZ = pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, m_dOffsetZ);
	m_dOffsetY = pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, m_dOffsetY);
	
	m_nCopyNum = m_edtCopyNum.GetEditValueInt();
	
	m_bCopy = m_rdoCopy.GetCheck();

	if(m_bCopy)
	{
		if(m_nCopyNum <= 0)		//Copy일때는 0이면 안되고,
			return FALSE;
	}							//Move일때는 nCopyNum이 의미없다.
	else
	{
		m_nCopyNum = 1;
	}

	return TRUE;
}

BOOL CCMDTendonTemplateCopyDlg::Data2Dlg()
{
	m_edtOffY.SetEditUnit(m_dOffsetY, 0);
	m_edtOffZ.SetEditUnit(m_dOffsetZ, 0);

	if(m_bCopy)
	{
		m_rdoCopy.SetCheck(TRUE);
		m_rdoMove.SetCheck(FALSE);
	}
	else
	{
		m_rdoCopy.SetCheck(FALSE);
		m_rdoMove.SetCheck(TRUE);
	}

	OnChangeRdoButton();

	m_edtCopyNum.SetValue(m_nCopyNum);

	return TRUE;
}

void CCMDTendonTemplateCopyDlg::OnChangeRdoButton()
{
	BOOL bCopy = m_rdoCopy.GetCheck();
	BOOL bMove = m_rdoMove.GetCheck();

	if(bCopy != bMove)
	{
		if(bCopy)
		{
			//m_spnCopyNum.EnableWindow(TRUE);
			m_edtCopyNum.EnableWindow(TRUE);
		}
		else
		{
			//m_spnCopyNum.EnableWindow(FALSE);
			m_edtCopyNum.EnableWindow(FALSE);
		}
	}
	else
	{
		ASSERT(0);
		m_rdoCopy.SetCheck(TRUE);
		m_rdoMove.SetCheck(FALSE);
	}

	SetPreviewDataDraw();
}

void CCMDTendonTemplateCopyDlg::OnChangeInfo()
{
	SetPreviewDataDraw();
}

void CCMDTendonTemplateCopyDlg::OnDeltaposCmdCopyNum( NMHDR* pNMHDR, LRESULT* pResult )
{	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_CMD_TENDON_TEMPLATE_COPYDLG_COPYNUM, 1, 1000, pNMHDR, pResult);
	*pResult = 0;	
	
	SetPreviewDataDraw();
}

void CCMDTendonTemplateCopyDlg::InitData(BOOL bType)
{
	m_vecKeyList.clear();
	m_dOffsetY = 0.0;
	m_dOffsetZ = 0.0;
	m_nCopyNum = 1;
	if(bType)
		m_bCopy = TRUE;

	Data2Dlg();
}
