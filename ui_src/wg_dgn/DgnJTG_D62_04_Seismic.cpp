// DgnJTG_D62_04_Seismic.cpp: implementation of the CDgnJTG_D62_04_Seismic class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnJTG_D62_04_Seismic.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\..\dgnengine\src\CVL_DgnCalc_CH\DgnCalc_CH_RC_Struct.h"
#include "CH_DGN_INTERFACE_UNICODE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDgnJTG_D62_04_Seismic::CDgnJTG_D62_04_Seismic()
{
    m_PierList.RemoveAll();     
    m_AbutmentList.RemoveAll(); 
    m_FoundationList.RemoveAll();
    m_BentCapList.RemoveAll();    
    m_RubberLinkList.RemoveAll();
    m_ActiveLinkList.RemoveAll();
    m_FixedLinkList.RemoveAll();
    m_MainArchList.RemoveAll();

    m_iTotLineNum=0;
	m_dZero = 1.0E-07;
 	m_pDoc = CDBDoc::GetDocPoint();
 	ASSERT(m_pDoc);
	
    m_E1Pier_Comp_KList.RemoveAll();      
    m_E1Arch_Cross_KList.RemoveAll();     
    m_E1Arch_Comp_KList.RemoveAll();      
    m_E1BentCap_Bend_KList.RemoveAll();   
    m_E1BentCap_Shear_KList.RemoveAll();  
    m_E1Foundation_KList.RemoveAll();     
    m_E1RubberThick_KList.RemoveAll();    
    m_E1RubberSlip_KList.RemoveAll();     
    m_E1ActiveHoriDisp_KList.RemoveAll(); 
    m_E1StatHoriForce_KList.RemoveAll();  
    m_E2Pier_Comp_KList.RemoveAll();      
    m_E2Arch_Cross_KList.RemoveAll();     
    m_E2Arch_Comp_KList.RemoveAll();      
    m_E2BentCap_Bend_KList.RemoveAll();   
    m_E2BentCap_Shear_KList.RemoveAll();  
    m_E2Foundation_KList.RemoveAll();     
    m_E2Pier_Shear_KList.RemoveAll();     
    m_E2RubberThick_KList.RemoveAll();    
    m_E2RubberSlip_KList.RemoveAll();     
    m_E2ActiveHoriDisp_KList.RemoveAll(); 
    m_E2StatHoriForce_KList.RemoveAll();  
    m_E2FrustaTopDisp_KList.RemoveAll();  
    m_E2CornerDisp_KList.RemoveAll();     
    
    m_Rc_D_CH.Initialize();
}

CDgnJTG_D62_04_Seismic::~CDgnJTG_D62_04_Seismic()
{
}

BOOL CDgnJTG_D62_04_Seismic::Design()
{
	if (m_pDoc->IsLimitedTrialVersion())
    {
		m_pDoc->ShowLimitedTrialVersionErrMsg();
        return FALSE;
    }
    int iStageNum = m_pDoc->m_pAttrCtrl->GetCountStag();
    T_STAG_K CurStag = m_pDoc->m_pStagCtrl->GetCurStag();
    int iCurStep = m_pDoc->m_pStagCtrl->GetCurStagStepIndex()+1;
    if (iStageNum>0)
        m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();
    if(!MakeData()) //
    {
        if(iStageNum>0) 
        {
            m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
        }
        return FALSE;
    }
	
    CFileException FileException;
    CString strPathName=m_pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
	CString strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_seismic.jpo");
    CString outFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_seismic.mrpt");  
	Set_FileName(strFilePathName);
	
    TRY
    {
        if(!m_cfile.Open(strFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
        if(!m_outfile.Open(outFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
        if(!Write_ImportData()) //
        {
            m_cfile.Close();
            m_outfile.Close();
            CFileStatus status;
            if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
            if(iStageNum>0)
                m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
            return FALSE;
        }
        m_cfile.Close();
        m_outfile.Close();
    }
    CATCH(CFileException, e)
    {
        e->ReportError();
        m_cfile.Abort();
        m_outfile.Abort();
        CFileStatus status;
        return FALSE;
    }
    END_CATCH 
	{
		return TRUE; 
	}
}


CString CDgnJTG_D62_04_Seismic::Get_DllFileName()
{
	BOOL bDebug = FALSE;
    CString strDllName = _T("");
#if defined(_DEBUG)
	bDebug = TRUE;
#endif
	strDllName = bDebug ? _T("CVL_DgnCalc_CHD.dgne") : _T("CVL_DgnCalc_CH.dgne");      
    return strDllName;
}

BOOL CDgnJTG_D62_04_Seismic::AfterCheck(BOOL bVersionOK, CString str, BOOL &bPrintTitle, BOOL bPrinti, T_DETAIL_STR DetailStr)
{
	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}	
	if (bPrintTitle && bPrinti) 
	{
		m_outfile.WriteString(str);
		bPrintTitle = FALSE;
	}
	if (bPrinti) 
	{
		for (int p=0; p<DetailStr.arString.GetSize(); p++)
		{
			m_outfile.WriteString(DetailStr.CW2A(p));
		}
	}
	return TRUE;
}

BOOL CDgnJTG_D62_04_Seismic::Write_ImportData()
{
    T_JTG_SEISMIC_D dgnD; dgnD.Initialize();
    if(!m_DataCtrl.Get_SeismicD(dgnD)) return FALSE;
    CString strCalcDll = Get_DllFileName();  
    if((m_hDllCalc = LoadLibrary(strCalcDll)) == NULL)
    {
        AfxMessageBox(_T("Can't Find CVL_DgnCalc_CH.dll"));
    }  
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
    m_DataCtrl.Set_UnitIndex(CodeUnit);	
//  m_DataCtrl.m_DataCtrl.Set_UnitIndex(CodeUnit);
    if (dgnD.iLoadType==0 && dgnD.bDesignTpye[0] )	SeismicPierComp(TRUE);    //桥墩桥台单元强度
    if (dgnD.iLoadType==0 && dgnD.bDesignTpye[1] )	SeismicAbutmentComp(TRUE);//桥台单元强度
	if (dgnD.iLoadType==0 && dgnD.bDesignTpye[2] )	SeismicFoundation(TRUE);  //基础强度验算
	if (dgnD.iLoadType==0 && dgnD.bDesignTpye[3] )
	{	
		SeismicBentCapBend(TRUE,TRUE);            //盖梁强度-抗弯 E1
		SeismicBentCapShear(TRUE,TRUE);           //盖梁强度-抗剪 E1
	}
	if (dgnD.iLoadType==0 && dgnD.iCheckType==0 && dgnD.bDesignTpye[4] )
	{	
		SeismicArchCross(TRUE);                //E1主拱圈强度-抗弯-改成柱截面
		SeismicArchComp(TRUE);                 //E1主拱圈强度-偏压-改成柱截面
	}
	if (dgnD.iLoadType==0 && dgnD.bDesignTpye[6] )	SeismicRubberThick(TRUE);   //支座厚度（板式橡胶）
	if (dgnD.iLoadType==0 && dgnD.bDesignTpye[7] )	SeismicRubberSlip(TRUE);    //支座抗滑稳定性（板式橡胶）
	if (dgnD.iLoadType==0 && dgnD.bDesignTpye[8] )	SeismicActiveHoriDisp(TRUE);//支座水平位移（盆式活动支座）
	if (dgnD.iLoadType==0 && dgnD.bDesignTpye[9] )	SeismicStatHoriForce(TRUE); //支座水平承载力（盆式固定支座）
	
	//E2
	// 0-桥墩强度; 1-桥台强度; 2-基础强度; 3-盖梁强度; 4-主拱圈强度; 5-桥墩塑性铰区抗剪强度;
    // 6-支座厚度（板式橡胶支座）; 7-支座抗滑稳定性（板式橡胶支座）; 8-支座水平位移（活动盆式支座）; 9-支座水平承载力（固定盆式支座）; 10-墩顶位移(规则桥梁); 11-塑性转角位移(非规则桥梁); 
    if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[0]) 	SeismicPierComp(FALSE);    //E2桥墩、桥台单元强度
    if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[1]) 	SeismicAbutmentComp(FALSE);//E2桥台单元强度
	if (dgnD.iLoadType==1 && dgnD.bDesignTpye[2]) 	SeismicFoundation(FALSE);  //基础强度验算
    if (dgnD.iLoadType==1 && dgnD.bDesignTpye[3])
    {
        SeismicBentCapBend(FALSE, TRUE);              //盖梁强度-抗弯 E2弹性
        SeismicBentCapShear(FALSE, TRUE);             //盖梁强度-抗剪 E2弹性
    }
    if (dgnD.iLoadType==2 && dgnD.bDesignTpye[3])
	{	
		SeismicBentCapBend(FALSE, FALSE);              //盖梁强度-抗弯 E2弹塑性   
		SeismicBentCapShear(FALSE, FALSE);             //盖梁强度-抗剪 E2弹塑性   
	}
	if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[4] )
	{	
		SeismicArchCross(FALSE);                //E2主拱圈强度-抗弯
		SeismicArchComp(FALSE);                 //E2主拱圈强度-偏压
	}
	if (dgnD.iLoadType==2 && dgnD.bDesignTpye[5] )  SeismicE2PierShear();        //桥墩塑性铰区抗剪强度//E2作用(弹塑性阶段)
	if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[6] )	SeismicRubberThick(FALSE);   //支座厚度（板式橡胶）
	if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[7] )	SeismicRubberSlip(FALSE);    //支座抗滑稳定性（板式橡胶）
	if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[8] )	SeismicActiveHoriDisp(FALSE);//支座水平位移（盆式活动支座）
	if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[9] )	SeismicStatHoriForce(FALSE); //支座水平承载力（盆式固定支座）
	if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[10])	SeismicE2FrustaTopDisp();    //墩顶位移（规则桥梁）SFTD_JT
	if (dgnD.iLoadType!=0 && dgnD.bDesignTpye[11])	SeismicE2CornerDisp();       //塑形转角位移（非规则桥梁）SCND_JT	
	return TRUE;
}

