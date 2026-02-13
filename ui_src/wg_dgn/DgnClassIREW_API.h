#ifndef __DGN_IDEA_RS_EXPORT_API_WRITE_H__
#define __DGN_IDEA_RS_EXPORT_API_WRITE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef CArray<UINT, UINT> KEYArray;

class CDBDoc;
struct T_NODE_D;
struct T_ELEM_D;
struct T_MEMB_D;
struct T_MATL_ANALYSIS;
struct T_MATL_DESIGN;
struct T_MATL_DESIGN;
struct T_MATL_REBAR;
struct T_MATD_D;
struct T_MATL_D;
struct T_GSEC_VERTEX;
struct T_GSEC_POLYGON;
struct T_SECT_STIFFNESS;
struct T_GSEC_PART;
struct T_SECT_SECTBASE_D;
struct T_SECT_SECTION_D;
struct T_SECT_D;
struct T_CONS_D;
struct T_STLD_D;
struct T_LCOM_BASE;
struct T_LCOM_D;
struct T_LENG_D;
struct T_KFAC_D;
struct T_LTSR_D;
struct T_CMFT_D;
struct T_FMAG_D;
struct T_DCON_D;
struct T_RDGN_BEAM;
struct T_RDGN_COLM;
struct T_RCHK_BEAM;
struct T_RCHK_COLM;
struct T_REBT_MABR;
struct T_REBT_SHBR;
struct T_REBT_D;
struct T_POINT_SSM;
struct T_RPSC_MBAR_SSM;
struct T_RPSC_SBAR_SSM;
struct T_RPSC_SSM_D;
struct T_GSEC_TNDN;
struct T_REAC_D;
struct T_DISP_D;
struct T_STRT_D;
struct T_STRB_D;
struct T_SBCF_D;
struct T_STAG_D;
struct T_CSCS_D;
struct T_GRUP_D;
struct T_BNGR_D;
struct T_LDGR_D;
struct T_THIK_D;

#include "DGN_IDEA_ST.H"
#include "DgnClassIRED.h"
#include "..\wg_db\SectUtil.h"

#include "..\wg_base\TestEnvMgr.h"

#include "HeaderPre.h"
typedef enum { vt_int=0, vt_uint, vt_double, vt_str } vtype;
class __MY_EXT_CLASS__ CDgnIDEAArchiveAPI : public CArchive
{
public:
	// Flag values
	CDgnIDEAArchiveAPI(LPCTSTR lpszTxtFile, CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL);
	virtual ~CDgnIDEAArchiveAPI();
	
	// insertion operations
	CDgnIDEAArchiveAPI& operator<<(BYTE by);
	CDgnIDEAArchiveAPI& operator<<(WORD w);
	CDgnIDEAArchiveAPI& operator<<(LONG l);
	CDgnIDEAArchiveAPI& operator<<(DWORD dw);
	CDgnIDEAArchiveAPI& operator<<(float f);
	CDgnIDEAArchiveAPI& operator<<(double d);
	
	CDgnIDEAArchiveAPI& operator<<(int i);
	CDgnIDEAArchiveAPI& operator<<(short w);
	CDgnIDEAArchiveAPI& operator<<(TCHAR ch);
	CDgnIDEAArchiveAPI& operator<<(unsigned u);
	CDgnIDEAArchiveAPI& operator<<(const CString& string);

public:
	void WriteText(LPCTSTR lpszFmt, ...);
	void Write(const void* pData, UINT nSize, vtype type);
	void indent(int nCount) { for (int i = 0; i < nCount; i++) WriteText(_T(" ")); }
	void endl() { WriteText(_T("\n")); }
protected:
	CString GetDoubleStr(double dVal);
	CString m_csTextFile;
	FILE*   m_pTextFile;
};

class __MY_EXT_CLASS__ CDgnClassIREWAPI
{
	// Construction
public:
	CDgnClassIREWAPI();
	~CDgnClassIREWAPI();

