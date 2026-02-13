// DgnStlKRParamTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlKRParamTableDlg.h"

#include "DgnSteelKRGrid.h"

#include "..\wg_db\DgnDataCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlKRParamTableDlg dialog


CDgnStlKRParamTableDlg::CDgnStlKRParamTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlKRParamTableDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nElemCount = m_pDoc->m_pAttrCtrl->GetCountStdp();	
	m_Grid = new CDgnSteelKRGrid(m_nElemCount);

	//{{AFX_DATA_INIT(CDgnStlKRParamTableDlg)	
	//}}AFX_DATA_INIT
}


void CDgnStlKRParamTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlKRParamTableDlg)
	DDX_Control(pDX, IDC_DGN_GRID, *m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlKRParamTableDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnStlKRParamTableDlg)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlKRParamTableDlg message handlers


BOOL CDgnStlKRParamTableDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();



	// Init Grid
	m_Grid->Initialize();  

	DataToGrid();  
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnStlKRParamTableDlg::DestroyWindow() 
{
	m_arElemk.RemoveAll();
	m_arStdpD.RemoveAll();
	m_arLengD.RemoveAll();
	m_arKfacD.RemoveAll();

	return CDialogMove::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OnOK 관련 
//

BOOL CDgnStlKRParamTableDlg::CheckInputData()
{  
	return TRUE;
}


void CDgnStlKRParamTableDlg::OnCancel() 
{	
	CDialogMove::OnCancel();
}

void CDgnStlKRParamTableDlg::OnOK() 
{
	// Data Checking
	if(CheckInputData() == FALSE) return;

	// Save Data
	GridToData();

	if(m_pDoc->m_pDgnDataCtrl->SetSteelDgnParam_KR(m_arElemk, m_arStdpD, m_arLengD, m_arKfacD))
		CDialogMove::OnOK();
	else
		AfxMessageBox(_LS(IDS_DGN_INPUT_DATA_ERROR_KR), MB_OK);//_T("입력값이 올바르지 않습니다.")
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Grid 관련 
//

void CDgnStlKRParamTableDlg::DataToGrid()
{
	CString srtElem			= _T("");
	CString srtPart			= _T("");
	CString srtSectChk	= _T("");
	CString srtChkAxis	= _T("");
	CString srtLx				= _T("");
	CString srtLy				= _T("");
	CString srtLb				= _T("");
	CString srtky				= _T("");
	CString srtkz				= _T("");
	CString	srtDonotConsider = _T("");

	CArray<T_STDP_K, T_STDP_K> arStdpK;
	m_pDoc->m_pAttrCtrl->GetStdpKeyList(arStdpK);
	int arStdpKSize = arStdpK.GetSize();
	for(int i=0 ; i<arStdpKSize ; i++)
	{
		int nElemk = arStdpK[i];
		srtElem.Format(_T("%d"), nElemk);
		
		T_STDP_D stdpData;
		stdpData.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetStdp(nElemk, stdpData))
		{
			if(stdpData.bPosi[0] && stdpData.bPosi[1])
				srtPart = _T("i, j");
			else if(stdpData.bPosi[0] && !stdpData.bPosi[1])
				srtPart = _T("i");
			else if(!stdpData.bPosi[0] && stdpData.bPosi[1])
				srtPart = _T("j");
			else
				ASSERT(0);

			if(stdpData.iChkMethod == 0)
				srtSectChk = _LS(IDS_DGN_STEEL_CURVE_CHECK);//_T("휨검토")
			else if(stdpData.iChkMethod == 1)
				srtSectChk = _LS(IDS_DGN_STEEL_LANG_CHECK);//_T("축력검토")
			else if(stdpData.iChkMethod == 2)
				srtSectChk = _LS(IDS_DGN_STEEL_CURVE_LANG_CHECK);//_T("휨&축력검토")
			else
				ASSERT(0);

			if(stdpData.iChkAxial == 0)
				srtChkAxis = _T("y");
			else if(stdpData.iChkAxial == 1)
				srtChkAxis = _T("z");
			else if(stdpData.iChkAxial == 2)
				srtChkAxis = _T("y&z");
			else
				ASSERT(0);

		}
		else
			ASSERT(0);

		T_LENG_D lengData;
		lengData.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetLeng(nElemk, lengData))
		{
			srtLx.Format(_T("%f"), lengData.BLy);
			srtLy.Format(_T("%f"), lengData.BLz);
			srtDonotConsider.Format(_T("%d"), lengData.bNotUseBLu);			
			srtLb.Format(_T("%f"), lengData.BLu);
		}
		else
		{
			srtLx = _T("-");
			srtLy = _T("-");
			srtDonotConsider.Format(_T("%d"), lengData.bNotUseBLu);
			srtLb = _T("-");
		}

		T_KFAC_D kfacData;
		kfacData.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetKfac(nElemk, kfacData))
		{
			srtky.Format(_T("%f"), kfacData.BKy);
			srtkz.Format(_T("%f"), kfacData.BKz);
		}
		else
		{
			srtky = _T("-");
			srtkz = _T("-");
		}
	
		m_Grid->SetValueRange(CGXRange(i+1,0), srtElem);
		m_Grid->SetValueRange(CGXRange(i+1,1), srtPart);
		m_Grid->SetValueRange(CGXRange(i+1,2), srtSectChk);
		m_Grid->SetValueRange(CGXRange(i+1,3), srtChkAxis);
		m_Grid->SetValueRange(CGXRange(i+1,4), srtLx);
		m_Grid->SetValueRange(CGXRange(i+1,5), srtLy);
		m_Grid->SetValueRange(CGXRange(i+1,6), srtDonotConsider);
		m_Grid->SetValueRange(CGXRange(i+1,7), srtLb);
		m_Grid->SetValueRange(CGXRange(i+1,8), srtky);
		m_Grid->SetValueRange(CGXRange(i+1,9), srtkz);
		
	} 
}

