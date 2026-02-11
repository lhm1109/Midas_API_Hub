// EQ_REC_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EQ_REC_DLG.h"
#include "..\wg_db\MITC_CommonTool.h"
#include "..\wg_db\EearthquakeWaveMgr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove
#define _EXTRAWAVE_ _T("_EXTRAWAVE_")
/////////////////////////////////////////////////////////////////////////////
// EQ_REC_DLG dialog


EQ_REC_DLG::EQ_REC_DLG(CWnd* pParent /*=NULL*/)
	: CDialog(EQ_REC_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(EQ_REC_DLG)
	m_fScaleFactor = 1.0f;
	m_fTimeScale = 1.0f;
	m_strSelectedEQ = _T("");
	m_strEarthquake = _T("");
	m_bGuanDong_CH = FALSE;
	//}}AFX_DATA_INIT
}


void EQ_REC_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EQ_REC_DLG)
	DDX_Control(pDX, IDC_ETC_cmbEARTHQUAKE, m_ctlCmbEarthquake);
	DDX_Text(pDX, IDC_ETC_edtSCALE, m_fScaleFactor);
	DDX_Text(pDX, IDC_ETC_edtTimeSCALE, m_fTimeScale);
	DDX_Text(pDX, IDC_ETC_edtSELECTED_EQ, m_strSelectedEQ);
	DDX_CBString(pDX, IDC_ETC_cmbEARTHQUAKE, m_strEarthquake);
	DDX_Check(pDX, IDC_ETC_EQ_GUANDONG_CHECK_CH, m_bGuanDong_CH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EQ_REC_DLG, CDialog)
	//{{AFX_MSG_MAP(EQ_REC_DLG)
	ON_CBN_SELCHANGE(IDC_ETC_cmbEARTHQUAKE, OnSelchangecmbEARTHQUAKE)
	ON_CBN_SELCHANGE(IDC_COMBO_WAVECH, OnSelchangecmbTg)
	ON_CBN_SELCHANGE(IDC_COMBO_EQ_WAVECH2, OnSelGDCate_CH)
	ON_CBN_SELCHANGE(IDC_COMBO_EQ_WAVECH3, OnSelGDTg_CH)
	ON_BN_CLICKED(IDC_ETC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ETC_EQ_GUANDONG_CHECK_CH, &EQ_REC_DLG::OnEqGuandongCheck_CH)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EQ_REC_DLG message handlers

BOOL EQ_REC_DLG::IsUseExtraWave()
{
#ifdef _MGEN_CH
	 return TRUE;   
#endif
#ifdef _CIVIL_CH
	 return TRUE;   
#endif
	 return FALSE;
}

BOOL EQ_REC_DLG::IsUseExtraWave_GEN()
{
#ifdef _MGEN_CH
	return TRUE;   
#endif
#ifdef _CIVIL_CH
	return TRUE;   
#endif

	return FALSE;
}

BOOL EQ_REC_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//
	AdjustUILayoutForChinese();
	GetDlgItem(IDC_STATIC_WAVECH_TG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_WAVECH)->ShowWindow(SW_HIDE);
	int nCmdShow =  IsUseExtraWave_GEN() ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_WG_CMD_STATIC10)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_WG_CMD_STATIC11)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_EQ_WAVECH2)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_COMBO_EQ_WAVECH3)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_ETC_EQ_GUANDONG_CHECK_CH)->ShowWindow(nCmdShow);
	if(IsUseExtraWave())
	{
		GetDlgItem(IDC_STATIC_WAVECH_TG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_WAVECH)->ShowWindow(SW_SHOW);
		if(nCmdShow)
		{
			GetDlgItem(IDC_WG_CMD_STATIC10)->EnableWindow(m_bGuanDong_CH);
			GetDlgItem(IDC_WG_CMD_STATIC11)->EnableWindow(m_bGuanDong_CH);
			GetDlgItem(IDC_COMBO_EQ_WAVECH2)->EnableWindow(m_bGuanDong_CH);
			GetDlgItem(IDC_COMBO_EQ_WAVECH3)->EnableWindow(m_bGuanDong_CH);
		}

		CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_COMBO_WAVECH);
		CString arTg[] ={_T("--"), _T("0.20"),_T("0.25"),_T("0.30"),_T("0.35"),_T("0.40"),_T("0.45"),_T("0.55"),_T("0.65"),_T("0.75"),_T("0.90")};
		for (int i=0; i<sizeof(arTg)/sizeof(CString); i++)
			pCombox->AddString(arTg[i]);
		pCombox->SetCurSel(0);
	}
	InitGuangdongWaveFolder();
	//
	InitialWaveCombx();  
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




void EQ_REC_DLG::OnSelchangecmbEARTHQUAKE() 
{
	m_ctlCmbEarthquake.GetLBText(m_ctlCmbEarthquake.GetCurSel(),m_strEarthquake);
		//
	m_strSelectedEQ = GetCurDescription();
	UpdateData(FALSE);
}

void EQ_REC_DLG::OnSelchangecmbTg()
{
	if(IsUseExtraWave_GEN() && m_bGuanDong_CH)
	{
		CStringArray str;
		CComboBox* pCombox2 = (CComboBox *)GetDlgItem(IDC_COMBO_EQ_WAVECH3);
		pCombox2->ResetContent();
		GetComboStrByIndex(2,str);
		for(int i = 0;i < str.GetSize();i++)
		{
			pCombox2->AddString(str[i]);
		}
		pCombox2->SetCurSel(0);//Default
		InitialWaveCombx_GD_CH();
	}
	else
		InitialWaveCombx();
}

BOOL EQ_REC_DLG::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


