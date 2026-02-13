// Beam.cpp: implementation of the CRCrailway class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RCrailway.h"
#include "math.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRCrailway::CRCrailway()
{ 
    for(int i=0; i<15; i++) m_bcheckitem[i]=FALSE;
	
    for(int j=0; j<6; j++)
    {	 
		dMy[j]=0.0; dMy1[j]=0.0; dMy2[j]=0.0; dMz[j]=0.0;
		dNx[j]=0.0; dVz[j]=0.0;	 
		iNx[j]=0; iMy[j]=0; iVz[j]=0; iMz[j]=0;
    }  
	m_db=0; m_dh=0; m_dhf=0;m_dh_f=0; m_dbf=0; m_db_f=0;m_das=0; m_das_d=0; m_da_s_d=0; 
    m_da_s=0; m_dAs=0; m_dA_s=0; m_dn=0; m_dAsR=0;
    m_dLo=0; m_dEc=0; m_dEs=0; m_dK=2; m_dK1=0.8; m_da_K2=0.3; m_dK3=1; m_dd=0; 
    m_dR=0; m_dm=0; 
    m_dconcrete_c=0; m_dconcrete_b=0; m_dconcrete_shear=0; m_dconcrete_tp=0; m_drebar=0; m_dconcrete_u=0; m_dconcrete_wf=0;//容许的应力
	
	// [21/02/2007 : maxiao] For CIVIL(CH Railway Design) 2007
	m_elemlist.RemoveAll();
    m_iTotLineNum=0;
    m_BeamConKList.RemoveAll();
    m_BeamShearKList.RemoveAll();
    m_BeamRebarKList.RemoveAll();
    m_BeamSheartKList.RemoveAll();
    m_BeamShearbKList.RemoveAll();
    m_BeamCrackKList.RemoveAll();
    m_ColumnConKList.RemoveAll();
    m_ColumnShearKList.RemoveAll();
    m_ColumnRebarcKList.RemoveAll();
    m_ColumnRebarsKList.RemoveAll();
    m_ColumnStabKList.RemoveAll();
    m_ColumnCrackKList.RemoveAll();
    m_SeismicConKList.RemoveAll()   ;
    m_SeismicRebarcKList.RemoveAll();
    m_SeismicRebarsKList.RemoveAll();
    m_SeismicEKList.RemoveAll()     ;
    m_SeismicStabKList.RemoveAll()  ;
    m_SeismicDuctKList.RemoveAll()  ;
    
	m_pDoc = CDBDoc::GetDocPoint();
 	ASSERT(m_pDoc);
	
}


CRCrailway::~CRCrailway()
{
	
}