BOOL CDgnJTG_D62_04_Seismic::MakeData()
{
    m_DataCtrl.Initialize();
	
	BOOL bCheck = FALSE;
    T_JTG_SEISMIC_D dgnD; dgnD.Initialize();
    if(!m_DataCtrl.Get_SeismicD(dgnD)) return FALSE;

    if(!m_DataCtrl.Get_Rcd(m_Rc_D_CH)) return FALSE;
	for (int i=0; i<12; i++)
	{
		if (dgnD.bDesignTpye[i]) bCheck = TRUE;
	}
	if (!bCheck) return FALSE;	
    m_PierList.RemoveAll();     
    m_AbutmentList.RemoveAll(); 
    m_FoundationList.RemoveAll();
    m_BentCapList.RemoveAll();    
    m_RubberLinkList.RemoveAll();
    m_ActiveLinkList.RemoveAll();
    m_FixedLinkList.RemoveAll();
    m_MainArchList.RemoveAll();  
    m_DataCtrl.Get_PierMemberListForSeismicDgn(m_PierList);
    m_DataCtrl.Get_AbutmentElemListForSeismicDgn(m_AbutmentList);
    m_DataCtrl.Get_FoundationElemListForSeismicDgn(m_FoundationList);
    m_DataCtrl.Get_CapBeamMemberListForSeismicDgn(m_BentCapList);
    m_DataCtrl.Get_RubberLinkListForSeismicDgn(m_RubberLinkList);
    m_DataCtrl.Get_ActiveLinkListForSeismicDgn(m_ActiveLinkList);
    m_DataCtrl.Get_FixedLinkListForSeismicDgn(m_FixedLinkList);
    m_DataCtrl.Get_MainArchElemListForSeismicDgn(m_MainArchList);
    
	if ((m_PierList.GetSize()+m_AbutmentList.GetSize()+m_FoundationList.GetSize()+ m_BentCapList.GetSize() 
             + m_RubberLinkList.GetSize()+m_ActiveLinkList.GetSize()+m_FixedLinkList.GetSize()+m_MainArchList.GetSize() ) <=0)
    {
        GSaveHistoryFormatNF(_T("[错误] 没有指定抗震验算构件类型."));
        return FALSE;
    }
    for (int i=0; i<m_FoundationList.GetSize();i++ ) 
	{
		_DGN_RC_MATL_CH matl;
        if (!m_DataCtrl.Get_Matl(m_FoundationList.GetAt(i),matl))
		{
            GSaveHistoryFormatNF(_T("[错误] 抗震设计单元材料选择不正确."));
            return FALSE;
        }
    }
    //+++++++++++++++++++++
    if(!m_DataCtrl.Get_LcomDataMap(m_DgnLcomKMap))	return FALSE;
	//	// Get Forces for Design.
    CArray<T_ELEM_K,T_ELEM_K> aElemK;
    if(m_DataCtrl.Get_ElemListForSeismicDgn(aElemK))
    {
        if(aElemK.GetSize()>0)
        {
            if(!m_DataCtrl.Get_ForcDataMap(FALSE,m_ComfDMap,TRUE))	return FALSE;
        }
    }
    return TRUE;
}