	void Initialize();
	BOOL ExportIDEAData(CDgnIDEAArchiveAPI& ar, KEYArray& aElement, CString& szMCTPath);
	BOOL ExportIDEADataTest(CDgnIDEAArchiveAPI& ar, CString& szMCTPath, CDgnClassIRED& rData);
	BOOL GenerateData(KEYArray& aElement);
	void MakeArrayData(CDBDoc* pDoc, KEYArray& aElement);

protected:
	// Modeling
	KEYArray  m_aElement;
	KEYArray  m_aNode;
	KEYArray  m_aMember;
	KEYArray  m_aMaterial;
	KEYArray  m_aMaterialST;
	KEYArray  m_aMaterialCN;
	KEYArray  m_aMaterialRB;
	KEYArray  m_aMaterialAD;
	KEYArray  m_aSection;
	KEYArray  m_aElemTapered;
	KEYArray  m_aElemCSCS;  // CSCS가지고 있는 요소 Key
	KEYArray  m_aElemCSCSK; // 요소와 매칭되는 CSCS Key
	KEYArray  m_aSectionCS;
	KEYArray  m_aBoundary;
	KEYArray  m_aStaticLC;  
	KEYArray  m_aResponLC;  
	KEYArray  m_aEccentLC;  
	KEYArray  m_aTmHistLC;
	KEYArray  m_aMovingLC;  
	KEYArray  m_aSettleLC;  
	KEYArray  m_aLoadCombi;
	KEYArray  m_aLoadType;
	KEYArray  m_aTendon;  
	KEYArray  m_aTendonLoss;  

	// for 2D Element
	KEYArray  m_aThickness;
	// for construction Stage
	KEYArray  m_aCSCaseLC;
	KEYArray  m_aCSCaseAT;
	KEYArray  m_aStage;
	KEYArray  m_aComSecCS;
	KEYArray  m_aStrtGrp;
	KEYArray  m_aBndrGrp;
	KEYArray  m_aLoadGrp;  

	// Design
	KEYArray  m_aDgnLeng;
	KEYArray  m_aDgnKfac;
	KEYArray  m_aDgnLtsr;
	KEYArray  m_aDgnCmft;
	KEYArray  m_aDgnFmag;
	// HashTable Size
	// 결과는 사이즈가 꽤 있으므로 한번 돌면서 세어본다.
	int       m_nTruss;
	int       m_nBeam;
	// for 2D Element
	int       m_n2DElem;
	
	// Convert Data
	void CD2ID_NODE(T_NODE_D&           cNode, S_NODE_D&          iNode);
	void CD2ID_ELEM(T_ELEM_D&           cElem, S_ELEM_D&          iElem);
	void CD2ID_MEMB(T_MEMB_D&           cMemb, S_MEMB_D&          iMemb);
	void CD2ID_MTAN(T_MATL_ANALYSIS&    cMatl, S_MATL_PROP&       iMatl);
	void CD2ID_MTSD(T_MATL_DESIGN&      cMtst, S_MATL_STEEL_D&    iMtst);
	void CD2ID_MTST(T_MATD_D&           cMatd, S_MATL_STEEL_D&    iMtst);  
	void CD2ID_MTCC(T_MATD_D&           cMtcc, S_MATL_CONCRETE_D& iMtcc);
	void CD2ID_MTRB(T_MATL_REBAR&       cMtrb, S_MATL_REBAR&      iMtrb);
	void CD2ID_MTRI(T_MATD_D&           cMtri, S_MATL_REBAR_INFO& iMtri);
	void CD2ID_MTAD(T_MATL_D&           cMtad, S_MATL_ADIF_D&     iMtad);
	void CD2ID_SGVT(T_GSEC_VERTEX&      cSgvt, S_GSEC_VERTEX&     iSgvt);
	void CD2ID_SGPG(T_GSEC_POLYGON&     cSgpg, S_GSEC_POLYGON&    iSgpg);
	void CD2ID_SRSF(T_SECT_STIFFNESS&   cStsf, S_SECT_STIFFNESS&  iStsf);
	void CD2ID_SGPT(T_SECT_SECTBASE_D&  cStbs, T_GSEC_PART&       cPart, S_GSEC_PART&       iSgpt);
	void CD2ID_LINE(const T_SECT_SECTION_D& cStsn, S_SECT_D& iSect);
	void CD2ID_LNLP(const T_SECT_SECTION_D& cStsn, S_SECT_D& iSect);
	void CD2ID_STBS(int                 nType,T_SECT_SECTBASE_D&  cStbs, S_SECT_SECTBASE_D& iStbs);
	void IDDATA_POLYGON(S_SECT_SECTBASE_D&  rSect, DGN_GSEC_POLYGON_LIST&  arOut1, DGN_GSEC_POLYGON_LIST& arOut2, DGN_GSEC_POLYGON_LIST& arIn1, DGN_GSEC_POLYGON_LIST& arIn2);
	void CD2ID_SECT(CDBDoc*             pDoc,  T_SECT_D&           cSectD,S_SECT_D& iSectD);
	void CD2ID_CONS(T_CONS_D&           cCons, S_CONS_D&          iCons);
	void CD2ID_STLD(T_STLD_D&           cStld, S_STLD_D&          iStld);
	void CD2ID_LCMB(CDBDoc*             pDoc,  T_LCOM_BASE&        cLcmb, S_LCOM_BASE&       iLcmb);
	void CD2ID_LCOM(CDBDoc*             pDoc,  T_LCOM_D&           cLcom, S_LCOM_D&          iLcom);
	void CD2ID_LENG(T_LENG_D&           cLeng, S_LENG_D&          iLeng);
	void CD2ID_KFAC(T_KFAC_D&           cKfac, S_KFAC_D&          iKfac);
	void CD2ID_LTSR(T_LTSR_D&           cLtsr, S_LTSR_D&          iLtsr);
	void CD2ID_CMFT(T_CMFT_D&           cCmft, S_CMFT_D&          iCmft);
	void CD2ID_FMAG(T_FMAG_D&           cFmag, S_FMAG_D&          iFmag);
	void CD2ID_DCON(T_DCON_D&           cDcon, S_DCON_D&          iDcon);
	void CD2ID_RDBM(T_RDGN_BEAM&        cRdbm, S_RDGN_BEAM&       iRdbm);
	void CD2ID_RDCN(T_RDGN_COLM&        cRdcn, S_RDGN_COLM&       iRdcn);
	void CD2ID_RCBM(T_RCHK_BEAM&        cRcbm, S_RCHK_BEAM&       iRcbm);
	void CD2ID_RCCN(T_RCHK_COLM&        cRccn, S_RCHK_COLM&       iRccn);