BOOL CRCrailway::RCrailway_CH(BOOL isBeam)
{
    if (CDBDoc::GetDocPoint()->IsLimitedTrialVersion())
    {
        //AfxMessageBox(_LS(IDS_TRIAL_NO_LICENSE));
        CDBDoc::GetDocPoint()->ShowLimitedTrialVersionErrMsg();
        return FALSE;
    }
    int iStageNum = m_pDoc->m_pAttrCtrl->GetCountStag();
    T_STAG_K CurStag = m_pDoc->m_pStagCtrl->GetCurStag();
    int iCurStep = m_pDoc->m_pStagCtrl->GetCurStagStepIndex()+1;
    if (iStageNum>0)
        m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();
    if(!MakeData(isBeam)) 
    {
        if(iStageNum>0)
            m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
        return FALSE;
    }
	
    CFileException FileException;
    CString strPathName=m_pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
    CString strFilePathName,outFilePathName,ReportPathNamme;
    if (isBeam) 
	{
        strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_chBeam.jpo");
        outFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_chBeam.mrpt");
    }
	else
	{
        strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_chColumn.jpo");
        outFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_chColumn.mrpt");
    }
    Set_FileName(strFilePathName);
    TRY
    {
        if(!m_cfile.Open(strFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
        if(!file.Open(outFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
        if(!Write_ImportData(isBeam)) 
        {
            m_cfile.Close();
            file.Close();
            CFileStatus status;
            if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
            if(iStageNum>0)
                m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
            return FALSE;
        }
        m_cfile.Close();
        file.Close();
    }
    CATCH(CFileException, e)
    {
        e->ReportError();
        m_cfile.Abort();
        file.Abort();
        CFileStatus status;
        return FALSE;
    }
    END_CATCH 
		return TRUE; 
}

CString CRCrailway::Get_DllFileName(int iCode)
{
	BOOL bDebug = FALSE;
    CString strDllName = _T("");
	
#if defined(_DEBUG)
	bDebug = TRUE;
#endif
	
	if(iCode==RC_TB_10002_3_05)
	{
		strDllName = bDebug ? _T("CVL_DgnCalc_CHD.dgne") : _T("CVL_DgnCalc_CH.dgne");      
	}
	else 
		ASSERT(0);
	
    return strDllName;
}

BOOL CRCrailway::Write_ImportData(BOOL isBeam){
    T_RC_D_CH Rcd;  
    if (!m_DataCtrl.Get_Rcd(Rcd)) 
	{ 
        return FALSE;
    }
    if(Rcd.iDgnCode != RC_TB_10002_3_05)
	{
        ASSERT(0);
        return TRUE;
    }
	
    CString strCalcDll = Get_DllFileName(RC_TB_10002_3_05);  
    	
	{// AFX_MANAGE_STATE(AfxGetModuleState())篮 _T("{"),_T("}") 郴何俊 沥狼
		// LoadLibrary()俊辑 府家胶 眉牢阑 噶啊哆府绰 版快啊 乐绢辑
		// 捞甫 汗盔窍扁 困秦 AfxGetModuleState() 窃荐甫 荤侩窃.
		AFX_MANAGE_STATE(AfxGetModuleState());

		m_hDllCalc = GetModuleHandle(strCalcDll);
		if(m_hDllCalc == NULL)
		{		
			if((m_hDllCalc = LoadLibrary(strCalcDll)) == NULL)
			{
				CString strMsg = _T("Can't Find ")+strCalcDll;
				AfxMessageBox(strMsg);
			}
		}
	}
	
	for(int i=0; i<12; i++)
	{//design choose
		m_bcheckitem[i] = Rcd.bDesignPrint[i];
	}
    //get [Wf]
    m_dconcrete_wf = Rcd.iCrackCode;
    BOOL isSeismic = (Rcd.iCrackType==1)? TRUE:FALSE ;// iCrackType =1 is seismic,=0 is not seismic
	
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
    m_DataCtrl.Set_UnitIndex(CodeUnit);	
    //for beam
    if (isBeam) 
	{
		if(m_bcheckitem[0]==TRUE) beam_concrete_c(); //混凝土压应力验算
		if(m_bcheckitem[1]==TRUE) beam_shear();      //混凝土中性轴处剪应力验算
		if(m_bcheckitem[2]==TRUE) beam_rebar_s();    //钢筋拉应力验算
        if(m_bcheckitem[3]==TRUE) beam_crack();      //梁裂缝宽度验算   
		if(m_bcheckitem[4]==TRUE) beam_shear_t();    //混凝土上翼缘剪应力验算
		if(m_bcheckitem[5]==TRUE) beam_shear_b();    //混凝土下翼缘剪应力验算
    }
	else
	{//for column
		if(m_bcheckitem[6]==TRUE) column_concrete_c(FALSE); //混凝土压应力验算 
		if(m_bcheckitem[7]==TRUE) column_shear();           //混凝土主拉应力验算
		if(m_bcheckitem[8]==TRUE) column_rebar_c(FALSE);    //钢筋压应力验算 
		if(m_bcheckitem[9]==TRUE) column_rebar_s(FALSE);    //钢筋拉应力验算
		if(m_bcheckitem[10]==TRUE) column_crack();          //柱裂缝宽度验算
		if(m_bcheckitem[11]==TRUE) column_stability(FALSE); //柱稳定性验算
		if(isSeismic)
		{
			column_concrete_c(TRUE); //混凝土压应力验算
			column_rebar_c(TRUE);    //钢筋压应力验算
			column_rebar_s(TRUE);    //钢筋拉应力验算
            column_e(TRUE);          //偏心距验算
			column_stability(TRUE);  //柱稳定性验算
			seismic_ductility();     //延性比验算
		}
    }
	return TRUE;
}

BOOL CRCrailway::MakeData( BOOL isBeam)
{
    m_DataCtrl.Initialize();
    //get design elemlist
    m_elemlist.RemoveAll();
    if (!m_DataCtrl.Get_CrcElemListForDgn(FALSE,m_elemlist,isBeam)) 
	{
        return FALSE;
    }
    if(!m_DataCtrl.Get_LcomDataMap(m_DgnLcomKMap))	return FALSE;
	if(!m_DataCtrl.Get_ForcDataMap(FALSE,m_ComfDMap))	return FALSE;
	
    for (int i=0; i<m_elemlist.GetSize();i++ ) 
	{
		double d1, d2;
        if (!Get_concreteValue(m_elemlist.GetAt(i), d1, d2))
		{
            GSaveHistoryFormatNF(_T("[错误] RC设计单元材料选择不正确."));
            return FALSE;
        }
	}
    return TRUE;
}

//截面参数的初始化
void CRCrailway::section_Initialize()
{
    m_db=0; m_dh=0; m_dhf=0;m_dh_f=0; m_dbf=0; m_db_f=0;m_das=0; m_das_d=0; m_da_s_d=0; 
    m_da_s=0; m_dAs=0; m_dA_s=0; m_dn=0; m_dAsR=0;
    m_dLo=0; m_dEc=0; m_dEs=0; m_dK=2; m_dK1=0.8; m_da_K2=0.3; m_dK3=1; m_dd=0; 
    m_dR=0; m_dm=0; 
    m_dconcrete_c=0; m_dconcrete_b=0; m_dconcrete_shear=0; m_dconcrete_tp=0; m_drebar=0; m_dconcrete_u=0;//容许的应力
}

//截面荷载的初始化
void CRCrailway::force_Initialize()
{    
	
	for(int i=0; i<6; i++)
	{		 
		dMy[i]=0; dMy1[i]=0; dMy2[i]=0;		 
		dNx[i]=0.0; dVz[i]=0;		 
		iNx[i]=0; iMy[i]=0; iVz[i]=0;
	}
}


//得到截面数据
void CRCrailway::get_section(int Elemi, int iPosi,BOOL isBeam)
{
    _DGN_RC_SECTION_CH sectioni; sectioni.Initialize();
    _DGN_RC_COLUMN_SECTION_CH sectionk; sectionk.Initialize();
    T_CWRC_D CwrcD;	CwrcD.Initialize();
    if(m_pDoc->m_pAttrCtrl->GetCwrc(Elemi, CwrcD))
	{
        m_dK1    = CwrcD.dK1;  //钢筋表面形状影响系数       规范 p29 （界面上用户输入） ;
        m_da_K2  = CwrcD.dA;   //系数a   规范 p29 （界面上用户输入）
    }
	else
	{
        m_dK1    = 0.8;   m_da_K2  =  0.3;
    }
    if (isBeam) 
	{
        if (!m_DataCtrl.Get_BeamSection(Elemi,iPosi,sectioni)) 	return;
        m_db     = sectioni.db;    //宽度 
        m_dh     = sectioni.dh;    //高度
        m_db_f   = sectioni.dbf1;  //上翼缘宽度
        m_dbf    = sectioni.dbf2;  //下翼缘宽度
        m_dh_f   = sectioni.dhf1;  //上翼缘高度
        m_dhf    = sectioni.dhf2;  //下翼缘高度
        m_das    = sectioni.das;   //受拉钢筋保护层厚度
        m_da_s   = sectioni.das1;  //受压钢筋保护层厚度
        m_dAs    = sectioni.dAs;   //受拉钢筋面积
        m_dA_s   = sectioni.dAs1;  //受压钢筋面积
		//m_das_d  最外层受拉钢筋至混凝土受拉边缘的距离;	//m_da_s_d 最外层受压钢筋至混凝土受压边缘的距离
		m_DataCtrl.GetBeamOutRebarDistance(Elemi,m_dh,m_das_d,m_da_s_d);
        m_dd     = sectioni.dd;    //受拉钢筋直径
//		m_dR     =  sectionk.dr;  //圆形截面半径
//		m_dAsR   = (m_dR > 0) ? (m_dAs + m_dA_s) : 0.0; //圆形截面纵向钢筋面积
//    m_dasR   =  sectionk.dC;  //圆形钢筋保护层厚度	
	  m_dIy    = sectioni.dIyy;//对y轴的惯性距
		m_dIz    = sectioni.dIzz;//对z轴的惯性距
        m_dCzp   = sectioni.dCzp;
		m_dCzm   = sectioni.dCzm;
		m_dCyp   = sectioni.dCyp;
		m_dCym   = sectioni.dCym;	
    }
	else
	{
        if (!m_DataCtrl.Get_ColumnSection(Elemi,iPosi,sectionk))   return;
        m_db     = sectionk.db;    //宽度 
        m_dh     = sectionk.dh;    //高度
        m_db_f   = sectionk.dbf1;  //上翼缘宽度
        m_dbf    = sectionk.dbf2;  //下翼缘宽度
        m_dh_f   = sectionk.dhf1;  //上翼缘高度
        m_dhf    = sectionk.dhf2;  //下翼缘高度
        m_das    = sectionk.das;   //受拉钢筋保护层厚度
        m_da_s   = sectionk.das1;  //受压钢筋保护层厚度
        m_dAs    = sectionk.dAs;   //受拉钢筋面积
        m_dA_s   = sectionk.dAs1;  //受压钢筋面积
		//m_das_d  最外层受拉钢筋至混凝土受拉边缘的距离;	//m_da_s_d 最外层受压钢筋至混凝土受压边缘的距离
		m_DataCtrl.GetColumnOutRebarDistance(Elemi,m_dh,m_das_d,m_da_s_d);
        m_dLo    = sectionk.dl0;   //压杆的计算长度
        m_dd     = sectionk.dd;    //受拉钢筋直径
		m_dR     = sectionk.dr;    //圆形截面半径
		m_dAsR   = (m_dR > 0) ? (m_dAs + m_dA_s + sectionk.dAsl + sectionk.dAsr) : 0.0; //圆形截面纵向钢筋面积
        m_dasR   = sectionk.dC;  //圆形钢筋保护层厚度	
		
        m_dIy    = sectionk.dIyy;//对y轴的惯性距
		m_dIz    = sectionk.dIzz;//对z轴的惯性距
        m_dCzp   = sectionk.dCzp;
		m_dCzm   = sectionk.dCzm;
		m_dCyp   = sectionk.dCyp;
		m_dCym   = sectionk.dCym;
    }

	//混凝土容许应力值
    _DGN_RC_MATL_CH matl; matl.Initialize();
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetElem(Elemi,ElemD))	{ASSERT(0); return ;}
	// Get Matl Data.
	T_MATD_D MatdD;	MatdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat,MatdD))	{ASSERT(0); return ;}
	if(MatdD.Data1.CodeName!=MATLCODE_CON_TB05)	{ASSERT(0); return ;}
    CString concrete = MatdD.Data1.CodeMatlName; //混凝土的标号
    CString rebar = MatdD.MainRebar_RebarName;   //钢筋的标号    //  MatdD.SubRebar_RebarName;
	 
	if(concrete==_T("C20")) {m_dconcrete_c=5.4; m_dconcrete_b=6.8; m_dconcrete_tp=1.53; m_dconcrete_shear=0.85; m_dEc=28000.0;}     
	else if(concrete==_T("C25")) {m_dconcrete_c=6.8; m_dconcrete_b=8.5; m_dconcrete_tp=1.80; m_dconcrete_shear=1.0; m_dEc=230000.0;}     
	else if(concrete==_T("C30")) {m_dconcrete_c=8.0; m_dconcrete_b=10.0; m_dconcrete_tp=1.98; m_dconcrete_shear=1.1; m_dEc=32000.0;}     
	else if(concrete==_T("C35")) {m_dconcrete_c=9.4; m_dconcrete_b=11.8; m_dconcrete_tp=2.25; m_dconcrete_shear=1.25; m_dEc=33000;}     
	else if(concrete==_T("C40")) {m_dconcrete_c=10.8; m_dconcrete_b=13.5; m_dconcrete_tp=2.43; m_dconcrete_shear=1.35; m_dEc=34000.0;}    
	else if(concrete==_T("C45")) {m_dconcrete_c=12.0; m_dconcrete_b=15.0; m_dconcrete_tp=2.61; m_dconcrete_shear=1.45; m_dEc=34500.0;}    
	else if(concrete==_T("C50")) {m_dconcrete_c=13.4; m_dconcrete_b=16.8; m_dconcrete_tp=2.79; m_dconcrete_shear=1.55; m_dEc=35500.0;}   
	else if(concrete==_T("C55")) {m_dconcrete_c=14.8; m_dconcrete_b=18.5; m_dconcrete_tp=2.97; m_dconcrete_shear=1.65; m_dEc=36000.0;}  
	else if(concrete==_T("C60")) {m_dconcrete_c=16.0; m_dconcrete_b=20.0; m_dconcrete_tp=3.15; m_dconcrete_shear=1.75; m_dEc=36500.0;}
	 
	//钢筋弹性模量与混凝土变形模量之比	 
	if(concrete==_T("C20")) { if(isBeam==TRUE) m_dn=20; else m_dn=15;}     
	else if(concrete==_T("C25")) { if(isBeam==TRUE) m_dn=15; else m_dn=10;}    
	else if(concrete==_T("C30")) { if(isBeam==TRUE) m_dn=15; else m_dn=10;}    
	else if(concrete==_T("C35")) { if(isBeam==TRUE) m_dn=15; else m_dn=10;}    
	else if(concrete==_T("C40")) { if(isBeam==TRUE) m_dn=10; else m_dn=8;}     
	else if(concrete==_T("C45")) { if(isBeam==TRUE) m_dn=10; else m_dn=8;}    
	else if(concrete==_T("C50")) { if(isBeam==TRUE) m_dn=10; else m_dn=8;}    
	else if(concrete==_T("C55")) { if(isBeam==TRUE) m_dn=10; else m_dn=8;}   
	else if(concrete==_T("C60")) { if(isBeam==TRUE) m_dn=10; else m_dn=8;}
    
	//受压钢筋计算强度与混凝土抗压极限强度之比 
	if(concrete==_T("C20")) { if(rebar==_T("Q235")) m_dm=17.4; if(rebar==_T("HRB335")) m_dm=24.8;}     
	 else if(concrete==_T("C25")) { if(rebar==_T("Q235")) m_dm=13.8; if(rebar==_T("HRB335")) m_dm=19.7;}     
	 else if(concrete==_T("C30")) { if(rebar==_T("Q235")) m_dm=11.8; if(rebar==_T("HRB335")) m_dm=16.8;}    
	 else if(concrete==_T("C35")) { if(rebar==_T("Q235")) m_dm=10.0; if(rebar==_T("HRB335")) m_dm=14.3;}     
	 else if(concrete==_T("C40")) { if(rebar==_T("Q235")) m_dm=8.7; if(rebar==_T("HRB335")) m_dm=12.4;}    
	 else if(concrete==_T("C45")) { if(rebar==_T("Q235")) m_dm=7.8; if(rebar==_T("HRB335")) m_dm=11.2;}    
	 else if(concrete==_T("C50")) { if(rebar==_T("Q235")) m_dm=7.0; if(rebar==_T("HRB335")) m_dm=10.0;}    
	 else if(concrete==_T("C55")) { if(rebar==_T("Q235")) m_dm=6.4; if(rebar==_T("HRB335")) m_dm=9.1;}    
	 else if(concrete==_T("C60")) { if(rebar==_T("Q235")) m_dm=5.9; if(rebar==_T("HRB335")) m_dm=8.4;}
	 
	 //截面最小配筋率  
	 if(concrete==_T("C20")) { if(rebar==_T("Q235")) m_dconcrete_u=0.15; if(rebar==_T("HRB335")) m_dconcrete_u=0.1;}
     else if(concrete==_T("C25")) { if(rebar==_T("Q235")) m_dconcrete_u=0.2; if(rebar==_T("HRB335")) m_dconcrete_u=0.15;}
     else if(concrete==_T("C30")) { if(rebar==_T("Q235")) m_dconcrete_u=0.2; if(rebar==_T("HRB335")) m_dconcrete_u=0.15;}
     else if(concrete==_T("C35")) { if(rebar==_T("Q235")) m_dconcrete_u=0.2; if(rebar==_T("HRB335")) m_dconcrete_u=0.15;}
     else if(concrete==_T("C40")) { if(rebar==_T("Q235")) m_dconcrete_u=0.2; if(rebar==_T("HRB335")) m_dconcrete_u=0.15;}
     else if(concrete==_T("C45")) { if(rebar==_T("Q235")) m_dconcrete_u=0.2; if(rebar==_T("HRB335")) m_dconcrete_u=0.15;}
     else if(concrete==_T("C50")) { if(rebar==_T("Q235")) m_dconcrete_u=0.25; if(rebar==_T("HRB335")) m_dconcrete_u=0.2;}
     else if(concrete==_T("C55")) { if(rebar==_T("Q235")) m_dconcrete_u=0.25; if(rebar==_T("HRB335")) m_dconcrete_u=0.2;}
     else if(concrete==_T("C60")) { if(rebar==_T("Q235")) m_dconcrete_u=0.25; if(rebar==_T("HRB335")) m_dconcrete_u=0.2;}
	 
	 //钢筋的弹性模量及容许应力值 
	 if(rebar==_T("Q235"))        {m_dEs=210000.0; m_drebar=130.0;}
     else if(rebar==_T("HRB335")) {m_dEs=200000.0; m_drebar=180.0;}
}

/************************************************************************/
//	0－My最大值、2-Vz最大值	      
//  1－My最小值、3-Vz最小值      
//  My1－My中的活载弯矩值     //  My2－My中的恒载弯矩值 
/************************************************************************/
void CRCrailway::Get_Beam_force(int Elemi,int iPosi)
{
    T_RAIL_BEAM_FORCE beamForce;
    m_DataCtrl.Get_RailBeamForce(Elemi,iPosi,beamForce);
    for (int k=0;k<4;k++) 
	{
        dMy[k]  = beamForce.dMy[k];
        iMy[k]  = beamForce.iMy[k];
        dMy1[k] = beamForce.dMy1[k];
        dMy2[k] = beamForce.dMy2[k];
        dVz[k]  = beamForce.dVz[k];
        iVz[k]  = beamForce.iVz[k];		
        isaddforce[k] = beamForce.isAddForce[k]; //isaddforce该荷载是否为主力加附加力组合
    } 
}

/************************************************************************/
//	0－My最大值、Vz最大值、Nx最大值	      
//  1－My最小值、Vz最小值、Nx最小值        
//  My1－My中的活载弯矩值     //  My2－My中的恒载弯矩值 
/************************************************************************/
void CRCrailway::Get_Column_force(int Elemi,int iPosi,BOOL isSeismic)
{
    T_RAIL_COLUMN_FORCE columnForce;
    m_DataCtrl.Get_RailColumnForce(Elemi,iPosi,columnForce,isSeismic);
    for (int k=0;k<6;k++) 
	{
        dMy[k]  = columnForce.dMy[k];
        iMy[k]  = columnForce.iMy[k];
        dMy1[k] = columnForce.dMy1[k];
        dMy2[k] = columnForce.dMy2[k];
        dVz[k]  = columnForce.dVz[k];
        iVz[k]  = columnForce.iVz[k];
        dNx[k]  = columnForce.dNx[k];
        iNx[k]  = columnForce.iNx[k];      
        isaddforce[k] = columnForce.isAddForce[k]; //isaddforce该荷载是否为主力加附加力组合
    }
}
/************************************************************************/
//	0－My最大值、Vz最大值、Nx最大值	      
//  1－My最小值、Vz最小值、Nx最小值        
//  My1－My中的活载弯矩值     //  My2－My中的恒载弯矩值 
/************************************************************************/
void CRCrailway::Get_Seismic_Force(int Elemi,int iPosi)
{
    T_RAIL_COLUMN_FORCE columnForce;
    m_DataCtrl.Get_RailColumnForce(Elemi,iPosi,columnForce,TRUE);
    for (int k=0;k<6;k++) 
	{
        int temp = k;
        if ( k>= 4) temp = temp+2;    
		dMy[k]  = columnForce.dMy[temp];
		iMy[k]  = columnForce.iMy[temp];
		dMy1[k] = columnForce.dMy1[temp];
		dMy2[k] = columnForce.dMy2[temp];
		dMz[k]  = columnForce.dMz[temp];
		iMz[k]  = columnForce.iMz[temp];
        dNx[k]  = columnForce.dNx[temp];
        iNx[k]  = columnForce.iNx[temp];      
        isaddforce[k] = columnForce.isAddForce[temp]; //isaddforce该荷载是否为主力加附加力组合
    }
}

void CRCrailway::Isaddforce(int t)
{
	double dthird;
    m_dK = 2.0;
    if(isaddforce[t]==TRUE)
	{
		dthird=1.3;
	  if((m_drebar-130.0)<1.0E-07)   m_drebar=160.0;
		if((m_drebar-180.0)<1.0E-07)   m_drebar=230.0;
		m_dconcrete_c*=dthird;
		m_dconcrete_b*=dthird;
        m_dK = 1.6;
	}
}

void CRCrailway::beam_concrete_c() 
{    
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_concrete_c")); 
	//==================================================================================================
	
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Beam_concrete_c_W);
    sizeRes = sizeof(CONCRETE_C);  
	
    BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环	   
	{
        int iElemK = m_elemlist.GetAt(i);
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,0))  continue ;//add by maxiao 2007.05.16
        int iCount = 0;
		for(int j=0;j<2;j++)//单元两端截面的计算循环
		{   
			section_Initialize();
            get_section(iElemK,j,TRUE);
			
			force_Initialize();
			Get_Beam_force(iElemK,j);

            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b;			
            for(int t=0;t<2;t++)
			{				
				//
                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB;
				Isaddforce(t);
				
				T_Beam_concrete_c_W BeamConcD; // Input Data
				BeamConcD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				BeamConcD.nElemK   = iElemK; 				BeamConcD.nPart    = j;
				BeamConcD.db       = m_db; 	  			BeamConcD.dh       = m_dh;
				BeamConcD.db_f     = m_db_f; 	  		BeamConcD.dbf      = m_dbf;
				BeamConcD.dh_f     = m_dh_f; 	  		BeamConcD.dhf      = m_dhf;
				BeamConcD.das      = m_das; 	  		BeamConcD.da_s     = m_da_s;
				BeamConcD.dAs      = m_dAs; 	  		BeamConcD.dA_s     = m_dA_s;
				BeamConcD.das_d    = m_das_d; 	  	BeamConcD.da_s_d   = m_da_s_d;
				BeamConcD.dLo      = m_dLo; 	  		BeamConcD.dd       = m_dd;
				BeamConcD.dAsR     = m_dAsR; 	  		BeamConcD.dR       = m_dR;
				BeamConcD.dasR     = m_dasR; 	  		BeamConcD.dIy      = m_dIy;
				BeamConcD.dIz      = m_dIz; 	  		BeamConcD.dCzp     = m_dCzp;
				BeamConcD.dCzm     = m_dCzm; 	  		BeamConcD.dCyp     = m_dCyp;
				BeamConcD.dCym     = m_dCym; 	  		BeamConcD.dconcrete_c    = m_dconcrete_c;
				BeamConcD.dconcrete_b   = m_dconcrete_b; 	  			BeamConcD.dconcrete_shear   = m_dconcrete_shear;
				BeamConcD.dconcrete_tp  = m_dconcrete_tp; 	  		BeamConcD.drebar            = m_drebar;
				BeamConcD.dconcrete_u   = m_dconcrete_u; 	  	  	BeamConcD.dconcrete_wf      = m_dconcrete_wf;
				BeamConcD.dK1     = m_dK1; 	  		BeamConcD.da_K2     = m_da_K2;
				BeamConcD.dK      = m_dK; 	  		BeamConcD.dK3       = m_dK3;
				BeamConcD.dEc     = m_dEc; 	  		BeamConcD.dEs       = m_dEs;
				BeamConcD.dm      = m_dm; 	  		BeamConcD.dn        = m_dn;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++				
				CONCRETE_C Concrete_cD;  // Output Data
				Concrete_cD.Initialize();
				Concrete_cD.dM = dMy[t];
				Concrete_cD.dN = dNx[t];
				Concrete_cD.itype = (t==0 ? 9:10);
				Concrete_cD.Lcomk = iMy[t];
				Concrete_cD.bCHK =  m_DataCtrl.Get_ChkFlag(iElemK,j, 0);
				if (!Concrete_cD.bCHK)  continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&BeamConcD, &Concrete_cD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				
				//report-data 
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PrintOption(m_elemlist.GetAt(i),j,t);//true:print to report-data;
				if (bPrinti && bPrintTitle)
				{
					file.WriteString(_T("**BEGIN RC梁－混凝土压应力验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}	
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}
				
                //Change unit
                Concrete_cD.dM          = m_DataCtrl.Cng_MomentFromCodeToCurr(Concrete_cD.dM);
                Concrete_cD.dconcrete_c = m_DataCtrl.Cng_StressFromCodeToCurr(Concrete_cD.dconcrete_c);
                Concrete_cD.dconcrete_b = m_DataCtrl.Cng_StressFromCodeToCurr(Concrete_cD.dconcrete_b);
				Write_Command(_T("RC_BC"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j+1);//截面位置
				Write_Int(t+1);//最大/最小               
				Write_Int(Concrete_cD.Lcomk);//组合名称
				Write_Int(Concrete_cD.itype);//类型                
				Write_Double(Concrete_cD.dM);  
				Write_Double(Concrete_cD.dconcrete_c);
				Write_Double(Concrete_cD.dconcrete_b);		
                m_iTotLineNum++;
                iCount++;
			}
		}
        if(iCount > 0) m_BeamConKList.Add(iElemK);
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

//混凝土梁剪应力验算
void CRCrailway::beam_shear() 
{    
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_shear")); 
	//==================================================================================================
	
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Beam_shear_W);
    sizeRes = sizeof(CONCRETE_TP);  
	BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环
	{
        int iElemK = m_elemlist[i];
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,1))  continue ;//add by maxiao 2007.05.16
		double dconcrete_tp = 0.0,dconcrete_shear=0.0;
		if(!Get_concreteValue(iElemK, dconcrete_tp,dconcrete_shear)) continue;
        int iCount = 0;
		for(int j=0;j<2;j++)//单元两端截面的计算循环
		{
			section_Initialize();
            get_section(iElemK,j,TRUE);			
			force_Initialize();
			Get_Beam_force(iElemK,j);
// 			int 	iLcomkMax = 0,iLcomkMin = 0;
// 			double dSig_max = 0.0, dSig_min = 0.0;
// 			Get_Shear(iElemK,j,dSig_max, dSig_min, iLcomkMax, iLcomkMin);	
            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b; 
			for(int t=0;t<2;t++)//当前截面最大和最小剪应力验算
            { 
                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB;
				Isaddforce(t);
				T_Beam_shear_W BeamShearD; // Input Data
				BeamShearD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				BeamShearD.nElemK   = iElemK; 				BeamShearD.nPart    = j;
				BeamShearD.db       = m_db; 	  			BeamShearD.dh       = m_dh;
				BeamShearD.db_f     = m_db_f; 	  		BeamShearD.dbf      = m_dbf;
				BeamShearD.dh_f     = m_dh_f; 	  		BeamShearD.dhf      = m_dhf;
				BeamShearD.das      = m_das; 	  	  	BeamShearD.da_s     = m_da_s;
				BeamShearD.dAs      = m_dAs; 	  		  BeamShearD.dA_s     = m_dA_s;
				BeamShearD.das_d    = m_das_d; 	  	  BeamShearD.da_s_d   = m_da_s_d;
				BeamShearD.dLo      = m_dLo; 	  	  	BeamShearD.dd       = m_dd;
				BeamShearD.dAsR     = m_dAsR; 	  		BeamShearD.dR       = m_dR;
				BeamShearD.dasR     = m_dasR; 	  		BeamShearD.dIy      = m_dIy;
				BeamShearD.dIz      = m_dIz; 	  	  	BeamShearD.dCzp     = m_dCzp;
				BeamShearD.dCzm     = m_dCzm; 	  		BeamShearD.dCyp     = m_dCyp;
				BeamShearD.dCym     = m_dCym; 	  		BeamShearD.dconcrete_c    = m_dconcrete_c;
				BeamShearD.dconcrete_b   = m_dconcrete_b; 	  		BeamShearD.dconcrete_shear   = m_dconcrete_shear;
				BeamShearD.dconcrete_tp  = m_dconcrete_tp; 	  		BeamShearD.drebar            = m_drebar;
				BeamShearD.dconcrete_u   = m_dconcrete_u; 	    	BeamShearD.dconcrete_wf      = m_dconcrete_wf;
				BeamShearD.dK1     = m_dK1; 	  		BeamShearD.da_K2     = m_da_K2;
				BeamShearD.dK      = m_dK; 	  			BeamShearD.dK3       = m_dK3;
				BeamShearD.dEc     = m_dEc; 	  		BeamShearD.dEs       = m_dEs;
				BeamShearD.dm      = m_dm; 	  			BeamShearD.dn        = m_dn;
				
				CONCRETE_TP Concrete_TPD;  // Output Data
				Concrete_TPD.Initialize();
// 				Concrete_TPD.Lcomk = (t==0? iLcomkMax: iLcomkMin);
// 				Concrete_TPD.dshear = (t==0) ? dSig_max:dSig_min;
// 				Concrete_TPD.itype = t;
				//2-Vz最大值	3-Vz最小值
				Concrete_TPD.dM = dMy[t+2];
				Concrete_TPD.dV = dVz[t+2];
				Concrete_TPD.itype = (t==0 ? 5:6);
				Concrete_TPD.Lcomk = iVz[t+2];

				Concrete_TPD.dconcrete_shear = dconcrete_tp;
				Concrete_TPD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 1);
				if (!Concrete_TPD.bCHK) continue;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&BeamShearD, &Concrete_TPD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				
				//report-data 				
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,2);//true:print to report-data;	
				if (bPrinti && bPrintTitle) 
				{
					file.WriteString(_T("**BEGIN RC梁－中性轴处剪应力验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}
				
                //Change unit
                Concrete_TPD.dshear = m_DataCtrl.Cng_StressFromCodeToCurr(Concrete_TPD.dshear);
                Concrete_TPD.dconcrete_shear = m_DataCtrl.Cng_StressFromCodeToCurr(Concrete_TPD.dconcrete_shear);
				Write_Command(_T("RC_BTP"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j+1);//截面位置
				Write_Int(t+1);//最大/最小
                Write_Int(Concrete_TPD.Lcomk);
				Write_Int(Concrete_TPD.itype);//类型no type need output..
                Write_Double(Concrete_TPD.dV);	 
				Write_Double(Concrete_TPD.dshear);
				Write_Double(Concrete_TPD.dconcrete_shear);
                m_iTotLineNum++;
                iCount++;
			}	
		}
        if(iCount > 0) m_BeamShearKList.Add(iElemK);
    }  
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

//梁内钢筋的拉应力验算
void CRCrailway::beam_rebar_s() 
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_rebar_s")); 
	//==================================================================================================
	
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Beam_rebar_s_W);
    sizeRes = sizeof(REBAR_S);  
	
	BOOL bPrintTitle = TRUE;
	 for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环  
	 {
         int iElemK = m_elemlist.GetAt(i);
     	 //Check Calculating Item.
         if(!m_DataCtrl.Is_CalcItem(iElemK,0))  continue ;//add by maxiao 2007.05.16
         int iCount = 0;	 
		 for(int j=0;j<2;j++)//单元两端截面的计算循环
		 {
             //report-data 
             BOOL bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,6);//true:print to report-data;
			 section_Initialize();        
			 get_section(iElemK,j,TRUE);            
			 force_Initialize();
			 Get_Beam_force(iElemK,j);   
             double dConC = m_dconcrete_c;
             double dConB = m_dconcrete_b;
			 for(int t=0;t<2;t++)
			 {   	
                 m_dconcrete_c = dConC;
                 m_dconcrete_b = dConB; 
				 Isaddforce(t);
				 
				 T_Beam_rebar_s_W BeamRebarD; // Input Data
				 BeamRebarD.Initialize();
				 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				 BeamRebarD.nElemK   = iElemK; 				BeamRebarD.nPart    = j;
				 BeamRebarD.db       = m_db; 	  			BeamRebarD.dh       = m_dh;
				 BeamRebarD.db_f     = m_db_f; 	  		BeamRebarD.dbf      = m_dbf;
				 BeamRebarD.dh_f     = m_dh_f; 	  		BeamRebarD.dhf      = m_dhf;
				 BeamRebarD.das      = m_das; 	  		BeamRebarD.da_s     = m_da_s;
				 BeamRebarD.dAs      = m_dAs; 	  		BeamRebarD.dA_s     = m_dA_s;
				 BeamRebarD.das_d    = m_das_d; 	  	BeamRebarD.da_s_d   = m_da_s_d;
				 BeamRebarD.dLo      = m_dLo; 	  		BeamRebarD.dd       = m_dd;
				 BeamRebarD.dAsR     = m_dAsR; 	  		BeamRebarD.dR       = m_dR;
				 BeamRebarD.dasR     = m_dasR; 	  		BeamRebarD.dIy      = m_dIy;
				 BeamRebarD.dIz      = m_dIz; 	  		BeamRebarD.dCzp     = m_dCzp;
				 BeamRebarD.dCzm     = m_dCzm; 	  		BeamRebarD.dCyp     = m_dCyp;
				 BeamRebarD.dCym     = m_dCym; 	  		BeamRebarD.dconcrete_c    = m_dconcrete_c;
				 BeamRebarD.dconcrete_b   = m_dconcrete_b; 	  			BeamRebarD.dconcrete_shear   = m_dconcrete_shear;
				 BeamRebarD.dconcrete_tp  = m_dconcrete_tp; 	  		BeamRebarD.drebar            = m_drebar;
				 BeamRebarD.dconcrete_u   = m_dconcrete_u; 	  	  	BeamRebarD.dconcrete_wf      = m_dconcrete_wf;
				 BeamRebarD.dK1     = m_dK1; 	  		BeamRebarD.da_K2     = m_da_K2;
				 BeamRebarD.dK      = m_dK; 	  		BeamRebarD.dK3       = m_dK3;
				 BeamRebarD.dEc     = m_dEc; 	  		BeamRebarD.dEs       = m_dEs;
				 BeamRebarD.dm      = m_dm; 	  		BeamRebarD.dn        = m_dn;
				 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				 REBAR_S Rebar_SD;  // Output Data
				 Rebar_SD.Initialize();
				 Rebar_SD.itype = (t==0?9:10);
				 Rebar_SD.Lcomk = iMy[t]; 
				 Rebar_SD.dM    = dMy[t];
				 Rebar_SD.dN    = dNx[t]; 
				 Rebar_SD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 0);
				 if (!Rebar_SD.bCHK) continue;

				 //
				 T_DETAIL_STR DetailStr;
				 DetailStr.Initialize();
				 //!/
				 BOOL bVersionOK=FALSE;
				 //==================================================================================================
				 BOOL bCheck = (*pDgnCalc)(&BeamRebarD, &Rebar_SD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				 if(!bVersionOK) continue;
				 //==================================================================================================
				 //report-data
				 if (bPrinti && bPrintTitle)
				 {
					 file.WriteString(_T("**BEGIN RC梁－钢筋拉应力验算: [TB 10002.3--2005]\n"));
					 bPrintTitle = FALSE;
				 }
				 if (bPrinti) 
				 {
					 for (int p=0; p<DetailStr.arString.GetSize(); p++)
					 {
						 file.WriteString(DetailStr.CW2A(p));
					 }
				 }
				 
				 //Change unit
                 Rebar_SD.dM = m_DataCtrl.Cng_MomentFromCodeToCurr(Rebar_SD.dM);
                 Rebar_SD.drebar_s = m_DataCtrl.Cng_StressFromCodeToCurr(Rebar_SD.drebar_s);
                 Rebar_SD.drebar = m_DataCtrl.Cng_StressFromCodeToCurr(Rebar_SD.drebar);
				 Write_Command(_T("RC_BRS"));//验算名称
				 Write_Int(iElemK);//单元号
				 Write_Int(j+1);//截面位置
				 Write_Int(t+1);//最大/最小 
				 Write_Int(Rebar_SD.Lcomk);
				 Write_Int(Rebar_SD.itype);//类型                 
				 Write_Double(Rebar_SD.dM);
				 Write_Double(Rebar_SD.drebar_s);
				 Write_Double(Rebar_SD.drebar);
                 m_iTotLineNum++;
                 iCount++;         
			 }		 
		 }	   
         if(iCount > 0) m_BeamRebarKList.Add(iElemK);
	 }  	
	 if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

//当中性轴在腹板内时还需要验算板与梗相交处的剪应力验算(T形和工形截面)
void CRCrailway::beam_shear_t() 
{    
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_shear_t")); 
	//==================================================================================================
	
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Beam_shear_t_W);
    sizeRes = sizeof(BEAM_SHEAR_T);  
	
	BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环	   
	{
		UINT iElemK = m_elemlist.GetAt(i);
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,1))  continue ;//add by maxiao 2007.05.16
		double dconcrete_tp = 0.0,dconcrete_shear = 0.0;
		if(!Get_concreteValue(iElemK, dconcrete_tp,dconcrete_shear)) continue;
        int iCount = 0;
		for(int j=0;j<2;j++)//单元两端截面的计算循环
		{   
			section_Initialize();             
			get_section(iElemK,j,TRUE);
			force_Initialize();
			Get_Beam_force(iElemK,j);

// 			int 	iLcomkMax = 0,iLcomkMin = 0;
// 			double dSig_max = 0.0, dSig_min = 0.0;
// 			Get_Shear(iElemK,j,dSig_max, dSig_min, iLcomkMax, iLcomkMin);
            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b;
			for(int t=0;t<2;t++)//当前截面最大和最小内力的验算
			{        
                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB;
				Isaddforce(t);
				T_Beam_shear_t_W BeamShearD; // Input Data
				BeamShearD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				BeamShearD.nElemK   = iElemK; 				BeamShearD.nPart    = j;
				BeamShearD.db       = m_db; 	  			BeamShearD.dh       = m_dh;
				BeamShearD.db_f     = m_db_f; 	  		BeamShearD.dbf      = m_dbf;
				BeamShearD.dh_f     = m_dh_f; 	  		BeamShearD.dhf      = m_dhf;
				BeamShearD.das      = m_das; 	  		BeamShearD.da_s     = m_da_s;
				BeamShearD.dAs      = m_dAs; 	  		BeamShearD.dA_s     = m_dA_s;
				BeamShearD.das_d    = m_das_d; 	  	BeamShearD.da_s_d   = m_da_s_d;
				BeamShearD.dLo      = m_dLo; 	  		BeamShearD.dd       = m_dd;
				BeamShearD.dAsR     = m_dAsR; 	  		BeamShearD.dR       = m_dR;
				BeamShearD.dasR     = m_dasR; 	  		BeamShearD.dIy      = m_dIy;
				BeamShearD.dIz      = m_dIz; 	  		BeamShearD.dCzp     = m_dCzp;
				BeamShearD.dCzm     = m_dCzm; 	  		BeamShearD.dCyp     = m_dCyp;
				BeamShearD.dCym     = m_dCym; 	  		BeamShearD.dconcrete_c    = m_dconcrete_c;
				BeamShearD.dconcrete_b   = m_dconcrete_b; 	  			BeamShearD.dconcrete_shear   = m_dconcrete_shear;
				BeamShearD.dconcrete_tp  = m_dconcrete_tp; 	  		BeamShearD.drebar            = m_drebar;
				BeamShearD.dconcrete_u   = m_dconcrete_u; 	  	  	BeamShearD.dconcrete_wf      = m_dconcrete_wf;
				BeamShearD.dK1     = m_dK1; 	  		BeamShearD.da_K2     = m_da_K2;
				BeamShearD.dK      = m_dK; 	  		BeamShearD.dK3       = m_dK3;
				BeamShearD.dEc     = m_dEc; 	  		BeamShearD.dEs       = m_dEs;
				BeamShearD.dm      = m_dm; 	  		BeamShearD.dn        = m_dn;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				
				BEAM_SHEAR_T Shear_tD;  // Output Data
				Shear_tD.Initialize();
// 				Shear_tD.Lcomk = (t==0? iLcomkMax: iLcomkMin);
// 				Shear_tD.dshear = (t==0) ? dSig_max:dSig_min;
// 				Shear_tD.itype = t;
				//2-Vz最大值	3-Vz最小值
				Shear_tD.dM = dMy[t+2];
				Shear_tD.dV = dVz[t+2];
				Shear_tD.itype = (t==0 ? 5:6);
				Shear_tD.Lcomk = iVz[t+2];
				Shear_tD.dconcrete_shear = dconcrete_shear;
				Shear_tD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 1);
				if (!Shear_tD.bCHK)  continue;
				
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&BeamShearD, &Shear_tD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				
                //report-data //true:print to report-data;
				BOOL bPrinti = FALSE;
                bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,2);//2-shear stress
				if (bPrinti && bPrintTitle)
				{
					file.WriteString(_T("**BEGIN RC梁－上翼缘剪应力验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}
				
				//Change unit
				Shear_tD.dshear = m_DataCtrl.Cng_StressFromCodeToCurr(Shear_tD.dshear);
				Shear_tD.dshear_t = m_DataCtrl.Cng_StressFromCodeToCurr(Shear_tD.dshear_t);
				Shear_tD.dconcrete_shear = m_DataCtrl.Cng_StressFromCodeToCurr(Shear_tD.dconcrete_shear);
				Write_Command(_T("RC_BTS"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j+1);//截面位置
				Write_Int(t+1);//最大/最小                   
				Write_Int(Shear_tD.Lcomk);
				Write_Int(Shear_tD.itype);//类型                   
				Write_Double(Shear_tD.dshear);
				Write_Double(Shear_tD.dshear_t);
				Write_Double(Shear_tD.dconcrete_shear);
				m_iTotLineNum++;
				iCount++;   
			}
		}
		if(iCount > 0) m_BeamSheartKList.Add(iElemK);
	}            
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

//梁梗与翼缘相交处的剪应力验算(工形截面)
void CRCrailway::beam_shear_b() 
{  
 	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_shear_b")); 
	//==================================================================================================
	
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Beam_shear_b_W);
    sizeRes = sizeof(BEAM_SHEAR_B);  
	
	BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环
	{
		UINT iElemK = m_elemlist.GetAt(i);
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,1))  continue ;//add by maxiao 2007.05.16
		int 	iLcomkMax = 0,iLcomkMin = 0;
		double dconcrete_tp = 0.0,dconcrete_shear = 0.0;
		if(!Get_concreteValue(iElemK, dconcrete_tp,dconcrete_shear)) continue;
        int iCount = 0;
		for(int j=0;j<2;j++)//单元两端截面的计算循环
		{
			section_Initialize();            
			get_section(iElemK,j,TRUE);      
			force_Initialize();
			Get_Beam_force(iElemK,j);			
// 			int 	iLcomkMax = 0,iLcomkMin = 0;
// 			double dSig_max = 0.0, dSig_min = 0.0;
// 			Get_Shear(iElemK,j,dSig_max, dSig_min, iLcomkMax, iLcomkMin);
            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b;
			for(int t=0;t<2;t++)//当前截面最大和最小内力的验算			 
            {   	 
                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB;		
				Isaddforce(t);
				T_Beam_shear_b_W BeamShearD; // Input Data
				BeamShearD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				BeamShearD.nElemK   = iElemK; 				BeamShearD.nPart    = j;
				BeamShearD.db       = m_db; 	  			BeamShearD.dh       = m_dh;
				BeamShearD.db_f     = m_db_f; 	  		BeamShearD.dbf      = m_dbf;
				BeamShearD.dh_f     = m_dh_f; 	  		BeamShearD.dhf      = m_dhf;
				BeamShearD.das      = m_das; 	  		BeamShearD.da_s     = m_da_s;
				BeamShearD.dAs      = m_dAs; 	  		BeamShearD.dA_s     = m_dA_s;
				BeamShearD.das_d    = m_das_d; 	  	BeamShearD.da_s_d   = m_da_s_d;
				BeamShearD.dLo      = m_dLo; 	  		BeamShearD.dd       = m_dd;
				BeamShearD.dAsR     = m_dAsR; 	  		BeamShearD.dR       = m_dR;
				BeamShearD.dasR     = m_dasR; 	  		BeamShearD.dIy      = m_dIy;
				BeamShearD.dIz      = m_dIz; 	  		BeamShearD.dCzp     = m_dCzp;
				BeamShearD.dCzm     = m_dCzm; 	  		BeamShearD.dCyp     = m_dCyp;
				BeamShearD.dCym     = m_dCym; 	  		BeamShearD.dconcrete_c    = m_dconcrete_c;
				BeamShearD.dconcrete_b   = m_dconcrete_b; 	  			BeamShearD.dconcrete_shear   = m_dconcrete_shear;
				BeamShearD.dconcrete_tp  = m_dconcrete_tp; 	  		BeamShearD.drebar            = m_drebar;
				BeamShearD.dconcrete_u   = m_dconcrete_u; 	  	  	BeamShearD.dconcrete_wf      = m_dconcrete_wf;
				BeamShearD.dK1     = m_dK1; 	  		BeamShearD.da_K2     = m_da_K2;
				BeamShearD.dK      = m_dK; 	  		BeamShearD.dK3       = m_dK3;
				BeamShearD.dEc     = m_dEc; 	  		BeamShearD.dEs       = m_dEs;
				BeamShearD.dm      = m_dm; 	  		BeamShearD.dn        = m_dn;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++				
				BEAM_SHEAR_B Shear_bD;  // Output Data
				Shear_bD.Initialize();
// 				Shear_bD.Lcomk = (t==0? iLcomkMax: iLcomkMin);
// 				Shear_bD.dshear = (t==0) ? dSig_max:dSig_min;
// 				Shear_bD.itype = t;
				//2-Vz最大值	3-Vz最小值
				Shear_bD.dM = dMy[t+2];
				Shear_bD.dV = dVz[t+2];
				Shear_bD.itype = (t==0 ? 5:6);
				Shear_bD.Lcomk = iVz[t+2];

				Shear_bD.dconcrete_shear = dconcrete_shear;
				Shear_bD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 1);
				if (!Shear_bD.bCHK) continue;
				
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&BeamShearD, &Shear_bD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				
				//report-data //true:print to report-data;
		    BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,2);//2-shear stress  
				if (bPrinti && bPrintTitle)
				{
				  file.WriteString(_T("**BEGIN RC梁－下翼缘剪应力验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}
				
				//Change unit
                Shear_bD.dshear = m_DataCtrl.Cng_StressFromCodeToCurr(Shear_bD.dshear);
                Shear_bD.dshear_b = m_DataCtrl.Cng_StressFromCodeToCurr(Shear_bD.dshear_b);
                Shear_bD.dconcrete_shear = m_DataCtrl.Cng_StressFromCodeToCurr(Shear_bD.dconcrete_shear);
				Write_Command(_T("RC_BBS"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j+1);//截面位置
				Write_Int(t+1);//最大/最小                   
				Write_Int(Shear_bD.Lcomk);
				Write_Int(Shear_bD.itype);//类型
                Write_Double(Shear_bD.dshear);
				Write_Double(Shear_bD.dshear_b);
				Write_Double(Shear_bD.dconcrete_shear);
                m_iTotLineNum++;
                iCount++;          
			}
		}
        if(iCount > 0) m_BeamShearbKList.Add(iElemK);
	}  	 
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

//混凝土梁裂缝宽度验算
void CRCrailway::beam_crack()
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_crack_RC")); 
	//==================================================================================================
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Beam_crack_rc_rail_W);
    sizeRes = sizeof(CRACK);
    
    BOOL bPrintTitle = TRUE;
	 for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环	   
	 {
         int iElemK = m_elemlist.GetAt(i);
     	 //Check Calculating Item.
         if(!m_DataCtrl.Is_CalcItem(iElemK,0))  continue ;//add by maxiao 2007.05.16
         int iCount = 0;
		 for(int j=0;j<2;j++)//单元两端截面的计算循环
		 {   
			 section_Initialize();            
			 get_section(iElemK,j,TRUE);             
			 force_Initialize();
			 Get_Beam_force(iElemK,j);
             double dConC = m_dconcrete_c;
             double dConB = m_dconcrete_b;
             for(int t=0;t<2;t++)//当前截面最大和最小内力时的验算
             { 
                 m_dconcrete_c = dConC;
                 m_dconcrete_b = dConB;
				 Isaddforce(t);
				 
				 T_Beam_crack_rc_rail_W BeamCrackD; // Input Data
				 BeamCrackD.Initialize();
				 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				 BeamCrackD.dMy   = dMy[t];           BeamCrackD.t        = t;  
				 BeamCrackD.dMy1  = dMy1[t];          BeamCrackD.dMy2     = dMy2[t];
				 BeamCrackD.nElemK   = iElemK;  			BeamCrackD.nPart    = j;
				 BeamCrackD.db       = m_db; 	  			BeamCrackD.dh       = m_dh;
				 BeamCrackD.db_f     = m_db_f; 	  		BeamCrackD.dbf      = m_dbf;
				 BeamCrackD.dh_f     = m_dh_f; 	  		BeamCrackD.dhf      = m_dhf;
				 BeamCrackD.das      = m_das; 	  		BeamCrackD.da_s     = m_da_s;
				 BeamCrackD.dAs      = m_dAs; 	  		BeamCrackD.dA_s     = m_dA_s;
				 BeamCrackD.das_d    = m_das_d; 	  	BeamCrackD.da_s_d   = m_da_s_d;
				 BeamCrackD.dLo      = m_dLo; 	  		BeamCrackD.dd       = m_dd;
				 BeamCrackD.dAsR     = m_dAsR; 	  		BeamCrackD.dR       = m_dR;
				 BeamCrackD.dasR     = m_dasR; 	  		BeamCrackD.dIy      = m_dIy;
				 BeamCrackD.dIz      = m_dIz; 	  		BeamCrackD.dCzp     = m_dCzp;
				 BeamCrackD.dCzm     = m_dCzm; 	  		BeamCrackD.dCyp     = m_dCyp;
				 BeamCrackD.dCym     = m_dCym; 	  		BeamCrackD.dconcrete_c    = m_dconcrete_c;
				 BeamCrackD.dconcrete_b   = m_dconcrete_b; 	  			BeamCrackD.dconcrete_shear   = m_dconcrete_shear;
				 BeamCrackD.dconcrete_tp  = m_dconcrete_tp; 	  		BeamCrackD.drebar            = m_drebar;
				 BeamCrackD.dconcrete_u   = m_dconcrete_u; 	  	  	
				 BeamCrackD.dconcrete_wf      =(isaddforce[t] ? 1.2:1.0) *m_dconcrete_wf;
				 BeamCrackD.dK1     = m_dK1; 	  		BeamCrackD.da_K2     = m_da_K2;
				 BeamCrackD.dK      = m_dK; 	  		BeamCrackD.dK3       = m_dK3;
				 BeamCrackD.dEc     = m_dEc; 	  		BeamCrackD.dEs       = m_dEs;
				 BeamCrackD.dm      = m_dm; 	  		BeamCrackD.dn        = m_dn;
				 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				 CRACK CrackD;  // Output Data
				 CrackD.Initialize();
				 CrackD.Lcomk = iMy[t];
				 CrackD.itype =(t==0?9:10);
				 CrackD.iKind =1;
				 CrackD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 0);
				 if (!CrackD.bCHK) continue;
				 //
				 T_DETAIL_STR DetailStr;
				 DetailStr.Initialize();
				 //!/
				 BOOL bVersionOK=FALSE;
				 //==================================================================================================
				 BOOL bCheck = (*pDgnCalc)(&BeamCrackD, &CrackD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				 if(!bVersionOK) continue;
				 //==================================================================================================
				 
				 //report-data 
				 BOOL bPrinti = FALSE;
                 bPrinti = m_DataCtrl.Get_PrintOption(m_elemlist.GetAt(i),j,t);//true:print to report-data;
				 if (bPrinti && bPrintTitle)
				 {
					 file.WriteString(_T("**BEGIN RC梁－裂缝宽度验算: [TB 10002.3--2005]\n"));
					 bPrintTitle = FALSE;
				 }	
				 if (bPrinti) 
				 {
					 for (int p=0; p<DetailStr.arString.GetSize(); p++)
					 {
						 file.WriteString(DetailStr.CW2A(p));
					 }
				 }
				 
				 //Change unit
                 CrackD.drebar_s = m_DataCtrl.Cng_StressFromCodeToCurr(CrackD.drebar_s);
                 CrackD.dwf = m_DataCtrl.Cng_LengthFromCodeToCurr(CrackD.dwf,1);
                 CrackD.dconcrete_wf = m_DataCtrl.Cng_LengthFromCodeToCurr(CrackD.dconcrete_wf,1);
				 Write_Command(_T("RC_BK"));//验算名称
				 Write_Int(iElemK);//单元号
				 Write_Int(j+1);//截面位置
				 Write_Int(t+1);//最大/最小
                 Write_Int(CrackD.Lcomk);
				 Write_Int(CrackD.itype);//类型
				 Write_Int(CrackD.iKind);
                 Write_Double(CrackD.drebar_s);
				 Write_Double(CrackD.dwf);
				 Write_Double(CrackD.dconcrete_wf);
                 m_iTotLineNum++;
                 iCount++;
			 }
		 }
         if(iCount > 0) m_BeamCrackKList.Add(iElemK);
	 }  
	 if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