void CDgnStlKRParamTableDlg::GridToData()
{
	m_arElemk.RemoveAll();
	m_arStdpD.RemoveAll();
	m_arLengD.RemoveAll();
	m_arKfacD.RemoveAll();
	//m_aBngrK.RemoveAll();
	//m_arEwsdD.RemoveAll();

	CString srtPart			= _T("");
	CString srtSectChk	= _T("");
	CString srtChkAxis	= _T("");
	CString srtLx				= _T("");
	CString srtLy				= _T("");
	CString srtLb				= _T("");
	CString srtky				= _T("");
	CString srtkz				= _T("");
	CString srtDonotConsider = _T("");

	CArray<T_STDP_K, T_STDP_K> arStdpK;
	m_pDoc->m_pAttrCtrl->GetStdpKeyList(arStdpK);
	int arStdpKSize = arStdpK.GetSize();
	for(int i= 0 ; i<arStdpKSize ; i++)
	{		
		T_KEY elenK = _ttoi(m_Grid->GetValueRowCol(i+1, 0));
		m_arElemk.Add(elenK);

		srtPart			=	m_Grid->GetValueRowCol(i+1, 1);
		srtSectChk	= m_Grid->GetValueRowCol(i+1, 2);
		srtChkAxis	= m_Grid->GetValueRowCol(i+1, 3);
		srtLx				= m_Grid->GetValueRowCol(i+1, 4);
		srtLy				= m_Grid->GetValueRowCol(i+1, 5);
		srtDonotConsider = m_Grid->GetValueRowCol(i+1, 6);
		srtLb				= m_Grid->GetValueRowCol(i+1, 7);
		srtky				= m_Grid->GetValueRowCol(i+1, 8);
		srtkz				= m_Grid->GetValueRowCol(i+1, 9);
		

		T_STDP_D stdpData;
		stdpData.Initialize();
		if(srtPart == _T("i, j"))
		{
			stdpData.bPosi[0] = TRUE;
			stdpData.bPosi[1] = TRUE;
		}
		else  if(srtPart == _T("i"))
		{
			stdpData.bPosi[0] = TRUE;
			stdpData.bPosi[1] = FALSE;
		}
		else if(srtPart == _T("j"))
		{
			stdpData.bPosi[0] = FALSE;
			stdpData.bPosi[1] = TRUE;
		}

		if(srtSectChk == _LS(IDS_DGN_STEEL_CURVE_CHECK))
			stdpData.iChkMethod = 0;
		else if(srtSectChk == _LS(IDS_DGN_STEEL_LANG_CHECK))
			stdpData.iChkMethod = 1;
		else if(srtSectChk == _LS(IDS_DGN_STEEL_CURVE_LANG_CHECK))
			stdpData.iChkMethod = 2;

		if(srtChkAxis == _T("y"))
			stdpData.iChkAxial = 0;
		else if(srtChkAxis == _T("z"))
			stdpData.iChkAxial = 1;
		else if(srtChkAxis == _T("y&z"))
			stdpData.iChkAxial = 2;

		m_arStdpD.Add(stdpData);

		T_LENG_D lengData;
		lengData.Initialize();
		if(srtLx != _T("-"))
			lengData.BLy = _tstof(srtLx);
		if(srtLy != _T("-"))
			lengData.BLz = _tstof(srtLy);
		lengData.bNotUseBLu = _ttoi(srtDonotConsider);		
		if(srtLb != _T("-"))
			lengData.BLu = _tstof(srtLb);		
		
		m_arLengD.Add(lengData);

		T_KFAC_D kfacData;
		kfacData.Initialize();
		if(srtky != _T("-"))
			kfacData.BKy = _tstof(srtky);
		if(srtkz != _T("-"))
			kfacData.BKz = _tstof(srtkz);		
		m_arKfacD.Add(kfacData);		
	}
}