CString EQ_REC_DLG::GetFileName(LPCTSTR lpName)
{
	 CString file;
	 CString strEarthquake = lpName;
	if(strEarthquake==_T("1940, El Centro Site, 270 Deg")) 
	  file = _T("Elcent_h.dbs");
	else if(strEarthquake==_T("1940, El Centro Site, 180 Deg")) 
	  file = _T("Elcent_t.dbs");
	else if(strEarthquake==_T("1940, El Centro Site, Vertical")) 
	  file = _T("Elcent_v.dbs");
	else if(strEarthquake==_T("1952, Taft Lincoln School, 69 Deg")) 
	  file =_T("Taft_h.dbs");
	else if(strEarthquake==_T("1952, Taft Lincoln School, 339 Deg")) 
	  file = _T("Taft_t.dbs");
	else if(strEarthquake==_T("1952, Taft Lincoln School, Vertical")) 
	  file= _T("Taft_v.dbs");
	else if(strEarthquake==_T("1952, Hollywood Storage P.E., 270 Deg"))
	  file =_T("Holly_h.dbs");
	else if(strEarthquake==_T("1952, Hollywood Storage P.E., 0 Deg"))
	  file =_T("Holly_t.dbs");
	else if(strEarthquake==_T("1952, Hollywood Storage P.E., Vertical"))
	  file = _T("Holly_v.dbs");
	else if(strEarthquake==_T("1971, San Fernando, 69 Deg"))
	  file =_T("Sanfer_h.dbs");
	else if(strEarthquake==_T("1971, San Fernando, 159 Deg"))
	  file = _T("Sanfer_t.dbs");
	else if(strEarthquake==_T("1971, San Fernando, Down"))
	  file= _T("Sanfer_v.dbs");
	else if(strEarthquake==_T("1979, James RD. El Centro, 220 Deg"))
	  file =_T("James_h.dbs");
	else if(strEarthquake==_T("1979, James RD. El Centro, 310 Deg"))
	  file = _T("James_t.dbs");
	else if(strEarthquake==_T("1979, James RD. El Centro, Up"))
	  file= _T("James_v.dbs");
	else if(strEarthquake==_T("1985, Mexico City, Station 1, 180 Deg"))
	  file= _T("mexcit2.dbs");
	else if(strEarthquake==_T("1985, Mexico City, Station 1, 270 Deg"))
	  file= _T("mexcit1.dbs");
	else if(strEarthquake==_T("1994, Northridge, Sylmar County Hosp., 90 Deg"))
	  file= _T("nridge1.dbs");
	else if(strEarthquake==_T("1994, Northridge, Santa Monica, City Hall Grounds, 0 Deg"))
	  file= _T("s_monic2.dbs");
	else if(strEarthquake==_T("1994, Northridge, Santa Monica, City Hall Grounds, 90 Deg"))
	  file= _T("s_monic1.dbs");
	else if(strEarthquake==_T("1994, Northridge, Arleta and Nordhoff Fire Station, 90 Deg"))
	  file= _T("nridge3.dbs");
	else if(strEarthquake==_T("1989, Loma Prieta, Oakland Outer Wharf, 270 Deg"))
	  file= _T("oakwh1.dbs");
	else if(strEarthquake==_T("1989, Loma Prieta, Oakland Outer Wharf, 0 Deg"))
	  file= _T("oakwh2.dbs");
	else if(strEarthquake==_T("1971, San Fernando Pocoima Dam, 196 Deg"))
	  file= _T("pacoima1.dbs");
	else if(strEarthquake==_T("1971, San Fernando Pocoima Dam, 286 Deg"))
	  file= _T("pacoima2.dbs");
	else if(strEarthquake==_T("1966, Parkfield Cholame,Shandon, 40 Deg"))
	  file= _T("park040.dbs");
	else if(strEarthquake==_T("1966, Parkfield Cholame,Shandon, 130 Deg"))
	  file= _T("park130.dbs");
	else if(strEarthquake==_T("1971, San Fernando 8244 Orion Blvd., 90 Deg"))
	  file= _T("sanfern1.dbs");
	else if(strEarthquake==_T("1971, San Fernando 8244 Orion Blvd., 180 Deg"))
	  file= _T("sanfern2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY1))
		file = _T("org_wv1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY2))
		file = _T("org_wv2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY3))
		file = _T("org_wv3.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_MIYAGI_COAST_LG))
		file = _T("T1-I-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_MIYAGI_COAST_TR))
		file = _T("T1-I-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_COAST_LG))
		file = _T("T1-I-3.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYUGANADA_COAST_LG))
		file = _T("T1-II-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYUGANADA_COAST_TR))
		file = _T("T1-II-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_EASTCOAST_TR))
		file = _T("T1-II-3.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_NIHONKAI_CENTRAL_TR))
		file = _T("T1-III-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_NIHONKAI_CENTRAL_LG))
		file = _T("T1-III-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_EASTCOAST_LG))
		file = _T("T1-III-3.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS))
		file = _T("T2-I-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW))
		file = _T("T2-I-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS2))
		file = _T("T2-I-3.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS3))
		file = _T("T2-II-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW2))
		file = _T("T2-II-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_N30W))
		file = _T("T2-II-3.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_N12W))
		file = _T("T2-III-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS4))
		file = _T("T2-III-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW3))
		file = _T("T2-III-3.dbs");

	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW1))
		file = _T("H24_T1-I-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW1))
		file = _T("H24_T1-I-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS1))
		file = _T("H24_T1-I-3.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW2))
		file = _T("H24_T1-II-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW2))
		file = _T("H24_T1-II-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS2))
		file = _T("H24_T1-II-3.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW3))
		file = _T("H24_T1-III-1.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW3))
		file = _T("H24_T1-III-2.dbs");
	else if(strEarthquake==_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS3))
		file = _T("H24_T1-III-3.dbs");
	else 
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__ERROR__File_Not_Found));  
	  return _T("");
	}
	return file;
}
CString EQ_REC_DLG::GetFilePath(LPCTSTR lpFileName,BOOL bGuangDong_CH)
{
		TCHAR lpFilename[_MAX_PATH+1];
		GetModuleFileName(NULL,lpFilename,_MAX_PATH);

		TCHAR path_buffer[_MAX_PATH];
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];

		_tsplitpath( lpFilename, drive, dir, fname, ext );
		_tcscpy(path_buffer,drive);
		_tcscat(path_buffer,dir);
		_tcscat(path_buffer,_T("dbase\\"));
		if(bGuangDong_CH) _tcscat(path_buffer,_T("GuangDong\\"));
		_tcscat(path_buffer,lpFileName);

		CString strFullPath = path_buffer;
		return strFullPath;
}
void EQ_REC_DLG::GetFile()
{
	if(IsUseExtraWave_GEN() && m_mapCheckGuangDongWave.PLookup(m_strEarthquake))
	{
		CString strFolder,str[3];
		GetDlgItem(IDC_COMBO_EQ_WAVECH2)->GetWindowText(str[0]);
		if(str[0] == _T("I0&I1"))
			str[0] = _T("I");
		GetDlgItem(IDC_COMBO_WAVECH)->GetWindowText(str[1]);
		str[1] += _T("s");
		GetDlgItem(IDC_COMBO_EQ_WAVECH3)->GetWindowText(str[2]);
		strFolder.Format(_T("%s-%s-%s"),str[0],str[1],str[2]);
		//strFullPath.Format(_T("%s\\/*.dbs"), GetFilePath(strFolder,TRUE));
		FilePath = GetFilePath(strFolder,TRUE) + _T("\\") + m_strEarthquake + _T(".dbs");
		m_sFileTitle = m_strEarthquake;
		return;
	}
	if(IsExtraWave())
	{
		CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_COMBO_WAVECH);
		int nTgType = pCombox->GetCurSel();
		m_sDescript = m_strEarthquake;
		IEearthquakeWaveMgr::WAVE_TG waveTg = GetWaveTg(nTgType);
		FilePath = IEearthquakeWaveMgr::Inst()->GetFilePath(waveTg) + m_strEarthquake + _T(".dbs");
		m_sFileTitle = m_strEarthquake;
		int idx = m_strEarthquake.Find(_T(" "));
		if(idx > 0)
			m_sFileTitle = m_strEarthquake.Left(idx);
	}
	else
	{
		CString file = GetFileName(m_strEarthquake);
		m_sDescript = m_strEarthquake;
		FilePath=GetFilePath(file);
		m_sFileTitle = file;
		m_sFileTitle = m_sFileTitle.Left(m_sFileTitle.GetLength()-4);  
	}
}

void EQ_REC_DLG::OnButton1() 
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,_LS(IDS_WG_CMD__ADDD__SGS_dbase_file___dbs___dbs__));
	if(dlg.DoModal()==IDOK)
	{
		FilePath=dlg.GetPathName();
		bGetFile=TRUE;
		m_strEarthquake = _LS(IDS_WG_CMD__ADDD__User_imported_Data);

		CWnd* pWnd = GetDlgItem(IDC_ETC_cmbEARTHQUAKE);
		pWnd->EnableWindow(FALSE);

		m_strSelectedEQ = _LS(IDS_WG_CMD__ADDD__User_imported_Data___r_n)+ FilePath;
		m_sDescript.Format(_LS(IDS_WG_CMD__ADDD__User_imported_Data____s), dlg.GetFileName());
		m_sFileTitle = dlg.GetFileTitle();
		UpdateData(FALSE);  
	}
}

void EQ_REC_DLG::OnOK() 
{
	UpdateData(TRUE);	
	
	if(bGetFile!=TRUE) // bGetFile : 烙器飘甫 沁绰啊?
		GetFile();
		
	m_fp = _tfopen(FilePath, _T("rt"));
		if(m_fp == NULL )
		{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__File_not_found_));
				return;
		}	
	CDialog::OnOK();
}

BOOL EQ_REC_DLG::GetWaveData(LPCTSTR lpWaveName, WAVE_DATA_CH &waveData)
{
		if(!IsUseExtraWave())
				return FALSE;
		return m_mapWaveCH.Lookup(lpWaveName, waveData);
}

void EQ_REC_DLG::InitialWaveCombx()
{
		//
	int nTgType = 0;
	if(IsUseExtraWave())
	{
			CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_COMBO_WAVECH);
			nTgType = pCombox->GetCurSel();
	}
	//
	bGetFile = FALSE;
	// 瘤柳急琶 霓焊 檬扁拳 	
	CString strEarthQuake[]={
			_T("1940, El Centro Site, 270 Deg"),
			_T("1940, El Centro Site, 180 Deg"),
			_T("1940, El Centro Site, Vertical"),

			_T("1952, Taft Lincoln School, 69 Deg"),
			_T("1952, Taft Lincoln School, 339 Deg"),
			_T("1952, Taft Lincoln School, Vertical"),

			_T("1952, Hollywood Storage P.E., 270 Deg"),
			_T("1952, Hollywood Storage P.E., 0 Deg"),
			_T("1952, Hollywood Storage P.E., Vertical"),

			_T("1971, San Fernando, 69 Deg"),
			_T("1971, San Fernando, 159 Deg"),
			_T("1971, San Fernando, Down"),

			_T("1979, James RD. El Centro, 220 Deg"),
			_T("1979, James RD. El Centro, 310 Deg"),
			_T("1979, James RD. El Centro, Up"),

			_T("1985, Mexico City, Station 1, 180 Deg"),
			_T("1985, Mexico City, Station 1, 270 Deg"),

			_T("1994, Northridge, Sylmar County Hosp., 90 Deg"),
			_T("1994, Northridge, Santa Monica, City Hall Grounds, 0 Deg"),
			_T("1994, Northridge, Santa Monica, City Hall Grounds, 90 Deg"),
			_T("1994, Northridge, Arleta and Nordhoff Fire Station, 90 Deg"),

			_T("1989, Loma Prieta, Oakland Outer Wharf, 270 Deg"),
			_T("1989, Loma Prieta, Oakland Outer Wharf, 0 Deg"),

			_T("1971, San Fernando Pocoima Dam, 196 Deg"),
			_T("1971, San Fernando Pocoima Dam, 286 Deg"),

			_T("1966, Parkfield Cholame,Shandon, 40 Deg"),
			_T("1966, Parkfield Cholame,Shandon, 130 Deg"),

			_T("1971, San Fernando 8244 Orion Blvd., 90 Deg"),
			_T("1971, San Fernando 8244 Orion Blvd., 180 Deg"),

			_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY1),
			_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY2),
			_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY3),

			_LS(IDS_WG_CMD_TIME_FUNCTION_MIYAGI_COAST_LG),
			_LS(IDS_WG_CMD_TIME_FUNCTION_MIYAGI_COAST_TR),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_COAST_LG),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYUGANADA_COAST_LG),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYUGANADA_COAST_TR),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_EASTCOAST_TR),
			_LS(IDS_WG_CMD_TIME_FUNCTION_NIHONKAI_CENTRAL_TR),
			_LS(IDS_WG_CMD_TIME_FUNCTION_NIHONKAI_CENTRAL_LG),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_EASTCOAST_LG),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS2),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS3),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW2),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_N30W),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_N12W),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS4),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW3),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW1),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW1),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS1),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW2),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW2),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS2),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW3),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW3),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS3)
	};

	CString strEarthquakeDetail[]=
	{
			_LS(IDS_WG_CMD__ADDD__Peak___0_3487_g__Duration___53_76),	//_T("1940, El Centro Site, S00E"),
			_LS(IDS_WG_CMD__ADDD__Peak___0_2142_g__Duration___53_48),	//_T("1940, El Centro Site, S90W"),
			_LS(IDS_WG_CMD__ADDD__Peak___0_2103_g__Duration___53_74),	//_T("1940, El Centro Site, Vertical"),

			_LS(IDS_WG_CMD__ADDD__Peak____0_1557___Duration___54_38),	//_T("1952, Taft Lincoln School, N21E"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_1793___Duration___54_40),	//_T("1952, Taft Lincoln School, S69E"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_1048___Duration___54_26),	//_T("1952, Taft Lincoln School, Vertical"),

			_LS(IDS_WG_CMD__ADDD__Peak___0_05923_g__Duration___78_6),	//_T("1952, Hollywood Storage P.E., S00W"),
			_LS(IDS_WG_CMD__ADDD__Peak___0_04204_g__Duration___78_6),	//_T("1952, Hollywood Storage P.E., N90E"),
			_LS(IDS_WG_CMD__ADDD__Peak___0_02046_g__Duration___78_5),	//_T("1952, Hollywood Storage P.E., Vertical"),

			_LS(IDS_WG_CMD__ADDD__Peak___0_3154_g__Duration___61_84),	//_T("1971, San Fernando, N21E"),
			_LS(IDS_WG_CMD__ADDD__Peak___0_2706_g__Duration___61_88),	//_T("1971, San Fernando, N69W"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_1563_g__Duration___61_8),	//_T("1971, San Fernando, Down"),

			_LS(IDS_WG_CMD__ADDD__Peak___0_3673_g__Duration___37_68),	//_T("1979, James RD. El Centro, S50W"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_5502_g__Duration___37_8),	//_T("1979, James RD. El Centro, S40W"),
			_LS(IDS_WG_CMD__ADDD__Peak___0_4784_g__Duration___39_36),	//_T("1979, James RD. El Centro, Up"),

			//		_LS(IDS_WG_CMD__ADDD__Peak___0_7777_g__Duration___37_68),	//"1979, Bonds Corner El Centro, S50W",
			//		_LS(IDS_WG_CMD__ADDD__Peak____0_5952_g__Duration___37_8),	//"1979, Bonds Corner El Centro, S40E",
			//		_LS(IDS_WG_CMD__ADDD__Peak____0_3273_g__Duration___37_8),	//"1979, Bonds Corner El Centro, Up",

			_LS(IDS_WG_CMD__ADDD__Peak____0_1714_g__Duration___180_),	//_T("1985, Mexico City, Station 1, 180D"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_1000_g__Duration___180_),	//_T("1985, Mexico City, Station 1, 270D"),

			_LS(IDS_WG_CMD__ADDD__Peak___0_6047_g__Duration___59_98),	//_T("1994, Northridge, Sylmar County Hosp., 90D"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_3703_g__Duration___59_9),	//_T("1994, Northridge, Santa Monica, City Hall Grounds, 0D"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_8836_g__Duration___59_9),	//_T("1994, Northridge, Santa Monica, City Hall Grounds, 90D"),
			_LS(IDS_WG_CMD__ADDD__Peak___0_3442_g__Duration___59_98),	//_T("1994, Northridge, Arleta and Nordhoff Fire Station, 90D"),

			_LS(IDS_WG_CMD__ADDD__Peak___0_2759_g__Duration___39_98),	//_T("1989, Loma Prieta, Oakland Outer Wharf, 270D"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_2199_g__Duration___39_9),	//_T("1989, Loma Prieta, Oakland Outer Wharf, 0D"),

			_LS(IDS_WG_CMD__ADDD__Peak___0_1076_g__Duration___41_70),	//_T("1971, San Fernando Pocoima Dam, 196D"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_1171_g__Duration___41_7),	//_T("1971, San Fernando Pocoima Dam, 286D"),

			_LS(IDS_WG_CMD__ADDD__Peak____0_237_g__Duration___26_18),	//_T("1966, Parkfield Cholame,Shandon, 40D"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_275_g__Duration___26_14),	//_T("1966, Parkfield Cholame,Shandon, 130D"),

			_LS(IDS_WG_CMD__ADDD__Peak____0_255_g__Duration___59_48),	//_T("1971, San Fernando 8244 Orion Blvd., 90D"),
			_LS(IDS_WG_CMD__ADDD__Peak____0_134_g__Duration___59_58),	//_T("1971, San Fernando 8244 Orion Blvd., 180D"),

			_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY1_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY2_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_SEISMIC_INTENSITY3_DETAIL),

			_LS(IDS_WG_CMD_TIME_FUNCTION_MIYAGI_COAST_LG_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_MIYAGI_COAST_TR_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_COAST_LG_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYUGANADA_COAST_LG_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYUGANADA_COAST_TR_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_EASTCOAST_TR_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_NIHONKAI_CENTRAL_TR_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_NIHONKAI_CENTRAL_LG_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HOKKAIDO_EASTCOAST_LG_DETAIL),

			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS2_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS3_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW2_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_N30W_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_N12W_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_NS4_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_HYOUGOKEN_SOUTH_EW3_DETAIL),

			_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW1_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW1_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS1_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW2_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW2_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS2_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOKACHI_EW3_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_EW3_DETAIL),
			_LS(IDS_WG_CMD_TIME_FUNCTION_TOHOKU_NS3_DETAIL)

	};
	//
	InitialWaveData(strEarthQuake, sizeof(strEarthQuake)/sizeof(CString));
	//
	m_strEarthquakeDetail.RemoveAll();
	m_ctlCmbEarthquake.ResetContent();
	for(int i = 0; i < sizeof(strEarthQuake)/sizeof(CString); i++)
	{
		if(!IsNeedShowWave(strEarthQuake[i], nTgType))
			continue;
		m_ctlCmbEarthquake.AddString(strEarthQuake[i]);
		m_strEarthquakeDetail.Add(strEarthquakeDetail[i]);
	}
	AddExtraWaveToComb(nTgType);
	if(m_ctlCmbEarthquake.GetCount() > 0)
			m_ctlCmbEarthquake.SetCurSel(0);
	UpdateData(TRUE);    
	m_strSelectedEQ = GetCurDescription();
	UpdateData(FALSE);
}