	void CD2ID_RBMB(T_REBT_MABR&        cRbmb, S_REBT_MABR&       iRbmb);
	void CD2ID_RBSB(T_REBT_SHBR&        cRbsb, S_REBT_SHBR&       iRbsb);
	void CD2ID_REBT(T_REBT_D&           cRebt, S_REBT_D&          iRebt);
	void CD2ID_PTSM(T_POINT_SSM&        cPtsm, S_POINT_SSM&       iPtsm);
	void CD2ID_RPMS(T_RPSC_MBAR_SSM&    cRpms, S_RPSC_MBAR_SSM&   iRpms);
	void CD2ID_RPSS(T_RPSC_SBAR_SSM&    cRpss, S_RPSC_SBAR_SSM&   iRpss);
	void CD2ID_RPSD(T_RPSC_SSM_D&       cRpsd, S_RPSC_SSM_D&      iRpsd);
	void CD2ID_TNDN(T_GSEC_TNDN&        cTnsn, S_SECG_TNDN&       iTnsn, CString& szTName, UINT nKey);
	BOOL MakeTdlsData(CDBDoc* pDoc, T_KEY& nTndn, T_KEY& nStag, T_KEY& nStep, S_TDLS_D& TdlsD);

	// for 2D Element
	void CD2ID_THIK(T_THIK_D&           cThik, S_THIK_D&          iThik);
	void CD2ID_STRP(T_STRP_DL&          cStrp, S_STRP_DL&         iStrp);
	void CD2ID_STPN(T_STPN_D&           cStrp, S_STRP_DL&         iStrp);  // Plane Strain
	void CD2ID_STRM(T_STRM_DL&          cStrp, S_STRP_DL&         iStrp);  // Plane Stress
	// for construction Stage
	void CD2ID_CSLC(T_STLD_D&           cStld, S_STLD_D&          iCslc);
	void CD2ID_CLAT(T_STLD_D&           cStld, S_STLD_D&          iClat);
	void CD2ID_STAG(T_STAG_D&           cStag, S_STAG_D&          iStag);
	void CD2ID_CSCS(T_CSCS_D&           cCscs, S_CSCS_D&          iCscs);
	void CD2ID_GRUP(T_GRUP_D&           cGrup, S_GRUP_D&          iGrup);
	void CD2ID_BNGR(T_BNGR_D&           cBngr, S_BNGR_D&          iBngr);
	void CD2ID_LDGR(T_LDGR_D&           cLdgr, S_LDGR_D&          iLdgr);
	