/************************************************************************/
/* 得到中性轴处最大、最小剪应力                                         */
/************************************************************************/
BOOL CRCrailway::Get_Shear(INT elemK,INT sectioni, double& dSig_max, double& dSig_min, int& iLcomkMax,int& iLcomkMin)
{
	iLcomkMax = 0,iLcomkMin = 0;
	dSig_max = 0.0, dSig_min = 0.0;
	CArray<T_LCOM_K,T_LCOM_K> arLcomKey;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKey);
	T_LCOM_D LcomData;
	for(int il=0; il<arLcomKey.GetSize(); il++)
	{
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKey.GetAt(il), LcomData);
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData, 0, 0);
		T_STSS_D t_stss;
		m_pDoc->m_pPostCtrl->GetStssNew(elemK, 0, 0, &t_stss, 2);//2:主应力
		for (int kj=7;kj<9;kj++)
		{
			double dshear = t_stss.dblStress[4*sectioni][kj][6] + t_stss.dblStress[4*sectioni][kj][7] - 0.55*t_stss.dblStress[4*sectioni][kj][5];
			dshear = m_DataCtrl.Cng_StressFromCurrToCode(dshear);//change Unit
			if (dshear >= dSig_max) 
			{
				dSig_max = dshear; iLcomkMax = il+1; 
			}else{
				dSig_min = dshear; iLcomkMin = il+1; 
			}
		}
	}	
	return TRUE;
}