void EQ_REC_DLG::InitialWaveCombx_GD_CH()
{
	m_strEarthquakeDetail.RemoveAll();
	m_ctlCmbEarthquake.ResetContent();
	AddExtraWaveToComb_GuangDong_CH();
	if(m_ctlCmbEarthquake.GetCount())
		m_ctlCmbEarthquake.SetCurSel(0);
	OnSelchangecmbEARTHQUAKE();
}

BOOL EQ_REC_DLG::IsExtraWave()
{
		if(!IsUseExtraWave())
				return FALSE;
		int nSel = m_ctlCmbEarthquake.GetCurSel();
		if(nSel < 0 && nSel >= m_strEarthquakeDetail.GetCount())
				return FALSE;
		CString strSelectedEQ = m_strEarthquakeDetail[nSel];
		return strSelectedEQ == _EXTRAWAVE_;
}

CString EQ_REC_DLG::GetCurDescription()
{
		int nSel = m_ctlCmbEarthquake.GetCurSel();
		if(nSel < 0 && nSel >= m_strEarthquakeDetail.GetCount())
				return _T("");
		CString strSelectedEQ = m_strEarthquake + _T("\r\n")+ m_strEarthquakeDetail[nSel];
		if(IsUseExtraWave())
		{
				if(m_strEarthquakeDetail[nSel] == _EXTRAWAVE_)
				{
						strSelectedEQ = m_strEarthquake;
				}
				else
				{
						CString strCH = _T("");
						WAVE_DATA_CH WaveCh(0,0,0);
						if(GetWaveData(m_strEarthquake, WaveCh))
								strCH.Format(_T("\r\n EPA=%.6f g, EPV=%.6f m/s, Tg=%.6f s"), WaveCh.m_dEPA, WaveCh.m_dEPV, WaveCh.m_dTg);
						strSelectedEQ += strCH;
				}
		}
		return strSelectedEQ;
}

