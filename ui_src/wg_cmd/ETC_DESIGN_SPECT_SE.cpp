// ETC_DESIGN_SPECT_SE.cpp : implementation file
//
///////
#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_DESIGN_SPECT_SE.h"

#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\LockOption.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ETC_GDS_KISTEC2013.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

/////////////////////////////////////////////////////////////////////////////
// CETC_DESIGN_SPECT_SE dialog

enum DLG_INDEX { KISTEC2013=0, }; // 이거 DB생기면 그쪽으로 옮기자
enum EN_DESIGN_SPECTRA_SHOWMODE 
{
	EN_MODE_RSFUNC, EN_MODE_PO_FEMA440, EN_MODE_PO_ATC
};

CETC_DESIGN_SPECT_SE::CETC_DESIGN_SPECT_SE(T_SPFC_D* pParamData, BOOL bModify, int nCase/*=0*/, CWnd* pParent /*=NULL*/ )
	: CDialogMove(CETC_DESIGN_SPECT_SE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CETC_DESIGN_SPECT_SE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_bModify = bModify;
	m_mapChildDlg.RemoveAll();
	m_mapChildDlg.InitHashTable(11);

	m_pParamData = pParamData; 
	m_TempParamData = *m_pParamData;
	m_nShowMode = EN_MODE_RSFUNC; 
	m_pDampingWnd = nullptr;
	m_nCurrentDlg = 0;

#define M_DEFINE_RS_CODE(SEQ, NAME, DLG)    m_aCodeSeq.Add(SEQ); m_aCodeName.Add(NAME); m_mapChildDlg.SetAt(SEQ, DLG)
	M_DEFINE_RS_CODE(KISTEC2013, _T("KISTEC2013"), new CETC_GDS_KISTEC2013(pParamData));
#undef M_DEFINE_RS_CODE
}

CETC_DESIGN_SPECT_SE::~CETC_DESIGN_SPECT_SE()
{
	// Close message를 대비해서...
 	CMyChildDialog* dlg;
	int key;

	if(m_mapChildDlg.Lookup(m_nCurrentDlg,dlg))
		if(dlg->IsCreated())   dlg->End();

	POSITION pos = m_mapChildDlg.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos,key,dlg);
		delete dlg;
	}
	m_mapChildDlg.RemoveAll();
}

void CETC_DESIGN_SPECT_SE::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_DESIGN_SPECT_SE)
	DDX_Control(pDX, IDC_ETC_GDS_SPECTRUM_CMB, m_cmbDesignCode);
	//}}AFX_DATA_MAP
}


void CETC_DESIGN_SPECT_SE::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CETC_DESIGN_SPECT_SE::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SEME_D Data;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SEME_ADD):
			{
				UpdateCurDlgData();
			}
			break;
		default:
			break;
		}
	} // end of while
}

BEGIN_MESSAGE_MAP(CETC_DESIGN_SPECT_SE, CDialogMove)
	//{{AFX_MSG_MAP(CETC_DESIGN_SPECT_SE)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_SPECTRUM_CMB, OnSelchangeEtcGdsSpectrumCmb)
	ON_CBN_CLOSEUP(IDC_ETC_GDS_SPECTRUM_CMB, OnCloseupEtcGdsSpectrumCmb)
	ON_BN_CLICKED(IDC_CMD_METHOD_BTN, OnMethodBtn)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_DESIGN_SPECT_SE message handlers