BOOL CRCrailway::Get_concreteValue(INT elemk,double& concrete_tp ,double&  dconcrete_shear)
{
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetElem(elemk,ElemD))	{ASSERT(0); return FALSE;}
	// Get Matl Data.
	T_MATD_D MatdD;	MatdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat,MatdD))	{ASSERT(0); return FALSE;}
	if(MatdD.Data1.CodeName!=MATLCODE_CON_TB05)	{ASSERT(0); return FALSE;}
    CString concrete = MatdD.Data1.CodeMatlName; //混凝土的标号 
	if(concrete==_T("C20")) { concrete_tp=1.53; dconcrete_shear=0.85;}     
	else if(concrete==_T("C25")) { concrete_tp=1.80; dconcrete_shear=1.0; }     
	else if(concrete==_T("C30")) { concrete_tp=1.98; dconcrete_shear=1.1; }     
	else if(concrete==_T("C35")) { concrete_tp=2.25; dconcrete_shear=1.25;}     
	else if(concrete==_T("C40")) { concrete_tp=2.43; dconcrete_shear=1.35;}    
	else if(concrete==_T("C45")) { concrete_tp=2.61; dconcrete_shear=1.45;}    
	else if(concrete==_T("C50")) { concrete_tp=2.79; dconcrete_shear=1.55;}   
	else if(concrete==_T("C55")) { concrete_tp=2.97; dconcrete_shear=1.65;}  
	else if(concrete==_T("C60")) { concrete_tp=3.15; dconcrete_shear=1.75;}
    else return FALSE;

    //for check matl.
    if(MatdD.Rebar_CodeName != MATLCODE_CON_TB05) return FALSE;   
    if(MatdD.MainRebar_RebarName!=_T("Q235") && MatdD.MainRebar_RebarName!=_T("HRB335")) return FALSE;      
    if(MatdD.SubRebar_RebarName!=_T("Q235") && MatdD.SubRebar_RebarName!=_T("HRB335")) return FALSE;      

	return TRUE;
}