BOOL EQ_REC_DLG::IsNeedShowWave(LPCTSTR lpWaveName, int nTgType)
{
	if(!IsUseExtraWave())
	{
			return TRUE;
	}
	if(IsUseExtraWave_GEN() && m_bGuanDong_CH)
	{
		nTgType = GetWaveTg(nTgType) - IEearthquakeWaveMgr::WAVE_TG_NULL;
	}
	if(nTgType <= 0)
			return TRUE;
	//double arTg[]    ={0    ,  0.20,  0.25,  0.30,  0.35,  0.40, 0.45, 0.55, 0.65,  0.75, 0.90};
	double arMaxTg[] ={-1e12, 0.225, 0.275, 0.325, 0.375, 0.425,  0.5,  0.6,  0.7, 0.825, 1e12};

	double dMaxVal = arMaxTg[nTgType];
	double dMinVal = arMaxTg[nTgType-1];
	//double dTg = arTg[nTgType];
	WAVE_DATA_CH waveData(0,0,0);
	if(!GetWaveData(lpWaveName, waveData))
	{
			ASSERT(FALSE);
			return TRUE;
	}
	if(waveData.m_dTg > dMinVal+1e-12 && waveData.m_dTg < dMaxVal+1e-12)
			return TRUE;
	return FALSE;
}