BOOL CETC_DESIGN_SPECT_SE::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_strCodeName = _T("");
	m_strOldCodeName = _T("KISTEC2013");
	
	// Init Combo box
	for(int i = 0; i < m_aCodeName.GetSize(); i++) 
	{
		CDlgUtil::CobxAddItem(m_cmbDesignCode, m_aCodeName[i], i);
	}
	m_cmbDesignCode.SetCurSel(m_nCurrentDlg);

	POSITION pos = m_mapChildDlg.GetStartPosition();
	CMyChildDialog* dlg;
	int key;

	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos,key,dlg);
		dlg->m_parAccel = m_parAccel;
		dlg->m_parPeriod = m_parPeriod;
	}

	if(m_pParamData->arSpecFuncData.GetSize() < 1)
	{
		m_pParamData->ScaleFactor = 1.0;
		m_pParamData->Gravity = M_InitValueCurUnit(9.806, TON, M, CUnitCtrl::m_SPFC_UNIT.Gravity);
		m_pParamData->dDampingRatio = 0.05;
	}
	
	UpdateCurDlg();      // Data2Dlg
	
	// 이 값은 Seismic Evaluation Method 대화상자 값을 따라감
	m_cmbDesignCode.EnableWindow(FALSE);
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_DESIGN_SPECT_SE::OnOK() 
{
	int nSel = m_cmbDesignCode.GetCurSel();

	CMyChildDialog* dlg;
	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);

	int nCurCodeItem = m_cmbDesignCode.GetItemData(nSel);

	if( dlg->IsCreated() )
	{
		dlg->UpdateData(TRUE);
		if(!dlg->ISVALID()) return;
		dlg->End();
		if(m_nShowMode == EN_MODE_RSFUNC)
		{
			dlg->MakeSpectrumData();
			m_sDescript = dlg->m_sDescript;
			m_strFuncName = dlg->m_strFuncName;
		}
		else if(m_nShowMode) dlg->MakeSpectrumData(TRUE);
		else CalcCurrentPOSpectra();
		if (m_pDampingWnd) dlg->UpdateDampingWnd(m_pDampingWnd);    
	}

	dlg->Dlg2Data();

	CString strCodeName; m_cmbDesignCode.GetWindowText(strCodeName);
	m_pParamData->SpecFuncName = strCodeName; 
	m_pParamData->nSpecCodeType = GetCodeFromItem( m_cmbDesignCode.GetItemData(nSel) );

	// 이렇게 ok버튼을 누르면 itemdlg의 상태가 modify가 되도록 한다.
	//m_bSetParam은 변하지 않음.  

	m_TempParamData = *m_pParamData;

	CDialogMove::OnOK();
}

// 20071203 mylee - SPFC 코드 저장위해 변환하는 함수
int CETC_DESIGN_SPECT_SE::GetCodeFromItem(int nItem)
{
	if(nItem==KISTEC2013) return KISTEC2013;
	else ASSERT(0);

	return D_SPFC_CODE_UNDEFINED;
}

void CETC_DESIGN_SPECT_SE::CalcCurrentPOSpectra()
{
	CMyChildDialog* dlg;

	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);

	if(m_nShowMode==EN_MODE_PO_FEMA440)  //  FEMA440 계산방식
	{
		m_aPOPeriod[0].RemoveAll();
		m_aPOAccel[0].RemoveAll();

		dlg->m_parPeriod = &m_aPOPeriod[0];
		dlg->m_parAccel  = &m_aPOAccel[0];
		dlg->MakeSpectrumData(TRUE);
		for (int i=1; i<5; i++)
		{
			m_aPOPeriod[i].RemoveAll();
			m_aPOAccel[i].RemoveAll();
			m_aPOPeriod[i].Copy(m_aPOPeriod[0]);
			m_aPOAccel[i].Copy(m_aPOAccel[0]);

			double dB_betaeff = 4.0/(5.6 - log(m_dPODamping[i]));
			for (int j=0; j<m_aPOAccel[i].GetSize(); j++)
			{
				m_aPOAccel[i][j] = m_aPOAccel[i][j]/dB_betaeff;
			}
		}
	}
	else
	{
		for(int i=0; i < 5; i++)  // // fixed for PO dampings 수정하지 말것
		{
			m_aPOPeriod[i].RemoveAll();
			m_aPOAccel[i].RemoveAll();

			if(m_dPODamping[i] <= 0)
				continue;

			dlg->MakePOSpectrumData(m_dPODamping[i],-1,-1,m_aPOPeriod[i],m_aPOAccel[i]);
		}
	}
}