//桥墩单元强度
BOOL CDgnJTG_D62_04_Seismic::SeismicPierComp(BOOL bE1)
{ 
	BOOL bPrintTitle = TRUE;
	//======================Explicit Linkage for DLL====================================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_seismic_compressive")); 
	//==================================================================================================
    size_t sizeIn  = sizeof(_DGN_CSCS_D_W);
    size_t sizeResu = sizeof(T_CSTCS_CH_BASE);
    
    for (int m=0; m<m_PierList.GetSize(); m++) //all pier members
    {
        UINT PierMember = m_PierList.GetAt(m);
        CArray<UINT,UINT> PierElemKList;
        if(!m_DataCtrl.Get_PierElemListByPierID (PierMember, PierElemKList)) continue;
        
        //Get parameters data from pier
        BOOL bOnlyBot = TRUE, bDoublePier = FALSE;
        double dHn =.0, dDisp =.0;
        int nOutputType = 0;
        if(!m_DataCtrl.GetParaFromPier(PierMember,bOnlyBot,dHn,bDoublePier,dDisp,nOutputType)) continue;

        for (int i=0; i<PierElemKList.GetSize(); i++) //Check elemlist
        {
            T_ELEM_K ElemK = PierElemKList.GetAt(i); 		      
            //0.get design matl of checking ELEM
            _DGN_RC_MATL_CH matl;		matl.Initialize();
            if (!m_DataCtrl.Get_Matl(ElemK,matl)) continue;
            int iCount = 0;
            //get force
            T_CSTCS_CH cscs;	cscs.Initialize();
            if (!m_DataCtrl.Get_SeismicComp(bE1,ElemK,TRUE,cscs)) continue;
            for (int j=0;j<10;j++) 
            {
                if (!cscs.cstcsBase[j].bCHK) continue;
                //1:get section of checking ELEM
                _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
                if (!m_DataCtrl.Get_ColumnSection(ElemK,j/5,sectioni)) continue; 
                
                BOOL bPrinti = GetPrintRpt(nOutputType, j/5);
                //!/
                _DGN_CSCS_D_W DgnCscsD;       DgnCscsD.Initialize();
                DgnCscsD.nElemK = ElemK;  	DgnCscsD.nPart = j;
                DgnCscsD.RcMatlCH  =WA_Convert::Convert_A2W(matl);
                DgnCscsD.SectionCH =WA_Convert::Convert_A2W(sectioni);//
                //==================================================================================================
                T_DETAIL_STR DetailStr;  DetailStr.Initialize();
                BOOL bVersionOK=FALSE;
                BOOL bCheck = (*pDgnCalc)(&DgnCscsD, &cscs.cstcsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
                BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN桥墩单元强度验算:\n"), bPrintTitle,bPrinti,DetailStr);
                if (!bAfterCheck) continue;
                //==================================================================================================
            }//endof for(j) check compressive stress of eccentricity
            for (int j =0;j<10;j++) 
            {
                m_DataCtrl.Get_CHRCCstcsBaseCodeToCurr(cscs.cstcsBase[j]);  //change UNIT      
                if (!cscs.cstcsBase[j].bCHK) continue;
                Write_Command(_T("SPCP_JT"));//SeismicPierComp
                Write_Int(ElemK);
                Write_Int(j/5);
                Write_Int(j%5);//位置标示
                Write_Int(cscs.cstcsBase[j].LcomK);
                Write_Int(cscs.cstcsBase[j].bOK);
                Write_Double(cscs.cstcsBase[j].dx);
                Write_Double(cscs.cstcsBase[j].dNd);
                Write_Double(cscs.cstcsBase[j].drNd);
                Write_Double(cscs.cstcsBase[j].drNde);
                Write_Double(cscs.cstcsBase[j].drNde1);
                Write_Double(cscs.cstcsBase[j].dNn);
                Write_Double(cscs.cstcsBase[j].dNne);
                Write_Double(cscs.cstcsBase[j].dNne1);
                m_iTotLineNum++;
                iCount++;
            }//endof for(j) section
            if(iCount > 0)
            {
                if(bE1) m_E1Pier_Comp_KList.Add(ElemK);
                if(!bE1) m_E2Pier_Comp_KList.Add(ElemK);		
            }       
        }//endof for(i) ELEM
    }//endof for(m) Member
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
    return TRUE;
}

//桥台单元强度
BOOL CDgnJTG_D62_04_Seismic::SeismicAbutmentComp(BOOL bE1)
{ 
    BOOL bPrintTitle = TRUE;
    //======================Explicit Linkage for DLL====================================================
    typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
    PDGNCALC *pDgnCalc;
    pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_seismic_compressive")); 
    //==================================================================================================
    size_t sizeIn  = sizeof(_DGN_CSCS_D_W);
    size_t sizeResu = sizeof(T_CSTCS_CH_BASE);
    for (int i=0; i<m_AbutmentList.GetSize(); i++) //Check elemlist
    {
        T_ELEM_K ElemK = m_AbutmentList.GetAt(i); 
        
        //for design items
        T_RSDM_D seisType;  seisType.Initialize();
        if(!m_DataCtrl.GetElemSeisChkType(ElemK, seisType)) continue;
        //if (seisType.nMemberType!=1)  continue;//Abutment
        
        if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
        {
            if (seisType.nMemberType!=1 )  continue;
        }
        else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (seisType.nMemberType!=11 && seisType.nMemberType!=12)  continue;
        }
        else
        {
            continue;
        }

        //0.get design matl of checking ELEM
        _DGN_RC_MATL_CH matl;		matl.Initialize();
        if (!m_DataCtrl.Get_Matl(ElemK,matl)) continue;
        int iCount = 0;
        //get force
        T_CSTCS_CH cscs;	cscs.Initialize();
        if (!m_DataCtrl.Get_SeismicComp(bE1,ElemK,TRUE,cscs)) continue;
        for (int j=0;j<10;j++) 
        {
            if (!cscs.cstcsBase[j].bCHK) continue;
            //1:get section of checking ELEM
            _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
            if (!m_DataCtrl.Get_ColumnSection(ElemK,j/5,sectioni)) continue; 
            
            BOOL bPrinti = GetPrintRpt(seisType.nOutputType, j/5);
            //!/
            _DGN_CSCS_D_W DgnCscsD;       DgnCscsD.Initialize();
            DgnCscsD.nElemK = ElemK;  	DgnCscsD.nPart = j;
            DgnCscsD.RcMatlCH  = WA_Convert::Convert_A2W(matl);
            DgnCscsD.SectionCH = WA_Convert::Convert_A2W(sectioni);//
            //==================================================================================================
            T_DETAIL_STR DetailStr;  DetailStr.Initialize();
            BOOL bVersionOK=FALSE;
            BOOL bCheck = (*pDgnCalc)(&DgnCscsD, &cscs.cstcsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
            BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN桥台单元强度验算:\n"), bPrintTitle,bPrinti,DetailStr);
            if (!bAfterCheck) continue;
            //==================================================================================================
        }//endof for(j) check compressive stress of eccentricity
        for (int j =0;j<10;j++) 
        {
            m_DataCtrl.Get_CHRCCstcsBaseCodeToCurr(cscs.cstcsBase[j]);  //change UNIT      
            if (!cscs.cstcsBase[j].bCHK) continue;
            Write_Command(_T("SPCP_JT"));//SeismicPierComp//same to Pier
            Write_Int(ElemK);
            Write_Int(j/5);
            Write_Int(j%5);//位置标示
            Write_Int(cscs.cstcsBase[j].LcomK);
            Write_Int(cscs.cstcsBase[j].bOK);
            Write_Double(cscs.cstcsBase[j].dx);
            Write_Double(cscs.cstcsBase[j].dNd);
            Write_Double(cscs.cstcsBase[j].drNd);
            Write_Double(cscs.cstcsBase[j].drNde);
            Write_Double(cscs.cstcsBase[j].drNde1);
            Write_Double(cscs.cstcsBase[j].dNn);
            Write_Double(cscs.cstcsBase[j].dNne);
            Write_Double(cscs.cstcsBase[j].dNne1);
            m_iTotLineNum++;
            iCount++;
        }//endof for(j) section
        if(iCount > 0)
        {
            if(bE1) m_E1Pier_Comp_KList.Add(ElemK);//same to Pier
            if(!bE1) m_E2Pier_Comp_KList.Add(ElemK);		
        }       
    }//endof for(i) ELEM
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
    return TRUE;
}

//主拱圈强度-抗弯
BOOL CDgnJTG_D62_04_Seismic::SeismicArchCross(BOOL bE1)
{ 
	BOOL bPrintTitle = TRUE;
	//======================Explicit Linkage for DLL====================================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_seismic_cross")); 
	//==================================================================================================
    size_t sizeIn  = sizeof(_DGN_CBCS_D_W);
    size_t sizeResu = sizeof(T_CBCS_CH_BASE);
    for (int i=0; i<m_MainArchList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_MainArchList.GetAt(i); 
		
		//for design items
		T_RSDM_D seisType;  seisType.Initialize();
		if(!m_DataCtrl.GetElemSeisChkType(ElemK, seisType)) continue;
		//if (seisType.nMemberType!=7)  continue;//7-mainArch
        if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
        {
            if (seisType.nMemberType!=7 )  continue;
        }
        else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (seisType.nMemberType!=17 )  continue;
        }
        else
        {
            continue;
        }
		//1:get design material of checking ELEM
		_DGN_RC_MATL_CH matl;		matl.Initialize();
		if (!m_DataCtrl.Get_Matl(ElemK,matl))	continue;
        //2:get force data
        T_CBCS_CH cbcs;	cbcs.Initialize();
		if (!m_DataCtrl.Get_SeismicCbcs(bE1,ElemK,cbcs)) continue;		
        int iCount = 0;
        for (int j=0; j<4; j++) 
		{
            if (!cbcs.cbcsBase[j].bCHK) continue;
			//3:get section of checking ELEM
            _DGN_RC_COLUMN_SECTION_CH sectionC; sectionC.Initialize();
            if (!m_DataCtrl.Get_ColumnSection(ElemK,j/2,sectionC)) continue; //数据库/用户截面
//      if (!m_DataCtrl.Get_BeamSection(ElemK,j/2,sectioni))	continue;
            if(sectionC.dr > m_dZero) continue;//如果圆形，不计算
            _DGN_RC_SECTION_CH sectioni; sectioni.Initialize();
            sectioni.dA = sectionC.dA;
            sectioni.dAs = sectionC.dAs;  sectioni.dAs1 = sectionC.dAs1;
            sectioni.das = sectionC.das;  sectioni.das1 = sectionC.das1;
            sectioni.dhf1 = sectionC.dhf1;  sectioni.dhf2 = sectionC.dhf2;
            sectioni.dbf1 = sectionC.dbf1;  sectioni.dbf2 = sectionC.dbf2;
            sectioni.db = sectionC.db;  sectioni.dh = sectionC.dh;
            sectioni.dh0 = sectionC.dh0;       

            _DGN_CBCS_D_W DgnCbcsD;         DgnCbcsD.Initialize();
            DgnCbcsD.nElemK = ElemK; 			DgnCbcsD.nPart = j;
            DgnCbcsD.RcMatlCH  = WA_Convert::Convert_A2W(matl);   
            DgnCbcsD.SectionCH = WA_Convert::Convert_A2W(sectioni);
            //==================================================================================================
			T_DETAIL_STR DetailStr; DetailStr.Initialize();
            BOOL bVersionOK=FALSE;
            BOOL bCheck = (*pDgnCalc)(&DgnCbcsD, &cbcs.cbcsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
			BOOL bPrinti = GetPrintRpt(seisType.nOutputType, j/2);
			BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN主拱圈抗弯强度验算:\n"), bPrintTitle,bPrinti,DetailStr);
			if (!bAfterCheck) continue;
            //==================================================================================================
		}//endof for(j)
		for (int j=0;j<4;j++) 
		{
            m_DataCtrl.Get_CHRCCbcsBaseCodeToCurr(cbcs.cbcsBase[j]);  //change UNIT      
            if (!cbcs.cbcsBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			int iTyp = (j==0 || j==2 ? 1 : 2);
			Write_Command(_T("SACS_JT"));    //SeismicArchCross();           
			Write_Int(ElemK);
			Write_Int(iPos);
			Write_Int(iTyp);
			Write_Int(cbcs.cbcsBase[j].LcomK);
			Write_Int(cbcs.cbcsBase[j].iMax);
			Write_Int(cbcs.cbcsBase[j].bOK);
			Write_Double(cbcs.cbcsBase[j].dMuy);
			Write_Double(cbcs.cbcsBase[j].drMuy);	   
			Write_Double(cbcs.cbcsBase[j].dMny);		   
			Write_Double(cbcs.cbcsBase[j].dRatY); 
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount)
        {
            if(bE1) m_E1Arch_Cross_KList.Add(ElemK);		
            if(!bE1) m_E2Arch_Cross_KList.Add(ElemK);
        }
	}//endof for(i)	
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

//主拱圈强度-偏压
BOOL CDgnJTG_D62_04_Seismic::SeismicArchComp(BOOL bE1)
{ 
	BOOL bPrintTitle = TRUE;
	//=========================Explicit Linkage for DLL=================================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_seismic_compressive")); 
	//==================================================================================================
    size_t sizeIn  = sizeof(_DGN_CSCS_D_W);
    size_t sizeResu = sizeof(T_CSTCS_CH_BASE);
    for (int i=0; i<m_MainArchList.GetSize(); i++) //Check elemlist
	{
        T_ELEM_K ElemK = m_MainArchList.GetAt(i); 		
		//for design items
		T_RSDM_D seisType;  seisType.Initialize();
		if(!m_DataCtrl.GetElemSeisChkType(ElemK, seisType)) continue;
		//if (seisType.nMemberType!=7)  continue;
        if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
        {
            if (seisType.nMemberType!=7 )  continue;
        }
        else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (seisType.nMemberType!=17 )  continue;
        }
        else
        {
            continue;
        }
		//0.get design matl of checking ELEM
		_DGN_RC_MATL_CH matl;		matl.Initialize();
        if (!m_DataCtrl.Get_Matl(ElemK,matl)) continue;
        //get force
		T_CSTCS_CH cscs;	cscs.Initialize();
		if (!m_DataCtrl.Get_SeismicComp(bE1,ElemK,TRUE,cscs)) continue;

        int iCount = 0;    
        for (int j=0;j<10;j++) 
        {
            if (!cscs.cstcsBase[j].bCHK) continue;
            //1:get section of checking ELEM
//			_DGN_RC_SECTION_CH section; section.Initialize();
//			if (!m_DataCtrl.Get_BeamSection(ElemK,j/5,section)) continue;  //梁单元 
			_DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
            if (!m_DataCtrl.Get_ColumnSection(ElemK,j/5,sectioni)) continue; //数据库/用户截面

			BOOL bPrinti = GetPrintRpt(seisType.nOutputType, j/5);
            //!/
            _DGN_CSCS_D_W DgnCscsD;       DgnCscsD.Initialize();
			DgnCscsD.nElemK = ElemK;		DgnCscsD.nPart = j;
            DgnCscsD.RcMatlCH  = WA_Convert::Convert_A2W(matl);
            DgnCscsD.SectionCH = WA_Convert::Convert_A2W(sectioni);//
            //==================================================================================================
            T_DETAIL_STR DetailStr;   DetailStr.Initialize();
            BOOL bVersionOK=FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnCscsD, &cscs.cstcsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
			BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN主拱圈偏压强度验算:\n"), bPrintTitle,bPrinti,DetailStr);
			if (!bAfterCheck) continue;
            //==================================================================================================
        }//endof for(j) check compressive stress of eccentricity
        for (int j =0;j<10;j++) 
		{
            m_DataCtrl.Get_CHRCCstcsBaseCodeToCurr(cscs.cstcsBase[j]);  //change UNIT      
            if (!cscs.cstcsBase[j].bCHK) continue;
            Write_Command(_T("SACP_JT"));//SeismicArchComp
            Write_Int(ElemK);
            Write_Int(j/5);
            Write_Int(j%5);//位置标示
            Write_Int(cscs.cstcsBase[j].LcomK);
			Write_Int(cscs.cstcsBase[j].bOK);
            Write_Double(cscs.cstcsBase[j].dx);
            Write_Double(cscs.cstcsBase[j].dNd);
            Write_Double(cscs.cstcsBase[j].drNd);
            Write_Double(cscs.cstcsBase[j].drNde);
            Write_Double(cscs.cstcsBase[j].drNde1);
            Write_Double(cscs.cstcsBase[j].dNn);
            Write_Double(cscs.cstcsBase[j].dNne);
            Write_Double(cscs.cstcsBase[j].dNne1);
            m_iTotLineNum++;
            iCount++;
        }//endof for(j) section		
        if(iCount > 0 && bE1) m_E1Arch_Comp_KList.Add(ElemK);
        if(iCount > 0 && !bE1) m_E2Arch_Comp_KList.Add(ElemK);    
    }//endof for(i) ELEM
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//盖梁强度-抗弯（E2-弹塑性）
BOOL CDgnJTG_D62_04_Seismic::SeismicBentCapBend(BOOL bE1, BOOL bElastic)
{
    BOOL bPrintTitle = TRUE;
    //====================Explicit Linkage for DLL==================================================
    typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
    PDGNCALC *pDgnCalc;
    pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_BentCap_Bend")); 
    //==============================================================================================
    size_t sizeIn  = sizeof(_DGN_BENTCAP_D_W);
    size_t sizeResu = sizeof(T_CBCS_CH_BASE);
    
    for (int m=0; m<m_BentCapList.GetSize(); m++) //all pier members
    {
        UINT BentCapMember = m_BentCapList.GetAt(m);
        CArray<UINT,UINT> BentCapElemKList;
        if(!m_DataCtrl.Get_CapBeamElemListByCapBeamId(BentCapMember, BentCapElemKList)) continue;    

        //Get parameters data from cap Beam; 
        int nOutputType = 0;
        double dL0 = 0;//seisType.dCappingBeamSpan
        if(!m_DataCtrl.GetParaFromCapBeam(BentCapMember,dL0, nOutputType)) continue;

        //盖梁相连的墩构件列表
        CArray<UINT,UINT> BentCapPierList;
        double dMs = .0;
        if(!bE1 && !bElastic)
        {
            if(!m_DataCtrl.Get_PierListofCapBeam(BentCapMember, BentCapPierList)) continue;
            if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
                dMs= GetBentCapMs(BentCapPierList);
        }
        
        for(int i=0; i<BentCapElemKList.GetSize(); i++)
        {
            T_ELEM_K ElemK = BentCapElemKList.GetAt(i);       
            //0.get design material of checking ELEM
            _DGN_RC_MATL_CH matl;		matl.Initialize();
            if (!m_DataCtrl.Get_Matl(ElemK,matl)) continue;
         T_CBCS_CH cbcs;	
            if (!m_DataCtrl.Get_SeismicCapBeamCbcs(bElastic,ElemK,cbcs)) continue;		
            int iCount = 0;
            for (int j=0; j<4; j++) //i,j
            {
                if (!cbcs.cbcsBase[j].bCHK) continue;
                if(!bE1 && !bElastic)
                {
                    if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
                    {
                        dMs= GetBentCapMs_CJJ((j==0 || j==2 ? TRUE : FALSE),BentCapPierList);
                    }
                    double dMG = m_DataCtrl.Get_MGofElem(ElemK,j);//取得截面最大Mg
                    cbcs.cbcsBase[j].dMuy = (1.2*dMs + dMG) + cbcs.cbcsBase[j].dMuy;
                    cbcs.cbcsBase[j].drMuy = cbcs.cbcsBase[j].dMuy;
                }
                
                //3:get section of checking ELEM
                _DGN_RC_SECTION_CH sectioni; sectioni.Initialize();
                if (!m_DataCtrl.Get_BeamSection(ElemK,j/2,sectioni))	continue;
                _DGN_BENTCAP_D_W DgnBentCapD;     DgnBentCapD.Initialize();
                DgnBentCapD.nElemK = ElemK;			DgnBentCapD.nPart = j;
                DgnBentCapD.RcMatlCH  = WA_Convert::Convert_A2W(matl);    
                DgnBentCapD.SectionCH = WA_Convert::Convert_A2W(sectioni);
                DgnBentCapD.dL0 = dL0;
                //==================================================================================================
                T_DETAIL_STR DetailStr; DetailStr.Initialize();
                BOOL bVersionOK = FALSE;
                BOOL bCheck = (*pDgnCalc)(&DgnBentCapD, &cbcs.cbcsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
                BOOL bPrinti = GetPrintRpt(nOutputType, j);
                BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN盖梁抗弯强度验算:\n"), bPrintTitle,bPrinti,DetailStr);
                if (!bAfterCheck) continue;
                //==================================================================================================
            }//endof for(j)
            for (int j=0;j<4;j++) 
            {
                m_DataCtrl.Get_CHRCCbcsBaseCodeToCurr(cbcs.cbcsBase[j]);  //change UNIT      
                if (!cbcs.cbcsBase[j].bCHK) continue;
                int iPos = (j==0 || j==1 ? 1 : 2);
                int iTyp = (j==0 || j==2 ? 1 : 2);
                Write_Command(_T("SBBD_JT"));//BentCap_Bend         
                Write_Int(ElemK);
                Write_Int(iPos);
                Write_Int(iTyp);
                Write_Int(cbcs.cbcsBase[j].LcomK);
                Write_Int(cbcs.cbcsBase[j].iMax);
                Write_Int(cbcs.cbcsBase[j].bOK);
                Write_Double(cbcs.cbcsBase[j].dMuy);//Mp0
                Write_Double(cbcs.cbcsBase[j].drMuy);	   
                Write_Double(cbcs.cbcsBase[j].dMny);		   
                Write_Double(cbcs.cbcsBase[j].dRatY); 
                m_iTotLineNum++;
                iCount++;
            }
            if(iCount > 0)
            {
                if(bE1) m_E1BentCap_Bend_KList.Add(ElemK);
                if(!bE1) m_E2BentCap_Bend_KList.Add(ElemK);
            }
        }//endof for(i)	
    }
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}              
//盖梁强度-抗剪（E2-弹塑性）
BOOL CDgnJTG_D62_04_Seismic::SeismicBentCapShear(BOOL bE1, BOOL bElastic)
{
    BOOL bPrintTitle = TRUE;
    //===========================Explicit Linkage for DLL===============================================
    typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
    PDGNCALC *pDgnCalc;
    pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_BentCap_Shear")); 
    //==================================================================================================
    size_t sizeIn  = sizeof(_DGN_BENTCAP_D_W);
    size_t sizeResu = sizeof(T_CSIS_CH_BASE);
    for (int m=0; m<m_BentCapList.GetSize(); m++) //all pier members
    {
        UINT BentCapMember = m_BentCapList.GetAt(m);
        CArray<UINT,UINT> BentCapElemKList;
        if(!m_DataCtrl.Get_CapBeamElemListByCapBeamId (BentCapMember, BentCapElemKList)) continue;    

        //Get parameters data from cap Beam; 
        int nOutputType = 0;
        double dL0 = 0;//seisType.dCappingBeamSpan
        if(!m_DataCtrl.GetParaFromCapBeam(BentCapMember,dL0, nOutputType)) continue;

        //E2弹塑性
        int elemk[2], ij[2]; 
        double dVc0=.0;
        if(!bE1 && !bElastic)
        {
            m_DataCtrl.GetElemkijFromCapBeamMemberID(BentCapMember, elemk, ij);
            _DGN_RC_SECTION_CH sectionMemberi, sectionMemberj; 
            if (!m_DataCtrl.Get_BeamSection(elemk[0],ij[0],sectionMemberi)) continue;  
            if (!m_DataCtrl.Get_BeamSection(elemk[1],ij[1],sectionMemberj)) continue;
            
            _DGN_RC_MATL_CH Membermatli,Membermatlj;
            if (!m_DataCtrl.Get_Matl(elemk[0],Membermatli)) continue;
            if (!m_DataCtrl.Get_Matl(elemk[1],Membermatlj)) continue;
            dVc0 =  GetBentCapVc0(dL0,Membermatli,Membermatlj,sectionMemberi,sectionMemberj);
        }
        
        for (int i=0; i<BentCapElemKList.GetSize(); i++) 
        {
            T_ELEM_K ElemK = BentCapElemKList.GetAt(i); 
            
            //0.get design material of checking ELEM
            _DGN_RC_MATL_CH matl;		matl.Initialize();
            if (!m_DataCtrl.Get_Matl(ElemK,matl)) continue;
            T_CSIS_CH csis; csis.Initialize();
            if (!m_DataCtrl.Get_SeismicCapBeamShear(bElastic,ElemK,csis)) continue;//E2
            int iCount = 0;
            for (int j=0; j<4; j++) 
            {
                if (!csis.CsisBase[j].bCHK) continue;
                if(!bE1 && !bElastic)
                {
                    csis.CsisBase[j].dVu  = dVc0 + fabs(csis.CsisBase[j].dVu);//7.3.6
                    csis.CsisBase[j].drVu  = csis.CsisBase[j].dVu;//7.3.6
                }

                //1:get section of checking ELEM
                _DGN_RC_SECTION_CH sectioni; sectioni.Initialize();
                if (!m_DataCtrl.Get_BeamSection(ElemK,j/2,sectioni)) continue;
         
                //!/
                _DGN_BENTCAP_D_W DgnBentCapD;     DgnBentCapD.Initialize();
                DgnBentCapD.nElemK = ElemK;			DgnBentCapD.nPart = j;
                DgnBentCapD.RcMatlCH  = WA_Convert::Convert_A2W(matl);   
                DgnBentCapD.SectionCH = WA_Convert::Convert_A2W(sectioni);
                DgnBentCapD.dL0 = dL0;
                //==================================================================================================
                T_DETAIL_STR DetailStr;      DetailStr.Initialize();
                BOOL bVersionOK=FALSE;
                BOOL bCheck = (*pDgnCalc)(&DgnBentCapD, &csis.CsisBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
                BOOL bPrinti = GetPrintRpt(nOutputType, j/2);
                BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN盖梁抗剪强度验算:\n"), bPrintTitle,bPrinti,DetailStr);
                if (!bAfterCheck) continue;
                //==================================================================================================
            }//endof for(j)
            for (int j=0;j<4;j++) 
            {
                m_DataCtrl.Get_CHRCCsisBaseCodeToCurr(csis.CsisBase[j]);  //change UNIT
                if (!csis.CsisBase[j].bCHK) continue;
                int iPos = (j==0 || j==1 ? 1 : 2);
                int iTyp = (j==0 || j==2 ? 1 : 2);
                Write_Command(_T("SBSR_JT"));//BentCap_Shear
                Write_Int(ElemK);
                Write_Int(iPos);
                Write_Int(iTyp);
                Write_Int(csis.CsisBase[j].LcomK);
                Write_Int(csis.CsisBase[j].iMax);
                Write_Int(csis.CsisBase[j].bOK);
                Write_Double(csis.CsisBase[j].dVu);//Vc0
                Write_Double(csis.CsisBase[j].drVu);
                Write_Double(csis.CsisBase[j].dVn);//Vn
                Write_Double(csis.CsisBase[j].dRatV);
                Write_Double(csis.CsisBase[j].dShrSect);
                Write_Double(csis.CsisBase[j].dShrLoad);
                m_iTotLineNum++;
                iCount++;
            }
            if(iCount > 0)
            {
                if(bE1) m_E1BentCap_Shear_KList.Add(ElemK);
                if(!bE1) m_E2BentCap_Shear_KList.Add(ElemK);
            }
        }//endof for(i)
    }//endof for(m)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));  
	return TRUE;
}             
//基础强度验算
BOOL CDgnJTG_D62_04_Seismic::SeismicFoundation(BOOL bE1)
{  
    BOOL bPrintTitle = TRUE;
    //========================Explicit Linkage for DLL==================================================
    typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
    PDGNCALC *pDgnCalc;
    pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Foundation")); 
    //==================================================================================================
    size_t sizeIn  = sizeof(_DGN_FOUND_D_W);
    size_t sizeRes = sizeof(T_FOUND_CH_BASE);
    for (int i=0; i<m_FoundationList.GetSize(); i++) 
    {
        T_ELEM_K ElemK = m_FoundationList.GetAt(i); 
        
        //for design items
        T_RSDM_D seisType;  seisType.Initialize();
        if(!m_DataCtrl.GetElemSeisChkType(ElemK, seisType)) continue;
        //if (seisType.nMemberType!=2)  continue;
        if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
        {
            if (seisType.nMemberType!=2 )  continue;
        }
        else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (seisType.nMemberType!=13 )  continue;
        }
        else
        {
            continue;
        }
        //0.get design force
        T_FOUND_CH funddata;//result data
        if (!m_DataCtrl.Get_SeismicFounD(ElemK,funddata)) continue;//E2 getStress is same to E1
        int iCount = 0;
        for (int j=0; j<2; j++) 
        {
            funddata.FoundBase[j].dPn1 = seisType.dFdnAllowGround;
            //!/
            _DGN_FOUND_D_W DgnBentCapD;       DgnBentCapD.Initialize();
            DgnBentCapD.nElemK = ElemK;			DgnBentCapD.nPart = j;
            //==================================================================================================
            T_DETAIL_STR DetailStr;       DetailStr.Initialize();
            BOOL bVersionOK=FALSE;
            BOOL bCheck = (*pDgnCalc)(&DgnBentCapD, &funddata.FoundBase[j], &DetailStr, sizeIn, sizeRes, bVersionOK);
            //check or not
            if (bCheck)	funddata.FoundBase[j].bCHK = TRUE;
            BOOL bPrinti = GetPrintRpt(seisType.nOutputType, j/2);
            BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN基础强度验算:\n"), bPrintTitle,bPrinti,DetailStr);//E2作用下
            if (!bAfterCheck) continue;
            //==================================================================================================
        }//endof for(j)
        for (int j=0;j<2;j++) 
        {
            m_DataCtrl.Get_SeisFoundBaseCodeToCurr(funddata.FoundBase[j]);  //change UNIT
            if (!funddata.FoundBase[j].bCHK) continue;
            int iPos = (j==0 || j==1 ? 1 : 2);
            int iTyp = (j==0 || j==2 ? 1 : 2);
            Write_Command(_T("SFCK_JT"));//SeismicFoundation
            Write_Int(ElemK);
            Write_Int(iPos);
            Write_Int(iTyp);
            Write_Int(funddata.FoundBase[j].LcomK);
            Write_Int(funddata.FoundBase[j].iMax);
            Write_Int(funddata.FoundBase[j].bOK);
            Write_Double(funddata.FoundBase[j].dPu1);
            Write_Double(funddata.FoundBase[j].dPn1);
            Write_Double(funddata.FoundBase[j].dPu2);
            Write_Double(funddata.FoundBase[j].dPn2);
            m_iTotLineNum++;
            iCount++;
        }
        if(iCount > 0)
        {
            if(bE1) m_E1Foundation_KList.Add(ElemK);
            if(!bE1) m_E2Foundation_KList.Add(ElemK);
        }
    }//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

//桥墩塑性铰区抗剪强度
BOOL CDgnJTG_D62_04_Seismic::SeismicE2PierShear()
{ 
    BOOL bPrintTitle = TRUE;
    //====================Explicit Linkage for DLL==================================================
    typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
    PDGNCALC *pDgnCalc;
    
    if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
    {
        pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Pier_Shear")); 
    }
    else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
    {
        pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Pier_Shear_CJJ")); 
    }
    else
    {
        ASSERT(0);
    }
    //==============================================================================================
    size_t sizeIn  = sizeof(_DGN_PIER_D_W);
    size_t sizeRes = sizeof(T_CSIS_CH_BASE);
    
    for (int m=0; m<m_PierList.GetSize(); m++) //all pier members
    {
        UINT PierMember = m_PierList.GetAt(m);
        CArray<UINT,UINT> PierElemKList;
        if(!m_DataCtrl.Get_PierElemListByPierID (PierMember, PierElemKList)) continue;
        
        //取得构件所有荷载组合的内力-0:顶部；1:底部
        CArray <_DGN_FORC_CRC,_DGN_FORC_CRC> aForceList;
        if(!m_DataCtrl.GetEndForcefromPierMember(PierMember, aForceList)) continue;

        
        //0.get Δd of checking Member
        T_FRUSTATOPDISP_CH data; data.Initialize();
        if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (!m_DataCtrl.Get_SeismicE2FrustaTopDisp(PierMember,data))
                continue;//区分顺桥向横桥向
        }
        //Get parameters data from pier
        BOOL bOnlyBot = TRUE, bDoublePier = FALSE;
        double dHn =.0, dDisp =.0;
        int nOutputType = 0;
        if(!m_DataCtrl.GetParaFromPier(PierMember,bOnlyBot,dHn,bDoublePier,dDisp,nOutputType)) continue;
        
        
        for(int i=0; i<PierElemKList.GetSize(); i++)
        {
            T_ELEM_K ElemK = PierElemKList.GetAt(i);       
            //0.get design material of checking ELEM
            _DGN_RC_MATL_CH matl;		matl.Initialize();
            if (!m_DataCtrl.Get_Matl(ElemK,matl)) continue;
            T_CSIS_CH csis; csis.Initialize();     
            
            T_RSDL_D RsdlD;
            if(!m_pDoc->m_pAttrCtrl->GetRsdl(PierMember, RsdlD)) continue;

            T_MPCC_D MpccData;
            if(!m_pDoc->m_pAttrCtrl2->GetMpcc(RsdlD.PierMpccKey, MpccData)) { ASSERT(0); continue; }

            double dPhiy[2], dPhiu[2];
            if(!m_DataCtrl.Get_RsdlCurvature(PierMember, dPhiy, dPhiu)) continue;

            int iCount = 0;
            for (int j=0; j<4; j++) //i-Longitudinal,i-Transverse ; j-Longitudinal,j-Transverse 
            {
                csis.CsisBase[j].bCHK = TRUE;
                BOOL bLongitudinal = (j==0||j==2)?TRUE:FALSE;
                int iLcomk =0;
                csis.CsisBase[j].dVu = GetPierVc0(PierMember, aForceList, bLongitudinal, bOnlyBot, dHn,iLcomk);
                csis.CsisBase[j].LcomK = iLcomk;
                if (!csis.CsisBase[j].bCHK) continue;
                //1:get section of checking ELEM
                _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
                if (!m_DataCtrl.Get_ColumnSection(ElemK,j/2,sectioni)) continue;
                ////add by maxiao 2011-10-8.
                double dAx,dAy,dDx,dDy;
                BOOL bget = m_DataCtrl.Get_SeismicStirrup(ElemK, dAx, dAy, dDx, dDy);
                if (bget)
                {         
                    if (bLongitudinal)   {  sectioni.dAsh = dAx;    }
                    else                 {  sectioni.dAsh = dAy;    }
                }

                _DGN_PIER_D_W DgnD;     DgnD.Initialize();
                DgnD.nElemK = ElemK;			DgnD.nPart = j;
                DgnD.RcMatlCH  = WA_Convert::Convert_A2W(matl);    
                DgnD.SectionCH = WA_Convert::Convert_A2W(sectioni);
                if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
                {
                    if(RsdlD.nMemberType == 12)
                    {
                        DgnD.PairPierCH.dPc= (j==0 || j==2) ?RsdlD.dPireMaxMinForce[2]:RsdlD.dPireMaxMinForce[3];
                    }
                    else
                    {
                        DgnD.PairPierCH.dPc=MpccData.dAxialForce;
                    }
                    DgnD.PairPierCH.dDetd = data.DataBase[bLongitudinal?0:1].dHoriDisp;

                 
                     
                     if(RsdlD.bPierDColumnBent)
                     {
                         DgnD.PairPierCH.dPierAllowDisp = RsdlD.dPierAllowDisp;
                     }
                     else
                     {
                         double dCalPhiy = dPhiy[(j == 0 || j == 1) ? 0 : 1];
                         DgnD.PairPierCH.dPierAllowDisp = RsdlD.dPierHn * RsdlD.dPierHn * dCalPhiy /3.0;  //cjj166-2011 P104 (6)
                     }
                }

                //==================================================================================================
                T_DETAIL_STR DetailStr;     DetailStr.Initialize();
                BOOL bVersionOK=FALSE;
                BOOL bCheck = (*pDgnCalc)(&DgnD, &csis.CsisBase[j], &DetailStr, sizeIn, sizeRes, bVersionOK);
                BOOL bPrinti = GetPrintRpt(nOutputType, j/2);
                BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN桥墩塑性铰区抗剪强度验算:\n"), bPrintTitle,bPrinti,DetailStr);
                if (!bAfterCheck) continue;
                //==================================================================================================
            }//endof for(j)
            for (int j=0;j<4;j++) 
            {
                m_DataCtrl.Get_CHRCCsisBaseCodeToCurr(csis.CsisBase[j]);  //change UNIT
                if (!csis.CsisBase[j].bCHK) continue;
                int iPos = (j==0 || j==1 ? 1 : 2);
                int iTyp = (j==0 || j==2 ? 1 : 2);
                Write_Command(_T("SPSR_JT"));//Pier_Shear
                Write_Int(ElemK);
                Write_Int(iPos);
                Write_Int(iTyp);
                Write_Int(csis.CsisBase[j].LcomK);
                Write_Int(csis.CsisBase[j].iMax);
                Write_Int(csis.CsisBase[j].bOK);
                Write_Double(csis.CsisBase[j].dVu);//Vc0
                Write_Double(csis.CsisBase[j].drVu);
                Write_Double(csis.CsisBase[j].dVn);//Vn
                Write_Double(csis.CsisBase[j].dRatV);
                Write_Double(csis.CsisBase[j].dShrSect);
                Write_Double(csis.CsisBase[j].dShrLoad);
                m_iTotLineNum++;
                iCount++;
            }
            if(iCount > 0) m_E2Pier_Shear_KList.Add(ElemK);
        }//endof for(i)
    }//endof for(m)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
    return TRUE;
}

//支座厚度（板式橡胶）弹性连接
BOOL CDgnJTG_D62_04_Seismic::SeismicRubberThick(BOOL bE1)
{
	BOOL bPrintTitle = TRUE;
	//====================Explicit Linkage for DLL==================================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Rubber_Thick")); 
	//==============================================================================================
	size_t sizeIn  = sizeof(_DGN_RUBBERTHICK_D_W);
    size_t sizeRes = sizeof(T_SUPPORTDISP_CH);
    for (int i=0; i<m_RubberLinkList.GetSize(); i++) 
	{
        T_ELNK_K ElnkK = m_RubberLinkList.GetAt(i); 		
		//for design items
		T_RSDM_D seisType;  seisType.Initialize();
		if(!m_DataCtrl.GetElasticLinkSeisChkType(ElnkK, seisType)) continue;	
        
        if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
        {
            if (seisType.nMemberType!=4 )  continue;
        }
        else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (seisType.nMemberType!=14 )  continue;
        }
        else
        {
            continue;
        }
        //if (seisType.nMemberType!=4 || seisType.nMemberType!=14)  continue;
		//0.get design info
		CArray<T_SEIS_LINK_DISP,T_SEIS_LINK_DISP> aLinkDispList;
		if (!m_DataCtrl.GetElasticLinkDisp(ElnkK,bE1,aLinkDispList)) continue;
        int iCount = 0;
		T_SUPPORTDISP_CH SDisp; SDisp.Initialize();
		SDisp.bCHK = TRUE;
		SDisp.iKey = ElnkK;
		SDisp.dThick = seisType.dSupportRubberThick;

		//1:get data of checking ELEM
		_DGN_RUBBERTHICK_D_W DgnDataD;   
        DgnDataD.Initialize();
        WA_Convert::Convert_A2W(aLinkDispList, DgnDataD.aLinkDispList);
		//==================================================================================================
		T_DETAIL_STR DetailStr;     DetailStr.Initialize();
		BOOL bVersionOK=FALSE;
		BOOL bCheck = (*pDgnCalc)(&DgnDataD, &SDisp, &DetailStr, sizeIn, sizeRes, bVersionOK);
		BOOL bPrinti = GetPrintRpt(seisType.nOutputType, 0);
		BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN支座厚度验算:\n"), bPrintTitle,bPrinti,DetailStr);
		if (!bAfterCheck) continue;
		//==================================================================================================
		m_DataCtrl.Get_SeisSupportDispCodeToCurr(SDisp);  //change UNIT
		if (!SDisp.bCHK) continue;
		Write_Command(_T("SRBT_JT"));//
		Write_Int(SDisp.iKey);
		Write_Int(SDisp.iType);
		Write_Int(SDisp.LcomK);
		Write_Int(SDisp.bOK);
		Write_Double(SDisp.dHoriDispX);
		Write_Double(SDisp.dHoriDispY);
		Write_Double(SDisp.dThick);
		m_iTotLineNum++;
		iCount++;		
		if(iCount > 0 )
        {
            if(bE1) m_E1RubberThick_KList.Add(ElnkK);
            if(!bE1) m_E2RubberThick_KList.Add(ElnkK);
        }
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}
//支座抗滑稳定性（板式橡胶）弹性连接
BOOL CDgnJTG_D62_04_Seismic::SeismicRubberSlip(BOOL bE1)
{
	BOOL bPrintTitle = TRUE;
	//====================Explicit Linkage for DLL==================================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Rubber_Slip")); 
	//==============================================================================================
	size_t sizeIn  = sizeof(_DGN_RUBBERSLIP_D_W);
    size_t sizeRes = sizeof(T_SUPPORTFORCE_CH);
    for (int i=0; i<m_RubberLinkList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_RubberLinkList.GetAt(i); 		
		//for design items
		T_RSDM_D seisType;  seisType.Initialize();
		if(!m_DataCtrl.GetElasticLinkSeisChkType(ElemK, seisType)) continue;	
        //if (seisType.nMemberType!=4||seisType.nMemberType!=14)  continue;
        if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
        {
            if (seisType.nMemberType!=4 )  continue;
        }
        else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (seisType.nMemberType!=14 )  continue;
        }
        else
        {
            continue;
        }
		//0.get design info
		int iCount = 0;
		CArray<T_SEIS_LINK_FORCE,T_SEIS_LINK_FORCE> aLinkForceList;
		if (!m_DataCtrl.GetElasticLinkForce(ElemK,bE1,aLinkForceList)) continue;		
		T_SUPPORTFORCE_CH SForce; SForce.Initialize();
		SForce.bCHK = TRUE;
		SForce.iKey = ElemK;//支座编号

		//1:get data of checking ELEM
		_DGN_RUBBERSLIP_D_W DgnDataD;     DgnDataD.Initialize();
		WA_Convert::Convert_A2W(aLinkForceList, DgnDataD.aLinkForceList);
		DgnDataD.dFricCoef = seisType.dSupportFrictionFactor;
		//==================================================================================================
		T_DETAIL_STR DetailStr;     DetailStr.Initialize();
		BOOL bVersionOK=FALSE;
		BOOL bCheck = (*pDgnCalc)(&DgnDataD, &SForce, &DetailStr, sizeIn, sizeRes, bVersionOK);
		BOOL bPrinti = GetPrintRpt(seisType.nOutputType, 0);
		BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN支座抗滑稳定性验算:\n"), bPrintTitle,bPrinti,DetailStr);
		if (!bAfterCheck) continue;
		//==================================================================================================
		m_DataCtrl.Get_SeisSupportForceCodeToCurr(SForce);  //change UNIT
		if (!SForce.bCHK) continue;
		Write_Command(_T("SRBS_JT"));//
		Write_Int(SForce.iKey);
		Write_Int(SForce.iType);
		Write_Int(SForce.LcomK);
		Write_Int(SForce.bOK);
		Write_Double(SForce.dHoriForceX);
		Write_Double(SForce.dHoriForceY);
		Write_Double(SForce.dAllowForce);
		m_iTotLineNum++;
		iCount++;
        if(iCount >0)
        {
            if (bE1)	m_E1RubberSlip_KList.Add(ElemK);
            if (!bE1) m_E2RubberSlip_KList.Add(ElemK);
        }
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

//支座水平位移（盆式活动支座）一般连接
BOOL CDgnJTG_D62_04_Seismic::SeismicActiveHoriDisp(BOOL bE1)
{
	BOOL bPrintTitle = TRUE;
	//================================Explicit Linkage for DLL======================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Active_HoriDisp")); 
	//==============================================================================================
	size_t sizeIn  = sizeof(_DGN_RUBBERTHICK_D_W);
    size_t sizeRes = sizeof(T_SUPPORTDISP_CH);
    for (int i=0; i<m_ActiveLinkList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ActiveLinkList.GetAt(i); 		
		//for design items
		T_RSDM_D seisType;  seisType.Initialize();
		if(!m_DataCtrl.GetGeneralLinkSeisChkType(ElemK, seisType)) continue;		
        //if (seisType.nMemberType!=5)  continue;
        if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
        {
            if (seisType.nMemberType!=5 )  continue;
        }
        else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (seisType.nMemberType!=15 )  continue;
        }
        else
        {
            continue;
        }
		//0.get design info
		CArray<T_SEIS_LINK_DISP,T_SEIS_LINK_DISP> aLinkDispList;
		if (!m_DataCtrl.GetGeneralLinkDisp(ElemK,bE1,aLinkDispList)) continue;
        int iCount = 0;
		T_SUPPORTDISP_CH SDisp; SDisp.Initialize();
		SDisp.bCHK = TRUE;
		SDisp.iKey = ElemK;
		SDisp.dThick = seisType.dSupportAllowHorizonDis;//水平位移
		//1:get data of checking ELEM
		_DGN_RUBBERTHICK_D_W DgnDataD;     DgnDataD.Initialize();
		WA_Convert::Convert_A2W(aLinkDispList, DgnDataD.aLinkDispList);
		//==================================================================================================
		T_DETAIL_STR DetailStr;     DetailStr.Initialize();
		BOOL bVersionOK=FALSE;
		BOOL bCheck = (*pDgnCalc)(&DgnDataD, &SDisp, &DetailStr, sizeIn, sizeRes, bVersionOK);
		BOOL bPrinti = GetPrintRpt(seisType.nOutputType, 0);
		BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN支座水平位移验算:\n"), bPrintTitle,bPrinti,DetailStr);
		if (!bAfterCheck) continue;
		//==================================================================================================
		m_DataCtrl.Get_SeisSupportDispCodeToCurr(SDisp);  //change UNIT
		if (!SDisp.bCHK) continue;
		Write_Command(_T("SAHD_JT"));//
		Write_Int(SDisp.iKey);
		Write_Int(SDisp.iType);
		Write_Int(SDisp.LcomK);
		Write_Int(SDisp.bOK);
		Write_Double(SDisp.dHoriDispX);
		Write_Double(SDisp.dHoriDispY);
		Write_Double(SDisp.dThick);
		m_iTotLineNum++;
		iCount++;
        if (bE1){	if(iCount > 0) m_E1ActiveHoriDisp_KList.Add(ElemK);}
        else		{	if(iCount > 0) m_E2ActiveHoriDisp_KList.Add(ElemK);}
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}
//支座水平承载力（盆式固定支座）
BOOL CDgnJTG_D62_04_Seismic::SeismicStatHoriForce(BOOL bE1)
{
	BOOL bPrintTitle = TRUE;
	//====================Explicit Linkage for DLL==================================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Stat_HoriForce")); 
	//==============================================================================================
	size_t sizeIn  = sizeof(_DGN_RUBBERSLIP_D_W);
    size_t sizeRes = sizeof(T_SUPPORTFORCE_CH);
    for (int i=0; i<m_FixedLinkList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_FixedLinkList.GetAt(i); 		
		//for design items
		T_RSDM_D seisType;  seisType.Initialize();
		if(!m_DataCtrl.GetElasticLinkSeisChkType(ElemK, seisType)) continue;		
        //if (seisType.nMemberType!=6)  continue;
        if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
        {
            if (seisType.nMemberType!=6 )  continue;
        }
        else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
        {
            if (seisType.nMemberType!=16 )  continue;
        }
        else
        {
            continue;
        }
		//0.get design info
		int iCount = 0;
		CArray<T_SEIS_LINK_FORCE,T_SEIS_LINK_FORCE> aLinkForceList;
		if (!m_DataCtrl.GetElasticLinkForce(ElemK,bE1,aLinkForceList)) continue;		
		T_SUPPORTFORCE_CH SForce; SForce.Initialize();
		SForce.bCHK = TRUE;
		SForce.iKey = ElemK;//支座编号
		SForce.dAllowForce = seisType.dSupportMaxHorizonLoad;
		//1:get data of checking ELEM
		_DGN_RUBBERSLIP_D_W DgnDataD;     DgnDataD.Initialize();
        WA_Convert::Convert_A2W(aLinkForceList, DgnDataD.aLinkForceList);
		DgnDataD.dFricCoef = seisType.dSupportFrictionFactor;
		//==================================================================================================
		T_DETAIL_STR DetailStr;     DetailStr.Initialize();
		BOOL bVersionOK=FALSE;
		BOOL bCheck = (*pDgnCalc)(&DgnDataD, &SForce, &DetailStr, sizeIn, sizeRes, bVersionOK);
		BOOL bPrinti = GetPrintRpt(seisType.nOutputType, 0);
		BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN固定盆式支座水平承载力验算:\n"), bPrintTitle,bPrinti,DetailStr);
		if (!bAfterCheck) continue;
		//==================================================================================================
		m_DataCtrl.Get_SeisSupportForceCodeToCurr(SForce);  //change UNIT
		if (!SForce.bCHK) continue;
		Write_Command(_T("SSHD_JT"));//
		Write_Int(SForce.iKey);
		Write_Int(SForce.iType);
		Write_Int(SForce.LcomK);
		Write_Int(SForce.bOK);
		Write_Double(SForce.dHoriForceX);
		Write_Double(SForce.dHoriForceY);
		Write_Double(SForce.dAllowForce);
		m_iTotLineNum++;
		iCount++;
        if (bE1){	if(iCount > 0) m_E1StatHoriForce_KList.Add(ElemK);}
        else		{	if(iCount > 0) m_E2StatHoriForce_KList.Add(ElemK);}
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}             

//墩顶位移（规则桥梁）仅反应谱
//一个墩输出一组值：0-顺桥向；1-横桥向 
BOOL CDgnJTG_D62_04_Seismic::SeismicE2FrustaTopDisp()
{
	BOOL bPrintTitle = TRUE;
	//====================Explicit Linkage for DLL==================================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_FrustaTop_Disp")); 
    if(m_Rc_D_CH.iDgnCode == RC_JTG_B02_01_2008)
    {
        pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_FrustaTop_Disp")); 
    }
    else if(m_Rc_D_CH.iDgnCode == RC_CJJ_166_2011)
    {
        pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_FrustaTop_Disp_CJJ")); 
    }
    else
    {
        ASSERT(0);
    }
	//==============================================================================================
	size_t sizeIn  = sizeof(_DGN_DISP_D_W);
    size_t sizeRes = sizeof(T_FRUSTATOPDISP_CH_BASE);

    for (int i=0; i<m_PierList.GetSize(); i++) //all pier members
    {
        T_RSDL_K PierMember = m_PierList.GetAt(i);

        //Get parameters data from pier
        BOOL bOnlyBot = TRUE, bDoublePier = FALSE;
        double dHn =.0, dDisp =.0;
        int nOutputType = 0;
        if(!m_DataCtrl.GetParaFromPier(PierMember,bOnlyBot,dHn,bDoublePier,dDisp,nOutputType)) continue;

        //0.get Δd of checking Member
        T_FRUSTATOPDISP_CH data; data.Initialize();
        
        {
            if (!m_DataCtrl.Get_SeismicE2FrustaTopDisp(PierMember,data))
                continue;//区分顺桥向横桥向
        }

        //1.Get the Elemk and ij of Member.     
        T_ELEM_K iMemberElemk[2]; 
		int ij[2];
        if(!m_DataCtrl.GetElemkijFromPierMemberID(PierMember, iMemberElemk, ij)) continue; // 0:Top or left, 1:bottom or right. 
        int ElemK = iMemberElemk[0];

		//2.get design material of Member's Top ELEM
		_DGN_RC_MATL_CH matl;		matl.Initialize();
		if (!m_DataCtrl.Get_Matl(ElemK,matl)) continue;

        //3:get section of checking Member's Top ELEM
        _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
        if (!m_DataCtrl.Get_ColumnSection(ElemK,ij[0],sectioni)) continue;
        
        double dT=0.25, dTg = 0.25;
        if(!m_DataCtrl.Get_TTg(dT, dTg)) continue;//and by maxiao 2010.01.12
        int iCount = 0;
		for (int j=0; j<2; j++) 
		{
			data.DataBase[j].bCHK = TRUE;
            if (!data.DataBase[j].bCHK) continue;
            //change Unit..
            data.DataBase[j].dHoriDisp = m_DataCtrl.Cng_LengthFromCurrToCode(data.DataBase[j].dHoriDisp,1);
            _DGN_DISP_D_W DgnD;       DgnD.Initialize();
			DgnD.db = sectioni.db;  DgnD.dh = sectioni.dh;
			DgnD.dr = sectioni.dr;  DgnD.dL0 = dHn;
			DgnD.dfkh = matl.dfsky; DgnD.dfc = matl.dfcuk;
			DgnD.dfy = matl.dfsk;   DgnD.dDs = sectioni.dd; 
            DgnD.bDoublePier = bDoublePier; DgnD.dDisp = dDisp;
            DgnD.dT = dT;           DgnD.dTg = dTg;
            //Get Phi-y,Phi-u.
            double dPhiy[2], dPhiu[2];
            if(!m_DataCtrl.Get_RsdlCurvature(PierMember, dPhiy, dPhiu)) continue;
            DgnD.dPhiy = dPhiy[j]; DgnD.dPhiu = dPhiu[j];
            //  Get_PhiyPhiuOfPier(ElemK,ij[0],DgnD.dPhiy, DgnD.dPhiu)
            //==================================================================================================
            T_DETAIL_STR DetailStr;     DetailStr.Initialize();
            BOOL bVersionOK=FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnD, &data.DataBase[j], &DetailStr, sizeIn, sizeRes, bVersionOK);
			BOOL bPrinti = GetPrintRpt(nOutputType, j);
			BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN规则桥梁墩顶位移验算:\n"), bPrintTitle,bPrinti,DetailStr);
			if (!bAfterCheck) continue;
            //==================================================================================================
		}//endof for(j)
		for (int j=0;j<2;j++) 
		{
			m_DataCtrl.Get_SeismicFrustaTopBaseCodeToCurr(data.DataBase[j]);  //change UNIT
            if (!data.DataBase[j].bCHK) continue;
			Write_Command(_T("SFTD_JT"));                 //_T("SFTD_JT")
			Write_Int(data.DataBase[j].iKey);         //MemberID
			Write_Int(data.DataBase[j].iDirect);      //0-顺桥向；1-横桥向
			Write_Int(data.DataBase[j].LcomK);        //not use
			Write_Int(data.DataBase[j].bOK);          //OK/NG
			Write_Double(data.DataBase[j].dHoriDisp); //Δd
			Write_Double(data.DataBase[j].dAlwDisp);  //Δu
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_E2FrustaTopDisp_KList.Add(PierMember);
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

//塑形转角位移（非规则桥梁） 仅时程
//一个墩输出一组值：0-顺桥向；1-横桥向
BOOL CDgnJTG_D62_04_Seismic::SeismicE2CornerDisp()
{
	BOOL bPrintTitle = TRUE;
	//====================Explicit Linkage for DLL==================================================
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Coner_Disp")); 
	//==============================================================================================
	size_t sizeIn  = sizeof(_DGN_DISP_D_W);
    size_t sizeRes = sizeof(T_CORNERDISP_CH_BASE);

    for (int i=0; i<m_PierList.GetSize(); i++) //all pier members
    {
        T_RSDL_K PierMember = m_PierList.GetAt(i);

        //Get parameters data from pier
        BOOL bOnlyBot = TRUE, bDoublePier = FALSE;
        double dHn =.0, dDisp =.0;
        int nOutputType = 0;
        if(!m_DataCtrl.GetParaFromPier(PierMember,bOnlyBot,dHn,bDoublePier,dDisp,nOutputType)) continue;
        
        //0.get maxθp of checking Member
        T_CORNERDISP_CH data; data.Initialize();
//    CArray<UINT, UINT> LongitudinalElemKList;//顺桥向最大塑性铰区域单元List
//    CArray<UINT, UINT> TransverseElemKList;  //横桥向最大塑性铰区域单元List
        if (!m_DataCtrl.Get_SeismicE2Theta(PierMember,data)) continue;//区分顺桥向横桥向

        //1.Get Phi-y,Phi-u.//对应θp最大塑性区域所有单元的θu最小时
        double dPhiy[2]={.0,.0}, dPhiu[2]={.0,.0};
        
        if(!m_DataCtrl.Get_RsdlCurvature(PierMember, dPhiy, dPhiu)) continue;
        //get section
        CArray<T_ELEM_K, T_ELEM_K> ElemKList;
        if(!m_DataCtrl.Get_PierElemListByPierID (PierMember, ElemKList)) continue;
        _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
        if(ElemKList.GetSize()<=0) continue;
        if (!m_DataCtrl.Get_ColumnSection(ElemKList[0],0,sectioni)) continue;    
        _DGN_RC_MATL_CH matl;		matl.Initialize();
        if (!m_DataCtrl.Get_Matl(ElemKList[0],matl)) continue;
//    if(!GetThetauData(LongitudinalElemKList, dHn, db[0], dh[0], dr[0],dfkh, dfc, dfy, dDs,dPhiy[0],dPhiu[0])) continue;
//    if(!GetThetauData(TransverseElemKList, dHn, db[1], dh[1], dr[1],dfkh, dfc, dfy, dDs,dPhiy[1],dPhiu[1])) continue;
        int iCount = 0;
		for (int j=0; j<2; j++) 
		{
			data.DataBase[j].bCHK = TRUE;
            if (!data.DataBase[j].bCHK) continue;
            _DGN_DISP_D_W DgnD;       DgnD.Initialize();
			DgnD.db = sectioni.db;  DgnD.dh = sectioni.dh;
			DgnD.dr = sectioni.dr;  DgnD.dL0 = dHn;
//      DgnD.dfkh = matl.dfsky; DgnD.dfc = matl.dfcuk;//      DgnD.dfy = matl.dfsk;   DgnD.dDs = sectioni.dd; 
            DgnD.dfkh = matl.dfsky; DgnD.dfc = matl.dfcuk;
            DgnD.dfy = matl.dfsk;   DgnD.dDs = sectioni.dd; 
            DgnD.dPhiy = dPhiy[j];   DgnD.dPhiu = dPhiu[j]; 
            //==================================================================================================
            T_DETAIL_STR DetailStr;     DetailStr.Initialize();
            BOOL bVersionOK=FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnD, &data.DataBase[j], &DetailStr, sizeIn, sizeRes, bVersionOK);
			BOOL bPrinti = GetPrintRpt(nOutputType, j);
			BOOL bAfterCheck = AfterCheck(bVersionOK,_T("**BEGIN非规则桥梁塑性转角变形验算:\n"), bPrintTitle,bPrinti,DetailStr);
			if (!bAfterCheck) continue;
            //==================================================================================================
		}//endof for(j)
		for (int j=0;j<2;j++) 
        { //change UNIT转角不换算单位       
            if (!data.DataBase[j].bCHK) continue;
			Write_Command(_T("SCND_JT"));                //SCND_JT
			Write_Int(data.DataBase[j].iKey);        //MemberID
			Write_Int(data.DataBase[j].iDirect);     //0-顺桥向；1-横桥向
			Write_Int(data.DataBase[j].LcomK);       //not use
			Write_Int(data.DataBase[j].bOK);         //OK/NG
			Write_Double(data.DataBase[j].dHoriDisp);//θp
			Write_Double(data.DataBase[j].dAlwDisp); //θu
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_E2CornerDisp_KList.Add(PierMember);
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

/************************************************************************/
/* 获得是否输出计算书的标识
 * iPrintRpt:用户选择的计算书输出选项 0-不输出;1-i端;2-j端;3-i&j(输出)
 * ij:验算位置 0-i端; 1-j端                                             */
/************************************************************************/
BOOL CDgnJTG_D62_04_Seismic::GetPrintRpt(int iPrintRpt, int ij)
{
	BOOL bprint = FALSE;
	if (iPrintRpt==1 && ij==0) bprint = TRUE;
	else if (iPrintRpt==2 && ij ==1) bprint = TRUE;
	else if (iPrintRpt == 3) bprint= TRUE;
	return bprint;
}

//求延性柱剪力设计值 6.8.2
//iMember-构件号
//aForceList-构件所有荷载组合的内力-0:顶部；1:底部
//bLongitudinal-TRUE:顺桥向 FALSE:横桥向
//bOnlyBot-TRUE:底部为塑性铰区域 FALSE:顶底部为塑性铰区域
//dHn:构件长度
double CDgnJTG_D62_04_Seismic::GetPierVc0(T_RSDL_K iMember, CArray <_DGN_FORC_CRC,_DGN_FORC_CRC>& aForceList, BOOL bLongitudinal, BOOL bOnlyBot, double dHn, int& iLcomK)
{
    double dVc0 = 0.0;
    if(dHn<m_dZero) return .0;
    double dMx=0, dMs=0;
    for(int i=0; i<aForceList.GetSize(); i++)
    {
        _DGN_FORC_CRC force = aForceList.GetAt(i);
        //底部
        double dP = force.dFxx[1];//bot
        double dMn = 0.0;//取底部PM曲线对应的M值  //顺桥或者横桥
        if(m_DataCtrl.GetMfromPierPM(iMember,FALSE,bLongitudinal,dP,dMn))
        {
            if(dMn>=dMx) iLcomK = force.iRefNo[1];
            dMx = max(dMx,dMn);    
        }
        //顶底部，需要取顶部
        if(!bOnlyBot){
            dP = force.dFxx[0];//top
            if(m_DataCtrl.GetMfromPierPM(iMember,TRUE,bLongitudinal,dP,dMn)) //取顶部PM曲线对应的M值
            dMs = max(dMs,dMn);
        }
    }
    //求Vc0
    if(bOnlyBot)
    {
        dVc0 = 1.2*dMx/dHn;
    }
    else
    {
        dVc0 = 1.2*(dMx+dMs)/dHn;
    }
    //取Vc0的最大值
    return dVc0;
}


//求盖梁剪力设计值
double CDgnJTG_D62_04_Seismic::GetBentCapVc0(double dL0,_DGN_RC_MATL_CH matli,_DGN_RC_MATL_CH matlj,_DGN_RC_SECTION_CH sectionMemberi,_DGN_RC_SECTION_CH sectionMemberj)
{  //Mn的求法
    double dVc0 = 0.0;
    if(dL0<m_dZero) return .0;
    double dML = GetMn(matli, sectionMemberi);//i
    double dMR = GetMn(matlj, sectionMemberj);//j
    dVc0 = 1.2*(dML+dMR)/dL0;
    return dVc0;
}

//求弯矩承载力-仅考虑矩形，因此不考虑Muy引起的截面变化
double CDgnJTG_D62_04_Seismic::GetMn(_DGN_RC_MATL_CH RcMatlCH, _DGN_RC_SECTION_CH SectionCH)
{
    double dMny = 0.0;
    double dAs = 0.0, dAs1 = 0.0, das = 0.0, das1 = 0.0;
    double dfsd = 0.0, dfsd1 = 0.0, dfcd = 0.0, dftd = 0.0; 
    double dSib = 0.0;
    double db = 0.0, dh = 0.0, dh0 = 0.0;  
    dAs = SectionCH.dAs; dAs1 = SectionCH.dAs1;
    das = SectionCH.das; das1 = SectionCH.das1;
    dfsd = RcMatlCH.dfsk; dfsd1 = RcMatlCH.dfsk;   //此处换为标准值6.8.4
    dfcd = RcMatlCH.dfck1; dftd = RcMatlCH.dftk1;  //此处换为标准值6.8.4
    dSib = RcMatlCH.dsib;
    db = SectionCH.db; dh = SectionCH.dh;  
    dh0 = dh - das;
    
    //Only Rect.		
    double dx = (dfsd*dAs-dfsd1*dAs1)/(dfcd*db);//5.2.2-2
    //check x. see [JTG D62--2004] 5.2.2-3~5  P25.
    double sibho = dSib * dh0;
    if (dx > sibho)
    {
        dx = sibho;
    }
    //get Mny
    if (dx < 2*das1) 
    {
        dMny = dfsd*dAs*(dh0-das1);	
    }
    else
    {
        dMny = dfcd*db*dx*(dh0-dx/2) + dfsd1*dAs1*(dh0-das1);
    }
    return dMny;
}

//盖梁相连柱Ms最大
double CDgnJTG_D62_04_Seismic::GetBentCapMs(CArray<UINT,UINT>& BentCapPierList)
{
    double dMs = 0.0, dMn = 0.0;  
    for(int m=0; m<BentCapPierList.GetSize(); m++){
        UINT PierMember = BentCapPierList.GetAt(m);
        //取得构件所有荷载组合的内力-0:顶部；1:底部
        CArray <_DGN_FORC_CRC,_DGN_FORC_CRC> aForceList;
        if(!m_DataCtrl.GetEndForcefromPierMember(PierMember, aForceList)) continue;

        for(int i=0; i<aForceList.GetSize(); i++)
        {
            //顶部
            _DGN_FORC_CRC force = aForceList.GetAt(i);
            double dP = force.dFxx[0];//top
            if(m_DataCtrl.GetMfromPierPM(PierMember,TRUE,FALSE,dP,dMn)) //取顶部横桥向值
            {
                dMs = max(dMs,dMn);    
            }
        }//endof i (forceList)
    }//endof m (PierMember)
    return dMs;
}
double CDgnJTG_D62_04_Seismic::GetBentCapMs_CJJ(BOOL bMax ,CArray<UINT,UINT>& BentCapPierList)
{
    double dMs = 0.0, dMn = 0.0;  

    for(int m=0; m<BentCapPierList.GetSize(); m++){
        UINT PierMember = BentCapPierList.GetAt(m);
        //取得构件所有荷载组合的内力-0:顶部；1:底部
        CArray <_DGN_FORC_CRC,_DGN_FORC_CRC> aForceList;
        if(!m_DataCtrl.GetEndForcefromPierMember(PierMember, aForceList)) continue;
        T_RSDL_D RsdlD;
	  if(!m_pDoc->m_pAttrCtrl->GetRsdl(PierMember, RsdlD)) return FALSE;
        
        double dP = bMax ? RsdlD.dPireMaxMinForce[2] : RsdlD.dPireMaxMinForce[3];//top
        if(!RsdlD.bPierDColumnBent || fabs(dP)<1.0e-6)
        {
            for(int i=0; i<aForceList.GetSize(); i++)
            {
                //顶部
                _DGN_FORC_CRC force = aForceList.GetAt(i);
                dP = force.dFxx[0];//top
                if(m_DataCtrl.GetMfromPierPM(PierMember,TRUE,FALSE,dP,dMn)) //取顶部横桥向值
                {
                    dMs = max(dMs,dMn);    
                }
            }//endof i (forceList)
            
        }
        else
        {
            if(m_DataCtrl.GetMfromPierPM(PierMember,TRUE,FALSE,dP,dMn)) //取顶部横桥向值
            {
                dMs = max(dMs,dMn);    
            }
        }
     
    }//endof m (PierMember)
    return dMs;
}
/*
//Get Phi-y Phi-u of a Pier section (圆形、矩形) B.0.1~2
BOOL CDgnJTG_D62_04_Seismic::Get_PhiyPhiuOfPier(int ElemK,int ij, double& dPhiy, double& dPhiu)
{
    //0.get section
    _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
    if (!m_DataCtrl.Get_ColumnSection(ElemK,ij,sectioni)) return FALSE;    

    //1.get design material of checking ELEM
	_DGN_RC_MATL_CH matl;		matl.Initialize();
    if (!m_DataCtrl.Get_Matl(ElemK,matl)) return FALSE;
    double dfkh = matl.dfsky;
    double dfc = matl.dfcuk;
    double dfcc = 1.25* matl.dfcuk;
        
    //2.getεy
    if(matl.dEs < m_dZero) return FALSE;
    double dEpsilony = matl.dfsk/matl.dEs;//主筋

    //3.get Stirrup
    double dAx=.0, dAy=.0, dDx=.0, dDy=.0;
    if (!m_DataCtrl.Get_SeismicStirrup(ElemK, dAx, dAy, dDx, dDy)) return FALSE;

    //4.Get Phiy
    double dAg = 0;
    double dRho_s = 0.0; 
    if(sectioni.dr > m_dZero)
    {
        dPhiy = 2.213* dEpsilony/2/sectioni.dr;
        dAg = 3.14159*sectioni.dr*sectioni.dr - sectioni.dAs -sectioni.dAs1;
        if(sectioni.dPith < m_dZero) dRho_s=1;
        else dRho_s = (dAx*(2*sectioni.dr-2*dDx)+dAy*(2*sectioni.dr-2*dDy))/(3.14159*sectioni.dr*sectioni.dr*sectioni.dPith);
    }
    else if(sectioni.db>m_dZero && sectioni.dh>m_dZero)
    {
        dPhiy = 1.957/sectioni.dh;
        dAg = sectioni.db*sectioni.dh - sectioni.dAs -sectioni.dAs1;
        if(sectioni.dPith < m_dZero) dRho_s=1;
        else dRho_s = (dAx*(sectioni.dh-2*dDx)+dAy*(sectioni.db-2*dDy))/(sectioni.db*sectioni.dh*sectioni.dPith);
    }else return FALSE;
    double dEpsilon_cu = 0.004+1.4*dRho_s*dfkh*0.09/dfcc;

    //5.Get P
    _DGN_FORC_CRC aForce[12];
    if(!m_DataCtrl.Get_SeismicForce(ElemK,FALSE,aForce)) return FALSE;//获得荷载组合内力
    //6.Get Phiu
    for(int i=0;i<12;i++)
    {
        double dP = aForce[i].dFxx[ij];    
        double dPfA = dP/dfc/dAg;
        double dPhiu1=.0, dPhiu2=.0;
        if(sectioni.dr > m_dZero)
        {
            dPhiu1 = ((2826+6.850*dEpsilon_cu)-(8575+18.638*dEpsilon_cu)*dPfA)/2/sectioni.dr;
            dPhiu2 = ((1635+1.179*0.09)+(28.739*0.09*0.09+0.656*0.09+0.010)*dPfA)/2/sectioni.dr;      
        }
        else
        {      
            dPhiu1 = ((4999+11.825*dEpsilon_cu)-(7004+44.486*dEpsilon_cu)*dPfA)/sectioni.dh;
            dPhiu2 = ((5387+1.097*0.09)+(37.722*0.09*0.09+0.039*0.09+0.015)*dPfA)/sectioni.dh;
        }
        if(i ==0){ dPhiu = min(dPhiu1,dPhiu2);}
        dPhiu = min(dPhiu, min(dPhiu1,dPhiu2));
    }
    return TRUE;
}
*/
//求theta-u 最不利时，对应的截面、Phi-y、Phi-u等信息
BOOL CDgnJTG_D62_04_Seismic::GetThetauData(CArray<UINT, UINT>& ElemKList, double dH, double&db, double&dh, double&dr,
                                                                                     double&dfkh, double&dfc, double&dfy, double&dDs, double&dPhiy, double&dPhiu)
{ 
    double Lp =0.0, dThetau=.0;
    for(int k=0; k<ElemKList.GetSize();k++)//1
    {
        int kElemK = ElemKList.GetAt(k);
        //1.get design material of checking ELEM
        _DGN_RC_MATL_CH matl;		matl.Initialize();
        if (!m_DataCtrl.Get_Matl(kElemK,matl)) continue;
        //Get Phi-y,Phi-u.
        if(k == 0)
        {
            _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
            if (!m_DataCtrl.Get_ColumnSection(kElemK,0,sectioni)) continue;    
            double b = sectioni.dr>m_dZero ? 2*sectioni.dr : sectioni.db;
            Lp = min(max(0.08*dH+0.022*dfy*dDs,0.044*dfy*dDs),2*b/3.0);
            dThetau = Lp*(dPhiu - dPhiy)/2.0;
        }
        _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();
        if (!m_DataCtrl.Get_ColumnSection(kElemK,1,sectioni)) continue; 
        double b = sectioni.dr>m_dZero ? 2*sectioni.dr : sectioni.db;      
        Lp = min(max(0.08*dH+0.022*dfy*dDs,0.044*dfy*dDs),2*b/3.0);    
        if(Lp*(dPhiu - dPhiy)/2.0 - dThetau < m_dZero){
            dThetau = Lp*(dPhiu - dPhiy)/2.0;
            dfkh=matl.dfsky;          dfc= matl.dfcuk; 
            dfy=matl.dfsk; 
            dDs=sectioni.dd;          db = sectioni.db;
            dh = sectioni.dh;         dr = sectioni.dr;
        }
    }
    return TRUE;
}