//混凝土柱压应力验算
void CRCrailway::column_concrete_c(BOOL seismic)
{    	 
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_concrete_c")); 
	//==================================================================================================
	
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Column_concrete_c_W);
    sizeRes = sizeof(CONCRETE_C);  
	
	BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)
	{	 
        int iElemK = m_elemlist.GetAt(i);
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,2))  continue ;//add by maxiao 2007.05.16
        int iCount = 0;
		for(int j=0;j<2;j++)		 
		{
			section_Initialize();            
			get_section(iElemK,j,FALSE);   
            
			force_Initialize();            
			Get_Column_force(m_elemlist.GetAt(i),j,seismic);    

            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b;
            for(int t=0;t<4;t++)			
            {   
                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB;
                if(t==0) Isaddforce(1);
                else  	 Isaddforce(t);
				
				T_Column_concrete_c_W ColmConc_cD; // Input Data
				ColmConc_cD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ColmConc_cD.nElemK   = iElemK;   			ColmConc_cD.nPart    = j;
				ColmConc_cD.bseismic = seismic;       ColmConc_cD.t        = t;				
				ColmConc_cD.db       = m_db; 	  			ColmConc_cD.dh       = m_dh;
				ColmConc_cD.db_f     = m_db_f; 	  		ColmConc_cD.dbf      = m_dbf;
				ColmConc_cD.dh_f     = m_dh_f; 	  		ColmConc_cD.dhf      = m_dhf;
				ColmConc_cD.das      = m_das; 	  		ColmConc_cD.da_s     = m_da_s;
				ColmConc_cD.dAs      = m_dAs; 	  		ColmConc_cD.dA_s     = m_dA_s;
				ColmConc_cD.das_d    = m_das_d; 	  	ColmConc_cD.da_s_d   = m_da_s_d;
				ColmConc_cD.dLo      = m_dLo; 	  		ColmConc_cD.dd       = m_dd;
				ColmConc_cD.dAsR     = m_dAsR; 	  		ColmConc_cD.dR       = m_dR;
				ColmConc_cD.dasR     = m_dasR; 	  		ColmConc_cD.dIy      = m_dIy;
				ColmConc_cD.dIz      = m_dIz; 	  		ColmConc_cD.dCzp     = m_dCzp;
				ColmConc_cD.dCzm     = m_dCzm; 	  		ColmConc_cD.dCyp     = m_dCyp;
				ColmConc_cD.dCym     = m_dCym; 	  		ColmConc_cD.dconcrete_c    = m_dconcrete_c;
				ColmConc_cD.dconcrete_b   = m_dconcrete_b; 	  			ColmConc_cD.dconcrete_shear   = m_dconcrete_shear;
				ColmConc_cD.dconcrete_tp  = m_dconcrete_tp; 	  		ColmConc_cD.drebar            = m_drebar;
				ColmConc_cD.dconcrete_u   = m_dconcrete_u; 	  	  	ColmConc_cD.dconcrete_wf      = m_dconcrete_wf;
				ColmConc_cD.dK1     = m_dK1; 	  		ColmConc_cD.da_K2     = m_da_K2;
				ColmConc_cD.dK      = m_dK; 	  		ColmConc_cD.dK3       = m_dK3;
				ColmConc_cD.dEc     = m_dEc; 	  		ColmConc_cD.dEs       = m_dEs;
				ColmConc_cD.dm      = m_dm; 	  		ColmConc_cD.dn        = m_dn;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				CONCRETE_C Conc_CD;  // Output Data
				Conc_CD.Initialize();
				Conc_CD.dM=dMy[t];				Conc_CD.dN=dNx[t];
				if (t==0) 	    {	Conc_CD.dN=dNx[1];  Conc_CD.dM = .0; Conc_CD.Lcomk=iNx[1]; Conc_CD.itype=1;}//FMin
				else if (t==1)	{ Conc_CD.Lcomk=iNx[t];     Conc_CD.itype=2;}//FMin-My
				else if (t==2) 	{ Conc_CD.Lcomk=iMy[t];     Conc_CD.itype=9;}//MyMax
				else if (t==3) 	{	Conc_CD.Lcomk=iMy[t];     Conc_CD.itype=10;}//MyMin
				Conc_CD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 2);
				if (!Conc_CD.bCHK) continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&ColmConc_cD, &Conc_CD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				//report-data 
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,5);//true:print to report-data;
				if (bPrinti && bPrintTitle)
				{
					if(seismic==TRUE) file.WriteString(_T("**BEGIN 抗震柱－混凝土压应力验算: [GB 50111--2006]\n"));
					else              file.WriteString(_T("**BEGIN RC柱－混凝土压应力验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}
								
                //change unit
                Conc_CD.dM      =m_DataCtrl.Cng_MomentFromCodeToCurr(Conc_CD.dM    );
                Conc_CD.dN      =m_DataCtrl.Cng_ForceFromCodeToCurr(Conc_CD.dN    );
                Conc_CD.dconcrete_c =m_DataCtrl.Cng_StressFromCodeToCurr(Conc_CD.dconcrete_c    );
                Conc_CD.dconcrete_b =m_DataCtrl.Cng_StressFromCodeToCurr(Conc_CD.dconcrete_b    );
				
                if(!bCheck) continue;
                if (seismic)   Write_Command(_T("S_CC"));//验算名称
				else           Write_Command(_T("RC_CC"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j);//截面位置
				Write_Int(t);//验算截面位置				
                Write_Int(Conc_CD.Lcomk);//组合名称
				Write_Int(Conc_CD.itype);//类型
                Write_Double(Conc_CD.dM);
                Write_Double(Conc_CD.dN);
				Write_Double(Conc_CD.dconcrete_c);
				Write_Double(Conc_CD.dconcrete_b);
                m_iTotLineNum++;
                iCount++;
            }
        }
        if(iCount > 0) {
            if(seismic) m_SeismicConKList.Add(iElemK);
            else        m_ColumnConKList.Add(iElemK);
        }
    } 
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}