void EQ_REC_DLG::InitialWaveData(CString aWaveName[], int nSize)
{
		m_mapWaveCH.RemoveAll();
		WAVE_DATA_CH waveData[] = {WAVE_DATA_CH(0.295 ,	0.250 ,	0.544 ),
				WAVE_DATA_CH(0.200 ,	0.205 ,	0.658 ),
				WAVE_DATA_CH(0.122 ,	0.050 ,	0.265 ),
				WAVE_DATA_CH(0.148 ,	0.124 ,	0.537 ),
				WAVE_DATA_CH(0.161 ,	0.135 ,	0.537 ),
				WAVE_DATA_CH(0.102 ,	0.056 ,	0.353 ),
				WAVE_DATA_CH(0.054 ,	0.058 ,	0.686 ),
				WAVE_DATA_CH(0.047 ,	0.064 ,	0.885 ),
				WAVE_DATA_CH(0.019 ,	0.023 ,	0.757 ),
				WAVE_DATA_CH(0.271 ,	0.116 ,	0.274 ),
				WAVE_DATA_CH(0.219 ,	0.177 ,	0.518 ),
				WAVE_DATA_CH(0.132 ,	0.045 ,	0.217 ),
				WAVE_DATA_CH(0.789 ,	0.361 ,	0.293 ),
				WAVE_DATA_CH(0.507 ,	0.361 ,	0.456 ),
				WAVE_DATA_CH(0.243 ,	0.150 ,	0.395 ),
				WAVE_DATA_CH(0.085 ,	0.300 ,	2.270 ),
				WAVE_DATA_CH(0.049 ,	0.174 ,	2.260 ),
				WAVE_DATA_CH(0.416 ,	0.417 ,	0.642 ),
				WAVE_DATA_CH(0.267 ,	0.219 ,	0.527 ),
				WAVE_DATA_CH(0.621 ,	0.248 ,	0.256 ),
				WAVE_DATA_CH(0.275 ,	0.282 ,	0.657 ),
				WAVE_DATA_CH(0.181 ,	0.300 ,	1.062 ),
				WAVE_DATA_CH(0.175 ,	0.261 ,	0.958 ),
				WAVE_DATA_CH(0.806 ,	0.475 ,	0.378 ),
				WAVE_DATA_CH(0.834 ,	0.687 ,	0.528 ),
				WAVE_DATA_CH(0.154 ,	0.076 ,	0.317 ),
				WAVE_DATA_CH(0.208 ,	0.090 ,	0.277 ),
				WAVE_DATA_CH(0.233 ,	0.305 ,	0.838 ),
				WAVE_DATA_CH(0.144 ,	0.197 ,	0.878 ),
				WAVE_DATA_CH(0.082 ,	0.123 ,	0.959 ),
				WAVE_DATA_CH(0.100 ,	0.172 ,	1.101 ),
				WAVE_DATA_CH(0.113 ,	0.210 ,	1.194 ),
				WAVE_DATA_CH(0.292 ,	0.493 ,	1.083 ),
				WAVE_DATA_CH(0.283 ,	0.473 ,	1.071 ),
				WAVE_DATA_CH(0.291 ,	0.485 ,	1.068 ),
				WAVE_DATA_CH(0.341 ,	0.621 ,	1.168 ),
				WAVE_DATA_CH(0.338 ,	0.622 ,	1.179 ),
				WAVE_DATA_CH(0.337 ,	0.616 ,	1.171 ),
				WAVE_DATA_CH(0.386 ,	0.768 ,	1.275 ),
				WAVE_DATA_CH(0.390 ,	0.757 ,	1.245 ),
				WAVE_DATA_CH(0.383 ,	0.726 ,	1.216 ),
				WAVE_DATA_CH(0.715 ,	0.705 ,	0.632 ),
				WAVE_DATA_CH(0.724 ,	0.692 ,	0.613 ),
				WAVE_DATA_CH(0.730 ,	0.681 ,	0.598 ),
				WAVE_DATA_CH(0.567 ,	1.021 ,	1.154 ),
				WAVE_DATA_CH(0.563 ,	1.084 ,	1.234 ),
				WAVE_DATA_CH(0.561 ,	0.989 ,	1.130 ),
				WAVE_DATA_CH(0.433 ,	0.993 ,	1.470 ),
				WAVE_DATA_CH(0.443 ,	0.983 ,	1.422 ),
				WAVE_DATA_CH(0.433 ,	0.966 ,	1.431 ),
				WAVE_DATA_CH(0.563 ,	0.573 ,	0.652 ),
				WAVE_DATA_CH(0.569 ,	0.564 ,	0.635 ),
				WAVE_DATA_CH(0.580 ,	0.553 ,	0.611 ),
				WAVE_DATA_CH(0.513 ,	0.707 ,	0.884 ),
				WAVE_DATA_CH(0.525 ,	0.718 ,	0.877 ),
				WAVE_DATA_CH(0.512 ,	0.698 ,	0.874 ),
				WAVE_DATA_CH(0.447 ,	0.834 ,	1.196 ),
				WAVE_DATA_CH(0.438 ,	0.781 ,	1.143 ),
				WAVE_DATA_CH(0.447 ,	0.836 ,	1.199 )};
		int nDataSize = sizeof(waveData)/sizeof(WAVE_DATA_CH); ASSERT(nDataSize == nSize);
		int nSafeSize = min(nDataSize, nSize);
		for (int i=0; i<nSafeSize; i++)
		{
				CString waveName = aWaveName[i];
				const WAVE_DATA_CH &data = waveData[i];
				m_mapWaveCH.SetAt(waveName, data);
		}                                                
} 

