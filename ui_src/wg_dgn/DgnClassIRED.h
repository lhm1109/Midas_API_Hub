#ifndef __DGN_IDEA_RS_EXPORT_DATA_H__
#define __DGN_IDEA_RS_EXPORT_DATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DGN_IDEA_ST.h"
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnClassIRED
{
// Construction
public:
	CDgnClassIRED();
	~CDgnClassIRED();

	void Initialize();
	void RemoveMap();
	void SetMapSize(int* nSize);
//   void SetMapSize(int nNode, int nElem, int nMemb, int nMatl, int nMtst, int nMtcn, int nMtrb, int nSect, int nElst, int nBndr, 
//                   int nStld, int nLcom, int nDLen, int nDKfc, int nDLts, int nDCmt, int nDFmg, int nRdbm, int nRdcn, int nRcbm, 
//                   int nRccn, int nRebt, int nRpsc, int nTdif, int nReac, int nDisp, int nRecc, int nDspc, int nTfor, int nBfor, int nBcfo);

	static int Compare_UINTAsc(const void *v1, const void *v2)
	{
		UINT u1 = *((UINT*)v1);
		UINT u2 = *((UINT*)v2);
		if (u1 > u2) return 1;
		else if (u1 < u2) return -1;
		return 0;
	}
	
	// MCT Path
	void SetVersion(int& nVer) { m_nVersion = nVer; }
	int  GetVersion() { return m_nVersion; }
	
	void  SetMCTPath(CString& szPath) { m_szMCTPath = szPath; }
	CString GetMCTPath() { return m_szMCTPath; }

	// About Unit
	void SetUnit(S_UNIT_D unit);  
	void GetUnit(S_UNIT_D&  unit);

	// About Node
	BOOL GetNode(S_NODE_K nKey, S_NODE_D&  NodeD);
	int  GetNode(S_NODE_K_LIST& aKey, S_NODE_D_LIST&  aNodeD);
	int  GetNodeKeyList(S_NODE_K_LIST& aKey);
	int  GetNodeCount();
	BOOL AddNode(S_NODE_K nKey, S_NODE_D&  NodeD);  

	// About element
	BOOL GetElem(S_ELEM_K nKey, S_ELEM_D&  ElemD);
	int  GetElem(S_ELEM_K_LIST& aKey, S_ELEM_D_LIST&  aElemD);
	int  GetElemKeyList(S_ELEM_K_LIST& aKey);
	int  GetElemCount();
	BOOL AddElem(S_ELEM_K nKey, S_ELEM_D&  ElemD);  

	// About Member
	BOOL GetMemb(S_MEMB_K nKey, S_MEMB_D&  MembD);
	int  GetMemb(S_MEMB_K_LIST& aKey, S_MEMB_D_LIST&  aMembD);
	int  GetMembKeyList(S_MEMB_K_LIST& aKey);
	int  GetMembCount();
	BOOL AddMemb(S_MEMB_K nKey, S_MEMB_D&  MembD);

	// About Material - Steel  
	BOOL GetMatlST(S_MATL_STEEL_K nKey, S_MATL_STEEL_D&  MatlSTD);
	int  GetMatlST(S_MATL_STEEL_K_LIST& aKey, S_MATL_STEEL_D_LIST&  aMatlSTD);
	int  GetMatlSTKeyList(S_MATL_STEEL_K_LIST& aKey);
	int  GetMatlSTCount();
	BOOL AddMatlST(S_MATL_STEEL_K nKey, S_MATL_STEEL_D&  MatlSTD);

	// About Material - Concrete  
	BOOL GetMatlCN(S_MATL_CONCRETE_K nKey, S_MATL_CONCRETE_D&  MatlCND);
	int  GetMatlCN(S_MATL_CONCRETE_K_LIST& aKey, S_MATL_CONCRETE_D_LIST&  aMatlCND);
	int  GetMatlCNKeyList(S_MATL_CONCRETE_K_LIST& aKey);
	int  GetMatlCNCount();
	BOOL AddMatlCN(S_MATL_CONCRETE_K nKey, S_MATL_CONCRETE_D&  MatlCND);
	
	// About Material - Rebar
	BOOL GetMatlRB(S_MATL_REIF_K nKey, S_MATL_REBAR_INFO&  MatlRBD);
	int  GetMatlRB(S_MATL_REIF_K_LIST& aKey, S_MATL_REIF_D_LIST&  aMatlRBD);
	int  GetMatlRBKeyList(S_MATL_REIF_K_LIST& aKey);
	int  GetMatlRBCount();
	BOOL AddMatlRB(S_MATL_REIF_K nKey, S_MATL_REBAR_INFO&  MatlRBD);
	
	// About Material - Add Inform
	BOOL GetMatlAD(S_MATL_ADIF_K nKey, S_MATL_ADIF_D&  MatlADD);
	int  GetMatlAD(S_MATL_ADIF_K_LIST& aKey, S_MATL_ADIF_D_LIST&  aMatlADD);
	int  GetMatlADKeyList(S_MATL_ADIF_K_LIST& aKey);
	int  GetMatlADCount();
	BOOL AddMatlAD(S_MATL_ADIF_K nKey, S_MATL_ADIF_D&  MatlADD);

	// About Section
	BOOL GetSect(S_SECT_K nKey, S_SECT_D&  SectD);
	int  GetSect(S_SECT_K_LIST& aKey, S_SECT_D_LIST&  aSectD);
	int  GetSectKeyList(S_SECT_K_LIST& aKey);
	int  GetSectCount();
	BOOL AddSect(S_SECT_K nKey, S_SECT_D&  SectD); 
	
	// About Section
	BOOL GetElst(S_ELEM_K ElemK, S_SECT_K SectK, S_ELST_D& ElstD);
	int  GetElst(S_ELEM_K_LIST& aElemK, S_SECT_K_LIST& aSectK, S_ELST_D_LIST&  aElstD);  
	int  GetElstCount();
	BOOL AddElst(S_ELEM_K ElemK, S_SECT_K SectK, S_ELST_D& ElstD); 
	
	// About Section
	BOOL GetCset(S_ELEM_K ElemK, S_SECT_K SectK, S_PART_K PartK, S_CSET_D& CsetD);
	int  GetCset(S_ELEM_K_LIST& aElemK, S_SECT_K_LIST& aSectK, S_PART_K_LIST& aPartK, S_CSET_D_LIST&  aCsetD);  
	int  GetCsetCount();
	BOOL AddCset(S_ELEM_K ElemK, S_SECT_K SectK, S_PART_K PartK, S_CSET_D& CsetD); 

	// About Support
	BOOL GetThik(S_THIK_K nKey, S_THIK_D&  ThikD);
	int  GetThik(S_THIK_K_LIST& aKey, S_THIK_D_LIST&  aThikD);
	int  GetThikKeyList(S_THIK_K_LIST& aKey);
	int  GetThikCount();
	BOOL AddThik(S_THIK_K nKey, S_THIK_D&  ThikD); 

	// About Support
	BOOL GetCons(S_CONS_K nKey, S_CONS_D&  ConsD);
	int  GetCons(S_CONS_K_LIST& aKey, S_CONS_D_LIST&  aConsD);
	int  GetConsKeyList(S_CONS_K_LIST& aKey);
	int  GetConsCount();
	BOOL AddCons(S_CONS_K nKey, S_CONS_D&  ConsD); 

	// About Load Case
	BOOL GetStld(S_STLD_K nKey, S_STLD_D&  StldD);
	int  GetStld(S_STLD_K_LIST& aKey, S_STLD_D_LIST&  aStldD);
	int  GetStldKeyList(S_STLD_K_LIST& aKey);
	int  GetStldCount();
	BOOL AddStld(S_STLD_K nKey, S_STLD_D&  StldD); 

	// About Load Combination
	BOOL GetLcom(S_LCOM_T nType, S_LCOM_K nKey, S_LCOM_D&  LcomD);
	int  GetLcom(S_LCOM_T_LIST& aType, S_LCOM_K_LIST& aKey, S_LCOM_D_LIST&  aLcomD);  
	int  GetLcomCount();
	BOOL AddLcom(S_LCOM_T nType, S_LCOM_K nKey, S_LCOM_D&  LcomD); 

	// About Unbraced Length(L,Lb)
	BOOL GetLeng(S_LENG_K nKey, S_LENG_D&  LengD);
	int  GetLeng(S_LENG_K_LIST& aKey, S_LENG_D_LIST&  aLengD);
	int  GetLengKeyList(S_LENG_K_LIST& aKey);
	int  GetLengCount();
	BOOL AddLeng(S_LENG_K nKey, S_LENG_D&  LengD); 

	// About Effective Length Factor(K)
	BOOL GetKfac(S_KFAC_K nKey, S_KFAC_D&  KfacD);
	int  GetKfac(S_KFAC_K_LIST& aKey, S_KFAC_D_LIST&  aKfacD);
	int  GetKfacKeyList(S_KFAC_K_LIST& aKey);
	int  GetKfacCount();
	BOOL AddKfac(S_KFAC_K nKey, S_KFAC_D&  KfacD); 

	// About Limiting Slenderness Ratio
	BOOL GetLtsr(S_LTSR_K nKey, S_LTSR_D&  LtsrD);
	int  GetLtsr(S_LTSR_K_LIST& aKey, S_LTSR_D_LIST&  aLtsrD);
	int  GetLtsrKeyList(S_LTSR_K_LIST& aKey);
	int  GetLtsrCount();
	BOOL AddLtsr(S_LTSR_K nKey, S_LTSR_D&  LtsrD); 

	// About Equivalent Moment Correction Factor(Cm)
	BOOL GetCmft(S_CMFT_K nKey, S_CMFT_D&  CmftD);
	int  GetCmft(S_CMFT_K_LIST& aKey, S_CMFT_D_LIST&  aCmftD);
	int  GetCmftKeyList(S_CMFT_K_LIST& aKey);
	int  GetCmftCount();
	BOOL AddCmft(S_CMFT_K nKey, S_CMFT_D&  CmftD); 

	// About Moment Magnifier
	BOOL GetFmag(S_FMAG_K nKey, S_FMAG_D&  FmagD);
	int  GetFmag(S_FMAG_K_LIST& aKey, S_FMAG_D_LIST&  aFmagD);
	int  GetFmagKeyList(S_FMAG_K_LIST& aKey);
	int  GetFmagCount();
	BOOL AddFmag(S_FMAG_K nKey, S_FMAG_D&  FmagD); 

	// About Con'c Design Code
	BOOL SetDcon(S_DCON_D   dcon);
	void GetDcon(S_DCON_D&  dcon);

	// Beam Section Data for Design
	BOOL GetRdbm(S_RDBM_K nKey, S_RDGN_BEAM&  RdbmD);
	int  GetRdbm(S_RDBM_K_LIST& aKey, S_RDBM_D_LIST&  aRdbmD);
	int  GetRdbmKeyList(S_RDBM_K_LIST& aKey);
	int  GetRdbmCount();
	BOOL AddRdbm(S_RDBM_K nKey, S_RDGN_BEAM&  RdbmD); 
 
	// Column Section Data for Design
	BOOL GetRdcn(S_RDCN_K nKey, S_RDGN_COLM&  RdcnD);
	int  GetRdcn(S_RDCN_K_LIST& aKey, S_RDCN_D_LIST&  aRdcnD);
	int  GetRdcnKeyList(S_RDCN_K_LIST& aKey);
	int  GetRdcnCount();
	BOOL AddRdcn(S_RDCN_K nKey, S_RDGN_COLM&  RdcnD); 
	
	// Beam Section Data for Checking
	BOOL GetRcbm(S_RCBM_K nKey, S_RCHK_BEAM&  RcbmD);
	int  GetRcbm(S_RCBM_K_LIST& aKey, S_RCBM_D_LIST&  aRcbmD);
	int  GetRcbmKeyList(S_RCBM_K_LIST& aKey);
	int  GetRcbmCount();
	BOOL AddRcbm(S_RCBM_K nKey, S_RCHK_BEAM&  RcbmD); 
	
	// Column Section Data for Checking
	BOOL GetRccn(S_RCCN_K nKey, S_RCHK_COLM&  RccnD);
	int  GetRccn(S_RCCN_K_LIST& aKey, S_RCCN_D_LIST&  aRccnD);
	int  GetRccnKeyList(S_RCCN_K_LIST& aKey);
	int  GetRccnCount();
	BOOL AddRccn(S_RCCN_K nKey, S_RCHK_COLM&  RccnD);   

	// Rebar Input for General Section
	BOOL GetRebt(S_REBT_K nKey, S_REBT_D&  RebtD);
	int  GetRebt(S_REBT_K_LIST& aKey, S_REBT_D_LIST&  aRebtD);
	int  GetRebtKeyList(S_REBT_K_LIST& aKey);
	int  GetRebtCount();
	BOOL AddRebt(S_REBT_K nKey, S_REBT_D&  RebtD);   

	// Rebar for PSC
	BOOL GetRpsc(S_RPSC_K nKey, S_RPSC_SSM_D&  RpscD);
	int  GetRpsc(S_RPSC_K_LIST& aKey, S_RPSC_D_LIST&  aRpscD);
	int  GetRpscKeyList(S_RPSC_K_LIST& aKey);
	int  GetRpscCount();
	BOOL AddRpsc(S_RPSC_K nKey, S_RPSC_SSM_D&  RpscD);   
	
	// Tendon of Cross section
	BOOL GetTdif(S_TDIF_K nKey, S_TDIJ_K nIJ, S_TDIF_D&  TdifD);
	int  GetTdif(S_TDIF_K_LIST& aKey, S_TDIJ_K_LIST& aIJ, S_TDIF_D_LIST&  aTdifD);  
	int  GetTdifCount();
	BOOL AddTdif(S_TDIF_K nKey, S_TDIJ_K nIJ, S_TDIF_D&  TdifD);

	// Tendon Time-Dependent Loss Graph
	BOOL GetTdls(S_TDPF_K nKey, S_STAG_K nStag, S_STEP_K nStep, S_TDLS_D& TdlsD);
	int  GetTdls(S_TDPF_K_LIST& aKey, S_STAG_K_LIST& aStag, S_STEP_K_LIST& aStep, S_TDLS_D_LIST&  aTdlsD);  
	int  GetTdlsCount();
	BOOL AddTdls(S_TDPF_K nKey, S_STAG_K nStag, S_STEP_K nStep, S_TDLS_D&  TdlsD);

	// for Construction Stage
	// About Construction Stage Load Case
	BOOL GetCslc(S_STLD_K nKey, S_STLD_D&  StldD);
	int  GetCslc(S_STLD_K_LIST& aKey, S_STLD_D_LIST&  aStldD);
	int  GetCslcKeyList(S_STLD_K_LIST& aKey);
	int  GetCslcCount();
	BOOL AddCslc(S_STLD_K nKey, S_STLD_D&  StldD); 

	// Auto Generate Construction Stage LoadCase
	BOOL GetClat(S_STLD_K nKey, S_STLD_D&  StldD);
	int  GetClat(S_STLD_K_LIST& aKey, S_STLD_D_LIST&  aStldD);
	int  GetClatKeyList(S_STLD_K_LIST& aKey);
	int  GetClatCount();
	BOOL AddClat(S_STLD_K nKey, S_STLD_D&  StldD); 

	// Construction Stage
	BOOL GetStag(S_STAG_K nKey, S_STAG_D&  StagD);
	int  GetStag(S_STAG_K_LIST& aKey, S_STAG_D_LIST&  aStagD);
	int  GetStagKeyList(S_STAG_K_LIST& aKey);
	int  GetStagCount();
	BOOL AddStag(S_STAG_K nKey, S_STAG_D&  StagD);   

	// Composite Section for Construction Stage
	BOOL GetCscs(S_CSCS_K nKey, S_CSCS_D&  CscsD);
	int  GetCscs(S_CSCS_K_LIST& aKey, S_CSCS_D_LIST&  aCscsD);
	int  GetCscsKeyList(S_CSCS_K_LIST& aKey);
	int  GetCscsCount();
	BOOL AddCscs(S_CSCS_K nKey, S_CSCS_D&  CscsD);   
	
	// Element Group
	BOOL GetGrup(S_GRUP_K nKey, S_GRUP_D&  EGrpD);
	int  GetGrup(S_GRUP_K_LIST& aKey, S_GRUP_D_LIST&  aEGrpD);
	int  GetGrupKeyList(S_GRUP_K_LIST& aKey);
	int  GetGrupCount();
	BOOL AddGrup(S_GRUP_K nKey, S_GRUP_D&  EGrpD);   
	
	// Boundary Group
	BOOL GetBngr(S_BNGR_K nKey, S_BNGR_D&  BGrpD);
	int  GetBngr(S_BNGR_K_LIST& aKey, S_BNGR_D_LIST&  aBGrpD);
	int  GetBngrKeyList(S_BNGR_K_LIST& aKey);
	int  GetBngrCount();
	BOOL AddBngr(S_BNGR_K nKey, S_BNGR_D&  BGrpD);   
	
	// Load Group
	BOOL GetLdgr(S_LDGR_K nKey, S_LDGR_D&  LGrpD);
	int  GetLdgr(S_LDGR_K_LIST& aKey, S_LDGR_D_LIST&  aLGrpD);
	int  GetLdgrKeyList(S_LDGR_K_LIST& aKey);
	int  GetLdgrCount();
	BOOL AddLdgr(S_LDGR_K nKey, S_LDGR_D&  LGrpD);   
	
	// Reaction
	BOOL  GetReac(S_NODE_K  nNodeK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_REAC_D& reac);
	int   GetReac(S_NODE_K_LIST& aNodeK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_REAC_D_LIST& aReacD);
	int   GetReacCount();
	BOOL  AddReac(S_NODE_K  nNodeK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_REAC_D& reac);
	// Displacement
	BOOL  GetDisp(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_DISP_D& disp);
	int   GetDisp(S_NODE_K_LIST& aNodeK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_DISP_D_LIST& aDispD);
	int   GetDispCount();
	BOOL  AddDisp(S_NODE_K  nNodeK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_DISP_D& disp);
	// Truss Force
	BOOL  GetTfor(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_TFOR_D& tfor);
	int   GetTfor(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_TFOR_D_LIST& aTForD);
	int   GetTforCount();
	BOOL  AddTfor(S_ELEM_K  nElemK, S_LDCS_K  nStldK, S_LDTP_K nType, S_TFOR_D& tfor);
	// Beam Force
	BOOL  GetBfor(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_D& bfor);
	int   GetBfor(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_BFOR_D_LIST& aBForD);
	int   GetBforCount();
	BOOL  AddBfor(S_ELEM_K  nElemK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_BFOR_D& bfor);  
	// Plate Force
	BOOL  GetStrp(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_STRP_DL& strp);
	int   GetStrp(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_STRP_DL_LIST& aBForD);
	int   GetStrpCount();
	BOOL  AddStrp(S_ELEM_K  nElemK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_STRP_DL& strp);  
	// Reaction - Concurrent
	BOOL  GetRecc(S_NODE_K  nNodeK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_RECC_D& reac);
	int   GetRecc(S_NODE_K_LIST& aNodeK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_RECC_D_LIST& aReacD);
	int   GetReccCount();
	BOOL  AddRecc(S_NODE_K  nNodeK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_RECC_D& reac);
	// Displacement - Concurrent
	BOOL  GetDspc(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_DSPC_D& dspc);
	int   GetDspc(S_NODE_K_LIST& aNodeK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_DSPC_D_LIST& aDispD);
	int   GetDspcCount();
	BOOL  AddDspc(S_NODE_K  nNodeK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_DSPC_D& dspc);
	// Beam Concurrent Force
	BOOL  GetBcfo(S_ELEM_K  nElemK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_BCFO_B& reac);
	int   GetBcfo(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_BCFO_B_LIST& aBcfoD);
	int   GetBcfoCount();
	BOOL  AddBcfo(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BCFO_B& reac);

	// for Construction Stage
	// Displacement
	BOOL  GetDispCS(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_DISP_CSD& diap);
	int   GetDispCS(S_NODE_K_LIST& aNodeK, S_STLD_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_DISP_CSD_LIST& aDispD);
	int   GetDispCountCS();
	BOOL  AddDispCS(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_DISP_CSD& diap);
	// Truss Force
	BOOL  GetTforCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_TFOR_CSD& tfor);
	int   GetTforCS(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_TFOR_CSD_LIST& aTForD);
	int   GetTforCountCS();
	BOOL  AddTforCS(S_ELEM_K  nElemK, S_STLD_K  nStldK, S_STAG_K nStag, S_TFOR_CSD& tfor);
	// Beam Force
	BOOL  GetBforCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_BFOR_CSD& bfor);
	int   GetBforCS(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_BFOR_CSD_LIST& aBForD);
	int   GetBforCountCS();
	BOOL  AddBforCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_BFOR_CSD& bfor);  
	// Plate Force
	BOOL  GetStrpCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_STRP_CSDL& pfor);
	int   GetStrpCS(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_STRP_CSDL_LIST& apForD);
	int   GetStrpCountCS();
	BOOL  AddStrpCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_STRP_CSDL& pfor);  

	// Beam Part Force 
	BOOL  GetBforPT(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_PT& bfor);
	int   GetBforPT(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_BFOR_PT_LIST& aBForD);
	int   GetBforPTCount();
	BOOL  AddBforPT(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_PT& bfor);  
	// Beam Part Force CC
	BOOL  GetBforPTCC(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_PTCC& bfor);
	int   GetBforPTCC(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_BFOR_PTCC_LIST& aBForD);
	int   GetBforPTCCCount();
	BOOL  AddBforPTCC(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_PTCC& bfor);  
	// Beam Part Force For CS
	BOOL  GetBforPTCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_BFOR_PTCS& bfor);
	int   GetBforPTCS(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_BFOR_PTCS_LIST& aBForD);
	int   GetBforPTCSCount();
	BOOL  AddBforPTCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_BFOR_PTCS& bfor);  