//柱内钢筋压应力验算
void CRCrailway::column_rebar_c(BOOL seismic)
{    
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_rebar_c")); 
	//==================================================================================================
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Column_rebar_c_W);
    sizeRes = sizeof(REBAR_C);  
	
	BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)	   
	{
        int iElemK = m_elemlist.GetAt(i);
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,2))  continue ;//add by maxiao 2007.05.16
        int iCount = 0;
		for(int j=0;j<2;j++)
		{   
			section_Initialize();             
			get_section(iElemK,j,FALSE);     
            
			force_Initialize();            
			Get_Column_force(iElemK,j,seismic);     

            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b;
			for(int t=0;t<4;t++)		 
			{           
                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB;
                if(t==0) Isaddforce(1);
                else 			Isaddforce(t);
				
				T_Column_rebar_c_W ColmRebarD; // Input Data
				ColmRebarD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ColmRebarD.nElemK   = iElemK;   			ColmRebarD.nPart    = j;
				ColmRebarD.bseismic = seismic;       ColmRebarD.t        = t;				
				ColmRebarD.db       = m_db; 	  			ColmRebarD.dh       = m_dh;
				ColmRebarD.db_f     = m_db_f; 	  		ColmRebarD.dbf      = m_dbf;
				ColmRebarD.dh_f     = m_dh_f; 	  		ColmRebarD.dhf      = m_dhf;
				ColmRebarD.das      = m_das; 	  		ColmRebarD.da_s     = m_da_s;
				ColmRebarD.dAs      = m_dAs; 	  		ColmRebarD.dA_s     = m_dA_s;
				ColmRebarD.das_d    = m_das_d; 	  	ColmRebarD.da_s_d   = m_da_s_d;
				ColmRebarD.dLo      = m_dLo; 	  		ColmRebarD.dd       = m_dd;
				ColmRebarD.dAsR     = m_dAsR; 	  		ColmRebarD.dR       = m_dR;
				ColmRebarD.dasR     = m_dasR; 	  		ColmRebarD.dIy      = m_dIy;
				ColmRebarD.dIz      = m_dIz; 	  		ColmRebarD.dCzp     = m_dCzp;
				ColmRebarD.dCzm     = m_dCzm; 	  		ColmRebarD.dCyp     = m_dCyp;
				ColmRebarD.dCym     = m_dCym; 	  		ColmRebarD.dconcrete_c    = m_dconcrete_c;
				ColmRebarD.dconcrete_b   = m_dconcrete_b; 	  			ColmRebarD.dconcrete_shear   = m_dconcrete_shear;
				ColmRebarD.dconcrete_tp  = m_dconcrete_tp; 	  		ColmRebarD.drebar            = m_drebar;
				ColmRebarD.dconcrete_u   = m_dconcrete_u; 	  	  	ColmRebarD.dconcrete_wf      = m_dconcrete_wf;
				ColmRebarD.dK1     = m_dK1; 	  		ColmRebarD.da_K2     = m_da_K2;
				ColmRebarD.dK      = m_dK; 	  		ColmRebarD.dK3       = m_dK3;
				ColmRebarD.dEc     = m_dEc; 	  		ColmRebarD.dEs       = m_dEs;
				ColmRebarD.dm      = m_dm; 	  		ColmRebarD.dn        = m_dn;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				REBAR_C rebar_CD;  // Output Data
				rebar_CD.Initialize();
				rebar_CD.dM = dMy[t];   
				rebar_CD.dN = dNx[t];
				if (t==0) {	 rebar_CD.dN=dNx[1];  rebar_CD.dM = .0; rebar_CD.Lcomk=iNx[1]; rebar_CD.itype=1;	}//FMin
				if (t==1) {	 rebar_CD.Lcomk=iNx[t];       rebar_CD.itype=2;	}//FMin-My
				if (t==2) {	 rebar_CD.Lcomk=iMy[t];       rebar_CD.itype=9;	}//MyMax
				if (t==3) {	 rebar_CD.Lcomk=iMy[t];       rebar_CD.itype=10;}//MyMin

				rebar_CD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 2);
				if (!rebar_CD.bCHK) continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&ColmRebarD, &rebar_CD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				//report-data 
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,7);//true:print to report-data;		
				if (bPrinti && bPrintTitle) 
				{
					if(seismic==TRUE) file.WriteString(_T("**BEGIN 抗震柱－钢筋压应力验算: [GB 50111--2006]\n"));
					else              file.WriteString(_T("**BEGIN RC柱－钢筋压应力验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}				
                //change unit
                rebar_CD.dM    =m_DataCtrl.Cng_MomentFromCodeToCurr(rebar_CD.dM    );
                rebar_CD.dN    =m_DataCtrl.Cng_ForceFromCodeToCurr(rebar_CD.dN    );
                rebar_CD.drebar_c      =m_DataCtrl.Cng_StressFromCodeToCurr(rebar_CD.drebar_c    );
                rebar_CD.drebar        =m_DataCtrl.Cng_StressFromCodeToCurr(rebar_CD.drebar    );
				
                if (seismic)    Write_Command(_T("S_CRC"));//验算名称
				else            Write_Command(_T("RC_CRC"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j);//截面位置
				Write_Int(t);//验算位置
                Write_Int(rebar_CD.Lcomk);//组合名称
				Write_Int(rebar_CD.itype);//类型
                Write_Double(rebar_CD.dM);
                Write_Double(rebar_CD.dN);
				Write_Double(rebar_CD.drebar_c);
				Write_Double(rebar_CD.drebar);
                m_iTotLineNum++;
                iCount++;
			}
        }
        if(iCount > 0) {
            if(seismic) m_SeismicRebarcKList.Add(iElemK);
            else        m_ColumnRebarcKList.Add(iElemK);
        }
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

void CRCrailway::column_rebar_s(BOOL seismic)
{ 
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_rebar_s")); 
	//==================================================================================================
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Column_rebar_s_W);
    sizeRes = sizeof(REBAR_S);  
	
	BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环	   
	{
        int iElemK = m_elemlist.GetAt(i);
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,2))  continue ;//add by maxiao 2007.05.16
        int iCount = 0;
		for(int j=0;j<2;j++)//单元两端截面的计算循环 		
		{   			 
			section_Initialize();            
			get_section(iElemK,j,FALSE);     
			force_Initialize();            
			Get_Column_force(iElemK,j,seismic);   
            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b;      
			for(int t=0;t<4;t++)			 
			{   		        
                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB; 
				Isaddforce(t);
				T_Column_rebar_s_W ColmRebarD; // Input Data
				ColmRebarD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ColmRebarD.nElemK   = iElemK;   			ColmRebarD.nPart    = j;
				ColmRebarD.bseismic = seismic;       ColmRebarD.t        = t;				
				ColmRebarD.db       = m_db; 	  			ColmRebarD.dh       = m_dh;
				ColmRebarD.db_f     = m_db_f; 	  		ColmRebarD.dbf      = m_dbf;
				ColmRebarD.dh_f     = m_dh_f; 	  		ColmRebarD.dhf      = m_dhf;
				ColmRebarD.das      = m_das; 	  		ColmRebarD.da_s     = m_da_s;
				ColmRebarD.dAs      = m_dAs; 	  		ColmRebarD.dA_s     = m_dA_s;
				ColmRebarD.das_d    = m_das_d; 	  	ColmRebarD.da_s_d   = m_da_s_d;
				ColmRebarD.dLo      = m_dLo; 	  		ColmRebarD.dd       = m_dd;
				ColmRebarD.dAsR     = m_dAsR; 	  		ColmRebarD.dR       = m_dR;
				ColmRebarD.dasR     = m_dasR; 	  		ColmRebarD.dIy      = m_dIy;
				ColmRebarD.dIz      = m_dIz; 	  		ColmRebarD.dCzp     = m_dCzp;
				ColmRebarD.dCzm     = m_dCzm; 	  		ColmRebarD.dCyp     = m_dCyp;
				ColmRebarD.dCym     = m_dCym; 	  		ColmRebarD.dconcrete_c    = m_dconcrete_c;
				ColmRebarD.dconcrete_b   = m_dconcrete_b; 	  			ColmRebarD.dconcrete_shear   = m_dconcrete_shear;
				ColmRebarD.dconcrete_tp  = m_dconcrete_tp; 	  		ColmRebarD.drebar            = m_drebar;
				ColmRebarD.dconcrete_u   = m_dconcrete_u; 	  	  	ColmRebarD.dconcrete_wf      = m_dconcrete_wf;
				ColmRebarD.dK1     = m_dK1; 	  		ColmRebarD.da_K2     = m_da_K2;
				ColmRebarD.dK      = m_dK; 	  		ColmRebarD.dK3       = m_dK3;
				ColmRebarD.dEc     = m_dEc; 	  		ColmRebarD.dEs       = m_dEs;
				ColmRebarD.dm      = m_dm; 	  		ColmRebarD.dn        = m_dn;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				REBAR_S rebar_SD;  // Output Data
				rebar_SD.Initialize();
				rebar_SD.dM = dMy[t];   
				rebar_SD.dN = dNx[t];
				if (t==0) {	 rebar_SD.Lcomk=iNx[t];       rebar_SD.itype=1;	}
				if (t==1) {	 rebar_SD.Lcomk=iNx[t];       rebar_SD.itype=2;	}
				if (t==2) {	 rebar_SD.Lcomk=iMy[t];       rebar_SD.itype=9;	}
				if (t==3) {	 rebar_SD.Lcomk=iMy[t];       rebar_SD.itype=10;}		
				rebar_SD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 2);
				if (!rebar_SD.bCHK) continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&ColmRebarD, &rebar_SD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				//report-data 
		    BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,6);//true:print to report-data;
				if (bPrinti && bPrintTitle)
				{
					if(seismic==TRUE) file.WriteString(_T("**BEGIN 抗震柱－钢筋拉应力验算: [GB 50111--2006]\n"));
					else              file.WriteString(_T("**BEGIN RC柱－钢筋拉应力验算: [TB 10002.3--2005]\n"));    
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}				
				//change unit
                rebar_SD.dM        =m_DataCtrl.Cng_MomentFromCodeToCurr(rebar_SD.dM   );
                rebar_SD.dN        =m_DataCtrl.Cng_ForceFromCodeToCurr(rebar_SD.dN    );
                rebar_SD.drebar_s  =m_DataCtrl.Cng_StressFromCodeToCurr(rebar_SD.drebar_s  );
                rebar_SD.drebar    =m_DataCtrl.Cng_StressFromCodeToCurr(rebar_SD.drebar    );
                if (seismic)   Write_Command(_T("S_CRS"));//验算名称
				else           Write_Command(_T("RC_CRS"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j);//截面位置
				Write_Int(t);//验算位置
                Write_Int(rebar_SD.Lcomk);//组合名称
				Write_Int(rebar_SD.itype);//类型
                Write_Double(rebar_SD.dM);
                Write_Double(rebar_SD.dN);
				Write_Double(rebar_SD.drebar_s);
				Write_Double(rebar_SD.drebar);
                m_iTotLineNum++;
                iCount++;
			}
		}
        if(iCount > 0){
            if(seismic) m_SeismicRebarsKList.Add(iElemK);
            else        m_ColumnRebarsKList.Add(iElemK); 
        }
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}