IEearthquakeWaveMgr::WAVE_TG EQ_REC_DLG::GetWaveTg(int nTgType)
{
	if(IsUseExtraWave_GEN() && m_bGuanDong_CH)
	{
		CString str;
		GetDlgItem(IDC_COMBO_WAVECH)->GetWindowText(str);
		if(str == _T("0.2")) return IEearthquakeWaveMgr::WAVE_TG_020S;
		else if(str == _T("0.25")) return IEearthquakeWaveMgr::WAVE_TG_025S;
		else if(str == _T("0.3")) return IEearthquakeWaveMgr::WAVE_TG_030S;
		else if(str == _T("0.35")) return IEearthquakeWaveMgr::WAVE_TG_035S;
		else if(str == _T("0.4")) return IEearthquakeWaveMgr::WAVE_TG_040S;
		else if(str == _T("0.45")) return IEearthquakeWaveMgr::WAVE_TG_045S;
		else if(str == _T("0.55")) return IEearthquakeWaveMgr::WAVE_TG_055S;
		else if(str == _T("0.65")) return IEearthquakeWaveMgr::WAVE_TG_065S;
		else if(str == _T("0.75")) return IEearthquakeWaveMgr::WAVE_TG_075S;
		else if(str == _T("0.95")) return IEearthquakeWaveMgr::WAVE_TG_090S;
		else {ASSERT(FALSE);return IEearthquakeWaveMgr::WAVE_TG_NULL;}

	}
	if(nTgType == 0 || nTgType > 10)
		return IEearthquakeWaveMgr::WAVE_TG_NULL;
	IEearthquakeWaveMgr::WAVE_TG nWaveType[] = {IEearthquakeWaveMgr::WAVE_TG_NULL,
		IEearthquakeWaveMgr::WAVE_TG_020S,
		IEearthquakeWaveMgr::WAVE_TG_025S,
		IEearthquakeWaveMgr::WAVE_TG_030S, 
		IEearthquakeWaveMgr::WAVE_TG_035S, 
		IEearthquakeWaveMgr::WAVE_TG_040S,
		IEearthquakeWaveMgr::WAVE_TG_045S,
		IEearthquakeWaveMgr::WAVE_TG_055S,
		IEearthquakeWaveMgr::WAVE_TG_065S,
		IEearthquakeWaveMgr::WAVE_TG_075S,
		IEearthquakeWaveMgr::WAVE_TG_090S};
	return nWaveType[nTgType];
}

void EQ_REC_DLG::AddExtraWaveToComb(int nTgType)
{
		if(!IsUseExtraWave())
				return;
		IEearthquakeWaveMgr::WAVE_TG waveTg = GetWaveTg(nTgType);
		CStringArray aWaveName;
		IEearthquakeWaveMgr::Inst()->GetWaveList(waveTg, aWaveName);
		CString strText;
		for(int i = 0; i < aWaveName.GetCount(); i++)
		{
				m_ctlCmbEarthquake.AddString(aWaveName[i]);
				m_strEarthquakeDetail.Add(_EXTRAWAVE_);
		}
}

void EQ_REC_DLG::OnEqGuandongCheck_CH()
{
	if(!IsUseExtraWave_GEN())
		return;
	UpdateData();
	GetDlgItem(IDC_WG_CMD_STATIC10)->EnableWindow(m_bGuanDong_CH);
	GetDlgItem(IDC_WG_CMD_STATIC11)->EnableWindow(m_bGuanDong_CH);
	GetDlgItem(IDC_COMBO_EQ_WAVECH2)->EnableWindow(m_bGuanDong_CH);
	GetDlgItem(IDC_COMBO_EQ_WAVECH3)->EnableWindow(m_bGuanDong_CH);
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_COMBO_WAVECH);
	pCombox->ResetContent();
	if(m_bGuanDong_CH)
	{
		//CString arTg[] ={"--", "0.20","0.25","0.30","0.35","0.40","0.45","0.55","0.65","0.75","0.90","0.95"};
		//CString arCate[] ={"I0&I1","II","III","IV"};
		//CString arStTg[] ={"--", "0.20","0.25","0.30","0.35","0.40","0.45","0.55","0.65","0.75","0.90","0.95"};
		//II-0.35s-1.25~3s this Folder Is Default
		CStringArray str;
		GetComboStrByIndex(0,str);
		CComboBox *pCombox2 = (CComboBox *)GetDlgItem(IDC_COMBO_EQ_WAVECH2);
		if(pCombox2->GetCount() != 0)
		{
			ASSERT(FALSE);
			pCombox2->ResetContent();
		}
		int nSelectSel = 0;
		for(int i = 0;i < str.GetSize();i++)
		{
			if(str[i] == _T("I"))
				pCombox2->AddString(_T("I0&I1"));
			else if(str[i] == _T("II"))
				nSelectSel = pCombox2->AddString(str[i]);
			else
				pCombox2->AddString(str[i]);
		}
		//pCombox2->SetWindowText(_T("II"));//场地类别
		pCombox2->SetCurSel(nSelectSel);//场地类别

		GetComboStrByIndex(1,str);
		for(int i = 0;i < str.GetSize();i++)
		{
			if(str[i] == _T("0.35s"))
				nSelectSel = pCombox->AddString(str[i].Left(str[i].Find('s')));
			else
				pCombox->AddString(str[i].Left(str[i].Find('s')));
		}
		pCombox->SetCurSel(nSelectSel);//特征周期

		pCombox2 = (CComboBox *)GetDlgItem(IDC_COMBO_EQ_WAVECH3);
		if(pCombox2->GetCount() != 0)
		{
			ASSERT(FALSE);
			pCombox2->ResetContent();
		}
		GetComboStrByIndex(2,str);
		for(int i = 0;i < str.GetSize();i++)
		{
			if(str[i] == _T("1.25~3s"))
				nSelectSel = pCombox2->AddString(str[i]);
			else
				pCombox2->AddString(str[i]);
		}
		pCombox2->SetCurSel(nSelectSel);//结构周期
		InitialWaveCombx_GD_CH();
	}
	else
	{
		CString arTg[] ={_T("--"), _T("0.20"),_T("0.25"),_T("0.30"),_T("0.35"),_T("0.40"),_T("0.45"),_T("0.55"),_T("0.65"),_T("0.75"),_T("0.90")};
		for (int i=0; i<sizeof(arTg)/sizeof(CString); i++)
			pCombox->AddString(arTg[i]);
		pCombox->SetCurSel(0);
		CComboBox *pCombox2 = (CComboBox *)GetDlgItem(IDC_COMBO_EQ_WAVECH2);
		pCombox2->ResetContent();
		pCombox2 = (CComboBox *)GetDlgItem(IDC_COMBO_EQ_WAVECH3);
		pCombox2->ResetContent();
		InitialWaveCombx();
	}
}

