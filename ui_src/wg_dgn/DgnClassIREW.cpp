// DgnClassIRED.cpp : implementation file
// IDEA RS EXPORT : Write Class

#include "stdafx.h"
#include "DgnClassIREW.h"


#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\DB_ST_DT_UNIT.h"
#include "..\wg_db\DB_ST_DT_NODE.h"
#include "..\wg_db\DB_ST_DT_ELEM.h"
#include "..\wg_db\DB_ST_DT_MATL.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\wg_db\DB_ST_DT_SECT.h"
#include "..\wg_db\Db_st_dt_thik.h"
#include "..\wg_db\DB_ST_DT_BNDR.h"
#include "..\wg_db\DB_ST_DT_CASE.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_SecShape.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_TendonInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CCM_TYPE_STEEL  _ULS(S)
#define CCM_TYPE_CONCR  _ULS(C)
#define CCM_TYPE_SRC    _ULS(SRC)
#define CCM_TYPE_USER   _ULS(U)

CDgnIDEAArchive::CDgnIDEAArchive(LPCTSTR lpszTxtFile, CFile* pFile, UINT nMode, int nBufSize, void* lpBuf)
: CArchive(pFile, nMode, nBufSize, lpBuf)
{
	m_csTextFile = lpszTxtFile;
	if (CTestEnvMgr::GetTestEnvST(_LSX(MIDAS)) == _LSX(3F9CF245_047E_36CF_13F0_2292A8FD))
	{
		m_pTextFile = _wfopen(lpszTxtFile, _T("w"));
	}
	else m_pTextFile = 0;
}

CDgnIDEAArchive::~CDgnIDEAArchive()
{
	if (m_pTextFile)
	{
		fclose(m_pTextFile);
	}
}

CString CDgnIDEAArchive::GetDoubleStr(double dVal)
{
	CString csVal;
	csVal.Format(_T("%.14g"), dVal);
	if (csVal.Find('.', 0) == -1) csVal += _T(".0");
	return csVal;
}

void CDgnIDEAArchive::WriteText(LPCTSTR lpszFmt, ...)
{
	if (m_pTextFile == 0) return;
	va_list args;
	va_start(args, lpszFmt);

	CString sMsg;
	sMsg.FormatV(lpszFmt,args);
	fwprintf(m_pTextFile, sMsg);

	va_end(args);  

	return;
}

void CDgnIDEAArchive::Write(const void* pData, UINT nSize, vtype type)
{
	int nCount;
	switch(type)
	{
	case vt_int:
		{
			const int* pVal = (const int*)pData;
			nCount = nSize / 4;
			for (int i = 0; i < nCount; i++)
				WriteText(_T(" %d"), pVal[i]);
		}
		break;
	case vt_uint:
		{
			const unsigned int* pVal = (const unsigned int*)pData;
			nCount = nSize / 4;
			for (int i = 0; i < nCount; i++)
				WriteText(_T(" %u"), pVal[i]);
		}
		break;
	case vt_double:
		{
			const double* pVal = (const double*)pData;
			nCount = nSize / 8;
			for (int i = 0; i < nCount; i++)
				WriteText(_T(" %s"), GetDoubleStr(pVal[i]));
		}
		break;
	case vt_str:
		{
			const TCHAR* pVal = (const TCHAR*)pData;
			CString csStr;
			csStr.GetBufferSetLength(nSize);
            int i = 0;
			for (i = 0; i < nSize; i++)
			{
				if (pVal[i] == 0) break;
				csStr.SetAt(i, pVal[i]);
			}
			csStr.ReleaseBuffer(i);
			WriteText(_T(" %s"), csStr);
		}
		break;
	default: ASSERT(0);
	}
	CArchive::Write(pData, nSize);
	return;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(BYTE by)
{
	WriteText(_T(" %u"), by);
	CArchive::operator<<(by);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(WORD w)
{
	WriteText(_T(" %u"), w);
	CArchive::operator<<(w);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(LONG l)
{
	WriteText(_T(" %ld"), l);
	CArchive::operator<<(l);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(DWORD dw)
{
	WriteText(_T(" %ld"), dw);
	CArchive::operator<<(dw);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(float f)
{
	WriteText(_T(" %s"), GetDoubleStr((double)f));
	CArchive::operator<<(f);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(double d)
{
	WriteText(_T(" %s"), GetDoubleStr(d));
	CArchive::operator<<(d);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(int i)
{
	WriteText(_T(" %d"), i);
	CArchive::operator<<(i);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(short w)
{
	WriteText(_T(" %d"), w);
	CArchive::operator<<(w);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(TCHAR ch)
{
	WriteText(_T(" %c"), ch);
	CArchive::operator<<(ch);
	return *this;
}

CDgnIDEAArchive& CDgnIDEAArchive::operator<<(unsigned u)
{
	WriteText(_T(" %u"), u);
	CArchive::operator<<(u);
	return *this;
}

// hslee : 6.0에서의 CString 출력법 그대로 퍼와서 살렸음
CDgnIDEAArchive& CDgnIDEAArchive::operator<<(const CString& string)  
{
	// special signature to recognize unicode strings
#ifdef _UNICODE
	(*this) << (BYTE)0xff;
	(*this) << (WORD)0xfffe;
#endif

	if (string.GetLength() < 255)
	{
		(*this) << (BYTE)string.GetLength();
	}
	else if (string.GetLength() < 0xfffe)
	{
		(*this) << (BYTE)0xff;
		(*this) << (WORD)string.GetLength();
	}
	else
	{
		(*this) << (BYTE)0xff;
		(*this) << (WORD)0xffff;
		(*this) << (DWORD)string.GetLength();
	}

// #ifdef _UNICODE
// 	// hslee (UnicodeTmpCode) : Unicode를 대비해서 넣어놓은 코드.. 테스트를 해보진 않았음
// 	// 
// 	// (설명)
// 	// export > IDEA file 을 실행 하면 여기가 실행됨
// 	// vs6.0에서 TCHAR형으로 파일에 써버리기 때문에 여기서 쓸때도 TCHAR로 컨버트 해서 써줘야 하는데..
// 	// 그 함수가 WideCharToMultiByte()임.. 그래서 아래에서 불렀음
// 	// test부탁함니다. 컴파일도 안해봤어요..
// 
// 	TCHAR  strChar[1000]   = _T("");
// 	// UNICODE -> MULTIBYTE CODE
// 	WideCharToMultiByte(CP_ACP, NULL, string.GetData(), -1, strChar,  1000, NULL, FALSE);
// 
// 	((CArchive*)this)->Write((const void*)(strChar), (UINT)(string.GetLength()*sizeof(CHAR))); // hslee : 유니코드로 프로그램 세팅을 변경하면 vs6.0에서 저장한 파일을 불러올 때 문제가 발생한다.
// #else
	((CArchive*)this)->Write((const void*)(string.GetString()), (UINT)(string.GetLength()*sizeof(TCHAR))); // hslee : 유니코드로 프로그램 세팅을 변경하면 vs6.0에서 저장한 파일을 불러올 때 문제가 발생한다.
//#endif

	

	return (*this);
}

CDgnClassIREW::CDgnClassIREW()
{
	Initialize(); 
}

CDgnClassIREW::~CDgnClassIREW()
{
	Initialize(); 
}

void CDgnClassIREW::Initialize()
{
	m_Data.Initialize();
	m_nTruss = m_nBeam = m_n2DElem = 0;

	m_aElement.RemoveAll();
	m_aNode.RemoveAll();
	m_aMember.RemoveAll();
	m_aMaterial.RemoveAll();
	m_aMaterialST.RemoveAll();
	m_aMaterialCN.RemoveAll();
	m_aMaterialRB.RemoveAll();
	m_aMaterialAD.RemoveAll();
	m_aSection.RemoveAll();
	m_aThickness.RemoveAll();
	m_aElemTapered.RemoveAll();
	m_aElemCSCS.RemoveAll();
	m_aElemCSCSK.RemoveAll();
	m_aSectionCS.RemoveAll();
	m_aBoundary.RemoveAll();
	m_aStaticLC.RemoveAll();
	m_aResponLC.RemoveAll();
	m_aEccentLC.RemoveAll();
	m_aTmHistLC.RemoveAll();
	m_aMovingLC.RemoveAll();
	m_aSettleLC.RemoveAll();
	m_aLoadCombi.RemoveAll();
	m_aLoadType.RemoveAll();
	m_aTendon.RemoveAll();
	m_aTendonLoss.RemoveAll();

	// for construction Stage
	m_aCSCaseLC.RemoveAll();
	m_aCSCaseAT.RemoveAll();
	m_aStage.RemoveAll();
	m_aComSecCS.RemoveAll();
	m_aStrtGrp.RemoveAll();
	m_aBndrGrp.RemoveAll();
	m_aLoadGrp.RemoveAll();  

	m_aDgnLeng.RemoveAll();
	m_aDgnKfac.RemoveAll();
	m_aDgnLtsr.RemoveAll();
	m_aDgnCmft.RemoveAll();
	m_aDgnFmag.RemoveAll();
}

BOOL CDgnClassIREW::IsExistData(UINT nKey, KEYArray& ar)
{
	int i(0), nSize = ar.GetSize();
	if (nSize < 1) return FALSE;

	for (i=0; i<nSize; i++)
	{
		if (ar[i] == nKey) break;
	}

	return (nSize != i);
}

void CDgnClassIREW::AddData(UINT nKey, KEYArray& ar)
{
	if (IsExistData(nKey, ar) == TRUE) return;
	ar.Add(nKey);
}

BOOL CDgnClassIREW::IsCompositeSection(int& nType, BOOL bGeneral)
{
	if (nType == D_SECT_TYPE_COMPO_B  || nType == D_SECT_TYPE_COMPO_I  || nType == D_SECT_TYPE_COMPO_CI ||
			nType == D_SECT_TYPE_COMPO_CT || nType == D_SECT_TYPE_COMPO_PC ) return TRUE;
	if (bGeneral == TRUE && nType == D_SECT_TYPE_COMPO_G) { return TRUE; }

	return FALSE;
}

BOOL CDgnClassIREW::IsCompositeGeneralSection(int& nType)
{
	return (nType == D_SECT_TYPE_COMPO_G);
}


void CDgnClassIREW::MakePSCOutLine(CDBDoc* pDoc, double SectionSize[D_SECT_SIZE_NUM_MAX], T_SECT_SECTION_D& CSectD, S_GSEC_POLYGON_LIST& aOuter, S_GSEC_POLYGON_LIST& aInner, BOOL bPosI)
{
	int nNumCell=0, nVSize(0), i(0);
	double FrameLocalVector[3][3]={1.,0.,0., 0.,1.,0., 0.,0.,1.};
	double xyz[3]={0.,0.,0.};
	CArray<T_SECT_POLYGON, T_SECT_POLYGON&>aPolygon1, aPolygon2;
	aOuter.RemoveAll();
	aInner.RemoveAll();  
	
	if (CSectD.Shape == D_SECT_SHAPE_PSC_VALUE)
	{
		T_SECT_SECTBASE_D* pSect = bPosI ? &CSectD.SectI : &CSectD.SectJ;

		// Value 단면은 그냥 복사해준다.
		int nOutsize = pSect->aOuterPolygon.GetSize();
		if (nOutsize != 1) return;    
		aOuter.SetSize(1);
		aOuter[0].aVertex.SetSize(pSect->aOuterPolygon[0].aVertex.GetSize());
		for (i=0; i<pSect->aOuterPolygon[0].aVertex.GetSize(); i++)
		{
			aOuter[0].aVertex[i].dX = pSect->aOuterPolygon[0].aVertex[i].dX;
			aOuter[0].aVertex[i].dY = pSect->aOuterPolygon[0].aVertex[i].dY;
		}
		
		int nInnerSize = pSect->aInnerPolygon.GetSize();
		aInner.SetSize(nInnerSize);
		for (i=0; i<nInnerSize; i++)
		{
			nVSize = pSect->aInnerPolygon[i].aVertex.GetSize();
			aInner[i].aVertex.SetSize(nVSize);

			for (int j=0; j<nVSize; j++)
			{        
				aInner[i].aVertex[j].dX = pSect->aInnerPolygon[i].aVertex[j].dX;
				aInner[i].aVertex[j].dY = pSect->aInnerPolygon[i].aVertex[j].dY;
			}
		}
		
		return;
	}

	if(CSectD.Shape == D_SECT_SHAPE_PSC_1CELL)nNumCell=1;
	if(CSectD.Shape == D_SECT_SHAPE_PSC_2CELL)nNumCell=2;
	pDoc->m_pSecShape->makePSCOutLine(SectionSize, CSectD, FrameLocalVector, xyz, nNumCell, aPolygon1, aPolygon2,TRUE);
	
	// Outer는 모두 1개임
	nVSize = aPolygon1.GetSize();
	if (nVSize > 0)
	{
		aOuter.SetSize(1);
		aOuter[0].aVertex.SetSize(nVSize);
		for (i=0; i<nVSize; i++)
		{
			aOuter[0].aVertex[i].dX = aPolygon1[i].dCoor[1];
			aOuter[0].aVertex[i].dY = aPolygon1[i].dCoor[2];
		}
	}
	// Inner 는 타입별로 달라짐.
	// 1. 2-Cell일 경우
	nVSize = aPolygon2.GetSize();
	if      (CSectD.Shape == D_SECT_SHAPE_PSC_2CELL)
	{        
		ASSERT(nVSize == 20); // 2Cell 의 Inner는 반드시 20개
		if (nVSize == 20)
		{
			aInner.SetSize(2);
			aInner[0].aVertex.SetSize(10);
			aInner[1].aVertex.SetSize(10);

			for (i=1; i<10; i++)
			{
				aInner[0].aVertex[i-1].dX = aPolygon2[i].dCoor[1];
				aInner[0].aVertex[i-1].dY = aPolygon2[i].dCoor[2];

				aInner[1].aVertex[i-1].dX = aPolygon2[i+10].dCoor[1];
				aInner[1].aVertex[i-1].dY = aPolygon2[i+10].dCoor[2];
			}

			// 마감작업을 해주자.
			aInner[0].aVertex[9].dX = aPolygon2[0].dCoor[1];
			aInner[0].aVertex[9].dY = aPolygon2[0].dCoor[2];
			
			aInner[1].aVertex[9].dX = aPolygon2[10].dCoor[1];
			aInner[1].aVertex[9].dY = aPolygon2[10].dCoor[2];
		}
	} 
	// 2. 3-Cell 일 경우
	else if (CSectD.Shape == D_SECT_SHAPE_PSC_3CELL)
	{
		ASSERT(nVSize == 40); // 2Cell 의 Inner는 반드시 20개
		if (nVSize == 40)
		{
			aInner.SetSize(3);
			aInner[0].aVertex.SetSize(13);
			aInner[1].aVertex.SetSize(13);
			aInner[2].aVertex.SetSize(15);

			for (i=0; i<12; i++)
			{
				aInner[0].aVertex[i].dX = aPolygon2[i].dCoor[1];
				aInner[0].aVertex[i].dY = aPolygon2[i].dCoor[2];
				
				aInner[1].aVertex[i].dX = aPolygon2[i+12].dCoor[1];
				aInner[1].aVertex[i].dY = aPolygon2[i+12].dCoor[2];
			}
			// 마감작업을 해주자.
			aInner[0].aVertex[12].dX = aPolygon2[0].dCoor[1];
			aInner[0].aVertex[12].dY = aPolygon2[0].dCoor[2];      
			aInner[1].aVertex[12].dX = aPolygon2[12].dCoor[1];
			aInner[1].aVertex[12].dY = aPolygon2[12].dCoor[2];

			int nIndex(0);
			for (i=0; i<14; i++)
			{
				nIndex = (i<8) ? 24+i : 25+i;
				aInner[2].aVertex[i].dX = aPolygon2[nIndex].dCoor[1];
				aInner[2].aVertex[i].dY = aPolygon2[nIndex].dCoor[2];
			}

			// 마감작업을 해주자.
			aInner[2].aVertex[14].dX = aPolygon2[24].dCoor[1];
			aInner[2].aVertex[14].dY = aPolygon2[24].dCoor[2];
		}
	}
	// 3. nCell 일 경우
	else if (CSectD.Shape == D_SECT_SHAPE_PSC_4CELL)
	{
		int nCell = CSectD.nCellShape-1;
		int nTotal = 18 + nCell*8;
		ASSERT(nVSize == nTotal);
		if (nVSize == nTotal)
		{
			aInner.SetSize(2 + nCell);
			aInner[0].aVertex.SetSize(10);
			aInner[1].aVertex.SetSize(10);      
			for (i=0; i<nCell; i++) aInner[i+2].aVertex.SetSize(9);

			for (i=0; i<9; i++)
			{
				aInner[0].aVertex[i].dX = aPolygon2[i].dCoor[1];
				aInner[0].aVertex[i].dY = aPolygon2[i].dCoor[2];        
				aInner[1].aVertex[i].dX = aPolygon2[i+9].dCoor[1];
				aInner[1].aVertex[i].dY = aPolygon2[i+9].dCoor[2];
			}
			// 마감작업을 해주자.
			aInner[0].aVertex[9].dX = aPolygon2[0].dCoor[1];
			aInner[0].aVertex[9].dY = aPolygon2[0].dCoor[2];
			aInner[1].aVertex[9].dX = aPolygon2[9].dCoor[1];
			aInner[1].aVertex[9].dY = aPolygon2[9].dCoor[2];

			for (i=0; i<nCell; i++)
			{
				for (int j=0; j<8; j++)
				{
					aInner[i+2].aVertex[j].dX = aPolygon2[18+i*8+j].dCoor[1];
					aInner[i+2].aVertex[j].dY = aPolygon2[18+i*8+j].dCoor[2];
				}
				// 마감작업을 해주자.
				aInner[i+2].aVertex[8].dX = aPolygon2[18+i*8].dCoor[1];
				aInner[i+2].aVertex[8].dY = aPolygon2[18+i*8].dCoor[2];
			}      
		}
	}
	// 4. nCell2 일 경우 ( Polygon 과 Circle 분리 )
	else if (CSectD.Shape == D_SECT_SHAPE_PSC_NCEL2)
	{
		if (CSectD.nCellType == 0) // Polygon
		{
			int nCell = CSectD.nCellShape-2;
			int nTotal = 32 + nCell*8;
			ASSERT(nVSize == nTotal);
			if (nVSize == nTotal)
			{
				aInner.SetSize(2 + nCell);
				aInner[0].aVertex.SetSize(17);
				aInner[1].aVertex.SetSize(17);      
				for (i=0; i<nCell; i++) aInner[i+2].aVertex.SetSize(9);
				
				for (i=0; i<16; i++)
				{
					aInner[0].aVertex[i].dX = aPolygon2[i].dCoor[1];
					aInner[0].aVertex[i].dY = aPolygon2[i].dCoor[2];          
					
					aInner[1].aVertex[i].dX = aPolygon2[i+16].dCoor[1];
					aInner[1].aVertex[i].dY = aPolygon2[i+16].dCoor[2];
				}

				// 마감작업을 해주자.
				aInner[0].aVertex[16].dX = aPolygon2[0].dCoor[1];
				aInner[0].aVertex[16].dY = aPolygon2[0].dCoor[2];        
				aInner[1].aVertex[16].dX = aPolygon2[16].dCoor[1];
				aInner[1].aVertex[16].dY = aPolygon2[16].dCoor[2];
				
				for (i=0; i<nCell; i++)
				{
					for (int j=0; j<8; j++)
					{
						aInner[i+2].aVertex[j].dX = aPolygon2[32+i*8+j].dCoor[1];
						aInner[i+2].aVertex[j].dY = aPolygon2[32+i*8+j].dCoor[2];
					}
					// 마감작업을 해주자.
					aInner[i+2].aVertex[8].dX = aPolygon2[32+i*8].dCoor[1];
					aInner[i+2].aVertex[8].dY = aPolygon2[32+i*8].dCoor[2];
				}
			}
		}
		else if (CSectD.nCellType == 1) // Circle
		{
			// Hole 여부 - Circle 일 때만
			BOOL bHole = CSectD.bSmallHole;
			int nCell = CSectD.nCellShape;
			int nTotal = (bHole==TRUE) ? 48 + nCell*26 : nCell*26;
			ASSERT(nVSize == nTotal);
			if (nVSize == nTotal)
			{
				// 가독성을 위해 아예 분해해버리자. 디버깅하기도 짜증나겄다.
				if (bHole == TRUE)
				{
					aInner.SetSize(2+nCell);
					aInner[0].aVertex.SetSize(25);
					aInner[1].aVertex.SetSize(25);
					for (i=0; i<nCell; i++) aInner[i+2].aVertex.SetSize(27);

					for (i=0; i<24; i++)
					{
						aInner[0].aVertex[i].dX = aPolygon2[i].dCoor[1];
						aInner[0].aVertex[i].dY = aPolygon2[i].dCoor[2];
						
						aInner[1].aVertex[i].dX = aPolygon2[i+24+nCell*26].dCoor[1];
						aInner[1].aVertex[i].dY = aPolygon2[i+24+nCell*26].dCoor[2];
					}
					// 마감작업을 해주자.
					aInner[0].aVertex[24].dX = aPolygon2[0].dCoor[1];
					aInner[0].aVertex[24].dY = aPolygon2[0].dCoor[2];          
					aInner[1].aVertex[24].dX = aPolygon2[24+nCell*26].dCoor[1];
					aInner[1].aVertex[24].dY = aPolygon2[24+nCell*26].dCoor[2];          

					for (i=0; i<nCell; i++)
					{
						for (int j=0; j<26; j++)
						{
							aInner[i+2].aVertex[j].dX = aPolygon2[24+i*26+j].dCoor[1];
							aInner[i+2].aVertex[j].dY = aPolygon2[24+i*26+j].dCoor[2];
						}
						// 마감작업을 해주자.
						aInner[i+2].aVertex[26].dX = aPolygon2[24+i*26].dCoor[1];
						aInner[i+2].aVertex[26].dY = aPolygon2[24+i*26].dCoor[2];
					}
				}
				else
				{
					aInner.SetSize(nCell);
					for (i=0; i<nCell; i++)
					{
						aInner[i].aVertex.SetSize(27);
						for (int j=0; j<26; j++)
						{
							aInner[i].aVertex[j].dX = aPolygon2[i*26+j].dCoor[1];
							aInner[i].aVertex[j].dY = aPolygon2[i*26+j].dCoor[2];
						}
						// 마감작업을 해주자.
						aInner[i].aVertex[26].dX = aPolygon2[i*26].dCoor[1];
						aInner[i].aVertex[26].dY = aPolygon2[i*26].dCoor[2];
					}
				}
			}
		}
		else
		{
			ASSERT(FALSE);
		}   
	}
	// 5. 2PLAT-2CELL 일 경우
	else if (CSectD.Shape == D_SECT_SHAPE_PSC_BOX && CSectD.nCellType == 2)
	{
		ASSERT(nVSize == 48); // 2Cell 의 Inner는 반드시 20개
		if (nVSize == 48)
		{
			aInner.SetSize(2);
			aInner[0].aVertex.SetSize(25);
			aInner[1].aVertex.SetSize(25);
			
			for (i=0; i<24; i++)
			{
				aInner[0].aVertex[i].dX = aPolygon2[i].dCoor[1];
				aInner[0].aVertex[i].dY = aPolygon2[i].dCoor[2];
				
				aInner[1].aVertex[i].dX = aPolygon2[i+24].dCoor[1];
				aInner[1].aVertex[i].dY = aPolygon2[i+24].dCoor[2];
			}      
			// 마감작업을 해주자.
			aInner[0].aVertex[24].dX = aPolygon2[0].dCoor[1];
			aInner[0].aVertex[24].dY = aPolygon2[0].dCoor[2];      
			aInner[1].aVertex[24].dX = aPolygon2[24].dCoor[1];
			aInner[1].aVertex[24].dY = aPolygon2[24].dCoor[2];
		}
	}
	else
	{    
		if (nVSize > 0)
		{
			aInner.SetSize(1);
			aInner[0].aVertex.SetSize(nVSize);
			for (i=0; i<nVSize; i++)
			{
				aInner[0].aVertex[i].dX = aPolygon2[i].dCoor[1];
				aInner[0].aVertex[i].dY = aPolygon2[i].dCoor[2];
			}
		}
	}

#ifdef _DEBUG
	FILE *wfp = _wfopen(_T("C:\\IDEA_Psc_OutLine.txt"),_T("wt"));
	if(wfp)
	{
		fwprintf(wfp,_T("*NODE\n"));
		fwprintf(wfp, _T(" %s Outer : %d, Inner : %d \n"), (LPCTSTR)(CSectD.SName), static_cast<int>(aOuter.GetSize()), static_cast<int>(aInner.GetSize()));

		for(int ii=0; ii<aOuter.GetSize(); ii++)
		{    
			fwprintf(wfp,_T(" Outer : %d \n"), ii+1);
			for (int jj=0; jj<aOuter[ii].aVertex.GetSize(); jj++)
			{
				fwprintf(wfp,_T(" %d, %g, %g\n"), jj+1, aOuter[ii].aVertex[jj].dX, aOuter[ii].aVertex[jj].dY);
			}    
		}

		for(int ii=0; ii<aInner.GetSize(); ii++)
		{    
			fwprintf(wfp,_T(" Inner : %d \n"), ii+1);
			for (int jj=0; jj<aInner[ii].aVertex.GetSize(); jj++)
			{
				fwprintf(wfp,_T(" %d, %g, %g\n"), jj+1, aInner[ii].aVertex[jj].dX, aInner[ii].aVertex[jj].dY);
			}    
		}
		fclose(wfp);  
	}
	
#endif
}

void CDgnClassIREW::MakePSCOutLineCSCS(CDBDoc* pDoc, double SectionSize[D_SECT_SIZE_NUM_MAX], T_SECT_D& CSectD, T_CSCS_D&  CCscsD, S_GSEC_POLYGON_LIST& aOuter, BOOL bIncludeJ)
{
	// 좌표를 생성할 조건을 판단한다.
	// 조건은 1cell, 2cell, mid, half-none, half-polygon 이면서 CCscsD.nCompType 이 0이나 1이어여 한다. 그리고 합성중이어야 한다.
	aOuter.RemoveAll();

	if (CCscsD.nCompType!=0 && CCscsD.nCompType!=1)
	{
		// 합성단면 계산 조건이 아님..
		return;
	}

	if (CCscsD.aPart.GetSize() != 3)
	{
		ASSERT(FALSE);
		return;
	}

	// Part 정보를 셋팅한다.
	CArray<T_CSCS_BASE, T_CSCS_BASE&> aOrgPart;
	aOrgPart.Copy(CCscsD.aPart);
	CArray<T_CSCS_BASE, T_CSCS_BASE&> aTempPart;
	if (CCscsD.nCompType == 0)
	{
		aTempPart.Add(CCscsD.aPart[0]);
		aTempPart.Add(CCscsD.aPart[1]);
	}
	else if (CCscsD.nCompType == 1)
	{
		aTempPart.Add(CCscsD.aPart[0]);    
	}
//   else
//   {
//     CCscsD.aPart.Copy(aOrgPart);
//     return;
//   }  

	CCscsD.aPart.Copy(aTempPart);

	BOOL bExecute(FALSE), bReturn(FALSE);
	if (CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_1CELL || CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_2CELL || CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_MID ||
		 (CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_HALF && (CSectD.SectBefore.nCellShape == 0 || CSectD.SectBefore.nCellShape == 2))) { bExecute = TRUE; }
	if (bExecute == TRUE) { bExecute = (CCscsD.nCompType == 0 || CCscsD.nCompType == 1); }
	int nPart(0), i(0); // 1:1, 2:3, 3:1+2, 4:2+3
	if (bExecute == TRUE)
	{
		if (CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_HALF && CSectD.SectBefore.nCellShape == 0)
		{
			if(CCscsD.aPart.GetSize()!=1) { ASSERT(FALSE); CCscsD.aPart.Copy(aOrgPart); return; }
			nPart = CCscsD.aPart[0].nPart; 
			bReturn = nPart != 1;
		}
		else
		{      
			if(CCscsD.aPart.GetSize()==1)
			{
				if(CCscsD.aPart[0].nPart==1)     nPart=1;
				else if(CCscsD.aPart[0].nPart==3)nPart=2;
				else { ASSERT(FALSE); CCscsD.aPart.Copy(aOrgPart); return; }
				
			}
			else if(CCscsD.aPart.GetSize()==2)
			{
				if(CCscsD.aPart[1].nPart+CCscsD.aPart[0].nPart==3)      nPart=3;
				else if(CCscsD.aPart[1].nPart+CCscsD.aPart[0].nPart==5) nPart=4;
				else { ASSERT(FALSE); CCscsD.aPart.Copy(aOrgPart); return; }
			}
			else
			{
				ASSERT(FALSE);
				bReturn = TRUE;
			}
			
			if (bExecute == TRUE)
			{
				if (nPart == 2 || nPart == 4) bExecute = FALSE;
			}
		}
	}
	if (bReturn  == TRUE || bExecute == FALSE)
	{
		//MakePSCOutLine(pDoc, SectionSize, CSectD.SectBefore, aOuter, aInner);
		// 앗싸리 그리지 않는다. 왜냐고? Section에 있으니깐..
		CCscsD.aPart.Copy(aOrgPart);
		return;
	}

	// PSC의 시공단계는 모든 경우 폴리곤이 한개임.
	if (bIncludeJ == TRUE)
		aOuter.SetSize(2);
	else
		aOuter.SetSize(1);
	
	double dSectionSize[2][D_SECT_SIZE_NUM_MAX];
	memset(dSectionSize, 0, sizeof(dSectionSize));
	BOOL bThickness = FALSE;
	BOOL bTriangle  = FALSE;
	double FrameLocalVector[3][3]={1.,0.,0., 0.,1.,0., 0.,0.,1.};
	double xyz[2][3]={0.,0.,0., 0.,0.,0.};
	int rNumPlane;
	CArray<int, int>        aPlaneVCount;
	CArray<int, int>        aPlaneVertexIndex;
	CArray<BOOL, BOOL>      aEdgeFlag;
	CArray<double, double>  aVertex;
	int elnod[D_ELEM_MAXNOD];
	memset(elnod, 0, sizeof(elnod));    

	CString Shape;  
	pDoc->m_pAttrCtrl->GetSectionSize(CSectD, Shape, dSectionSize);

	if (CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_1CELL)
	{
		pDoc->m_pSecShape->makePSC_1Cell_CSCS(dSectionSize, CSectD.SectBefore, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);

		if (nPart == 1) // 1
		{      
			if (aVertex.GetSize() != 32*3)
			{
				ASSERT(FALSE);
				CCscsD.aPart.Copy(aOrgPart);
			}

			aOuter[0].aVertex.SetSize(14);
			for (i=0; i<8; i++)
			{
				aOuter[0].aVertex[i].dX = aVertex[i*3+1];
				aOuter[0].aVertex[i].dY = aVertex[i*3+2];
			}
			for (i=0; i<6; i++)
			{
				aOuter[0].aVertex[i+8].dX = aVertex[(15-i)*3+1];
				aOuter[0].aVertex[i+8].dY = aVertex[(15-i)*3+2];
			}

			if (bIncludeJ == TRUE)
			{        
				aOuter[1].aVertex.SetSize(14);
				for (i=0; i<8; i++)
				{
					aOuter[1].aVertex[i].dX = aVertex[i*3+1 + 16*3];
					aOuter[1].aVertex[i].dY = aVertex[i*3+2 + 16*3];
				}
				for (i=0; i<6; i++)
				{
					aOuter[1].aVertex[i+8].dX = aVertex[(15-i)*3+1 + 16*3];
					aOuter[1].aVertex[i+8].dY = aVertex[(15-i)*3+2 + 16*3];
				}
			}
		}
		else if (nPart == 3) // 1+2
		{
			if (aVertex.GetSize() != 48*3)
			{
				ASSERT(FALSE);
				CCscsD.aPart.Copy(aOrgPart);
			}

			aOuter[0].aVertex.SetSize(22);
			for (i=0; i<12; i++)
			{
				aOuter[0].aVertex[i].dX = aVertex[i*3+1];
				aOuter[0].aVertex[i].dY = aVertex[i*3+2];
			}
			for (i=0; i<10; i++)
			{
				aOuter[0].aVertex[i+12].dX = aVertex[(23-i)*3+1];
				aOuter[0].aVertex[i+12].dY = aVertex[(23-i)*3+2];
			}

			if (bIncludeJ == TRUE)
			{        
				aOuter[1].aVertex.SetSize(22);
				for (i=0; i<12; i++)
				{
					aOuter[1].aVertex[i].dX = aVertex[i*3+1 + 24*3];
					aOuter[1].aVertex[i].dY = aVertex[i*3+2 + 24*3];
				}
				for (i=0; i<10; i++)
				{
					aOuter[1].aVertex[i+12].dX = aVertex[(23-i)*3+1 + 24*3];
					aOuter[1].aVertex[i+12].dY = aVertex[(23-i)*3+2 + 24*3];
				}
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_2CELL)
	{
		pDoc->m_pSecShape->makePSC_2Cell_CSCS(dSectionSize, CSectD.SectBefore, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);    
		if (nPart == 1) // 1
		{      
			if (aVertex.GetSize() != 40*3)
			{
				ASSERT(FALSE);
				CCscsD.aPart.Copy(aOrgPart);
			}
			
			aOuter[0].aVertex.SetSize(18);
			for (i=0; i<10; i++)
			{
				aOuter[0].aVertex[i].dX = aVertex[(i+1)*3+1];
				aOuter[0].aVertex[i].dY = aVertex[(i+1)*3+2];
			}
			for (i=0; i<8; i++)
			{
				aOuter[0].aVertex[i+10].dX = aVertex[(19-i)*3+1];
				aOuter[0].aVertex[i+10].dY = aVertex[(19-i)*3+2];
			}

			if (bIncludeJ == TRUE)
			{        
				aOuter[1].aVertex.SetSize(18);
				for (i=0; i<10; i++)
				{
					aOuter[1].aVertex[i].dX = aVertex[(i+1)*3+1 + 20*3];
					aOuter[1].aVertex[i].dY = aVertex[(i+1)*3+2 + 20*3];
				}
				for (i=0; i<8; i++)
				{
					aOuter[1].aVertex[i+10].dX = aVertex[(19-i)*3+1 + 20*3];
					aOuter[1].aVertex[i+10].dY = aVertex[(19-i)*3+2 + 20*3];
				}
			}
		}
		else if (nPart == 3) // 1+2
		{
			if (aVertex.GetSize() != 64*3)
			{
				ASSERT(FALSE);
				CCscsD.aPart.Copy(aOrgPart);
			}
			
			aOuter[0].aVertex.SetSize(27);
			for (i=0; i<14; i++)
			{
				aOuter[0].aVertex[i].dX = aVertex[(i+2)*3+1];
				aOuter[0].aVertex[i].dY = aVertex[(i+2)*3+2];
			}
			for (i=0; i<13; i++)
			{
				aOuter[0].aVertex[i+14].dX = aVertex[(31-i)*3+1];
				aOuter[0].aVertex[i+14].dY = aVertex[(31-i)*3+2];
			}

			if (bIncludeJ == TRUE)
			{        
				aOuter[1].aVertex.SetSize(27);
				for (i=0; i<14; i++)
				{
					aOuter[1].aVertex[i].dX = aVertex[(i+2)*3+1 + 32*3];
					aOuter[1].aVertex[i].dY = aVertex[(i+2)*3+2 + 32*3];
				}
				for (i=0; i<13; i++)
				{
					aOuter[1].aVertex[i+14].dX = aVertex[(31-i)*3+1 + 32*3];
					aOuter[1].aVertex[i+14].dY = aVertex[(31-i)*3+2 + 32*3];
				}
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_MID)
	{
		pDoc->m_pSecShape->makePSC_Mid_CSCS(dSectionSize, CSectD.SectBefore, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);        
		if (nPart == 1) // 1
		{      
			if (aVertex.GetSize() == 40*3)
			{
				ASSERT(FALSE);
				CCscsD.aPart.Copy(aOrgPart);
			}
			
			aOuter[0].aVertex.SetSize(16);
			for (i=0; i<9; i++)
			{
				aOuter[0].aVertex[i].dX = aVertex[i*3+1];
				aOuter[0].aVertex[i].dY = aVertex[i*3+2];
			}
			for (i=0; i<7; i++)
			{
				aOuter[0].aVertex[i+9].dX = aVertex[(18-i)*3+1];
				aOuter[0].aVertex[i+9].dY = aVertex[(18-i)*3+2];
			}

			if (bIncludeJ == TRUE)
			{        
				aOuter[1].aVertex.SetSize(16);
				for (i=0; i<9; i++)
				{
					aOuter[1].aVertex[i].dX = aVertex[i*3+1 + 20*3];
					aOuter[1].aVertex[i].dY = aVertex[i*3+2 + 20*3];
				}
				for (i=0; i<7; i++)
				{
					aOuter[1].aVertex[i+9].dX = aVertex[(18-i)*3+1 + 20*3];
					aOuter[1].aVertex[i+9].dY = aVertex[(18-i)*3+2 + 20*3];
				}
			}
		}
		else if (nPart == 3) // 1+2
		{
			if (aVertex.GetSize() == 48*3)
			{
				ASSERT(FALSE);
				CCscsD.aPart.Copy(aOrgPart);
			}
			
			aOuter[0].aVertex.SetSize(17);
			for (i=0; i<9; i++)
			{
				aOuter[0].aVertex[i].dX = aVertex[i*3+1];
				aOuter[0].aVertex[i].dY = aVertex[i*3+2];
			}
			for (i=0; i<8; i++)
			{
				aOuter[0].aVertex[i+9].dX = aVertex[(21-i)*3+1];
				aOuter[0].aVertex[i+9].dY = aVertex[(21-i)*3+2];
			}

			if (bIncludeJ == TRUE)
			{        
				aOuter[1].aVertex.SetSize(17);
				for (i=0; i<9; i++)
				{
					aOuter[1].aVertex[i].dX = aVertex[i*3+1 + 24*3];
					aOuter[1].aVertex[i].dY = aVertex[i*3+2 + 24*3];
				}
				for (i=0; i<8; i++)
				{
					aOuter[1].aVertex[i+9].dX = aVertex[(21-i)*3+1 + 24*3];
					aOuter[1].aVertex[i+9].dY = aVertex[(21-i)*3+2 + 24*3];
				}
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (CSectD.SectBefore.Shape == D_SECT_SHAPE_PSC_HALF)
	{
		pDoc->m_pSecShape->makePSC_Half_CSCS(dSectionSize, CSectD.SectBefore, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);        
		if (CSectD.SectBefore.nCellShape == 0)
		{
			if (nPart == 1)
			{
				if (aVertex.GetSize() == 12*3)
				{
					ASSERT(FALSE);
					CCscsD.aPart.Copy(aOrgPart);
				}
				
				aOuter[0].aVertex.SetSize(6);
				for (i=0; i<6; i++)
				{
					aOuter[0].aVertex[i].dX = aVertex[i*3+1];
					aOuter[0].aVertex[i].dY = aVertex[i*3+2];
				}

				if (bIncludeJ == TRUE)
				{        
					aOuter[1].aVertex.SetSize(6);
					for (i=0; i<6; i++)
					{
						aOuter[1].aVertex[i].dX = aVertex[i*3+1+6*3];
						aOuter[1].aVertex[i].dY = aVertex[i*3+2+6*3];
					}
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else if (CSectD.SectBefore.nCellShape == 2)
		{
			if (nPart == 1) // 1
			{      
				if (aVertex.GetSize() == 16*3)
				{
					ASSERT(FALSE);
					CCscsD.aPart.Copy(aOrgPart);
				}
				
				aOuter[0].aVertex.SetSize(8);
				for (i=0; i<8; i++)
				{
					aOuter[0].aVertex[i].dX = aVertex[i*3+1];
					aOuter[0].aVertex[i].dY = aVertex[i*3+2];
				}

				if (bIncludeJ == TRUE)
				{        
					aOuter[1].aVertex.SetSize(8);
					for (i=0; i<8; i++)
					{
						aOuter[1].aVertex[i].dX = aVertex[i*3+1+8*3];
						aOuter[1].aVertex[i].dY = aVertex[i*3+2+8*3];
					}
				}
			}
			else if (nPart == 3) // 1+2
			{
				if (aVertex.GetSize() == 24*3)
				{
					ASSERT(FALSE);
					CCscsD.aPart.Copy(aOrgPart);
					return;
				}
				
				aOuter[0].aVertex.SetSize(12);
				for (i=0; i<12; i++)
				{
					aOuter[0].aVertex[i].dX = aVertex[i*3+1];
					aOuter[0].aVertex[i].dY = aVertex[i*3+2];
				}

				if (bIncludeJ == TRUE)
				{        
					aOuter[1].aVertex.SetSize(12);
					for (i=0; i<12; i++)
					{
						aOuter[1].aVertex[i].dX = aVertex[i*3+1 + 12*3];
						aOuter[1].aVertex[i].dY = aVertex[i*3+2 + 12*3];
					}
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	CCscsD.aPart.Copy(aOrgPart);

#ifdef _DEBUG
	FILE *wfp = _wfopen(_T("C:\\IDEA_Psc_OutLine_CSCS.txt"),_T("wt"));
	
	int ii(0), jj(0);
	fwprintf(wfp,_T("*NODE\n"));
	fwprintf(wfp,_T(" %s Outer : %d, Inner : %d \n"), (LPCTSTR)(CSectD.SName), static_cast<int>(aOuter.GetSize()), 0);
	
	for(ii=0; ii<aOuter.GetSize(); ii++)
	{    
		fwprintf(wfp,_T(" Outer : %d \n"), ii+1);
		for (jj=0; jj<aOuter[ii].aVertex.GetSize(); jj++)
		{
			fwprintf(wfp,_T(" %d, %g, %g\n"), jj+1, aOuter[ii].aVertex[jj].dX, aOuter[ii].aVertex[jj].dY);
		}    
	}
	
	fclose(wfp);  
#endif
}

void CDgnClassIREW::MakeCOMPOutLine(CDBDoc* pDoc, T_SECT_D& CSectD, S_GSEC_POLYGON_LIST& aOuter, S_GSEC_POLYGON_LIST& aInner, BOOL bTap, BOOL bPosI)
{
	aOuter.RemoveAll();
	aInner.RemoveAll();

	int nSTYPE = (bTap) ? CSectD.SectBefore.nStype : CSectD.nStype;

	// Composite General Section은 미리 처리한다.
	if (nSTYPE == D_SECT_TYPE_COMPO_G)
	{
		T_SECT_SECTBASE_D*  pSect = bPosI ? &CSectD.SectBefore.SectI : &CSectD.SectBefore.SectJ;
		int nOut = pSect->aOuterPolygon.GetSize();
		int nIn  = pSect->aInnerPolygon.GetSize();

		aOuter.SetSize(nOut);
		aInner.SetSize(nIn);

		int i(0), j(0), nSub(0);
		for (i=0; i<nOut; i++)
		{
			nSub = pSect->aOuterPolygon[i].aVertex.GetSize();
			aOuter[i].aVertex.SetSize(nSub);
			for (j=0; j<nSub; j++)
			{
				aOuter[i].aVertex[j].dX = pSect->aOuterPolygon[i].aVertex[j].dX;
				aOuter[i].aVertex[j].dY = pSect->aOuterPolygon[i].aVertex[j].dY;
			}
		}
		for (i=0; i<nIn; i++)
		{
			nSub = pSect->aInnerPolygon[i].aVertex.GetSize();
			aInner[i].aVertex.SetSize(nSub);
			for (j=0; j<nSub; j++)
			{
				aInner[i].aVertex[j].dX = pSect->aInnerPolygon[i].aVertex[j].dX;
				aInner[i].aVertex[j].dY = pSect->aInnerPolygon[i].aVertex[j].dY;
			}
		}
		return;
	}

	if (nSTYPE == D_SECT_TYPE_COMPO_B)
	{
		// DgnEngine 에 있는 건데 DB 통해서 불러야 할까? 그냥 계산함수 가지고 있을까?
		S_GSEC_POLYGON_LIST aOutTemp, aInTop, aInBot;
		double Bc  = CSectD.SectAfter.SectJ.Size[0];
		double tc  = CSectD.SectAfter.SectJ.Size[1];
		double Hh  = CSectD.SectAfter.SectJ.Size[2];
		double Hw  = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[0] : CSectD.CmpTapJ.Size[0];
		double tw  = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[1] : CSectD.CmpTapJ.Size[1];
		double B1  = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[2] : CSectD.CmpTapJ.Size[2];
		double Bf1 = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[3] : CSectD.CmpTapJ.Size[3];
		double tf1 = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[4] : CSectD.CmpTapJ.Size[4];
		double B2  = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[5] : CSectD.CmpTapJ.Size[5];
		double Bf2 = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[6] : CSectD.CmpTapJ.Size[6];
		double tf2 = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[7] : CSectD.CmpTapJ.Size[7];
		double Hr1 = CSectD.SectBefore.SectJ.Size[2];
		double Hr2 = CSectD.SectBefore.SectJ.Size[3];
		double tr1 = CSectD.SectBefore.SectJ.Size[4];
		double tr2 = CSectD.SectBefore.SectJ.Size[5];
		int    N1  = CSectD.SectBefore.nStiffNum[0];
		int    N2  = CSectD.SectBefore.nStiffNum[1];    
		double  leng[3], tempx(0.0), tempy(0.0), tempx1(0.0), tempy1(0.0);
		memset(leng, 0, sizeof(leng));

		aOutTemp.SetSize(5);
		for (int i=0; i<5; i++) aOutTemp[i].aVertex.SetSize(4);

		leng[0] = Bc;
		leng[1] = Bf1 + B1 + Bf1;
		leng[2] = Bf2 + B2 + Bf2;

		tempx = (leng[0] - leng[1]) * 0.5;
		tempy = tc + Hh;	  
		aOutTemp[0].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[0].aVertex[1] = S_GSEC_VERTEX(tempx+leng[1], -tempy);
		aOutTemp[0].aVertex[2] = S_GSEC_VERTEX(tempx+leng[1], -(tempy+tf1));
		aOutTemp[0].aVertex[3] = S_GSEC_VERTEX(tempx, -(tempy+tf1));

		tempx = (leng[0] - leng[2]) * 0.5;
		tempy = tc + Hh + tf1 + Hw;
		aOutTemp[1].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[1].aVertex[1] = S_GSEC_VERTEX(tempx+leng[2], -tempy);
		aOutTemp[1].aVertex[2] = S_GSEC_VERTEX(tempx+leng[2], -(tempy+tf2));
		aOutTemp[1].aVertex[3] = S_GSEC_VERTEX(tempx, -(tempy+tf2));

		tempx = (leng[0] - leng[1]) * 0.5 + Bf1;
		tempy = tc + Hh + tf1;
		tempx1 = (leng[0] - leng[2]) * 0.5 + Bf2;
		tempy1 = tc + Hh + tf1 + Hw;
		aOutTemp[2].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[2].aVertex[1] = S_GSEC_VERTEX(tempx-tw, -tempy);
		aOutTemp[2].aVertex[2] = S_GSEC_VERTEX(tempx1-tw, -tempy1);
		aOutTemp[2].aVertex[3] = S_GSEC_VERTEX(tempx1, -tempy1);

		tempx = (leng[0] - leng[1]) * 0.5 + Bf1 + B1;
		tempy = tc + Hh + tf1;
		tempx1 = (leng[0] - leng[2]) * 0.5 + Bf2 + B2;
		tempy1 = tc + Hh + tf1 + Hw;
		aOutTemp[3].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[3].aVertex[1] = S_GSEC_VERTEX(tempx+tw, -tempy);
		aOutTemp[3].aVertex[2] = S_GSEC_VERTEX(tempx1+tw, -tempy1);
		aOutTemp[3].aVertex[3] = S_GSEC_VERTEX(tempx1, -tempy1);

		// Slab
		tempx = 0;
		tempy = 0;    
		aOutTemp[4].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[4].aVertex[1] = S_GSEC_VERTEX(tempx+leng[0], -tempy);
		aOutTemp[4].aVertex[2] = S_GSEC_VERTEX(tempx+leng[0], -tc);
		aOutTemp[4].aVertex[3] = S_GSEC_VERTEX(tempx, -tc);
		// Rib ////////////////////////////////////////
		if(B1 > 0)
		{
			// 상단
			double term, ribhw, ribh, x;
			term = B1 / (N1 + 1);
			tempx = (leng[0] - leng[1]) * 0.5 + Bf1;
			tempy = tc + Hh + tf1;
			ribhw = tr1 * 0.5;
			ribh  = Hr1;
			aInTop.SetSize(N1);
			for(int i=0; i < N1; i++)
			{
				x = tempx + (term * (i+1));       
				aInTop[i].aVertex.SetSize(4);
				aInTop[i].aVertex[0] = S_GSEC_VERTEX(x-ribhw, -tempy);
				aInTop[i].aVertex[1] = S_GSEC_VERTEX(x+ribhw, -tempy);
				aInTop[i].aVertex[2] = S_GSEC_VERTEX(x+ribhw, -(tempy+ribh));
				aInTop[i].aVertex[3] = S_GSEC_VERTEX(x-ribhw, -(tempy+ribh));        
			}
		}
		if(B2 > 0)
		{
			// 하단
			double term, ribhw, ribh, x;
			term = B2 / (N2 + 1);
			tempx = (leng[0] - leng[2]) * 0.5 + Bf2;
			tempy = tc + Hh + tf1 + Hw;
			ribhw = tr2 * 0.5;
			ribh  = Hr2;
			aInBot.SetSize(N2);
			for(int i=0; i < N2; i++)
			{
				x = tempx + (term * (i+1));        
				aInBot[i].aVertex.SetSize(4);
				aInBot[i].aVertex[0] = S_GSEC_VERTEX(x-ribhw, -tempy+ribh);
				aInBot[i].aVertex[1] = S_GSEC_VERTEX(x+ribhw, -tempy+ribh);
				aInBot[i].aVertex[2] = S_GSEC_VERTEX(x+ribhw, -tempy);
				aInBot[i].aVertex[3] = S_GSEC_VERTEX(x-ribhw, -tempy);
			}
		}

		///////////////////////////////////////////////
		// 기존DATA 생성완료 -> 변환시작
		aOuter.SetSize(2);
		aOuter[0].aVertex.SetSize(12);
		aOuter[1].aVertex.SetSize(4);
		// 기본 Inner 4개에다가 Rib 별로 4개    
		// Box 외곽선
		aOuter[0].aVertex[0]  = aOutTemp[0].aVertex[0];
		aOuter[0].aVertex[1]  = aOutTemp[0].aVertex[3];
		aOuter[0].aVertex[2]  = aOutTemp[2].aVertex[0];
		aOuter[0].aVertex[3]  = aOutTemp[2].aVertex[3];
		aOuter[0].aVertex[4]  = aOutTemp[1].aVertex[0];
		aOuter[0].aVertex[5]  = aOutTemp[1].aVertex[3];
		aOuter[0].aVertex[6]  = aOutTemp[1].aVertex[2];
		aOuter[0].aVertex[7]  = aOutTemp[1].aVertex[1];
		aOuter[0].aVertex[8]  = aOutTemp[3].aVertex[2];
		aOuter[0].aVertex[9]  = aOutTemp[3].aVertex[1];
		aOuter[0].aVertex[10] = aOutTemp[0].aVertex[2];
		aOuter[0].aVertex[11] = aOutTemp[0].aVertex[1];
		// Slab 외곽선
		aOuter[1].aVertex.Copy(aOutTemp[4].aVertex);    

		aInner.SetSize(3);
		aInner[0].aVertex.SetSize(4);
		aInner[0].aVertex[0] = aOutTemp[2].aVertex[1];
		aInner[0].aVertex[1] = aOutTemp[2].aVertex[2];
		aInner[0].aVertex[2] = aOutTemp[3].aVertex[3];
		aInner[0].aVertex[3] = aOutTemp[3].aVertex[0];
		aInner[1].aVertex.SetSize(N1*4);
		for (int i=0; i<N1; i++)
		{
			aInner[1].aVertex[i*4+0] = aInTop[i].aVertex[0];
			aInner[1].aVertex[i*4+1] = aInTop[i].aVertex[1];
			aInner[1].aVertex[i*4+2] = aInTop[i].aVertex[2];
			aInner[1].aVertex[i*4+3] = aInTop[i].aVertex[3];
		}
		aInner[2].aVertex.SetSize(N2*4);
		for (int i=0; i<N2; i++)
		{      
			aInner[2].aVertex[i*4+0] = aInBot[i].aVertex[0];
			aInner[2].aVertex[i*4+1] = aInBot[i].aVertex[1];
			aInner[2].aVertex[i*4+2] = aInBot[i].aVertex[2];
			aInner[2].aVertex[i*4+3] = aInBot[i].aVertex[3];
		}
		// 변환완료
		///////////////////////////////////////////////  
	}
	else if (nSTYPE == D_SECT_TYPE_COMPO_I)
	{   
		double Bc  = CSectD.SectAfter.SectJ.Size[0];
		double tc  = CSectD.SectAfter.SectJ.Size[1];
		double Hh  = CSectD.SectAfter.SectJ.Size[2];        
		double Hw  = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[0] : CSectD.CmpTapJ.Size[0];
		double tw  = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[1] : CSectD.CmpTapJ.Size[1];
		double B1  = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[2] : CSectD.CmpTapJ.Size[2];
		double tf1 = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[3] : CSectD.CmpTapJ.Size[3];
		double B2  = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[4] : CSectD.CmpTapJ.Size[4];
		double tf2 = (bPosI == TRUE) ? CSectD.SectBefore.SectI.Size[5] : CSectD.CmpTapJ.Size[5];

		///////////////////////////////////////////////    
		S_GSEC_POLYGON_LIST aOutTemp;
		aOutTemp.SetSize(4);
		for (int i=0; i<4; i++) aOutTemp[i].aVertex.SetSize(4);

		// draw ///////////////////////////////////////
		double  leng[3], tempx(0.0), tempy(0.0);
		memset(leng, 0, sizeof(leng));

		leng[0] = Bc;
		leng[1] = B1;
		leng[2] = B2;        
		tempx = (leng[0] - leng[1]) * 0.5;
		tempy = tc + Hh;
		aOutTemp[0].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[0].aVertex[1] = S_GSEC_VERTEX(tempx+leng[1], -tempy);
		aOutTemp[0].aVertex[2] = S_GSEC_VERTEX(tempx+leng[1], -(tempy+tf1));
		aOutTemp[0].aVertex[3] = S_GSEC_VERTEX(tempx, -(tempy+tf1));
		
		tempx = (leng[0] - leng[2]) * 0.5;
		tempy = tc + Hh + tf1 + Hw;
		aOutTemp[1].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[1].aVertex[1] = S_GSEC_VERTEX(tempx+leng[2], -tempy);
		aOutTemp[1].aVertex[2] = S_GSEC_VERTEX(tempx+leng[2], -(tempy+tf2));
		aOutTemp[1].aVertex[3] = S_GSEC_VERTEX(tempx, -(tempy+tf2));
		
		tempx = leng[0] * 0.5 - tw * 0.5;
		tempy = tc + Hh + tf1;
		aOutTemp[2].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[2].aVertex[1] = S_GSEC_VERTEX(tempx+tw, -tempy);
		aOutTemp[2].aVertex[2] = S_GSEC_VERTEX(tempx+tw, -(tempy+Hw));
		aOutTemp[2].aVertex[3] = S_GSEC_VERTEX(tempx, -(tempy+Hw));

		tempx = tempy = 0.;
		aOutTemp[3].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[3].aVertex[1] = S_GSEC_VERTEX(tempx+leng[0], -tempy);
		aOutTemp[3].aVertex[2] = S_GSEC_VERTEX(tempx+leng[0], -tc);
		aOutTemp[3].aVertex[3] = S_GSEC_VERTEX(tempx, -tc);

		// 기존데이터 생성완료 -> 변환시작
		aOuter.SetSize(2);
		aOuter[0].aVertex.SetSize(12);
		aOuter[0].aVertex[0]  = aOutTemp[0].aVertex[0];
		aOuter[0].aVertex[1]  = aOutTemp[0].aVertex[3];
		aOuter[0].aVertex[2]  = aOutTemp[2].aVertex[0];
		aOuter[0].aVertex[3]  = aOutTemp[2].aVertex[3];
		aOuter[0].aVertex[4]  = aOutTemp[1].aVertex[0];
		aOuter[0].aVertex[5]  = aOutTemp[1].aVertex[3];
		aOuter[0].aVertex[6]  = aOutTemp[1].aVertex[2];
		aOuter[0].aVertex[7]  = aOutTemp[1].aVertex[1];
		aOuter[0].aVertex[8]  = aOutTemp[2].aVertex[2];
		aOuter[0].aVertex[9]  = aOutTemp[2].aVertex[1];
		aOuter[0].aVertex[10] = aOutTemp[0].aVertex[2];
		aOuter[0].aVertex[11] = aOutTemp[0].aVertex[1];

		aOuter[1].aVertex.Copy(aOutTemp[3].aVertex);
		///////////////////////////////////////////////    
	}
	else if (nSTYPE == D_SECT_TYPE_COMPO_CI || nSTYPE == D_SECT_TYPE_COMPO_CT || nSTYPE == D_SECT_TYPE_COMPO_PC)
	{
		CArray<T_SECT_POLYGON, T_SECT_POLYGON&>aPolygon1,aPolygon2,aPolygon3,aPolygon4;
		double FrameLocalVector[3][3]={1.,0.,0., 0.,1.,0., 0.,0.,1.};
		double xyz[3]={0.,0.,0.};

		pDoc->m_pSecShape->makeCompOutLine(&CSectD, FrameLocalVector, xyz, aPolygon1, aPolygon2, aPolygon3, aPolygon4, (bPosI==FALSE));

		if (nSTYPE == D_SECT_TYPE_COMPO_CI)
		{
			aOuter.SetSize(2);
			int i(0), nSize = aPolygon1.GetSize();   
			ASSERT(nSize == 32);
			if (nSize != 32) return;
			aOuter[0].aVertex.SetSize(30);
			for (i=0; i<15; i++)
			{
				aOuter[0].aVertex[i].dX = aPolygon1[i].dCoor[1];
				aOuter[0].aVertex[i].dY = aPolygon1[i].dCoor[2];

				aOuter[0].aVertex[i+15].dX = aPolygon1[i+16].dCoor[1];
				aOuter[0].aVertex[i+15].dY = aPolygon1[i+16].dCoor[2];
			}
			nSize = aPolygon2.GetSize();
			ASSERT(nSize == 4);
			if (nSize != 4) return;
			aOuter[1].aVertex.SetSize(nSize);
			for (i=0; i<nSize; i++)
			{
				aOuter[1].aVertex[i].dX = aPolygon2[i].dCoor[1];
				aOuter[1].aVertex[i].dY = aPolygon2[i].dCoor[2];
			}
		}
		else if (nSTYPE == D_SECT_TYPE_COMPO_CT)
		{
			aOuter.SetSize(2);
			int i(0), nSize = aPolygon1.GetSize(); 
			ASSERT(nSize == 26);
			if (nSize != 26) return;      
			aOuter[0].aVertex.SetSize(24);
			for (i=0; i<12; i++)
			{
				aOuter[0].aVertex[i].dX = aPolygon1[i].dCoor[1];
				aOuter[0].aVertex[i].dY = aPolygon1[i].dCoor[2];

				aOuter[0].aVertex[i+12].dX = aPolygon1[i+13].dCoor[1];
				aOuter[0].aVertex[i+12].dY = aPolygon1[i+13].dCoor[2];
			}
			nSize = aPolygon2.GetSize();
			ASSERT(nSize == 4);
			if (nSize != 4) return;      
			aOuter[1].aVertex.SetSize(nSize);
			for (i=0; i<nSize; i++)
			{
				aOuter[1].aVertex[i].dX = aPolygon2[i].dCoor[1];
				aOuter[1].aVertex[i].dY = aPolygon2[i].dCoor[2];
			}
		}
		else if (nSTYPE == D_SECT_TYPE_COMPO_PC)
		{
// 			aOuter.SetSize(2);
// 			int i(0), nSize = aPolygon1.GetSize(); 
// 			aOuter[0].aVertex.SetSize(nSize);
// 			// [Check before Release...!!] cjjeong-20120821 : 
// 			// 우선 Outer 만 처리한다. 정보를 셋팅해서 위에 있는 PSC함수를 불러서 Inner를 불러와야 하는데
// 			// 도심이 달라진걸 보완해야 한다. 가능한가??
// 			for (i=0; i<nSize; i++)
// 			{
// 				aOuter[0].aVertex[i].dX = aPolygon1[i].dCoor[1];
// 				aOuter[0].aVertex[i].dY = aPolygon1[i].dCoor[2];        
// 			}
// 			nSize = aPolygon2.GetSize();
// 			ASSERT(nSize == 4);
// 			if (nSize != 4) return;      
// 			aOuter[1].aVertex.SetSize(nSize);
// 			for (i=0; i<nSize; i++)
// 			{
// 				aOuter[1].aVertex[i].dX = aPolygon2[i].dCoor[1];
// 				aOuter[1].aVertex[i].dY = aPolygon2[i].dCoor[2];
// 			}

			T_SECT_SECTBASE_D*  pSect = bPosI ? &CSectD.SectBefore.SectI : &CSectD.SectBefore.SectJ;
			int nOut = pSect->aOuterPolygon.GetSize();
			int nIn  = pSect->aInnerPolygon.GetSize();
			aOuter.SetSize(nOut+1);
			aInner.SetSize(nIn);

			double dGapX = pSect->aOuterPolygon[0].aVertex[0].dX - aPolygon1[0].dCoor[1];
			double dGapY = pSect->aOuterPolygon[0].aVertex[0].dY - aPolygon1[0].dCoor[2];

			// OutPolygon - 거더
			int i(0), j(0), nSub(0);
			for (i=0; i<nOut; i++)
			{
				nSub = pSect->aOuterPolygon[i].aVertex.GetSize();
				aOuter[i].aVertex.SetSize(nSub);
				for (j=0; j<nSub; j++)
				{
					aOuter[i].aVertex[j].dX = pSect->aOuterPolygon[i].aVertex[j].dX - dGapX;
					aOuter[i].aVertex[j].dY = pSect->aOuterPolygon[i].aVertex[j].dY - dGapY;
				}
			}

			// OutPolygon - 슬래브
			int nSize = aPolygon2.GetSize();
			ASSERT(nSize == 4);
			if (nSize != 4) return;      
			aOuter[nOut].aVertex.SetSize(nSize);
			for (i=0; i<nSize; i++)
			{
				aOuter[nOut].aVertex[i].dX = aPolygon2[i].dCoor[1];  // 슬래브
				aOuter[nOut].aVertex[i].dY = aPolygon2[i].dCoor[2];
			}

			// InnerPolygon
			for (i=0; i<nIn; i++)
			{
				nSub = pSect->aInnerPolygon[i].aVertex.GetSize();
				aInner[i].aVertex.SetSize(nSub);
				for (int j=0; j<nSub; j++)
				{
					aInner[i].aVertex[j].dX = pSect->aInnerPolygon[i].aVertex[j].dX - dGapX;
					aInner[i].aVertex[j].dY = pSect->aInnerPolygon[i].aVertex[j].dY - dGapY;
				}
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		ASSERT(FALSE);    
	}

#ifdef _DEBUG
	FILE *wfp = _wfopen(_T("D:\\Sec_COMP_OutLine.txt"),_T("wt"));
	
	fwprintf(wfp,_T("*OUTER\n"));
	for(int kk=0; kk<aOuter.GetSize(); kk++)
	{
		for (int kkk=0; kkk<aOuter[kk].aVertex.GetSize(); kkk++)
		{
			fwprintf(wfp,_T(" %d, %d, %g, %g \n"), kk+1, kkk+1, aOuter[kk].aVertex[kkk].dX, aOuter[kk].aVertex[kkk].dY);
		}
		fwprintf(wfp,_T("\n"));
	}
	fwprintf(wfp,_T("\n"));
	fwprintf(wfp,_T("*Inner\n"));
	for(int kk=0; kk<aInner.GetSize(); kk++)
	{
		for (int kkk=0; kkk<aInner[kk].aVertex.GetSize(); kkk++)
		{
			fwprintf(wfp,_T(" %d, %d, %g, %g \n"), kk+1, kkk+1, aInner[kk].aVertex[kkk].dX, aInner[kk].aVertex[kkk].dY);
		}    
		fwprintf(wfp,_T("\n"));
	}
	fclose(wfp);
#endif
}

void CDgnClassIREW::MakeCOMPOutLineCSCS(CDBDoc* pDoc, double SectionSize[D_SECT_SIZE_NUM_MAX], T_SECT_D& CSectD, T_CSCS_D&  CCscsD, S_GSEC_POLYGON_LIST& aOuter, S_GSEC_POLYGON_LIST& aInner, BOOL bTap, BOOL bIncludeJ)
{
	// CCscsD.nCompType 이 0이나 1이어여 한다. 그리고 합성중이어야 한다.
	aOuter.RemoveAll();
	aInner.RemoveAll();

	if (CCscsD.nCompType != -2) { return; }  
	int nPart(0), i(0), j(0); // 1:1, 2:3, 3:1+2, 4:2+3
	CArray<T_CSCS_BASE, T_CSCS_BASE&> aOrgPart;  
	aOrgPart.Copy(CCscsD.aPart);
	if (CCscsD.aPart.GetSize() == 2)
	{
		CCscsD.aPart.RemoveAt(1);
	}

	int nSType = (bTap) ? CSectD.SectBefore.nStype : CSectD.nStype;

	double dSectionSize[2][D_SECT_SIZE_NUM_MAX];
	memset(dSectionSize, 0, sizeof(dSectionSize));
	BOOL bThickness = TRUE;
	BOOL bTriangle  = FALSE;
	double FrameLocalVector[3][3]={1.,0.,0., 0.,1.,0., 0.,0.,1.};
	double xyz[2][3]={0.,0.,0., 0.,0.,0.};
	int rNumPlane;
	CArray<int, int>        aPlaneVCount;
	CArray<int, int>        aPlaneVertexIndex;
	CArray<BOOL, BOOL>      aEdgeFlag;
	CArray<double, double>  aVertex;
	int elnod[D_ELEM_MAXNOD];
	memset(elnod, 0, sizeof(elnod));

	CString Shape;  
	pDoc->m_pAttrCtrl->GetSectionSize(CSectD, Shape, dSectionSize);

	if (nSType == D_SECT_TYPE_COMPO_B)
	{
		/*S_GSEC_POLYGON_LIST aOutTemp, aInTop, aInBot;
		double Bc  = 0.;
		double tc  = 0.;
		double Hh  = 0.;
		double Hw  = SectionSize[0];
		double tw  = SectionSize[1];
		double B1  = SectionSize[2];
		double Bf1 = SectionSize[3];
		double tf1 = SectionSize[4];
		double B2  = SectionSize[5];
		double Bf2 = SectionSize[6];
		double tf2 = SectionSize[7];
		double Hr1 = CSectD.SectBefore.SectJ.Size[2];
		double Hr2 = CSectD.SectBefore.SectJ.Size[3];
		double tr1 = CSectD.SectBefore.SectJ.Size[4];
		double tr2 = CSectD.SectBefore.SectJ.Size[5];
		int    N1  = CSectD.SectBefore.nStiffNum[0];
		int    N2  = CSectD.SectBefore.nStiffNum[1];    
		double  leng[3], tempx(0.0), tempy(0.0), tempx1(0.0), tempy1(0.0);
		memset(leng, 0, sizeof(leng));

		aOutTemp.SetSize(5);
		for (int i=0; i<5; i++) aOutTemp[i].aVertex.SetSize(4);

		leng[0] = Bc;
		leng[1] = Bf1 + B1 + Bf1;
		leng[2] = Bf2 + B2 + Bf2;

		tempx = (leng[0] - leng[1]) * 0.5;
		tempy = tc + Hh;	  
		aOutTemp[0].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[0].aVertex[1] = S_GSEC_VERTEX(tempx+leng[1], -tempy);
		aOutTemp[0].aVertex[2] = S_GSEC_VERTEX(tempx+leng[1], -(tempy+tf1));
		aOutTemp[0].aVertex[3] = S_GSEC_VERTEX(tempx, -(tempy+tf1));

		tempx = (leng[0] - leng[2]) * 0.5;
		tempy = tc + Hh + tf1 + Hw;
		aOutTemp[1].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[1].aVertex[1] = S_GSEC_VERTEX(tempx+leng[2], -tempy);
		aOutTemp[1].aVertex[2] = S_GSEC_VERTEX(tempx+leng[2], -(tempy+tf2));
		aOutTemp[1].aVertex[3] = S_GSEC_VERTEX(tempx, -(tempy+tf2));

		tempx = (leng[0] - leng[1]) * 0.5 + Bf1;
		tempy = tc + Hh + tf1;
		tempx1 = (leng[0] - leng[2]) * 0.5 + Bf2;
		tempy1 = tc + Hh + tf1 + Hw;
		aOutTemp[2].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[2].aVertex[1] = S_GSEC_VERTEX(tempx-tw, -tempy);
		aOutTemp[2].aVertex[2] = S_GSEC_VERTEX(tempx1-tw, -tempy1);
		aOutTemp[2].aVertex[3] = S_GSEC_VERTEX(tempx1, -tempy1);

		tempx = (leng[0] - leng[1]) * 0.5 + Bf1 + B1;
		tempy = tc + Hh + tf1;
		tempx1 = (leng[0] - leng[2]) * 0.5 + Bf2 + B2;
		tempy1 = tc + Hh + tf1 + Hw;
		aOutTemp[3].aVertex[0] = S_GSEC_VERTEX(tempx, -tempy);
		aOutTemp[3].aVertex[1] = S_GSEC_VERTEX(tempx+tw, -tempy);
		aOutTemp[3].aVertex[2] = S_GSEC_VERTEX(tempx1+tw, -tempy1);
		aOutTemp[3].aVertex[3] = S_GSEC_VERTEX(tempx1, -tempy1);

		// Rib ////////////////////////////////////////
		if(B1 > 0)
		{
			// 상단
			double term, ribhw, ribh, x;
			term = B1 / (N1 + 1);
			tempx = (leng[0] - leng[1]) * 0.5 + Bf1;
			tempy = tc + Hh + tf1;
			ribhw = tr1 * 0.5;
			ribh  = Hr1;
			aInTop.SetSize(N1);
			for(i=0; i < N1; i++)
			{
				x = tempx + (term * (i+1));       
				aInTop[i].aVertex.SetSize(4);
				aInTop[i].aVertex[0] = S_GSEC_VERTEX(x-ribhw, -tempy);
				aInTop[i].aVertex[1] = S_GSEC_VERTEX(x+ribhw, -tempy);
				aInTop[i].aVertex[2] = S_GSEC_VERTEX(x+ribhw, -(tempy+ribh));
				aInTop[i].aVertex[3] = S_GSEC_VERTEX(x-ribhw, -(tempy+ribh));        
			}
		}
		if(B2 > 0)
		{
			// 하단
			double term, ribhw, ribh, x;
			term = B2 / (N2 + 1);
			tempx = (leng[0] - leng[2]) * 0.5 + Bf2;
			tempy = tc + Hh + tf1 + Hw;
			ribhw = tr2 * 0.5;
			ribh  = Hr2;
			aInBot.SetSize(N2);
			for(i=0; i < N2; i++)
			{
				x = tempx + (term * (i+1));        
				aInBot[i].aVertex.SetSize(4);
				//aInBot[i].aVertex[0] = S_GSEC_VERTEX(x-ribhw, -tempy);
				//aInBot[i].aVertex[1] = S_GSEC_VERTEX(x+ribhw, -tempy);
				//aInBot[i].aVertex[2] = S_GSEC_VERTEX(x+ribhw, -(tempy+ribh));
				//aInBot[i].aVertex[3] = S_GSEC_VERTEX(x-ribhw, -(tempy+ribh));
				aInBot[i].aVertex[0] = S_GSEC_VERTEX(x-ribhw, -tempy+ribh);
				aInBot[i].aVertex[1] = S_GSEC_VERTEX(x+ribhw, -tempy+ribh);
				aInBot[i].aVertex[2] = S_GSEC_VERTEX(x+ribhw, -tempy);
				aInBot[i].aVertex[3] = S_GSEC_VERTEX(x-ribhw, -tempy);
			}
		}

		///////////////////////////////////////////////
		// 기존DATA 생성완료 -> 변환시작
		aOuter.SetSize(1);
		aOuter[0].aVertex.SetSize(12);    
		// 기본 Inner 4개에다가 Rib 별로 4개    
		// Box 외곽선
		aOuter[0].aVertex[0]  = aOutTemp[0].aVertex[0];
		aOuter[0].aVertex[1]  = aOutTemp[0].aVertex[3];
		aOuter[0].aVertex[2]  = aOutTemp[2].aVertex[0];
		aOuter[0].aVertex[3]  = aOutTemp[2].aVertex[3];
		aOuter[0].aVertex[4]  = aOutTemp[1].aVertex[0];
		aOuter[0].aVertex[5]  = aOutTemp[1].aVertex[3];
		aOuter[0].aVertex[6]  = aOutTemp[1].aVertex[2];
		aOuter[0].aVertex[7]  = aOutTemp[1].aVertex[1];
		aOuter[0].aVertex[8]  = aOutTemp[3].aVertex[2];
		aOuter[0].aVertex[9]  = aOutTemp[3].aVertex[1];
		aOuter[0].aVertex[10] = aOutTemp[0].aVertex[2];
		aOuter[0].aVertex[11] = aOutTemp[0].aVertex[1];

		aInner.SetSize(3);
		aInner[0].aVertex.SetSize(4);
		aInner[0].aVertex[0] = aOutTemp[2].aVertex[1];
		aInner[0].aVertex[1] = aOutTemp[2].aVertex[2];
		aInner[0].aVertex[2] = aOutTemp[3].aVertex[3];
		aInner[0].aVertex[3] = aOutTemp[3].aVertex[0];
		aInner[1].aVertex.SetSize(N1*4);
		for (i=0; i<N1; i++)
		{
			aInner[1].aVertex[i*4+0] = aInTop[i].aVertex[0];
			aInner[1].aVertex[i*4+1] = aInTop[i].aVertex[1];
			aInner[1].aVertex[i*4+2] = aInTop[i].aVertex[2];
			aInner[1].aVertex[i*4+3] = aInTop[i].aVertex[3];
		}
		aInner[2].aVertex.SetSize(N2*4);
		for (i=0; i<N2; i++)
		{      
			aInner[2].aVertex[i*4+0] = aInBot[i].aVertex[0];
			aInner[2].aVertex[i*4+1] = aInBot[i].aVertex[1];
			aInner[2].aVertex[i*4+2] = aInBot[i].aVertex[2];
			aInner[2].aVertex[i*4+3] = aInBot[i].aVertex[3];
		}
		// [Check before Release...!!] cjjeong-20120910 : 
		// 변환완료
		///////////////////////////////////////////////  */
		pDoc->m_pSecShape->makeCP_BShape_CSCS(dSectionSize, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);

		if (aVertex.GetSize() != 32*3)
		{
			ASSERT(FALSE);
			CCscsD.aPart.Copy(aOrgPart);
			return;
		}

		if (bIncludeJ == TRUE)
			aOuter.SetSize(2);
		else
			aOuter.SetSize(1);
		
		aOuter[0].aVertex.SetSize(17);
		for (i=0; i<16; i++)
		{
			aOuter[0].aVertex[i].dX = aVertex[3*i+1];
			aOuter[0].aVertex[i].dY = aVertex[3*i+2];
		}
		aOuter[0].aVertex[16].dX = aVertex[1];
		aOuter[0].aVertex[16].dY = aVertex[2];
		
		if (bIncludeJ == TRUE)
		{
			aOuter[1].aVertex.SetSize(17);
			for (i=0; i<16; i++)
			{
				aOuter[1].aVertex[i].dX = aVertex[3*i+1+16*3];
				aOuter[1].aVertex[i].dY = aVertex[3*i+2+16*3];
			}
			aOuter[1].aVertex[16].dX = aVertex[1+16*3];
			aOuter[1].aVertex[16].dY = aVertex[2+16*3];
		}
	}
	else if (nSType == D_SECT_TYPE_COMPO_I)
	{    
		pDoc->m_pSecShape->makeCP_IShape_CSCS(dSectionSize, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);            

		if (aVertex.GetSize() != 24*3)
		{
			ASSERT(FALSE);
			CCscsD.aPart.Copy(aOrgPart);
			return;
		}

		if (bIncludeJ == TRUE)
			aOuter.SetSize(2);
		else
			aOuter.SetSize(1);

		aOuter[0].aVertex.SetSize(13);
		for (i=0; i<12; i++)
		{
			aOuter[0].aVertex[i].dX = aVertex[3*i+1];
			aOuter[0].aVertex[i].dY = aVertex[3*i+2];
		}
		aOuter[0].aVertex[12].dX = aVertex[1];
		aOuter[0].aVertex[12].dY = aVertex[2];

		if (bIncludeJ == TRUE)
		{
			aOuter[1].aVertex.SetSize(13);
			for (i=0; i<12; i++)
			{
				aOuter[1].aVertex[i].dX = aVertex[3*i+1 + 12*3];
				aOuter[1].aVertex[i].dY = aVertex[3*i+2 + 12*3];
			}
			aOuter[1].aVertex[12].dX = aVertex[1 + 12*3];
			aOuter[1].aVertex[12].dY = aVertex[2 + 12*3];
		}
	}
	else if (nSType == D_SECT_TYPE_COMPO_CI)
	{
		pDoc->m_pSecShape->makeCP_CIShape_CSCS(dSectionSize, CSectD.SectBefore, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);

		if (aVertex.GetSize() != 80*3)
		{
			ASSERT(FALSE);
			CCscsD.aPart.Copy(aOrgPart);
			return;
		}

		if (bIncludeJ == TRUE)
			aOuter.SetSize(2);
		else
			aOuter.SetSize(1);
		
		aOuter[0].aVertex.SetSize(31);
		for (i=0; i<16; i++)
		{
			aOuter[0].aVertex[i].dX = aVertex[3*i+1];
			aOuter[0].aVertex[i].dY = aVertex[3*i+2];
		}
		for (i=0; i<14; i++)
		{
			aOuter[0].aVertex[i+16].dX = aVertex[(34-i)*3+1];
			aOuter[0].aVertex[i+16].dY = aVertex[(34-i)*3+2];
		}
		aOuter[0].aVertex[30].dX = aVertex[1];
		aOuter[0].aVertex[30].dY = aVertex[2];
		
		if (bIncludeJ == TRUE)
		{
			aOuter[1].aVertex.SetSize(31);
			for (i=0; i<16; i++)
			{
				aOuter[1].aVertex[i].dX = aVertex[3*i+1 + 40*3];
				aOuter[1].aVertex[i].dY = aVertex[3*i+2 + 40*3];
			}
			for (i=0; i<14; i++)
			{
				aOuter[1].aVertex[i+16].dX = aVertex[(34-i)*3+1 + 40*3];
				aOuter[1].aVertex[i+16].dY = aVertex[(34-i)*3+2 + 40*3];
			}
			aOuter[1].aVertex[30].dX = aVertex[1 + 40*3];
			aOuter[1].aVertex[30].dY = aVertex[2 + 40*3];
		}

		/*S_GSEC_VERTEXT_LIST  aTemp;
		aTemp.SetSize(35);
		for (i=0; i<35; i++)
		{
			aTemp[i].dX = aVertex[i*3+1];
			aTemp[i].dY = aVertex[i*3+2];
		}

		aOuter.SetSize(1);
		aOuter[0].aVertex.SetSize(30);
		aOuter[0].aVertex[ 0] = aTemp[ 0];
		aOuter[0].aVertex[ 1] = aTemp[ 1];
		aOuter[0].aVertex[ 2] = aTemp[ 2];
		aOuter[0].aVertex[ 3] = aTemp[ 3];
		aOuter[0].aVertex[ 4] = aTemp[ 4];
		aOuter[0].aVertex[ 5] = aTemp[ 5];
		aOuter[0].aVertex[ 6] = aTemp[ 6];
		aOuter[0].aVertex[ 7] = aTemp[ 7];
		aOuter[0].aVertex[ 8] = aTemp[ 8];
		aOuter[0].aVertex[ 9] = aTemp[ 9];
		aOuter[0].aVertex[10] = aTemp[10];
		aOuter[0].aVertex[11] = aTemp[11];
		aOuter[0].aVertex[12] = aTemp[12];
		aOuter[0].aVertex[13] = aTemp[13];
		aOuter[0].aVertex[14] = aTemp[14];
		aOuter[0].aVertex[15] = aTemp[15];
		aOuter[0].aVertex[16] = aTemp[34];
		aOuter[0].aVertex[17] = aTemp[33];
		aOuter[0].aVertex[18] = aTemp[32];
		aOuter[0].aVertex[19] = aTemp[31];
		aOuter[0].aVertex[20] = aTemp[30];
		aOuter[0].aVertex[21] = aTemp[29];
		aOuter[0].aVertex[22] = aTemp[28];
		aOuter[0].aVertex[23] = aTemp[27];
		aOuter[0].aVertex[24] = aTemp[26];
		aOuter[0].aVertex[25] = aTemp[25];
		aOuter[0].aVertex[26] = aTemp[24];
		aOuter[0].aVertex[27] = aTemp[23];
		aOuter[0].aVertex[28] = aTemp[22];
		aOuter[0].aVertex[29] = aTemp[21];*/
	}
	else if (nSType == D_SECT_TYPE_COMPO_CT)
	{
		pDoc->m_pSecShape->makeCP_CTShape_CSCS(dSectionSize, CSectD.SectBefore, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);        

		if (aVertex.GetSize() != 64*3)
		{
			ASSERT(FALSE);
			CCscsD.aPart.Copy(aOrgPart);
			return;
		}
		
		if (bIncludeJ == TRUE)
			aOuter.SetSize(2);
		else
			aOuter.SetSize(1);
		
		aOuter[0].aVertex.SetSize(24);
		for (i=0; i<12; i++)
		{
			aOuter[0].aVertex[i].dX = aVertex[3*i+1];
			aOuter[0].aVertex[i].dY = aVertex[3*i+2];
		}
		for (i=0; i<11; i++)
		{
			aOuter[0].aVertex[i+12].dX = aVertex[(27-i)*3+1];
			aOuter[0].aVertex[i+12].dY = aVertex[(27-i)*3+2];
		}
		aOuter[0].aVertex[23].dX = aVertex[1];
		aOuter[0].aVertex[23].dY = aVertex[2];
		
		if (bIncludeJ == TRUE)
		{
			aOuter[1].aVertex.SetSize(24);
			for (i=0; i<12; i++)
			{
				aOuter[1].aVertex[i].dX = aVertex[3*i+1 + 32*3];
				aOuter[1].aVertex[i].dY = aVertex[3*i+2 + 32*3];
			}
			for (i=0; i<11; i++)
			{
				aOuter[1].aVertex[i+12].dX = aVertex[(27-i)*3+1 + 32*3];
				aOuter[1].aVertex[i+12].dY = aVertex[(27-i)*3+2 + 32*3];
			}
			aOuter[1].aVertex[23].dX = aVertex[1 + 32*3];
			aOuter[1].aVertex[23].dY = aVertex[2 + 32*3];
		}


		/*S_GSEC_VERTEXT_LIST  aTemp;
		aTemp.SetSize(28);
		for (i=0; i<28; i++)
		{
			aTemp[i].dX = aVertex[i*3+1];
			aTemp[i].dY = aVertex[i*3+2];
		}
		
		aOuter.SetSize(1);
		aOuter[0].aVertex.SetSize(24);
		aOuter[0].aVertex[ 0] = aTemp[ 0];
		aOuter[0].aVertex[ 1] = aTemp[ 1];
		aOuter[0].aVertex[ 2] = aTemp[ 2];
		aOuter[0].aVertex[ 3] = aTemp[ 3];
		aOuter[0].aVertex[ 4] = aTemp[ 4];
		aOuter[0].aVertex[ 5] = aTemp[ 5];
		aOuter[0].aVertex[ 6] = aTemp[ 6];
		aOuter[0].aVertex[ 7] = aTemp[ 7];
		aOuter[0].aVertex[ 8] = aTemp[ 8];
		aOuter[0].aVertex[ 9] = aTemp[ 9];
		aOuter[0].aVertex[10] = aTemp[10];
		aOuter[0].aVertex[11] = aTemp[11];
		aOuter[0].aVertex[12] = aTemp[12];    
		aOuter[0].aVertex[13] = aTemp[27];
		aOuter[0].aVertex[14] = aTemp[26];
		aOuter[0].aVertex[15] = aTemp[25];
		aOuter[0].aVertex[16] = aTemp[24];
		aOuter[0].aVertex[17] = aTemp[23];
		aOuter[0].aVertex[18] = aTemp[22];
		aOuter[0].aVertex[19] = aTemp[21];
		aOuter[0].aVertex[20] = aTemp[20];
		aOuter[0].aVertex[21] = aTemp[19];
		aOuter[0].aVertex[22] = aTemp[18];
		aOuter[0].aVertex[23] = aTemp[17];*/
	}
	else if (nSType == D_SECT_TYPE_COMPO_PC)
	{
		if (bTap == TRUE)
		{
			ASSERT(FALSE);
			CCscsD.aPart.Copy(aOrgPart);
			return;
		}

		// D_SECT_TYPE_COMPO_PC 를 쓴 녀석을 찾는다. 왜? ElemKey가 필요해서.. 최선인가?
		T_ELEM_K  ElemK = -1;
		T_ELEM_D  ElemD;
		for (i=0; i<m_aElement.GetSize(); i++)
		{
			if (pDoc->m_pAttrCtrl->GetElem(m_aElement[i], ElemD) == FALSE) continue;
			if (ElemD.elpro == CCscsD.SectKey)
			{
				ElemK = m_aElement[i];
				break;
			}
		}
		if (ElemK == -1) 
		{
			ASSERT(FALSE);
			CCscsD.aPart.Copy(aOrgPart);
			return;
		}
		
		pDoc->m_pSecShape->makeCP_PCShape_CSCS(ElemK, dSectionSize, CSectD.SectBefore, FrameLocalVector, xyz, bThickness, bTriangle, rNumPlane, &aPlaneVCount, &aPlaneVertexIndex, &aEdgeFlag, &aVertex, elnod, NULL, CCscsD);

		int nSizeVertex = aVertex.GetSize();

		int nSizeCal = (nSizeVertex/3.0)/2.0 - 1;

		aOuter.SetSize(1);
		aOuter[0].aVertex.SetSize(nSizeCal);
		for (i=0; i<nSizeCal; i++)
		{
			aOuter[0].aVertex[i].dX = aVertex[i*3+1];
			aOuter[0].aVertex[i].dY = aVertex[i*3+2];
		}
	}
	else if (nSType == D_SECT_TYPE_COMPO_G)
	{
		int nPart = CCscsD.aPart.GetSize();

		for (i=0; i<nPart; i++)
		{
			// outer
			int nStartOuter = CSectD.SectBefore.SectI.aGeneralPart[i].IdxStart[0];
			int nEndOuter = CSectD.SectBefore.SectI.aGeneralPart[i].IdxEnd[0];
			int iOuterNum = nEndOuter-nStartOuter;

			if (iOuterNum > 0)
			{        
				for (int j=nStartOuter; j<nEndOuter; j++)
				{
					int nTemp = CSectD.SectBefore.SectI.aOuterPolygon[j].aVertex.GetSize();
					S_GSEC_POLYGON  TempAdd;
					TempAdd.aVertex.SetSize(nTemp);          
					for (int k=0; k<nTemp; k++)
					{
						TempAdd.aVertex[k].dX = CSectD.SectBefore.SectI.aOuterPolygon[j].aVertex[k].dX;
						TempAdd.aVertex[k].dY = CSectD.SectBefore.SectI.aOuterPolygon[j].aVertex[k].dY;
					}
					aOuter.Add(TempAdd);
				}
			}

			int nStartInner = CSectD.SectBefore.SectI.aGeneralPart[i].IdxStart[1];
			int nEndInner = CSectD.SectBefore.SectI.aGeneralPart[i].IdxEnd[1];
			int iInnerNum = nEndInner-nStartInner;

			if (iInnerNum > 0)
			{        
				for (j=nStartInner; j<nEndInner; j++)
				{
					int nTemp = CSectD.SectBefore.SectI.aInnerPolygon[j].aVertex.GetSize();
					S_GSEC_POLYGON  TempAdd;
					TempAdd.aVertex.SetSize(nTemp);          
					for (int k=0; k<nTemp; k++)
					{
						TempAdd.aVertex[k].dX = CSectD.SectBefore.SectI.aInnerPolygon[j].aVertex[k].dX;
						TempAdd.aVertex[k].dY = CSectD.SectBefore.SectI.aInnerPolygon[j].aVertex[k].dY;
					}
					aInner.Add(TempAdd);          
				}
			}
		}
	}

	CCscsD.aPart.Copy(aOrgPart);

#ifdef _DEBUG
// 	FILE *wfp = _wfopen(_T("C:\\Sec_COMP_CSCS_OutLine.txt"),_T("wt"));
// 	
// 	fwprintf(wfp,_T("*Vertex\n"));
// 	for(int kk=0; kk<aVertex.GetSize()/3; kk++)
// 	{
// 		fwprintf(wfp,_T(" %d, %g, %g, %g \n"), kk+1, aVertex[kk*3], aVertex[kk*3+1], aVertex[kk*3+2]);    
// 	}
// 	fclose(wfp);
// 
// 	wfp = _wfopen(_T("C:\\Sec_COMP_CSCS_OutLine2.txt"),_T("wt"));
// 	fwprintf(wfp,_T("*Vertex\n"));
// 	for(kk=0; kk<aOuter.GetSize(); kk++)    
// 	{
// 		for (int kkk=0; kkk<aOuter[kk].aVertex.GetSize(); kkk++)
// 		{
// 			fwprintf(wfp,_T(" %d, %d, %g, %g, %g \n"), kk+1, kkk+1, 0., aOuter[kk].aVertex[kkk].dX, aOuter[kk].aVertex[kkk].dY);    
// 		}    
// 	}
// 	fclose(wfp);
// 
// 	wfp = _wfopen(_T("C:\\Sec_COMP_CSCS_OutLine3.txt"),_T("wt"));
// 	fwprintf(wfp,_T("*Vertex\n"));
// 	for(kk=0; kk<aInner.GetSize(); kk++)    
// 	{
// 		for (int kkk=0; kkk<aInner[kk].aVertex.GetSize(); kkk++)
// 		{
// 			fwprintf(wfp,_T(" %d, %d, %g, %g, %g \n"), kk+1, kkk+1, 0., aInner[kk].aVertex[kkk].dX, aInner[kk].aVertex[kkk].dY);    
// 		}    
// 	}
// 	fclose(wfp);
#endif

}

void CDgnClassIREW::MakeCOMPGeneralOutLineCSCS(T_SECT_D& CSectD, S_SECT_D& SSectD, T_CSCS_D&  CCscsD, BOOL bIncludeJ)
{
	if (CCscsD.nCompType != 2) return;

	// -1 : 합성조건이 아님.  
	// 0 : 파트가 2개 이면서 서로 다른 단계에서 Active 되는 상태 - 1번 파트만 복사하면 됨.  
	// 1 : 파트가 3개 이면서 1번과 2번, 3번이 다른 단계에서 Active 되는 상태 - 1번으로 1파트 1, 2번으로 1파트 만들어야 함.
	int nCompType = 0;
	int nPartSize = CCscsD.aPart.GetSize();
	
	S_GSEC_POLYGON_LIST aOuter, aInner;

	if (nPartSize == 1)
	{   
		ASSERT(FALSE);
		return;
	}
	else if (nPartSize == 2)
	{
		nCompType = 0;

		if (bIncludeJ == TRUE)
		{
			aOuter.SetSize(2);  aOuter[0].aVertex.RemoveAll();  aOuter[1].aVertex.RemoveAll();
			aInner.SetSize(2);  aInner[0].aVertex.RemoveAll();  aInner[1].aVertex.RemoveAll();
		}
		else
		{
			aOuter.SetSize(1);  aOuter[0].aVertex.RemoveAll();
			aInner.SetSize(1);  aInner[0].aVertex.RemoveAll();
		}
		// 1번 파트의 Outer와 Inner만 복사해준다.
		int i=0, j=0;
		int nStart = CSectD.SectBefore.SectI.aGeneralPart[0].IdxStart[0];
		int nEnd   = CSectD.SectBefore.SectI.aGeneralPart[0].IdxEnd[0];
		if (nEnd - nStart > 0)
		{        
			S_GSEC_VERTEXT_LIST aVertex;
			for (i=nStart; i<nEnd; i++)
			{
				int nSubSize = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex.GetSize();
				aVertex.SetSize(nSubSize);
				for (j=0; j<nSubSize; j++)
				{
					aVertex[j].dX = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX;
					aVertex[j].dY = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY;
				}
				aOuter[0].aVertex.Append(aVertex);
			}
		}

		nStart = CSectD.SectBefore.SectI.aGeneralPart[0].IdxStart[1];
		nEnd   = CSectD.SectBefore.SectI.aGeneralPart[0].IdxEnd[1];
		if (nEnd - nStart > 0)
		{
			S_GSEC_VERTEXT_LIST aVertex;
			for (i=nStart; i<nEnd; i++)
			{
				int nSubSize = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex.GetSize();
				aVertex.SetSize(nSubSize);
				for (j=0; j<nSubSize; j++)
				{
					aVertex[j].dX = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex[j].dX;
					aVertex[j].dY = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex[j].dY;
				}
				aInner[0].aVertex.Append(aVertex);
			}
		}

		if (bIncludeJ == TRUE)
		{
			nStart = CSectD.SectBefore.SectJ.aGeneralPart[0].IdxStart[0];
			nEnd   = CSectD.SectBefore.SectJ.aGeneralPart[0].IdxEnd[0];
			if (nEnd - nStart > 0)
			{
				S_GSEC_VERTEXT_LIST aVertex;
				for (i=nStart; i<nEnd; i++)
				{
					int nSubSize = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex.GetSize();
					aVertex.SetSize(nSubSize);
					for (j=0; j<nSubSize; j++)
					{
						aVertex[j].dX = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex[j].dX;
						aVertex[j].dY = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex[j].dY;
					}
					aOuter[1].aVertex.Append(aVertex);
				}
			}
			
			nStart = CSectD.SectBefore.SectJ.aGeneralPart[0].IdxStart[1];
			nEnd   = CSectD.SectBefore.SectJ.aGeneralPart[0].IdxEnd[1];
			if (nEnd - nStart > 0)
			{
				S_GSEC_VERTEXT_LIST aVertex;          
				for (i=nStart; i<nEnd; i++)
				{
					int nSubSize = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex.GetSize();
					aVertex.SetSize(nSubSize);
					for (j=0; j<nSubSize; j++)
					{
						aVertex[j].dX = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex[j].dX;
						aVertex[j].dY = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex[j].dY;
					}
					aInner[1].aVertex.Append(aVertex);
				}
			}
		}
	}
	else if (nPartSize == 3)
	{
		nCompType = 1;

		if (bIncludeJ == TRUE)
		{
			aOuter.SetSize(4);
			aOuter[0].aVertex.RemoveAll();  aOuter[1].aVertex.RemoveAll();  
			aOuter[2].aVertex.RemoveAll();  aOuter[3].aVertex.RemoveAll();  
			aInner.SetSize(4);
			aInner[0].aVertex.RemoveAll();  aInner[1].aVertex.RemoveAll();
			aInner[2].aVertex.RemoveAll();  aInner[3].aVertex.RemoveAll();
		}
		else
		{
			aOuter.SetSize(2);  aOuter[0].aVertex.RemoveAll();  aOuter[1].aVertex.RemoveAll();
			aInner.SetSize(2);  aInner[0].aVertex.RemoveAll();  aOuter[1].aVertex.RemoveAll();
		}

		int i=0, j=0;
		int nStart = CSectD.SectBefore.SectI.aGeneralPart[0].IdxStart[0];
		int nEnd   = CSectD.SectBefore.SectI.aGeneralPart[0].IdxEnd[0];
		if (nEnd - nStart > 0)
		{        
			S_GSEC_VERTEXT_LIST aVertex;
			for (i=nStart; i<nEnd; i++)
			{
				int nSubSize = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex.GetSize();
				aVertex.SetSize(nSubSize);
				for (j=0; j<nSubSize; j++)
				{
					aVertex[j].dX = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX;
					aVertex[j].dY = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY;
				}
				aOuter[0].aVertex.Append(aVertex);
			}
		}
		
		nStart = CSectD.SectBefore.SectI.aGeneralPart[0].IdxStart[1];
		nEnd   = CSectD.SectBefore.SectI.aGeneralPart[0].IdxEnd[1];
		if (nEnd - nStart > 0)
		{       
			S_GSEC_VERTEXT_LIST aVertex;
			
			for (i=nStart; i<nEnd; i++)
			{
				int nSubSize = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex.GetSize();
				aVertex.SetSize(nSubSize);
				for (j=0; j<nSubSize; j++)
				{
					aVertex[j].dX = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex[j].dX;
					aVertex[j].dY = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex[j].dY;
				}
				aInner[0].aVertex.Append(aVertex);
			}        
		}

		for (int nPart = 0; nPart < 2; nPart++)
		{
			i=0, j=0;
			nStart = CSectD.SectBefore.SectI.aGeneralPart[nPart].IdxStart[0];
			nEnd   = CSectD.SectBefore.SectI.aGeneralPart[nPart].IdxEnd[0];
			if (nEnd - nStart > 0)
			{          
				S_GSEC_VERTEXT_LIST aVertex;
				for (i=nStart; i<nEnd; i++)
				{
					int nSubSize = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex.GetSize();
					aVertex.SetSize(nSubSize);
					for (j=0; j<nSubSize; j++)
					{
						aVertex[j].dX = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX;
						aVertex[j].dY = CSectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY;
					}
					aOuter[1].aVertex.Append(aVertex);
				}
			}
			nStart = CSectD.SectBefore.SectI.aGeneralPart[nPart].IdxStart[1];
			nEnd   = CSectD.SectBefore.SectI.aGeneralPart[nPart].IdxEnd[1];
			if (nEnd - nStart > 0)
			{
				S_GSEC_VERTEXT_LIST aVertex;
				
				for (i=nStart; i<nEnd; i++)
				{
					int nSubSize = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex.GetSize();
					aVertex.SetSize(nSubSize);
					for (j=0; j<nSubSize; j++)
					{
						aVertex[j].dX = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex[j].dX;
						aVertex[j].dY = CSectD.SectBefore.SectI.aInnerPolygon[i].aVertex[j].dY;
					}
					aInner[1].aVertex.Append(aVertex);
				}        
			}
		}

		if (bIncludeJ == TRUE)
		{
			// 1번 파트로 한개 만들어주 1+2 파트로 한개 만들어 준다.
			// 1번 파트의 Outer와 Inner만 복사해준다.
			nStart = CSectD.SectBefore.SectJ.aGeneralPart[0].IdxStart[0];
			nEnd   = CSectD.SectBefore.SectJ.aGeneralPart[0].IdxEnd[0];
			if (nEnd - nStart > 0)
			{        
				S_GSEC_VERTEXT_LIST aVertex;
				for (i=nStart; i<nEnd; i++)
				{
					int nSubSize = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex.GetSize();
					aVertex.SetSize(nSubSize);
					for (j=0; j<nSubSize; j++)
					{
						aVertex[j].dX = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex[j].dX;
						aVertex[j].dY = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex[j].dY;
					}
					aOuter[2].aVertex.Append(aVertex);
				}
			}
			
			nStart = CSectD.SectBefore.SectJ.aGeneralPart[0].IdxStart[1];
			nEnd   = CSectD.SectBefore.SectJ.aGeneralPart[0].IdxEnd[1];
			if (nEnd - nStart > 0)
			{       
				S_GSEC_VERTEXT_LIST aVertex;
				
				for (i=nStart; i<nEnd; i++)
				{
					int nSubSize = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex.GetSize();
					aVertex.SetSize(nSubSize);
					for (j=0; j<nSubSize; j++)
					{
						aVertex[j].dX = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex[j].dX;
						aVertex[j].dY = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex[j].dY;
					}
					aInner[2].aVertex.Append(aVertex);
				}        
			}
						
			for (int nPart = 0; nPart < 2; nPart++)
			{
				i=0, j=0;
				nStart = CSectD.SectBefore.SectJ.aGeneralPart[nPart].IdxStart[0];
				nEnd   = CSectD.SectBefore.SectJ.aGeneralPart[nPart].IdxEnd[0];
				if (nEnd - nStart > 0)
				{          
					S_GSEC_VERTEXT_LIST aVertex;
					for (i=nStart; i<nEnd; i++)
					{
						int nSubSize = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex.GetSize();
						aVertex.SetSize(nSubSize);
						for (j=0; j<nSubSize; j++)
						{
							aVertex[j].dX = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex[j].dX;
							aVertex[j].dY = CSectD.SectBefore.SectJ.aOuterPolygon[i].aVertex[j].dY;
						}
						aOuter[3].aVertex.Append(aVertex);
					}
				}
				nStart = CSectD.SectBefore.SectJ.aGeneralPart[nPart].IdxStart[1];
				nEnd   = CSectD.SectBefore.SectJ.aGeneralPart[nPart].IdxEnd[1];
				if (nEnd - nStart > 0)
				{
					S_GSEC_VERTEXT_LIST aVertex;
					
					for (i=nStart; i<nEnd; i++)
					{
						int nSubSize = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex.GetSize();
						aVertex.SetSize(nSubSize);
						for (j=0; j<nSubSize; j++)
						{
							aVertex[j].dX = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex[j].dX;
							aVertex[j].dY = CSectD.SectBefore.SectJ.aInnerPolygon[i].aVertex[j].dY;
						}
						aInner[3].aVertex.Append(aVertex);
					}        
				}
			}
		}
	}

	int j=0;
	switch(nCompType)
	{
	case -1:  break;
	case 0:          
		{
			SSectD.SectI.aSectPart.SetSize(1);
			if (bIncludeJ == TRUE) SSectD.SectJ.aSectPart.SetSize(1);

			int nOuterSize = aOuter[0].aVertex.GetSize();
			SSectD.SectI.aSectPart[0].aOuterPolygon.SetSize(1);
			SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.SetSize(nOuterSize);
			for (j=0; j<nOuterSize; j++)
			{                
				SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint[j].dX = aOuter[0].aVertex[j].dX;
				SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint[j].dY = aOuter[0].aVertex[j].dY;
			}
			int nInnerSize = aInner[0].aVertex.GetSize();
			SSectD.SectI.aSectPart[0].aInnerPolygon.SetSize(1);
			SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.SetSize(nInnerSize);
			for (j=0; j<nInnerSize; j++)
			{                
				SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint[j].dX = aInner[0].aVertex[j].dX;
				SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint[j].dY = aInner[0].aVertex[j].dY;
			}

			if (bIncludeJ == TRUE)
			{
				nOuterSize = aOuter[1].aVertex.GetSize();
				SSectD.SectJ.aSectPart[0].aOuterPolygon.SetSize(1);
				SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.SetSize(nOuterSize);
				for (j=0; j<nOuterSize; j++)
				{                
					SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dX = aOuter[1].aVertex[j].dX;
					SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dY = aOuter[1].aVertex[j].dY;
				}
				nInnerSize = aInner[1].aVertex.GetSize();
				SSectD.SectJ.aSectPart[0].aInnerPolygon.SetSize(1);
				SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.SetSize(nInnerSize);
				for (j=0; j<nInnerSize; j++)
				{                
					SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dX = aInner[1].aVertex[j].dX;
					SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dY = aInner[1].aVertex[j].dY;
				}
			}
		}
		break;
	case 1:
		{
			SSectD.SectI.aSectPart.SetSize(2);
			if (bIncludeJ == TRUE) SSectD.SectJ.aSectPart.SetSize(2);

			int nOuterSize = aOuter[0].aVertex.GetSize();
			SSectD.SectI.aSectPart[0].aOuterPolygon.SetSize(1);
			SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.SetSize(nOuterSize);
			for (j=0; j<nOuterSize; j++)
			{                
				SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint[j].dX = aOuter[0].aVertex[j].dX;
				SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint[j].dY = aOuter[0].aVertex[j].dY;
			}
			
			nOuterSize = aOuter[1].aVertex.GetSize();
			SSectD.SectI.aSectPart[1].aOuterPolygon.SetSize(1);
			SSectD.SectI.aSectPart[1].aOuterPolygon[0].aPoint.SetSize(nOuterSize);
			for (j=0; j<nOuterSize; j++)
			{                
				SSectD.SectI.aSectPart[1].aOuterPolygon[0].aPoint[j].dX = aOuter[1].aVertex[j].dX;
				SSectD.SectI.aSectPart[1].aOuterPolygon[0].aPoint[j].dY = aOuter[1].aVertex[j].dY;
			}
			
			int nInnerSize = aInner[0].aVertex.GetSize();
			SSectD.SectI.aSectPart[0].aInnerPolygon.SetSize(1);
			SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.SetSize(nInnerSize);
			for (j=0; j<nInnerSize; j++)
			{                
				SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint[j].dX = aInner[0].aVertex[j].dX;
				SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint[j].dY = aInner[0].aVertex[j].dY;
			}
			
			nInnerSize = aInner[1].aVertex.GetSize();
			SSectD.SectI.aSectPart[0].aInnerPolygon.SetSize(1);
			SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.SetSize(nInnerSize);
			for (j=0; j<nInnerSize; j++)
			{                
				SSectD.SectI.aSectPart[1].aInnerPolygon[0].aPoint[j].dX = aInner[1].aVertex[j].dX;
				SSectD.SectI.aSectPart[1].aInnerPolygon[0].aPoint[j].dY = aInner[1].aVertex[j].dY;
			}

			if (bIncludeJ == TRUE)
			{
				int nOuterSize = aOuter[2].aVertex.GetSize();
				SSectD.SectJ.aSectPart[0].aOuterPolygon.SetSize(1);
				SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.SetSize(nOuterSize);
				for (j=0; j<nOuterSize; j++)
				{                
					SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dX = aOuter[2].aVertex[j].dX;
					SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dY = aOuter[2].aVertex[j].dY;
				}
				
				nOuterSize = aOuter[3].aVertex.GetSize();
				SSectD.SectJ.aSectPart[1].aOuterPolygon.SetSize(1);
				SSectD.SectJ.aSectPart[1].aOuterPolygon[0].aPoint.SetSize(nOuterSize);
				for (j=0; j<nOuterSize; j++)
				{                
					SSectD.SectJ.aSectPart[1].aOuterPolygon[0].aPoint[j].dX = aOuter[3].aVertex[j].dX;
					SSectD.SectJ.aSectPart[1].aOuterPolygon[0].aPoint[j].dY = aOuter[3].aVertex[j].dY;
				}
				
				int nInnerSize = aInner[2].aVertex.GetSize();
				SSectD.SectJ.aSectPart[0].aInnerPolygon.SetSize(1);
				SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.SetSize(nInnerSize);
				for (j=0; j<nInnerSize; j++)
				{                
					SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dX = aInner[2].aVertex[j].dX;
					SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dY = aInner[2].aVertex[j].dY;
				}
				
				nInnerSize = aInner[3].aVertex.GetSize();
				SSectD.SectJ.aSectPart[0].aInnerPolygon.SetSize(1);
				SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.SetSize(nInnerSize);
				for (j=0; j<nInnerSize; j++)
				{                
					SSectD.SectJ.aSectPart[1].aInnerPolygon[0].aPoint[j].dX = aInner[3].aVertex[j].dX;
					SSectD.SectJ.aSectPart[1].aInnerPolygon[0].aPoint[j].dY = aInner[3].aVertex[j].dY;
				}
			}
		}            
		break;
	default:  ASSERT(FALSE);  break;
	}
}

void CDgnClassIREW::CopyCompGeneralOutLine(const S_SECT_D& SSectD, S_CSET_D& CsetD, int nCSetPart)
{
	int nSectPart = SSectD.SectI.aSectPart.GetSize();

	int i=0, j=0;

	if (nCSetPart == 1)
	{
		if (nSectPart == 1)
		{
			CsetD.aOPolyI.SetSize(1); CsetD.aOPolyI[0].aVertex.SetSize(1);
			CsetD.aIPolyI.SetSize(1); CsetD.aIPolyI[0].aVertex.SetSize(1);
			CsetD.aOPolyJ.SetSize(1); CsetD.aOPolyJ[0].aVertex.SetSize(1);
			CsetD.aIPolyJ.SetSize(1); CsetD.aIPolyJ[0].aVertex.SetSize(1);            
			
			CsetD.aOPolyI[0].aVertex.SetSize(SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyI[0].aVertex.SetSize(SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyJ[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyJ[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			
			for (j=0; j<SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aOPolyI[0].aVertex[j].dX = SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint[j].dX;
				CsetD.aOPolyI[0].aVertex[j].dY = SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint[j].dY;
			}
			for (j=0; j<SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aIPolyI[0].aVertex[j].dX = SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint[j].dX;
				CsetD.aIPolyI[0].aVertex[j].dY = SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint[j].dY;
			}
			for (j=0; j<SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aOPolyJ[0].aVertex[j].dX = SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dX;
				CsetD.aOPolyJ[0].aVertex[j].dY = SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dY;
			}
			for (j=0; j<SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aIPolyJ[0].aVertex[j].dX = SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dX;
				CsetD.aIPolyJ[0].aVertex[j].dY = SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dY;
			}
			
		}
		else if (nSectPart == 2)
		{
			CsetD.aOPolyI.SetSize(2); CsetD.aOPolyI[0].aVertex.SetSize(1);  CsetD.aOPolyI[1].aVertex.SetSize(1);
			CsetD.aIPolyI.SetSize(2); CsetD.aIPolyI[0].aVertex.SetSize(1);  CsetD.aOPolyI[1].aVertex.SetSize(1);
			CsetD.aOPolyJ.SetSize(2); CsetD.aOPolyJ[0].aVertex.SetSize(1);  CsetD.aOPolyJ[1].aVertex.SetSize(1);
			CsetD.aIPolyJ.SetSize(2); CsetD.aIPolyJ[0].aVertex.SetSize(1);  CsetD.aIPolyJ[1].aVertex.SetSize(1); 
			
			CsetD.aOPolyI[0].aVertex.SetSize(SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyI[0].aVertex.SetSize(SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyJ[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyJ[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyI[1].aVertex.SetSize(SSectD.SectI.aSectPart[1].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyI[1].aVertex.SetSize(SSectD.SectI.aSectPart[1].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyJ[1].aVertex.SetSize(SSectD.SectJ.aSectPart[1].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyJ[1].aVertex.SetSize(SSectD.SectJ.aSectPart[1].aInnerPolygon[0].aPoint.GetSize());
			
			for (i=0; i<2; i++)
			{
				for (j=0; j<SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aOPolyI[i].aVertex[j].dX = SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint[j].dX;
					CsetD.aOPolyI[i].aVertex[j].dY = SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint[j].dY;
				}
				for (j=0; j<SSectD.SectI.aSectPart[i].aInnerPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aIPolyI[i].aVertex[j].dX = SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint[j].dX;
					CsetD.aIPolyI[i].aVertex[j].dY = SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint[j].dY;
				}
				for (j=0; j<SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aOPolyJ[i].aVertex[j].dX = SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint[j].dX;
					CsetD.aOPolyJ[i].aVertex[j].dY = SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint[j].dY;
				}
				for (j=0; j<SSectD.SectJ.aSectPart[i].aInnerPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aIPolyJ[i].aVertex[j].dX = SSectD.SectJ.aSectPart[i].aInnerPolygon[0].aPoint[j].dX;
					CsetD.aIPolyJ[i].aVertex[j].dY = SSectD.SectJ.aSectPart[i].aInnerPolygon[0].aPoint[j].dY;
				}
			}
		}
	}
	else if (nCSetPart == 2)
	{
		if (nSectPart == 1)
		{
			CsetD.aOPolyQ.SetSize(1); CsetD.aOPolyQ[0].aVertex.SetSize(1);
			CsetD.aIPolyQ.SetSize(1); CsetD.aIPolyQ[0].aVertex.SetSize(1);
			CsetD.aOPolyH.SetSize(1); CsetD.aOPolyH[0].aVertex.SetSize(1);
			CsetD.aIPolyH.SetSize(1); CsetD.aIPolyH[0].aVertex.SetSize(1);            
			
			CsetD.aOPolyQ[0].aVertex.SetSize(SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyQ[0].aVertex.SetSize(SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyH[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyH[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			
			for (j=0; j<SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aOPolyQ[0].aVertex[j].dX = SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint[j].dX;
				CsetD.aOPolyQ[0].aVertex[j].dY = SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint[j].dY;
			}
			for (j=0; j<SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aIPolyQ[0].aVertex[j].dX = SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint[j].dX;
				CsetD.aIPolyQ[0].aVertex[j].dY = SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint[j].dY;
			}
			for (j=0; j<SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aOPolyH[0].aVertex[j].dX = SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dX;
				CsetD.aOPolyH[0].aVertex[j].dY = SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dY;
			}
			for (j=0; j<SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aIPolyH[0].aVertex[j].dX = SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dX;
				CsetD.aIPolyH[0].aVertex[j].dY = SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dY;
			}
			
		}
		else if (nSectPart == 2)
		{
			CsetD.aOPolyQ.SetSize(2); CsetD.aOPolyQ[0].aVertex.SetSize(1);  CsetD.aOPolyQ[1].aVertex.SetSize(1);
			CsetD.aIPolyQ.SetSize(2); CsetD.aIPolyQ[0].aVertex.SetSize(1);  CsetD.aOPolyQ[1].aVertex.SetSize(1);
			CsetD.aOPolyH.SetSize(2); CsetD.aOPolyH[0].aVertex.SetSize(1);  CsetD.aOPolyH[1].aVertex.SetSize(1);
			CsetD.aIPolyH.SetSize(2); CsetD.aIPolyH[0].aVertex.SetSize(1);  CsetD.aIPolyH[1].aVertex.SetSize(1); 
			
			CsetD.aOPolyQ[0].aVertex.SetSize(SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyQ[0].aVertex.SetSize(SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyH[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyH[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyQ[1].aVertex.SetSize(SSectD.SectI.aSectPart[1].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyQ[1].aVertex.SetSize(SSectD.SectI.aSectPart[1].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyH[1].aVertex.SetSize(SSectD.SectJ.aSectPart[1].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyH[1].aVertex.SetSize(SSectD.SectJ.aSectPart[1].aInnerPolygon[0].aPoint.GetSize());
			
			for (i=0; i<2; i++)
			{
				for (j=0; j<SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aOPolyQ[i].aVertex[j].dX = SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint[j].dX;
					CsetD.aOPolyQ[i].aVertex[j].dY = SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint[j].dY;
				}
				for (j=0; j<SSectD.SectI.aSectPart[i].aInnerPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aIPolyQ[i].aVertex[j].dX = SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint[j].dX;
					CsetD.aIPolyQ[i].aVertex[j].dY = SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint[j].dY;
				}
				for (j=0; j<SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aOPolyH[i].aVertex[j].dX = SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint[j].dX;
					CsetD.aOPolyH[i].aVertex[j].dY = SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint[j].dY;
				}
				for (j=0; j<SSectD.SectJ.aSectPart[i].aInnerPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aIPolyH[i].aVertex[j].dX = SSectD.SectJ.aSectPart[i].aInnerPolygon[0].aPoint[j].dX;
					CsetD.aIPolyH[i].aVertex[j].dY = SSectD.SectJ.aSectPart[i].aInnerPolygon[0].aPoint[j].dY;
				}
			}
		}
	}
	else if (nCSetPart == 3)
	{
		if (nSectPart == 1)
		{
			CsetD.aOPolyT.SetSize(1); CsetD.aOPolyT[0].aVertex.SetSize(1);
			CsetD.aIPolyT.SetSize(1); CsetD.aIPolyT[0].aVertex.SetSize(1);
			
			CsetD.aOPolyT[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyT[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			
			for (j=0; j<SSectD.SectI.aSectPart[0].aOuterPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aOPolyT[0].aVertex[j].dX = SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dX;
				CsetD.aOPolyT[0].aVertex[j].dY = SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint[j].dY;
			}
			for (j=0; j<SSectD.SectI.aSectPart[0].aInnerPolygon[0].aPoint.GetSize(); j++)
			{
				CsetD.aIPolyT[0].aVertex[j].dX = SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dX;
				CsetD.aIPolyT[0].aVertex[j].dY = SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint[j].dY;
			}      
		}
		else if (nSectPart == 2)
		{
			CsetD.aOPolyT.SetSize(2); CsetD.aOPolyT[0].aVertex.SetSize(1);  CsetD.aOPolyT[1].aVertex.SetSize(1);
			CsetD.aIPolyT.SetSize(2); CsetD.aIPolyT[0].aVertex.SetSize(1);  CsetD.aIPolyT[1].aVertex.SetSize(1);
			
			CsetD.aOPolyT[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyT[0].aVertex.SetSize(SSectD.SectJ.aSectPart[0].aInnerPolygon[0].aPoint.GetSize());
			CsetD.aOPolyT[1].aVertex.SetSize(SSectD.SectJ.aSectPart[1].aOuterPolygon[0].aPoint.GetSize());
			CsetD.aIPolyT[1].aVertex.SetSize(SSectD.SectJ.aSectPart[1].aInnerPolygon[0].aPoint.GetSize());
			
			for (i=0; i<2; i++)
			{
				for (j=0; j<SSectD.SectI.aSectPart[i].aOuterPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aOPolyT[i].aVertex[j].dX = SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint[j].dX;
					CsetD.aOPolyT[i].aVertex[j].dY = SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint[j].dY;
				}
				for (j=0; j<SSectD.SectI.aSectPart[i].aInnerPolygon[0].aPoint.GetSize(); j++)
				{
					CsetD.aIPolyT[i].aVertex[j].dX = SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint[j].dX;
					CsetD.aIPolyT[i].aVertex[j].dY = SSectD.SectJ.aSectPart[i].aOuterPolygon[0].aPoint[j].dY;
				}
			}
		}
	}

}

BOOL CDgnClassIREW::ExportIDEAData(CDgnIDEAArchive& ar, KEYArray& aElement, CString& szMCTPath)
{
	if (aElement.GetSize()<1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if (GenerateData(aElement) == FALSE) return FALSE;

	TRY
	{
		int i(0), nSize(0);
		
		// Basicrmation
		ar.WriteText(_T("%s"), _LSX(_T("* BASIC Information *")));
		if (WriteBASC(ar, szMCTPath) == FALSE) return FALSE;
		ar.endl();
		
		// Unit
		S_UNIT_D  UnitD;
		m_Data.GetUnit(UnitD);
		ar.WriteText(_T("%s \n"), _LSX(* UINT Information *));
		if (WriteUNIT(ar, UnitD) == FALSE) return FALSE;
		ar.endl();

		// Node
		S_NODE_K_LIST aNodeK;
		S_NODE_D_LIST aNodeD;
		ar.WriteText(_T("%s \n"), _LSX(* NODE *));
		nSize = m_Data.GetNode(aNodeK, aNodeD);
		for (i=0; i<nSize; i++)
		{
			if (WriteNODE(ar, aNodeK[i], aNodeD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Element
		S_ELEM_K_LIST aElemK;
		S_ELEM_D_LIST aElemD;
		ar.WriteText(_T("%s \n"), _LSX(* ELEMENT *));
		nSize = m_Data.GetElem(aElemK, aElemD);
		for (i=0; i<nSize; i++)
		{
			if (WriteELEM(ar, aElemK[i], aElemD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Element
		S_MEMB_K_LIST aMembK;
		S_MEMB_D_LIST aMembD;
		ar.WriteText(_T("%s \n"), _LSX(* MEMBER *));
		nSize = m_Data.GetMemb(aMembK, aMembD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMEMB(ar, aMembK[i], aMembD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Material - Steel
		S_MATL_STEEL_K_LIST aMtstK;
		S_MATL_STEEL_D_LIST aMtstD;
		ar.WriteText(_T("%s \n"), _LSX(* Material-Steel *));
		nSize = m_Data.GetMatlST(aMtstK, aMtstD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMTST(ar, aMtstK[i], aMtstD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Material - Concrete
		S_MATL_CONCRETE_K_LIST aMtccK;
		S_MATL_CONCRETE_D_LIST aMtccD;
		ar.WriteText(_T("%s \n"), _LSX(* Material-Concrete *));
		nSize = m_Data.GetMatlCN(aMtccK, aMtccD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMTCC(ar, aMtccK[i], aMtccD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Material - Rebar
		S_MATL_REIF_K_LIST aMtrbK;
		S_MATL_REIF_D_LIST aMtrbD;
		ar.WriteText(_T("%s \n"), _LSX(* Material-Rebar *));
		nSize = m_Data.GetMatlRB(aMtrbK, aMtrbD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMTRB(ar, aMtrbK[i], aMtrbD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Material - Addrm
		S_MATL_ADIF_K_LIST aMtadK;
		S_MATL_ADIF_D_LIST aMtadD;
		ar.WriteText(_T("%s \n"), _LSX(* Material-Add *));
		nSize = m_Data.GetMatlAD(aMtadK, aMtadD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMTAD(ar, aMtadK[i], aMtadD[i]) == FALSE) return FALSE;
		}
		ar.endl();
			
		// Section
		S_SECT_K_LIST aSectK;
		S_SECT_D_LIST aSectD;
		ar.WriteText(_T("%s \n"), _LSX(* Section *));
		nSize = m_Data.GetSect(aSectK, aSectD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSECT(ar, aSectK[i], aSectD[i]) == FALSE) return FALSE;
		}    
		ar.endl();

		// Section for Cross Section
		S_ELST_D_LIST aElstD;
		ar.WriteText(_T("%s \n"), _LSX(* Tapered Section for element *));
		nSize = m_Data.GetElst(aElemK, aSectK, aElstD);
		for (i=0; i<nSize; i++)
		{
			if (WriteELST(ar, aElemK[i], aSectK[i], aElstD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Section for Cross Section
		S_CSET_D_LIST aCsetD;
		S_PART_K_LIST aPartK;
		ar.WriteText(_T("%s \n"), _LSX(* Tapered Section for element use CSCS *));
		nSize = m_Data.GetCset(aElemK, aSectK, aPartK, aCsetD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCSET(ar, aElemK[i], aSectK[i], aPartK[i], aCsetD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// for 2D Element
		S_THIK_K_LIST aThikK;
		S_THIK_D_LIST aThikD;
		ar.WriteText(_T("%s \n"), _LSX(* Thickness *));
		nSize = m_Data.GetThik(aThikK, aThikD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTHIK(ar, aThikK[i], aThikD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Constraint - Support
		S_CONS_K_LIST aConsK;
		S_CONS_D_LIST aConsD;
		ar.WriteText(_T("%s \n"), _LSX(* Constraint - Support *));
		nSize = m_Data.GetCons(aConsK, aConsD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCONS(ar, aConsK[i], aConsD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Load Case
		S_STLD_K_LIST aStldK;
		S_STLD_D_LIST aStldD;
		ar.WriteText(_T("%s \n"), _LSX(* Load Case *));
		nSize = m_Data.GetStld(aStldK, aStldD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSTLD(ar, aStldK[i], aStldD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Load Combination
		S_LCOM_T_LIST aLcomT;
		S_LCOM_K_LIST aLcomK;
		S_LCOM_D_LIST aLcomD;
		ar.WriteText(_T("%s \n"), _LSX(* Load Combination *));
		nSize = m_Data.GetLcom(aLcomT, aLcomK, aLcomD);
		for (i=0; i<nSize; i++)
		{
			if (WriteLCOM(ar, aLcomT[i], aLcomK[i], aLcomD[i]) == FALSE) return FALSE;
		}     
		ar.endl();

		// Unbraced Length(L,Lb)
		S_LENG_K_LIST aLengK;
		S_LENG_D_LIST aLengD;
		ar.WriteText(_T("%s \n"), _LSX(* Unbraced Length(L,Lb) *));
		nSize = m_Data.GetLeng(aLengK, aLengD);
		for (i=0; i<nSize; i++)
		{
			if (WriteLENG(ar, aLengK[i], aLengD[i]) == FALSE) return FALSE;
		}
		ar.endl();
 
		// Effective Length Factor(K)
		S_KFAC_K_LIST aKfacK;
		S_KFAC_D_LIST aKfacD;
		ar.WriteText(_T("%s \n"), _LSX(* Effective Length Factor(K) *));
		nSize = m_Data.GetKfac(aKfacK, aKfacD);
		for (i=0; i<nSize; i++)
		{
			if (WriteKFAC(ar, aKfacK[i], aKfacD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Limiting Slenderness Ratio
		S_LTSR_K_LIST aLtsrK;
		S_LTSR_D_LIST aLtsrD;
		ar.WriteText(_T("%s \n"), _LSX(* Limiting Slenderness Ratio *));
		nSize = m_Data.GetLtsr(aLtsrK, aLtsrD);
		for (i=0; i<nSize; i++)
		{
			if (WriteLTSR(ar, aLtsrK[i], aLtsrD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Equivalent Moment Correction Factor(Cm)
		S_CMFT_K_LIST aCmftK;
		S_CMFT_D_LIST aCmftD;
		ar.WriteText(_T("%s \n"), _LSX(* Equivalent Moment Correction Factor(Cm) *));
		nSize = m_Data.GetCmft(aCmftK, aCmftD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCMFT(ar, aCmftK[i], aCmftD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Moment Magnifier
		S_FMAG_K_LIST aFmagK;
		S_FMAG_D_LIST aFmagD;
		ar.WriteText(_T("%s \n"), _LSX(* Moment Magnifier *));
		nSize = m_Data.GetFmag(aFmagK, aFmagD);
		for (i=0; i<nSize; i++)
		{
			if (WriteFMAG(ar, aFmagK[i], aFmagD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// RC Design Parameters
		S_DCON_D  DConD;
		ar.WriteText(_T("%s \n"), _LSX(* RC Design Parameters *));
		m_Data.GetDcon(DConD);
		if (WriteDCON(ar, DConD) == FALSE) return FALSE;
		ar.endl();

		//Beam Section Data for Design
		S_RDBM_K_LIST aRdbmK;
		S_RDBM_D_LIST aRdbmD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Section Data for Design *));
		nSize = m_Data.GetRdbm(aRdbmK, aRdbmD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRDBM(ar, aRdbmK[i], aRdbmD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Column Section Data for Design
		S_RDCN_K_LIST aRdcnK;
		S_RDCN_D_LIST aRdcnD;
		ar.WriteText(_T("%s \n"), _LSX(* Column Section Data for Design *));
		nSize = m_Data.GetRdcn(aRdcnK, aRdcnD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRDCN(ar, aRdcnK[i], aRdcnD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Beam Section Data for Checking
		S_RCBM_K_LIST aRcbmK;
		S_RCBM_D_LIST aRcbmD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Section Data for Checking *));
		nSize = m_Data.GetRcbm(aRcbmK, aRcbmD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRCBM(ar, aRcbmK[i], aRcbmD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Column Section Data for Checking
		S_RCCN_K_LIST aRccnK;
		S_RCCN_D_LIST aRccnD;
		ar.WriteText(_T("%s \n"), _LSX(* Column Section Data for Checking *));
		nSize = m_Data.GetRccn(aRccnK, aRccnD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRCCN(ar, aRccnK[i], aRccnD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Rebar Input for General Section
		S_REBT_K_LIST aRebtK;
		S_REBT_D_LIST aRebtD;
		ar.WriteText(_T("%s \n"), _LSX(* Rebar Input for General Section *));
		nSize = m_Data.GetRebt(aRebtK, aRebtD);
		for (i=0; i<nSize; i++)
		{
			if (WriteREBT(ar, aRebtK[i], aRebtD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Rebar for PSC
		S_RPSC_K_LIST aRpscK;
		S_RPSC_D_LIST aRpscD;
		ar.WriteText(_T("%s \n"), _LSX(* Rebar for PSC *));
		nSize = m_Data.GetRpsc(aRpscK, aRpscD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRPSC(ar, aRpscK[i], aRpscD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Tendon of Cross section
		S_TDIF_K_LIST aTdifK;
		S_TDIJ_K_LIST aTdIJ;
		S_TDIF_D_LIST aTdifD;
		ar.WriteText(_T("%s \n"), _LSX(* Tendon of Cross section *));
		nSize = m_Data.GetTdif(aTdifK, aTdIJ, aTdifD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTDIF(ar, aTdifK[i], aTdIJ[i], aTdifD[i]) == FALSE) return FALSE;
		}   
		ar.endl();
		
		// for Construction Stage
		ar.WriteText(_T("%s \n"), _LSX(* Construction Stage Load Case *));
		nSize = m_Data.GetCslc(aStldK, aStldD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCSLC(ar, aStldK[i], aStldD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		ar.WriteText(_T("%s \n"), _LSX(* Construction Stage Load Case - Auto Generate *));
		nSize = m_Data.GetClat(aStldK, aStldD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCLAT(ar, aStldK[i], aStldD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_STAG_K_LIST aStagK;
		S_STAG_D_LIST aStagD;
		ar.WriteText(_T("%s \n"), _LSX(* Construction Stage *));
		nSize = m_Data.GetStag(aStagK, aStagD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSTAG(ar, aStagK[i], aStagD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_CSCS_K_LIST aCscsK;
		S_CSCS_D_LIST aCscsD;
		ar.WriteText(_T("%s \n"), _LSX(* Composite Section for Construction Stage *));
		nSize = m_Data.GetCscs(aCscsK, aCscsD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCSCS(ar, aCscsK[i], aCscsD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_GRUP_K_LIST aGrupK;
		S_GRUP_D_LIST aGrupD;
		ar.WriteText(_T("%s \n"), _LSX(* Structure Group *));
		nSize = m_Data.GetGrup(aGrupK, aGrupD);
		for (i=0; i<nSize; i++)
		{
			if (WriteGRUP(ar, aGrupK[i], aGrupD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_BNGR_K_LIST aBngrK; 
		S_BNGR_D_LIST aBngrD; 
		ar.WriteText(_T("%s \n"), _LSX(* Boundary Group *));
		nSize = m_Data.GetBngr(aBngrK, aBngrD);
		for (i=0; i<nSize; i++)
		{
			if (WriteBNGR(ar, aBngrK[i], aBngrD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_LDGR_K_LIST aLdgrK;
		S_LDGR_D_LIST aLdgrD;
		ar.WriteText(_T("%s \n"), _LSX(* Load Group *));
		nSize = m_Data.GetLdgr(aLdgrK, aLdgrD);
		for (i=0; i<nSize; i++)
		{
			if (WriteLDGR(ar, aLdgrK[i], aLdgrD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_TDPF_K_LIST aTdpfK;
		S_STEP_K_LIST aStepK;
		S_TDLS_D_LIST aTdlsD;
		ar.WriteText(_T("%s \n"), _LSX(* TENDON time-Dependent Loss Graph *));
		nSize = m_Data.GetTdls(aTdpfK, aStagK, aStepK, aTdlsD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTDLS(ar, aTdpfK[i], aStagK[i], aStepK[i], aTdlsD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_LDTP_K_LIST aType;
		/*aNodeK.RemoveAll();
		aStldK.RemoveAll();
		S_REAC_D_LIST aReacD;
		ar.WriteText(_T("%s \n"), _LSX(* Reaction *));
		nSize = m_Data.GetReac(aNodeK, aStldK, aType, aReacD);
		for (i=0; i<nSize; i++)
		{
			if (WriteREAC(ar, aNodeK[i], aStldK[i], aType[i], aReacD[i]) == FALSE) return FALSE;
		}
		ar.endl();*/

		S_DISP_D_LIST aDispD;
		ar.WriteText(_T("%s \n"), _LSX(* Displacement *));
		nSize = m_Data.GetDisp(aNodeK, aStldK, aType, aDispD);
		for (i=0; i<nSize; i++)
		{
			if (WriteDISP(ar, aNodeK[i], aStldK[i], aType[i], aDispD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_TFOR_D_LIST aTForD;
		ar.WriteText(_T("%s \n"), _LSX(* Truss force *));
		nSize = m_Data.GetTfor(aElemK, aStldK, aType, aTForD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTFOR(ar, aElemK[i], aStldK[i], aType[i], aTForD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_BFOR_D_LIST aBForD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam force *));
		nSize = m_Data.GetBfor(aElemK, aStldK, aType, aBForD);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFOR(ar, aElemK[i], aStldK[i], aType[i], aBForD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_STRP_DL_LIST aPForD;
		ar.WriteText(_T("%s \n"), _LSX(* Plate force *));
		nSize = m_Data.GetStrp(aElemK, aStldK, aType, aPForD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSTRP(ar, aElemK[i], aStldK[i], aType[i], aPForD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		/*aNodeK.RemoveAll();
		aStldK.RemoveAll();
		aType.RemoveAll();
		S_RECC_D_LIST aReccD;    
		ar.WriteText(_T("%s \n"), _LSX(* Concurrent Reaction *));
		nSize = m_Data.GetRecc(aNodeK, aStldK, aType, aReccD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRECC(ar, aNodeK[i], aStldK[i], aType[i], aReccD[i]) == FALSE) return FALSE;
		}
		ar.endl();*/

		S_DSPC_D_LIST aDspcD;
		ar.WriteText(_T("%s \n"), _LSX(* Concurrent Displacement *));
		nSize = m_Data.GetDspc(aNodeK, aStldK, aType, aDspcD);
		for (i=0; i<nSize; i++)
		{
			if (WriteDSPC(ar, aNodeK[i], aStldK[i], aType[i], aDspcD[i]) == FALSE) throw  FALSE;
		}
		ar.endl();
		
		S_BCFO_B_LIST aBcfoD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Concurrent force *));
		nSize = m_Data.GetBcfo(aElemK, aStldK, aType, aBcfoD);
		for (i=0; i<nSize; i++)
		{
			if (WriteBCFO(ar, aElemK[i], aStldK[i], aType[i], aBcfoD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// for Construction Stage
		S_STAG_K_LIST aStag;
		S_DISP_CSD_LIST aDispCSD;
		ar.WriteText(_T("%s \n"), _LSX(* Displacement - Construction Stage *));
		nSize = m_Data.GetDispCS(aNodeK, aStldK, aStag, aDispCSD);
		for (i=0; i<nSize; i++)
		{
			if (WriteDISPCS(ar, aNodeK[i], aStldK[i], aStag[i], aDispCSD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_TFOR_CSD_LIST aTForCSD;
		ar.WriteText(_T("%s \n"), _LSX(* Truss force - Construction Stage *));
		nSize = m_Data.GetTforCS(aElemK, aStldK, aType, aTForCSD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTFORCS(ar, aElemK[i], aStldK[i], aStag[i], aTForCSD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_BFOR_CSD_LIST aBForCSD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam force - Construction Stage *));
		nSize = m_Data.GetBforCS(aElemK, aStldK, aStag, aBForCSD);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFORCS(ar, aElemK[i], aStldK[i], aStag[i], aBForCSD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_STRP_CSDL_LIST  aPForCSD;
		ar.WriteText(_T("%s \n"), _LSX(* Plate force - Construction Stage *));
		nSize = m_Data.GetStrpCS(aElemK, aStldK, aStag, aPForCSD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSTRPCS(ar, aElemK[i], aStldK[i], aStag[i], aPForCSD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_BFOR_PT_LIST  aBForPT;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Force for Part *));
		nSize = m_Data.GetBforPT(aElemK, aStldK, aType, aBForPT);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFORPT(ar, aElemK[i], aStldK[i], aType[i], aBForPT[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_BFOR_PTCC_LIST  aBForPTCC;
		ar.WriteText(_T("%s \n"), _LSX(* Beam concurrent force for Part *));
		nSize = m_Data.GetBforPTCC(aElemK, aStldK, aType, aBForPTCC);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFORPTCC(ar, aElemK[i], aStldK[i], aType[i], aBForPTCC[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_BFOR_PTCS_LIST  aBForPTCS;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Stage Force for Part *));
		nSize = m_Data.GetBforPTCS(aElemK, aStldK, aStag, aBForPTCS);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFORPTCS(ar, aElemK[i], aStldK[i], aStag[i], aBForPTCS[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
	}
	CATCH (CArchiveException, e)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	END_CATCH
 
	return TRUE;
}

BOOL CDgnClassIREW::GenerateData(KEYArray& aElement)
{
	Initialize();    
		
	if (aElement.GetSize()<1) return FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	if (pDoc == NULL) return FALSE;

	// 시공단계 해석일 경우 POSTCS로 강제셋팅한다.
	/*if (D_IDEARSFILE_VERSION > D_IDEARSFILE_110 && pDoc->m_pPostCtrl->StageAnalysis() == TRUE)
	{
		pDoc->m_pStagCtrl->ChangeCurStagStep(D_FINAL_STAGE_KEY, 0);  // PostCS인 경우 step은 의미 없음
	}*/

	// 사용자가 선택한 요소의 정보를 저장한다.
	MakeArrayData(pDoc, aElement);

	// 실제 Data를 만든다.
	// 먼저 단위계 고정
	int nMapSize[S_IDEA_NUM];
	memset(nMapSize, 0, sizeof(nMapSize));
	nMapSize[S_IDEA_NODE] = m_aNode.GetSize();
	nMapSize[S_IDEA_ELEM] = m_aElement.GetSize();
	nMapSize[S_IDEA_MEMB] = m_aMember.GetSize();
	nMapSize[S_IDEA_MATL] = m_aMaterial.GetSize();
	nMapSize[S_IDEA_MTST] = m_aMaterialST.GetSize();
	nMapSize[S_IDEA_MTCN] = m_aMaterialCN.GetSize();
	nMapSize[S_IDEA_MTRB] = m_aMaterialRB.GetSize();
	nMapSize[S_IDEA_SECT] = m_aSection.GetSize();  
	nMapSize[S_IDEA_ELST] = m_aElemTapered.GetSize();
	nMapSize[S_IDEA_CONS] = m_aBoundary.GetSize();
	nMapSize[S_IDEA_STLD] = m_aStaticLC.GetSize();
	nMapSize[S_IDEA_LCOM] = m_aLoadCombi.GetSize();
	nMapSize[S_IDEA_LENG] = m_aDgnLeng.GetSize();
	nMapSize[S_IDEA_KFAC] = m_aDgnKfac.GetSize();
	nMapSize[S_IDEA_LTSR] = m_aDgnLtsr.GetSize();
	nMapSize[S_IDEA_CMFT] = m_aDgnCmft.GetSize();
	nMapSize[S_IDEA_FMAG] = m_aDgnFmag.GetSize();
	nMapSize[S_IDEA_RDBM] = nMapSize[S_IDEA_SECT];             
	nMapSize[S_IDEA_RDCN] = nMapSize[S_IDEA_SECT];
	nMapSize[S_IDEA_RCBM] = nMapSize[S_IDEA_SECT];
	nMapSize[S_IDEA_RCCN] = nMapSize[S_IDEA_SECT];
	nMapSize[S_IDEA_REBT] = nMapSize[S_IDEA_SECT];
	nMapSize[S_IDEA_RPSC] = nMapSize[S_IDEA_SECT];
	nMapSize[S_IDEA_TDIF] = m_aTendon.GetSize() * 5;
	nMapSize[S_IDEA_REAC] = nMapSize[S_IDEA_NODE]*nMapSize[S_IDEA_STLD] + nMapSize[S_IDEA_NODE]*m_aResponLC.GetSize() + nMapSize[S_IDEA_NODE]*m_aEccentLC.GetSize() + nMapSize[S_IDEA_NODE]*m_aTmHistLC.GetSize();  // Reac = Node*Load
	nMapSize[S_IDEA_DISP] = nMapSize[S_IDEA_NODE]*nMapSize[S_IDEA_STLD] + nMapSize[S_IDEA_NODE]*m_aResponLC.GetSize() + nMapSize[S_IDEA_NODE]*m_aEccentLC.GetSize() + nMapSize[S_IDEA_NODE]*m_aTmHistLC.GetSize();  // Disp = Node*Load
	nMapSize[S_IDEA_TFOR] = m_nTruss*nMapSize[S_IDEA_STLD] + m_nTruss*m_aResponLC.GetSize()   + m_nTruss*m_aEccentLC.GetSize() + m_nTruss*m_aTmHistLC.GetSize();     // TFor = Trss*Load
	nMapSize[S_IDEA_BFOR] = m_nBeam*nMapSize[S_IDEA_STLD] + m_nBeam*m_aResponLC.GetSize()   + m_nBeam*m_aEccentLC.GetSize() +  m_nBeam*m_aTmHistLC.GetSize();    // BFor = Beam*Load
	nMapSize[S_IDEA_RECC] = nMapSize[S_IDEA_NODE]*m_aMovingLC.GetSize() + nMapSize[S_IDEA_NODE]*m_aSettleLC.GetSize();
	nMapSize[S_IDEA_DSPC] = nMapSize[S_IDEA_NODE]*m_aMovingLC.GetSize() + nMapSize[S_IDEA_NODE]*m_aSettleLC.GetSize();
	nMapSize[S_IDEA_BCFO] = m_nBeam*m_aMovingLC.GetSize() + m_nBeam*m_aSettleLC.GetSize();    // BCFO = Beam*Load
	// for Construction Stage
	nMapSize[S_IDEA_CSLC] = m_aCSCaseLC.GetSize();
	nMapSize[S_IDEA_CLAT] = m_aCSCaseAT.GetSize();  
	nMapSize[S_IDEA_STAG] = m_aStage.GetSize();
	nMapSize[S_IDEA_CSCS] = m_aComSecCS.GetSize();
	nMapSize[S_IDEA_EGRP] = m_aStrtGrp.GetSize();
	nMapSize[S_IDEA_BGRP] = m_aBndrGrp.GetSize();
	nMapSize[S_IDEA_LGRP] = m_aLoadGrp.GetSize();  
	nMapSize[S_IDEA_TDLS] = m_aTendonLoss.GetSize();  
	nMapSize[S_IDEA_CSDP] = m_aCSCaseAT.GetSize() * nMapSize[S_IDEA_NODE] * nMapSize[S_IDEA_STAG];
	nMapSize[S_IDEA_CSTF] = m_aCSCaseAT.GetSize() * m_nTruss * nMapSize[S_IDEA_STAG];
	nMapSize[S_IDEA_CSBF] = m_aCSCaseAT.GetSize() * m_nBeam  * nMapSize[S_IDEA_STAG];
	// MNET:XXXX-CJJEONG-20130221 (2088)
	int nELCS = m_aElemCSCS.GetSize();
	nMapSize[S_IDEA_EPTB] = nELCS*nMapSize[S_IDEA_STLD] + nELCS*m_aResponLC.GetSize() + nELCS*m_aEccentLC.GetSize() + nELCS*m_aTmHistLC.GetSize();  // Reac = Node*Load
	nMapSize[S_IDEA_EPTB] = nELCS*m_aMovingLC.GetSize() + nELCS*m_aSettleLC.GetSize();
	nMapSize[S_IDEA_EPCS] = nELCS*m_aCSCaseAT.GetSize()*nMapSize[S_IDEA_STAG];

	m_Data.Initialize();
	m_Data.SetMapSize(nMapSize);
	
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurrentIndex, ChangeIndex;
	// Primarily, Get Current Unit Index.
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrentIndex);
	ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	ChangeIndex.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);


	int i(0), j(0), nSize(0);    
	
	T_NODE_D  CNodeD;
	S_NODE_D  SNodeD;
	nSize = m_aNode.GetSize();;
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetNode(m_aNode[i], CNodeD) == FALSE) return FALSE;
		CD2ID_NODE(CNodeD, SNodeD);
		if (m_Data.AddNode(m_aNode[i], SNodeD) == FALSE) return FALSE;
	}

	T_ELEM_D  CElemD;
	S_ELEM_D  SElemD;
	nSize = m_aElement.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetElem(m_aElement[i], CElemD) == FALSE) return FALSE;
		CD2ID_ELEM(CElemD, SElemD);
		if (m_Data.AddElem(m_aElement[i], SElemD) == FALSE) return FALSE;
	}

	T_MEMB_D  CMembD;
	S_MEMB_D  SMembD;
	nSize = m_aMember.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetMemb(m_aMember[i], CMembD) == FALSE) return FALSE;
		CD2ID_MEMB(CMembD, SMembD);
		if (CMembD.aElemList.GetSize() < 1)
		{
			// 멤바리스트에 왜 요소번호가 없냐? 말이 되냐?
			ASSERT(FALSE);
			continue;
		}
		SMembD.nMbType = pDoc->m_pAttrCtrl->GetMemberType(CMembD.aElemList[0]);
		if (m_Data.AddMemb(m_aMember[i], SMembD) == FALSE) return FALSE;
	}
	
	T_MATD_D        CMatdD;
	S_MATL_STEEL_D  SMtstD;
	nSize = m_aMaterialST.GetSize();
	for (i=0; i<nSize; i++)
	{    
		if (pDoc->m_pAttrCtrl->GetMatlDesign(m_aMaterialST[i], CMatdD) == FALSE) return FALSE;
		if (CMatdD.Type == CCM_TYPE_STEEL || CMatdD.Type == CCM_TYPE_SRC)
		{
			CD2ID_MTST(CMatdD, SMtstD);
			if (m_Data.AddMatlST(m_aMaterialST[i], SMtstD) == FALSE) return FALSE;
		}
	}
	
	S_MATL_CONCRETE_D  SMtccD;
	nSize = m_aMaterialCN.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetMatlDesign(m_aMaterialCN[i], CMatdD) == FALSE) return FALSE;
		if (CMatdD.Type == CCM_TYPE_CONCR || CMatdD.Type == CCM_TYPE_SRC)
		{
			CD2ID_MTCC(CMatdD, SMtccD);
			if (m_Data.AddMatlCN(m_aMaterialCN[i], SMtccD) == FALSE) return FALSE;
		}
	}

	S_MATL_REBAR_INFO SMtriD;
	nSize = m_aMaterialRB.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetMatlDesign(m_aMaterialRB[i], CMatdD) == FALSE) return FALSE;
		CD2ID_MTRI(CMatdD, SMtriD);
		if (m_Data.AddMatlRB(m_aMaterialRB[i], SMtriD) == FALSE) return FALSE;
	}

	T_MATL_D      CMatlD;
	S_MATL_ADIF_D SMtaiD;
	nSize = m_aMaterial.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetMatl(m_aMaterial[i], CMatlD) == FALSE) return FALSE;
		CD2ID_MTAD(CMatlD, SMtaiD);
		if (m_Data.AddMatlAD(m_aMaterial[i], SMtaiD) == FALSE) return FALSE;
	}

	T_SECT_D  CSectD;
	S_SECT_D  SSectD;
	CArray<T_CSCS_K, T_CSCS_K>  aCscsK;
	T_CSCS_D  CscsD;
	nSize = m_aSection.GetSize();

	S_GSEC_POLYGON_LIST arOuter;
	S_GSEC_POLYGON_LIST arInner;
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetSect(m_aSection[i], CSectD) == FALSE) return FALSE;        

		CD2ID_SECT(pDoc, CSectD, SSectD);    

		// PSC, Composite Polygon 정보를 넘긴다.
		if (CSectD.nStype == D_SECT_TYPE_PSC)
		{
			MakePSCOutLine(pDoc, CSectD.SectBefore.SectI.Size, CSectD.SectBefore, arOuter, arInner, TRUE);
			SSectD.SectI.aOuterPolygon.Copy(arOuter);
			SSectD.SectI.aInnerPolygon.Copy(arInner);
		}
		else if (IsCompositeSection(CSectD.nStype, TRUE) == TRUE)
		{
			MakeCOMPOutLine(pDoc, CSectD, arOuter, arInner, FALSE, TRUE);
			SSectD.SectI.aOuterPolygon.Copy(arOuter);
			SSectD.SectI.aInnerPolygon.Copy(arInner);
		}   
		if (pDoc->m_pPostCtrl->StageAnalysis() == TRUE)
		{
			T_CSCS_K  nCurKey = 0;
			aCscsK.RemoveAll();
			pDoc->m_pAttrCtrl->GetCscsKeyList(aCscsK);

			for (j=0; j<aCscsK.GetSize(); j++)
			{
				CscsD.Initialize();
				if (pDoc->m_pAttrCtrl->GetCscs(aCscsK[j], CscsD) == FALSE) continue;
				if (CscsD.SectKey == m_aSection[i])
				{
					nCurKey = aCscsK[j];
					break;
				}
			}
			if (nCurKey!=0 && pDoc->m_pAttrCtrl->GetCscs(nCurKey, CscsD) == TRUE)
			{
				BOOL bCopy = TRUE;
				S_GSEC_POLYGON_LIST aOuter, aInner;

				if (CSectD.nStype == D_SECT_TYPE_PSC)
				{          
					MakePSCOutLineCSCS(pDoc, CSectD.SectBefore.SectI.Size, CSectD, CscsD, aOuter, FALSE);
					// 항상 1이어야 한다.
					ASSERT(aOuter.GetSize() != 1);
				}
				else if (IsCompositeSection(CSectD.nStype, FALSE) == TRUE)
				{
					MakeCOMPOutLineCSCS(pDoc, CSectD.SectBefore.SectI.Size, CSectD, CscsD, aOuter, aInner, FALSE, FALSE);
				}
				else if (IsCompositeGeneralSection(CSectD.nStype))
				{
					MakeCOMPGeneralOutLineCSCS(CSectD, SSectD, CscsD, FALSE);
					bCopy = FALSE;
				}

				if (bCopy)
				{
					SSectD.SectI.aSectPart.SetSize(1);
					int nOutSize = aOuter.GetSize();
					int nInSize  = aInner.GetSize();
					SSectD.SectI.aSectPart[0].aOuterPolygon.SetSize(nOutSize);
					SSectD.SectI.aSectPart[0].aInnerPolygon.SetSize(nInSize);
					for (j=0; j<nOutSize; j++)
					{
						int nSubSize = aOuter[j].aVertex.GetSize();            
						SSectD.SectI.aSectPart[0].aOuterPolygon[j].aPoint.SetSize(nSubSize);
						for (int k=0; k<nSubSize; k++)
						{
							SSectD.SectI.aSectPart[0].aOuterPolygon[j].aPoint[k].dX = aOuter[j].aVertex[k].dX;
							SSectD.SectI.aSectPart[0].aOuterPolygon[j].aPoint[k].dY = aOuter[j].aVertex[k].dY;
						}
					}
					for (j=0; j<nInSize; j++)
					{
						int nSubSize = aInner[j].aVertex.GetSize();            
						SSectD.SectI.aSectPart[0].aInnerPolygon[j].aPoint.SetSize(nSubSize);
						for (int k=0; k<nSubSize; k++)
						{
							SSectD.SectI.aSectPart[0].aInnerPolygon[j].aPoint[k].dX = aInner[j].aVertex[k].dX;
							SSectD.SectI.aSectPart[0].aInnerPolygon[j].aPoint[k].dY = aInner[j].aVertex[k].dY;
						}
					}
				}
			}
		}    
		if (m_Data.AddSect(m_aSection[i], SSectD) == FALSE) return FALSE;
	}
	
	T_TSGR_K  CTsgrK;
	T_TSGR_D  CTsgrD;
	S_ELST_D  SElstD;  
	T_SECT_D  CCalcSectD;
	T_SECT_SECTION_D* pSection = NULL;

	nSize = m_aElemTapered.GetSize();
	for (i=0; i<nSize; i++)
	{
		CElemD.Initialize();
		if (pDoc->m_pAttrCtrl->GetElem(m_aElemTapered[i], CElemD) == FALSE) 
		{
			ASSERT(FALSE);
			continue;
		}

		CSectD.Initialize();  CCalcSectD.Initialize();
		if (pDoc->m_pAttrCtrl->GetSect(CElemD.elpro, CSectD) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}

		CTsgrK = pDoc->m_pAttrCtrl->GetElemTsgrKey(m_aElemTapered[i]);
		if (CTsgrK == 0)
		{
			CTsgrD.Initialize();
			CTsgrD.GroupName.Format(_T("TSGR"));
			CTsgrD.aElemList.Add(m_aElemTapered[i]);
			double dLength = pDoc->calcLAVElem(CElemD);

			// 그룹으로 묶여있지 않으면 계산하지 말고 기존의 I, J단을 쓰자..
			// [Check before Release...!!] cjjeong-20120413 : 근데 Before를 쓰는게 맞나?
			// COMPOSITE I 와 B 는 After - SecShapeBDTL 참조
			if (CSectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_I || CSectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_B)
			{ pSection = &CSectD.SectAfter; }
			else
			{ pSection = &CSectD.SectBefore; }

			CD2ID_STBS(CSectD.SectBefore.nStype, pSection->SectI, SElstD.SectI);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, pSection->SectI.Size, CSectD.SectBefore, arOuter, arInner, TRUE);
				else                                             MakeCOMPOutLine(pDoc, CSectD, arOuter, arInner, TRUE, TRUE);

				SElstD.SectI.aOuterPolygon.Copy(arOuter);  SElstD.SectI.aInnerPolygon.Copy(arInner);
			}
			

			CD2ID_STBS(CSectD.SectBefore.nStype, pSection->SectJ, SElstD.SectJ);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, pSection->SectJ.Size, CSectD.SectBefore, arOuter, arInner, FALSE);
				else                                             MakeCOMPOutLine(pDoc, CSectD, arOuter, arInner, TRUE, FALSE);        
				SElstD.SectJ.aOuterPolygon.Copy(arOuter);  SElstD.SectJ.aInnerPolygon.Copy(arInner);
			}

			// 1/4, 1/2 지점의 강성계산
			if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dLength, dLength*S_ELEMPOS_RQ, dLength*S_ELEMPOS_RH, 2, CCalcSectD) == FALSE)
			{
				ASSERT(FALSE);
				continue;
			}
			CD2ID_STBS(CSectD.SectBefore.nStype, CCalcSectD.SectBefore.SectI, SElstD.SectQ);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, CCalcSectD.SectBefore.SectI.Size, CCalcSectD.SectBefore, arOuter, arInner, TRUE);
				else                                             MakeCOMPOutLine(pDoc, CCalcSectD, arOuter, arInner, TRUE, TRUE);        
				SElstD.SectQ.aOuterPolygon.Copy(arOuter);  SElstD.SectQ.aInnerPolygon.Copy(arInner);
			}
			
			CD2ID_STBS(CSectD.SectBefore.nStype, CCalcSectD.SectBefore.SectJ, SElstD.SectH);  
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, CCalcSectD.SectBefore.SectJ.Size, CCalcSectD.SectBefore, arOuter, arInner, FALSE);
				else                                             MakeCOMPOutLine(pDoc, CCalcSectD, arOuter, arInner, TRUE, FALSE);        
				SElstD.SectH.aOuterPolygon.Copy(arOuter);  SElstD.SectH.aInnerPolygon.Copy(arInner);
			}
			
			// 3/4 지점의 강성계산
			if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dLength, S_ELEMPOS_RI, dLength*S_ELEMPOS_RT, 2, CCalcSectD) == FALSE)
			{
				ASSERT(FALSE);
				continue;
			}
			CD2ID_STBS(CSectD.SectBefore.nStype, CCalcSectD.SectBefore.SectJ, SElstD.SectT);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, CCalcSectD.SectBefore.SectJ.Size, CCalcSectD.SectBefore, arOuter, arInner, FALSE);
				else                                             MakeCOMPOutLine(pDoc, CCalcSectD, arOuter, arInner, TRUE, FALSE);        
				SElstD.SectT.aOuterPolygon.Copy(arOuter);  SElstD.SectT.aInnerPolygon.Copy(arInner);
			}
		}
		else
		{
			if (pDoc->m_pAttrCtrl->GetTsgr(CTsgrK, CTsgrD) == FALSE) 
			{
				ASSERT(FALSE);
				continue;
			}

			// MNET:XXXX-CJJEONG-20120418 (2088)
			// 전체 그룹중에서 나의 위치와 전체 그룹의 길이를 찾는다.
			T_TSGR_INFO TsgrInfo;
			CMap<T_ELEM_K, T_ELEM_K, T_TSGR_INFO, T_TSGR_INFO&> mapTsgrInfo;
			mapTsgrInfo.InitHashTable(1);
			pDoc->CheckAndSortLineElemGroup(CTsgrD.aElemList, mapTsgrInfo);      
			T_ELEM_K  KeyElemTemp;
			VERIFY(mapTsgrInfo.Lookup(m_aElemTapered[i], TsgrInfo));      
			double dPositionI=TsgrInfo.dPositionI;
			double dPositionJ=TsgrInfo.dPositionJ;
			double dMyLength = dPositionJ - dPositionI;
			double dTotalLength(0.);
			POSITION pos=mapTsgrInfo.GetStartPosition();
			while(pos)
			{
				mapTsgrInfo.GetNextAssoc(pos, KeyElemTemp, TsgrInfo);
				dTotalLength+=TsgrInfo.dElemLength;
			}

			// I, J 단의 강성계산
			if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dTotalLength, dPositionI, dPositionJ, 2, CCalcSectD) == FALSE)
			{
				ASSERT(FALSE);
				continue;
			}
			CD2ID_STBS(CSectD.SectBefore.nStype, CCalcSectD.SectBefore.SectI, SElstD.SectI);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, CCalcSectD.SectBefore.SectI.Size, CCalcSectD.SectBefore, arOuter, arInner, TRUE);
				else                                             MakeCOMPOutLine(pDoc, CCalcSectD, arOuter, arInner, TRUE, TRUE);
				
				SElstD.SectI.aOuterPolygon.Copy(arOuter);  SElstD.SectI.aInnerPolygon.Copy(arInner);
			}

			CD2ID_STBS(CSectD.SectBefore.nStype, CCalcSectD.SectBefore.SectJ, SElstD.SectJ);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, CCalcSectD.SectBefore.SectJ.Size, CCalcSectD.SectBefore, arOuter, arInner, FALSE);
				else                                             MakeCOMPOutLine(pDoc, CCalcSectD, arOuter, arInner, TRUE, FALSE);        
				SElstD.SectJ.aOuterPolygon.Copy(arOuter);  SElstD.SectJ.aInnerPolygon.Copy(arInner);
			}


			// 1/4, 1/2 지점의 강성계산
			if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dTotalLength, dPositionI+dMyLength*S_ELEMPOS_RQ, dPositionI+dMyLength*S_ELEMPOS_RH, 2, CCalcSectD) == FALSE)
			{
				ASSERT(FALSE);
				continue;
			}
			CD2ID_STBS(CSectD.SectBefore.nStype, CCalcSectD.SectBefore.SectI, SElstD.SectQ);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, CCalcSectD.SectBefore.SectI.Size, CCalcSectD.SectBefore, arOuter, arInner, TRUE);
				else                                             MakeCOMPOutLine(pDoc, CCalcSectD, arOuter, arInner, TRUE, TRUE);
				SElstD.SectQ.aOuterPolygon.Copy(arOuter);  SElstD.SectQ.aInnerPolygon.Copy(arInner);
			}

			CD2ID_STBS(CSectD.SectBefore.nStype, CCalcSectD.SectBefore.SectJ, SElstD.SectH);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, CCalcSectD.SectBefore.SectJ.Size, CCalcSectD.SectBefore, arOuter, arInner, FALSE);
				else                                             MakeCOMPOutLine(pDoc, CCalcSectD, arOuter, arInner, TRUE, FALSE);
				SElstD.SectH.aOuterPolygon.Copy(arOuter);  SElstD.SectH.aInnerPolygon.Copy(arInner);
			}
			// 3/4 지점의 강성계산
			if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dTotalLength, dPositionI, dPositionI+dMyLength*S_ELEMPOS_RT, 2, CCalcSectD) == FALSE)
			{
				ASSERT(FALSE);
				continue;
			}
			CD2ID_STBS(CSectD.SectBefore.nStype, CCalcSectD.SectBefore.SectJ, SElstD.SectT);
			if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.nStype, TRUE) == TRUE)
			{
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC) MakePSCOutLine(pDoc, CCalcSectD.SectBefore.SectJ.Size, CCalcSectD.SectBefore, arOuter, arInner, FALSE);
				else                                             MakeCOMPOutLine(pDoc, CCalcSectD, arOuter, arInner, TRUE, FALSE);
				SElstD.SectT.aOuterPolygon.Copy(arOuter);  SElstD.SectT.aInnerPolygon.Copy(arInner);
			}
		}
		if (m_Data.AddElst(m_aElemTapered[i], CElemD.elpro, SElstD)== FALSE) return FALSE;
		
		if (pDoc->m_pPostCtrl->StageAnalysis() == TRUE && m_aComSecCS.GetSize() > 0 && 
			 (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC || IsCompositeSection(CSectD.SectBefore.nStype, FALSE)==TRUE))
		{
			// 시공단계 해석을 돌았으면 정보를 함 만들어 보까?
			// 해당섹션에 CSCS가 있는가?
			T_SECT_K  SectK = 0;
			T_CSCS_K  CscsK = 0;
			T_CSCS_D  CscsD;
			for (int nCSCS = 0; nCSCS<m_aComSecCS.GetSize(); nCSCS++)
			{        
				if (pDoc->m_pAttrCtrl->GetCscs(m_aComSecCS[nCSCS], CscsD) == FALSE) continue;
				if (CscsD.SectKey == CElemD.elpro)          
				{
					SectK = CscsD.SectKey;
					CscsK = m_aComSecCS[nCSCS];
					break;
				}
			}
			// CSCS에 할당되지 않은 단면이라면 위의 정보로 충분하다.
			if (SectK == 0) continue;
			// 사용자 지정단면이므로 그냥 그리면 된다. 강성은 알아서 계산하시던지 CSCS에서 받아가시던지..
			if (CscsD.nCompType == -1) continue;

			S_CSET_D  CsetD;
			S_GSEC_POLYGON_LIST aOuter;
			S_GSEC_POLYGON_LIST aInner;
			// 해당요소가 그룹에 할당되지 않았다면..
			if (CTsgrK == 0)
			{
				CTsgrD.Initialize();
				CTsgrD.GroupName.Format(_T("TSGR"));
				CTsgrD.aElemList.Add(m_aElemTapered[i]);
				double dLength = pDoc->calcLAVElem(CElemD);

				// 그룹으로 묶여있지 않으면 계산하지 말고 기존의 I, J단을 쓰자..
				// [Check before Release...!!] cjjeong-20120413 : 근데 Before를 쓰는게 맞나?
				// COMPOSITE I 와 B 는 After - SecShapeBDTL 참조

				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC)
				{
					MakePSCOutLineCSCS(pDoc, CSectD.SectBefore.SectI.Size, CSectD, CscsD, aOuter, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{
						CsetD.aOPolyI.SetSize(1); CsetD.aOPolyI[0].aVertex.Copy(aOuter[0].aVertex);
						CsetD.aOPolyJ.SetSize(1); CsetD.aOPolyJ[0].aVertex.Copy(aOuter[1].aVertex);
					}
				}
				else if (CSectD.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
				{
					//MakeCOMPGeneralOutLineCSCS(CSectD, SSectD, CscsD, TRUE);
					//CopyCompGeneralOutLine(SSectD, CsetD, 1);
				}
				else
				{
					MakeCOMPOutLineCSCS(pDoc, CSectD.SectBefore.SectI.Size, CSectD, CscsD, aOuter, aInner, TRUE, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{
						CsetD.aOPolyI.SetSize(1); CsetD.aOPolyI[0].aVertex.Copy(aOuter[0].aVertex);
						CsetD.aOPolyJ.SetSize(1); CsetD.aOPolyJ[0].aVertex.Copy(aOuter[1].aVertex);
					}
				}

				// 1/4, 1/2 지점의 강성계산
				if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dLength, dLength*S_ELEMPOS_RQ, dLength*S_ELEMPOS_RH, 2, CCalcSectD) == FALSE)
				{
					ASSERT(FALSE);
					continue;
				}

				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC)
				{
					MakePSCOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectI.Size, CCalcSectD, CscsD, aOuter, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{
						CsetD.aOPolyQ.SetSize(1); CsetD.aOPolyI[0].aVertex.Copy(aOuter[0].aVertex);
						CsetD.aOPolyH.SetSize(1); CsetD.aOPolyI[0].aVertex.Copy(aOuter[1].aVertex);
					}          
				}
				else if (CSectD.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
				{
					//MakeCOMPGeneralOutLineCSCS(CSectD, SSectD, CscsD, TRUE);
					//CopyCompGeneralOutLine(SSectD, CsetD, 2);
				}
				else
				{
					MakeCOMPOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectI.Size, CSectD, CscsD, aOuter, aInner, TRUE, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{
						CsetD.aOPolyQ.SetSize(1); CsetD.aOPolyI[0].aVertex.Copy(aOuter[0].aVertex);
						CsetD.aOPolyH.SetSize(1); CsetD.aOPolyI[0].aVertex.Copy(aOuter[1].aVertex);
					}          
				}
			
				// 3/4 지점의 강성계산
				if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dLength, S_ELEMPOS_RI, dLength*S_ELEMPOS_RT, 2, CCalcSectD) == FALSE)
				{
					ASSERT(FALSE);
					continue;
				}

				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC)
				{
					MakePSCOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectJ.Size, CCalcSectD, CscsD, aOuter, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{            
						CsetD.aOPolyT.SetSize(1); CsetD.aOPolyT[0].aVertex.Copy(aOuter[1].aVertex);
					}          
				}
				else if (CSectD.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
				{
					//MakeCOMPGeneralOutLineCSCS(CSectD, SSectD, CscsD, TRUE);
					//CopyCompGeneralOutLine(SSectD, CsetD, 3);
				}
				else
				{
					MakeCOMPOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectJ.Size, CSectD, CscsD, aOuter, aInner, TRUE, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{            
						CsetD.aOPolyT.SetSize(1); CsetD.aOPolyT[0].aVertex.Copy(aOuter[1].aVertex);
					}          
				}
			}
			else
			{
				if (pDoc->m_pAttrCtrl->GetTsgr(CTsgrK, CTsgrD) == FALSE) 
				{
					ASSERT(FALSE);
					continue;
				}

				// MNET:XXXX-CJJEONG-20120418 (2088)
				// 전체 그룹중에서 나의 위치와 전체 그룹의 길이를 찾는다.
				T_TSGR_INFO TsgrInfo;
				CMap<T_ELEM_K, T_ELEM_K, T_TSGR_INFO, T_TSGR_INFO&> mapTsgrInfo;
				mapTsgrInfo.InitHashTable(1);
				pDoc->CheckAndSortLineElemGroup(CTsgrD.aElemList, mapTsgrInfo);      
				T_ELEM_K  KeyElemTemp;
				VERIFY(mapTsgrInfo.Lookup(m_aElemTapered[i], TsgrInfo));      
				double dPositionI=TsgrInfo.dPositionI;
				double dPositionJ=TsgrInfo.dPositionJ;
				double dMyLength = dPositionJ - dPositionI;
				double dTotalLength(0.);
				POSITION pos=mapTsgrInfo.GetStartPosition();
				while(pos)
				{
					mapTsgrInfo.GetNextAssoc(pos, KeyElemTemp, TsgrInfo);
					dTotalLength+=TsgrInfo.dElemLength;
				}

				// I, J 단의 강성계산
				if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dTotalLength, dPositionI, dPositionJ, 2, CCalcSectD) == FALSE)
				{
					ASSERT(FALSE);
					continue;
				}

				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC)
				{
					MakePSCOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectI.Size, CCalcSectD, CscsD, aOuter, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{
						CsetD.aOPolyI.SetSize(1); CsetD.aOPolyI[0].aVertex.Copy(aOuter[0].aVertex);
						CsetD.aOPolyJ.SetSize(1); CsetD.aOPolyJ[0].aVertex.Copy(aOuter[1].aVertex);
					}
				}
				else if (CSectD.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
				{
					//MakeCOMPGeneralOutLineCSCS(CSectD, SSectD, CscsD, TRUE);
					//CopyCompGeneralOutLine(SSectD, CsetD, 1);
				}
				else
				{
					MakeCOMPOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectI.Size, CSectD, CscsD, aOuter, aInner, TRUE, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{
						CsetD.aOPolyI.SetSize(1); CsetD.aOPolyI[0].aVertex.Copy(aOuter[0].aVertex);
						CsetD.aOPolyJ.SetSize(1); CsetD.aOPolyJ[0].aVertex.Copy(aOuter[1].aVertex);
					}
				}

				// 1/4, 1/2 지점의 강성계산
				if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dTotalLength, dPositionI+dMyLength*S_ELEMPOS_RQ, dPositionI+dMyLength*S_ELEMPOS_RH, 2, CCalcSectD) == FALSE)
				{
					ASSERT(FALSE);
					continue;
				}
				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC)
				{
					MakePSCOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectI.Size, CCalcSectD, CscsD, aOuter, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{
						CsetD.aOPolyQ.SetSize(1); CsetD.aOPolyQ[0].aVertex.Copy(aOuter[0].aVertex);
						CsetD.aOPolyH.SetSize(1); CsetD.aOPolyH[0].aVertex.Copy(aOuter[1].aVertex);
					}
				}
				else if (CSectD.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
				{
					//MakeCOMPGeneralOutLineCSCS(CSectD, SSectD, CscsD, TRUE);
					//CopyCompGeneralOutLine(SSectD, CsetD, 2);
				}
				else
				{
					MakeCOMPOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectI.Size, CSectD, CscsD, aOuter, aInner, TRUE, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{
						CsetD.aOPolyQ.SetSize(1); CsetD.aOPolyQ[0].aVertex.Copy(aOuter[0].aVertex);
						CsetD.aOPolyH.SetSize(1); CsetD.aOPolyH[0].aVertex.Copy(aOuter[1].aVertex);
					}
				}
				
				// 3/4 지점의 강성계산
				if (pDoc->m_pSectDB->CalcTaperedSection(CSectD, CTsgrD, dTotalLength, dPositionI, dPositionI+dMyLength*S_ELEMPOS_RT, 2, CCalcSectD) == FALSE)
				{
					ASSERT(FALSE);
					continue;
				}

				if (CSectD.SectBefore.nStype == D_SECT_TYPE_PSC)
				{
					MakePSCOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectJ.Size, CCalcSectD, CscsD, aOuter, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{            
						CsetD.aOPolyT.SetSize(1); CsetD.aOPolyT[0].aVertex.Copy(aOuter[1].aVertex);
					}          
				}
				else if (CSectD.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
				{
					//MakeCOMPGeneralOutLineCSCS(CSectD, SSectD, CscsD, TRUE);
					//CopyCompGeneralOutLine(SSectD, CsetD, 3);
				}
				else
				{
					MakeCOMPOutLineCSCS(pDoc, CCalcSectD.SectBefore.SectJ.Size, CSectD, CscsD, aOuter, aInner, TRUE, TRUE);
					if (aOuter.GetSize() != 2)
					{
						ASSERT(FALSE);
					}
					else
					{            
						CsetD.aOPolyT.SetSize(1); CsetD.aOPolyT[0].aVertex.Copy(aOuter[1].aVertex);
					}          
				}
			}
			
			m_Data.AddCset(m_aElemTapered[i], CElemD.elpro, 1, CsetD);
		}
	}

	// for 2D Element
	T_THIK_D  CThikD;
	S_THIK_D  SThikD;
	nSize = m_aThickness.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetThik(m_aThickness[i], CThikD) == FALSE) return FALSE;
		CD2ID_THIK(CThikD, SThikD);
		if (m_Data.AddThik(m_aThickness[i], SThikD) == FALSE) return FALSE;
	}

	T_CONS_D  CConsD;
	S_CONS_D  SCondD;
	nSize = m_aBoundary.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetConsNode(m_aBoundary[i], CConsD) == FALSE) return FALSE;
		CD2ID_CONS(CConsD, SCondD);
		if (m_Data.AddCons(m_aBoundary[i], SCondD) == FALSE) return FALSE;
	}

	T_STLD_D  CStldD;
	S_STLD_D  SStldD;
	nSize = m_aStaticLC.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetStld(m_aStaticLC[i], CStldD) == FALSE) return FALSE;
		CD2ID_STLD(CStldD, SStldD);
		if (m_Data.AddStld(m_aStaticLC[i], SStldD) == FALSE) return FALSE;
	}   

	T_LCOM_D  CLcomD;
	S_LCOM_D  SLcomD;
	nSize = m_aLoadCombi.GetSize();
	ASSERT(m_aLoadType.GetSize() == m_aLoadCombi.GetSize());
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetLcom(m_aLoadType[i], m_aLoadCombi[i], CLcomD) == FALSE) return FALSE;
		CD2ID_LCOM(pDoc, CLcomD, SLcomD);
		if (m_Data.AddLcom(m_aLoadType[i], m_aLoadCombi[i], SLcomD) == FALSE) return FALSE;
	}

	T_LENG_D  CLengD;
	T_KFAC_D  CKfacD;
	T_LTSR_D  CLtsrD;
	T_CMFT_D  CCmftD;
	T_FMAG_D  CFmagD; 

	S_LENG_D  SLengD;
	S_KFAC_D  SKfacD;
	S_LTSR_D  SLtsrD;
	S_CMFT_D  SCmftD;
	S_FMAG_D  SFmagD;

	nSize = m_aElement.GetSize();
	for (i=0; i<nSize; i++)
	{
		// Unbraced Length(L,Lb)
		if (pDoc->m_pAttrCtrl->GetLeng(m_aElement[i], CLengD) == TRUE)
		{
			CD2ID_LENG(CLengD, SLengD);
			if (m_Data.AddLeng(m_aElement[i], SLengD) == FALSE) return FALSE;
		}

		// Effective Length Factor(K)
		if (pDoc->m_pAttrCtrl->GetKfac(m_aElement[i], CKfacD) == TRUE)
		{
			CD2ID_KFAC(CKfacD, SKfacD);
			if (m_Data.AddKfac(m_aElement[i], SKfacD) == FALSE) return FALSE;
		}

		// Limiting Slenderness Ratio
		if (pDoc->m_pAttrCtrl->GetLtsr(m_aElement[i], CLtsrD) == TRUE)
		{
			CD2ID_LTSR(CLtsrD, SLtsrD);
			if (m_Data.AddLtsr(m_aElement[i], SLtsrD) == FALSE) return FALSE;
		}

		// Equivalent Moment Correction Factor(Cm)
		if (pDoc->m_pAttrCtrl->GetCmft(m_aElement[i], CCmftD) == TRUE)
		{
			CD2ID_CMFT(CCmftD, SCmftD);
			if (m_Data.AddCmft(m_aElement[i], SCmftD) == FALSE) return FALSE;
		}

		// Moment Magnifier
		if (pDoc->m_pAttrCtrl->GetFmag(m_aElement[i], CFmagD) == TRUE)
		{
			CD2ID_FMAG(CFmagD, SFmagD);
			if (m_Data.AddFmag(m_aElement[i], SFmagD) == FALSE) return FALSE;
		}
	}
 
	T_DCON_D  CDConD;
	S_DCON_D  SDconD;
	if (pDoc->m_pAttrCtrl->GetDcon(CDConD) == FALSE)
	{
		CDConD.Initialize();
	}
	CD2ID_DCON(CDConD, SDconD);
	if (m_Data.SetDcon(SDconD) == FALSE) return FALSE;

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	
	
	T_RDGN_D    CRdgnD;  
	T_RCHK_D    CRchkD;
	S_RDGN_BEAM SRdbmD;
	S_RDGN_COLM SRdcnD;
	S_RCHK_BEAM SRcbmD;
	S_RCHK_COLM SRccnD;

	T_REBT_D    CRebtD;
	S_REBT_D    SRebtD;

	T_RPSC_SSM_D  CRpscD;
	S_RPSC_SSM_D  SRpscD;

	nSize = m_aSection.GetSize();
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetRdgn(m_aSection[i], CRdgnD) == TRUE)
		{
			if (CRdgnD.nType == 0)  // BEAM
			{
				CD2ID_RDBM(CRdgnD.BEAM, SRdbmD);
				if (m_Data.AddRdbm(m_aSection[i], SRdbmD) == FALSE) return FALSE;
			}
			else if (CRdgnD.nType == 1) // Column/Brace
			{
				CD2ID_RDCN(CRdgnD.COLM, SRdcnD);
				if (m_Data.AddRdcn(m_aSection[i], SRdcnD) == FALSE) return FALSE;
			}
			else
			{
				ASSERT(FALSE);
			}
		}

		if (pDoc->m_pAttrCtrl->GetRchk(m_aSection[i], CRchkD) == TRUE)
		{
			if (CRchkD.nType == 0) // beam
			{
				CD2ID_RCBM(CRchkD.BEAM, SRcbmD);
				if (m_Data.AddRcbm(m_aSection[i], SRcbmD) == FALSE) return FALSE;
			}
			else if (CRchkD.nType == 1) // Column/Brace
			{
				CD2ID_RCCN(CRchkD.COLM, SRccnD);
				if (m_Data.AddRccn(m_aSection[i], SRccnD) == FALSE) return FALSE;
			}
			else
			{
				ASSERT(FALSE);
			}
		}

		if (pDoc->m_pAttrCtrl->GetRebt(m_aSection[i], CRebtD) == TRUE)
		{
			CD2ID_REBT(CRebtD, SRebtD);
			if (m_Data.AddRebt(m_aSection[i], SRebtD) == FALSE) return FALSE;
		}

		if (pDoc->m_pAttrCtrl2->GetRpscSsm(m_aSection[i], CRpscD) == TRUE)
		{
			CD2ID_RPSD(CRpscD, SRpscD);
			if (m_Data.AddRpsc(m_aSection[i], SRpscD) == FALSE) return FALSE;
		}
	}
	
	UINT nTndnKEY = 0;
	S_TDIF_D  iTdifD;
	CArray<T_GSEC_TNDN, T_GSEC_TNDN&> aTndnData; 
	CArray<T_SMTN_SSM_D, T_SMTN_SSM_D&> aSmtnData; 
	CArray<int, int> arTendonPropID;
	CArray<CString, CString&> arTendonPropName;

	nSize = m_aTendon.GetSize();  
	for (i=0; i<nSize; i++)
	{
		if (pDoc->m_pAttrCtrl->IsTendonAssigned(m_aTendon[i]) == FALSE) continue;
		// I 단의 텐던정보    
		aTndnData.RemoveAll(); aSmtnData.RemoveAll();
		arTendonPropID.RemoveAll(); arTendonPropName.RemoveAll();    
		CSectUtil::Get_SectTendonData(m_aTendon[i], S_ELEMPOS_RI, aTndnData, aSmtnData, arTendonPropID, arTendonPropName);
		
		if (aTndnData.GetSize() > 0)
		{
			iTdifD.Initialize();
			iTdifD.aTndn.SetSize(aTndnData.GetSize());
			for (j=0; j<aTndnData.GetSize(); j++)
			{      
				nTndnKEY = pDoc->m_pAttrCtrl->GetTdnaKey(aSmtnData[j].strProfile);
				CD2ID_TNDN(aTndnData[j], iTdifD.aTndn[j], aSmtnData[j].strProfile, nTndnKEY);
			}
			m_Data.AddTdif(m_aTendon[i], S_ELEMPOS_I, iTdifD);
		}

		// 1/4 지점의 텐던정보
		aTndnData.RemoveAll(); aSmtnData.RemoveAll();
		arTendonPropID.RemoveAll(); arTendonPropName.RemoveAll();    
		CSectUtil::Get_SectTendonData(m_aTendon[i], S_ELEMPOS_RQ, aTndnData, aSmtnData, arTendonPropID, arTendonPropName);
		if (aTndnData.GetSize() > 0)
		{
			iTdifD.Initialize();
			iTdifD.aTndn.SetSize(aTndnData.GetSize());
			for (j=0; j<aTndnData.GetSize(); j++)
			{
				nTndnKEY = pDoc->m_pAttrCtrl->GetTdnaKey(aSmtnData[j].strProfile);
				CD2ID_TNDN(aTndnData[j], iTdifD.aTndn[j], aSmtnData[j].strProfile, nTndnKEY);
			}
			m_Data.AddTdif(m_aTendon[i], S_ELEMPOS_Q, iTdifD);
		}
		
		// 1/2 지점의 텐던정보
		aTndnData.RemoveAll(); aSmtnData.RemoveAll();
		arTendonPropID.RemoveAll(); arTendonPropName.RemoveAll();    
		CSectUtil::Get_SectTendonData(m_aTendon[i], S_ELEMPOS_RH, aTndnData, aSmtnData, arTendonPropID, arTendonPropName);
		if (aTndnData.GetSize() > 0)
		{
			iTdifD.Initialize();
			iTdifD.aTndn.SetSize(aTndnData.GetSize());
			for (j=0; j<aTndnData.GetSize(); j++)
			{
				nTndnKEY = pDoc->m_pAttrCtrl->GetTdnaKey(aSmtnData[j].strProfile);
				CD2ID_TNDN(aTndnData[j], iTdifD.aTndn[j], aSmtnData[j].strProfile, nTndnKEY);
			}
			m_Data.AddTdif(m_aTendon[i], S_ELEMPOS_H, iTdifD);
		}
		
		// 3/4 지점의 텐던정보
		aTndnData.RemoveAll(); aSmtnData.RemoveAll();
		arTendonPropID.RemoveAll(); arTendonPropName.RemoveAll();    
		CSectUtil::Get_SectTendonData(m_aTendon[i], S_ELEMPOS_RT, aTndnData, aSmtnData, arTendonPropID, arTendonPropName);
		if (aTndnData.GetSize() > 0)
		{
			iTdifD.Initialize();
			iTdifD.aTndn.SetSize(aTndnData.GetSize());
			for (j=0; j<aTndnData.GetSize(); j++)
			{
				nTndnKEY = pDoc->m_pAttrCtrl->GetTdnaKey(aSmtnData[j].strProfile);
				CD2ID_TNDN(aTndnData[j], iTdifD.aTndn[j], aSmtnData[j].strProfile, nTndnKEY);
			}
			m_Data.AddTdif(m_aTendon[i], S_ELEMPOS_T, iTdifD);
		}
		
		// J 단의 텐던정보
		aTndnData.RemoveAll(); aSmtnData.RemoveAll();
		arTendonPropID.RemoveAll(); arTendonPropName.RemoveAll();    
		CSectUtil::Get_SectTendonData(m_aTendon[i], S_ELEMPOS_RJ, aTndnData, aSmtnData, arTendonPropID, arTendonPropName);
		if (aTndnData.GetSize() > 0)
		{
			iTdifD.Initialize();
			iTdifD.aTndn.SetSize(aTndnData.GetSize());
			for (j=0; j<aTndnData.GetSize(); j++)
			{
				nTndnKEY = pDoc->m_pAttrCtrl->GetTdnaKey(aSmtnData[j].strProfile);
				CD2ID_TNDN(aTndnData[j], iTdifD.aTndn[j], aSmtnData[j].strProfile, nTndnKEY);
			}
			m_Data.AddTdif(m_aTendon[i], S_ELEMPOS_J, iTdifD);
		}
	}  

	// for Construction Stage  
	if (D_IDEARSFILE_VERSION > D_IDEARSFILE_110 && pDoc->m_pPostCtrl->StageAnalysis() == TRUE)
	{
		// Construnction Stage 해석을 돌렸을 경우에만
		// 필요한 정보를 로딩한다. 다른 경우 괜히 돌 필요 없잖아?    
		CStldD.Initialize();
		SStldD.Initialize();
		nSize = m_aCSCaseLC.GetSize();
		for (i=0; i<nSize; i++)
		{
			if (pDoc->m_pAttrCtrl->GetStld(m_aCSCaseLC[i], CStldD) == TRUE)
			{
				CD2ID_CSLC(CStldD, SStldD);
				m_Data.AddCslc(m_aCSCaseLC[i], SStldD);
			}     
		}

		pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(m_aCSCaseAT);
		nSize = m_aCSCaseAT.GetSize();
		for (i=0; i<nSize; i++)
		{
			if (pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(m_aCSCaseAT[i], CStldD) == TRUE)
			{
				CD2ID_CLAT(CStldD, SStldD);
				m_Data.AddClat(m_aCSCaseAT[i], SStldD);
			}     
		}

		T_STAG_D  CStagD;
		S_STAG_D  SStagD;
		nSize = m_aStage.GetSize();
		for (i=0; i<nSize; i++)
		{
			if (pDoc->m_pAttrCtrl->GetStag(m_aStage[i], CStagD) == TRUE)
			{
				CD2ID_STAG(CStagD, SStagD);
				m_Data.AddStag(m_aStage[i], SStagD);
			}
		}

		T_CSCS_D  CCscsD;
		S_CSCS_D  SCscsD;
		nSize = m_aComSecCS.GetSize();
		for (i=0; i<nSize; i++)
		{
			if (pDoc->m_pAttrCtrl->GetCscs(m_aComSecCS[i], CCscsD) == TRUE)
			{
				CSectD.Initialize();
				SSectD.Initialize();
				CD2ID_CSCS(CCscsD, SCscsD);
				m_Data.AddCscs(m_aComSecCS[i], SCscsD);
//         // 시공단계 합성단면이면서 PSC 단면이라면 각 파트의 정보를 저장해준다.
//         if (m_Data.GetSect(CCscsD.SectKey, SSectD) == TRUE)
//         {
//           if (pDoc->m_pAttrCtrl->GetSect(CCscsD.SectKey, CSectD) == TRUE)
//           {
//             if (SSectD.SectI.nStype == D_SECT_TYPE_PSC)
//             {
//               arOuter.RemoveAll();
//               arInner.RemoveAll();
//               //MakePSCOutLineCSCS(pDoc, CSectD.SectBefore.SectI.Size, CSectD, CCscsD, arOuter, arInner);
//             }
//             else if (SSectD.SectI.nStype == D_SECT_TYPE_COMPO_B  || SSectD.SectI.nStype == D_SECT_TYPE_COMPO_I  || SSectD.SectI.nStype == D_SECT_TYPE_COMPO_CI ||
//                      SSectD.SectI.nStype == D_SECT_TYPE_COMPO_CT || SSectD.SectI.nStype == D_SECT_TYPE_COMPO_PC || SSectD.SectI.nStype == D_SECT_TYPE_COMPO_G )
//             {
//               arOuter.RemoveAll();
//               arInner.RemoveAll();
//               //MakeCOMPOutLineCSCS(pDoc, CSectD.SectBefore.SectI.Size, CSectD, CCscsD, arOuter, arInner, (SSectD.SectI.nStype == D_SECT_TYPE_COMPO_G));
//             }
//           }
//         }
			}
		}

		T_GRUP_D  CGrupD;
		S_GRUP_D  SGrupD;
		nSize = m_aStrtGrp.GetSize();
		for (i=0; i<nSize; i++)
		{
			if (pDoc->m_pAttrCtrl->GetGrup(m_aStrtGrp[i], CGrupD) == TRUE)
			{
				CD2ID_GRUP(CGrupD, SGrupD);
				m_Data.AddGrup(m_aStrtGrp[i], SGrupD);
			}
		}
		
		T_BNGR_D  CBngrD;
		S_BNGR_D  SBngrD;
		nSize = m_aBndrGrp.GetSize();
		for (i=0; i<nSize; i++)
		{
			if (pDoc->m_pAttrCtrl->GetBngr(m_aBndrGrp[i], CBngrD) == TRUE)
			{
				CD2ID_BNGR(CBngrD, SBngrD);
				m_Data.AddBngr(m_aBndrGrp[i], SBngrD);
			}
		}
		
		T_LDGR_D  CLdgrD;
		S_LDGR_D  SLdgrD;
		nSize = m_aLoadGrp.GetSize();
		for (i=0; i<nSize; i++)
		{
			if (pDoc->m_pAttrCtrl->GetLdgr(m_aLoadGrp[i], CLdgrD) == TRUE)
			{
				CD2ID_LDGR(CLdgrD, SLdgrD);
				m_Data.AddLdgr(m_aLoadGrp[i], SLdgrD);
			}
		}

		S_TDLS_D  TdlsD;
		nSize = m_aTendonLoss.GetSize();
		for (i=0; i<nSize; i++)
		{      
			CArray<T_STAG_K,T_STAG_K> aStagKeys;
			pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagKeys);
			
			int nCurSel = 0;
			for(j=0; j< aStagKeys.GetSize(); j++)
			{
				CString Step;        
				T_STAG_STEP_INFO stage_info;
				if(!pDoc->m_pAttrCtrl->GetStagStepInfo(aStagKeys[j], stage_info)) continue;		

				if(stage_info.bSaveResultStep && stage_info.dDurationInfo > 0.0)
				{          
					for(UINT k=0; k<stage_info.aInfo.GetSize(); k++)
					{
						UINT nTKey = k + 1;
						if (MakeTdlsData(pDoc, m_aTendonLoss[i], aStagKeys[j], nTKey, TdlsD) == TRUE)
						{
							m_Data.AddTdls(m_aTendonLoss[i], aStagKeys[j], nTKey, TdlsD);
						}
						else
						{
							ASSERT(FALSE);
						}
					}
				}
			}
		}
	}

	//T_REAC_D  CReacD;
	//S_REAC_D  SReacD;
	T_DISP_D  CDispD;
	S_DISP_D  SDispD;
	T_STRT_D  CTforD;
	S_TFOR_D  STforD;
	T_STRB_D  CBforD;
	S_BFOR_D  SBforD;
	T_SBCF_D  CBcfoD;
	T_SBCF_D  CBcfoD2;
	S_BCFO_B  SBcfoD;
	S_BCFO_B  SBcfoD2;
	T_STRP_DL CStrpD;
	T_STPN_D  CStpnD;
	T_STRM_DL CStrmD;
	S_STRP_DL SStrpD;

	// Static Load Case
	nSize = m_aStaticLC.GetSize();
	T_LCOM_D  LComD;
	for (i=0; i<nSize; i++)
	{
		pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STATIC, m_aStaticLC[i], LComD);
		pDoc->m_pPostCtrl->SelectLoadCombNew(LComD);

		for (j=0; j<m_aNode.GetSize(); j++)
		{      
			/*if (pDoc->m_pPostCtrl->GetReacNew(m_aNode[j], &CReacD, NULL, NULL) == TRUE)
			{
				CD2ID_PREA(CReacD, SReacD);
				if (m_Data.AddReac(m_aNode[j], m_aStaticLC[i], S_IDEA_STLC, SReacD) == FALSE) return FALSE;
			}*/
			if (pDoc->m_pPostCtrl->GetDispNew(m_aNode[j], &CDispD, NULL, NULL) == TRUE)
			{
				CD2ID_PDIS(CDispD, SDispD);
				if (m_Data.AddDisp(m_aNode[j], m_aStaticLC[i], S_IDEA_STLC, SDispD) == FALSE) return FALSE;
			}
		}

		for (j=0; j<m_aElement.GetSize(); j++)
		{
			if (pDoc->m_pAttrCtrl->GetElem(m_aElement[j], CElemD) == FALSE) continue;
			if (CElemD.eltyp == TRUSS_EL)
			{
				if (pDoc->m_pPostCtrl->GetStrtNew(m_aElement[j], &CTforD, NULL, NULL) == TRUE)
				{
					CD2ID_PTFO(CTforD, STforD);
					if (m_Data.AddTfor(m_aElement[j], m_aStaticLC[i], S_IDEA_STLC, STforD) == FALSE) return FALSE;
				}
			}
			else if (CElemD.eltyp == BEAM_EL)
			{
				if (pDoc->m_pPostCtrl->GetStrbNew(m_aElement[j], &CBforD, NULL, NULL, TRUE) == TRUE)
				{
					CD2ID_PBFO(CBforD, SBforD);
					if (m_Data.AddBfor(m_aElement[j], m_aStaticLC[i], S_IDEA_STLC, SBforD) == FALSE) return FALSE;
				}
			}
			else
			{
				// Plane Stress
				if (CElemD.eltyp == PLSTRS_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrmNew_L(m_aElement[j], &CStrmD, NULL, NULL) == TRUE) { CD2ID_STRM(CStrmD, SStrpD); }
					else { continue; }
				}
				// Plane Strain
				else if (CElemD.eltyp == PLSTRN_EL)
				{
					if (pDoc->m_pPostCtrl->GetStpnNew_L(m_aElement[j], &CStpnD, NULL, NULL) == TRUE) CD2ID_STPN(CStpnD, SStrpD);     
					else { continue; }
				}
				// Plate
				else if (CElemD.eltyp == PLATE_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrpNew_L(m_aElement[j], &CStrpD, NULL, NULL) == TRUE) { CD2ID_STRP(CStrpD, SStrpD); }
					else { continue; }
				}
				else
				{
					ASSERT(FALSE);
					continue;
				}
				if (m_Data.AddStrp(m_aElement[j], m_aStaticLC[i], S_IDEA_STLC, SStrpD) == FALSE) return FALSE;
			}
		}

		if (m_aElemCSCS.GetSize() > 0 && m_aElemCSCS.GetSize() == m_aElemCSCSK.GetSize())
		{
			T_CSCS_D  CscsD;
			S_BFOR_PT BForcPT;

			for (j=0; j<m_aElemCSCS.GetSize(); ++j)
			{
				// 파트의 개수를 알아와야지.
				if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
				int nPart = CscsD.aPart.GetSize();
				if (nPart < 1) { ASSERT(FALSE); continue;}
				
				BForcPT.aBFOR.SetSize(nPart);
				for (int i_part=0; i_part < nPart; ++i_part)
				{
					if (pDoc->m_pPostCtrl->GetStrbPartNew(m_aElemCSCS[j], i_part+1, &CBforD, NULL, NULL) == TRUE)
					{
						CD2ID_PBFO(CBforD, BForcPT.aBFOR[i_part]);
					}
				}
				if (m_Data.AddBforPT(m_aElemCSCS[j], m_aStaticLC[i], S_IDEA_STLC, BForcPT) == FALSE) return FALSE;
			}
		}
	}

	nSize = m_aResponLC.GetSize();
	for (i=0; i<nSize; i++)
	{
		pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_SPECTRUM, m_aResponLC[i], LComD);
		pDoc->m_pPostCtrl->SelectLoadCombNew(LComD);
		
		for (j=0; j<m_aNode.GetSize(); j++)
		{      
			/*if (pDoc->m_pPostCtrl->GetReacNew(m_aNode[j], &CReacD, NULL, NULL) == TRUE)
			{
				CD2ID_PREA(CReacD, SReacD);
				if (m_Data.AddReac(m_aNode[j], m_aResponLC[i], S_IDEA_RSLC, SReacD) == FALSE) return FALSE;
			}*/
			if (pDoc->m_pPostCtrl->GetDispNew(m_aNode[j], &CDispD, NULL, NULL) == TRUE)
			{
				CD2ID_PDIS(CDispD, SDispD);
				if (m_Data.AddDisp(m_aNode[j], m_aResponLC[i], S_IDEA_RSLC, SDispD) == FALSE) return FALSE;
			}
		}
		
		for (j=0; j<m_aElement.GetSize(); j++)
		{
			if (pDoc->m_pAttrCtrl->GetElem(m_aElement[j], CElemD) == FALSE) continue;
			if (CElemD.eltyp == TRUSS_EL)
			{
				if (pDoc->m_pPostCtrl->GetStrtNew(m_aElement[j], &CTforD, NULL, NULL) == TRUE)
				{
					CD2ID_PTFO(CTforD, STforD);
					if (m_Data.AddTfor(m_aElement[j], m_aResponLC[i], S_IDEA_RSLC, STforD) == FALSE) return FALSE;
				}
			}
			else if (CElemD.eltyp == BEAM_EL)
			{
				ASSERT(CElemD.eltyp == BEAM_EL);
				if (pDoc->m_pPostCtrl->GetStrbNew(m_aElement[j], &CBforD, NULL, NULL, TRUE) == TRUE)
				{
					CD2ID_PBFO(CBforD, SBforD);
					if (m_Data.AddBfor(m_aElement[j], m_aResponLC[i], S_IDEA_RSLC, SBforD) == FALSE) return FALSE;
				}
			}
			else
			{
				// Plane Stress
				if (CElemD.eltyp == PLSTRS_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrmNew_L(m_aElement[j], &CStrmD, NULL, NULL) == TRUE) { CD2ID_STRM(CStrmD, SStrpD); }
					else { continue; }
				}
				// Plane Strain
				else if (CElemD.eltyp == PLSTRN_EL)
				{
					if (pDoc->m_pPostCtrl->GetStpnNew_L(m_aElement[j], &CStpnD, NULL, NULL) == TRUE) CD2ID_STPN(CStpnD, SStrpD);     
					else { continue; }
				}
				// Plate
				else if (CElemD.eltyp == PLATE_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrpNew_L(m_aElement[j], &CStrpD, NULL, NULL) == TRUE) { CD2ID_STRP(CStrpD, SStrpD); }
					else { continue; }
				}
				else
				{
					ASSERT(FALSE);
					continue;
				}
				if (m_Data.AddStrp(m_aElement[j], m_aResponLC[i], S_IDEA_RSLC, SStrpD) == FALSE) return FALSE;
			}
		}

		if (m_aElemCSCS.GetSize() > 0 && m_aElemCSCS.GetSize() == m_aElemCSCSK.GetSize())
		{
			T_CSCS_D  CscsD;
			S_BFOR_PT BForcPT;
			
			for (j=0; j<m_aElemCSCS.GetSize(); ++j)
			{
				// 파트의 개수를 알아와야지.
				if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
				int nPart = CscsD.aPart.GetSize();
				if (nPart < 1) { ASSERT(FALSE); continue;}
				
				BForcPT.aBFOR.SetSize(nPart);
				for (int i_part=0; i_part < nPart; ++i_part)
				{
					if (pDoc->m_pPostCtrl->GetStrbPartNew(m_aElemCSCS[j], i_part+1, &CBforD, NULL, NULL) == TRUE)
					{
						CD2ID_PBFO(CBforD, BForcPT.aBFOR[i_part]);
					}
				}
				if (m_Data.AddBforPT(m_aElemCSCS[j], m_aResponLC[i], S_IDEA_RSLC, BForcPT) == FALSE) return FALSE;
			}
		}
	}

	nSize = m_aEccentLC.GetSize();
	for (i=0; i<nSize; i++)
	{
		pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_ECCENSPEC, m_aEccentLC[i], LComD);
		pDoc->m_pPostCtrl->SelectLoadCombNew(LComD);
		
		for (j=0; j<m_aNode.GetSize(); j++)
		{      
			/*if (pDoc->m_pPostCtrl->GetReacNew(m_aNode[j], &CReacD, NULL, NULL) == TRUE)
			{
				CD2ID_PREA(CReacD, SReacD);
				if (m_Data.AddReac(m_aNode[j], m_aEccentLC[i], S_IDEA_ERLC, SReacD) == FALSE) return FALSE;
			}*/
			if (pDoc->m_pPostCtrl->GetDispNew(m_aNode[j], &CDispD, NULL, NULL) == TRUE)
			{
				CD2ID_PDIS(CDispD, SDispD);
				if (m_Data.AddDisp(m_aNode[j], m_aEccentLC[i], S_IDEA_ERLC, SDispD) == FALSE) return FALSE;
			}
		}
		
		for (j=0; j<m_aElement.GetSize(); j++)
		{
			if (pDoc->m_pAttrCtrl->GetElem(m_aElement[j], CElemD) == FALSE) continue;
			if (CElemD.eltyp == TRUSS_EL)
			{
				if (pDoc->m_pPostCtrl->GetStrtNew(m_aElement[j], &CTforD, NULL, NULL) == TRUE)
				{
					CD2ID_PTFO(CTforD, STforD);
					if (m_Data.AddTfor(m_aElement[j], m_aEccentLC[i], S_IDEA_ERLC, STforD) == FALSE) return FALSE;
				}
			}
			else if (CElemD.eltyp == BEAM_EL)
			{
				if (pDoc->m_pPostCtrl->GetStrbNew(m_aElement[j], &CBforD, NULL, NULL, TRUE) == TRUE)
				{
					CD2ID_PBFO(CBforD, SBforD);
					if (m_Data.AddBfor(m_aElement[j], m_aEccentLC[i], S_IDEA_ERLC, SBforD) == FALSE) return FALSE;
				}
			}
			else
			{
				// Plane Stress
				if (CElemD.eltyp == PLSTRS_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrmNew_L(m_aElement[j], &CStrmD, NULL, NULL) == TRUE) { CD2ID_STRM(CStrmD, SStrpD); }
					else { continue; }
				}
				// Plane Strain
				else if (CElemD.eltyp == PLSTRN_EL)
				{
					if (pDoc->m_pPostCtrl->GetStpnNew_L(m_aElement[j], &CStpnD, NULL, NULL) == TRUE) CD2ID_STPN(CStpnD, SStrpD);     
					else { continue; }
				}
				// Plate
				else if (CElemD.eltyp == PLATE_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrpNew_L(m_aElement[j], &CStrpD, NULL, NULL) == TRUE) { CD2ID_STRP(CStrpD, SStrpD); }
					else { continue; }
				}
				else
				{
					ASSERT(FALSE);
					continue;
				}
				if (m_Data.AddStrp(m_aElement[j], m_aEccentLC[i], S_IDEA_ERLC, SStrpD) == FALSE) return FALSE; 
			}
		}

		if (m_aElemCSCS.GetSize() > 0 && m_aElemCSCS.GetSize() == m_aElemCSCSK.GetSize())
		{
			T_CSCS_D  CscsD;
			S_BFOR_PT BForcPT;
			
			for (j=0; j<m_aElemCSCS.GetSize(); ++j)
			{
				// 파트의 개수를 알아와야지.
				if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
				int nPart = CscsD.aPart.GetSize();
				if (nPart < 1) { ASSERT(FALSE); continue;}
				
				BForcPT.aBFOR.SetSize(nPart);
				for (int i_part=0; i_part < nPart; ++i_part)
				{
					if (pDoc->m_pPostCtrl->GetStrbPartNew(m_aElemCSCS[j], i_part+1, &CBforD, NULL, NULL) == TRUE)
					{
						CD2ID_PBFO(CBforD, BForcPT.aBFOR[i_part]);
					}
				}
				if (m_Data.AddBforPT(m_aElemCSCS[j], m_aEccentLC[i], S_IDEA_ERLC, BForcPT) == FALSE) return FALSE;
			}
		}
	}

	nSize = m_aTmHistLC.GetSize();
	for (i=0; i<nSize; i++)
	{
		pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, m_aTmHistLC[i], LComD);
		pDoc->m_pPostCtrl->SelectLoadCombNew(LComD);
		
		for (j=0; j<m_aNode.GetSize(); j++)
		{      
			/*if (pDoc->m_pPostCtrl->GetReacNew(m_aNode[j], &CReacD, NULL, NULL) == TRUE)
			{
				CD2ID_PREA(CReacD, SReacD);
				if (m_Data.AddReac(m_aNode[j], m_aTmHistLC[i], S_IDEA_THLC, SReacD) == FALSE) return FALSE;
			}*/
			if (pDoc->m_pPostCtrl->GetDispNew(m_aNode[j], &CDispD, NULL, NULL) == TRUE)
			{
				CD2ID_PDIS(CDispD, SDispD);
				if (m_Data.AddDisp(m_aNode[j], m_aTmHistLC[i], S_IDEA_THLC, SDispD) == FALSE) return FALSE;
			}
		}
		
		for (j=0; j<m_aElement.GetSize(); j++)
		{
			if (pDoc->m_pAttrCtrl->GetElem(m_aElement[j], CElemD) == FALSE) continue;
			if (CElemD.eltyp == TRUSS_EL)
			{
				if (pDoc->m_pPostCtrl->GetStrtNew(m_aElement[j], &CTforD, NULL, NULL) == TRUE)
				{
					CD2ID_PTFO(CTforD, STforD);
					if (m_Data.AddTfor(m_aElement[j], m_aTmHistLC[i], S_IDEA_THLC, STforD) == FALSE) return FALSE;
				}
			}
			else if (CElemD.eltyp == BEAM_EL)
			{
				if (pDoc->m_pPostCtrl->GetStrbNew(m_aElement[j], &CBforD, NULL, NULL, TRUE) == TRUE)
				{
					CD2ID_PBFO(CBforD, SBforD);
					if (m_Data.AddBfor(m_aElement[j], m_aTmHistLC[i], S_IDEA_THLC, SBforD) == FALSE) return FALSE;
				}
			}
			else
			{
				// Plane Stress
				if (CElemD.eltyp == PLSTRS_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrmNew_L(m_aElement[j], &CStrmD, NULL, NULL) == TRUE) { CD2ID_STRM(CStrmD, SStrpD); }
					else { continue; }
				}
				// Plane Strain
				else if (CElemD.eltyp == PLSTRN_EL)
				{
					if (pDoc->m_pPostCtrl->GetStpnNew_L(m_aElement[j], &CStpnD, NULL, NULL) == TRUE) CD2ID_STPN(CStpnD, SStrpD);     
					else { continue; }
				}
				// Plate
				else if (CElemD.eltyp == PLATE_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrpNew_L(m_aElement[j], &CStrpD, NULL, NULL) == TRUE) { CD2ID_STRP(CStrpD, SStrpD); }
					else { continue; }
				}
				else
				{
					ASSERT(FALSE);
					continue;
				}
				if (m_Data.AddStrp(m_aElement[j], m_aTmHistLC[i], S_IDEA_THLC, SStrpD) == FALSE) return FALSE;
			}
		}

		if (m_aElemCSCS.GetSize() > 0 && m_aElemCSCS.GetSize() == m_aElemCSCSK.GetSize())
		{
			T_CSCS_D  CscsD;
			S_BFOR_PT BForcPT;
			
			for (j=0; j<m_aElemCSCS.GetSize(); ++j)
			{
				// 파트의 개수를 알아와야지.
				if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
				int nPart = CscsD.aPart.GetSize();
				if (nPart < 1) { ASSERT(FALSE); continue;}
				
				BForcPT.aBFOR.SetSize(nPart);
				for (int i_part=0; i_part < nPart; ++i_part)
				{
					if (pDoc->m_pPostCtrl->GetStrbPartNew(m_aElemCSCS[j], i_part+1, &CBforD, NULL, NULL) == TRUE)
					{
						CD2ID_PBFO(CBforD, BForcPT.aBFOR[i_part]);
					}
				}
				if (m_Data.AddBforPT(m_aElemCSCS[j], m_aTmHistLC[i], S_IDEA_THLC, BForcPT) == FALSE) return FALSE;
			}
		}
	}
	
	//T_REAC_D  CReacD2;
	//S_RECC_D  SReccD;
	T_DISP_D  CDispD2;
	S_DSPC_D  SDspcD;
	nSize = m_aMovingLC.GetSize();
	for (i=0; i<nSize; i++)
	{
		pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_MOVING, m_aMovingLC[i], LComD);
		pDoc->m_pPostCtrl->SelectLoadCombNew(LComD);

		// mid파일에는 이동하중케이스 key가 SerialNo로 저장되어 있으므로 
		// 아래 결과들에 대한 이동하중케이스 key도 같은 방식으로 저장한다. 
		int nMovingLC = pDoc->m_pPostCtrl->GetSerialMvld(m_aMovingLC[i]); 

		for (j=0; j<m_aNode.GetSize(); j++)
		{      
			/*if (pDoc->m_pPostCtrl->GetReacNew(m_aNode[j], &CReacD, &CReacD2, NULL) == TRUE)
			{
				CD2ID_PREC(CReacD, CReacD2, SReccD);
				if (m_Data.AddRecc(m_aNode[j], nMovingLC, S_IDEA_MLLC, SReccD) == FALSE) return FALSE;
			}*/
			if (pDoc->m_pPostCtrl->GetDispNew(m_aNode[j], &CDispD, &CDispD2, NULL) == TRUE)
			{
				CD2ID_PDIC(CDispD, CDispD2, SDspcD);
				if (m_Data.AddDspc(m_aNode[j], nMovingLC, S_IDEA_MLLC, SDspcD) == FALSE) return FALSE;
			}
		}
		
		for (j=0; j<m_aElement.GetSize(); j++)
		{
			if (pDoc->m_pAttrCtrl->GetElem(m_aElement[j], CElemD) == FALSE) continue;
			if (CElemD.eltyp == TRUSS_EL) continue;

			if (CElemD.eltyp == BEAM_EL)
			{
				if (pDoc->m_pPostCtrl->GetSbcfNew(m_aElement[j], &CBcfoD, &CBcfoD2, NULL, TRUE) == TRUE)
				{
					CD2ID_PBCF(CBcfoD, CBcfoD2, SBcfoD);
					if (m_Data.AddBcfo(m_aElement[j], nMovingLC, S_IDEA_MLLC, SBcfoD) == FALSE) return FALSE;
				}
			}
			else
			{
				// Plane Stress
				if (CElemD.eltyp == PLSTRS_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrmNew_L(m_aElement[j], &CStrmD, NULL, NULL) == TRUE) { CD2ID_STRM(CStrmD, SStrpD); }
					else { continue; }
				}
				// Plane Strain
				else if (CElemD.eltyp == PLSTRN_EL)
				{
					if (pDoc->m_pPostCtrl->GetStpnNew_L(m_aElement[j], &CStpnD, NULL, NULL) == TRUE) CD2ID_STPN(CStpnD, SStrpD);     
					else { continue; }
				}
				// Plate
				else if (CElemD.eltyp == PLATE_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrpNew_L(m_aElement[j], &CStrpD, NULL, NULL) == TRUE) { CD2ID_STRP(CStrpD, SStrpD); }
					else { continue; }
				}
				else
				{
					ASSERT(FALSE);
					continue;
				}
				if (m_Data.AddStrp(m_aElement[j], nMovingLC, S_IDEA_MLLC, SStrpD) == FALSE) return FALSE;        
			}
		}

		if (m_aElemCSCS.GetSize() > 0 && m_aElemCSCS.GetSize() == m_aElemCSCSK.GetSize())
		{
			T_CSCS_D    CscsD;
			/*S_BFOR_PTCC BForcPTCC;      
			for (j=0; j<m_aElemCSCS.GetSize(); ++j)
			{
				// 파트의 개수를 알아와야지.
				if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
				int nPart = CscsD.aPart.GetSize();
				if (nPart < 1) { ASSERT(FALSE); continue;}
				
				BForcPTCC.aBFOR.SetSize(nPart);
				for (int i_part=0; i_part < nPart; ++i_part)
				{
					if (pDoc->m_pPostCtrl->GetSbcfPartNew(m_aElemCSCS[j], i_part+1, &CBcfoD, &CBcfoD2, NULL) == TRUE)
					{
						CD2ID_PBCF(CBcfoD, CBcfoD2, BForcPTCC.aBFOR[i_part]);
					}
				}
				if (m_Data.AddBforPTCC(m_aElemCSCS[j], nMovingLC, S_IDEA_MLLC, BForcPTCC) == FALSE) return FALSE;
			} */     
			S_BFOR_PT BForcPT;
			
			for (j=0; j<m_aElemCSCS.GetSize(); ++j)
			{
				// 파트의 개수를 알아와야지.
				if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
				int nPart = CscsD.aPart.GetSize();
				if (nPart < 1) { ASSERT(FALSE); continue;}
				
				BForcPT.aBFOR.SetSize(nPart);
				for (int i_part=0; i_part < nPart; ++i_part)
				{
					if (pDoc->m_pPostCtrl->GetStrbPartNew(m_aElemCSCS[j], i_part+1, &CBforD, NULL, NULL) == TRUE)
					{
						CD2ID_PBFO(CBforD, BForcPT.aBFOR[i_part]);
					}
				}
				if (m_Data.AddBforPT(m_aElemCSCS[j], nMovingLC, S_IDEA_MLLC, BForcPT) == FALSE) return FALSE;
			}
		}
	}

	nSize = m_aSettleLC.GetSize();
	for (i=0; i<nSize; i++)
	{
		pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_SETTLE, m_aSettleLC[i], LComD);
		pDoc->m_pPostCtrl->SelectLoadCombNew(LComD);
		
		for (j=0; j<m_aNode.GetSize(); j++)
		{      
			/*if (pDoc->m_pPostCtrl->GetReacNew(m_aNode[j], &CReacD, &CReacD2, NULL) == TRUE)
			{
				CD2ID_PREC(CReacD, CReacD2, SReccD);
				if (m_Data.AddRecc(m_aNode[j], m_aSettleLC[i], S_IDEA_SMLC, SReccD) == FALSE) return FALSE;
			}*/
			if (pDoc->m_pPostCtrl->GetDispNew(m_aNode[j], &CDispD, &CDispD2, NULL) == TRUE)
			{
				CD2ID_PDIC(CDispD, CDispD2, SDspcD);
				if (m_Data.AddDspc(m_aNode[j], m_aSettleLC[i], S_IDEA_SMLC, SDspcD) == FALSE) return FALSE;
			}
		}    
		for (j=0; j<m_aElement.GetSize(); j++)
		{
			if (pDoc->m_pAttrCtrl->GetElem(m_aElement[j], CElemD) == FALSE) continue;
			if (CElemD.eltyp == TRUSS_EL) continue;

			if (CElemD.eltyp == BEAM_EL)
			{
				if (pDoc->m_pPostCtrl->GetSbcfNew(m_aElement[j], &CBcfoD, &CBcfoD2, NULL, TRUE) == TRUE)
				{
					CD2ID_PBCF(CBcfoD, CBcfoD2, SBcfoD);
					if (m_Data.AddBcfo(m_aElement[j], m_aSettleLC[i], S_IDEA_SMLC, SBcfoD) == FALSE) return FALSE;
				}
			}
			else
			{
				// Plane Stress
				if (CElemD.eltyp == PLSTRS_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrmNew_L(m_aElement[j], &CStrmD, NULL, NULL) == TRUE) { CD2ID_STRM(CStrmD, SStrpD); }
					else { continue; }
				}
				// Plane Strain
				else if (CElemD.eltyp == PLSTRN_EL)
				{
					if (pDoc->m_pPostCtrl->GetStpnNew_L(m_aElement[j], &CStpnD, NULL, NULL) == TRUE) CD2ID_STPN(CStpnD, SStrpD);     
					else { continue; }
				}
				// Plate
				else if (CElemD.eltyp == PLATE_EL)
				{
					if (pDoc->m_pPostCtrl->GetStrpNew_L(m_aElement[j], &CStrpD, NULL, NULL) == TRUE) { CD2ID_STRP(CStrpD, SStrpD); }
					else { continue; }
				}
				else
				{
					ASSERT(FALSE);
					continue;
				}
				if (m_Data.AddStrp(m_aElement[j], m_aSettleLC[i], S_IDEA_SMLC, SStrpD) == FALSE) return FALSE;
			}
		}

		if (m_aElemCSCS.GetSize() > 0 && m_aElemCSCS.GetSize() == m_aElemCSCSK.GetSize())
		{
			/*T_CSCS_D    CscsD;
			S_BFOR_PTCC BForcPTCC;      
			for (j=0; j<m_aElemCSCS.GetSize(); ++j)
			{
				// 파트의 개수를 알아와야지.
				if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
				int nPart = CscsD.aPart.GetSize();
				if (nPart < 1) { ASSERT(FALSE); continue;}
				
				BForcPTCC.aBFOR.SetSize(nPart);
				for (int i_part=0; i_part < nPart; ++i_part)
				{
					if (pDoc->m_pPostCtrl->GetSbcfPartNew(m_aElemCSCS[j], i_part+1, &CBcfoD, &CBcfoD2, NULL) == TRUE)
					{
						CD2ID_PBCF(CBcfoD, CBcfoD2, BForcPTCC.aBFOR[i_part]);
					}
				}
				if (m_Data.AddBforPTCC(m_aElemCSCS[j], m_aSettleLC[i], S_IDEA_SMLC, BForcPTCC) == FALSE) return FALSE;
			}*/
			T_CSCS_D  CscsD;
			S_BFOR_PT BForcPT;
			
			for (j=0; j<m_aElemCSCS.GetSize(); ++j)
			{
				// 파트의 개수를 알아와야지.
				if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
				int nPart = CscsD.aPart.GetSize();
				if (nPart < 1) { ASSERT(FALSE); continue;}
				
				BForcPT.aBFOR.SetSize(nPart);
				for (int i_part=0; i_part < nPart; ++i_part)
				{
					if (pDoc->m_pPostCtrl->GetStrbPartNew(m_aElemCSCS[j], i_part+1, &CBforD, NULL, NULL) == TRUE)
					{
						CD2ID_PBFO(CBforD, BForcPT.aBFOR[i_part]);
					}
				}
				if (m_Data.AddBforPT(m_aElemCSCS[j], m_aSettleLC[i], S_IDEA_SMLC, BForcPT) == FALSE) return FALSE;
			}
		}
	}

	// for Construnction Stage
	if (D_IDEARSFILE_VERSION > D_IDEARSFILE_110 && pDoc->m_pPostCtrl->StageAnalysis() == TRUE && m_aCSCaseAT.GetSize()>0)
	{
		S_DISP_CSD  DispCSD;
		S_TFOR_CSD  TForCSD;
		S_BFOR_CSD  BForCSD;
		S_STRP_CSDL StrpCSDL;
		CStageInfo* pStageInfo = pDoc->m_pPostCtrl->GetStageInfo();    
				
		for (int nStag=0; nStag < m_aStage.GetSize(); nStag++)
		{
			T_STAG_STEP_INFO StageInfo; StageInfo.Initialize();
			pDoc->m_pAttrCtrl->GetStagStepInfo(m_aStage[nStag], StageInfo);

			// 저장할 결과가 없다.
			if (StageInfo.bSaveResultStage == FALSE) continue;
			BOOL bSaveStep  = StageInfo.bSaveResultStep;            
			int  iStep = StageInfo.aInfo.GetSize();      
			int  nStepSerial = 0;

			for (i=0; i<m_aCSCaseAT.GetSize(); i++)
			{
				if (pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STAGE, m_aCSCaseAT[i], LComD) == FALSE) { ASSERT(FALSE); continue; }

				for (j=0; j<m_aNode.GetSize(); j++)
				{
					if (pStageInfo->IsActiveNodeInTgtStag(m_aStage[nStag], m_aNode[j]) == FALSE) continue;

					DispCSD.Initialize();
					if (bSaveStep == TRUE)
					{
						DispCSD.aDisp.SetSize(iStep);
						for (int nStep=0; nStep < iStep; nStep++)
						{
							if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], nStep+1, nStepSerial) == FALSE) { ASSERT(FALSE); continue; }
							pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction
							if (pDoc->m_pPostCtrl->GetDispNew(m_aNode[j], &CDispD, NULL, NULL) == TRUE) { CD2ID_PDIS(CDispD, SDispD); }
							DispCSD.aDisp[nStep] = SDispD;
						}
					}
					else
					{
						DispCSD.aDisp.SetSize(1);
						if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], iStep, nStepSerial) == TRUE)
						{
							pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
							if (pDoc->m_pPostCtrl->GetDispNew(m_aNode[j], &CDispD, NULL, NULL) == TRUE) { CD2ID_PDIS(CDispD, SDispD); }
						}
						else
						{
							ASSERT(FALSE);
							SDispD.Initialize();
						}
						DispCSD.aDisp[0] = SDispD;
					}

					if (m_Data.AddDispCS(m_aNode[j], m_aCSCaseAT[i], m_aStage[nStag], DispCSD) == FALSE) return FALSE;
				}
				
				for (j=0; j<m_aElement.GetSize(); j++)
				{
					if (pStageInfo->IsActiveElemInTgtStag(m_aStage[nStag], m_aElement[j]) == FALSE) continue;
					if (pDoc->m_pAttrCtrl->GetElem(m_aElement[j], CElemD) == FALSE) continue;
					if (CElemD.eltyp == TRUSS_EL)
					{
						TForCSD.Initialize();

						if (bSaveStep == TRUE)
						{
							TForCSD.aTFOR.SetSize(iStep);            
							for (int nStep = 0; nStep < iStep; nStep++)
							{
								if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], nStep+1, nStepSerial) == FALSE) { ASSERT(FALSE); continue; }
								pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
								if (pDoc->m_pPostCtrl->GetStrtNew(m_aElement[j], &CTforD, NULL, NULL) == TRUE) { CD2ID_PTFO(CTforD, STforD); }
								TForCSD.aTFOR[nStep] = STforD;
							}
						}
						else
						{
							TForCSD.aTFOR.SetSize(1);            
							if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], iStep, nStepSerial) == TRUE)
							{
								pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
								if (pDoc->m_pPostCtrl->GetStrtNew(m_aElement[j], &CTforD, NULL, NULL) == TRUE) { CD2ID_PTFO(CTforD, STforD); }
							}
							else
							{
								ASSERT(FALSE);
								STforD.Initialize();
							}
							TForCSD.aTFOR[0] = STforD;
						}
						if (m_Data.AddTforCS(m_aElement[j], m_aCSCaseAT[i], m_aStage[nStag], TForCSD) == FALSE) return FALSE;
					}
					else if (CElemD.eltyp == BEAM_EL)
					{
						BForCSD.Initialize();

						if (bSaveStep == TRUE)
						{
							BForCSD.aBFOR.SetSize(iStep);            
							for (int nStep = 0; nStep < iStep; nStep++)
							{
								if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], nStep+1, nStepSerial) == FALSE) { ASSERT(FALSE); continue; }
								pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
								if (pDoc->m_pPostCtrl->GetStrbNew(m_aElement[j], &CBforD, NULL, NULL, TRUE) == TRUE) { CD2ID_PBFO(CBforD, SBforD); }
								BForCSD.aBFOR[nStep] = SBforD;
							}
						}
						else
						{
							BForCSD.aBFOR.SetSize(1);
							if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], iStep, nStepSerial) == TRUE)
							{
								pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
								if (pDoc->m_pPostCtrl->GetStrbNew(m_aElement[j], &CBforD, NULL, NULL, TRUE) == TRUE) { CD2ID_PBFO(CBforD, SBforD); }
							}
							else
							{
								ASSERT(FALSE);
								SBforD.Initialize();
							}
							BForCSD.aBFOR[0] = SBforD;              
						}
						if (m_Data.AddBforCS(m_aElement[j], m_aCSCaseAT[i], m_aStage[nStag], BForCSD) == FALSE) return FALSE;
					}
					else
					{
						StrpCSDL.Initialize();

						if (bSaveStep == TRUE)
						{
							StrpCSDL.aSTRP.SetSize(iStep);            
							for (int nStep = 0; nStep < iStep; nStep++)
							{
								if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], nStep+1, nStepSerial) == FALSE)
								{
									ASSERT(FALSE);
									continue;
								}
								pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
								// Plane Stress
								if (CElemD.eltyp == PLSTRS_EL)
								{
									if (pDoc->m_pPostCtrl->GetStrmNew_L(m_aElement[j], &CStrmD, NULL, NULL) == TRUE) { CD2ID_STRM(CStrmD, SStrpD); }
									else { continue; }
								}
								// Plane Strain
								else if (CElemD.eltyp == PLSTRN_EL)
								{
									if (pDoc->m_pPostCtrl->GetStpnNew_L(m_aElement[j], &CStpnD, NULL, NULL) == TRUE) CD2ID_STPN(CStpnD, SStrpD);     
									else { continue; }
								}
								// Plate
								else if (CElemD.eltyp == PLATE_EL)
								{
									if (pDoc->m_pPostCtrl->GetStrpNew_L(m_aElement[j], &CStrpD, NULL, NULL) == TRUE) { CD2ID_STRP(CStrpD, SStrpD); }
									else { continue; }
								}
								else
								{
									ASSERT(FALSE);
									continue;
								}
								StrpCSDL.aSTRP[nStep] = SStrpD;
							}
						}
						else
						{
							StrpCSDL.aSTRP.SetSize(1);            
							if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], iStep, nStepSerial) == TRUE)
							{
								pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
								// Plane Stress
								if (CElemD.eltyp == PLSTRS_EL)
								{
									if (pDoc->m_pPostCtrl->GetStrmNew_L(m_aElement[j], &CStrmD, NULL, NULL) == TRUE) { CD2ID_STRM(CStrmD, SStrpD); }
								}
								// Plane Strain
								else if (CElemD.eltyp == PLSTRN_EL)
								{
									if (pDoc->m_pPostCtrl->GetStpnNew_L(m_aElement[j], &CStpnD, NULL, NULL) == TRUE) CD2ID_STPN(CStpnD, SStrpD);     
								}
								// Plate
								else if (CElemD.eltyp == PLATE_EL)
								{
									if (pDoc->m_pPostCtrl->GetStrpNew_L(m_aElement[j], &CStrpD, NULL, NULL) == TRUE) { CD2ID_STRP(CStrpD, SStrpD); }
								}
							}
							else
							{
								ASSERT(FALSE);
								SStrpD.Initialize();
							}
							StrpCSDL.aSTRP[0] = SStrpD;
						}
						if (m_Data.AddStrpCS(m_aElement[j], m_aCSCaseAT[i], m_aStage[nStag], StrpCSDL) == FALSE) return FALSE;
					}
				}

				if (m_aElemCSCS.GetSize() > 0 && m_aElemCSCS.GetSize() == m_aElemCSCSK.GetSize())
				{
					T_CSCS_D    CscsD;
					S_BFOR_PTCS BForcPTCS;
					
					for (j=0; j<m_aElemCSCS.GetSize(); ++j)
					{
						// 파트의 개수를 알아와야지.
						if (pDoc->m_pAttrCtrl->GetCscs(m_aElemCSCSK[j], CscsD) == FALSE) continue;
						int nPart = CscsD.aPart.GetSize();
						if (nPart < 1) { ASSERT(FALSE); continue;}
				 
						BForcPTCS.Initialize();                                  

						if (bSaveStep == TRUE)
						{
							BForcPTCS.aBFORPT.SetSize(iStep);              
							for (int nStep = 0; nStep < iStep; nStep++)
							{
								if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], nStep+1, nStepSerial) == FALSE) { ASSERT(FALSE); continue; }
								pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.

								BForcPTCS.aBFORPT[nStep].aBFOR.SetSize(nPart);
								for (int i_part=0; i_part < nPart; ++i_part)
								{
									if (pDoc->m_pPostCtrl->GetStrbPartNew(m_aElemCSCS[j], i_part+1, &CBforD, NULL, NULL) == TRUE)
									{
										CD2ID_PBFO(CBforD, BForcPTCS.aBFORPT[nStep].aBFOR[i_part]);
									}
								}
							}              
						}
						else
						{
							BForcPTCS.aBFORPT.SetSize(1);                                          
							if (pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(m_aStage[nStag], iStep, nStepSerial) == FALSE) { ASSERT(FALSE); continue; }
							pDoc->m_pPostCtrl->SelectLoadCombNew(LComD, nStepSerial, 2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
							
							BForcPTCS.aBFORPT[0].aBFOR.SetSize(nPart);
							for (int i_part=0; i_part < nPart; ++i_part)
							{
								if (pDoc->m_pPostCtrl->GetStrbPartNew(m_aElemCSCS[j], i_part+1, &CBforD, NULL, NULL) == TRUE)
								{
									CD2ID_PBFO(CBforD, BForcPTCS.aBFORPT[0].aBFOR[i_part]);
								}
							}
						}

						if (m_Data.AddBforPTCS(m_aElemCSCS[j], m_aCSCaseAT[i], m_aStage[nStag], BForcPTCS) == FALSE) return FALSE;
					}          
				}        
			}
		}
	}
	
	return TRUE;
}

void CDgnClassIREW::MakeArrayData(CDBDoc* pDoc, KEYArray& aElement)
{
	int i(0), nElemSize = aElement.GetSize();
	m_aElement.Copy(aElement);

	// 요소를 돌면서 TRUSS와 BEAM을 제외한 녀석들은 골라낸다. 재료와 단면의 키를 저장해둔다.
	T_ELEM_D  ElemD;
	T_SECT_D  SectD;
	KEYArray  arRemove; arRemove.RemoveAll();
	for (i=0; i<nElemSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetElem(m_aElement[i], ElemD) == FALSE) continue;
		if (ElemD.eltyp == TRUSS_EL || ElemD.eltyp == BEAM_EL)
		{
			if (pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD) == FALSE)
			{
				ASSERT(FALSE);
				continue;
			}
			
			// MNET:XXXX-CJJEONG-20130903 (2088) : Composite User 단면추가
			if ( SectD.nStype != D_SECT_TYPE_REGULAR  && (SectD.nStype == D_SECT_TYPE_USER && SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN) == FALSE &&
				SectD.nStype != D_SECT_TYPE_TAPERED  && SectD.nStype != D_SECT_TYPE_PSC      && 
				SectD.nStype != D_SECT_TYPE_COMPO_B  && SectD.nStype != D_SECT_TYPE_COMPO_I  && SectD.nStype != D_SECT_TYPE_COMPO_CI && 
				SectD.nStype != D_SECT_TYPE_COMPO_CT && SectD.nStype != D_SECT_TYPE_COMPO_PC && SectD.nStype != D_SECT_TYPE_COMPO_G  &&
				SectD.nStype != D_SECT_TYPE_CONSTRUCTION)
			{
				arRemove.Add(i);
				continue;
			}
		}

		if (ElemD.eltyp == TRUSS_EL)
		{
			m_nTruss++;
		}
		else if (ElemD.eltyp == BEAM_EL)
		{
			m_nBeam++;
		}
		else if (ElemD.eltyp == PLSTRS_EL || ElemD.eltyp == PLATE_EL || ElemD.eltyp == PLSTRN_EL)    
		{
			m_n2DElem++;
		}
		else
		{
			arRemove.Add(i);
			continue;
		}
		
		AddData(ElemD.elmat, m_aMaterial);
		if (ElemD.eltyp == TRUSS_EL || ElemD.eltyp == BEAM_EL)
			AddData(ElemD.elpro, m_aSection);
		else
		{
			// Plate Strain은 Thickness가 없다.
			if (ElemD.eltyp == PLSTRS_EL || ElemD.eltyp == PLATE_EL) AddData(ElemD.elpro, m_aThickness);
		}

		if (SectD.nStype == D_SECT_TYPE_TAPERED && (ElemD.eltyp == TRUSS_EL || ElemD.eltyp == BEAM_EL))
		{
			AddData(m_aElement[i], m_aElemTapered);
		}
	}
	for (i=arRemove.GetSize()-1; i>-1; i--)
	{
		m_aElement.RemoveAt(arRemove[i]);
	}
	nElemSize = m_aElement.GetSize();

	pDoc->m_pAttrCtrl->GetElemNodeKeyList(m_aElement, m_aNode);

	// 사용자가 선택한 요소가 포함된 MEMBER의 정보를 얻어온다.
	UINT  nMembKey(0);
	for (i=0; i<nElemSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetMembAssigned(m_aElement[i], nMembKey) == FALSE) continue;    
		AddData(nMembKey, m_aMember);    
	}

	// 재료분기 Steel, Conc, Rebar, Add
	T_MATL_D  MatlD;
	int nMatlSize = m_aMaterial.GetSize();
	for (i=0; i<nMatlSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetMatl(m_aMaterial[i], MatlD) == FALSE) continue;

		AddData(m_aMaterial[i], m_aMaterialAD);
		if (MatlD.Type == CCM_TYPE_STEEL)
		{
			AddData(m_aMaterial[i], m_aMaterialST);
		}
		else if (MatlD.Type == CCM_TYPE_CONCR)
		{
			AddData(m_aMaterial[i], m_aMaterialCN);      
		}
		else if (MatlD.Type == CCM_TYPE_SRC)
		{
			// SRC의 경우에는 두 군데 모두 값을 넣고 AD 에서 찾아다가 쓴다.
			AddData(m_aMaterial[i], m_aMaterialST);
			AddData(m_aMaterial[i], m_aMaterialCN);
		} 
		else
		{
			ASSERT(FALSE);      
		}
	}

	// 철근데이터를 찾아보고 있으면 키배열에 넣어준다.  
	// [Check before Release...!!] cjjeong-20120404 : 재료키를 사용하는가?
	T_MATD_D  MatdD;
	int nMarbSize = m_aMaterialCN.GetSize();
	for (i=0; i<nMarbSize; i++)
	{
		if(!pDoc->m_pAttrCtrl->GetMatlDesign(m_aMaterialCN[i], MatdD) == FALSE)	continue;
		AddData(m_aMaterialCN[i], m_aMaterialRB);
	}

	/* 단면추가 - 시공단계별
	/ [Check before Release...!!] cjjeong-20120404 : 시공단계별 합성단면은 단면의 키를 사용하는가?
	T_CSCS_D  CscsD;
	T_SECT_D  SectD;
	int nSectSize = m_aSection.GetSize();
	for (i=0; i<nSectSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetCscs(m_aSection[i], CscsD) == TRUE) AddData(m_aSection[i], m_aSectionCS);

		// Design 관련도 여기서 한방에 돌자.
		// [Check before Release...!!] cjjeong-20120404 : 모두 Section의 키를 사용하는가?
		// 구지 확인할 필요가 있는가? 아무리 많아봤자 섹션을 넘어가지 않는다. SECTSize 이로 Initialize 하자.
		//if (pDoc->m_pAttrCtrl->GetSectDesign(m_aSection[i], SectD) == TRUE) AddData(m_aSection[i], m_aDgnRdbm);
		//if (pDoc->m_pAttrCtrl->GetSectDesign(m_aSection[i], SectD) == TRUE) AddData(m_aSection[i], m_aDgnRdcn);
		//if (pDoc->m_pAttrCtrl->GetSectDesign(m_aSection[i], SectD) == TRUE) AddData(m_aSection[i], m_aDgnRcbm);
		//if (pDoc->m_pAttrCtrl->GetSectDesign(m_aSection[i], SectD) == TRUE) AddData(m_aSection[i], m_aDgnRccn);
		//if (pDoc->m_pAttrCtrl->GetSectDesign(m_aSection[i], SectD) == TRUE) AddData(m_aSection[i], m_aDgnRebt);
		//if (pDoc->m_pAttrCtrl->GetSectDesign(m_aSection[i], SectD) == TRUE) AddData(m_aSection[i], m_aDgnRpsc);
		//if (pDoc->m_pAttrCtrl->GetSectDesign(m_aSection[i], SectD) == TRUE) AddData(m_aSection[i], m_aDgnTdif);
	}*/
	
	// 경계조건
	T_CONS_D  ConsD;
	int nNodeSize = m_aNode.GetSize();
	for (i=0; i<nNodeSize; i++)
	{
		if (pDoc->m_pAttrCtrl->GetConsNode(m_aNode[i], ConsD) == FALSE) continue;
		AddData(m_aNode[i], m_aBoundary);
	}

	arRemove.RemoveAll();
	T_STLD_D  StldD;
	pDoc->m_pAttrCtrl->GetStldKeyList(m_aStaticLC);
	int nSLCSize = m_aStaticLC.GetSize();
	if (pDoc->m_pPostCtrl->StageAnalysis() == TRUE)
	{
		for (i=0; i<nSLCSize; i++)
		{
			if (pDoc->m_pAttrCtrl->GetStld(m_aStaticLC[i], StldD)==FALSE || StldD.LoadCaseType == _T("CS") || StldD.LoadCaseType == _T("TCS"))
			{
				arRemove.Add(i);
			}
		}
		for(i=arRemove.GetSize()-1; i>=0; i--)
		{
			// for Construction Stage
			if (D_IDEARSFILE_VERSION > D_IDEARSFILE_110) m_aCSCaseLC.Add(m_aStaticLC[arRemove[i]]);
			m_aStaticLC.RemoveAt(arRemove[i]);
		}

		// for Construction Stage
		if (D_IDEARSFILE_VERSION > D_IDEARSFILE_110) 
		{
			pDoc->m_pAttrCtrl->GetStagKeyList(m_aStage);
			pDoc->m_pAttrCtrl->GetCscsKeyList(m_aComSecCS);
			pDoc->m_pAttrCtrl->GetGrupKeyList(m_aStrtGrp);
			pDoc->m_pAttrCtrl->GetBngrKeyList(m_aBndrGrp);
			pDoc->m_pAttrCtrl->GetLdgrKeyList(m_aLoadGrp);
			pDoc->m_pAttrCtrl->GetTdnaKeyList(m_aTendonLoss);
		}
	}
	
	pDoc->m_pAttrCtrl->GetSplcKeyList(m_aResponLC);  
	pDoc->m_pAttrCtrl->GetEspcKeyList(m_aEccentLC);  
	pDoc->m_pAttrCtrl->GetThisKeyList(m_aTmHistLC);  
	//pDoc->m_pAttrCtrl->GetMvldKeyList(m_aMovingLC);  
	pDoc->m_pAttrCtrl->GetSmlcKeyList(m_aSettleLC);  

	// General
	CArray<T_MVLD_K,T_MVLD_K> aMvldK; 	aMvldK.RemoveAll();
	pDoc->m_pAttrCtrl->GetMvldKeyList(aMvldK);
	m_aMovingLC.Append(aMvldK);
	// British Standard.
	CArray<T_MVLDbs_K,T_MVLDbs_K> aMvldbsK; 	aMvldbsK.RemoveAll();
	pDoc->m_pAttrCtrl->GetMvldbsKeyList(aMvldbsK);
	m_aMovingLC.Append(aMvldbsK);
	// France
	CArray<T_MVLDfr_K, T_MVLDfr_K> aMvldfrK; 	aMvldfrK.RemoveAll();
	pDoc->m_pAttrCtrl->GetMvldfrKeyList(aMvldfrK);
	m_aMovingLC.Append(aMvldfrK);
	// China.
	CArray<T_MVLDch_K,T_MVLDch_K> aMvldchK; 	aMvldchK.RemoveAll();
	pDoc->m_pAttrCtrl->GetMvldchKeyList(aMvldchK);
	m_aMovingLC.Append(aMvldchK);
	// India.
	CArray<T_MVLDid_K,T_MVLDid_K> aMvldidK; 	aMvldidK.RemoveAll();
	pDoc->m_pAttrCtrl->GetMvldidKeyList(aMvldidK);
	m_aMovingLC.Append(aMvldidK);
	// Japan.
	CArray<T_MVLDjp_K,T_MVLDjp_K> aMvldjpK; 	aMvldjpK.RemoveAll();
	pDoc->m_pAttrCtrl->GetMvldjpKeyList(aMvldjpK);
	m_aMovingLC.Append(aMvldjpK);

	CArray<UINT, UINT>  arCurLCom;  
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL,  arCurLCom); m_aLoadCombi.Append(arCurLCom);  
	for (i=0; i<arCurLCom.GetSize(); i++) m_aLoadType.Add(D_LCOMTYPE_GENERAL);
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL,    arCurLCom); m_aLoadCombi.Append(arCurLCom);
	for (i=0; i<arCurLCom.GetSize(); i++) m_aLoadType.Add(D_LCOMTYPE_STEEL);
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arCurLCom); m_aLoadCombi.Append(arCurLCom);
	for (i=0; i<arCurLCom.GetSize(); i++) m_aLoadType.Add(D_LCOMTYPE_CONCRETE);
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_FDN,      arCurLCom); m_aLoadCombi.Append(arCurLCom);
	for (i=0; i<arCurLCom.GetSize(); i++) m_aLoadType.Add(D_LCOMTYPE_FDN);
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SRC,      arCurLCom); m_aLoadCombi.Append(arCurLCom);
	for (i=0; i<arCurLCom.GetSize(); i++) m_aLoadType.Add(D_LCOMTYPE_SRC);
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_ALUMINUM, arCurLCom); m_aLoadCombi.Append(arCurLCom);
	for (i=0; i<arCurLCom.GetSize(); i++) m_aLoadType.Add(D_LCOMTYPE_ALUMINUM);
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STLCOMP , arCurLCom); m_aLoadCombi.Append(arCurLCom);
	for (i=0; i<arCurLCom.GetSize(); i++) m_aLoadType.Add(D_LCOMTYPE_STLCOMP);


	// Design Data
	for (i=0; i<nElemSize; i++)
	{
		if (pDoc->m_pAttrCtrl->ExistLeng(m_aElement[i]) == TRUE) AddData(m_aElement[i], m_aDgnLeng);
		if (pDoc->m_pAttrCtrl->ExistKfac(m_aElement[i]) == TRUE) AddData(m_aElement[i], m_aDgnKfac);
		if (pDoc->m_pAttrCtrl->ExistLtsr(m_aElement[i]) == TRUE) AddData(m_aElement[i], m_aDgnLtsr);
		if (pDoc->m_pAttrCtrl->ExistCmft(m_aElement[i]) == TRUE) AddData(m_aElement[i], m_aDgnCmft);
		if (pDoc->m_pAttrCtrl->ExistFmag(m_aElement[i]) == TRUE) AddData(m_aElement[i], m_aDgnFmag);
		if (pDoc->m_pAttrCtrl->IsTendonAssigned(m_aElement[i]) == TRUE) AddData(m_aElement[i], m_aTendon);    
	}

	// Composite Section for Construction Stage : 합성단면을 가진 요소번호를 저장해 두자. 왜? 결과 찍어주게..
	if (m_aComSecCS.GetSize() > 0)
	{    
		T_CSCS_K Key;
		T_CSCS_D Data;
		POSITION pos;

		for (i=0; i<m_aElement.GetSize(); ++i)
		{
			if (pDoc->m_pAttrCtrl->GetElem(m_aElement[i], ElemD) == FALSE) continue;
			if (ElemD.eltyp != BEAM_EL) continue;
			
			BOOL bExist = FALSE;
			pos=pDoc->m_pAttrCtrl->GetStartCscs();
			while(pos != NULL)
			{
				pDoc->m_pAttrCtrl->GetNextCscs(pos, Key, Data);
				if (Data.SectKey == ElemD.elpro)
				{
					bExist = TRUE;
					break;
				}
			}
			if (bExist == FALSE) continue;

			m_aElemCSCS.Add(m_aElement[i]);
			m_aElemCSCSK.Add(Key);
		}
	}
}

void CDgnClassIREW::CD2ID_NODE(T_NODE_D& cNode, S_NODE_D& iNode)
{
	iNode.Initialize();

	iNode.dx = cNode.x;
	iNode.dy = cNode.y;
	iNode.dz = cNode.z;
}

void CDgnClassIREW::CD2ID_ELEM(T_ELEM_D& cElem, S_ELEM_D& iElem)
{
	iElem.Initialize();

	if      (cElem.eltyp == TRUSS_EL)  iElem.nEltype = TRUSS_EL;  
	else if (cElem.eltyp == BEAM_EL)   iElem.nEltype = BEAM_EL;  
	else if (cElem.eltyp == PLSTRS_EL) iElem.nEltype = PLSTRS_EL;  
	else if (cElem.eltyp == PLATE_EL)  iElem.nEltype = PLATE_EL;  
	else if (cElem.eltyp == PLSTRN_EL) iElem.nEltype = PLSTRN_EL;  
	else
	{
		ASSERT(FALSE);    
	}
			
	iElem.MatlK = cElem.elmat;
	iElem.SectK = cElem.elpro;
	iElem.dAngle = cElem.angle;
	memcpy(iElem.NodeK, cElem.elnod, sizeof(iElem.NodeK));
}

void CDgnClassIREW::CD2ID_MEMB(T_MEMB_D& cMemb, S_MEMB_D& iMemb)
{
	iMemb.Initialize();
	
	iMemb.aElemK.Copy(cMemb.aElemList);
	iMemb.bReverseDir = cMemb.bReverseDir;
	// [Check before Release...!!] cjjeong-20120404 : 부재타입은?? Material에서 가져와야 하나?
	
}

void CDgnClassIREW::CD2ID_MTAN(T_MATL_ANALYSIS& cMatl, S_MATL_PROP& iMatl)
{  
	iMatl.dElast        = cMatl.Elast;
	iMatl.dPoisson      = cMatl.Poisson;
	iMatl.dThermal      = cMatl.Thermal;
	iMatl.dDensity      = cMatl.Density;
	iMatl.dMassDensity  = cMatl.MassDensity;
	iMatl.dShearModulus = cMatl.ShearModulus;
}

void CDgnClassIREW::CD2ID_MTSD(T_MATL_DESIGN& cMtst, S_MATL_STEEL_D& iMtst)
{ 
	iMtst.dS_Fu  = cMtst.S_Fu;
	iMtst.dS_Fy  = cMtst.S_Fy;
	iMtst.dS_Fy1 = cMtst.S_Fy1;
	iMtst.dS_Fy2 = cMtst.S_Fy2;
	iMtst.dS_Fy3 = cMtst.S_Fy3;
	iMtst.dS_Fy4 = cMtst.S_Fy4;
	iMtst.dS_Fy5 = cMtst.S_Fy5;
	iMtst.dS_Fy6 = cMtst.S_Fy6;
}

void CDgnClassIREW::CD2ID_MTST(T_MATD_D& cMatd, S_MATL_STEEL_D& iMtst)
{
	iMtst.Initialize();
	iMtst.szCode    = cMatd.Data1.CodeName;
	iMtst.szMatName = cMatd.Data1.CodeMatlName;
	CD2ID_MTAN(cMatd.Data1.Analysis, iMtst.PropD);
	CD2ID_MTSD(cMatd.Data1.Design, iMtst);
}

void CDgnClassIREW::CD2ID_MTCC(T_MATD_D& cMtcc, S_MATL_CONCRETE_D& iMtcc)
{
	iMtcc.Initialize();
	if (cMtcc.Type == CCM_TYPE_CONCR)
	{
		iMtcc.szCode = cMtcc.Data1.CodeName;
		iMtcc.szMatName = cMtcc.Data1.CodeMatlName;
		CD2ID_MTAN(cMtcc.Data1.Analysis, iMtcc.PropD);
		iMtcc.dC_fc = cMtcc.Data1.Design.C_fc;
	}
	else
	{
		iMtcc.szCode = cMtcc.Data2.CodeName;
		iMtcc.szMatName = cMtcc.Data2.CodeMatlName;
		CD2ID_MTAN(cMtcc.Data2.Analysis, iMtcc.PropD);
		iMtcc.dC_fc = cMtcc.Data2.Design.C_fc;
	}
}

void CDgnClassIREW::CD2ID_MTRB(T_MATL_REBAR& cMtrb, S_MATL_REBAR& iMtrb)
{
	iMtrb.Initialize();
	iMtrb.dB_fy     = cMtrb.B_fy;
	iMtrb.dB_Elast  = cMtrb.B_Elast;
}

void CDgnClassIREW::CD2ID_MTRI(T_MATD_D& cMtri, S_MATL_REBAR_INFO& iMtri)
{
	iMtri.szRebar_CodeName      = cMtri.Rebar_CodeName;
	iMtri.szMainRebar_RebarName = cMtri.MainRebar_RebarName;
	iMtri.szSubRebar_RebarName  = cMtri.SubRebar_RebarName;
	CD2ID_MTRB(cMtri.MainRebarData, iMtri.MainRebarData);
	CD2ID_MTRB(cMtri.SubRebarData,  iMtri.SubRebarData);
}

void CDgnClassIREW::CD2ID_MTAD(T_MATL_D& cMtad, S_MATL_ADIF_D& iMtad)
{
	iMtad.Initialize();
	iMtad.szType          = cMtad.Type;
	iMtad.szName          = cMtad.Name;
	iMtad.dSpecificHeat   = cMtad.dSpecificHeat;
	iMtad.dHeatConduction = cMtad.dHeatConduction;
	iMtad.nThermalUnit    = cMtad.nThermalUnit;
}

void CDgnClassIREW::CD2ID_SGVT(T_GSEC_VERTEX& cSgvt, S_GSEC_VERTEX& iSgvt)
{
	iSgvt.Initialize();
	iSgvt.dX = cSgvt.dX;
	iSgvt.dY = cSgvt.dY;
}

void CDgnClassIREW::CD2ID_SGPG(T_GSEC_POLYGON& cSgpg, S_GSEC_POLYGON& iSgpg)
{
	iSgpg.Initialize();
	int nSize = cSgpg.aVertex.GetSize();
	if (nSize < 1) return;

	iSgpg.aVertex.SetSize(nSize); 
	for (int i=0; i<nSize; i++)
	{
		CD2ID_SGVT(cSgpg.aVertex[i], iSgpg.aVertex[i]);
	}
}

void CDgnClassIREW::CD2ID_SRSF(T_SECT_STIFFNESS& cStsf, S_SECT_STIFFNESS& iStsf)
{
	iStsf.Initialize();
	iStsf.dArea   = cStsf.Area;
	iStsf.dAsy    = cStsf.Asy;
	iStsf.dAsz    = cStsf.Asz; 
	iStsf.dRxx    = cStsf.Rxx; 
	iStsf.dRyy    = cStsf.Ryy; 
	iStsf.dRzz    = cStsf.Rzz; 
	iStsf.dCyp    = cStsf.Cyp; 
	iStsf.dCym    = cStsf.Cym; 
	iStsf.dCzp    = cStsf.Czp; 
	iStsf.dCzm    = cStsf.Czm; 
	iStsf.dQyb    = cStsf.Qyb; 
	iStsf.dQzb    = cStsf.Qzb; 
	iStsf.dWArea  = cStsf.WArea;
	iStsf.dx1     = cStsf.dx1;
	iStsf.dy1     = cStsf.dy1;
	iStsf.dx2     = cStsf.dx2;
	iStsf.dy2     = cStsf.dy2;
	iStsf.dx3     = cStsf.dx3;
	iStsf.dy3     = cStsf.dy3;
	iStsf.dx4     = cStsf.dx4;
	iStsf.dy4     = cStsf.dy4;
}
// [Check before Release...!!] cjjeong-20120405 : 
void CDgnClassIREW::CD2ID_SGPT(T_SECT_SECTBASE_D& cStbs, T_GSEC_PART& cPart, S_GSEC_PART& iSgpt)
{
	iSgpt.Initialize();

	iSgpt.szShape  = cStbs.Shape;
	iSgpt.szDBName = cStbs.DBName;
	memcpy(iSgpt.dSize, cStbs.Size, sizeof(iSgpt.dSize));
	iSgpt.nBuiltUpFlag  = cStbs.BuiltUpFlag;

	CD2ID_SRSF(cPart.Stiffness, iSgpt.Stiffness);
	CD2ID_SRSF(cPart.StiffnessSum, iSgpt.StiffnessCom);
	iSgpt.dPeriIn     = cPart.PeriIn;
	iSgpt.dPeriOut    = cPart.PeriOut;
	iSgpt.dPeriInCom  = cPart.PeriInSum;
	iSgpt.dPeriOutCom = cPart.PeriOutSum;

	memcpy(iSgpt.IdxStart, cPart.IdxStart, sizeof(iSgpt.IdxStart));
	memcpy(iSgpt.IdxEnd  , cPart.IdxEnd  , sizeof(iSgpt.IdxEnd  ));

	// [Check before Release...!!] cjjeong-20120410 : 
	// 시공단계 정보가 없어도 폴리곤을 주어야 할까?
}

void CDgnClassIREW::CD2ID_LINE(const T_SECT_SECTION_D& cStsn, S_SECT_D& iSect)
{
	int nSize = cStsn.SectI.aLine.GetSize();
	iSect.SectI.aLine.SetSize(nSize);
	for(int i=0; i<nSize; i++)
	{
		iSect.SectI.aLine[i].v1     = cStsn.SectI.aLine[i].v1;
		iSect.SectI.aLine[i].v2     = cStsn.SectI.aLine[i].v2;
		iSect.SectI.aLine[i].dThik  = cStsn.SectI.aLine[i].dThik;
		iSect.SectI.aLine[i].nAlign = cStsn.SectI.aLine[i].nAlign;
	}

	nSize = cStsn.SectJ.aLine.GetSize();
	iSect.SectJ.aLine.SetSize(nSize);
	for(int i=0; i<nSize; i++)
	{
		iSect.SectJ.aLine[i].v1     = cStsn.SectJ.aLine[i].v1;
		iSect.SectJ.aLine[i].v2     = cStsn.SectJ.aLine[i].v2;
		iSect.SectJ.aLine[i].dThik  = cStsn.SectJ.aLine[i].dThik;
		iSect.SectJ.aLine[i].nAlign = cStsn.SectJ.aLine[i].nAlign;
	}
}

void CDgnClassIREW::CD2ID_LNLP(const T_SECT_SECTION_D& cStsn, S_SECT_D& iSect)
{
	iSect.SectI.aLineLoop.Copy(cStsn.SectI.aLineLoop);
	iSect.SectJ.aLineLoop.Copy(cStsn.SectJ.aLineLoop);
}

void CDgnClassIREW::CD2ID_STBS(int nType,  T_SECT_SECTBASE_D& cStbs, S_SECT_SECTBASE_D& iStbs)
{
	iStbs.Initialize();
	iStbs.nStype   = nType;

	int i(0), nSize(0);
	
	memcpy(iStbs.dSize, cStbs.Size, sizeof(iStbs.dSize));
	CD2ID_SRSF(cStbs.Stiffness, iStbs.Stiffness);  

	nSize = cStbs.aOuterPolygon.GetSize();
	iStbs.aOuterPolygon.SetSize(nSize);
	for (i=0; i<nSize; i++) CD2ID_SGPG(cStbs.aOuterPolygon[i], iStbs.aOuterPolygon[i]);
	
	nSize = cStbs.aInnerPolygon.GetSize();
	iStbs.aInnerPolygon.SetSize(nSize);
	for (i=0; i<nSize; i++) CD2ID_SGPG(cStbs.aInnerPolygon[i], iStbs.aInnerPolygon[i]);
	
	nSize = cStbs.aGeneralPart.GetSize();
	iStbs.aSectPart.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		if (cStbs.aGeneralPart[i].bBaseMaterial == TRUE)
		{
			iStbs.nPartBaseK = i;
		}
		CD2ID_SGPT(cStbs, cStbs.aGeneralPart[i], iStbs.aSectPart[i]);
	}
	
}

void CDgnClassIREW::CD2ID_SECT(CDBDoc* pDoc, T_SECT_D& cSectD, S_SECT_D& iSectD)
{
	iSectD.Initialize();
	T_SECT_SECTION_D* pSection = NULL;
	// Composite 단면의 경우 T_SECT_D의 After 를 이용해야 한다.
	if (cSectD.nStype == D_SECT_TYPE_COMPO_B  || cSectD.nStype == D_SECT_TYPE_COMPO_I  || cSectD.nStype == D_SECT_TYPE_COMPO_CI ||
			cSectD.nStype == D_SECT_TYPE_COMPO_CT || cSectD.nStype == D_SECT_TYPE_COMPO_PC || cSectD.nStype == D_SECT_TYPE_COMPO_G )
	{
		pSection = &cSectD.SectAfter;
	}
	else
	{
		pSection = &cSectD.SectBefore;
	}
	
	// MNET:XXXX-CJJEONG-20130903 (2088) : Composite User 단면추가
	if (cSectD.nStype == D_SECT_TYPE_CONSTRUCTION)
	{
		CD2ID_CPUR(cSectD, iSectD);
	}
	else
	{
		CD2ID_STBS(cSectD.nStype, pSection->SectI, iSectD.SectI);
		CD2ID_STBS(cSectD.nStype, pSection->SectJ, iSectD.SectJ);
	}

	// Composite General인 경우 Composite 단면이므로 After단면을 사용하는데 파트별 정보는 Before에 있어서 새로 복사를 해준다.
	if (cSectD.nStype == D_SECT_TYPE_COMPO_G)
	{
		iSectD.SectI.aSectPart.RemoveAll();
		int nPSize = cSectD.SectBefore.SectI.aGeneralPart.GetSize();
		iSectD.SectI.aSectPart.SetSize(nPSize);
		for (int i=0; i<nPSize; i++)
		{
			if (cSectD.SectBefore.SectI.aGeneralPart[i].bBaseMaterial == TRUE)
			{
				iSectD.SectI.nPartBaseK = i;
			}
			CD2ID_SGPT(cSectD.SectBefore.SectI, cSectD.SectBefore.SectI.aGeneralPart[i], iSectD.SectI.aSectPart[i]);
		}
	}

	if (cSectD.nStype == D_SECT_TYPE_COMPO_B  || cSectD.nStype == D_SECT_TYPE_COMPO_I  || cSectD.nStype == D_SECT_TYPE_COMPO_CI ||
			cSectD.nStype == D_SECT_TYPE_COMPO_CT || cSectD.nStype == D_SECT_TYPE_COMPO_PC )
	{
		CD2ID_SRSF(pSection->Stiffness, iSectD.SectI.Stiffness);
		CD2ID_SRSF(pSection->Stiffness, iSectD.SectJ.Stiffness);
	}
	else if (cSectD.nStype == D_SECT_TYPE_COMPO_G)
	{
		int nIndex = cSectD.SectBefore.SectI.aGeneralPart.GetSize()-1;
		if (nIndex >=0)
		{
			CD2ID_SRSF(cSectD.SectBefore.SectI.aGeneralPart[nIndex].StiffnessSum, iSectD.SectI.Stiffness);
			CD2ID_SRSF(cSectD.SectBefore.SectI.aGeneralPart[nIndex].StiffnessSum, iSectD.SectJ.Stiffness);
		}
	}
	else if (cSectD.nStype == D_SECT_TYPE_TAPERED && cSectD.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
	{
		int nIndex = cSectD.SectBefore.SectI.aGeneralPart.GetSize()-1;
		if (nIndex >=0)
		{
			CD2ID_SRSF(cSectD.SectBefore.SectI.aGeneralPart[nIndex].StiffnessSum, iSectD.SectI.Stiffness);
			CD2ID_SRSF(cSectD.SectBefore.SectJ.aGeneralPart[nIndex].StiffnessSum, iSectD.SectJ.Stiffness);
		}
	}

	// Line, LineLoop 데이터는 항상 SectBefore에 있다.
	CD2ID_LINE(cSectD.SectBefore, iSectD);
	CD2ID_LNLP(cSectD.SectBefore, iSectD);

	// String 써주는 부분
	iSectD.SectI.szShape  = pSection->Shape;
	iSectD.SectI.szName   = cSectD.SName;
	iSectD.SectI.szDBName = pSection->SectI.DBName;
	iSectD.SectJ.szShape  = pSection->Shape;
	iSectD.SectJ.szName   = cSectD.SName;
	iSectD.SectJ.szDBName = pSection->SectJ.DBName;
}

void CDgnClassIREW::IDDATA_POLYGON(S_SECT_SECTBASE_D&  rSect, DGN_GSEC_POLYGON_LIST&  arOut1, DGN_GSEC_POLYGON_LIST& arOut2, DGN_GSEC_POLYGON_LIST& arIn1, DGN_GSEC_POLYGON_LIST& arIn2)
{
	int i(0), j(0), nSize(0), nSubSize(0);

	ASSERT(rSect.aOuterPolygon.GetSize()<1);
	ASSERT(rSect.aInnerPolygon.GetSize()<1);
	rSect.aOuterPolygon.RemoveAll();
	rSect.aInnerPolygon.RemoveAll();

	// About Outter
	nSize = arOut1.GetSize();
	rSect.aOuterPolygon.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		nSubSize = arOut1[i].aVertex.GetSize();
		rSect.aOuterPolygon[i].aVertex.SetSize(nSubSize);

		for (j=0; j<nSubSize; j++)
		{
			rSect.aOuterPolygon[i].aVertex[j].dX = arOut1[i].aVertex[j].dX;
			rSect.aOuterPolygon[i].aVertex[j].dY = arOut1[i].aVertex[j].dY;
		}
	}
	nSize = arOut2.GetSize();
	if (nSize > 0)
	{
		S_GSEC_POLYGON  GsecPolygon;    
		for (i=0; i<nSize; i++)
		{
			GsecPolygon.Initialize();
			nSubSize = arOut2[i].aVertex.GetSize();
			GsecPolygon.aVertex.SetSize(nSubSize);
			for (j=0; j<nSubSize; j++)
			{
				GsecPolygon.aVertex[j].dX = arOut2[i].aVertex[j].dX;
				GsecPolygon.aVertex[j].dY = arOut2[i].aVertex[j].dY;
			}
			rSect.aOuterPolygon.Add(GsecPolygon);
		}
	}

	// About Inner
	nSize = arIn1.GetSize();
	rSect.aInnerPolygon.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		nSubSize = arIn1[i].aVertex.GetSize();
		rSect.aInnerPolygon[i].aVertex.SetSize(nSubSize);    
		for (j=0; j<nSubSize; j++)
		{
			rSect.aInnerPolygon[i].aVertex[j].dX = arIn1[i].aVertex[j].dX;
			rSect.aInnerPolygon[i].aVertex[j].dY = arIn1[i].aVertex[j].dY;
		}
	}
	nSize = arIn2.GetSize();
	if (nSize > 0)
	{
		S_GSEC_POLYGON  GsecPolygon;    
		for (i=0; i<nSize; i++)
		{
			GsecPolygon.Initialize();
			nSubSize = arIn2[i].aVertex.GetSize();
			GsecPolygon.aVertex.SetSize(nSubSize);
			for (j=0; j<nSubSize; j++)
			{
				GsecPolygon.aVertex[j].dX = arIn2[i].aVertex[j].dX;
				GsecPolygon.aVertex[j].dY = arIn2[i].aVertex[j].dY;
			}
			rSect.aInnerPolygon.Add(GsecPolygon);
		}
	}  
}

void CDgnClassIREW::CD2ID_CONS(T_CONS_D& cCons, S_CONS_D& iCons)
{
	iCons.Initialize();  
	memcpy(iCons.sConstraint, cCons.Constraint, sizeof(iCons.sConstraint));
}

void CDgnClassIREW::CD2ID_STLD(T_STLD_D& cStld, S_STLD_D& iStld)
{
	iStld.Initialize();
	iStld.szLoadCaseName = cStld.LoadCaseName;
	iStld.szLoadCaseType = cStld.LoadCaseType;
	iStld.szDescription  = cStld.Description;
	iStld.nLoadCaseId    = cStld.LoadCaseId;
}

void CDgnClassIREW::CD2ID_LCMB(CDBDoc* pDoc, T_LCOM_BASE& cLcmb, S_LCOM_BASE& iLcmb)
{
	iLcmb.Initialize();
	iLcmb.nAnalType     = cLcmb.AnalType;
	if(iLcmb.nAnalType==D_LCOM_MOVING)
	{
		int nSerialNo = pDoc->m_pPostCtrl->GetSerialMvld(cLcmb.LoadCaseKey);
		iLcmb.nLoadCaseKey  = nSerialNo;
	}
	else
	{
		iLcmb.nLoadCaseKey  = cLcmb.LoadCaseKey;
	}
	 
	iLcmb.dFactor       = cLcmb.Factor;
}

void CDgnClassIREW::CD2ID_LCOM(CDBDoc* pDoc, T_LCOM_D& cLcom, S_LCOM_D& iLcom)
{
	iLcom.Initialize();
	iLcom.szLoadCombName = cLcom.LoadCombName;
	iLcom.nLoadCombType  = cLcom.LoadCombType;
	iLcom.szDescription  = cLcom.Description;
	iLcom.nActive        = cLcom.nActive;
	iLcom.nLoadCombId    = cLcom.LoadCombId;
	int nSize = cLcom.aCombination.GetSize();
	if (nSize > DT_LCOM_NUMCOMB) nSize = DT_LCOM_NUMCOMB;
	for (int i=0; i< nSize; i++) CD2ID_LCMB(pDoc, cLcom.aCombination[i], iLcom.Combination[i]);
	iLcom.nServLcomType    = cLcom.nServLcomType;
}

void CDgnClassIREW::CD2ID_LENG(T_LENG_D& cLeng, S_LENG_D& iLeng)
{
	iLeng.Initialize();
	iLeng.dBLy        = cLeng.BLy;
	iLeng.dBLz        = cLeng.BLz;
	iLeng.bNotUseBLu  = cLeng.bNotUseBLu;
	iLeng.dBLu        = cLeng.BLu;
	iLeng.bAutoCalcLe = cLeng.bAutoCalcLe;
}

void CDgnClassIREW::CD2ID_KFAC(T_KFAC_D& cKfac, S_KFAC_D& iKfac)
{
	iKfac.Initialize();
	iKfac.dBKy = cKfac.BKy;
	iKfac.dBKz = cKfac.BKz;
}

void CDgnClassIREW::CD2ID_LTSR(T_LTSR_D& cLtsr, S_LTSR_D& iLtsr)
{
	iLtsr.Initialize();
	iLtsr.bNotCheck = cLtsr.bNotCheck;
	iLtsr.dCLMT     = cLtsr.CLMT;
	iLtsr.dTLMT     = cLtsr.TLMT;
}

void CDgnClassIREW::CD2ID_CMFT(T_CMFT_D& cCmft, S_CMFT_D& iCmft)
{
	iCmft.Initialize();
	iCmft.bAuto = cCmft.bAuto;
	iCmft.dCmy  = cCmft.Cmy;
	iCmft.dCmz  = cCmft.Cmz;
}

void CDgnClassIREW::CD2ID_FMAG(T_FMAG_D& cFmag, S_FMAG_D& iFmag)
{
	iFmag.Initialize();
	iFmag.dB1y = cFmag.B1y;
	iFmag.dB1z = cFmag.B1z;
	iFmag.dB2y = cFmag.B2y;
	iFmag.dB2z = cFmag.B2z;
}

void CDgnClassIREW::CD2ID_DCON(T_DCON_D& cDcon, S_DCON_D& iDcon)
{
	iDcon.Initialize();
	iDcon.szDesignCode    = cDcon.DesignCode;
	memcpy(iDcon.dPhi, cDcon.dPhi, sizeof(iDcon.dPhi));  
	iDcon.dRhoc           = cDcon.dRhoc;
	iDcon.dRhor           = cDcon.dRhor;
	iDcon.bSpecialSeismic = cDcon.bSpecialSeismic;
	iDcon.dMrft           = cDcon.dMrft;
	iDcon.nSubCode        = cDcon.iSubCode;
	iDcon.dAlphacc        = cDcon.dAlphacc;
	iDcon.dMinRhoc        = cDcon.dMinRhoc;
	iDcon.dStrutAngle     = cDcon.dStrutAngle;  
}

void CDgnClassIREW::CD2ID_RDBM(T_RDGN_BEAM& cRdbm, S_RDGN_BEAM& iRdbm)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	iRdbm.Initialize();
	iRdbm.dDt         = cRdbm.dDt[0];
	iRdbm.dDb         = cRdbm.dDb[0];
	iRdbm.dSubBarNum  = cRdbm.dSubBarNum[0];
	iRdbm.szSubBarNa  = cRdbm.strSubBarNa[0];
}

void CDgnClassIREW::CD2ID_RDCN(T_RDGN_COLM& cRdcn, S_RDGN_COLM& iRdcn)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	iRdcn.Initialize();
	iRdcn.dDc = cRdcn.dDc[0];
	iRdcn.dSubBarNum = cRdcn.dSubBarNum[0];
	iRdcn.szSubBarNa = cRdcn.strSubBarNa[0];
}

void CDgnClassIREW::CD2ID_RCBM(T_RCHK_BEAM& cRcbm, S_RCHK_BEAM& iRcbm)
{
	iRcbm.Initialize();
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<5; j++)
		{
			iRcbm.dDb[i][j]         = cRcbm.dDb[i][j];
			iRcbm.dDb[i][j]         = cRcbm.dDb[i][j];
			iRcbm.nTopBarNum[i][j]  = cRcbm.iTopBarNum[i][j];
			iRcbm.nBotBarNum[i][j]  = cRcbm.iBotBarNum[i][j];
			iRcbm.szTopBarNa1[i][j] = cRcbm.strTopBarNa1[i][j];
			iRcbm.szTopBarNa2[i][j] = cRcbm.strTopBarNa2[i][j];
			iRcbm.szBotBarNa1[i][j] = cRcbm.strBotBarNa1[i][j];
			iRcbm.szBotBarNa2[i][j] = cRcbm.strBotBarNa2[i][j];
		}
		iRcbm.dSubBarNum[i]  = cRcbm.dSubBarNum[i];
		iRcbm.dSubBarDist[i] = cRcbm.dSubBarDist[i];
		iRcbm.szSubBarNa[i]  = cRcbm.strSubBarNa[i];
	}  
}
void CDgnClassIREW::CD2ID_RCCN(T_RCHK_COLM& cRccn, S_RCHK_COLM& iRccn)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	iRccn.Initialize();
	memcpy(iRccn.dDc, cRccn.dDc, sizeof(iRccn.dDc));
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<5; j++)
		{
			iRccn.nBarNum[i][j]  = cRccn.iBarNum[0][i][j];
			iRccn.szBarNa1[i][j] = cRccn.strBarNa1[0][i][j];
			iRccn.szBarNa2[i][j] = cRccn.strBarNa2[0][i][j];
		}
	}
	iRccn.dSubBarNum = min(cRccn.dSubBarNum_y[0], cRccn.dSubBarNum_z[0]);
	iRccn.dSubBarDist = cRccn.dSubBarDist[0];
	iRccn.szSubBarNa  = cRccn.strSubBarNa[0];
}

void CDgnClassIREW::CD2ID_RBMB(T_REBT_MABR& cRbmb, S_REBT_MABR& iRbmb)
{
	iRbmb.Initialize();
	iRbmb.nLabel      = cRbmb.iLabel;
	iRbmb.dCenterY    = cRbmb.dCenterY;
	iRbmb.dCenterZ    = cRbmb.dCenterZ;
	iRbmb.szRebarName = cRbmb.strRebarName;  
}

void CDgnClassIREW::CD2ID_RBSB(T_REBT_SHBR& cRbsb, S_REBT_SHBR& iRbsb)
{
	iRbsb.Initialize();
	iRbsb.dSubRebarSpace  = cRbsb.dSubRebarSpace;
	iRbsb.dSubRebarNum[0] = cRbsb.dSubRebarNum[0];
	iRbsb.dSubRebarNum[1] = cRbsb.dSubRebarNum[1];
	iRbsb.szSubRebarName  = cRbsb.strSubRebarName;
}

void CDgnClassIREW::CD2ID_REBT(T_REBT_D& cRebt, S_REBT_D& iRebt)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	iRebt.Initialize();
	iRebt.nCenterType = cRebt.iCenterType;
	int i(0), nSize = cRebt.arMainRebar[0].GetSize();
	iRebt.aMainRebar.SetSize(nSize);
	for (i=0; i<nSize; i++) CD2ID_RBMB(cRebt.arMainRebar[0].GetAt(i), iRebt.aMainRebar[i]);
	CD2ID_RBSB(cRebt.ShearRebar[0], iRebt.ShearRebar);
}

void CDgnClassIREW::CD2ID_PTSM(T_POINT_SSM& cPtsm, S_POINT_SSM& iPtsm)
{
	iPtsm.Initialize();
	iPtsm.dPointY = cPtsm.dPointY;
	iPtsm.dPointZ = cPtsm.dPointZ;
}

void CDgnClassIREW::CD2ID_RPMS(T_RPSC_MBAR_SSM& cRpms, S_RPSC_MBAR_SSM& iRpms)
{
	iRpms.Initialize();

	iRpms.nType = cRpms.nType;
	iRpms.nRefY = cRpms.nRefY;
	iRpms.nRefZ = cRpms.nRefZ;
	CD2ID_PTSM(cRpms.Start , iRpms.Start);
	CD2ID_PTSM(cRpms.Center , iRpms.Center);
	CD2ID_PTSM(cRpms.End , iRpms.End);
	int i(0), nSize = cRpms.aPassage.GetSize();
	iRpms.aPassage.SetSize(nSize);
	for (i=0; i<nSize; i++) CD2ID_PTSM(cRpms.aPassage[i], iRpms.aPassage[i]);
	iRpms.dRadius   = cRpms.dRadius;
	iRpms.nNumorCTC = cRpms.nNumorCTC;
	iRpms.nNum      = cRpms.nNum;
	iRpms.dCTC      = cRpms.dCTC;
	iRpms.bEdgeBar  = cRpms.bEdgeBar;
	iRpms.szBar     = cRpms.strBar;
	iRpms.nStandRef = cRpms.nStandRef;
	iRpms.nPart     = cRpms.nPart;
	iRpms.nRefY_A   = cRpms.nRefY_A;
	iRpms.nRefZ_A   = cRpms.nRefZ_A;
	iRpms.dRefY_A   = cRpms.dRefY_A;
	iRpms.dRefZ_A   = cRpms.dRefZ_A;
	iRpms.dSpacing  = cRpms.dSpacing;  
}

void CDgnClassIREW::CD2ID_RPSS(T_RPSC_SBAR_SSM& cRpss, S_RPSC_SBAR_SSM& iRpss)
{
	iRpss.Initialize();
	iRpss.bDiagonal       = cRpss.bDiagonal;
	iRpss.dDiagonalPitch  = cRpss.dDiagonalPitch;
	iRpss.dDiagonalAngle  = cRpss.dDiagonalAngle;
	iRpss.dDiagonalAw     = cRpss.dDiagonalAw;
	iRpss.bSteelBar       = cRpss.bSteelBar;
	iRpss.dSteelBarPitch  = cRpss.dSteelBarPitch;
	iRpss.dSteelBarAngle  = cRpss.dSteelBarAngle;
	iRpss.dSteelBarAp     = cRpss.dSteelBarAp;
	iRpss.dSteelBarPe     = cRpss.dSteelBarPe;
	iRpss.bTorsional      = cRpss.bTorsional;
	iRpss.dTorsionalPitch = cRpss.dTorsionalPitch;
	iRpss.dTorsionalAwt   = cRpss.dTorsionalAwt;
	iRpss.dTorsionalAlt   = cRpss.dTorsionalAlt;
	iRpss.bLbarFlg        = cRpss.bLbarFlg;
	iRpss.dLbarThick      = cRpss.dLbarThick;
	iRpss.bIncFC          = cRpss.bIncFC;
}

// [Check before Release...!!] cjjeong-20120405 : 
void CDgnClassIREW::CD2ID_RPSD(T_RPSC_SSM_D& cRpsd, S_RPSC_SSM_D& iRpsd)
{
	iRpsd.Initialize();
	int nSize = cRpsd.Mbar[0].GetSize();
	iRpsd.aMbar.SetSize(nSize);
	for (int i=0; i<nSize; i++) CD2ID_RPMS(cRpsd.Mbar[0].GetAt(i), iRpsd.aMbar[i]);
	CD2ID_RPSS(cRpsd.Sbar[0], iRpsd.Sbar);
	iRpsd.bCracked = cRpsd.bCracked;

}
void CDgnClassIREW::CD2ID_TNDN(T_GSEC_TNDN& cTnsn, S_SECG_TNDN& iTnsn, CString& szTName, UINT nTendonKey)
{
	iTnsn.Initialize();

	iTnsn.bPrestressed = cTnsn.bPrestressed;
	iTnsn.nTendonType  = cTnsn.nTendonType;
	iTnsn.nLoadType    = cTnsn.nLoadType;
	iTnsn.nBondType    = cTnsn.nBondType;
	iTnsn.dDia         = cTnsn.dDia;
	iTnsn.dDuctDia     = cTnsn.dDuctDia;
	iTnsn.dAp			     = cTnsn.dAp;
	iTnsn.dyz[0]	     = cTnsn.dyz[0];
	iTnsn.dyz[1]	     = cTnsn.dyz[1];
	iTnsn.dEp			     = cTnsn.dEp;
	iTnsn.dDp			     = cTnsn.dDp;
	iTnsn.dPp          = cTnsn.dPp;
	iTnsn.szTendonName = szTName;
	iTnsn.nTendonKey   = nTendonKey;
}

BOOL CDgnClassIREW::MakeTdlsData(CDBDoc* pDoc, T_KEY& nTndnK, T_KEY& nStagK, T_KEY& nStepK, S_TDLS_D& TdlsD)
{
	int nStep;
	CArray<double, double> aTendonForce;
	T_TENDON_INFO tendon_info;
	T_TENDON_INFO_ELEM tendon_info_elem;
	T_TENDON_INFO_BASE tendon_info_base;
	
	TdlsD.Initialize();
	
	if(!pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(nStagK,nStepK,nStep)) 
		return FALSE;
	if(!pDoc->m_pPostCtrl->GetTdfc(nTndnK, nStep, aTendonForce)) 
		return FALSE;
	if(!pDoc->m_pPostCtrl->GetTendonInfo()->GetTendonInfo(nTndnK, tendon_info)) 
		return FALSE;
	
	int nCount = 0;
	for(int i=0; i<tendon_info.aTendonInfoElem.GetSize(); i++)
	{
		tendon_info_elem = tendon_info.aTendonInfoElem[i];
		for(int j=0; j< tendon_info_elem.aTendonInfoBase.GetSize(); j++)
		{
			tendon_info_base = tendon_info_elem.aTendonInfoBase[j];
			TdlsD.aTendonPos.Add(tendon_info_base.dTendonLength);
			TdlsD.aTendonFrc.Add(aTendonForce[nCount]);
			
			nCount++;
		}
	}
	
	ASSERT(nCount == TdlsD.aTendonPos.GetSize());
	
	return TRUE;

}

void CDgnClassIREW::CD2ID_THIK(T_THIK_D& cThik, S_THIK_D& iThik)
{
	iThik.Initialize();

	iThik.nType       = cThik.nType;
	// Value
	iThik.nThickType  = cThik.Value.nThickType;
	iThik.ThickIn     = cThik.Value.ThickIn;
	iThik.ThickOut    = cThik.Value.ThickOut;
	iThik.bOffset     = cThik.Value.bOffset;
	iThik.nOffsetType = cThik.Value.nOffsetType;
	iThik.dValue      = cThik.Value.dValue;
	iThik.dRatio      = cThik.Value.dRatio;

	// Stiffened
	iThik.nStifType  = cThik.Stiffened.nType;
	iThik.szDBName   = cThik.Stiffened.DBName;
	iThik.dPlateThik = cThik.Stiffened.dPlateThik;
	iThik.dPlateThik = cThik.Stiffened.dPlateThik;
	iThik.nRibPos    = cThik.Stiffened.nRibPosition;

	for (int i=0; i<2; i++)
	{
		T_THIK_STIFFENED_VALUE& rValue = (i==0) ? cThik.Stiffened.yzValue: cThik.Stiffened.xzValue;

		iThik.szStVShape[i] = rValue.Shape;
		iThik.dThickIn[i]   = rValue.ThickIn;
		iThik.dThickOut[i]  = rValue.ThickOut;
		iThik.dHu[i]        = rValue.Hu;
		iThik.dHl[i]        = rValue.Hl;
		
		T_THIK_STIFFENED_SECT& rSect = (i==0) ? cThik.Stiffened.yzSect: cThik.Stiffened.xzSect;
		iThik.bRibAttach[i] = rSect.bRibAttached;
		iThik.szStSShape[i] = rSect.Shape;
		iThik.szName[i]     = rSect.SName;
		iThik.dDist[i]      = rSect.Dist;
		memcpy(iThik.dSize[i], rSect.Size, sizeof(iThik.dSize[i]));    
		iThik.dPeriIn[i]    = rSect.PeriIn;
		iThik.dPeriOut[i]   = rSect.PeriOut;
	}
}

void CDgnClassIREW::CD2ID_STRP(T_STRP_DL& cStrp, S_STRP_DL& iStrp)
{
	iStrp.Initialize();

	for (int i=0; i<5; i++)
	{
		iStrp.dblStress[i][0] = cStrp.dblStress[i][14]; // Fxx
		iStrp.dblStress[i][1] = cStrp.dblStress[i][15]; // Fyy
		iStrp.dblStress[i][2] = cStrp.dblStress[i][16]; // Fxy
		iStrp.dblStress[i][3] = cStrp.dblStress[i][20]; // Mxx
		iStrp.dblStress[i][4] = cStrp.dblStress[i][21]; // Myy
		iStrp.dblStress[i][5] = cStrp.dblStress[i][22]; // Mxy
		iStrp.dblStress[i][6] = cStrp.dblStress[i][26]; // Vxx
		iStrp.dblStress[i][7] = cStrp.dblStress[i][27]; // Vyy
	}
}

void CDgnClassIREW::CD2ID_STPN(T_STPN_D& cStrp, S_STRP_DL& iStrp)  // Plane Strain
{
	iStrp.Initialize();
	
	for (int i=0; i<5; i++)
	{
		iStrp.dblStress[i][0] = cStrp.dblStress[i][0]; // Sig-xx
		iStrp.dblStress[i][1] = cStrp.dblStress[i][1]; // Sig-yy 
		iStrp.dblStress[i][2] = cStrp.dblStress[i][2]; // Sig-zz 
		iStrp.dblStress[i][3] = cStrp.dblStress[i][3]; // Sig-xy 
		iStrp.dblStress[i][4] = cStrp.dblStress[i][6]; // Sig-P1 
		iStrp.dblStress[i][5] = cStrp.dblStress[i][7]; // Sig-P2 
		iStrp.dblStress[i][6] = cStrp.dblStress[i][8]; // Sig-P3
		iStrp.dblStress[i][7] = cStrp.dblStress[i][9]; // Max-Shear 
	}
}
void CDgnClassIREW::CD2ID_STRM(T_STRM_DL& cStrp, S_STRP_DL& iStrp)  // Plane Stress
{
	iStrp.Initialize();
	
	for (int i=0; i<5; i++)
	{
		for (int j=0; j<8; j++)
		{
			iStrp.dblStress[i][j] = cStrp.dblStress[i][j];
		}    
	}
}


// for construction Stage
void CDgnClassIREW::CD2ID_CSLC(T_STLD_D& cStld, S_STLD_D& iCslc)
{
	iCslc.Initialize();
	iCslc.szLoadCaseName = cStld.LoadCaseName;
	iCslc.szLoadCaseType = cStld.LoadCaseType;
	iCslc.szDescription  = cStld.Description;
	iCslc.nLoadCaseId    = cStld.LoadCaseId;
}

void CDgnClassIREW::CD2ID_CLAT(T_STLD_D& cStld, S_STLD_D& iClat)
{
	CD2ID_CSLC(cStld, iClat);
}

void CDgnClassIREW::CD2ID_STAG(T_STAG_D& cStag, S_STAG_D& iStag)
{
	iStag.Initialize();
	iStag.szStageName = cStag.StageName;
	iStag.dDuration = cStag.dDuration;
	iStag.nStageId  = cStag.nStageId;

	int i(0), nSize(0);
	iStag.aUserDefStep.Copy(cStag.aUserDefStep);
	nSize = cStag.aActiveElem.GetSize();
	iStag.aActiveElem.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		iStag.aActiveElem[i].GrupKey = cStag.aActiveElem[i].GrupKey;
		iStag.aActiveElem[i].dAge    = cStag.aActiveElem[i].dAge;
	}

	nSize = cStag.aDeactiveElem.GetSize();
	iStag.aDeactiveElem.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		iStag.aDeactiveElem[i].GrupKey         = cStag.aDeactiveElem[i].GrupKey;
		iStag.aDeactiveElem[i].dRedistribution = cStag.aDeactiveElem[i].dRedistribution;
	}

	nSize = cStag.aActiveBndr.GetSize();
	iStag.aActiveBndr.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		iStag.aActiveBndr[i].BngrKey  = cStag.aActiveBndr[i].BngrKey;
		iStag.aActiveBndr[i].nPosition= cStag.aActiveBndr[i].nPosition;
	}
	iStag.aDeactiveBndr.Copy(cStag.aDeactiveBndr);

	nSize = cStag.aActiveLoad.GetSize();
	iStag.aActiveLoad.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		iStag.aActiveLoad[i].LdgrKey = cStag.aActiveLoad[i].LdgrKey;
		iStag.aActiveLoad[i].dDay    = cStag.aActiveLoad[i].dDay;
	}
	
	nSize = cStag.aDeactiveLoad.GetSize();
	iStag.aDeactiveLoad.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		iStag.aDeactiveLoad[i].LdgrKey = cStag.aDeactiveLoad[i].LdgrKey;
		iStag.aDeactiveLoad[i].dDay    = cStag.aDeactiveLoad[i].dDay;
	}
}

void CDgnClassIREW::CD2ID_CSCS(T_CSCS_D& cCscs, S_CSCS_D& iCscs)
{
	iCscs.Initialize();
	iCscs.nSectKey    = cCscs.SectKey;
	iCscs.nActStagKey = cCscs.ActStagKey;
	iCscs.nCompType  = cCscs.nCompType;

	int i(0), nSize = cCscs.aPart.GetSize();
	iCscs.aPart.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		iCscs.aPart[i].nPart        = cCscs.aPart[i].nPart;
		iCscs.aPart[i].nMatlKey     = cCscs.aPart[i].MatlKey;
		iCscs.aPart[i].nCompStagKey = cCscs.aPart[i].CompStagKey;
		iCscs.aPart[i].dAge         = cCscs.aPart[i].dAge;
		CD2ID_SRSF(cCscs.aPart[i].StiffScale, iCscs.aPart[i].StiffScale);
		CD2ID_SRSF(cCscs.aPart[i].Stiffness,  iCscs.aPart[i].Stiffness);
		CD2ID_SRSF(cCscs.aPart[i].StiffnessJ, iCscs.aPart[i].StiffnessJ);
		iCscs.aPart[i].YBar  = cCscs.aPart[i].YBar;
		iCscs.aPart[i].ZBar  = cCscs.aPart[i].ZBar;
		iCscs.aPart[i].YBarJ = cCscs.aPart[i].YBarJ;
		iCscs.aPart[i].ZBarJ = cCscs.aPart[i].ZBarJ;
		iCscs.aPart[i].dH    = cCscs.aPart[i].dH;
	}
}

void CDgnClassIREW::CD2ID_GRUP(T_GRUP_D& cGrup, S_GRUP_D& iGrup)
{
	iGrup.Initialize();
	iGrup.szGroupName = cGrup.GroupName;
	iGrup.nGroupId    = cGrup.nGroupId;

	iGrup.arKeyNode.Copy(cGrup.arKeyNode);
	iGrup.arKeyElem.Copy(cGrup.arKeyElem);
}

void CDgnClassIREW::CD2ID_BNGR(T_BNGR_D& cBngr, S_BNGR_D& iBngr)
{
	iBngr.Initialize();
	iBngr.szGroupName = cBngr.GroupName;
	iBngr.nGroupId  = cBngr.nGroupId;
}

void CDgnClassIREW::CD2ID_LDGR(T_LDGR_D& cLdgr, S_LDGR_D& iLdgr)
{
	iLdgr.Initialize();
	iLdgr.szGroupName = cLdgr.GroupName;
	iLdgr.nGroupId  = cLdgr.nGroupId;
}

void CDgnClassIREW::CD2ID_PREA(T_REAC_D& cReac, S_REAC_D& iReac)
{
	iReac.Initialize();
	// MNET:XXXX-CJJEONG-20130829 (2088) : 자유도 갯수 변경으로 인한 수정 -> 7자유도 반영해야 하나?
	for (int i=0; i<6; ++i) iReac.dblReac[i] = cReac.dblReac[i];  
}

void CDgnClassIREW::CD2ID_PDIS(T_DISP_D& cDisp, S_DISP_D& iDisp)
{
	iDisp.Initialize();
	// MNET:XXXX-CJJEONG-20130829 (2088) : 자유도 갯수 변경으로 인한 수정 -> 7자유도 반영해야 하나?
	for (int i=0; i<6; ++i) iDisp.dblDisp[i] = cDisp.dblDisp[i];    
}

void CDgnClassIREW::CD2ID_PTFO(T_STRT_D& cTfor, S_TFOR_D& iTfor)
{
	iTfor.Initialize();
	// MNET:XXXX-CJJEONG-20130829 (2088) : 자유도 갯수 변경으로 인한 수정 -> 7자유도 반영해야 하나?
	for (int i=0; i<2; ++i) iTfor.dblForce[i] = cTfor.dblForce[i];      
}

void CDgnClassIREW::CD2ID_PBFO(T_STRB_D& cPbfo, S_BFOR_D& iPbfo)
{
	iPbfo.Initialize();
	// MNET:XXXX-CJJEONG-20130829 (2088) : 자유도 갯수 변경으로 인한 수정 -> 7자유도 반영해야 하나?
	for (int i=0; i<4; ++i)
	{
		iPbfo.dblForce[i][0][0] = cPbfo.dblForce[i][0];
		iPbfo.dblForce[i][0][1] = cPbfo.dblForce[i][1];
		iPbfo.dblForce[i][0][2] = cPbfo.dblForce[i][2];
		iPbfo.dblForce[i][0][3] = cPbfo.dblForce[i][3];
		iPbfo.dblForce[i][0][4] = cPbfo.dblForce[i][4];
		iPbfo.dblForce[i][0][5] = cPbfo.dblForce[i][5];
		
		iPbfo.dblForce[i][1][0] = cPbfo.dblForce[i][6];
		iPbfo.dblForce[i][1][1] = cPbfo.dblForce[i][7];
		iPbfo.dblForce[i][1][2] = cPbfo.dblForce[i][8];
		iPbfo.dblForce[i][1][3] = cPbfo.dblForce[i][9];
		iPbfo.dblForce[i][1][4] = cPbfo.dblForce[i][10];
		iPbfo.dblForce[i][1][5] = cPbfo.dblForce[i][11];    
	}
}

void CDgnClassIREW::CD2ID_PREC(T_REAC_D& cRecx, T_REAC_D& cRecn, S_RECC_D& iReac)
{
	iReac.Initialize();

	// MNET:XXXX-CJJEONG-20130829 (2088) : 자유도 갯수 변경으로 인한 수정 -> 7자유도 반영해야 하나?
	for (int i=0; i<6; ++i) iReac.dblReac[i]   = cRecx.dblReac[i];  
	for (int i=0; i<6; ++i) iReac.dblReac[i+6] = cRecn.dblReac[i];  
}

void CDgnClassIREW::CD2ID_PDIC(T_DISP_D& cDspx, T_DISP_D& cDspn, S_DSPC_D& iDspc)
{
	iDspc.Initialize();
	// MNET:XXXX-CJJEONG-20130829 (2088) : 자유도 갯수 변경으로 인한 수정 -> 7자유도 반영해야 하나?
	for (int i=0; i<6; ++i) iDspc.dblDisp[i]   = cDspx.dblDisp[i];  
	for (int i=0; i<6; ++i) iDspc.dblDisp[i+6] = cDspn.dblDisp[i];  
}

void CDgnClassIREW::CD2ID_PBCF(T_SBCF_D& cPbcx, T_SBCF_D& cPbcfn, S_BCFO_B& iPbcf)
{
	iPbcf.Initialize();
	int i(0), j(0), k(0), l(0);
	for (i=0; i<4; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<6; k++)
			{
				for (l=0; l<6; l++)
				{
					iPbcf.dblForce[i][j][k][l] = cPbcx.dblForce[i][j][k][l];
					iPbcf.dblForce[i][j][k+6][l] = cPbcfn.dblForce[i][j][k][l];
				}
			}
		}
	}
}

void CDgnClassIREW::CD2ID_CPUR(T_SECT_D& cSectD, S_SECT_D& iSectD)
{
	// MNET:XXXX-CJJEONG-20130904 (2088)
	// 물고있는 단면이 Tapered 이면 J단을 가져갈 방법이 없어서 파트 정보에 넣어준다. Shape이랑 Size, Stiffness만 있음.
	CD2ID_STBS(cSectD.SectBefore.nStype, cSectD.SectBefore.SectI, iSectD.SectI);
	if (cSectD.SectBefore.SectJ.Shape.IsEmpty() == FALSE)
	{
		S_GSEC_PART GSecPart;

		GSecPart.nBuiltUpFlag = cSectD.SectBefore.SectJ.BuiltUpFlag;
		GSecPart.dPeriIn      = cSectD.SectBefore.SectJ.PeriIn;
		GSecPart.dPeriOut     = cSectD.SectBefore.SectJ.PeriOut;

		memcpy(GSecPart.dSize, cSectD.SectBefore.SectJ.Size, sizeof(GSecPart.dSize));

		CD2ID_SRSF(cSectD.SectBefore.SectJ.Stiffness, GSecPart.Stiffness);  
		iSectD.SectI.aSectPart.Add(GSecPart);
	}

	CD2ID_STBS(cSectD.SectAfter.nStype,  cSectD.SectAfter.SectI,  iSectD.SectJ);   
	if (cSectD.SectAfter.SectJ.Shape.IsEmpty() == FALSE)
	{
		S_GSEC_PART GSecPart;

		GSecPart.nBuiltUpFlag = cSectD.SectAfter.SectJ.BuiltUpFlag;
		GSecPart.dPeriIn      = cSectD.SectAfter.SectJ.PeriIn;
		GSecPart.dPeriOut     = cSectD.SectAfter.SectJ.PeriOut;

		memcpy(GSecPart.dSize, cSectD.SectAfter.SectJ.Size, sizeof(GSecPart.dSize));
		CD2ID_SRSF(cSectD.SectAfter.SectJ.Stiffness, GSecPart.Stiffness);  
		iSectD.SectJ.aSectPart.Add(GSecPart);
	}
}

BOOL CDgnClassIREW::WriteBASC(CDgnIDEAArchive&  ar, CString& szMCTPath)
{
	// Version
	ar.endl();
	UINT nVer = D_IDEARSFILE_VERSION;
	ar << nVer;   
	ar << m_Data.GetNodeCount();  // Numbder of Node
	ar << m_Data.GetElemCount();  // Numbder of Element
	ar << m_Data.GetMembCount();  // Numbder of Member  

	ar << m_Data.GetMatlADCount();  // Material - Added Information
	ar << m_Data.GetMatlSTCount();  // Material - Steel
	ar << m_Data.GetMatlCNCount();  // Material - Concrete
	ar << m_Data.GetMatlRBCount();  // Material - Rebar    

	ar << m_Data.GetSectCount();  // SectD
	ar << m_Data.GetElstCount();
	ar << m_Data.GetConsCount();  // Constraint - Boundary  

	ar << m_Data.GetStldCount();  // Load Case
	ar << m_Data.GetLcomCount();  // Load Combination

	ar << m_Data.GetLengCount();  // Unbraced Length
	ar << m_Data.GetKfacCount();  // Effective Length Factor(K)
	ar << m_Data.GetLtsrCount();  // Limiting Slenderness Ratio
	ar << m_Data.GetCmftCount();  // Equivalent Moment Correction Factor(Cm)
	ar << m_Data.GetFmagCount();  // Moment Magnifier

	ar << m_Data.GetRdbmCount();  // Beam Section Data for Design
	ar << m_Data.GetRdcnCount();  // Column Section Data for Design
	ar << m_Data.GetRcbmCount();  // Beam Section Data for Checking
	ar << m_Data.GetRccnCount();  // Column Section Data for Checking

	ar << m_Data.GetRebtCount();  // Rebar Input for General Section
	ar << m_Data.GetRpscCount();  // Rebar for PSC
	ar << m_Data.GetTdifCount();  // endon of Cross section

	//ar << m_Data.GetReacCount();  // Reaction NodeK + LoadCaseK
	ar << 0;
	ar << m_Data.GetDispCount();  // Displacement NodeK + LoadCaseK
	ar << m_Data.GetTforCount();  // Truss Force ElemK + LoadCaseK 
	ar << m_Data.GetBforCount();  // Beam Force ElemK + LoadCaseK  
	//ar << m_Data.GetReccCount();  // Reaction Nok
	ar << 0;
	ar << m_Data.GetDspcCount();  // Displacement NodeK + LoadCaseK
	ar << m_Data.GetBcfoCount();  // Beam concurrent Force ElemK + LoadCaseK  

	// for Construction Stage
	ar << m_Data.GetThikCount();
	ar << m_Data.GetStrpCount();
	ar << m_Data.GetCslcCount();  // CS Load Case
	ar << m_Data.GetClatCount();  // CS Load Case
	ar << m_Data.GetStagCount();  // Construction Stage
	ar << m_Data.GetCscsCount();  // Composite Section for Construction Stage
	ar << m_Data.GetGrupCount();  // Structure Group
	ar << m_Data.GetBngrCount();  // Boundary Group
	ar << m_Data.GetLdgrCount();  // Load Group
	ar << m_Data.GetDispCountCS();  // Displacement Node * LoadCase * Stage
	ar << m_Data.GetTforCountCS();  // Truss Force Node * LoadCase * Stage
	ar << m_Data.GetBforCountCS();  // Beam Force Node * LoadCase * Stage
	ar << m_Data.GetStrpCountCS();

	ar << m_Data.GetCsetCount();
	ar << m_Data.GetTdlsCount();

	// Composite Section Result
	ar << m_Data.GetBforPTCount();
	ar << m_Data.GetBforPTCCCount();
	ar << m_Data.GetBforPTCSCount();

	ar << szMCTPath;
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteUNIT(CDgnIDEAArchive&  ar, S_UNIT_D&  UnitD)
{
	ar << UnitD.nBase_Length;
	ar << UnitD.nBase_Force;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteNODE(CDgnIDEAArchive&  ar, UINT nKey, S_NODE_D& NodeD)
{
	ar << nKey;
	ar << NodeD.dx;
	ar << NodeD.dy;
	ar << NodeD.dz;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteELEM(CDgnIDEAArchive&ar, UINT nKey, S_ELEM_D& ElemD)
{
	ar << nKey;
	ar << ElemD.nEltype;
	ar << ElemD.MatlK;
	ar << ElemD.SectK;
	ar << ElemD.dAngle;
	for (int i=0; i<DT_ELEM_MAXNOD; i++) ar << ElemD.NodeK[i];

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteMEMB(CDgnIDEAArchive&ar, UINT nKey, S_MEMB_D& MembD)
{
	ar << nKey;
	ar << MembD.nMbType;
	int i(0), nSize = MembD.aElemK.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++) ar << MembD.aElemK[i];
	ar << MembD.bReverseDir;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteMtPr(CDgnIDEAArchive&ar, S_MATL_PROP& MtPrD)
{
	ar.WriteText(_T("%s \n"), _LSX(** Material Property **));
	ar << MtPrD.dElast;
	ar << MtPrD.dPoisson;
	ar << MtPrD.dThermal;
	ar << MtPrD.dDensity;
	ar << MtPrD.dMassDensity;
	ar << MtPrD.dShearModulus;
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteMTST(CDgnIDEAArchive& ar, UINT nKey, S_MATL_STEEL_D& MtstD)
{
	ar << nKey;
	ar << MtstD.szCode;
	ar << MtstD.szMatName;
	WriteMtPr(ar, MtstD.PropD);  
	ar << MtstD.dS_Fu;
	ar << MtstD.dS_Fy;
	ar << MtstD.dS_Fy1;
	ar << MtstD.dS_Fy2;
	ar << MtstD.dS_Fy3;
	ar << MtstD.dS_Fy4;
	ar << MtstD.dS_Fy5;
	ar << MtstD.dS_Fy6;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteMTCC(CDgnIDEAArchive& ar, UINT nKey, S_MATL_CONCRETE_D& MtccD)
{
	ar << nKey;
	ar << MtccD.szCode;
	ar << MtccD.szMatName;
	WriteMtPr(ar, MtccD.PropD);  
	ar << MtccD.dC_fc;
	
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteMTRB(CDgnIDEAArchive& ar, UINT nKey, S_MATL_REBAR_INFO& MtrbD)
{
	ar << nKey;
	ar << MtrbD.szRebar_CodeName;
	ar << MtrbD.szMainRebar_RebarName;
	ar << MtrbD.szSubRebar_RebarName;
	ar << MtrbD.MainRebarData.dB_fy;
	ar << MtrbD.MainRebarData.dB_Elast;
	ar << MtrbD.SubRebarData.dB_fy;
	ar << MtrbD.SubRebarData.dB_Elast;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteMTAD(CDgnIDEAArchive& ar, UINT nKey, S_MATL_ADIF_D& MtadD)
{
	ar << nKey;
	ar << MtadD.szType;
	ar << MtadD.szName;
	ar << MtadD.dSpecificHeat;
	ar << MtadD.dHeatConduction;
	ar << MtadD.nThermalUnit;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteSTSF(CDgnIDEAArchive& ar, S_SECT_STIFFNESS& StsfD)
{
	ar.WriteText(_T("%s \n"), _LSX(*** Section Stiffness ***));
	ar << StsfD.dArea;
	ar << StsfD.dAsy;
	ar << StsfD.dAsz;
	ar << StsfD.dRxx;
	ar << StsfD.dRyy;
	ar << StsfD.dRzz;
	ar << StsfD.dCyp;
	ar << StsfD.dCym;
	ar << StsfD.dCzp;
	ar << StsfD.dCzm;
	ar << StsfD.dQyb;
	ar << StsfD.dQzb;
	ar << StsfD.dWArea;
	ar << StsfD.dx1;
	ar << StsfD.dy1;
	ar << StsfD.dx2;
	ar << StsfD.dy2;
	ar << StsfD.dx3;
	ar << StsfD.dy3;
	ar << StsfD.dx4;
	ar << StsfD.dy4;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteSTPG(CDgnIDEAArchive& ar, S_GSEC_POLYGON& StpgD)
{  
	ar.WriteText(_T("%s \n"), _LSX(*** Section Polygon ***));
	int i(0), nSize = StpgD.aVertex.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << StpgD.aVertex[i].dX;
		ar << StpgD.aVertex[i].dY;
		ar.endl();
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteSECT(CDgnIDEAArchive& ar, UINT nKey, S_SECT_D& SectD)
{
	ar << nKey;

	for (int nIJ=0; nIJ<S_SECT_NUM; nIJ++)
	{
		S_SECT_SECTBASE_D&  Sect = (nIJ == 0) ? SectD.SectI : SectD.SectJ;
		ar.endl();
		ar.WriteText(_T("%s \n"), (nIJ == 0) ? _LSX(** Section I **) : _LSX(** Section J **));

		ar << Sect.nStype;
		ar << Sect.szShape;  
		ar << Sect.szDBName; 
		ar << Sect.szName;   
		ar << Sect.nPartBaseK; 
		ar.endl();

		int i(0), nSize=DT_SECT_SIZE_NUM_MAX;
		for (i=0; i<nSize; i++) ar << Sect.dSize[i];
		ar.endl();
		WriteSTCS(ar, nKey, Sect.PartProp);
		WriteSTSF(ar, Sect.Stiffness);
		
		nSize = Sect.aOuterPolygon.GetSize();
		ar << nSize;
		ar.endl();
		for (i=0; i<nSize; i++) WriteSTPG(ar, Sect.aOuterPolygon[i]);
		
		nSize = Sect.aInnerPolygon.GetSize();
		ar << nSize;
		ar.endl();
		for (i=0; i<nSize; i++) WriteSTPG(ar, Sect.aInnerPolygon[i]);
		
		// if aSectPart is empty, make one temporarily
		if(Sect.aSectPart.GetSize()<1 && Sect.aLine.GetSize()>0)
		{
			S_GSEC_PART GSecPart;
			GSecPart.Initialize();
			GSecPart.IdxStart[0] = 0;
			GSecPart.IdxEnd  [0] = 1;
			GSecPart.IdxStart[2] = 0;
			GSecPart.IdxEnd  [2] = Sect.aLine.GetSize();
			Sect.aSectPart.Add(GSecPart);
		}

		nSize  = Sect.aSectPart.GetSize();
		ar << nSize;
		for (i=0; i<nSize; i++) WriteCSPT(ar, 0, 0, 0, Sect.aSectPart[i], FALSE);
		ar.endl();

		WriteLINE(ar, Sect.aLine);    
		WriteLNLP(ar, Sect.aLineLoop);
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteELST(CDgnIDEAArchive&  ar, UINT nEKey, UINT nSKey, S_ELST_D& ElstD)
{
	ar << nEKey;
	ar << nSKey;
	
	S_SECT_SECTBASE_D*  SectDP = NULL;  
	for (int nIJ=0; nIJ<S_ELEMSECT_NUM; nIJ++)
	{
		ar.endl();
		switch(nIJ)
		{
		case 0: 
			SectDP = &ElstD.SectI;
			ar.WriteText(_T("%s \n"), _LSX(** Section I **));
			break;
		case 1: 
			SectDP = &ElstD.SectQ;
			ar.WriteText(_T("%s \n"), _LSX(** Section 1/4 **));
			break;
		case 2: 
			SectDP = &ElstD.SectH;
			ar.WriteText(_T("%s \n"), _LSX(** Section 1/2 **));
			break;
		case 3: 
			SectDP = &ElstD.SectT;
			ar.WriteText(_T("%s \n"), _LSX(** Section 3/4 **));
			break;
		case 4: 
			SectDP = &ElstD.SectJ;
			ar.WriteText(_T("%s \n"), _LSX(** Section J **));
			break;
		}
		
		ar << SectDP->nStype;
		ar << SectDP->szShape;
		ar << SectDP->szDBName;
		ar << SectDP->szName;
		ar << SectDP->nPartBaseK;
		ar.endl();
		int i(0), nSize=DT_SECT_SIZE_NUM_MAX;
		for (i=0; i<nSize; i++) ar << SectDP->dSize[i];
		WriteSTCS(ar, nSKey, SectDP->PartProp);
		WriteSTSF(ar, SectDP->Stiffness);
		
		nSize = SectDP->aOuterPolygon.GetSize();
		ar << nSize;
		ar.endl();
		for (i=0; i<nSize; i++) WriteSTPG(ar, SectDP->aOuterPolygon[i]);
		
		nSize = SectDP->aInnerPolygon.GetSize();
		ar << nSize;
		ar.endl();
		for (i=0; i<nSize; i++) WriteSTPG(ar, SectDP->aInnerPolygon[i]);
		
		nSize  = SectDP->aSectPart.GetSize();
		ar << nSize;
		for (i=0; i<nSize; i++) WriteCSPT(ar, 0, 0, 0, SectDP->aSectPart[i], FALSE);

		ar.endl();
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteCSET(CDgnIDEAArchive&  ar, UINT nEKey, UINT nSKey, UINT nPKey, S_CSET_D& CsetD)
{
	ar << nEKey;
	ar << nSKey;
	ar << nPKey;

	ar << CsetD.nActiveStage;
	int i=0, nSize=0;
	
	nSize = CsetD.aOPolyI.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aOPolyI[i]);
	
	nSize = CsetD.aIPolyI.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aIPolyI[i]);
	
	nSize = CsetD.aOPolyQ.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aOPolyQ[i]);
	
	nSize = CsetD.aIPolyQ.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aIPolyQ[i]);
	
	nSize = CsetD.aOPolyH.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aOPolyH[i]);
	
	nSize = CsetD.aIPolyH.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aIPolyH[i]);
	
	nSize = CsetD.aOPolyT.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aOPolyT[i]);
	
	nSize = CsetD.aIPolyT.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aIPolyT[i]);
	
	nSize = CsetD.aOPolyJ.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aOPolyJ[i]);
	
	nSize = CsetD.aIPolyJ.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteSTPG(ar, CsetD.aIPolyJ[i]);

	return TRUE;
}

BOOL CDgnClassIREW::WriteSTCS(CDgnIDEAArchive& ar, UINT nKey, S_PROP_D& StcsD)
{
	ar.WriteText(_T("%s \n"), _LSX(*** Section Part Property ***));
	ar << nKey;
	int i(0), nSize = StcsD.aPropD.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << StcsD.aPropD[i].SectK;
		ar << StcsD.aPropD[i].SectPartK;
		ar << StcsD.aPropD[i].SectIJK;
		ar << StcsD.aPropD[i].MatlK;
	}
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteGSPO(CDgnIDEAArchive& ar, S_GSPO_D& GspoD)
{
	ar.WriteText(_T("%s \n"), _LSX(*** Section Construction-Stage Outer Polygon ***));
	int i(0), nSize = GspoD.aPoint.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << GspoD.aPoint[i].dX;
		ar << GspoD.aPoint[i].dY;
		ar.endl();
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteGSPI(CDgnIDEAArchive& ar, S_GSPI_D& GspiD)
{  
	ar.WriteText(_T("%s \n"), _LSX(*** Section Construction-Stage Inner Polygon ***));
	int i(0), nSize = GspiD.aPoint.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << GspiD.aPoint[i].dX;
		ar << GspiD.aPoint[i].dY;
		ar.endl();
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteCSPT(CDgnIDEAArchive&  ar, UINT nSkey, UINT nPkey, UINT nIJ, S_GSEC_PART& CsptD, BOOL bPrintKey)
{
	ar.endl();
	ar.WriteText(_T("%s \n"), _LSX(*** Section Construction-Stage Part ***));
	if (bPrintKey == TRUE)
	{
		ar << nSkey;
		ar << nPkey;
		ar << nIJ;
	}
	ar << CsptD.szShape;
	ar << CsptD.szDBName;
	int i(0), nSize = DT_SECT_SIZE_NUM_MAX;
	for (i=0; i<nSize; i++) ar << CsptD.dSize[i];
	ar << CsptD.nBuiltUpFlag;

	ar.endl();
	WriteSTSF(ar, CsptD.Stiffness);
	ar.endl();
	WriteSTSF(ar, CsptD.StiffnessCom);

	ar << CsptD.dPeriIn;
	ar << CsptD.dPeriOut;
	ar << CsptD.dPeriInCom;
	ar << CsptD.dPeriOutCom;

	nSize = CsptD.aOuterPolygon.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteGSPO(ar, CsptD.aOuterPolygon[i]);
	nSize = CsptD.aInnerPolygon.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteGSPI(ar, CsptD.aInnerPolygon[i]);
	ar.endl();

	// IdxStart, IdxEnd
	ar.WriteText(_T("%s \n"), _LSX(*** IdxStart / IdxEnd ***));
	ar << CsptD.IdxStart[0] << CsptD.IdxStart[1] << CsptD.IdxStart[2] << CsptD.IdxStart[3] << CsptD.IdxStart[4]; ar.endl();
	ar << CsptD.IdxEnd  [0] << CsptD.IdxEnd  [1] << CsptD.IdxEnd  [2] << CsptD.IdxEnd  [3] << CsptD.IdxEnd  [4]; ar.endl();

	return TRUE;
}

BOOL CDgnClassIREW::WriteLINE(CDgnIDEAArchive& ar, const CArray<S_GSEC_LINE, S_GSEC_LINE&>& aLine)
{
	ar.WriteText(_T("%s \n"), _LSX(*** Line Information ***));

	int nSize = aLine.GetSize();
	ar << nSize; ar.endl();
	for(int i=0; i<nSize; i++)
	{
		ar << aLine[i].v1;
		ar << aLine[i].v2;
		ar << aLine[i].dThik;
		ar << aLine[i].nAlign;
		ar.endl();
	}
	ar.endl();

	return TRUE;
}

BOOL CDgnClassIREW::WriteLNLP(CDgnIDEAArchive& ar, const CArray<int, int>& aLineLoop)
{  
	ar.WriteText(_T("%s \n"), _LSX(*** Closed Loop Information ***));

	int nSize = aLineLoop.GetSize();
	ar << nSize; ar.endl();
	for(int i=0; i<nSize; i++) ar << aLineLoop[i];
	ar.endl();

	return TRUE;
}

BOOL CDgnClassIREW::WriteCONS(CDgnIDEAArchive& ar, UINT nKey, S_CONS_D& ConsD)
{
	ar << nKey;
	for (int i=0; i<6; i++) ar << ConsD.sConstraint[i];

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteSTLD(CDgnIDEAArchive& ar, UINT nKey, S_STLD_D& StldD)
{
	ar << nKey;
	ar << StldD.szLoadCaseName;
	ar << StldD.szLoadCaseType;
	ar << StldD.szDescription;
	ar << StldD.nLoadCaseId;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteLCBS(CDgnIDEAArchive& ar, S_LCOM_BASE& LcbsD)
{  
	ar.WriteText(_T("%s \n"), _LSX(** Load-Combination Base **));
	ar << LcbsD.nAnalType;
	ar << LcbsD.nLoadCaseKey;
	ar << LcbsD.dFactor;

	ar.endl();
	return TRUE;
}


BOOL CDgnClassIREW::WriteLCOM(CDgnIDEAArchive& ar, UINT nType, UINT nKey, S_LCOM_D& LcomD)
{
	ar << nType;
	ar << nKey;
	ar << LcomD.szLoadCombName;
	ar << LcomD.nLoadCombType;
	ar << LcomD.szDescription;
	ar << LcomD.nActive;
	ar << LcomD.nLoadCombId;
	ar.endl();
	for (int i=0; i<DT_LCOM_NUMCOMB; i++) WriteLCBS(ar, LcomD.Combination[i]);
	ar << LcomD.nServLcomType;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteLENG(CDgnIDEAArchive& ar, UINT nKey, S_LENG_D& LengD)
{
	ar << nKey;
	ar << LengD.dBLy;
	ar << LengD.dBLz;
	ar << LengD.bNotUseBLu;
	ar << LengD.dBLu;
	ar << LengD.bAutoCalcLe;  

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteKFAC(CDgnIDEAArchive& ar, UINT nKey, S_KFAC_D& KfacD)
{
	ar << nKey;
	ar << KfacD.dBKy;
	ar << KfacD.dBKz;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteLTSR(CDgnIDEAArchive& ar, UINT nKey, S_LTSR_D& LtsrD)
{
	ar << nKey;
	ar << LtsrD.bNotCheck;
	ar << LtsrD.dCLMT;
	ar << LtsrD.dTLMT;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteCMFT(CDgnIDEAArchive& ar, UINT nKey, S_CMFT_D& CmftD)
{
	ar << nKey;
	ar << CmftD.bAuto;
	ar << CmftD.dCmy;
	ar << CmftD.dCmz;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteFMAG(CDgnIDEAArchive& ar, UINT nKey, S_FMAG_D& FmagD)
{
	ar << nKey;
	ar << FmagD.dB1y;
	ar << FmagD.dB1z;
	ar << FmagD.dB2y;
	ar << FmagD.dB2z;
	
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteDCON(CDgnIDEAArchive& ar, S_DCON_D& DConD)
{
	ar << DConD.szDesignCode;
	for (int i=0; i<6; i++) ar << DConD.dPhi[i];
	ar << DConD.dRhoc;
	ar << DConD.dRhor;
	ar << DConD.bSpecialSeismic;
	ar << DConD.dMrft;
	ar << DConD.nSubCode;
	ar << DConD.dAlphacc;
	ar << DConD.dMinRhoc;
	ar << DConD.dStrutAngle;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteRDBM(CDgnIDEAArchive& ar, UINT nKey, S_RDGN_BEAM& RdbmD)
{
	ar << nKey;
	ar << RdbmD.dDt;
	ar << RdbmD.dDb;
	ar << RdbmD.dSubBarNum;
	ar << RdbmD.szSubBarNa;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteRDCN(CDgnIDEAArchive& ar, UINT nKey, S_RDGN_COLM& RbcnD)
{
	ar << nKey;
	ar << RbcnD.dDc;
	ar << RbcnD.dSubBarNum;
	ar << RbcnD.szSubBarNa;  

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteRCBM(CDgnIDEAArchive& ar, UINT nKey, S_RCHK_BEAM& RcbmD)
{
	ar << nKey;
	int i(0), j(0);
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			ar << RcbmD.dDt[i][j];
			ar << RcbmD.dDb[i][j];
			ar << RcbmD.nTopBarNum[i][j];
			ar << RcbmD.nBotBarNum[i][j];
			ar << RcbmD.szTopBarNa1[i][j];
			ar << RcbmD.szTopBarNa2[i][j];
			ar << RcbmD.szBotBarNa1[i][j];
			ar << RcbmD.szBotBarNa2[i][j];
		}
		ar << RcbmD.dSubBarNum[i];
		ar << RcbmD.dSubBarDist[i];
		ar << RcbmD.szSubBarNa[i];
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteRCCN(CDgnIDEAArchive& ar, UINT nKey, S_RCHK_COLM& RccnD)
{
	ar << nKey;
	int i(0), j(0);
	for (i=0; i<5; i++) ar << RccnD.dDc[i];
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{     
			ar << RccnD.nBarNum[i][j];
			ar << RccnD.szBarNa1[i][j];
			ar << RccnD.szBarNa2[i][j];
		}
	}
	ar << RccnD.dSubBarNum;
	ar << RccnD.dSubBarDist;
	ar << RccnD.szSubBarNa;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteREBT(CDgnIDEAArchive& ar, UINT nKey, S_REBT_D& RebtD)
{
	ar << nKey;
	ar << RebtD.nCenterType;
	int i(0), nSize = RebtD.aMainRebar.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << RebtD.aMainRebar[i].nLabel;
		ar << RebtD.aMainRebar[i].dCenterY;
		ar << RebtD.aMainRebar[i].dCenterZ;
		ar << RebtD.aMainRebar[i].szRebarName;
	}

	ar << RebtD.ShearRebar.dSubRebarSpace;
	ar << RebtD.ShearRebar.dSubRebarNum[0];
	ar << RebtD.ShearRebar.dSubRebarNum[1];
	ar << RebtD.ShearRebar.szSubRebarName;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteRPMS(CDgnIDEAArchive& ar, S_RPSC_MBAR_SSM& RpmsD)
{  
	ar.WriteText(_T("%s \n"), _LSX(** PSC Main Rebar **));
	ar << RpmsD.nType;
	ar << RpmsD.nRefY;
	ar << RpmsD.nRefZ;
	ar << RpmsD.Start.dPointY;
	ar << RpmsD.Start.dPointZ;
	ar << RpmsD.Center.dPointY;
	ar << RpmsD.Center.dPointZ;
	int i(0), nSize = RpmsD.aPassage.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << RpmsD.aPassage[i].dPointY;
		ar << RpmsD.aPassage[i].dPointZ;
	}
	ar << RpmsD.End.dPointY;
	ar << RpmsD.End.dPointZ;
	ar << RpmsD.dRadius;
	ar << RpmsD.nNumorCTC;
	ar << RpmsD.nNum;
	ar << RpmsD.dCTC;
	ar << RpmsD.bEdgeBar;
	ar << RpmsD.szBar;
	ar << RpmsD.nStandRef;
	ar << RpmsD.nPart;
	ar << RpmsD.nRefY_A;
	ar << RpmsD.nRefZ_A;
	ar << RpmsD.dRefY_A;
	ar << RpmsD.dRefZ_A;
	ar << RpmsD.dSpacing;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteRPSS(CDgnIDEAArchive& ar, S_RPSC_SBAR_SSM& RpssD)
{
	ar.WriteText(_T("%s \n"), _LSX(** PSC Sub Rebar **));
	ar << RpssD.bDiagonal;
	ar << RpssD.dDiagonalPitch;
	ar << RpssD.dDiagonalAngle;
	ar << RpssD.dDiagonalAw;
	ar << RpssD.bSteelBar;
	ar << RpssD.dSteelBarPitch;
	ar << RpssD.dSteelBarAngle;
	ar << RpssD.dSteelBarAp;
	ar << RpssD.dSteelBarPe;
	ar << RpssD.bTorsional;
	ar << RpssD.dTorsionalPitch;
	ar << RpssD.dTorsionalAwt;
	ar << RpssD.dTorsionalAlt;
	ar << RpssD.bLbarFlg;
	ar << RpssD.dLbarThick;
	ar << RpssD.bIncFC;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteRPSC(CDgnIDEAArchive& ar, UINT nKey, S_RPSC_SSM_D& RpscD)
{
	ar << nKey;
	int i(0), nSize = RpscD.aMbar.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++) WriteRPMS(ar, RpscD.aMbar[i]);
	WriteRPSS(ar, RpscD.Sbar);
	ar << RpscD.bCracked;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteTDIF(CDgnIDEAArchive& ar, UINT nKey, UINT nIJ, S_TDIF_D& TdifD)
{
	ar << nKey;
	ar << nIJ;
	int i(0), nSize = TdifD.aTndn.GetSize();
	ar << nSize;
	ar.endl();
	for (i=0; i<nSize; i++)
	{
		ar << TdifD.aTndn[i].bPrestressed;
		ar << TdifD.aTndn[i].nTendonType;
		ar << TdifD.aTndn[i].nLoadType;
		ar << TdifD.aTndn[i].nBondType;
		ar << TdifD.aTndn[i].dDia;
		ar << TdifD.aTndn[i].dDuctDia;
		ar << TdifD.aTndn[i].dAp;
		ar << TdifD.aTndn[i].dyz[0];
		ar << TdifD.aTndn[i].dyz[1];
		ar << TdifD.aTndn[i].dEp;
		ar << TdifD.aTndn[i].dDp;
		ar << TdifD.aTndn[i].dPp;
		ar << TdifD.aTndn[i].szTendonName;
		ar << TdifD.aTndn[i].nTendonKey;
		ar.endl();
	}
	
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteTDLS(CDgnIDEAArchive&  ar, UINT nKey, UINT nStag, UINT nStep, S_TDLS_D& TdlsD)
{
	ar << nKey;
	ar << nStag;
	ar << nStep;
	int i(0), nSize = TdlsD.aTendonFrc.GetSize();
	if (TdlsD.aTendonFrc.GetSize() != TdlsD.aTendonPos.GetSize())
	{
		ASSERT(FALSE);
		nSize = MIN(TdlsD.aTendonPos.GetSize(), TdlsD.aTendonFrc.GetSize());
	}
	ar << nSize;
	ar.endl();

	for (i=0; i<nSize; i++)
	{
		ar << TdlsD.aTendonPos[i];
		ar << TdlsD.aTendonFrc[i];
		ar.endl();
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteTHIK(CDgnIDEAArchive&  ar, UINT nKey, S_THIK_D& iThik)
{
	ar.WriteText(_T("%s \n"), _LSX(** Plate Thickness **));

	ar << nKey;
	ar << iThik.nType;
	ar << iThik.nThickType;
	ar << iThik.ThickIn;
	ar << iThik.ThickOut;
	ar << iThik.bOffset;
	ar << iThik.nOffsetType;
	ar << iThik.dValue;
	ar << iThik.dRatio;

	ar << iThik.nStifType;
	ar << iThik.szDBName;
	ar << iThik.dPlateThik;
	ar << iThik.dPlateThik;
	ar << iThik.nRibPos;

	int i(0), j(0);
	for (i=0; i<2; i++)
	{
		// Value
		ar << iThik.szStVShape[i];
		ar << iThik.dThickIn[i];
		ar << iThik.dThickOut[i];
		ar << iThik.dHu[i];
		ar << iThik.dHl[i];
	}

	for (i=0; i<2; i++)
	{
		// Sect
		ar << iThik.bRibAttach[i];
		ar << iThik.szStSShape[i];
		ar << iThik.szName[i];
		ar << iThik.dDist[i];
		for (j=0; j<10; j++) ar << iThik.dSize[i][j];
		ar << iThik.dPeriIn[i];
		ar << iThik.dPeriOut[i];
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteSTRP(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_STRP_DL& iStrp)
{
	ar.WriteText(_T("%s \n"), _LSX(** Plate Result **));
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	ar.endl();

	for (int i=0; i<5; i++)
	{
		for (int j=0; j<8; j++)
		{
			ar << iStrp.dblStress[i][j];
		}
		ar.endl();
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteSTRPCS(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_STRP_CSDL& iStrp)
{
	ar.WriteText(_T("%s \n"), _LSX(** Plate Result **));
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	int nSize = iStrp.aSTRP.GetSize();
	ar << nSize;
	ar.endl();
	
	for (int i=0; i<nSize; i++)
	{
		for (int j=0; j<5; j++)
		{
			for (int k=0; k<8; k++)
			{
				ar << iStrp.aSTRP[i].dblStress[j][k];
			}
			ar.endl();
		}
	}
	
	ar.endl();
	return TRUE;
}

// for construction Stage
BOOL CDgnClassIREW::WriteCSLC(CDgnIDEAArchive&  ar, UINT nKey, S_STLD_D& iCslc)
{
	ar << nKey;
	ar << iCslc.szLoadCaseName;
	ar << iCslc.szLoadCaseType;
	ar << iCslc.szDescription;
	ar << iCslc.nLoadCaseId;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteCLAT(CDgnIDEAArchive&  ar, UINT nKey, S_STLD_D& iClat)
{
	return WriteCSLC(ar, nKey, iClat);
}

BOOL CDgnClassIREW::WriteSTAG(CDgnIDEAArchive&  ar, UINT nKey, S_STAG_D& iStag)
{
	int i(0), nSize(0);
	ar << nKey;
	ar << iStag.szStageName;
	ar << iStag.dDuration;

	nSize = iStag.aUserDefStep.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++) ar << iStag.aUserDefStep[i];

	nSize = iStag.aActiveElem.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << iStag.aActiveElem[i].GrupKey;
		ar << iStag.aActiveElem[i].dAge;
	}

	nSize = iStag.aDeactiveElem.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << iStag.aDeactiveElem[i].GrupKey;
		ar << iStag.aDeactiveElem[i].dRedistribution;
	}
	
	nSize = iStag.aActiveBndr.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << iStag.aActiveBndr[i].BngrKey;
		ar << iStag.aActiveBndr[i].nPosition;
	}
	nSize = iStag.aDeactiveBndr.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++) ar << iStag.aDeactiveBndr[i];

	
	nSize = iStag.aActiveLoad.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << iStag.aActiveLoad[i].LdgrKey;
		ar << iStag.aActiveLoad[i].dDay;
	}
	
	nSize = iStag.aDeactiveLoad.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << iStag.aDeactiveLoad[i].LdgrKey;
		ar << iStag.aDeactiveLoad[i].dDay;
	}
	
	ar << iStag.nStageId;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteCSCS(CDgnIDEAArchive&  ar, UINT nKey, S_CSCS_D& iCscs)
{
	int i(0), nSize(0);
	ar << nKey;
	ar << iCscs.nSectKey;
	ar << iCscs.nActStagKey;
	ar << iCscs.nCompType;

	nSize = iCscs.aPart.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++)
	{
		ar << iCscs.aPart[i].nPart;
		ar << iCscs.aPart[i].nMatlKey;
		ar << iCscs.aPart[i].nCompStagKey;
		ar << iCscs.aPart[i].dAge;
		ar.endl();
		WriteSTSF(ar, iCscs.aPart[i].StiffScale);
		ar.endl();
		WriteSTSF(ar, iCscs.aPart[i].Stiffness);
		ar.endl();
		WriteSTSF(ar, iCscs.aPart[i].StiffnessJ);    
		ar.endl();
		ar << iCscs.aPart[i].YBar;
		ar << iCscs.aPart[i].ZBar;
		ar << iCscs.aPart[i].YBarJ;
		ar << iCscs.aPart[i].ZBarJ;
		ar << iCscs.aPart[i].dH;
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteGRUP(CDgnIDEAArchive&  ar, UINT nKey, S_GRUP_D& iGrup)
{
	int i(0), nSize(0);
	ar << nKey;
	ar << iGrup.szGroupName;  

	nSize = iGrup.arKeyNode.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++) ar << iGrup.arKeyNode[i];

	nSize = iGrup.arKeyElem.GetSize();
	ar << nSize;
	for (i=0; i<nSize; i++) ar << iGrup.arKeyElem[i];

	ar << iGrup.nGroupId;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteBNGR(CDgnIDEAArchive&  ar, UINT nKey, S_BNGR_D& iBngr)
{
	ar << nKey;
	ar << iBngr.szGroupName;
	ar << iBngr.nGroupId;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteLDGR(CDgnIDEAArchive&  ar, UINT nKey, S_LDGR_D& iLdgr)
{
	ar << nKey;
	ar << iLdgr.szGroupName;
	ar << iLdgr.nGroupId;

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteREAC(CDgnIDEAArchive& ar, UINT nEKey, UINT nLKey, UINT nType, S_REAC_D& ReacD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	for (int i=0; i<6; i++) ar << ReacD.dblReac[i];

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteDISP(CDgnIDEAArchive& ar, UINT nEKey, UINT nLKey, UINT nType, S_DISP_D& DispD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	for (int i=0; i<6; i++) ar << DispD.dblDisp[i];

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteDISPCS(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_DISP_CSD& DispD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;

	int nSize = DispD.aDisp.GetSize();
	ar << nSize;
	ar.endl();

	for (int i=0; i<nSize; i++)
	{
		for (int j=0; j<6; j++) 
			ar << DispD.aDisp[i].dblDisp[j];
		ar.endl();

	}  
	
	return TRUE;

}

BOOL CDgnClassIREW::WriteTFOR(CDgnIDEAArchive& ar, UINT nEKey, UINT nLKey, UINT nType, S_TFOR_D& TforD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	ar << TforD.dblForce[0];
	ar << TforD.dblForce[1];

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteTFORCS(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_TFOR_CSD& TforD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	
	int nSize = TforD.aTFOR.GetSize();
	ar << nSize;
	ar.endl();

	for (int i=0; i<nSize; i++)
	{
		ar << TforD.aTFOR[i].dblForce[0];
		ar << TforD.aTFOR[i].dblForce[1];
		ar.endl();
	}
	
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteBFOR(CDgnIDEAArchive& ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_D& BforD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	int i(0), j(0), k(0);
	for (i=0; i<4; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<6; k++)
			{
				ar << BforD.dblForce[i][j][k];
			}
		}
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteBFORCS(CDgnIDEAArchive& ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_CSD& BforD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;

	int nSize = BforD.aBFOR.GetSize();
	ar << nSize;
	ar.endl();

	int i(0), j(0), k(0);
	for (int nCS = 0; nCS < nSize; nCS++)
	{
		for (i=0; i<4; i++)
		{
			for (j=0; j<2; j++)
			{
				for (k=0; k<6; k++)
				{
					ar << BforD.aBFOR[nCS].dblForce[i][j][k];
				}
			}
		}
		ar.endl();
	}
	
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteBFORPT(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_PT& BforDPT)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	int i(0), j(0), k(0);
	int nPart = BforDPT.aBFOR.GetSize();
	ar << nPart;

	ar.endl();
	for (int i_part = 0; i_part < nPart; i_part++)
	{
		for (i=0; i<4; i++)
		{
			for (j=0; j<2; j++)
			{
				for (k=0; k<6; k++)
				{
					ar << BforDPT.aBFOR[i_part].dblForce[i][j][k];          
				}
				ar.endl();
			}
		}
	}
	
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteBFORPTCC(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_PTCC& BforDPTCC)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;  
	int i(0), j(0), k(0), l(0);

	int nPart = BforDPTCC.aBFOR.GetSize();
	ar << nPart;
	ar.endl();

	for (int i_part = 0; i_part < nPart; i_part++)
	{
		for (i=0; i<4; i++)
		{
			for (j=0; j<2; j++)
			{
				for (k=0; k<12; k++)
				{
					for (l=0; l<6; l++)
					{
						ar << BforDPTCC.aBFOR[i_part].dblForce[i][j][k][l];            
					}
					ar.endl();
				}
			}
		}
	}
	
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteBFORPTCS(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_BFOR_PTCS& BforDPTCS)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	
	int nSize = BforDPTCS.aBFORPT.GetSize();
	ar << nSize;
	ar.endl();
	
	int i(0), j(0), k(0);
	for (int nCS = 0; nCS < nSize; nCS++)
	{
		int nPSize = BforDPTCS.aBFORPT[nCS].aBFOR.GetSize();
		ar << nPSize;
		for (int i_part = 0; i_part < nPSize; ++i_part)
		{
			for (i=0; i<4; i++)
			{
				for (j=0; j<2; j++)
				{
					for (k=0; k<6; k++)
					{
						ar << BforDPTCS.aBFORPT[nCS].aBFOR[i_part].dblForce[i][j][k];            
					}
					ar.endl();
				}
			}
		}
		ar.endl();
	}
	
	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteRECC(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_RECC_D& ReccD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	for (int i=0; i<12; i++) ar << ReccD.dblReac[i];

	ar.endl();
	return TRUE;
}
BOOL CDgnClassIREW::WriteDSPC(CDgnIDEAArchive&  ar, UINT nEKey, UINT nLKey, UINT nType, S_DSPC_D& DspcD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;
	for (int i=0; i<12; i++) ar << DspcD.dblDisp[i];

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::WriteBCFO(CDgnIDEAArchive& ar, UINT nEKey, UINT nLKey, UINT nType, S_BCFO_B& BcfoD)
{
	ar << nEKey;
	ar << nLKey;
	ar << nType;  
	int i(0), j(0), k(0), l(0);
	for (i=0; i<4; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<12; k++)
			{
				for (l=0; l<6; l++)
				{
					ar << BcfoD.dblForce[i][j][k][l];
				}        
				ar.endl();
			}
		}
	}

	ar.endl();
	return TRUE;
}

BOOL CDgnClassIREW::ExportIDEADataTest(CDgnIDEAArchive& ar, CString& szMCTPath, CDgnClassIRED& rData)
{
	TRY
	{
		int i(0), nSize(0);

		
		// Basicrmation
		ar.WriteText(_T("%s"), _LSX(_T("* BASIC Information *")));
		if (WriteBASC(ar, szMCTPath) == FALSE) return FALSE;
		ar.endl();
		
		// Unit
		S_UNIT_D  UnitD;
		rData.GetUnit(UnitD);
		ar.WriteText(_T("%s \n"), _LSX(* UINT Information *));
		if (WriteUNIT(ar, UnitD) == FALSE) return FALSE;
		ar.endl();

		// Node
		S_NODE_K_LIST aNodeK;
		S_NODE_D_LIST aNodeD;
		ar.WriteText(_T("%s \n"), _LSX(* NODE *));
		nSize = rData.GetNode(aNodeK, aNodeD);
		for (i=0; i<nSize; i++)
		{
			if (WriteNODE(ar, aNodeK[i], aNodeD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Element
		S_ELEM_K_LIST aElemK;
		S_ELEM_D_LIST aElemD;
		ar.WriteText(_T("%s \n"), _LSX(* ELEMENT *));
		nSize = rData.GetElem(aElemK, aElemD);
		for (i=0; i<nSize; i++)
		{
			if (WriteELEM(ar, aElemK[i], aElemD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Element
		S_MEMB_K_LIST aMembK;
		S_MEMB_D_LIST aMembD;
		ar.WriteText(_T("%s \n"), _LSX(* MEMBER *));
		nSize = rData.GetMemb(aMembK, aMembD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMEMB(ar, aMembK[i], aMembD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Material - Steel
		S_MATL_STEEL_K_LIST aMtstK;
		S_MATL_STEEL_D_LIST aMtstD;
		ar.WriteText(_T("%s \n"), _LSX(* Material-Steel *));
		nSize = rData.GetMatlST(aMtstK, aMtstD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMTST(ar, aMtstK[i], aMtstD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Material - Concrete
		S_MATL_CONCRETE_K_LIST aMtccK;
		S_MATL_CONCRETE_D_LIST aMtccD;
		ar.WriteText(_T("%s \n"), _LSX(* Material-Concrete *));
		nSize = rData.GetMatlCN(aMtccK, aMtccD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMTCC(ar, aMtccK[i], aMtccD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Material - Rebar
		S_MATL_REIF_K_LIST aMtrbK;
		S_MATL_REIF_D_LIST aMtrbD;
		ar.WriteText(_T("%s \n"), _LSX(* Material-Rebar *));
		nSize = rData.GetMatlRB(aMtrbK, aMtrbD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMTRB(ar, aMtrbK[i], aMtrbD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Material - Addrm
		S_MATL_ADIF_K_LIST aMtadK;
		S_MATL_ADIF_D_LIST aMtadD;
		ar.WriteText(_T("%s \n"), _LSX(* Material-Add *));
		nSize = rData.GetMatlAD(aMtadK, aMtadD);
		for (i=0; i<nSize; i++)
		{
			if (WriteMTAD(ar, aMtadK[i], aMtadD[i]) == FALSE) return FALSE;
		}
		ar.endl();
			
		// Section
		S_SECT_K_LIST aSectK;
		S_SECT_D_LIST aSectD;
		ar.WriteText(_T("%s \n"), _LSX(* Section *));
		nSize = rData.GetSect(aSectK, aSectD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSECT(ar, aSectK[i], aSectD[i]) == FALSE) return FALSE;
		}    
		ar.endl();

		// Section for Cross Section
		S_ELST_D_LIST aElstD;
		ar.WriteText(_T("%s \n"), _LSX(* Tapered Section for element *));
		nSize = rData.GetElst(aElemK, aSectK, aElstD);
		for (i=0; i<nSize; i++)
		{
			if (WriteELST(ar, aElemK[i], aSectK[i], aElstD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Section for Cross Section
		S_CSET_D_LIST aCsetD;
		S_PART_K_LIST aPartK;
		ar.WriteText(_T("%s \n"), _LSX(* Tapered Section for element use CSCS *));
		nSize = rData.GetCset(aElemK, aSectK, aPartK, aCsetD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCSET(ar, aElemK[i], aSectK[i], aPartK[i], aCsetD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// for 2D Element
		S_THIK_K_LIST aThikK;
		S_THIK_D_LIST aThikD;
		ar.WriteText(_T("%s \n"), _LSX(* Thickness *));
		nSize = rData.GetThik(aThikK, aThikD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTHIK(ar, aThikK[i], aThikD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Constraint - Support
		S_CONS_K_LIST aConsK;
		S_CONS_D_LIST aConsD;
		ar.WriteText(_T("%s \n"), _LSX(* Constraint - Support *));
		nSize = rData.GetCons(aConsK, aConsD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCONS(ar, aConsK[i], aConsD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Load Case
		S_STLD_K_LIST aStldK;
		S_STLD_D_LIST aStldD;
		ar.WriteText(_T("%s \n"), _LSX(* Load Case *));
		nSize = rData.GetStld(aStldK, aStldD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSTLD(ar, aStldK[i], aStldD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Load Combination
		S_LCOM_T_LIST aLcomT;
		S_LCOM_K_LIST aLcomK;
		S_LCOM_D_LIST aLcomD;
		ar.WriteText(_T("%s \n"), _LSX(* Load Combination *));
		nSize = rData.GetLcom(aLcomT, aLcomK, aLcomD);
		for (i=0; i<nSize; i++)
		{
			if (WriteLCOM(ar, aLcomT[i], aLcomK[i], aLcomD[i]) == FALSE) return FALSE;
		}     
		ar.endl();

		// Unbraced Length(L,Lb)
		S_LENG_K_LIST aLengK;
		S_LENG_D_LIST aLengD;
		ar.WriteText(_T("%s \n"), _LSX(* Unbraced Length(L,Lb) *));
		nSize = rData.GetLeng(aLengK, aLengD);
		for (i=0; i<nSize; i++)
		{
			if (WriteLENG(ar, aLengK[i], aLengD[i]) == FALSE) return FALSE;
		}
		ar.endl();
 
		// Effective Length Factor(K)
		S_KFAC_K_LIST aKfacK;
		S_KFAC_D_LIST aKfacD;
		ar.WriteText(_T("%s \n"), _LSX(* Effective Length Factor(K) *));
		nSize = rData.GetKfac(aKfacK, aKfacD);
		for (i=0; i<nSize; i++)
		{
			if (WriteKFAC(ar, aKfacK[i], aKfacD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Limiting Slenderness Ratio
		S_LTSR_K_LIST aLtsrK;
		S_LTSR_D_LIST aLtsrD;
		ar.WriteText(_T("%s \n"), _LSX(* Limiting Slenderness Ratio *));
		nSize = rData.GetLtsr(aLtsrK, aLtsrD);
		for (i=0; i<nSize; i++)
		{
			if (WriteLTSR(ar, aLtsrK[i], aLtsrD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Equivalent Moment Correction Factor(Cm)
		S_CMFT_K_LIST aCmftK;
		S_CMFT_D_LIST aCmftD;
		ar.WriteText(_T("%s \n"), _LSX(* Equivalent Moment Correction Factor(Cm) *));
		nSize = rData.GetCmft(aCmftK, aCmftD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCMFT(ar, aCmftK[i], aCmftD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Moment Magnifier
		S_FMAG_K_LIST aFmagK;
		S_FMAG_D_LIST aFmagD;
		ar.WriteText(_T("%s \n"), _LSX(* Moment Magnifier *));
		nSize = rData.GetFmag(aFmagK, aFmagD);
		for (i=0; i<nSize; i++)
		{
			if (WriteFMAG(ar, aFmagK[i], aFmagD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// RC Design Parameters
		S_DCON_D  DConD;
		ar.WriteText(_T("%s \n"), _LSX(* RC Design Parameters *));
		rData.GetDcon(DConD);
		if (WriteDCON(ar, DConD) == FALSE) return FALSE;
		ar.endl();

		//Beam Section Data for Design
		S_RDBM_K_LIST aRdbmK;
		S_RDBM_D_LIST aRdbmD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Section Data for Design *));
		nSize = rData.GetRdbm(aRdbmK, aRdbmD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRDBM(ar, aRdbmK[i], aRdbmD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Column Section Data for Design
		S_RDCN_K_LIST aRdcnK;
		S_RDCN_D_LIST aRdcnD;
		ar.WriteText(_T("%s \n"), _LSX(* Column Section Data for Design *));
		nSize = rData.GetRdcn(aRdcnK, aRdcnD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRDCN(ar, aRdcnK[i], aRdcnD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Beam Section Data for Checking
		S_RCBM_K_LIST aRcbmK;
		S_RCBM_D_LIST aRcbmD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Section Data for Checking *));
		nSize = rData.GetRcbm(aRcbmK, aRcbmD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRCBM(ar, aRcbmK[i], aRcbmD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Column Section Data for Checking
		S_RCCN_K_LIST aRccnK;
		S_RCCN_D_LIST aRccnD;
		ar.WriteText(_T("%s \n"), _LSX(* Column Section Data for Checking *));
		nSize = rData.GetRccn(aRccnK, aRccnD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRCCN(ar, aRccnK[i], aRccnD[i]) == FALSE) return FALSE;
		}
		ar.endl();
	
		// Rebar Input for General Section
		S_REBT_K_LIST aRebtK;
		S_REBT_D_LIST aRebtD;
		ar.WriteText(_T("%s \n"), _LSX(* Rebar Input for General Section *));
		nSize = rData.GetRebt(aRebtK, aRebtD);
		for (i=0; i<nSize; i++)
		{
			if (WriteREBT(ar, aRebtK[i], aRebtD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Rebar for PSC
		S_RPSC_K_LIST aRpscK;
		S_RPSC_D_LIST aRpscD;
		ar.WriteText(_T("%s \n"), _LSX(* Rebar for PSC *));
		nSize = rData.GetRpsc(aRpscK, aRpscD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRPSC(ar, aRpscK[i], aRpscD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// Tendon of Cross section
		S_TDIF_K_LIST aTdifK;
		S_TDIJ_K_LIST aTdIJ;
		S_TDIF_D_LIST aTdifD;
		ar.WriteText(_T("%s \n"), _LSX(* Tendon of Cross section *));
		nSize = rData.GetTdif(aTdifK, aTdIJ, aTdifD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTDIF(ar, aTdifK[i], aTdIJ[i], aTdifD[i]) == FALSE) return FALSE;
		}   
		ar.endl();
		
		// for Construction Stage
		ar.WriteText(_T("%s \n"), _LSX(* Construction Stage Load Case *));
		nSize = rData.GetCslc(aStldK, aStldD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCSLC(ar, aStldK[i], aStldD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		ar.WriteText(_T("%s \n"), _LSX(* Construction Stage Load Case - Auto Generate *));
		nSize = rData.GetClat(aStldK, aStldD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCLAT(ar, aStldK[i], aStldD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_STAG_K_LIST aStagK;
		S_STAG_D_LIST aStagD;
		ar.WriteText(_T("%s \n"), _LSX(* Construction Stage *));
		nSize = rData.GetStag(aStagK, aStagD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSTAG(ar, aStagK[i], aStagD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_CSCS_K_LIST aCscsK;
		S_CSCS_D_LIST aCscsD;
		ar.WriteText(_T("%s \n"), _LSX(* Composite Section for Construction Stage *));
		nSize = rData.GetCscs(aCscsK, aCscsD);
		for (i=0; i<nSize; i++)
		{
			if (WriteCSCS(ar, aCscsK[i], aCscsD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_GRUP_K_LIST aGrupK;
		S_GRUP_D_LIST aGrupD;
		ar.WriteText(_T("%s \n"), _LSX(* Structure Group *));
		nSize = rData.GetGrup(aGrupK, aGrupD);
		for (i=0; i<nSize; i++)
		{
			if (WriteGRUP(ar, aGrupK[i], aGrupD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_BNGR_K_LIST aBngrK; 
		S_BNGR_D_LIST aBngrD; 
		ar.WriteText(_T("%s \n"), _LSX(* Boundary Group *));
		nSize = rData.GetBngr(aBngrK, aBngrD);
		for (i=0; i<nSize; i++)
		{
			if (WriteBNGR(ar, aBngrK[i], aBngrD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_LDGR_K_LIST aLdgrK;
		S_LDGR_D_LIST aLdgrD;
		ar.WriteText(_T("%s \n"), _LSX(* Load Group *));
		nSize = rData.GetLdgr(aLdgrK, aLdgrD);
		for (i=0; i<nSize; i++)
		{
			if (WriteLDGR(ar, aLdgrK[i], aLdgrD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_TDPF_K_LIST aTdpfK;
		S_STEP_K_LIST aStepK;
		S_TDLS_D_LIST aTdlsD;
		ar.WriteText(_T("%s \n"), _LSX(* TENDON time-Dependent Loss Graph *));
		nSize = rData.GetTdls(aTdpfK, aStagK, aStepK, aTdlsD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTDLS(ar, aTdpfK[i], aStagK[i], aStepK[i], aTdlsD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_LDTP_K_LIST aType;
		/*aNodeK.RemoveAll();
		aStldK.RemoveAll();
		S_REAC_D_LIST aReacD;
		ar.WriteText(_T("%s \n"), _LSX(* Reaction *));
		nSize = rData.GetReac(aNodeK, aStldK, aType, aReacD);
		for (i=0; i<nSize; i++)
		{
			if (WriteREAC(ar, aNodeK[i], aStldK[i], aType[i], aReacD[i]) == FALSE) return FALSE;
		}
		ar.endl();*/

		S_DISP_D_LIST aDispD;
		ar.WriteText(_T("%s \n"), _LSX(* Displacement *));
		nSize = rData.GetDisp(aNodeK, aStldK, aType, aDispD);
		for (i=0; i<nSize; i++)
		{
			if (WriteDISP(ar, aNodeK[i], aStldK[i], aType[i], aDispD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_TFOR_D_LIST aTForD;
		ar.WriteText(_T("%s \n"), _LSX(* Truss force *));
		nSize = rData.GetTfor(aElemK, aStldK, aType, aTForD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTFOR(ar, aElemK[i], aStldK[i], aType[i], aTForD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_BFOR_D_LIST aBForD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam force *));
		nSize = rData.GetBfor(aElemK, aStldK, aType, aBForD);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFOR(ar, aElemK[i], aStldK[i], aType[i], aBForD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_STRP_DL_LIST aPForD;
		ar.WriteText(_T("%s \n"), _LSX(* Plate force *));
		nSize = rData.GetStrp(aElemK, aStldK, aType, aPForD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSTRP(ar, aElemK[i], aStldK[i], aType[i], aPForD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		/*aNodeK.RemoveAll();
		aStldK.RemoveAll();
		aType.RemoveAll();
		S_RECC_D_LIST aReccD;    
		ar.WriteText(_T("%s \n"), _LSX(* Concurrent Reaction *));
		nSize = rData.GetRecc(aNodeK, aStldK, aType, aReccD);
		for (i=0; i<nSize; i++)
		{
			if (WriteRECC(ar, aNodeK[i], aStldK[i], aType[i], aReccD[i]) == FALSE) return FALSE;
		}
		ar.endl();*/

		S_DSPC_D_LIST aDspcD;
		ar.WriteText(_T("%s \n"), _LSX(* Concurrent Displacement *));
		nSize = rData.GetDspc(aNodeK, aStldK, aType, aDspcD);
		for (i=0; i<nSize; i++)
		{
			if (WriteDSPC(ar, aNodeK[i], aStldK[i], aType[i], aDspcD[i]) == FALSE) throw  FALSE;
		}
		ar.endl();
		
		S_BCFO_B_LIST aBcfoD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Concurrent force *));
		nSize = rData.GetBcfo(aElemK, aStldK, aType, aBcfoD);
		for (i=0; i<nSize; i++)
		{
			if (WriteBCFO(ar, aElemK[i], aStldK[i], aType[i], aBcfoD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		// for Construction Stage
		S_STAG_K_LIST aStag;
		S_DISP_CSD_LIST aDispCSD;
		ar.WriteText(_T("%s \n"), _LSX(* Displacement - Construction Stage *));
		nSize = rData.GetDispCS(aNodeK, aStldK, aStag, aDispCSD);
		for (i=0; i<nSize; i++)
		{
			if (WriteDISPCS(ar, aNodeK[i], aStldK[i], aStag[i], aDispCSD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_TFOR_CSD_LIST aTForCSD;
		ar.WriteText(_T("%s \n"), _LSX(* Truss force - Construction Stage *));
		nSize = rData.GetTforCS(aElemK, aStldK, aType, aTForCSD);
		for (i=0; i<nSize; i++)
		{
			if (WriteTFORCS(ar, aElemK[i], aStldK[i], aStag[i], aTForCSD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_BFOR_CSD_LIST aBForCSD;
		ar.WriteText(_T("%s \n"), _LSX(* Beam force - Construction Stage *));
		nSize = rData.GetBforCS(aElemK, aStldK, aStag, aBForCSD);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFORCS(ar, aElemK[i], aStldK[i], aStag[i], aBForCSD[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_STRP_CSDL_LIST  aPForCSD;
		ar.WriteText(_T("%s \n"), _LSX(* Plate force - Construction Stage *));
		nSize = rData.GetStrpCS(aElemK, aStldK, aStag, aPForCSD);
		for (i=0; i<nSize; i++)
		{
			if (WriteSTRPCS(ar, aElemK[i], aStldK[i], aStag[i], aPForCSD[i]) == FALSE) return FALSE;
		}
		ar.endl();

		S_BFOR_PT_LIST  aBForPT;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Force for Part *));
		nSize = m_Data.GetBforPT(aElemK, aStldK, aType, aBForPT);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFORPT(ar, aElemK[i], aStldK[i], aType[i], aBForPT[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_BFOR_PTCC_LIST  aBForPTCC;
		ar.WriteText(_T("%s \n"), _LSX(* Beam concurrent force for Part *));
		nSize = m_Data.GetBforPTCC(aElemK, aStldK, aType, aBForPTCC);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFORPTCC(ar, aElemK[i], aStldK[i], aType[i], aBForPTCC[i]) == FALSE) return FALSE;
		}
		ar.endl();
		
		S_BFOR_PTCS_LIST  aBForPTCS;
		ar.WriteText(_T("%s \n"), _LSX(* Beam Stage Force for Part *));
		nSize = m_Data.GetBforPTCS(aElemK, aStldK, aStag, aBForPTCS);
		for (i=0; i<nSize; i++)
		{
			if (WriteBFORPTCS(ar, aElemK[i], aStldK[i], aStag[i], aBForPTCS[i]) == FALSE) return FALSE;
		}
		ar.endl();
	}
	CATCH (CArchiveException, e)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	END_CATCH
 
	return TRUE;
}