protected:
	// Common
	int       m_nVersion;
	S_UNIT_D  m_unit;
	CString   m_szMCTPath;
	// Geometry
	CMapIDEA<S_NODE_K, S_NODE_K, S_NODE_D, S_NODE_D&>  m_mNode;
	CMapIDEA<S_ELEM_K, S_ELEM_K, S_ELEM_D, S_ELEM_D&>  m_mElem;
	CMapIDEA<S_MEMB_K, S_MEMB_K, S_MEMB_D, S_MEMB_D&>  m_mMemb;
	// Material
	CMapIDEA<S_MATL_STEEL_K,    S_MATL_STEEL_K,     S_MATL_STEEL_D,     S_MATL_STEEL_D&>      m_mMatlST;
	CMapIDEA<S_MATL_CONCRETE_K, S_MATL_CONCRETE_K,  S_MATL_CONCRETE_D,  S_MATL_CONCRETE_D&>   m_mMatlCN;
	CMapIDEA<S_MATL_REIF_K,     S_MATL_REIF_K,      S_MATL_REBAR_INFO,  S_MATL_REBAR_INFO&>   m_mMatlRB;
	CMapIDEA<S_MATL_K,          S_MATL_K,           S_MATL_ADIF_D,      S_MATL_ADIF_D&>       m_mMatlAD;
	// Section
	CMapIDEA<S_SECT_K, S_SECT_K, S_SECT_D, S_SECT_D&> m_mSect;
	CMapIDEA<T_IDEA_K<UINT>, T_IDEA_K<UINT>, S_ELST_D, S_ELST_D&> m_mElst;
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_CSET_D, S_CSET_D&> m_mCset;
	// Boundary - Support
	CMapIDEA<S_CONS_K, S_CONS_K, S_CONS_D, S_CONS_D&> m_mCons;
	// Load
	CMapIDEA<S_STLD_K, S_STLD_K, S_STLD_D, S_STLD_D&> m_mStld;  // Load-Case 
	CMapIDEA<T_IDEA_K<UINT>, T_IDEA_K<UINT>, S_LCOM_D, S_LCOM_D&> m_mLcom;  // Load-Combination
	// Design Data
	CMapIDEA<S_LENG_K, S_LENG_K, S_LENG_D, S_LENG_D&> m_mLeng;  // Unbraced Length
	CMapIDEA<S_KFAC_K, S_KFAC_K, S_KFAC_D, S_KFAC_D&> m_mKfac;  // Effective Length Factor(K)
	CMapIDEA<S_LTSR_K, S_LTSR_K, S_LTSR_D, S_LTSR_D&> m_mLtsr;  // Limiting Slenderness Ratio
	CMapIDEA<S_CMFT_K, S_CMFT_K, S_CMFT_D, S_CMFT_D&> m_mCmft;  // Equivalent Moment Correction Factor(Cm)
	CMapIDEA<S_FMAG_K, S_FMAG_K, S_FMAG_D, S_FMAG_D&> m_mFmag;  // Moment Magnifier
	
	S_DCON_D  m_Dcon; // Con'c Design parameter
	CMapIDEA<S_RDBM_K, S_RDBM_K, S_RDGN_BEAM, S_RDGN_BEAM&> m_mRdbm;  // Beam Section Data for Design
	CMapIDEA<S_RDCN_K, S_RDCN_K, S_RDGN_COLM, S_RDGN_COLM&> m_mRdcn;  // Column Section Data for Design
	CMapIDEA<S_RCBM_K, S_RCBM_K, S_RCHK_BEAM, S_RCHK_BEAM&> m_mRcbm;  // Beam Section Data for Checking
	CMapIDEA<S_RCCN_K, S_RCCN_K, S_RCHK_COLM, S_RCHK_COLM&> m_mRccn;  // Column Section Data for Checking

	CMapIDEA<S_REBT_K, S_REBT_K, S_REBT_D, S_REBT_D&> m_mRebt;  // Rebar Input for General Section
	CMapIDEA<S_RPSC_K, S_RPSC_K, S_RPSC_SSM_D, S_RPSC_SSM_D&> m_mRpsc;  // Rebar for PSC
	CMapIDEA<T_IDEA_K<UINT>, T_IDEA_K<UINT>, S_TDIF_D, S_TDIF_D&> m_mTdif;  // Tendon of Cross section
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_TDLS_D, S_TDLS_D&> m_mTdls;  // Tendon Time-Dependent Loss Graph

	// for 2D Element
	CMapIDEA<S_THIK_K, S_THIK_K, S_THIK_D, S_THIK_D&> m_mThik;
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_STRP_DL, S_STRP_DL&> m_mSTRP;  // Reaction NodeK + LoadCaseK

	// for Construction Stage
	CMapIDEA<S_STLD_K, S_STLD_K, S_STLD_D, S_STLD_D&> m_mCslc;  // Construction Stage Load Case in Static Loadcase
	CMapIDEA<S_STLD_K, S_STLD_K, S_STLD_D, S_STLD_D&> m_mClat;  // Auto Generateed Construction Stage Load Case 
	CMapIDEA<S_STAG_K, S_STAG_K, S_STAG_D, S_STAG_D&> m_mStag;  // Construction Stage
	CMapIDEA<S_CSCS_K, S_CSCS_K, S_CSCS_D, S_CSCS_D&> m_mCscs;  // Composite Section for Construction Stage
	CMapIDEA<S_GRUP_K, S_GRUP_K, S_GRUP_D, S_GRUP_D&> m_mGrup;  // Element Group
	CMapIDEA<S_BNGR_K, S_BNGR_K, S_BNGR_D, S_BNGR_D&> m_mBngr;  // Boundary Group
	CMapIDEA<S_LDGR_K, S_LDGR_K, S_LDGR_D, S_LDGR_D&> m_mLdgr;  // Load Group
	
	// result
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_REAC_D, S_REAC_D&> m_mReac;  // Reaction NodeK + LoadCaseK
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_DISP_D, S_DISP_D&> m_mDisp;  // Reaction NodeK + LoadCaseK
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_TFOR_D, S_TFOR_D&> m_mTfor;  // Truss Force ElemK + LoadCaseK  
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_BFOR_D, S_BFOR_D&> m_mBfor;  // Beam Force ElemK + LoadCaseK      
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_RECC_D, S_RECC_D&> m_mRecc;  // Reaction NodeK + LoadCaseK
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_DSPC_D, S_DSPC_D&> m_mDspc;  // Reaction NodeK + LoadCaseK
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_BCFO_B, S_BCFO_B&> m_mBcfo;  // Beam concurrent Force ElemK + LoadCaseK
	
	// for Construction Stage
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_DISP_CSD,  S_DISP_CSD&>  m_mCsds;  // Displacement Construction Stage - NodeK + CaseK + StepK
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_TFOR_CSD,  S_TFOR_CSD&>  m_mCstf;  // Truss-Force Construction Stage  - ElemK + CaseK + StepK
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_BFOR_CSD,  S_BFOR_CSD&>  m_mCsbf;  // Beam-Force Construction Stage   - ElemK + CaseK + StepK
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_STRP_CSDL, S_STRP_CSDL&> m_mCspf;  // Plate-Force Construction Stage   - ElemK + CaseK + StepK

	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_BFOR_PT, S_BFOR_PT&> m_mBforPT;  // Beam Force ElemK + LoadCaseK      
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_BFOR_PTCC, S_BFOR_PTCC&> m_mBforPTCC;  // Beam Force ElemK + LoadCaseK      
	CMapIDEA<T_IDEA_K3<UINT>, T_IDEA_K3<UINT>, S_BFOR_PTCS, S_BFOR_PTCS&> m_mBforPTCS;  // Beam-Force Construction Stage   - ElemK + CaseK + StepK
};
#include "HeaderPost.h"
#endif  // __DGN_IDEA_RS_EXPORT_DATA_H__