void EQ_REC_DLG::OnSelGDCate_CH()
{
	UpdateData();
	if(!IsUseExtraWave_GEN() || !m_bGuanDong_CH)
		return;
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_COMBO_WAVECH);
	pCombox->ResetContent();
	CStringArray str;
	GetComboStrByIndex(1,str);
	for(int i = 0;i < str.GetSize();i++)
	{
		pCombox->AddString(str[i].Left(str[i].Find('s')));
	}
	pCombox->SetCurSel(0);//Default
	OnSelchangecmbTg();
}

void EQ_REC_DLG::OnSelGDTg_CH()
{
	UpdateData();
	if(!IsUseExtraWave_GEN()|| !m_bGuanDong_CH)
		return;
	InitialWaveCombx_GD_CH();
}

void EQ_REC_DLG::InitComboString_CH()
{
	if(!IsUseExtraWave_GEN())
		return;
	//m_mapWaveCH
}
void EQ_REC_DLG::InitGuangdongWaveFolder()
{
	if(!IsUseExtraWave_GEN())
		return;
	long hFile = 0;

	TCHAR lpFilename[_MAX_PATH+1];
	GetModuleFileName(NULL,lpFilename,_MAX_PATH);

	TCHAR path_buffer[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	TCHAR InfoName[_MAX_DIR];
	_tsplitpath( lpFilename, drive, dir, fname, ext );
	_tcscpy(path_buffer,drive);
	_tcscat(path_buffer,dir);
	_tcscpy(InfoName,path_buffer);
	_tcscat(path_buffer,_T("dbase\\GuangDong\\/*.*"));
	_tcscat(InfoName,_T("dbase\\Guangdong\\GDInfodbs"));
	CFileFind find;
	BOOL IsFind = find.FindFile(path_buffer);
	while(IsFind)
	{
		IsFind = find.FindNextFile();
		if(find.IsDirectory() && !find.IsDots())
			m_strGuangdongWave.Add(find.GetFileName());
	}
	find.Close();
	ReadGuangDongInfo(InfoName);
}

void EQ_REC_DLG::GetComboStrByIndex(int nIndex,CStringArray& str)
{
	if(!IsUseExtraWave_GEN())
		return;
	str.RemoveAll();
	CString strTest;
	CMap<CString,LPCTSTR,int,int> map_Filter;
	CStringArray strFilt;
	if(nIndex == 0)
		strFilt.Copy(m_strGuangdongWave);
	else if(nIndex == 1)
	{
		CString str1;
		GetDlgItem(IDC_COMBO_EQ_WAVECH2)->GetWindowText(str1);
		if(str1 == _T("I0&I1"))
			str1 = _T("I");
		for(int i = 0;i < m_strGuangdongWave.GetSize();i++)
		{
			if(str1 == GetFileStrBy_Index(0,m_strGuangdongWave[i]))
				strFilt.Add(m_strGuangdongWave[i]);
		}
	}
	else if(nIndex == 2)
	{
		CString str1,str2;
		GetDlgItem(IDC_COMBO_EQ_WAVECH2)->GetWindowText(str1);
		GetDlgItem(IDC_COMBO_WAVECH)->GetWindowText(str2);
		str2 += _T("s");
		if(str1 == _T("I0&I1"))
			str1 = _T("I");
		for(int i = 0;i < m_strGuangdongWave.GetSize();i++)
		{
			if(str1 == GetFileStrBy_Index(0,m_strGuangdongWave[i]) && str2 == GetFileStrBy_Index(1,m_strGuangdongWave[i]))
				strFilt.Add(m_strGuangdongWave[i]);
		}
	}

	for(int i = 0;i < strFilt.GetSize();i++)
	{
		strTest = GetFileStrBy_Index(nIndex,strFilt[i]);
		if(!map_Filter.PLookup(strTest))
		{
			map_Filter.SetAt(strTest,1);
			str.Add(strTest);
		}
	}
}

CString EQ_REC_DLG::GetFileStrBy_Index(int nIndex,const CString& strArr)
{
	if(nIndex == 0) return strArr.Left(strArr.Find('-'));
	else if(nIndex == 1)
	{
		int nLeft = strArr.Find('-');
		int nRight = strArr.ReverseFind('-');
		return strArr.Mid(nLeft+1,nRight-nLeft-1);
	}
	else if(nIndex == 2) return strArr.Right(strArr.GetLength()-1-strArr.ReverseFind('-'));
	else
	{
		ASSERT(FALSE);
		return _T("");
	}
}

void EQ_REC_DLG::GetGuangdongWaveFileName(CStringArray& strArr)
{
	strArr.RemoveAll();
	if(!IsUseExtraWave_GEN() || !m_bGuanDong_CH)
		return;
	CString strFolder,str[3];
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_COMBO_EQ_WAVECH2);
	if(pCombox->GetCurSel() == -1)
	{
		ASSERT(FALSE);
		return;
	}
	pCombox->GetWindowText(str[0]);
	if(str[0] == _T("I0&I1"))
		str[0] = _T("I");
	pCombox = (CComboBox *)GetDlgItem(IDC_COMBO_WAVECH);
	if(pCombox->GetCurSel() == -1)
	{
		ASSERT(FALSE);
		return;
	}
	pCombox->GetWindowText(str[1]);
	str[1] += _T("s");
	pCombox = (CComboBox *)GetDlgItem(IDC_COMBO_EQ_WAVECH3);
	if(pCombox->GetCurSel() == -1)
	{
		ASSERT(FALSE);
		return;
	}
	pCombox->GetWindowText(str[2]);
	strFolder.Format(_T("%s-%s-%s"),str[0],str[1],str[2]);
 	CString strFullPath;
	strFullPath.Format(_T("%s\\/*.dbs"), GetFilePath(strFolder,TRUE));
	CFileFind find;
	BOOL IsFind = find.FindFile(strFullPath);
	CString strFileName;
	while(IsFind)
	{
		IsFind = find.FindNextFile();
		if(find.IsArchived())
		{
			strFileName = find.GetFileName();
			strFileName = strFileName.Left(strFileName.ReverseFind('.'));
			strArr.Add(strFileName);
		}
	}
	find.Close();
	ASSERT(!strArr.IsEmpty());
}

void EQ_REC_DLG::AddExtraWaveToComb_GuangDong_CH()
{
	if(!IsUseExtraWave_GEN() || !m_bGuanDong_CH)
		return;
	CStringArray str;
	CString strDetail;
	GetGuangdongWaveFileName(str);
	m_mapCheckGuangDongWave.RemoveAll();
	for(int i = 0;i < str.GetSize();i++)
	{
		m_ctlCmbEarthquake.AddString(str[i]);
		if(m_mapGuangDongDetail.Lookup(str[i],strDetail))
			m_strEarthquakeDetail.Add(strDetail);
		else
			m_strEarthquakeDetail.Add(_EXTRAWAVE_);
		m_mapCheckGuangDongWave.SetAt(str[i],1);
	}
}