	void CD2ID_PREA(T_REAC_D&           cReac, S_REAC_D&          iReac);
	void CD2ID_PDIS(T_DISP_D&           cDisp, S_DISP_D&          iDisp);
	void CD2ID_PTFO(T_STRT_D&           cTfor, S_TFOR_D&          iTfor);
	void CD2ID_PBFO(T_STRB_D&           cPbfo, S_BFOR_D&          iPbfo);
	void CD2ID_PREC(T_REAC_D&           cRecx, T_REAC_D&          cRecn,  S_RECC_D& iReac);
	void CD2ID_PDIC(T_DISP_D&           cDspx, T_DISP_D&          cDspn,  S_DSPC_D& iReac);
	void CD2ID_PBCF(T_SBCF_D&           cPbcfx,T_SBCF_D&          cPbcfn, S_BCFO_B& iPbcf);

	// for Composite user Section
	void CD2ID_CPUR(T_SECT_D&           cSectD,S_SECT_D&          iSectD);

	// Write Data
	BOOL WriteBASC(CDgnIDEAArchiveAPI&  ar, CString& szMCTPath);
	BOOL WriteUNIT(CDgnIDEAArchiveAPI&  ar, S_UNIT_D&  UnitD);
	BOOL WriteNODE(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_NODE_D& NodeD);
	BOOL WriteELEM(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_ELEM_D& ElemD);
	BOOL WriteMEMB(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_MEMB_D& MembD);
	BOOL WriteMtPr(CDgnIDEAArchiveAPI&  ar, S_MATL_PROP& MtPrD);
	BOOL WriteMTST(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_MATL_STEEL_D&    MtstD);
	BOOL WriteMTCC(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_MATL_CONCRETE_D& MtccD);
	BOOL WriteMTRB(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_MATL_REBAR_INFO& MtrbD);
	BOOL WriteMTAD(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_MATL_ADIF_D&     MtadD);

	BOOL WriteSTSF(CDgnIDEAArchiveAPI&  ar, S_SECT_STIFFNESS& StsfD);
	BOOL WriteSTPG(CDgnIDEAArchiveAPI&  ar, S_GSEC_POLYGON& StpgD);
	BOOL WriteSECT(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_SECT_D& SectD);
	BOOL WriteELST(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nSKey, S_ELST_D& ElstD);
	BOOL WriteCSET(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nSKey, UINT nPKey, S_CSET_D& CsetD);
	BOOL WriteSTCS(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_PROP_D& StcsD);
	BOOL WriteGSPO(CDgnIDEAArchiveAPI&  ar, S_GSPO_D& GspoD);
	BOOL WriteGSPI(CDgnIDEAArchiveAPI&  ar, S_GSPI_D& GspiD);
	BOOL WriteCSPT(CDgnIDEAArchiveAPI&  ar, UINT nSkey, UINT nPkey, UINT nIJ, S_GSEC_PART& CsptD, BOOL bPrintKey);
	BOOL WriteLINE(CDgnIDEAArchiveAPI&  ar, const CArray<S_GSEC_LINE, S_GSEC_LINE&>& aLine);
	BOOL WriteLNLP(CDgnIDEAArchiveAPI&  ar, const CArray<int, int>& aLineLoop);

	BOOL WriteCONS(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_CONS_D& ConsD);
	BOOL WriteSTLD(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_STLD_D& StldD);
	BOOL WriteLCBS(CDgnIDEAArchiveAPI&  ar, S_LCOM_BASE& LcomD);
	BOOL WriteLCOM(CDgnIDEAArchiveAPI&  ar, UINT nType, UINT nKey, S_LCOM_D& LcomD);

	BOOL WriteLENG(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_LENG_D& LengD);
	BOOL WriteKFAC(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_KFAC_D& KfacD);
	BOOL WriteLTSR(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_LTSR_D& LtsrD);
	BOOL WriteCMFT(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_CMFT_D& CmftD);
	BOOL WriteFMAG(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_FMAG_D& FmagD);

	BOOL WriteDCON(CDgnIDEAArchiveAPI&  ar, S_DCON_D& DConD);
	BOOL WriteRDBM(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_RDGN_BEAM& RdbmD);
	BOOL WriteRDCN(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_RDGN_COLM& RbcnD);
	BOOL WriteRCBM(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_RCHK_BEAM& RcbmD);
	BOOL WriteRCCN(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_RCHK_COLM& RccnD);