//混凝土柱剪应力验算
void CRCrailway::column_shear()
{    
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_shear")); 
	//==================================================================================================
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Column_shear_rc_rail_W);
    sizeRes = sizeof(CONCRETE_TP);  
    
	BOOL bPrintTitle = TRUE;
	 for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环	   
	 {
         int iElemK = m_elemlist.GetAt(i);
		 //Check Calculating Item.
         if(!m_DataCtrl.Is_CalcItem(iElemK,1))  continue ;//add by maxiao 2007.05.16
         int iCount = 0;
		 for(int j=0;j<2;j++)//单元两端截面的计算循环
		 {
			 section_Initialize();
             get_section(m_elemlist.GetAt(i),j,FALSE);
			 
			 
			 force_Initialize();
             Get_Column_force(iElemK,j,FALSE);
             double dConC = m_dconcrete_c;
             double dConB = m_dconcrete_b;
             for(int t=0;t<6;t++)
			 { 
                 m_dconcrete_c = dConC;
                 m_dconcrete_b = dConB;
				 Isaddforce(t);
				 
				 T_Column_shear_rc_rail_W ColmShearD; // Input Data
				 ColmShearD.Initialize();
				 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				 ColmShearD.nElemK   = iElemK;   			ColmShearD.nPart    = j;
				 ColmShearD.t        = t;				
				 ColmShearD.dMy      = dMy[t];  			ColmShearD.dNx      = dNx[t];
				 ColmShearD.db       = m_db; 	  			ColmShearD.dh       = m_dh;
				 ColmShearD.db_f     = m_db_f; 	  		ColmShearD.dbf      = m_dbf;
				 ColmShearD.dh_f     = m_dh_f; 	  		ColmShearD.dhf      = m_dhf;
				 ColmShearD.das      = m_das; 	  		ColmShearD.da_s     = m_da_s;
				 ColmShearD.dAs      = m_dAs; 	  		ColmShearD.dA_s     = m_dA_s;
				 ColmShearD.das_d    = m_das_d; 	  	ColmShearD.da_s_d   = m_da_s_d;
				 ColmShearD.dLo      = m_dLo; 	  		ColmShearD.dd       = m_dd;
				 ColmShearD.dAsR     = m_dAsR; 	  		ColmShearD.dR       = m_dR;
				 ColmShearD.dasR     = m_dasR; 	  		ColmShearD.dIy      = m_dIy;
				 ColmShearD.dIz      = m_dIz; 	  		ColmShearD.dCzp     = m_dCzp;
				 ColmShearD.dCzm     = m_dCzm; 	  		ColmShearD.dCyp     = m_dCyp;
				 ColmShearD.dCym     = m_dCym; 	  		ColmShearD.dconcrete_c    = m_dconcrete_c;
				 ColmShearD.dconcrete_b   = m_dconcrete_b; 	  			ColmShearD.dconcrete_shear   = m_dconcrete_shear;
				 ColmShearD.dconcrete_tp  = m_dconcrete_tp; 	  		ColmShearD.drebar            = m_drebar;
				 ColmShearD.dconcrete_u   = m_dconcrete_u; 	  	  	ColmShearD.dconcrete_wf      = m_dconcrete_wf;
				 ColmShearD.dK1     = m_dK1; 	  		ColmShearD.da_K2     = m_da_K2;
				 ColmShearD.dK      = m_dK; 	  		ColmShearD.dK3       = m_dK3;
				 ColmShearD.dEc     = m_dEc; 	  		ColmShearD.dEs       = m_dEs;
				 ColmShearD.dm      = m_dm; 	  		ColmShearD.dn        = m_dn;
				 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				 CONCRETE_TP Conc_TPD;  // Output Data
				 Conc_TPD.Initialize();
				 Conc_TPD.dV = dVz[t];   
				 if (t==0) {	 Conc_TPD.Lcomk=iNx[t];       Conc_TPD.itype=1;	}
				 if (t==1) {	 Conc_TPD.Lcomk=iNx[t];       Conc_TPD.itype=2;	}
				 if (t==2) {	 Conc_TPD.Lcomk=iMy[t];       Conc_TPD.itype=9;	}
				 if (t==3) {	 Conc_TPD.Lcomk=iMy[t];       Conc_TPD.itype=10;}				
				 if (t==4) {	 Conc_TPD.Lcomk=iVz[t];       Conc_TPD.itype=5;}				
				 if (t==5) {	 Conc_TPD.Lcomk=iVz[t];       Conc_TPD.itype=6;}		
				 Conc_TPD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 1);
				 if (!Conc_TPD.bCHK) continue;

				 //
				 T_DETAIL_STR DetailStr;
				 DetailStr.Initialize();
				 //!/
				 BOOL bVersionOK=FALSE;
				 //==================================================================================================
				 BOOL bCheck = (*pDgnCalc)(&ColmShearD, &Conc_TPD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				 if(!bVersionOK) continue;
				 //==================================================================================================
				 //report-data 
				 BOOL bPrinti = FALSE;
				 bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,2);//true:print to report-data;				 
				 if (bPrinti && bPrintTitle) 
				 {
					 file.WriteString(_T("**BEGIN RC柱－主拉应力验算: [TB 10002.3--2005]\n"));
					 bPrintTitle = FALSE;
				 }
				 if (bPrinti) 
				 {
					 for (int p=0; p<DetailStr.arString.GetSize(); p++)
					 {
						 file.WriteString(DetailStr.CW2A(p));
					 }
				 }				 
				 //change unit
				 Conc_TPD.dV              =m_DataCtrl.Cng_ForceFromCodeToCurr(Conc_TPD.dV    );
				 Conc_TPD.dshear          =m_DataCtrl.Cng_StressFromCodeToCurr(Conc_TPD.dshear    );
				 Conc_TPD.dconcrete_shear =m_DataCtrl.Cng_StressFromCodeToCurr(Conc_TPD.dconcrete_shear    );
				 Write_Command(_T("RC_CTP"));//验算名称
				 Write_Int(iElemK);//单元号
				 Write_Int(j);//截面位置
				 Write_Int(t);//验算位置
				 Write_Int(Conc_TPD.Lcomk);//组合名称
				 Write_Int(Conc_TPD.itype);//类型
				 Write_Double(Conc_TPD.dV);
				 Write_Double(Conc_TPD.dshear);
				 Write_Double(Conc_TPD.dconcrete_shear);
				 m_iTotLineNum++;
				 iCount++;
			 }
		 }
		 if(iCount > 0) m_ColumnShearKList.Add(iElemK);
	 }
	 if (!bPrintTitle) file.WriteString(_T("**END\n"));
}


void CRCrailway::column_stability(BOOL seismic)
{    
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_stability")); 
	//==================================================================================================
	
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Column_stability_W);
    sizeRes = sizeof(STABILITY);  
	
	BOOL bPrintTitle = TRUE;
	 for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环	   
	 {
         int iElemK = m_elemlist.GetAt(i);
		 //Check Calculating Item.
         if(!m_DataCtrl.Is_CalcItem(iElemK,2))  continue ;//add by maxiao 2007.05.16
         int iCount = 0;
		 for(int j=0;j<2;j++)//单元两端截面的计算循环
		 { 
			 section_Initialize();
             get_section(iElemK,j,FALSE);
			 force_Initialize();
             Get_Column_force(iElemK,j,seismic);
			 int t = 1;//only 1
			 Isaddforce(t);
			 
             T_Column_stability_W ColmStabD; // Input Data
             ColmStabD.Initialize();
             //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			 ColmStabD.nElemK   = iElemK;   			ColmStabD.nPart    = j;
			 ColmStabD.bseismic = seismic;       ColmStabD.t        = t;				
			 ColmStabD.db       = m_db; 	  			ColmStabD.dh       = m_dh;
			 ColmStabD.db_f     = m_db_f; 	  		ColmStabD.dbf      = m_dbf;
			 ColmStabD.dh_f     = m_dh_f; 	  		ColmStabD.dhf      = m_dhf;
			 ColmStabD.das      = m_das; 	  		ColmStabD.da_s     = m_da_s;
			 ColmStabD.dAs      = m_dAs; 	  		ColmStabD.dA_s     = m_dA_s;
			 ColmStabD.das_d    = m_das_d; 	  	ColmStabD.da_s_d   = m_da_s_d;
			 ColmStabD.dLo      = m_dLo; 	  		ColmStabD.dd       = m_dd;
			 ColmStabD.dAsR     = m_dAsR; 	  		ColmStabD.dR       = m_dR;
			 ColmStabD.dasR     = m_dasR; 	  		ColmStabD.dIy      = m_dIy;
			 ColmStabD.dIz      = m_dIz; 	  		ColmStabD.dCzp     = m_dCzp;
			 ColmStabD.dCzm     = m_dCzm; 	  		ColmStabD.dCyp     = m_dCyp;
			 ColmStabD.dCym     = m_dCym; 	  		ColmStabD.dconcrete_c    = m_dconcrete_c;
			 ColmStabD.dconcrete_b   = m_dconcrete_b; 	  			ColmStabD.dconcrete_shear   = m_dconcrete_shear;
			 ColmStabD.dconcrete_tp  = m_dconcrete_tp; 	  		ColmStabD.drebar            = m_drebar;
			 ColmStabD.dconcrete_u   = m_dconcrete_u; 	  	  	ColmStabD.dconcrete_wf      = m_dconcrete_wf;
			 ColmStabD.dK1     = m_dK1; 	  		ColmStabD.da_K2     = m_da_K2;
			 ColmStabD.dK      = m_dK; 	  		ColmStabD.dK3       = m_dK3;
			 ColmStabD.dEc     = m_dEc; 	  		ColmStabD.dEs       = m_dEs;
			 ColmStabD.dm      = m_dm; 	  		ColmStabD.dn        = m_dn;       
			 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
             STABILITY StabD;  // Output Data
             StabD.Initialize();
			 StabD.Lcomk=iNx[t];//only 1
             StabD.itype=2;     //only 1
			 StabD.dN=dNx[t];   //only 1
			 StabD.bCHK = m_DataCtrl.Get_ChkFlag(iElemK,j, 2);
			 if (!StabD.bCHK) continue;
			 //
			 T_DETAIL_STR DetailStr;
			 DetailStr.Initialize();
             //!/
             BOOL bVersionOK=FALSE;
             //==================================================================================================
			 BOOL bCheck = (*pDgnCalc)(&ColmStabD, &StabD, &DetailStr, sizeIn, sizeRes, bVersionOK);
             if(!bVersionOK) continue;
             //==================================================================================================
			 //report-data 
			 BOOL bPrinti = FALSE;
             bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,5);//true:print to report-data;	
			 if (bPrinti && bPrintTitle)
			 {
				 if(seismic==TRUE) file.WriteString(_T("**BEGIN 抗震柱－稳定性验算: [GB 50111--2006]\n"));
				 else              file.WriteString(_T("**BEGIN RC柱－稳定性验算: [TB 10002.3--2005]\n"));
				 bPrintTitle = FALSE;
			 }
			 if (bPrinti) 
			 {
				 for (int p=0; p<DetailStr.arString.GetSize(); p++)
				 {
					 file.WriteString(DetailStr.CW2A(p));
				 }
			 }
			 
			 //change unit
             StabD.dN               =m_DataCtrl.Cng_ForceFromCodeToCurr(StabD.dN    );
             StabD.dconcrete_s      =m_DataCtrl.Cng_StressFromCodeToCurr(StabD.dconcrete_s    );
             StabD.dconcrete_c      =m_DataCtrl.Cng_StressFromCodeToCurr(StabD.dconcrete_c    );
             if (seismic)   Write_Command(_T("S_CS"));//验算名称
			 else           Write_Command(_T("RC_CS"));//验算名称
			 Write_Int(iElemK);//单元号
			 Write_Int(j);//截面位置
             Write_Int(StabD.Lcomk);//组合名称
			 Write_Int(StabD.itype);//类型
             Write_Double(StabD.dN);
			 Write_Double(StabD.dconcrete_s);
			 Write_Double(StabD.dconcrete_c);
             m_iTotLineNum++;
             iCount++;
		 }
         if(iCount > 0){
             if (seismic) m_SeismicStabKList.Add(iElemK);
             else         m_ColumnStabKList.Add(iElemK);
         }
	 }
	 if (!bPrintTitle) file.WriteString(_T("**END\n"));
}