int EQ_REC_DLG::lex(FILE *fp, char *yytext)
{
	int		c, index;

	index	= 0;
	c		= _gettc(fp);

	// Discard white characters
	while ((c==' ') || (c=='\r') || (c=='\t') || (c=='\n')) {
		c = _gettc(fp);
	}

	if (c == EOF ) return -1;

	// Handle(Discard) Comments
	while (c == '*') {
		c = _gettc(fp);
		if (c == '*') {
			c = _gettc(fp);
			while (c != '\n')
				c = _gettc(fp);
			c = _gettc(fp);
		}
		else {
			_ungettc(c, fp);
			yytext[index++] = '*';
			yytext[index] = '\0';
			return 1;
		}
	}

	switch (c) {
	case ',':
		yytext[index++] = c;
		yytext[index] = '\0';
		break ;
	default:
		while ((c!='*') && (c!=',') && (c!='\r') && (c!='\n')) {
			yytext[index++] = c;
			c = _gettc(fp);
			if (c == EOF || index >=120)
				return -1;
		}
		yytext[index] = '\0';
		if ((c=='*') || (c==','))
			_ungettc(c, fp);
	}

	return 1;
}

BOOL EQ_REC_DLG::ReadGuangDongInfo(const CString& strFileName)
{
	CStringA strPathTemp(strFileName);
	FILE* fp = fopen(strPathTemp, "rt");
	if(fp == NULL )
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__File_not_found_));
		return FALSE;
	}
	m_mapGuangDongDetail.RemoveAll();
	char		yytext[120];
	int nNumOfData=0;
	CString strWaveName,strDetail;
	try
	{
		while (lex(fp,yytext) == 1) // Read WaveName
		{	
			strWaveName = yytext;
			if(nNumOfData>=3) 
			{ 
				//m_dSampleTime = m_arTime[2] - m_arTime[1]; // 基敲狼 矫埃埃拜阑 茫扁 困窃 

			}
			//Peak
			lex(fp, yytext);// comma
			if(strcmp(yytext, ",") != 0) 
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
				fclose(fp);
				return FALSE;
			}
			lex(fp,yytext);
			strDetail = _T("Peak = ") + CString(yytext)+ _T(" GAL");
			//Duration
			lex(fp, yytext);// comma
			if(strcmp(yytext, ",") != 0) 
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
				fclose(fp);
				return FALSE;
			}
			lex(fp,yytext);
			strDetail +=  CString(_T("  持续时间 = ")) + CString(yytext)+ "s" + "\r\n";
			//DIRECTION V
			lex(fp, yytext);// comma
			if(strcmp(yytext, ",") != 0) 
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
				fclose(fp);
				return FALSE;
			}
			int nRlt = lex(fp, yytext);
			strDetail += CString("DIRECTION ") + CString(yytext);
			m_mapGuangDongDetail.SetAt(strWaveName,strDetail);
			nNumOfData++;
			if(nRlt == -1)
				break;
		}
		if(nNumOfData < 1) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Data_Found_));
			fclose(fp);
			return FALSE;
		}
		fclose(fp);
	}
	catch (CFileException& e)
	{
		e.ReportError();
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
		return FALSE;
	}
	return TRUE;

}

void EQ_REC_DLG::AdjustUILayoutForChinese()
{
	if (!IsUseExtraWave())
		return;

	// Step 1: IDs to move upward
	CArray<UINT> aTempID;
	aTempID.Add(IDC_STATIC_WAVECH_TG);     // Topmost
	aTempID.Add(IDC_COMBO_WAVECH);
	aTempID.Add(IDC_ETC_EQ_GUANDONG_CHECK_CH);
	aTempID.Add(IDC_WG_CMD_STATIC10);
	aTempID.Add(IDC_COMBO_EQ_WAVECH2);
	aTempID.Add(IDC_WG_CMD_STATIC11);// Bottom of moving group
	aTempID.Add(IDC_COMBO_EQ_WAVECH3);

	const int Padding = 8; // extra spacing above new inserted group

	// Step 2: Get target position (insertion point)
	CRect rcTarget;
	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(&rcTarget);
	ScreenToClient(&rcTarget);

	// Step 3: Get current position of the topmost item to move
	CRect rcTop;
	GetDlgItem(IDC_STATIC_WAVECH_TG)->GetWindowRect(&rcTop);
	ScreenToClient(&rcTop);

	// Calculate how far to move upward
	int deltaY = rcTop.top - rcTarget.top + Padding;

	// Step 4: Move aTempID upward by deltaY
	for (int i = 0; i < aTempID.GetCount(); ++i)
	{
		CWnd* pCtrl = GetDlgItem(aTempID[i]);
		if (pCtrl && ::IsWindow(pCtrl->m_hWnd))
		{
			CRect rc;
			pCtrl->GetWindowRect(&rc);
			ScreenToClient(&rc);
			rc.OffsetRect(0, -deltaY); // move upward
			pCtrl->SetWindowPos(nullptr, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}

	// Step 5: Get new bounding box of inserted group (bottom)
	CRect rcBottom;
	GetDlgItem(IDC_WG_CMD_STATIC11)->GetWindowRect(&rcBottom);
	ScreenToClient(&rcBottom);

	int totalMoveHeight = rcBottom.bottom + Padding - rcTarget.top;

	// Step 6: Move all controls that are below IDC_WG_CMD_STATIC0 and not in aTempID
	CRect rcThreshold;
	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(&rcThreshold);
	ScreenToClient(&rcThreshold);
	int thresholdY = rcThreshold.bottom;

	CWnd* pChild = GetWindow(GW_CHILD);
	while (pChild)
	{
		UINT ctrlID = pChild->GetDlgCtrlID();

		// Skip invalid or moved controls
		bool bSkip = false;
		for (int i = 0; i < aTempID.GetCount(); ++i)
		{
			if (aTempID[i] == ctrlID)
			{
				bSkip = true;
				break;
			}
		}

		if (!bSkip && ctrlID != (UINT)-1)
		{
			CRect rcCtrl;
			pChild->GetWindowRect(&rcCtrl);
			ScreenToClient(&rcCtrl);

			if (rcCtrl.top >= rcTarget.top)
			{
				rcCtrl.OffsetRect(0, totalMoveHeight); // move downward
				pChild->SetWindowPos(nullptr, rcCtrl.left, rcCtrl.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}
		}

		pChild = pChild->GetNextWindow();
	}

	// Step 7: Resize dialog height
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	int newClientHeight = rcClient.Height() + totalMoveHeight;
	int newWindowHeight = rcWindow.Height() + totalMoveHeight;

	SetWindowPos(nullptr, 0, 0, rcWindow.Width(), newWindowHeight, SWP_NOMOVE | SWP_NOZORDER);
}