void CETC_DESIGN_SPECT_SE::CalcPOSpectrumSRaSRv(double SRa,double SRv,
																						 CArray <double,double>& aPeriod,
																						 CArray <double,double>& aPOAccel)
{
	CMyChildDialog* dlg;
	aPeriod.RemoveAll();
	aPOAccel.RemoveAll();
	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	dlg->MakePOSpectrumData(-1,SRa,SRv,aPeriod,aPOAccel);
}

void CETC_DESIGN_SPECT_SE::CalcPOSpectrumByDamping(double dDamping,CArray <double,double>& aPeriod,CArray <double,double>& aPOAccel)
{
	CMyChildDialog* dlg;
	aPeriod.RemoveAll();
	aPOAccel.RemoveAll();
	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	dlg->MakePOSpectrumData(dDamping,-1,-1,aPeriod,aPOAccel);
}

void CETC_DESIGN_SPECT_SE::OnSelchangeEtcGdsSpectrumCmb() 
{
	CMyChildDialog* dlg;
	CString strCurrentDlgSeq;
	int nOldCodeIndex = m_nCurrentDlg;

	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	dlg->UpdateData(TRUE);
	dlg->End();
	
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDesignCode, (DWORD&)m_nCurrentDlg);
	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);  
	m_Graph.KillComponentList();
	UpdateCurDlg();
}

void CETC_DESIGN_SPECT_SE::UpdateCurDlg()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(IDC_ETC_GDS_PLACEHOLDER);
	
	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);  
	dlg->CreateInit(pPlaceHolder);    
	dlg->UpdateData(FALSE);
	dlg->ShowWindow(SW_SHOW);

	AlignDialog();
}

void CETC_DESIGN_SPECT_SE::UpdateCurDlgData()
{
	CMyChildDialog* dlg;

	if(!m_mapChildDlg.Lookup(m_nCurrentDlg,dlg)) return;

	if(m_nCurrentDlg == KISTEC2013) ((CETC_GDS_KISTEC2013*)dlg)->UpdateDlgData();
}

void CETC_DESIGN_SPECT_SE::OnCloseupEtcGdsSpectrumCmb() 
{		
}

void CETC_DESIGN_SPECT_SE::OnMethodBtn() 
{		
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEISEVAL_METHOD,0));
}

void CETC_DESIGN_SPECT_SE::OnCancel() 
{
	CMyChildDialog* dlg;
	 
	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	if(dlg->IsCreated())
	{
		dlg->UpdateData(TRUE);
		dlg->End();
	}	

	int nSel = m_cmbDesignCode.GetCurSel();  
	int nCurCodeItem = m_cmbDesignCode.GetItemData(nSel);
	
	CDialogMove::OnCancel();
}

void CETC_DESIGN_SPECT_SE::SetShowMode(int nShowMode)
{
	m_nShowMode = nShowMode;
}

void CETC_DESIGN_SPECT_SE::PostNcDestroy() 
{
	CDialogMove::PostNcDestroy();
}

void CETC_DESIGN_SPECT_SE::OnClose() 
{
	CMyChildDialog* dlg;
	 
	m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	if(dlg->IsCreated())
	{
		dlg->UpdateData(TRUE);
		dlg->End();
	}	
	CDialogMove::OnClose();  
}

void CETC_DESIGN_SPECT_SE::AlignDialog()
{
}

void CETC_DESIGN_SPECT_SE::NewSpectralData()
{
	m_pParamData->arSpecFuncData.RemoveAll();
	m_Graph.KillData();
}

void CETC_DESIGN_SPECT_SE::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);	
}

void CETC_DESIGN_SPECT_SE::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}