	BOOL WriteREBT(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_REBT_D&     RebtD);
	BOOL WriteRPMS(CDgnIDEAArchiveAPI&  ar, S_RPSC_MBAR_SSM& RpmsD);
	BOOL WriteRPSS(CDgnIDEAArchiveAPI&  ar, S_RPSC_SBAR_SSM& RpssD);
	BOOL WriteRPSC(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_RPSC_SSM_D& RpscD);
	BOOL WriteTDIF(CDgnIDEAArchiveAPI&  ar, UINT nKey, UINT nIJ, S_TDIF_D&     TdifD);  

	// for 2D Element
	BOOL WriteTHIK(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_THIK_D& iThik);
	BOOL WriteSTRP(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_STRP_DL& iStrp);
	BOOL WriteSTRPCS(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_STRP_CSDL& iStrp);

	// for construction Stage
	BOOL WriteCSLC(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_STLD_D& iCslc);
	BOOL WriteCLAT(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_STLD_D& iClat);  
	BOOL WriteSTAG(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_STAG_D& iStag);
	BOOL WriteCSCS(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_CSCS_D& iCscs);
	BOOL WriteGRUP(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_GRUP_D& iGrup);
	BOOL WriteBNGR(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_BNGR_D& iBngr);
	BOOL WriteLDGR(CDgnIDEAArchiveAPI&  ar, UINT nKey, S_LDGR_D& iLdgr);
	BOOL WriteTDLS(CDgnIDEAArchiveAPI&  ar, UINT nKey, UINT nStag, UINT nStep, S_TDLS_D& TdlsD);

	BOOL WriteREAC(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_REAC_D& ReacD);
	BOOL WriteDISP(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_DISP_D& DispD);
	BOOL WriteTFOR(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_TFOR_D& TforD);
	BOOL WriteBFOR(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_D& BforD);
	BOOL WriteRECC(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_RECC_D& ReccD);
	BOOL WriteDSPC(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_DSPC_D& DspcD);
	BOOL WriteBCFO(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BCFO_B& BcfoD);

	BOOL WriteDISPCS(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_DISP_CSD& DispD);
	BOOL WriteTFORCS(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_TFOR_CSD& TforD);
	BOOL WriteBFORCS(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_CSD& BforD);

	BOOL WriteBFORPT(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_PT& BforD);
	BOOL WriteBFORPTCC(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_PTCC& BforD);
	BOOL WriteBFORPTCS(CDgnIDEAArchiveAPI&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_PTCS& BforD);

	// Result 는 전체 절점과 전체 요소의 * 하중조건으로 조합되므로 따로 저장할 필요까진 없어보인다. 
	void MakePSCOutLine(CDBDoc* pDoc, double SectionSize[D_SECT_SIZE_NUM_MAX], T_SECT_SECTION_D& CSectD, S_GSEC_POLYGON_LIST& aOuter, S_GSEC_POLYGON_LIST& aInner, BOOL bPosI);
	void MakePSCOutLineCSCS(CDBDoc* pDoc, double SectionSize[D_SECT_SIZE_NUM_MAX], T_SECT_D& CSectD, T_CSCS_D&  CCscsD, S_GSEC_POLYGON_LIST& aOuter, BOOL bIncludeJ);
	void MakeCOMPOutLine(CDBDoc* pDoc, T_SECT_D& CSectD, S_GSEC_POLYGON_LIST& aOuter, S_GSEC_POLYGON_LIST& aInner, BOOL bTap, BOOL bPosI);
	void MakeCOMPOutLineCSCS(CDBDoc* pDoc, double SectionSize[D_SECT_SIZE_NUM_MAX], T_SECT_D& CSectD, T_CSCS_D&  CCscsD, S_GSEC_POLYGON_LIST& aOuter, S_GSEC_POLYGON_LIST& aInner, BOOL bTap, BOOL bIncludeJ);
	void MakeCOMPGeneralOutLineCSCS(T_SECT_D& CSectD, S_SECT_D& SSectD, T_CSCS_D&  CCscsD, BOOL bIncludeJ);
	void CopyCompGeneralOutLine(const S_SECT_D& SSectD, S_CSET_D& CsetD, int nCSetPart);
	BOOL IsExistData(UINT nKey, KEYArray& ar);
	void AddData(UINT nKey, KEYArray& ar);
	BOOL IsCompositeSection(int& nType, BOOL bGeneral);
	BOOL IsCompositeGeneralSection(int& nType);

	CDgnClassIRED  m_Data;
};
#include "HeaderPost.h"

#endif  // __DGN_IDEA_RS_EXPORT_WRITE_H__