//柱截面的混凝土裂缝宽度
void CRCrailway::column_crack()
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_crack")); 
	//==================================================================================================
	
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Column_crack_W);
    sizeRes = sizeof(CRACK);  
	
	BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)//单元的计算循环	   
	{
        int iElemK = m_elemlist.GetAt(i);
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,2))  continue ;//add by maxiao 2007.05.16
        int iCount = 0;
		for(int j=0;j<2;j++)//单元两端截面的计算循环		
		{ 
			section_Initialize();             
			get_section(iElemK,j,FALSE);    
			force_Initialize();
            Get_Column_force(iElemK,j,FALSE);
            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b;
            for(int t=0;t<4;t++)
			{         
                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB;
				Isaddforce(t);
				
				T_Column_crack_W ColmCrackD; // Input Data
				ColmCrackD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ColmCrackD.nElemK   = iElemK;   			ColmCrackD.nPart    = j;
				ColmCrackD.t        = t;		
				ColmCrackD.dMy      = dMy[t]; 	  		ColmCrackD.dNx      = dNx[t];
				ColmCrackD.db       = m_db; 	  			ColmCrackD.dh       = m_dh;
				ColmCrackD.db_f     = m_db_f; 	  		ColmCrackD.dbf      = m_dbf;
				ColmCrackD.dh_f     = m_dh_f; 	  		ColmCrackD.dhf      = m_dhf;
				ColmCrackD.das      = m_das; 	  		  ColmCrackD.da_s     = m_da_s;
				ColmCrackD.dAs      = m_dAs; 	  		  ColmCrackD.dA_s     = m_dA_s;
				ColmCrackD.das_d    = m_das_d; 	  	  ColmCrackD.da_s_d   = m_da_s_d;
				ColmCrackD.dLo      = m_dLo; 	  		  ColmCrackD.dd       = m_dd;
				ColmCrackD.dAsR     = m_dAsR; 	  		ColmCrackD.dR       = m_dR;
				ColmCrackD.dasR     = m_dasR; 	  		ColmCrackD.dIy      = m_dIy;
				ColmCrackD.dIz      = m_dIz; 	  		  ColmCrackD.dCzp     = m_dCzp;
				ColmCrackD.dCzm     = m_dCzm; 	  		ColmCrackD.dCyp     = m_dCyp;
				ColmCrackD.dCym     = m_dCym; 	  		ColmCrackD.dconcrete_c    = m_dconcrete_c;
				ColmCrackD.dconcrete_b   = m_dconcrete_b; 	  			ColmCrackD.dconcrete_shear   = m_dconcrete_shear;
				ColmCrackD.dconcrete_tp  = m_dconcrete_tp; 	  		  ColmCrackD.drebar            = m_drebar;
				ColmCrackD.dconcrete_u   = m_dconcrete_u; 	  	  	
				ColmCrackD.dconcrete_wf      = (isaddforce[t] ? 1.2:1.0) *m_dconcrete_wf;
				ColmCrackD.dK1     = m_dK1; 	  		ColmCrackD.da_K2     = m_da_K2;
				ColmCrackD.dK      = m_dK; 	  		  ColmCrackD.dK3       = m_dK3;
				ColmCrackD.dEc     = m_dEc; 	  		ColmCrackD.dEs       = m_dEs;
				ColmCrackD.dm      = m_dm; 	  		  ColmCrackD.dn        = m_dn;
				
				CRACK CrackD;  // Output Data
				CrackD.Initialize();
				CrackD.iKind=2;
				if (t==0) {	 CrackD.Lcomk=iNx[t];       CrackD.itype=1;	}
				if (t==1) {	 CrackD.Lcomk=iNx[t];       CrackD.itype=2;	}
				if (t==2) {	 CrackD.Lcomk=iMy[t];       CrackD.itype=9;	}
				if (t==3) {	 CrackD.Lcomk=iMy[t];       CrackD.itype=10;}	
				CrackD.bCHK =  m_DataCtrl.Get_ChkFlag(iElemK,j, 2);
				if (!CrackD.bCHK) continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&ColmCrackD, &CrackD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				//report-data 
		    BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,4);//true:print to report-data;	

				if (bPrinti && bPrintTitle)
				{
				  file.WriteString(_T("**BEGIN RC柱－裂缝宽度验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}				
                //change unit
				CrackD.drebar_s      =m_DataCtrl.Cng_StressFromCodeToCurr(CrackD.drebar_s    );
				CrackD.dwf           =m_DataCtrl.Cng_LengthFromCodeToCurr(CrackD.dwf,1    );
				CrackD.dconcrete_wf  =m_DataCtrl.Cng_LengthFromCodeToCurr(CrackD.dconcrete_wf,1 );
				Write_Command(_T("RC_CK"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j);//截面位置
				Write_Int(t);//验算位置
				Write_Int(CrackD.Lcomk);
				Write_Int(CrackD.itype);//类型
				Write_Int(CrackD.iKind);
				Write_Double(CrackD.drebar_s);
				Write_Double(CrackD.dwf);
				Write_Double(CrackD.dconcrete_wf);	
				m_iTotLineNum++;
				iCount++;
			}
		}  
        if(iCount > 0) m_ColumnCrackKList.Add(iElemK);
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

void CRCrailway::column_e(BOOL seismic)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_e")); 
	//==================================================================================================
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Column_e_W);
    sizeRes = sizeof(ECCENTRICITY);  
	
	BOOL bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)
	{
        int iElemK = m_elemlist.GetAt(i);
     	//Check Calculating Item.
        if(!m_DataCtrl.Is_CalcItem(iElemK,0))  continue ;//add by maxiao 
        int iCount = 0;
		for(int j=0;j<2;j++)		 
		{   		
            //report-data 
            BOOL bPrinti = m_DataCtrl.Get_PrintOption(iElemK,j,4);//true:print to report-data;
			section_Initialize();            
			get_section(iElemK,j,FALSE);            
			
			force_Initialize();            
			Get_Seismic_Force(iElemK,j);
            double dConC = m_dconcrete_c;
            double dConB = m_dconcrete_b;			
			
			for(int t=0;t<6;t++)			
			{   
				T_Column_e_W Colm_eD; // Input Data
				Colm_eD.Initialize();

                m_dconcrete_c = dConC;
                m_dconcrete_b = dConB;
                if(t>=4)      Isaddforce(t+2);
                else Isaddforce(t);
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				Colm_eD.nElemK   = iElemK;   			Colm_eD.nPart    = j;
				Colm_eD.t        = t;		   			  Colm_eD.bseismic = seismic;
				Colm_eD.dMy      = dMy[t]; 	  		Colm_eD.dNx      = dNx[t];
				Colm_eD.dMz      = dMz[t]; 
				Colm_eD.db       = m_db; 	  			Colm_eD.dh       = m_dh;
				Colm_eD.db_f     = m_db_f; 	  		Colm_eD.dbf      = m_dbf;
				Colm_eD.dh_f     = m_dh_f; 	  		Colm_eD.dhf      = m_dhf;
				Colm_eD.das      = m_das; 	  		Colm_eD.da_s     = m_da_s;
				Colm_eD.dAs      = m_dAs; 	  		Colm_eD.dA_s     = m_dA_s;
				Colm_eD.das_d    = m_das_d; 	  	Colm_eD.da_s_d   = m_da_s_d;
				Colm_eD.dLo      = m_dLo; 	  		Colm_eD.dd       = m_dd;
				Colm_eD.dAsR     = m_dAsR; 	  		Colm_eD.dR       = m_dR;
				Colm_eD.dasR     = m_dasR; 	  		Colm_eD.dIy      = m_dIy;
				Colm_eD.dIz      = m_dIz; 	  		Colm_eD.dCzp     = m_dCzp;
				Colm_eD.dCzm     = m_dCzm; 	  		Colm_eD.dCyp     = m_dCyp;
				Colm_eD.dCym     = m_dCym; 	  		Colm_eD.dconcrete_c    = m_dconcrete_c;
				Colm_eD.dconcrete_b   = m_dconcrete_b; 	  			Colm_eD.dconcrete_shear   = m_dconcrete_shear;
				Colm_eD.dconcrete_tp  = m_dconcrete_tp; 	  		Colm_eD.drebar            = m_drebar;
				Colm_eD.dconcrete_u   = m_dconcrete_u; 	  	  	Colm_eD.dconcrete_wf      = m_dconcrete_wf;
				Colm_eD.dK1     = m_dK1; 	  		Colm_eD.da_K2     = m_da_K2;
				Colm_eD.dK      = m_dK; 	  		Colm_eD.dK3       = m_dK3;
				Colm_eD.dEc     = m_dEc; 	  		Colm_eD.dEs       = m_dEs;
				Colm_eD.dm      = m_dm; 	  		Colm_eD.dn        = m_dn;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ECCENTRICITY EccenD;  // Output Data
				EccenD.Initialize();
				if (t==0) {	 EccenD.Lcomk=iNx[t];       EccenD.itype=1;	}
				if (t==1) {	 EccenD.Lcomk=iNx[t];       EccenD.itype=2;	}
				if (t==2) {	 EccenD.Lcomk=iMy[t];       EccenD.itype=9;	}
				if (t==3) {	 EccenD.Lcomk=iMy[t];       EccenD.itype=10;}		
				if (t==4) {	 EccenD.Lcomk=iMz[t];       EccenD.itype=11;}
				if (t==5) {	 EccenD.Lcomk=iMz[t];       EccenD.itype=12;}		
				EccenD.bCHK =  m_DataCtrl.Get_ChkFlag(iElemK,j, 0);
				if (!EccenD.bCHK) continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&Colm_eD, &EccenD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================
				if (bPrinti && bPrintTitle)
				{
					file.WriteString(_T("**BEGIN 抗震柱－偏心距验算: [GB 50111--2006]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}				
				//change unit
                EccenD.de      =m_DataCtrl.Cng_LengthFromCodeToCurr(EccenD.de,1      );
                EccenD.de_alw  =m_DataCtrl.Cng_LengthFromCodeToCurr(EccenD.de_alw,1  );
				Write_Command(_T("S_E"));//验算名称
				Write_Int(iElemK);//单元号
				Write_Int(j);//截面位置
				Write_Int(t);//截面位置
                Write_Int(EccenD.Lcomk);//组合名称
				Write_Int(EccenD.itype);//类型
				Write_Double(EccenD.de);
				Write_Double(EccenD.de_alw);
                m_iTotLineNum++;
                iCount++;
            }
        }
        if(iCount > 0) m_SeismicEKList.Add(iElemK);
    } 
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
} 

//column seismic ductility check
void CRCrailway::seismic_ductility()
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Seismic_ductility")); 
	//==================================================================================================
    size_t sizeIn, sizeRes;
    sizeIn  = sizeof(T_Seismic_ductility_W);
    sizeRes = sizeof(DUCTILITY);  
	
	BOOL bPrintTitle = TRUE;
    DUCTILITY duct;	duct.Initialize();
    CArray <UINT,UINT> elemk;
    CThisInfo* pThisInfo = m_pDoc->m_pPostCtrl->GetThisInfo();
    if (pThisInfo == NULL) { ASSERT(0); return; } 
    CArray<T_4TABLE_ELEM_D, T_4TABLE_ELEM_D&> aData;
    m_pDoc->m_pPostCtrl->GetThisInfo()->GetInelastHngeElemListDist(elemk);// GetInelastHngeElemListLump
    int iCount = 0;

    if (pThisInfo->GetInelastHngeResultDist(0,elemk, aData))//分布    GetInelastHngeResultLump
	{
		for (int ie =0; ie < elemk.GetSize(); ie++)//elem
		{
			T_Seismic_ductility_W SeisDuctD[6]; // Input Data
			DUCTILITY DuctiD[6];  // Output
			for (int i =0; i<6; i++)
			{
				SeisDuctD[i].Initialize();
				DuctiD[i].Initialize();
			}
			double dValue = 0.0; 
			for (int k=0; k < aData.GetSize(); k++ )//data
			{
				T_4TABLE_ELEM_D data = aData[k];
				if (data.ElemK != elemk.GetAt(ie)) continue;
				
				int nDof = data.nDof;
			//	dValue = data.Data[nDof].dYeildDuctility_Neg;
				if (fabs(data.Data[nDof].dYeildDuctility_Pos) >= fabs(data.Data[nDof].dYeildDuctility_Neg))
				{
					dValue = data.Data[nDof].dYeildDuctility_Pos;
				}
				else
				{
					dValue = data.Data[nDof].dYeildDuctility_Neg;
				}
				if (dValue > 1.0E23) dValue = .0;

				int ipos = 0;
				if (data.csHingeLocation ==_T("2-Pos")) ipos = 1;
				else if (data.csHingeLocation ==_T("3-Pos")) ipos = 2;
				if (fabs(dValue) > DuctiD[nDof].du)
				{
					SeisDuctD[nDof].nElemK   = data.ElemK;
					SeisDuctD[nDof].nPart    = ipos;
					
					DuctiD[nDof].du = dValue;
					DuctiD[nDof].itype = nDof;
				}
			}
			for(int i=0; i<6; i++)
			{
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
                //!/
                BOOL bVersionOK=FALSE;
                //==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&SeisDuctD[i], &DuctiD[i], &DetailStr, sizeIn, sizeRes, bVersionOK);
                if(!bVersionOK) continue;
                //==================================================================================================				
				BOOL bPrinti = FALSE;
				if (bPrintTitle && bPrinti) 
				{
					file.WriteString(_T("**BEGIN 抗震柱－延性比验算: [GB 50111--2006]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti) 
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}				
				Write_Command(_T("S_DUCT"));//验算名称
				Write_Int(elemk.GetAt(ie));//单元号
				Write_Int(SeisDuctD[i].nPart);//截面位置
				Write_Int(0);//荷载
				Write_Int(DuctiD[i].itype);//类型
				Write_Double(DuctiD[i].du);
				Write_Double(DuctiD[i].du_alw);
				m_iTotLineNum++;
				iCount++;
			}

// 		for (int k = 0; k < aData.GetSize(); k++)
// 		{
// 			T_4TABLE_ELEM_D data = aData[k];
// 			for (int i = 0; i < 6; i++)
// 			{
// 				double dValue = 0.0;          
// 				if (fabs(data.Data[i].dYeildDuctility_Pos) >= fabs(data.Data[i].dYeildDuctility_Neg))
// 				{
// 					dValue = data.Data[i].dYeildDuctility_Pos;
// 				}
// 				else
// 				{
// 					dValue = data.Data[i].dYeildDuctility_Neg;
// 				}
// 				int ipos = 0;
// 				if (data.csHingeLocation ==_T("2-Pos")) ipos = 1;
// 				else if (data.csHingeLocation ==_T("3-Pos")) ipos = 2;
// 				
//         T_Seismic_ductility SeisDuctD; // Input Data
//         SeisDuctD.Initialize();
//         //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//         SeisDuctD.nElemK   = data.ElemK;
//         SeisDuctD.nPart    = ipos;
//         //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//         DUCTILITY DuctiD;  // Output Data
//         DuctiD.Initialize();
// 				DuctiD.du = dValue;
// 				DuctiD.itype = i;
// 				//
// 				T_DETAIL_STR DetailStr;
// 				DetailStr.Initialize();
//         //!/
//         BOOL bVersionOK=FALSE;
//         //==================================================================================================
// 				BOOL bCheck = (*pDgnCalc)(&SeisDuctD, &DuctiD, &DetailStr, sizeIn, sizeRes, bVersionOK);
//         if(!bVersionOK) continue;
//         //==================================================================================================				
// 				BOOL bPrinti = FALSE;
// 				if (bPrintTitle && bPrinti) 
// 				{
// 					file.WriteString(_T("**BEGIN 抗震柱－延性比验算: [GB 50111--2006]\n"));
// 					bPrintTitle = FALSE;
// 				}
// 				if (bPrinti) 
// 				{
// 					for (int p=0; p<DetailStr.arString.GetSize(); p++)
// 					{
// 						file.WriteString(DetailStr.arString.GetAt(p));
// 					}
// 				}				
// 				Write_Command(_T("S_DUCT"));//验算名称
// 				Write_Int(data.ElemK);//单元号
// 				Write_Int(ipos);//截面位置
// 				Write_Int(0);//荷载
// 				Write_Int(DuctiD.itype);//类型
// 				Write_Double(DuctiD.du);
// 				Write_Double(DuctiD.du_alw);
// 				m_iTotLineNum++;
// 				iCount++;
// 			}
			if(iCount > 0 ) m_SeismicDuctKList.Add(elemk.GetAt(ie));
		}
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
} 
