// Db_DllBaseManager.cpp: implementation of the CDb_DllBaseManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Db_DllBaseManager.h"
#include <vector>
#include "AttrCtrl.h"
#include "SectUtil.h"
#include "SectDB.h"
#include "DBLib.h"

#include "UserDefType.h"
#include "BaseMeshTool.h"
#include "FileCtrl.h"
#include "DebugFile.h"
#include "MultiKeyIndex.h"
#include "QSort.h"
#include "GeomData.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\PolyMaker.h"
#include "..\dgnengine\src\GEN_DgnCalc_KR\DgnCalc_KR_Load_Struct.h"
#include "..\dgnengine\src\DgnBase\DgnBase_CodeStruct.h"
#include "UnitCtrl.h"
// for Mesh
#include "AutoMesh_PlanarArea.h"    // for CAutoMesh_PlanarArea
#include "BaseMeshTool.h"
#include "DataCtrl.h"
#include "..\MIT_lib\StructKeyForStdMap.h"
#include "..\MIT_lib\MMathLib.h"
#include <set>
#include <fstream>

/////////////////////////////////////////////////
#define GRIDSET_DEFAULT_DIV  100
#define GRIDSET_MARGIN       (0.001)

class CGridSet
{
public:
    CGridSet(const double ptMin[3], const  double ptMax[3], const long nDivision)
    {
        double dMinMax[6] =
        {
            ptMin[0], ptMin[1], ptMin[2],
            ptMax[0], ptMax[1], ptMax[2]
        };

        if ( fabs(ptMax[0]-ptMin[0]) < 1e-13 &&
            fabs(ptMax[1]-ptMin[1]) < 1e-13 &&
            fabs(ptMax[2]-ptMin[2]) < 1e-13 )
        {
            dMinMax[0] -= 1e-13; dMinMax[1] -= 1e-13; dMinMax[2] -= 1e-13;
            dMinMax[3] += 1e-13; dMinMax[4] += 1e-13; dMinMax[5] += 1e-13;
        }

        double dDistX = dMinMax[3] - dMinMax[0];
        double dDistY = dMinMax[4] - dMinMax[1];
        double dDistZ = dMinMax[5] - dMinMax[2];
        double dMaxDist = dDistX;
        if ( dDistY > dMaxDist ) dMaxDist = dDistY;
        if ( dDistZ > dMaxDist ) dMaxDist = dDistZ;

        long nDiv = nDivision;

        // nDivision 을 적절하게 정해야 한다.
        // Boundary 영역이 크면 많이 등분하자.
        // 너무 많이 하면 메모리가...
        // 좀 더 스마트하게 정할수 있으면.. 수정해야함.
        /*
        if (dMaxDist > 400000.) nDiv = 900;
        else if (dMaxDist > 200000.) nDiv = 800;
        else if (dMaxDist > 100000.) nDiv = 700;
        else if (dMaxDist > 50000.)  nDiv = 600;
        else if (dMaxDist > 30000.)  nDiv = 500;
        else if (dMaxDist > 10000.)  nDiv = 400;
        else if (dMaxDist > 5000.)  nDiv = 300;
        else if (dMaxDist > 3000.)  nDiv = 200;
        */

        if ( nDiv % 2 == 0 ) nDiv += 1; // Division은 내부에서는 항상 홀수로 한다.

        dMaxDist += dMaxDist*GRIDSET_MARGIN;  // 최소 경계보다 0.1% 여유를 둔다.
        m_dCellSize = dMaxDist / nDiv;

        dDistX += dMaxDist*GRIDSET_MARGIN;
        dDistY += dMaxDist*GRIDSET_MARGIN;
        dDistZ += dMaxDist*GRIDSET_MARGIN;

        m_nCellNum[0] = long(ceil(dDistX / m_dCellSize));
        m_nCellNum[1] = long(ceil(dDistY / m_dCellSize));
        m_nCellNum[2] = long(ceil(dDistZ / m_dCellSize));
        // 홀수개가 되도록 한다.
        if ( m_nCellNum[0] % 2 == 0 ) m_nCellNum[0] += 1;
        if ( m_nCellNum[1] % 2 == 0 ) m_nCellNum[1] += 1;
        if ( m_nCellNum[2] % 2 == 0 ) m_nCellNum[2] += 1;

        double dCentX = ( dMinMax[3] + dMinMax[0] )/2.0;
        double dCentY = ( dMinMax[4] + dMinMax[1] )/2.0;
        double dCentZ = ( dMinMax[5] + dMinMax[2] )/2.0;

        m_ptMin[0] = dCentX - ( m_dCellSize*( m_nCellNum[0]/2 ) + m_dCellSize/2.0 );
        m_ptMin[1] = dCentY - ( m_dCellSize*( m_nCellNum[1]/2 ) + m_dCellSize/2.0 );
        m_ptMin[2] = dCentZ - ( m_dCellSize*( m_nCellNum[2]/2 ) + m_dCellSize/2.0 );

        //m_ptMax[0] = m_ptMin[0] + m_dCellSize*m_nCellNum[0];
        //m_ptMax[1] = m_ptMin[1] + m_dCellSize*m_nCellNum[1];
        //m_ptMax[2] = m_ptMin[2] + m_dCellSize*m_nCellNum[2];

        m_nPoolSize = m_nCellNum[0]*m_nCellNum[1]*m_nCellNum[2];

        m_aPool = new std::list<std::pair<int, T_POINT3D>>[m_nPoolSize];
    }

    ~CGridSet()
    {
        RemoveAll();
        if ( m_aPool ) delete[] m_aPool;
    }

    void RemoveAll()
    {
    }

    BOOL Insert(const double pt[3], const int nKey)
    {
        long index = GetPoolAddr(pt);
        if ( index < 0 ) return FALSE;

        std::pair<int, T_POINT3D> Pair;
        Pair.first = nKey;
        Pair.second.x = pt[0];
        Pair.second.y = pt[1];
        Pair.second.z = pt[2];
        m_aPool[index].push_back(Pair);

        return TRUE;
    }

    BOOL Insert(const int nX, const int nY, const int nZ, const T_POINT3D& Pos, const int nKey)
    {
        long index = GetPoolAddr(nX, nY, nZ);
        if ( index < 0 ) return FALSE;

        std::pair<int, T_POINT3D> Pair;
        Pair.first = nKey;
        Pair.second = Pos;
        m_aPool[index].push_back(Pair);

        return TRUE;
    }

    BOOL Delete(const double pt[3], const int nKey)
    {
        unsigned long index = GetPoolAddr(pt);
        if ( index < 0 ) return FALSE;

        std::list<std::pair<int, T_POINT3D>>& List = m_aPool[index];
        for ( auto ItrLoop = List.begin(); ItrLoop != List.end(); ItrLoop++ )
        {
            if ( ItrLoop->first == nKey )
            {
                List.erase(ItrLoop);
                return TRUE;
            }
        }

        return FALSE;
    }

    void GetBoxIndexByTriangle(const float pt1[3], const float pt2[3], const float pt3[3], int nStartIndex[3], int nEndIndex[3]) const
    {
        for ( int i = 0; i < 3; i++ )
        {
            nStartIndex[i] = m_nCellNum[i]-1;
            nEndIndex[i] = 0;
        }
        for ( int i = 0; i < 3; i++ )
        {
            nStartIndex[i] = MIN(nStartIndex[i], int(( pt1[i] - m_ptMin[i] ) / m_dCellSize));
            nEndIndex[i]   = MAX(nEndIndex[i], int(( pt1[i] - m_ptMin[i] ) / m_dCellSize));
        }
        for ( int i = 0; i < 3; i++ )
        {
            nStartIndex[i] = MIN(nStartIndex[i], int(( pt2[i] - m_ptMin[i] ) / m_dCellSize));
            nEndIndex[i]   = MAX(nEndIndex[i], int(( pt2[i] - m_ptMin[i] ) / m_dCellSize));
        }
        for ( int i = 0; i < 3; i++ )
        {
            nStartIndex[i] = MIN(nStartIndex[i], int(( pt3[i] - m_ptMin[i] ) / m_dCellSize));
            nEndIndex[i]   = MAX(nEndIndex[i], int(( pt3[i] - m_ptMin[i] ) / m_dCellSize));
            if ( nStartIndex[i] < 0 ) nStartIndex[i] = 0;
            if ( nEndIndex[i] >= m_nCellNum[i] ) nEndIndex[i] = m_nCellNum[i]-1;
        }
    }

    std::pair<int, T_POINT3D>& FindNearest(const double pt[3], const double dTol) const
    {
        int nStartIndex[3], nEndIndex[3];
        CalcIntersectAddr(pt, dTol, nStartIndex, nEndIndex);

        double dMinDist = dTol*dTol;
        double dDist = 0.0;
        std::pair<int, T_POINT3D>* pNear = NULL;

        int nXY = m_nCellNum[0]*m_nCellNum[1]; // 곱하기연산 최소화
        for ( int z = nStartIndex[2]; z <= nEndIndex[2]; z++ )
        {
            int nZ = z*nXY;                    // 곱하기연산 최소화
            for ( int y = nStartIndex[1]; y <= nEndIndex[1]; y++ )
            {
                int nY = y*m_nCellNum[0];      // 곱하기연산 최소화
                for ( int x = nStartIndex[0]; x <= nEndIndex[0]; x++ )
                {
                    unsigned long addr = x + nY + nZ;
                    std::list<std::pair<int, T_POINT3D>>& List = m_aPool[addr];
                    for ( auto EachItem : List )
                    {
                        dDist = CMathFunc::mathLength(pt[0], pt[1], pt[2], EachItem.second.x, EachItem.second.y, EachItem.second.z);
                        if ( dDist < dMinDist )
                        {
                            pNear = &EachItem;
                            dMinDist = dDist;
                        }
                    }
                }
            }
        }
        return *pNear;
    }

    int FindWithinTol(const double pt[3], const double dTol, std::list<std::pair<int, T_POINT3D>>& lstRst) const
    {
        double dTol2 = dTol*dTol;

        lstRst.clear();

        int nStartIndex[3], nEndIndex[3];
        CalcIntersectAddr(pt, dTol, nStartIndex, nEndIndex);

        double dDist;
        int nXY = m_nCellNum[0]*m_nCellNum[1]; // 곱하기연산 최소화
        for ( int z = nStartIndex[2]; z <= nEndIndex[2]; z++ )
        {
            int nZ = z*nXY;                    // 곱하기연산 최소화
            for ( int y = nStartIndex[1]; y <= nEndIndex[1]; y++ )
            {
                int nY = y*m_nCellNum[0];      // 곱하기연산 최소화
                for ( int x = nStartIndex[0]; x <= nEndIndex[0]; x++ )
                {
                    unsigned long addr = x + nY + nZ;
                    std::list<std::pair<int, T_POINT3D>>& listPool = m_aPool[addr];
                    for ( auto EachItem : listPool )
                    {
                        T_POINT3D& EachPos = EachItem.second;
                        dDist = CMathFunc::mathLength(pt[0], pt[1], pt[2], EachPos.x, EachPos.y, EachPos.z);
                        if ( dDist <= dTol2 ) lstRst.push_back(EachItem);
                    }
                }
            }
        }
        return static_cast< int >( lstRst.size() );
    }

    BOOL ExistDuplicate(const double pt[3], const double dTol) const
    {
        double dTol2 = dTol*dTol;

        int nStartIndex[3], nEndIndex[3];
        CalcIntersectAddr(pt, dTol, nStartIndex, nEndIndex);

        double dDist;
        int nXY = m_nCellNum[0]*m_nCellNum[1]; // 곱하기연산 최소화
        for ( int z = nStartIndex[2]; z <= nEndIndex[2]; z++ )
        {
            int nZ = z*nXY;                    // 곱하기연산 최소화
            for ( int y = nStartIndex[1]; y <= nEndIndex[1]; y++ )
            {
                int nY = y*m_nCellNum[0];      // 곱하기연산 최소화
                for ( int x = nStartIndex[0]; x <= nEndIndex[0]; x++ )
                {
                    unsigned long addr = x + nY + nZ;
                    std::list<std::pair<int, T_POINT3D>>& listPool = m_aPool[addr];
                    for ( auto EachItem : listPool )
                    {
                        T_POINT3D& EachPos = EachItem.second;
                        dDist = CMathFunc::mathLength(pt[0], pt[1], pt[2], EachPos.x, EachPos.y, EachPos.z);
                        if ( dDist <= dTol2 ) return TRUE;
                    }
                }
            }
        }
        return FALSE;
    }

protected:
    unsigned long GetPoolAddr(const double pt[3]) const
    {
        unsigned long index = -1;

        double dDistX = pt[0] - m_ptMin[0];
        double dDistY = pt[1] - m_ptMin[1];
        double dDistZ = pt[2] - m_ptMin[2];
        long ix = long(dDistX / m_dCellSize);
        long iy = long(dDistY / m_dCellSize);
        long iz = long(dDistZ / m_dCellSize);

        index = ix + iy * m_nCellNum[0] + iz * m_nCellNum[0] * m_nCellNum[1];

        if ( index >= m_nPoolSize ) { ASSERT(0); return -1; }
        return index;
    }

    unsigned long GetPoolAddr(const int nX, const int nY, const int nZ) const
    {
        unsigned long index = -1;

        long ix = long(nX);
        long iy = long(nY);
        long iz = long(nZ);

        index = ix + iy * m_nCellNum[0] + iz * m_nCellNum[0] * m_nCellNum[1];

        if ( index >= m_nPoolSize ) { ASSERT(0); return -1; }
        return index;
    }

    void CalcIntersectAddr(const double pt[3], const double dTol, int nStartIndex[3], int nEndIndex[3]) const
    {
        for ( int i = 0; i < 3; i++ )
        {
            nStartIndex[i] = int(( ( pt[i] - dTol ) - m_ptMin[i] ) / m_dCellSize);
            nEndIndex[i]   = int(( ( pt[i] + dTol ) - m_ptMin[i] ) / m_dCellSize);
            if ( nStartIndex[i] < 0 ) nStartIndex[i] = 0;
            if ( nEndIndex[i] >= m_nCellNum[i] ) nEndIndex[i] = m_nCellNum[i]-1;
        }
    }

    void CalcIntersectAddr(const float pt[3], const double dTol, int nStartIndex[3], int nEndIndex[3]) const
    {
        for ( int i = 0; i < 3; i++ )
        {
            nStartIndex[i] = int(( ( pt[i] - dTol ) - m_ptMin[i] ) / m_dCellSize);
            nEndIndex[i]   = int(( ( pt[i] + dTol ) - m_ptMin[i] ) / m_dCellSize);
            if ( nStartIndex[i] < 0 ) nStartIndex[i] = 0;
            if ( nEndIndex[i] >= m_nCellNum[i] ) nEndIndex[i] = m_nCellNum[i]-1;
        }
    }

protected:
    std::list<std::pair<int, T_POINT3D>>* m_aPool;
    long		    m_nCellNum[3];
    unsigned long   m_nPoolSize;
    double          m_dCellSize;
    double          m_ptMin[3];
    //double        m_ptMax[3];
};

// #include "DebugFile.h" 로 이동
// class CDebugFile
// {
// public:
// 	CDebugFile(LPCTSTR lpszFileName);
// 	~CDebugFile();
// public:
// 	BOOL Write(LPCTSTR lpszFmt, ...);
// 	BOOL Open();
// 	BOOL Close();
// 	BOOL IsOpen() { return m_fp != 0; }
// 	BOOL Clear();
// protected:
// 	CString m_csFileName;
// 	FILE* m_fp;
// };

// CDebugFile::CDebugFile(LPCTSTR lpszFileName)
// {
// 	m_csFileName = lpszFileName;
// 	m_fp = 0;
// }
//
// CDebugFile::~CDebugFile()
// {
// 	if (m_fp != 0)
// 	{
// 		fclose(m_fp);
// 		m_fp = 0;
// 	}
// }
//
// BOOL CDebugFile::Open()
// {
// 	if (m_fp != 0) return TRUE;
// 	m_fp = fopen(m_csFileName, "a+");
// 	if (m_fp == 0) return FALSE;
// 	return TRUE;
// }
//
// BOOL CDebugFile::Close()
// {
// 	if (m_fp != 0)
// 	{
// 		fclose(m_fp);
// 		m_fp = 0;
// 	}
// 	return TRUE;
// }
//
// BOOL CDebugFile::Write(LPCTSTR lpszFmt, ...)
// {
// 	// Open된 경우는 계속 열어두고
// 	// Open되지 않은 경우는 Open하고 쓰고 Close한다.
// 	BOOL bIsOpen = IsOpen();
// 	if (!bIsOpen)
// 	{
// 		if (!Open()) return FALSE;
// 	}
//
// 	CString sData;
// 	va_list args;
// 	va_start(args, lpszFmt);
// 	sData.FormatV(lpszFmt, args);
// 	int nLen = sData.GetLength();
// 	if (nLen > 0) fprintf(m_fp, "%s", sData);
// 	va_end(args);
//
// 	if (!bIsOpen) Close();
// 	return TRUE;
// }
//
// // Log File 내용을 지움
// BOOL CDebugFile::Clear()
// {
// 	BOOL bIsOpen = IsOpen();
// 	if (bIsOpen) Close();
// 	FILE* fp = fopen(m_csFileName, "wt");
// 	if (fp == 0) return TRUE;
// 	fclose(fp);
// 	if (bIsOpen) Open();
// 	return TRUE;
// }

/////////////////////////////////////////////////
// SHIN DAE-SEOCK
namespace ipf_data
{
    struct Point
    {
        double a_uv[2];
        Point() {}
        Point(double u, double v)
        {
            a_uv[0] = u;
            a_uv[1] = v;
        }
    };
    struct Line
    {
        int p1, p2;  // 0부터의 index
    };
    ///////////////////////////////////////////////
    double distance_uv(const double* a_uv_1, const double* a_uv_2);
    double distance_uv(const double* a_uv_1, const double x, const double y);
    double triangle_area_uv(const double* a_xy_1, const double* a_xy_2, const double* a_xy_3);
    double signed_angle_uv(const double* a_vector_1, const double* a_vector_2);
    bool intersect_segment(const double* a_uv_1, const double* a_uv_2, const double* a_uv_3, const double* a_uv_4, double& d_xi_1, double& d_xi_2);
    bool is_in_line(const double* a_luv_1, const double* a_luv_2, const double* a_uv);
    int get_point_index(std::vector<Point>& v_point, const double* a_uv);
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CLineIntersector class

class CLineIntersector
{
public:
    CLineIntersector();
    virtual ~CLineIntersector();
    void Clear();

    void SetLines(const vector<T_LINE2D>& vLines);
    BOOL PerformSplit();
    BOOL IsSplit() const { return m_bSplited; }
    const vector<T_LINE2D>& GetResultLines() const { return m_vResLines; }
    void ChangeTol(double dTolBig, double dTolSmall);

private:
    int intersect_split_polygon(vector<ipf_data::Point>& v_point, vector< vector<int> >& v_v_polygon);
    void check_duplicate_segment(const vector<ipf_data::Point>& v_point, vector< vector<int> >& v_v_polygon);
    void split_polygon(vector<ipf_data::Point>& v_point, vector< vector<int> >& v_v_polygon, int n_polygon_index, double d_polygon_length, vector<double>& v_xi);

    void MakeSplitedLines();

private:
    // 재구성된 정보
    vector<ipf_data::Point> m_vPoint;
    vector<ipf_data::Line>  m_vLine;

    BOOL                    m_bSplited;
    vector<T_LINE2D>        m_vResLines;

    double m_dTolBig;
    double m_dTolSmall;
};

//const double ZERO    = 1.0e-6;
const double ZERO_7    = 1.0e-7;
const double ZERO_4    = 1.0e-4;
using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////
// SHIN DAE SEOCK
// Domain facet generator for plane loading
/////////////////////////////////////////////////////////////////////
double ipf_data::distance_uv(const double* a_uv_1, const double* a_uv_2)
{
    return sqrt(pow(a_uv_2[0]-a_uv_1[0], 2.0)+pow(a_uv_2[1]-a_uv_1[1], 2.0));
}
double ipf_data::distance_uv(const double* a_uv_1, const double x, const double y)
{
    return sqrt(pow(x-a_uv_1[0], 2.0)+pow(y-a_uv_1[1], 2.0));
}
/////////////////////////////////////////////////////////////////////
double ipf_data::triangle_area_uv(const double* a_xy_1, const double* a_xy_2, const double* a_xy_3)
{
    double d_distance_12 = distance_uv(a_xy_1, a_xy_2);
    double d_distance_23 = distance_uv(a_xy_2, a_xy_3);
    double d_distance_31 = distance_uv(a_xy_3, a_xy_1);
    double d_s           = ( d_distance_12 + d_distance_23 + d_distance_31 ) / 2.0;
    return sqrt(fabs(d_s * ( d_s - d_distance_12 ) * ( d_s - d_distance_23 ) * ( d_s - d_distance_31 )));
}
///////////////////////////////////////////////
double ipf_data::signed_angle_uv(const double* a_vector_1, const double* a_vector_2)
{
    const double PPII    = atan(1.0) * 4.0;
    double a_null_uv[2] = { 0.0, 0.0 };
    double a_vector_3[2];
    transform(a_vector_2, a_vector_2+2, a_vector_1, a_vector_3, minus<double>());
    double d_norm_1      = distance_uv(a_null_uv, a_vector_1);
    double d_norm_2      = distance_uv(a_null_uv, a_vector_2);
    double d_norm_3      = distance_uv(a_null_uv, a_vector_3);
    double d_numerator   = pow(d_norm_1, 2.0) + pow(d_norm_2, 2.0) - pow(d_norm_3, 2.0);
    double d_denominator = 2.0 * d_norm_1 * d_norm_2;
    /////////////////////////////////////////
    if ( d_denominator < ZERO_4 ) return 0.0;
    /////////////////////////////////////////
    double d_cosine = d_numerator / d_denominator;
    if ( d_cosine >(1.0 - ZERO_4) ) return   0.0;
    if ( d_cosine < ( -1.0 + ZERO_4 ) ) return 180.0;
    /////////////////////////////////////////
    double d_angle = fabs(acos(d_cosine) * 180.0 / PPII);
    if ( ( a_vector_1[0] * a_vector_2[1] - a_vector_2[0] * a_vector_1[1] ) < 0.0 ) return -d_angle;
    return d_angle;
}
///////////////////////////////////////////////
bool ipf_data::intersect_segment(const double* a_uv_1, const double* a_uv_2, const double* a_uv_3, const double* a_uv_4, double& d_xi_1, double& d_xi_2)
{
    // 제한범위 오차가 너무 느슨하여 오류 발생됨(MQC12983). 이 때문에 오차크기 수정함.
    // intersect 조건은 d_xi_1, d_xi_2 가 0이상, 1이하 되어야 함.
    double d_det  = ( a_uv_3[0] - a_uv_4[0] ) * ( a_uv_1[1] - a_uv_2[1] ) - ( a_uv_1[0] - a_uv_2[0] ) * ( a_uv_3[1] - a_uv_4[1] );
    if ( fabs(d_det) < ZERO_7 ) return false;
    d_xi_1 = ( ( a_uv_4[0] - a_uv_3[0] ) * ( a_uv_3[1] - a_uv_1[1] ) + ( a_uv_3[0] - a_uv_1[0] ) * ( a_uv_3[1] - a_uv_4[1] ) ) / d_det;
    d_xi_2 = ( ( a_uv_3[0] - a_uv_1[0] ) * ( a_uv_1[1] - a_uv_2[1] ) + ( a_uv_2[0] - a_uv_1[0] ) * ( a_uv_3[1] - a_uv_1[1] ) ) / d_det;
    if ( ( d_xi_1 > -ZERO_7 && d_xi_1 < 1.0 + ZERO_7 ) && ( d_xi_2 > -ZERO_7 && d_xi_2 < 1.0 + ZERO_7 ) ) return true;
    return false;
}
///////////////////////////////////////////////
bool ipf_data::is_in_line(const double* a_luv_1, const double* a_luv_2, const double* a_uv)
{
    double a_vec_1[2] = { a_luv_1[0] - a_uv[0], a_luv_1[1] - a_uv[1] };
    double a_vec_2[2] = { a_luv_2[0] - a_uv[0], a_luv_2[1] - a_uv[1] };
    double d_dot_product = a_vec_1[0] * a_vec_2[0] + a_vec_1[1] * a_vec_2[1];
    double d_norm_1   = sqrt(pow(a_vec_1[0], 2.0)+pow(a_vec_1[1], 2.0));
    double d_norm_2   = sqrt(pow(a_vec_2[0], 2.0)+pow(a_vec_2[1], 2.0));
    if ( fabs(d_norm_1) < ZERO_4 || fabs(d_norm_2) < ZERO_4 ) return false;
    double d_cosine  = d_dot_product / ( d_norm_1 * d_norm_2 );
    return ( d_cosine > -1-ZERO_4 && d_cosine < -1+ZERO_4 );
}
///////////////////////////////////////////////
int ipf_data::get_point_index(vector<Point>& v_point, const double* a_uv)
{
    CDBDoc    *pDoc  = CDBDoc::GetDocPoint(); ASSERT(pDoc);
    CUnitCtrl *pUnit = pDoc->m_pUnitCtrl;   ASSERT(pUnit);

    double dZERO_7 = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, ZERO_7);

    for ( int i=0; i<v_point.size(); ++i )
    {
        double dDist = distance_uv(v_point[i].a_uv, a_uv);

        if ( dDist < dZERO_7 ) return i;
    }
    v_point.push_back(Point(a_uv[0], a_uv[1]));
    return ( int ) v_point.size()-1;
}
/////////////////////////////////////////////////////////////////////

struct T_SIMPLE_MESH_NODE_D
{
    int nNo;
    double y, z;
    T_SIMPLE_MESH_NODE_D()
    {
        nNo = 0;
        y = z = 0.0;
    }
    void Set(int nNo, double y, double z)
    {
        this->nNo = nNo;
        this->y   = y;
        this->z   = z;
    }
    BOOL IsSame(const T_SIMPLE_MESH_NODE_D& ptComp, double dTol) const
    {
        if ( fabs(y-ptComp.y) > dTol )  return FALSE;
        if ( fabs(z-ptComp.z) > dTol )  return FALSE;

        return TRUE;
    }
};

struct T_SIMPLE_MESH_EDGE_D
{
    int nNo;
    int iNode1, iNode2;
    T_SIMPLE_MESH_EDGE_D()
    {
        nNo = 0;
        iNode1 = iNode2 = 0;
    }
    void Set(int nNo, int iNode1, int iNode2)
    {
        this->nNo    = nNo;
        this->iNode1 = iNode1;
        this->iNode2 = iNode2;
    }
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineIntersector::CLineIntersector()
{
    m_bSplited = FALSE;

    m_dTolBig = 1.0e-03;
    m_dTolSmall = 1.0e-6;
}

CLineIntersector::~CLineIntersector()
{
}

void CLineIntersector::Clear()
{
    m_vLine.clear();
    m_vPoint.clear();
    m_vResLines.clear();
}

void CLineIntersector::SetLines(const vector<T_LINE2D>& vLines)
{
    Clear();

    // 점 중복체크부분 비효율적이라도 개체수가 적으므로 개별 비교
    size_t nLine = vLines.size();
    size_t i, j;
    double dZERO_4 = M_InitValueCurUnit(ZERO_4, KN, M, D_UNITSYS_BASE_LENGTH);
    m_vLine.resize(nLine);
    ipf_data::Line  NewLine;
    for ( i = 0; i<nLine; i++ )
    {
        const T_LINE2D& CurLine = vLines[i];
        NewLine.p1 = NewLine.p2 = -1;

        // check p1
        for ( j=0; j<m_vPoint.size(); j++ )
        {
            if ( ipf_data::distance_uv(m_vPoint[j].a_uv, CurLine.P1.x, CurLine.P1.y) < dZERO_4 )
            {
                NewLine.p1 = ( int ) j;
                break;
            }
        }
        if ( NewLine.p1 == -1 )
        {
            NewLine.p1 = static_cast< int >( m_vPoint.size() );
            ipf_data::Point NewPt;
            NewPt.a_uv[0] = CurLine.P1.x;
            NewPt.a_uv[1] = CurLine.P1.y;
            m_vPoint.push_back(NewPt);
        }

        // check p2
        for ( j=0; j<m_vPoint.size(); j++ )
        {
            if ( ipf_data::distance_uv(m_vPoint[j].a_uv, CurLine.P2.x, CurLine.P2.y) < dZERO_4 )
            {
                NewLine.p2 = ( int ) j;
                break;
            }
        }
        if ( NewLine.p2 == -1 )
        {
            NewLine.p2 = static_cast< int >( m_vPoint.size() );
            ipf_data::Point NewPt;
            NewPt.a_uv[0] = CurLine.P2.x;
            NewPt.a_uv[1] = CurLine.P2.y;
            m_vPoint.push_back(NewPt);
        }

        m_vLine[i] = NewLine;
    }
}

BOOL CLineIntersector::PerformSplit()
{
    size_t nOriPt = m_vPoint.size();
    size_t nOriLn = m_vLine.size();
    size_t i, j;

    BOOL bNeedResult = FALSE;
    while ( 1 )
    {
        size_t nPt = m_vPoint.size();
        size_t nLine  = m_vLine.size();

        vector< vector<int> > vvLines;
        vvLines.push_back(vector<int>());
        vvLines.push_back(vector<int>());
        BOOL bSplited = FALSE;
        for ( i = 0; i<nLine-1; i++ )
        {
            int p1=m_vLine[i].p1;
            int p2=m_vLine[i].p2;
            vvLines[0].clear();
            vvLines[0].push_back(p1);
            vvLines[0].push_back(p2);

            int  jj = -1;
            for ( j=i+1; j<nLine; j++ )
            {
                int p3=m_vLine[j].p1;
                int p4=m_vLine[j].p2;
                vvLines[1].clear();
                vvLines[1].push_back(p3);
                vvLines[1].push_back(p4);

                if ( ( p1==p3 && p2==p4 ) || ( p1==p4 && p2==p3 ) )
                {
                    m_vLine.erase(m_vLine.begin()+j);
                    bSplited = TRUE;
                    break;
                }

                int nRet = intersect_split_polygon(m_vPoint, vvLines);
                if ( nRet == 0 )  return FALSE;
                else if ( nRet == 1 ) { jj = ( int ) j;  break; }
            }

            if ( jj > 0 )
            {
                m_vLine.erase(m_vLine.begin()+jj);
                m_vLine.erase(m_vLine.begin()+i);
                for ( j=0; j<vvLines.size(); j++ )
                {
                    for ( int k=0; k<vvLines[j].size()-1; k++ )
                    {
                        ipf_data::Line NewLine;
                        NewLine.p1 = vvLines[j][k];
                        NewLine.p2 = vvLines[j][k+1];
                        m_vLine.push_back(NewLine);
                    }
                }// j
                bSplited = TRUE;
                break;
            }
            if ( bSplited ) break;
        } // i

        if ( !bSplited )
            break;
        bNeedResult = TRUE;
    }

    if ( bNeedResult )
    {
        m_bSplited = TRUE;
        MakeSplitedLines();
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////
void CLineIntersector::MakeSplitedLines()
{
    size_t nLine = m_vLine.size();
    size_t i;
    m_vResLines.clear();
    m_vResLines.resize(nLine);

    for ( i = 0; i<nLine; i++ )
    {
        T_LINE2D NewLine;
        const ipf_data::Line& CurLine = m_vLine[i];
        NewLine.P1.x = m_vPoint[CurLine.p1].a_uv[0];
        NewLine.P1.y = m_vPoint[CurLine.p1].a_uv[1];
        NewLine.P2.x = m_vPoint[CurLine.p2].a_uv[0];
        NewLine.P2.y = m_vPoint[CurLine.p2].a_uv[1];

        m_vResLines[i] = NewLine;
    }
}

/////////////////////////////////////////////////////////////////////
// @return 0:Failed, -1: nothing splited, 1: splited
int CLineIntersector::intersect_split_polygon(vector<ipf_data::Point>& v_point, vector< vector<int> >& v_v_polygon)
{
    using namespace ipf_data;
    ///////////////////////////////////////////////
    vector<double> av_ip_xi[2];
    double         a_polygon_length[2] = { 0.0, 0.0 };
    ///////////////////////////////////////////////
    double ZERO_4 = m_dTolSmall;
    for ( int i=0; i<2; ++i )
    {
        const vector<int>& v_polygon_1 = i == 0 ? v_v_polygon[0] : v_v_polygon[1];
        for ( int j=0; j<v_polygon_1.size()-1; ++j )
        {
            const Point& point_1 = v_point[v_polygon_1[j]];
            const Point& point_2 = v_point[v_polygon_1[j+1]];
            a_polygon_length[i] += distance_uv(point_1.a_uv, point_2.a_uv);
        }
        double d_accumulate_length = 0.0;
        for ( int j=0; j<v_polygon_1.size()-1; ++j )
        {
            const Point& point_1          = v_point[v_polygon_1[j]];
            const Point& point_2          = v_point[v_polygon_1[j+1]];
            double       d_segment_length = distance_uv(point_1.a_uv, point_2.a_uv);
            d_accumulate_length          += d_segment_length;
            const vector<int>& v_polygon_2 = i == 0 ? v_v_polygon[1] : v_v_polygon[0];
            for ( int k=0; k<v_polygon_2.size()-1; ++k )
            {
                const Point& point_3 = v_point[v_polygon_2[k]];
                const Point& point_4 = v_point[v_polygon_2[k+1]];
                double       d_xi_1, d_xi_2;
                if ( !intersect_segment(point_1.a_uv, point_2.a_uv, point_3.a_uv, point_4.a_uv, d_xi_1, d_xi_2) )
                {
                    if ( is_in_line(point_1.a_uv, point_2.a_uv, point_3.a_uv) )
                    {
                        double d_length_13 = distance_uv(point_1.a_uv, point_3.a_uv);
                        double d_local_xi  = d_length_13 / d_segment_length;
                        double d_global_xi = ( ( d_accumulate_length - d_segment_length ) + d_segment_length * d_local_xi ) / a_polygon_length[i];
                        if ( fabs(d_global_xi) > ZERO_4 && fabs(1.0-d_global_xi) > ZERO_4 ) av_ip_xi[i].push_back(d_global_xi);
                    }
                    if ( is_in_line(point_1.a_uv, point_2.a_uv, point_4.a_uv) )
                    {
                        double d_length_14 = distance_uv(point_1.a_uv, point_4.a_uv);
                        double d_local_xi  = d_length_14 / d_segment_length;
                        double d_global_xi = ( ( d_accumulate_length - d_segment_length ) + d_segment_length * d_local_xi ) / a_polygon_length[i];
                        if ( fabs(d_global_xi) > ZERO_4 && fabs(1.0-d_global_xi) > ZERO_4 ) av_ip_xi[i].push_back(d_global_xi);
                    }
                    continue;
                }
                double d_global_xi = ( ( d_accumulate_length - d_segment_length ) + d_segment_length * d_xi_1 ) / a_polygon_length[i];
                if ( fabs(d_global_xi) > ZERO_4 && fabs(1.0-d_global_xi) > ZERO_4 )
                {
                    av_ip_xi[i].push_back(d_global_xi);
                }
            }
        }
    }
    BOOL bSplited = ( av_ip_xi[0].size() > 0 || av_ip_xi[1].size() > 0 );
    ///////////////////////////////////////////////
    for ( int i=0; i<2; ++i ) split_polygon(v_point, v_v_polygon, i, a_polygon_length[i], av_ip_xi[i]);
    ///////////////////////////////////////////////
    vector< vector<int> > v_v_temp;
    {
        for ( int i=0; i<v_v_polygon.size(); ++i )
        {
            const vector<int>& v_polygon = v_v_polygon[i];
            for ( int j=0; j<v_polygon.size()-1; ++j )
            {
                vector<int> v_seg;
                v_seg.push_back(v_polygon[j]);
                v_seg.push_back(v_polygon[j+1]);
                v_v_temp.push_back(v_seg);
            }
        }
    }
    v_v_polygon = v_v_temp;
    check_duplicate_segment(v_point, v_v_polygon);
    ///////////////////////////////////////////////

    if ( bSplited )  return 1;
    else           return -1;
} // end: intersect_split_polygon()
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
void CLineIntersector::check_duplicate_segment(const vector<ipf_data::Point>& v_point, vector< vector<int> >& v_v_polygon)
{
    using namespace ipf_data;
    ///////////////////////////////////////////////
    for ( int i=0; i<v_v_polygon.size(); ++i )
    {
        const vector<int>& v_polygon_1 = v_v_polygon[i];
        if ( v_polygon_1.empty() ) continue;
        int          n_index_1 = v_polygon_1.front();
        int          n_index_2 = v_polygon_1.back();
        for ( int j=0; j<v_v_polygon.size(); ++j )
        {
            if ( i == j ) continue;
            vector<int>& v_polygon_2 = v_v_polygon[j];
            if ( v_polygon_2.empty() ) continue;
            int          n_index_3 = v_polygon_2.front();
            int          n_index_4 = v_polygon_2.back();
            ///////////////////////////////////////////
            if ( ( n_index_1 == n_index_3 && n_index_2 == n_index_4 ) || ( n_index_1 == n_index_4 && n_index_2 == n_index_3 ) )
            {
                vector<int>    a_v_polygon[2] = { v_polygon_1, v_polygon_2 };
                double         a_a_uv[2][2];
                vector<double> v_TOL(2);
                for ( int k=0; k<2; ++k )
                {
                    const vector<int>& v_polygon        = a_v_polygon[k];
                    double             d_polygon_length = 0.0;
                    for ( int m=0; m<v_polygon.size()-1; ++m )
                    {
                        const Point& point_1 = v_point[v_polygon[m]];
                        const Point& point_2 = v_point[v_polygon[m+1]];
                        d_polygon_length    += distance_uv(point_1.a_uv, point_2.a_uv);
                    }
                    v_TOL[k]          = d_polygon_length * 0.01;
                    double d_residual = d_polygon_length * 0.5;
                    for ( int m=0; m<v_polygon.size()-1; ++m )
                    {
                        const Point& point_1  = v_point[v_polygon[m]];
                        const Point& point_2  = v_point[v_polygon[m+1]];
                        double       d_length = distance_uv(point_1.a_uv, point_2.a_uv);
                        if ( d_length > d_residual || fabs(d_length-d_residual) < v_TOL[k] )
                        {
                            for ( int n=0; n<2; ++n ) a_a_uv[k][n] = point_1.a_uv[n]  + d_residual / d_length * ( point_2.a_uv[n] - point_1.a_uv[n] );
                            break;
                        }
                        d_residual -= d_length;
                    }
                }
                /////////////////////////////////////////
                if ( distance_uv(a_a_uv[0], a_a_uv[1]) < *min_element(v_TOL.begin(), v_TOL.end()) )
                {
                    v_polygon_2.clear();
                }
            }
        }
    }
    /*
    for (i=v_v_polygon.size()-1; i>=0; i--)
    {
    if (v_v_polygon[i].empty())
    {
    v_v_polygon.erase(i);
    }
    }
    */

    for ( int i=0; i<v_v_polygon.size(); ++i )
    {
        if ( v_v_polygon[i].empty() )
        {
            v_v_polygon.erase(v_v_polygon.begin()+i);
            i--;
        }
    }
} // end: check_duplicate_segment()
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
void CLineIntersector::split_polygon(vector<ipf_data::Point>& v_point, vector< vector<int> >& v_v_polygon, int n_polygon_index, double d_polygon_length, vector<double>& v_xi)
{
    using namespace ipf_data;
    ///////////////////////////////////////////////
    if ( v_xi.empty() ) return;
    ///////////////////////////////////////////////
    double TOL = m_dTolBig;
    ///////////////////////////////////////////////
    sort(v_xi.begin(), v_xi.end());
    for ( vector<double>::iterator dvi=v_xi.begin(); dvi!=v_xi.end()-1; ++dvi )
    {
        if ( fabs(*dvi - *( dvi+1 )) < TOL )
        {
            if ( dvi != v_xi.begin() )  v_xi.erase(dvi--);
        }
    }
    vector<int>& v_polygon = v_v_polygon[n_polygon_index];
    ///////////////////////////////////////////////
    const int             n_N_xi = ( int ) v_xi.size();
    vector< vector<int> > v_v_new_polygon(n_N_xi+1);
    ///////////////////////////////////////////////
    int n_mark = -1;
    for ( int i=0; i<n_N_xi; i++ )
    {
        double d_residual = d_polygon_length * v_xi[i];
        for ( int j=0; j<v_polygon.size()-1; ++j )
        {
            int          n_index_1 = v_polygon[j];
            int          n_index_2 = v_polygon[j+1];
            const Point& point_1   = v_point[n_index_1];
            const Point& point_2   = v_point[n_index_2];
            if ( j > n_mark ) v_v_new_polygon[i].push_back(n_index_1);
            double       d_length  = distance_uv(point_1.a_uv, point_2.a_uv);
            if ( d_length >= d_residual )
            {
                double a_uv[2];
                for ( int k=0; k<2; ++k ) a_uv[k] = point_1.a_uv[k] + d_residual / d_length * ( point_2.a_uv[k] - point_1.a_uv[k] );
                int n_index = get_point_index(v_point, a_uv);
                if ( n_index != n_index_1 ) v_v_new_polygon[i].push_back(n_index);
                v_v_new_polygon[i+1].push_back(n_index);
                if ( n_index == n_index_2 ) n_mark = j + 1;
                else                      n_mark = j;
                break;
            }
            d_residual -= d_length;
        }
    }
    for ( int i=n_mark+1; i<v_polygon.size(); ++i ) v_v_new_polygon[n_N_xi].push_back(v_polygon[i]);
    ///////////////////////////////////////////////
    v_polygon = v_v_new_polygon[0];
    for ( int i=1; i<n_N_xi+1; i++ ) v_v_polygon.push_back(v_v_new_polygon[i]);
} // end: split_polygon()

void CLineIntersector::ChangeTol(double dTolBig, double dTolSmall)
{
    m_dTolBig = dTolBig;
    m_dTolSmall = dTolSmall;
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// SimpleMesh Generator class
// 용도 : 순수 Mesh 생성용 (7자유도 단면 강성 계산용 Lib에 Mesh를 넘기기 위한 용도)
class CSimpleMesh
{
    // Construction
public:
    CSimpleMesh();   // standard constructor

private:
    double  m_dMinMeshSize;

public:
    static BOOL IsSRC(const T_SECT_D& SectD);
    static BOOL IsComposite(const T_SECT_D& SectD);
    static BOOL IsSectWithStiffener(const T_SECT_D& SectD);
    static int  GetInnerDomainLevel(const T_SECT_D& SectD);
    static BOOL IsCompositeSlabNoGap(const T_SECT_D& SectD);
    static BOOL IsCompositeSlabGapMesh(const T_SECT_D& SectD);

private:
    void GetMinMaxAutoMeshSize(const T_SECT_D& SectD, CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge, double& dMinSize, double& dMaxSize, int nCompShapeType, int nTypeBeforeAfter, BOOL bSizeBySectD);
    BOOL GetMaxWebFlngThik(const T_SECT_D& SectD, double& dMaxThik);
    BOOL CheckGirderSlabWidth(const T_SECT_D& SectD, CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge);

public:
    BOOL m_bUseMaxSectMesh;
    BOOL m_bTubSectClosed;

public:
    BOOL GenerateMesh(const T_SECT_D& SectD,
        CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge,
        CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeAll,
        CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
		std::vector<int>& raDomainElemCount,
        int nCompShapeType, int nTypeBeforeAfter);

    double GetMeshSizeRatio(CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> &aEdge, int i, double dMaxSize);
    BOOL GetDomainMaterialType(CArray<int, int>& raDomainMaterialType,
        const T_SECT_D& SectD,
        const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
        const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
        const std::vector<int>& aDomainElemCount,
        const int nTypeBeforeAfter);

    BOOL RemoveDomainHole(const T_SECT_D& SectD,
        CArray<int, int>& raDomainMaterialType,
        CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
		std::vector<int>& raDomainElemCount);

    BOOL MergeDomainMesh(const T_SECT_D& SectD,
        CArray<int, int>& raDomainMaterialType,
        CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
		std::vector<int>& raDomainElemCount);
    BOOL MergeDomainMesh_AllSect(const T_SECT_D& SectD,
        CArray<int, int>& raDomainMaterialType,
        CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
		std::vector<int>& raDomainElemCount);

    BOOL GetDomainMaterialType_SRC(CArray<int, int>& raDomainMaterialType,
        const T_SECT_D& SectD,
        const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
        const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
        const std::vector<int>& aDomainElemCount);
    BOOL GetDomainMaterialType_Composite(CArray<int, int>& raDomainMaterialType,
        const T_SECT_D& SectD,
        const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
        const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
        const std::vector<int>& aDomainElemCount,
        const int nTypeBeforeAfter);
    BOOL GetDomainMaterialType_Composite_AllSect(CArray<int, int>& raDomainMaterialType,
        const T_SECT_D& SectD,
        const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
        const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
        const std::vector<int>& aDomainElemCount,
        const int nTypeBeforeAfter);
    BOOL GetDomainMaterialType_PSC_CMPWEB(CArray<int, int>& raDomainMaterialType,
        const T_SECT_D& SectD,
        const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
        const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
        const std::vector<int>& aDomainElemCount);
    BOOL GetDomainMaterialType_SectWithStiffener(CArray<int, int>& raDomainMaterialType,
        const T_SECT_D& SectD,
        const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
        const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
        const std::vector<int>& aDomainElemCount);
    BOOL GetSectMesh(T_SECT_D& SectD, int nPos, int nTypeBeforeAfter,
        CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
		std::vector<int>& raDomainElemCount,
        CArray<int, int>& raDomainMaterialType,
        BOOL bAdjustCoordToCentroid,
        T_SECT_DESIGN* pSectDesign);
    int GetStlgMCellHoleNum(const T_SECT_D& SectD);
    BOOL GetSectRegularMesh(int nPos, T_SECT_D &SectD, std::vector<int> &raDomainElemCount, CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> &raMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD, CArray<int, int> &raDomainMaterialType, int nTypeBeforeAfter, BOOL bMerge = TRUE);

    BOOL GetSectMergeMesh(T_SECT_D &SectD, int nPos, int nTypeBeforeAfter, CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> &raMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&> &raMeshNodeD, CArray<int, int> &raDomainMaterialType, std::vector<int> &raDomainElemCount);

    BOOL GetSectMeshByPolygon(T_SECT_D &SectD, int nTypeBeforeAfter,
        CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge,
        CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeAll,
        CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
		std::vector<int>& raDomainElemCount,
        CArray<int, int>& raDomainMaterialType,
        BOOL bMerge=TRUE);

    BOOL GetSectMeshByPolygon_AllSect(T_SECT_D &SectD, int nTypeBeforeAfter,
        CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge,
        CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeAll,
        CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
		std::vector<int>& raDomainElemCount,
        CArray<int, int>& raDomainMaterialType,
        BOOL bMerge=TRUE);

    BOOL GetSectPolygon(T_SECT_D &SectD, int nPos, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, int nTypeBeforeAfter=0);

    void GroupingPlanInfoByProp(CArray<int, int> &aPlaneProperty, CArray<int, int> &aPlaneVCount, CArray<int, int>& aPlaneVertexIndex, CArray<double, double>& aVertex, std::map<int, T_EachPlanInfo> &mapPlanePropertyToInfo);

    BOOL GetSectPolygonBySectShape(CArray<int, int>& aPlaneVCount, CArray<int, int>& aPlaneVertexIndex, CArray<double, double>& aVertex, T_SECT_D &SectD, BOOL bDoNewAlgorithm, BOOL bStiff, CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge);

    BOOL AdjustCoordToCentroid(int nTypeBeforeAfter, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD, T_SECT_DESIGN* pSectDesign=NULL);

    BOOL RenumberNodeByOffset(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
        int nCountNode_offset);

    static BOOL CalcCentroid(double& rdCentY, double& rdCentZ,
        const std::vector<int>& aDomainElemCount,
        const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
        const CArray<double, double>& aElaMod, std::vector<int>* pIncludeDomain = nullptr);

	static BOOL CalcPlasticModulus(double& rdZyy, double&rdZzz, const std::vector<int>& aDomainElemCount,
		const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
		CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
		const CArray<double, double>& aElaMod, std::vector<int>* pIncludeDomain = nullptr);

    static BOOL CalcCentroid(double& rdCentY, double& rdCentZ, double& rdArea,
        const T_MESH_ELEM_D& MeshElemD,
        CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD);
    BOOL GetLineInterSectPoint(T_POINT3D& P1, T_POINT3D& P2, T_POINT3D& P3, T_POINT3D& P4, T_POINT3D& IntersectPoint, double dTol);
    void MovePt(T_POINT3D& posOri, double aDir[3], double dMoveLen, T_POINT3D& posMoved);
    void GetDir(T_POINT3D& posA, T_POINT3D& posB, double dDir[3]);
    void AppendDivPnt(T_LINE_FOR_MESH_G& Edge, double dSize, BOOL bIncludeEachEnd, CArray<T_POINT3D, T_POINT3D&>& aPntDiv);
    BOOL MakeShareEdgeWhenComp(int nTypeBeforeAfter, T_SECT_D &SectD, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, BOOL bShareWidthSlab);
    BOOL MakeShareEdgeWhenCompNew(int nTypeBeforeAfter, T_SECT_D &SectD, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, BOOL bShareWidthSlab);
    BOOL MakeShareEdgeWhenComp_AllSect(int nTypeBeforeAfter, T_SECT_D &SectD, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, BOOL bShareWidthSlab);
    BOOL MakeShareEdgeWhenComp_GapCross(int nTypeBeforeAfter, T_SECT_D &SectD, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge,
        CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdgeAll, BOOL bShareWidthSlab, BOOL bGapnSlab=FALSE);
    void SetSimilarPntToSameNew(CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge);
    void GetMinMaxPos(double ptMin[3], double ptMax[3], CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D &> &aaEdge);

    BOOL IsLineOverlap(T_LINE_FOR_MESH_G& Line1, T_LINE_FOR_MESH_G& Line2);
    void SetSimilarPntToSame(CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge1, CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge2);
    int GetBoxGirderShapeNum(int nStype);
    BOOL IsBoxGirderShape(int nStype);
    int GetCompSectStifSize(T_SECT_D& SectD);

    BOOL IsClosedAfterComposite(T_SECT_D& SectD, int nPos);
    BOOL GetSlabPosition(T_SECT_D& SectD, int nPos, double* pointySl, double* pointzSl);
};

CSimpleMesh::CSimpleMesh()
{
    m_dMinMeshSize = 0.0;
    m_bUseMaxSectMesh = FALSE;
    m_bTubSectClosed = FALSE;
}

BOOL CSimpleMesh::GetMaxWebFlngThik(const T_SECT_D& SectD, double& dMaxThik)
{
    dMaxThik = 0.0;
    CString strShape = SectD.SectBefore.Shape;

    std::vector<double> aWebFlngThik;

    if ( strShape == D_SECT_SHAPE_COMPO_B )
    {
        //double Hw  = SectD.SectBefore.SectI.Size[0];
        double tw  = SectD.SectBefore.SectI.Size[1];
        //double B1  = SectD.SectBefore.SectI.Size[2];
        //double Bf1 = SectD.SectBefore.SectI.Size[3];
        double tf1 = SectD.SectBefore.SectI.Size[4];
        //double B2  = SectD.SectBefore.SectI.Size[5];
        //double Bf2 = SectD.SectBefore.SectI.Size[6];
        double tf2 = SectD.SectBefore.SectI.Size[7];
        aWebFlngThik.push_back(tw);
        aWebFlngThik.push_back(tf1);
        aWebFlngThik.push_back(tf2);
    }
    else if ( strShape == D_SECT_SHAPE_COMPO_I )
    {
        //double Hw  = SectD.SectBefore.SectI.Size[0];
        double tw  = SectD.SectBefore.SectI.Size[1];
        //double B1  = SectD.SectBefore.SectI.Size[2];
        double tf1 = SectD.SectBefore.SectI.Size[3];
        //double B2  = SectD.SectBefore.SectI.Size[4];
        double tf2 = SectD.SectBefore.SectI.Size[5];
        aWebFlngThik.push_back(tw);
        aWebFlngThik.push_back(tf1);
        aWebFlngThik.push_back(tf2);
    }
    else if ( strShape == D_SECT_SHAPE_COMPO_TUB )
    {
        //double Hw  = SectD.SectBefore.SectI.Size[0];
        double tw  = SectD.SectBefore.SectI.Size[1];
        //double B1  = SectD.SectBefore.SectI.Size[2];
        //double Bf1 = SectD.SectBefore.SectI.Size[3];
        double tf1 = SectD.SectBefore.SectI.Size[4];
        //double B2  = SectD.SectBefore.SectI.Size[5];
        //double Bf2 = SectD.SectBefore.SectI.Size[6];
        double tf2 = SectD.SectBefore.SectI.Size[7];
        //double Bf3 = SectD.SectBefore.SectI.Size[8];
        aWebFlngThik.push_back(tw);
        aWebFlngThik.push_back(tf1);
        aWebFlngThik.push_back(tf2);
    }
    else if ( strShape == D_SECT_SHAPE_COMPO_STLG_B )
    {
        //double B2  = SectD.SectBefore.SectI.Size[1];
        //double B3  = SectD.SectBefore.SectI.Size[2];
        //double B4  = SectD.SectBefore.SectI.Size[3];
        //double B5  = SectD.SectBefore.SectI.Size[4];
        //double B6  = SectD.SectBefore.SectI.Size[5];
        //double H   = SectD.SectBefore.SectI.Size[6];
        double t1  = SectD.SectBefore.SectI.Size[7];
        double t2  = SectD.SectBefore.SectI.Size[8];
        double tw1 = SectD.SectBefore.SectI.Size[9];
        double tw2 = SectD.SectBefore.SectI.Size[10];
        aWebFlngThik.push_back(t1);
        aWebFlngThik.push_back(t2);
        aWebFlngThik.push_back(tw1);
        aWebFlngThik.push_back(tw2);
    }
    else if ( strShape == D_SECT_SHAPE_COMPO_STLG_I )
    {
        //double B1 = SectD.SectBefore.SectI.Size[0];
        //double B2 = SectD.SectBefore.SectI.Size[1];
        //double B3 = SectD.SectBefore.SectI.Size[2];
        //double B4 = SectD.SectBefore.SectI.Size[3];
        //double H  = SectD.SectBefore.SectI.Size[4];
        double t1 = SectD.SectBefore.SectI.Size[5];
        double t2 = SectD.SectBefore.SectI.Size[6];
        double tw = SectD.SectBefore.SectI.Size[7];
        aWebFlngThik.push_back(t1);
        aWebFlngThik.push_back(t2);
        aWebFlngThik.push_back(tw);
    }
    else if ( strShape == D_SECT_SHAPE_COMPO_STLG_TUB )
    {
        //double B1  = rData.SectBefore.SectI.Size[0];
        //double B2  = rData.SectBefore.SectI.Size[1];
        //double B3  = rData.SectBefore.SectI.Size[2];
        //double B4  = rData.SectBefore.SectI.Size[3];
        //double B5  = rData.SectBefore.SectI.Size[4];
        //double B6  = rData.SectBefore.SectI.Size[5];
        //double H   = rData.SectBefore.SectI.Size[6];
        double t1  = SectD.SectBefore.SectI.Size[7];
        double t2  = SectD.SectBefore.SectI.Size[8];
        double tw1 = SectD.SectBefore.SectI.Size[9];
        double tw2 = SectD.SectBefore.SectI.Size[10];
        //double bf1 = rData.SectBefore.SectI.Size[11];
        //double bf2 = rData.SectBefore.SectI.Size[12];
        aWebFlngThik.push_back(t1);
        aWebFlngThik.push_back(t2);
        aWebFlngThik.push_back(tw1);
        aWebFlngThik.push_back(tw2);
    }

    if ( aWebFlngThik.size() == 0 )
        return FALSE;

    dMaxThik = DBL_MIN;
    for ( int i = 0; i < aWebFlngThik.size(); i++ )
    {
        if ( aWebFlngThik[i] > ZERO_4 )
        {
            dMaxThik = max(dMaxThik, aWebFlngThik[i]);
        }
    }
    if ( dMaxThik == DBL_MIN )
    {
        return FALSE;
    }
    // Min으로 넣어보는 코드
    //dMaxThik = DBL_MAX;
    //for (int i = 0; i < aWebFlngThik.size(); i++)
    //{
    //  if(aWebFlngThik[i] > ZERO_4)
    //  {
    //    dMaxThik = min(dMaxThik, aWebFlngThik[i]);
    //  }
    //}
    //if(dMaxThik == DBL_MAX)
    //{
    //  return FALSE;
    //}
    return TRUE;
}

void CSimpleMesh::GetMinMaxAutoMeshSize(const T_SECT_D& SectD, CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge, double& dMinSize, double& dMaxSize, int nCompShapeType, int nTypeBeforeAfter, BOOL bSizeBySectD)
{
    dMinSize = M_InitValueCurUnit(0.02, N, M, D_UNITSYS_BASE_LENGTH);

    // nStype == D_SECT_TYPE_STLG_MCELL 은 nCompShapeType 은 항상 1
    // 그외는  nCompShapeType = 0:Slab, 1:Girder, 2:Stiffner

    double dX_max = -DBL_MAX;
    double dX_min =  DBL_MAX;
    double dY_max = -DBL_MAX;
    double dY_min =  DBL_MAX;
    double dLenMin = DBL_MAX;
    double dLength = 0.0;
    for ( int i=0; i<aEdge.GetSize(); ++i )
    {
        if ( aEdge[i].p1.x > dX_max )  dX_max = aEdge[i].p1.x;
        if ( aEdge[i].p1.x < dX_min )  dX_min = aEdge[i].p1.x;
        if ( aEdge[i].p2.y > dY_max )  dY_max = aEdge[i].p2.y;
        if ( aEdge[i].p2.y < dY_min )  dY_min = aEdge[i].p2.y;

        dLength = aEdge[i].GetLineLength();
        if ( dLength < dLenMin )  dLenMin = dLength;
    }

    double dWidth, dHeight;
    if ( bSizeBySectD )
    {
        CDBDoc *pDoc  = CDBDoc::GetDocPoint(); ASSERT(pDoc);
        double hi, hj, bi, bj;
        pDoc->m_pAttrCtrl->GetSectDimen(static_cast< T_SECT_D >( SectD ), hi, bi, hj, bj);

        if ( CSectDB::IsSectSteelComposite(SectD) )
        {
            hi -= SectD.SectAfter.SectJ.Size[2]; // gap은 제외
        }

        dWidth = bi;
        dHeight = hi;
    }
    else
    {
        dWidth = dX_max - dX_min;
        dHeight= dY_max - dY_min;
    }

    double dLenMin_apply = dLenMin * 1.5;   // 1.5 기냥... (mesher 내부에서 min mesh 크기 적용시 dMinSize보다 어느 정도 작은 값을 사용하는거 같아서, 좀 키워줬다...)

    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFLoopForMeshSizeFind"), _T("yes")) )
    {
        dMaxSize = max(dWidth, dHeight) / CDb_DllBaseManager::m_dMeshSizeRatio; // hslee : 최대 Size를 정해주는 이유는 Warping계산 시 너무 크게 쪼개면 값이 잘못되기 때문
    }
    else
    {
        dMaxSize = max(dWidth, dHeight) / 20; // hslee(이훈석) : 최대 Size.. Before asy값 수렴 그래프를 통해 성실애과장님과 결정한 Size임. 변경해야 하는 상황이 있으면 연락 바람.
    }

    if ( nCompShapeType == 2 )
    {
        if ( bSizeBySectD )
        {
            dMaxSize *= 0.5;// 크기를 단면 전체 기준으로 할 경우 Stiffner를 더 잘게 쪼갬
        }
        else
        {
            dMaxSize *= 6.0; // 크기를 각 Stiffener 기준으로 할 경우엔 Stiffener를 더 크게
        }
    }
    else if ( nCompShapeType == 0 )
    {
        if ( CDb_DllBaseManager::m_bAllSectMesh )
        {
            // 절점 공유 되는 상황임.
        }
        else
        {
            dMaxSize *= 2.0; // Girder와 Slab의 절점공유가 빠지면서 Slab를 잘게 쪼갤 이유가 없어짐.. 2배 더 듬성하게 쪼개자
        }
    }

    dMaxSize = dMaxSize * 0.75; // 원레 근접한 것까지 고려한 Size로 했었는데.. 알고보니 Edge index가 근접한것 끼리 붙어있지 않더라.. 그래서.. 근접한거 구하려면 느려 질 거 같아서 그냥 전체 size를 요걸로..

    if ( nCompShapeType == 1 )
    {
        // Girder는 Web, Flange 두께보다 작게 쪼개도록 함 ( 이 알고리즘은 동작시키지 않음.. Mesh 모양이 좋아지긴 한데 속도가 느려져서 보류 )
        //double dMaxWebFlngThik;
        //if(GetMaxWebFlngThik(SectD, dMaxWebFlngThik))
        //{
        //  dMaxWebFlngThik *= 1.5; // CM2Mesh에서 좀더 작게 만들기 때문에 약간 키움
        //  dMaxSize = min(dMaxSize, dMaxWebFlngThik);
        //}
    }

    if ( nCompShapeType != 0 )
    {
        // 정해를 찾기위해 잠시 넣은 코드임
        //dMaxSize /= 4;

        // Web Thik를 Stiff에도 적용하기 위한 임시 코드
        //dMaxSize = min(dMaxSize, 24.0);
    }

    int nStype;
    nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;
    if ( IsBoxGirderShape(nStype) )
    {
        dMaxSize *= 0.7; // Box 에 구멍빼는 알고리즘이 없어지면서 속도가 빨라졌기때문에.. 강성값의 정확도를 좀더 높임
    }

    /*
    //sjhuh - MQC8556 관련 속도 개선을 처리하기 위한 메쉬 사이즈 축소.
    if(dWidth != 0 && dHeight != 0)
    {
    int nMultiple = 0;
    if(dWidth > dHeight)				nMultiple = dWidth / dHeight;
    else if(dWidth < dHeight)		nMultiple = dHeight / dWidth;
    else												nMultiple = 1;

    if(nMultiple >= 5)	dMinSize = __min(dWidth, dHeight)/1.0;
    else								dMinSize = __min(dWidth, dHeight)/5.0;
    }

    if (CTestEnvMgr::IsTestEnvST(_T("MaxSectMesh"), _T("yes"))) return;    // 테스트시 속도 때문에 일단 메쉬를 크게.
    if (m_bUseMaxSectMesh) return;
    if (dLenMin_apply < dMinSize) dMinSize = __max(dMinSize/10.0, dLenMin_apply);
    */
}
BOOL CSimpleMesh::CheckGirderSlabWidth(const T_SECT_D& SectD, CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge)
{
    double dX_max = -DBL_MAX;
    double dX_min =  DBL_MAX;
    double dY_max = -DBL_MAX;
    double dY_min =  DBL_MAX;
    double dLenMin = DBL_MAX;
    double dLength = 0.0;
    for ( int i=0; i<aEdge.GetSize(); ++i )
    {
        if ( aEdge[i].p1.x > dX_max )  dX_max = aEdge[i].p1.x;
        if ( aEdge[i].p1.x < dX_min )  dX_min = aEdge[i].p1.x;
        if ( aEdge[i].p2.y > dY_max )  dY_max = aEdge[i].p2.y;
        if ( aEdge[i].p2.y < dY_min )  dY_min = aEdge[i].p2.y;

        dLength = aEdge[i].GetLineLength();
        if ( dLength < dLenMin )  dLenMin = dLength;
    }

    double dWidthGr = dX_max - dX_min;
    double dHeightGr= dY_max - dY_min;

    // 	CDBDoc *pDoc  = CDBDoc::GetDocPoint(); ASSERT(pDoc);
    // 	double hi, hj, bi, bj;
    // 	pDoc->m_pAttrCtrl->GetSectDimen(static_cast<T_SECT_D>(SectD), hi, bi, hj, bj);

    double dWidthSl = SectD.SectAfter.SectJ.Size[0]; //bi;
    //double dHeightSl = hi;

    BOOL bSlab = ( dWidthSl >= dWidthGr ) ? TRUE : FALSE;
    return bSlab;
}

BOOL CSimpleMesh::IsBoxGirderShape(int nStype)
{
    if (m_bTubSectClosed)
        return TRUE;

    return nStype == D_SECT_TYPE_COMPO_B || nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_STLG_B;
}

int CSimpleMesh::GetCompSectStifSize(T_SECT_D& SectD)
{
    int nStype;
    nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    int nStifType; // 0 : OldStif, 1 : SODStif
    switch ( nStype )
    {
    case D_SECT_TYPE_COMPO_B:
    case D_SECT_TYPE_COMPO_I:
    case D_SECT_TYPE_COMPO_TUB:
        nStifType = 0;
        break;
    case D_SECT_TYPE_STLG_B:
    case D_SECT_TYPE_STLG_I:
    case D_SECT_TYPE_COMPO_STLG_B:
    case D_SECT_TYPE_COMPO_STLG_I:
    case D_SECT_TYPE_COMPO_STLG_TUB:
    case D_SECT_TYPE_STLG_MCELL:
        nStifType = 1;
        break;
    default:
        return 0;
        break;
    }

    int nStiff = 0;

    if ( nStifType == 0 )
    {
        CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>* pStiffProfile[4];
        pStiffProfile[0] = &SectD.SectBefore.SectI.Stiffener.aStiffLeft;
        pStiffProfile[1] = &SectD.SectBefore.SectI.Stiffener.aStiffRight;
        pStiffProfile[2] = &SectD.SectBefore.SectI.Stiffener.aStiffBottom;
        pStiffProfile[3] = &SectD.SectBefore.SectI.Stiffener.aStiffTop;

        for ( int i = 0; i < 4; i++ )
        {
            for ( int j = 0; j < pStiffProfile[i]->GetSize(); j++ )
            {
                if ( pStiffProfile[i]->GetAt(j).bCalc )
                {
                    nStiff++;
                }
            }
        }
    }
    else if ( nStifType == 1 )
    {
        for ( int i = 0; i < SectD.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize(); i++ )
        {
            for ( int j = 0; j < SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData.GetSize(); j++ )
            {
                if ( SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData[j].bCalc )
                {
                    nStiff++;
                }
            }
        }
    }

    return nStiff;
}

int CSimpleMesh::GetBoxGirderShapeNum(int nStype)
{
	if (m_bTubSectClosed)
		return 6;

    ASSERT(nStype == D_SECT_TYPE_COMPO_B || nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_STLG_B);
    return 4;
}

// calc centroid
BOOL CSimpleMesh::CalcCentroid(double& rdCentY, double& rdCentZ, double& rdArea,
    const T_MESH_ELEM_D& MeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD)
{
    ASSERT(MeshElemD.aNodeK.GetSize() == 3);

    T_MESH_NODE_D& NodeD0 = aMeshNodeD[MeshElemD.aNodeK[0]];
    T_MESH_NODE_D& NodeD1 = aMeshNodeD[MeshElemD.aNodeK[1]];
    T_MESH_NODE_D& NodeD2 = aMeshNodeD[MeshElemD.aNodeK[2]];

    double dxx[3] = { NodeD0.x, NodeD1.x, NodeD2.x };
    double dyy[3] = { NodeD0.y, NodeD1.y, NodeD2.y };

    double dCentX = 0.0;
    double dCentY = 0.0;
    double dArea  = 0.0;
    if ( !CMathFunc::mathPolyCentroid(3, dxx, dyy, dCentX, dCentY, dArea) ) { ASSERT(0); return FALSE; }

    rdCentY = dCentX;
    rdCentZ = dCentY;
    rdArea  = dArea;

    return TRUE;
}

// calc centroid
BOOL CSimpleMesh::CalcCentroid(double& rdCentY, double& rdCentZ,
    const std::vector<int>& aDomainElemCount,
    const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
    const CArray<double, double>& aElaMod, std::vector<int>* pIncludeDomain/* = nullptr*/)
{
    rdCentY = 0.0;
    rdCentZ = 0.0;

    double dCentY = 0.0;
    double dCentZ = 0.0;
    double dArea = 0.0;
    double dFacteredArea = 0.0;
    double dFact = 0.0;
    double dAreaSum = 0.0;
    double dCentYSum = 0.0;
    double dCentZSum = 0.0;

    int iElem = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )
    {
        dFact = aElaMod[i];   // 탄성비

        if ( dFact < 1.0e-07 ) dFact = 0.0; // Gap은 중립축 계산에 포함안시킨다. Gap =1.0e-08

		if ( pIncludeDomain )
		{
			if ( (std::find(pIncludeDomain->begin(), pIncludeDomain->end(), aDomainElemCount[i]) != pIncludeDomain->end()) == false )
			{
				iElem += (aDomainElemCount[i]-1);
				continue;
			}
		}

        for ( int k=0; k<aDomainElemCount[i]; ++k )
        {
            if ( !CalcCentroid(dCentY, dCentZ, dArea, aMeshElemD[iElem], aMeshNodeD) ) { ASSERT(0); continue; }

            dFacteredArea = dArea*dFact;
            dAreaSum += dFacteredArea;

            dCentYSum += dFacteredArea*dCentY;
            dCentZSum += dFacteredArea*dCentZ;

            iElem++;
        }
    }
    if ( dAreaSum == 0.0 )
    {
        rdCentY = 0.0;
        rdCentZ = 0.0;
    }
    else
    {
        rdCentY = dCentYSum/dAreaSum;
        rdCentZ = dCentZSum/dAreaSum;
    }

    return TRUE;
}


BOOL CSimpleMesh::CalcPlasticModulus(double& rdZyy, double&rdZzz, const std::vector<int>& aDomainElemCount,
	const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
	CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
	const CArray<double, double>& aElaMod, std::vector<int>* pIncludeDomain /*= nullptr*/)
{
	rdZyy = 0.0;
	rdZzz = 0.0;

	double dCenY = 0.0;
	double dCenZ = 0.0;
	if ( !CalcCentroid(dCenY, dCenZ, aDomainElemCount, aMeshElemD, aMeshNodeD, aElaMod, pIncludeDomain) )
	{
		ASSERT(0); return FALSE;
	}

	double dCentY = 0.0;
	double dCentZ = 0.0;
	double dArea = 0.0;
	double dFacteredArea = 0.0;
	
	int iElem = 0;
	for ( int i=0; i<aDomainElemCount.size(); ++i )
	{
		if ( pIncludeDomain )
		{
			if ( (std::find(pIncludeDomain->begin(), pIncludeDomain->end(), aDomainElemCount[i]) != pIncludeDomain->end()) == false )
			{
				iElem += (aDomainElemCount[i]-1);
				continue;
			}
		}

		for ( int k=0; k<aDomainElemCount[i]; ++k )
		{
			if ( !CalcCentroid(dCentY, dCentZ, dArea, aMeshElemD[iElem], aMeshNodeD) ) { ASSERT(0); continue; }

			dFacteredArea = dArea;
			rdZzz += dFacteredArea*fabs(dCenY - dCentY);
			rdZyy += dFacteredArea*fabs(dCenZ - dCentZ);

			iElem++;
		}
	}
	return TRUE;
}

void WriteMgtOfSectPoint(int nNode, double* p_ynode, double* p_znode, LPCTSTR lpszPostFix)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    // 데이터 확인용 txt 생성
    //     *NODE    ; Nodes
    //     ; iNO, X, Y, Z
    //     1, 0.5, 3.5, 0
    //
    //     *ELEMENT    ; Elements
    //     ; iEL, TYPE, iMAT, iPRO, iN1, iN2, ANGLE, iSUB, EXVAL, iOPT(EXVAL2) ; Frame  Element
    //     ; iEL, TYPE, iMAT, iPRO, iN1, iN2, iN3, iN4, iSUB, iWID             ; Planar Element
    //     1, BEAM  ,    1,     1,     1,     2,     0
    //     1, PLATE ,    1,     1,     1,     2,     3,     0,     1

    CString strModelPath=_T("");
    strModelPath = pDoc->GetPathName();
    CString strMctPath = _T("");

    if ( !strModelPath.IsEmpty() )
    {
        CFileCtrl FileCtrl(strModelPath);
        CString csFolder=_T("");
        csFolder = FileCtrl.GetFilePathAndNameWithoutExtension();
        strMctPath = csFolder + _T("_SectPoint") + lpszPostFix + _T(".mgt");
    }
    else { ASSERT(0); return; }

    CDebugFile log(strMctPath);
    log.Clear();

    // *NODE ------------------------
    log.Write(_T("*NODE\n"));
    for ( int i=0; i<nNode; ++i )
    {
        log.Write(_T("%d, %g, %g, 0.0 \n"), i+1, p_ynode[i]*100, p_znode[i]*100);
    }
    log.Write(_T("\n"));
    log.Write(_T("\n"));
}

void WriteNodeOfMesh(CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    T_NODE_K NodeK1;
    T_NODE_K NodeK2;
    T_NODE_D NodeD;

    CArray<T_NODE_K, T_NODE_K> aNodeK;
    CArray<T_NODE_D, T_NODE_D&> aNodeD;
    T_NODE_K NodeKFirst = pDoc->m_pAttrCtrl->GetLastNumNode()+1;

    double dAddPos = 2000;

    T_ELEM_K ElemK;
    T_ELEM_D ElemD;

    CArray<T_ELEM_K, T_ELEM_K>  aElemK;
    CArray<T_ELEM_D, T_ELEM_D&> aElemD;
    T_ELEM_K ElemKFirst = pDoc->m_pAttrCtrl->GetLastNumElem()+1;

    for ( int i = 0; i < aEdge.GetSize(); i++ )
    {
        if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFLoopForMeshSizeFind"), _T("yes")) ) // Mesh크기를 조절해 가면서 수렴값을 찾기 위한 레지스트리
        {
            dAddPos = 200 + 50 * CDb_DllBaseManager::m_dMeshSizeRatio;
        }

        NodeK1 = NodeKFirst;
        NodeKFirst++;
        NodeD.Initialize();
        NodeD.x = aEdge[i].p1.x + dAddPos; // 이미 만들어져 있는 모델이랑 겹치지 않게 더함
        NodeD.y = aEdge[i].p1.y + dAddPos;
        NodeD.z = dAddPos;

        aNodeK.Add(NodeK1);
        aNodeD.Add(NodeD);

        NodeK2 = NodeKFirst;
        NodeKFirst++;
        NodeD.Initialize();
        NodeD.x = aEdge[i].p2.x + dAddPos; // 이미 만들어져 있는 모델이랑 겹치지 않게 더함
        NodeD.y = aEdge[i].p2.y + dAddPos;
        NodeD.z = dAddPos;

        aNodeK.Add(NodeK2);
        aNodeD.Add(NodeD);

        ElemK = ElemKFirst;
        ElemKFirst++;
        ElemD.Initialize();
        ElemD.eltyp = BEAM_EL;
        ElemD.elmat = 1;
        ElemD.elpro = 1;
        ElemD.elnod[0] = NodeK1;
        ElemD.elnod[1] = NodeK2;

        aElemK.Add(ElemK);
        aElemD.Add(ElemD);
    }

    pDoc->m_pDataCtrl->AddNode(aNodeK, aNodeD);
    pDoc->m_pDataCtrl->AddElem(aElemK, aElemD);
}

void WriteNodeOfMesh(CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    T_NODE_K NodeK1;
    T_NODE_K NodeK2;
    T_NODE_D NodeD;

    CArray<T_NODE_K, T_NODE_K> aNodeK;
    CArray<T_NODE_D, T_NODE_D&> aNodeD;
    T_NODE_K NodeKFirst = pDoc->m_pAttrCtrl->GetLastNumNode()+1;

    double dAddPos = 2000;

    T_ELEM_K ElemK;
    T_ELEM_D ElemD;

    CArray<T_ELEM_K, T_ELEM_K>  aElemK;
    CArray<T_ELEM_D, T_ELEM_D&> aElemD;
    T_ELEM_K ElemKFirst = pDoc->m_pAttrCtrl->GetLastNumElem()+1;

    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFLoopForMeshSizeFind"), _T("yes")) ) // Mesh크기를 조절해 가면서 수렴값을 찾기 위한 레지스트리
            {
                dAddPos = 200 + 50 * CDb_DllBaseManager::m_dMeshSizeRatio;
            }

            NodeK1 = NodeKFirst;
            NodeKFirst++;
            NodeD.Initialize();
            NodeD.x = aaEdge[i].aAr[j].p1.x + dAddPos; // 이미 만들어져 있는 모델이랑 겹치지 않게 더함
            NodeD.y = aaEdge[i].aAr[j].p1.y + dAddPos;
            NodeD.z = dAddPos;

            aNodeK.Add(NodeK1);
            aNodeD.Add(NodeD);

            NodeK2 = NodeKFirst;
            NodeKFirst++;
            NodeD.Initialize();
            NodeD.x = aaEdge[i].aAr[j].p2.x + dAddPos; // 이미 만들어져 있는 모델이랑 겹치지 않게 더함
            NodeD.y = aaEdge[i].aAr[j].p2.y + dAddPos;
            NodeD.z = dAddPos;

            aNodeK.Add(NodeK2);
            aNodeD.Add(NodeD);

            ElemK = ElemKFirst;
            ElemKFirst++;
            ElemD.Initialize();
            ElemD.eltyp = BEAM_EL;
            ElemD.elmat = 1;
            ElemD.elpro = 1;
            ElemD.elnod[0] = NodeK1;
            ElemD.elnod[1] = NodeK2;

            aElemK.Add(ElemK);
            aElemD.Add(ElemD);
        }
    }

    pDoc->m_pDataCtrl->AddNode(aNodeK, aNodeD);
    pDoc->m_pDataCtrl->AddElem(aElemK, aElemD);
}

void WriteNodeOfMesh(CArray<T_SIMPLE_MESH_EDGE_D, T_SIMPLE_MESH_EDGE_D&>& aEdge_simple, CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&>& aNode_simple)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    T_NODE_K NodeK;
    T_NODE_D NodeD;

    CArray<T_NODE_K, T_NODE_K> aNodeK;
    CArray<T_NODE_D, T_NODE_D&> aNodeD;
    T_NODE_K NodeKFirst = pDoc->m_pAttrCtrl->GetLastNumNode()+1;

    double dAddPos = 2000;
    for ( int i = 0; i < aNode_simple.GetSize(); i++ )
    {
        if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFLoopForMeshSizeFind"), _T("yes")) ) // Mesh크기를 조절해 가면서 수렴값을 찾기 위한 레지스트리
        {
            dAddPos = 200 + 20 * CDb_DllBaseManager::m_dMeshSizeRatio;
        }

        NodeK = NodeKFirst + i;
        NodeD.Initialize();
        NodeD.x = aNode_simple[i].y + dAddPos; // 이미 만들어져 있는 모델이랑 겹치지 않게 2000을 더함
        NodeD.y = aNode_simple[i].z + dAddPos;
        NodeD.z = dAddPos;

        aNodeK.Add(NodeK);
        aNodeD.Add(NodeD);
    }

    T_ELEM_K ElemK;
    T_ELEM_D ElemD;

    CArray<T_ELEM_K, T_ELEM_K>  aElemK;
    CArray<T_ELEM_D, T_ELEM_D&> aElemD;
    T_ELEM_K ElemKFirst = pDoc->m_pAttrCtrl->GetLastNumElem()+1;
    for ( int i = 0; i < aEdge_simple.GetSize(); i++ )
    {
        ElemK = ElemKFirst + i;
        ElemD.Initialize();
        ElemD.eltyp = BEAM_EL;
        ElemD.elmat = 1;
        ElemD.elpro = 1;
        ElemD.elnod[0] = aEdge_simple[i].iNode1 + NodeKFirst - 1;
        ElemD.elnod[1] = aEdge_simple[i].iNode2 + NodeKFirst - 1;

        aElemK.Add(ElemK);
        aElemD.Add(ElemD);
    }

    pDoc->m_pDataCtrl->AddNode(aNodeK, aNodeD);
    pDoc->m_pDataCtrl->AddElem(aElemK, aElemD);
}

void WriteNodeOfMesh(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    T_NODE_K NodeK;
    T_NODE_D NodeD;

    CArray<T_NODE_K, T_NODE_K> aNodeK;
    CArray<T_NODE_D, T_NODE_D&> aNodeD;
    T_NODE_K NodeKFirst = pDoc->m_pAttrCtrl->GetLastNumNode()+1;

    double dAddPos = 2000;
    for ( int i = 0; i < raMeshNodeD.GetSize(); i++ )
    {
        if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFLoopForMeshSizeFind"), _T("yes")) ) // Mesh크기를 조절해 가면서 수렴값을 찾기 위한 레지스트리
        {
            dAddPos = 200 + 20 * CDb_DllBaseManager::m_dMeshSizeRatio;
        }

        NodeK = NodeKFirst + i;
        NodeD.Initialize();
        NodeD.x = raMeshNodeD[i].x + dAddPos; // 이미 만들어져 있는 모델이랑 겹치지 않게 2000을 더함
        NodeD.y = raMeshNodeD[i].y + dAddPos;
        NodeD.z = raMeshNodeD[i].z + dAddPos;

        aNodeK.Add(NodeK);
        aNodeD.Add(NodeD);
    }

    T_ELEM_K ElemK;
    T_ELEM_D ElemD;

    CArray<T_ELEM_K, T_ELEM_K>  aElemK;
    CArray<T_ELEM_D, T_ELEM_D&> aElemD;
    T_ELEM_K ElemKFirst = pDoc->m_pAttrCtrl->GetLastNumElem()+1;
    for ( int i = 0; i < raMeshElemD.GetSize(); i++ )
    {
        ElemK = ElemKFirst + i;
        ElemD.Initialize();
        ElemD.eltyp = PLATE_EL;
        ElemD.elmat = 1;
        ElemD.elpro = 1;
        ElemD.elnod[0] = raMeshElemD[i].aNodeK[0] + NodeKFirst;
        ElemD.elnod[1] = raMeshElemD[i].aNodeK[1] + NodeKFirst;
        ElemD.elnod[2] = raMeshElemD[i].aNodeK[2] + NodeKFirst;

        int nNodeSize = raMeshElemD[i].aNodeK.GetSize();    ASSERT(nNodeSize == 3);
        ElemD.elnod[3] = ( nNodeSize == 4 ) ? raMeshElemD[i].aNodeK[3] + NodeKFirst : 0;

        aElemK.Add(ElemK);
        aElemD.Add(ElemD);
    }

    pDoc->m_pDataCtrl->AddNode(aNodeK, aNodeD);
    pDoc->m_pDataCtrl->AddElem(aElemK, aElemD);
}

void WriteMctOfMesh2(int nNode, double* p_ynode, double* p_znode, LPCTSTR lpszPostFix)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    // 데이터 확인용 mct 생성
    //     *NODE    ; Nodes
    //     ; iNO, X, Y, Z
    //     1, 0.5, 3.5, 0
    //
    //     *ELEMENT    ; Elements
    //     ; iEL, TYPE, iMAT, iPRO, iN1, iN2, ANGLE, iSUB, EXVAL, iOPT(EXVAL2) ; Frame  Element
    //     ; iEL, TYPE, iMAT, iPRO, iN1, iN2, iN3, iN4, iSUB, iWID             ; Planar Element
    //     1, BEAM  ,    1,     1,     1,     2,     0
    //     1, PLATE ,    1,     1,     1,     2,     3,     0,     1

    CString strModelPath=_T("");
    strModelPath = pDoc->GetPathName();
    CString strMctPath = _T("");

    if ( !strModelPath.IsEmpty() )
    {
        CFileCtrl FileCtrl(strModelPath);
        CString csFolder=_T("");
        csFolder = FileCtrl.GetFilePathAndNameWithoutExtension();
        strMctPath = csFolder + _T("_mesh_toSolver") + lpszPostFix + (CProduct::IsRusLocal() ? _T(".nbt") : _T(".mct"));
    }
    else { ASSERT(0); return; }

    CDebugFile log(strMctPath);
    log.Clear();

    // *NODE ------------------------
    log.Write(_T("*NODE\n"));
    for ( int i=0; i<nNode; ++i )
    {
        log.Write(_T("%d, 0.0, %.10lf, %.10lf\n"), i+1, p_ynode[i], p_znode[i]);
    }
    log.Write(_T("\n"));

    //   // *ELEMENT ------------------------
    //   log.Write(_T("*ELEMENT\n"));
    //   for (int k=0; k<raMeshElemD.GetSize(); ++k)
    //   {
    //     int nNodeSize = raMeshElemD[k].aNodeK.GetSize();    ASSERT(nNodeSize == 3);
    //     int iNode1 = raMeshElemD[k].aNodeK[0] +1;
    //     int iNode2 = raMeshElemD[k].aNodeK[1] +1;
    //     int iNode3 = raMeshElemD[k].aNodeK[2] +1;
    //     int iNode4 = (nNodeSize == 4)? raMeshElemD[k].aNodeK[3] +1 : 0;
    //     log.Write(_T("%d, PLATE, 1, 1, %d, %d, %d, %d, 1\n"),
    //       k+1, iNode1, iNode2, iNode3, iNode4);
    //   }
    log.Write(_T("\n"));
}

void WriteMctOfMesh(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD, LPCTSTR lpszPostFix)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    // 데이터 확인용 mct 생성
    //     *NODE    ; Nodes
    //     ; iNO, X, Y, Z
    //     1, 0.5, 3.5, 0
    //
    //     *ELEMENT    ; Elements
    //     ; iEL, TYPE, iMAT, iPRO, iN1, iN2, ANGLE, iSUB, EXVAL, iOPT(EXVAL2) ; Frame  Element
    //     ; iEL, TYPE, iMAT, iPRO, iN1, iN2, iN3, iN4, iSUB, iWID             ; Planar Element
    //     1, BEAM  ,    1,     1,     1,     2,     0
    //     1, PLATE ,    1,     1,     1,     2,     3,     0,     1

    CString strModelPath=_T("");
    strModelPath = pDoc->GetPathName();
    CString strMctPath = _T("");

    if ( !strModelPath.IsEmpty() )
    {
        CFileCtrl FileCtrl(strModelPath);
        CString csFolder=_T("");
        csFolder = FileCtrl.GetFilePathAndNameWithoutExtension();
        strMctPath = csFolder + _T("_mesh") + lpszPostFix + (CProduct::IsRusLocal() ? _T(".nbt") : _T(".mct"));
    }
    else { ASSERT(0); return; }

    CDebugFile log(strMctPath);
    log.Clear();

    // *NODE ------------------------
    log.Write(_T("*NODE\n"));
    for ( int i=0; i<raMeshNodeD.GetSize(); ++i )
    {
        log.Write(_T("%d, %g, %g, %g\n"), i+1, raMeshNodeD[i].x, raMeshNodeD[i].y, raMeshNodeD[i].z);
    }
    log.Write(_T("\n"));

    // *ELEMENT ------------------------
    log.Write(_T("*ELEMENT\n"));
    for ( int k=0; k<raMeshElemD.GetSize(); ++k )
    {
        int nNodeSize = raMeshElemD[k].aNodeK.GetSize();    ASSERT(nNodeSize == 3);
        int iNode1 = raMeshElemD[k].aNodeK[0] +1;
        int iNode2 = raMeshElemD[k].aNodeK[1] +1;
        int iNode3 = raMeshElemD[k].aNodeK[2] +1;
        int iNode4 = ( nNodeSize == 4 ) ? raMeshElemD[k].aNodeK[3] +1 : 0;
        log.Write(_T("%d, PLATE, 1, 1, %d, %d, %d, %d, 1\n"),
            k+1, iNode1, iNode2, iNode3, iNode4);
    }
    log.Write(_T("\n"));
}

double CSimpleMesh::GetMeshSizeRatio(CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> &aEdge, int i, double dMaxSize)
{
    double dSeedSizeRatio;

    if ( aEdge[i].bNoDivWhenMesh )
    {
        return 1.0;
    }

    double dLen = aEdge[i].GetLineLength() + 1.0e-3;

    double dMeshLen = min(dMaxSize, dLen);

    double d1mm = M_InitValueCurUnit(0.001, KN, M, D_UNITSYS_BASE_LENGTH);   // 1mm hslee :Slab를 말도안되게 작게 만든 단면에 대한 처리를 위해 Mesh의 최소 Size를 1mm로 함
    dMeshLen = max(d1mm, dMeshLen);

    dSeedSizeRatio = dMeshLen / dLen;
    if ( dSeedSizeRatio > 0.9 )
    {
        dSeedSizeRatio = 1;
    }

    return dSeedSizeRatio;
}

BOOL CSimpleMesh::GenerateMesh(const T_SECT_D& SectD,
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge,
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeAll,
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
    std::vector<int>& raDomainElemCount,
    int nCompShapeType, int nTypeBeforeAfter)
{
    // bGradationMesh는 무조건 TRUE인걸로 처리.. 사실 이 변수 제거 해야 함..

    // nStype == D_SECT_TYPE_STLG_MCELL 은 nCompShapeType 은 항상 1
    // 그외는  nCompShapeType = 0:Slab, 1:Girder, 2:Stiffner

    CAutoMesh_PlanarArea Mesher;
    int NbIndividual = 1;
    int nIth = 0;

    AUTOMESH_ARGUMENTS AutoMeshArg;
    AutoMeshArg.Mesher        = D_MESHER_DELAUNAY;
    AutoMeshArg.MesherType    = D_MESHTYPE_TRIA;
    AutoMeshArg.nInnerDomain  = 0;    // m_bInnerDomain;
    AutoMeshArg.bIncInterEdge = FALSE;
    AutoMeshArg.bRelax        = TRUE;
    AutoMeshArg.dRefineFactor = 1;
    AutoMeshArg.theSizeInfo.nConstraintType = SEED_SIZE_METHOD;
    AutoMeshArg.theSizeInfo.bEmpty = FALSE;
    AutoMeshArg.theSizeInfo.dSize = 1000.0;
    AutoMeshArg.nPropID = 1;

    // SRC이면 타입별로 nInnerDomain 값이 달라져야 함.
    if ( IsSRC(SectD) )                 AutoMeshArg.nInnerDomain  = GetInnerDomainLevel(SectD);
    if ( IsSectWithStiffener(SectD) )   AutoMeshArg.nInnerDomain  = GetInnerDomainLevel(SectD);
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB )   AutoMeshArg.nInnerDomain  = 1;

    m_dMinMeshSize = 0.0;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    int nStype;
    nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    //double dMinSize = 1000.0;
    //double dMinSize = 10.0;
    //double dMinSize = 0.100;
    double dMinSize;
    double dMaxSize;
    GetMinMaxAutoMeshSize(SectD, aEdgeAll, dMinSize, dMaxSize, nCompShapeType, nTypeBeforeAfter, nStype == D_SECT_TYPE_STLG_MCELL || IsBoxGirderShape(nStype));

    AutoMeshArg.theSizeInfo.dSize = dMinSize;

    CArray<double, double&> aSeedSize;   aSeedSize.RemoveAll();
    aSeedSize.SetSize(aEdge.GetSize());
    //for (int i=0; i<aEdge.GetSize(); ++i)   aSeedSize[i] = 1;   // seed 사용 안할 때가 1.
    for ( int i=0; i<aEdge.GetSize(); ++i )   aSeedSize[i] = 0;

    /* // hslee : 이 코드를 뺌. 뺀이유:7자유도 속도 개선 전 Mesh를 크게 나눌 때 당시 Mesh가 커서 Von-Mises and Max-Shear 계산시에 문제가 발생하여.. 이 경우만 Mesh를 작게 만드는 코드를 넣었었음
                                                                         7자유도 개선 후에는 충분히 Mesh를 작게 만들기 때문에 이 코드를 넣을 필요가 없어졌음
    BOOL bDoMeshPerform = TRUE;
    T_ACTL_D DataActl;
    if(!pDoc->m_pAttrCtrl->GetActl(DataActl)) DataActl.Initialize();
    if(DataActl.bCalculateBeamStresses == TRUE)
    {
        // Calculate Beam Stresses (Von-Mises and Max-Shear) 수행을 위해서는 기존 방식인 "최소 mesh 크기로 분할"되어야 함.
        if(CSectUtil::IsShapeEffective(SectD))
        {
            bDoMeshPerform=FALSE;
            double dDivision = 0.1;
            CString strDivision = CTestEnvMgr::GetTestEnvST(_T("TightDivision"));
            if(strDivision.IsEmpty()==FALSE)
            {
                if (strDivision.CompareNoCase(_T("yes")) == 0)
                {
                    dDivision = 0.1;
                }
                else
                {
                    dDivision = _ttof(strDivision);
                }
                if(dDivision<1.0e-6) dDivision = 1.0;
            }
            AutoMeshArg.theSizeInfo.dSize *= dDivision;
        }
    }

    if(bDoMeshPerform)
    {*/
    for ( int i=0; i<aEdge.GetSize(); ++i )
    {
        aSeedSize[i] = GetMeshSizeRatio(aEdge, i, dMaxSize);
    }
    //}

	std::vector<T_LINE_G> aEdgeParent;
    for ( int i = 0; i < aEdge.GetSize(); i++ )
    {
        aEdgeParent.push_back(aEdge[i]);
    }

    //////////////////////////////////////////////////////////////////////////
    // Perform Mesh
    CArray<T_POLYLINE3D*, T_POLYLINE3D*> aDomainPolyLine;
    raDomainElemCount.clear();
    CArray<T_POINT3D, T_POINT3D&> m_aSelectNode;   m_aSelectNode.RemoveAll();
    int nChangeMethod = Mesher.AutoMesh_PlanarArea(aEdgeParent, m_aSelectNode, AutoMeshArg, NbIndividual, nIth, raDomainElemCount, m_dMinMeshSize, aSeedSize, aDomainPolyLine);
    if ( nChangeMethod == -1 )
    {
        pDoc->DisplayHistoryMessage(_T("Simple-mesh failed!"));
        return FALSE;
    }
    // 이 부분은 예전에 만든건데 cppcheck에서 걸림. 절대 안들어오고 있음.
    // 기존에 잘 되고 있는 터라 일단 전체를 막음.
    //   else if(nChangeMethod == 1 && nChangeMethod == 2)
    //   {
    //     AutoMeshArg.MesherType = D_MESHTYPE_TRIAQUAD;
    //   }

    raMeshElemD.Copy(Mesher.AllElems());
    raMeshNodeD.Copy(Mesher.AllNodes());

    for ( int i=0; i<aDomainPolyLine.GetSize(); i++ )
    {
        delete aDomainPolyLine[i];
        aDomainPolyLine[i] = NULL;
    }
    aDomainPolyLine.RemoveAll();

    return TRUE;
}

BOOL CSimpleMesh::IsSRC(const T_SECT_D& SectD)
{
    if ( SectD.nStype == D_SECT_TYPE_SRC ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_SRC ) return TRUE;

    return FALSE;
}

BOOL CSimpleMesh::IsComposite(const T_SECT_D& SectD)
{
    return CSectDB::IsSectComposite(SectD);
}

BOOL CSimpleMesh::IsCompositeSlabNoGap(const T_SECT_D& SectD)
{
    return TRUE;

    // 	if (IsComposite(SectD) && SectD.SectAfter.SectJ.Size[2] <= 0.0)
    // 		return TRUE;
    // 	else
    // 		return FALSE;

    // 	if(CTestEnvMgr::IsTestEnvST(_T("Test_SectTest"), _T("yes")))
    // 	{
    // 		if(CTestEnvMgr::IsTestEnvST(_T("Test_GapMesh"), _T("yes")))
    // 		{
    // 			return TRUE;
    // 		}
    // 		else
    // 		{
    // 			if (IsComposite(SectD) && SectD.SectAfter.SectJ.Size[2] < 1.e-8)
    // 				return TRUE;
    // 			else
    // 				return FALSE;
    // 		}
    // 	}
    // 	else
    // 	{
    // 		return FALSE;
    // 	}
}
BOOL CSimpleMesh::IsCompositeSlabGapMesh(const T_SECT_D& SectD)
{  //Gap을 mesh로 만든 경우
    if ( IsComposite(SectD) && SectD.SectAfter.SectJ.Size[2] > 0.0 )
        return TRUE;
    else
        return FALSE;

    // 	if(CTestEnvMgr::IsTestEnvST(_T("Test_SectTest"), _T("yes")))
    // 	{
    // 		if(CTestEnvMgr::IsTestEnvST(_T("Test_GapMesh"), _T("yes")))
    // 		{
    // 			if (IsComposite(SectD) && SectD.SectAfter.SectJ.Size[2] > 1.e-8)
    // 				return TRUE;
    // 			else
    // 				return FALSE;
    // 		}
    // 	}

    return FALSE;
}

BOOL CSimpleMesh::IsClosedAfterComposite(T_SECT_D& SectD, int nPos)
{
    // 	BOOL bCheck = FALSE;
    // 	if(CTestEnvMgr::IsTestEnvST(_T("Test_SectTest"), _T("yes"))) bCheck = TRUE;
    // 	if(!bCheck) return FALSE;

    int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;
    if ( nStype != D_SECT_TYPE_COMPO_PC ) return FALSE;

    CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdge;
    if ( !GetSectPolygon(SectD, nPos, aaEdge) ) return FALSE;

    int nSlabIdx = 0;
    std::set<int> setGirderIdx;
    setGirderIdx.insert(1);
    // 	if(nStype == D_SECT_TYPE_STLG_MCELL)
    // 	{
    // 		for (int i = 1; i < aaEdge.GetSize(); i++)
    // 		{
    // 			setGirderIdx.insert(i);//MCell 단면은 각 조각(2DShape기준)을 따로 Mesh 한 다음 마지막에 merge 한다.
    // 		}
    // 	}
    // 	else if(IsBoxGirderShape(nStype))
    // 	{
    // 		for (int i = 1; i <= GetBoxGirderShapeNum(nStype); i++)
    // 		{
    // 			setGirderIdx.insert(i); // Box형상의 단면은 각 조각(2DShape기준)을 따로 Mesh 한 다음 마지막에 merge 한다... Stiffner와 Girder Size를 다르게 하기 위해서 MCell과 다른 처리를 추가로 함
    // 		}
    // 	}
    // 	else
    // 	{
    // 		setGirderIdx.insert(1);
    // 	}

    for ( int nGirderIdx : setGirderIdx )
    {
        for ( int i = 0; i < aaEdge.GetSize(); i++ )
        {
            if ( i == nGirderIdx ) continue;

            //if(i == nSlabIdx && !bShareWidthSlab) continue;

            if ( aaEdge[i].aAr.GetSize() == 0 )
                continue;  // Steel Girder 단면의 Slab

// 			BOOL bStiffener;
// 			if(nStype == D_SECT_TYPE_STLG_MCELL)
// 			{
// 				bStiffener = FALSE;
// 			}
// 			else if(IsBoxGirderShape(nStype))
// 			{
// 				bStiffener = i > GetBoxGirderShapeNum(nStype);
// 			}
// 			else
// 			{
// 				bStiffener = i >= 2;
// 			}
//
            CArray<int, int> arIntsGirderIdx;
            //std::vector<int> aIntsGirderIdx;
            std::vector<int> aIntsLoopIdx;

            int nIntsCnt = 0;

            // 슬래브에 중복되는 거더 line 정보 (arIntsGirderIdx)
            CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeGirder = aaEdge[nGirderIdx].aAr;
            CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeLoop = aaEdge[i].aAr;
            for ( int j = 0; j < aEdgeGirder.GetSize(); j++ )
            {
                for ( int k = 0; k < aEdgeLoop.GetSize(); k++ )
                {
                    BOOL bOverlap = IsLineOverlap(aEdgeGirder[j], aEdgeLoop[k]); // 두개의 선분이 겹치는지

                    if ( bOverlap )
                    {
                        if ( fabs(aEdgeGirder[j].p1.y -  aEdgeGirder[j].p2.y) > 1.0e-08 ) continue;

                        arIntsGirderIdx.Add(j);
                        //aIntsGirderIdx.push_back(j);
                        aIntsLoopIdx.push_back(k);
                        nIntsCnt++;
                    }
                }
            }

            //
            int nGirderIdx = arIntsGirderIdx.GetSize();
            for ( int j = 0; j < nGirderIdx-1; j++ )
            {
                int nIdx = arIntsGirderIdx[j];

                int nIdxNxt = arIntsGirderIdx[j+1];

                if ( nIdx != nGirderIdx-1 )
                {
                    if ( nIdxNxt != nIdx+1 ) return TRUE;  // 연결되는 line이 아님 => 합성 후 Closed 단면임
                }
                else
                {// 마지막 위치인 경우
                    if ( nIdxNxt != 0 ) return TRUE;  // 연결되는 line이 아님 => 합성 후 Closed 단면임
                }
            }
        }
    }

    return FALSE;
}

BOOL CSimpleMesh::GetSlabPosition(T_SECT_D& SectD, int nPos, double* pointySl, double* pointzSl)
{
    if ( !CSectDB::IsSectSteelComposite(SectD) ) return FALSE;

    CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdge;
    if ( !GetSectPolygon(SectD, nPos, aaEdge) ) return FALSE;

    if ( aaEdge[0].aAr.GetSize() != 4 ) return FALSE;// Slab는 직선 4개임.

    for ( int i=0; i<4; i++ )
    {
        pointySl[i] =   aaEdge[0].aAr[i].p1.x;
        pointzSl[i] =   aaEdge[0].aAr[i].p1.y;
    }

    return TRUE;
}

BOOL CSimpleMesh::IsSectWithStiffener(const T_SECT_D& SectD)
{
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF )   return TRUE;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF )   return TRUE;

    return FALSE;
}

// =0: 내부 도메인 안만들기, =1: 내부 도메인 모두 만들기, =2: 내부의 내부 도메인 안만들기
int CSimpleMesh::GetInnerDomainLevel(const T_SECT_D& SectD)
{
    int nInnerDomain = 0;   // =0: 내부 도메인 안만들기, =1: 내부 도메인 모두 만들기, =2: 내부의 내부 도메인 안만들기
    if ( IsSRC(SectD) )
    {
        nInnerDomain = 1;

        // 제일 안쪽이 hole인 경우
        if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RBO )   nInnerDomain = 2;
        if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RPO )   nInnerDomain = 2;
        if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CBO )   nInnerDomain = 2;
        if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CPO )   nInnerDomain = 2;
        if (CSectDB::IsSectDumbAndStub(SectD))                  nInnerDomain = 0;
    }
    else
    {
        // Inner Domain들 중 Center가 Hole인 경우
        if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF )   nInnerDomain = 1;
        if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF )   nInnerDomain = 1;
    }

    return nInnerDomain;
}

// SRC의 경우 Domain별 재질 타입
// @param raDomainMaterialType (0: RC, 1: Steel)
// <재질 위치 파악 방법>
// - SRC 각 Domain의 최상단 point를 가지고 단면별로 판단.
// - Stiffener가 있는 경우에는 면적이 제일 큰 단면이 RC
BOOL CSimpleMesh::GetDomainMaterialType_SRC(CArray<int, int>& raDomainMaterialType,
    const T_SECT_D& SectD,
    const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
    const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
    const std::vector<int>& aDomainElemCount)
{
    raDomainMaterialType.RemoveAll();

    BOOL bHaveMultiMaterial = FALSE;
    if ( IsSRC(SectD) ) bHaveMultiMaterial = TRUE;

    if ( !bHaveMultiMaterial )  return FALSE;

    int nCountDomain = (int)aDomainElemCount.size();

    // 각 domain별 최상단 위치
    CArray<double, double> aDomainTopPos;  aDomainTopPos.RemoveAll();
    int iStart = 0;
    int iEndCount = 0;
    int iCentDomain = 0;    // for steel box domain which is hole.
    double dDistFromZero = DBL_MAX;    // 가운데 부분이 hole인 경우를 찾기 위함. (0,0)에 가까운 위치를 찾음.
    double dDist = 0.0;

    for ( int i=0; i<nCountDomain; ++i )        // for domain
    {
        if ( i>0 ) iStart += aDomainElemCount[i-1];
        iEndCount = iStart+aDomainElemCount[i];

        CMapEx<int, int, int, int> mNodeOfDomain;    mNodeOfDomain.RemoveAll();

        // 개별 Domain에 대한 loop
        double dTopPos = -DBL_MAX;
        T_MESH_NODE_K NodeK = 0;
        for ( int k=iStart; k<iEndCount; ++k )    // for elem of domain
        {
            NodeK = aMeshElemD[k].aNodeK[0];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
            NodeK = aMeshElemD[k].aNodeK[1];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
            NodeK = aMeshElemD[k].aNodeK[2];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;

            NodeK = aMeshElemD[k].aNodeK[0];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
            NodeK = aMeshElemD[k].aNodeK[1];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
            NodeK = aMeshElemD[k].aNodeK[2];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
        }

        aDomainTopPos.Add(dTopPos);
    }

    // 위치별로 domain 정렬
    int nSize=aDomainTopPos.GetSize();    if ( nSize < 2 ) { ASSERT(0); return FALSE; }
    CArray<int, int>aDomainIndexByPos;   for ( int i=0; i<nSize; i++ ) aDomainIndexByPos.Add(i);
    CQSort::QSortDblWithIndex(aDomainIndexByPos.GetData(), aDomainTopPos.GetData(), nSize);

    // 위치별 재질 위치
    int nFixedType   = 0;   // 0: Steel위치를 찾고 나머지는 RC,  1: RC 위치를 찾고 나머지는 Steel
    int iDomainSteel = 0;
    int iDomainRC    = 0;
    int iDomainVoid  = 0;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RBO ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RBC ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RPO ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RPC ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CBO ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CBC ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CPO ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CPC )
    {
        nFixedType = 0;
        int nTargetMaterialOrder = 2;   // n번째 위치
        iDomainSteel = aDomainIndexByPos[aDomainIndexByPos.GetSize()-nTargetMaterialOrder];    // steel domain
    }
    else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_EBC ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_EPC )
    {
        nFixedType = 1;
        int nTargetMaterialOrder = 2;   // n번째 위치
        iDomainRC = aDomainIndexByPos[aDomainIndexByPos.GetSize()-nTargetMaterialOrder];    // RC domain
    }
    else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RIB  ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CIB  ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RH2T ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RHT )
    {
        nFixedType = 1;
        int nTargetMaterialOrder = 1;   // n번째 위치
        iDomainRC = aDomainIndexByPos[aDomainIndexByPos.GetSize()-nTargetMaterialOrder];    // RC domain
    }
    else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_BSTF ||
        SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_PSTF )
    {
        // Stiffener가 있는 경우에는 제일 안쪽을 RC로 판단
        nFixedType = 1;
        iDomainRC = iCentDomain;    // RC domain
    }
    else if (SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_STUB)
    {
        ASSERT(0);
    }
    else if (SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_DUMB)
    {
        nFixedType = 2;
        iDomainSteel = 0;
        iDomainVoid = 3;
    }
    else  // 일단 제일 안쪽이 RC (Box/Pipe with Stiffener, ... etc)
    {
        ASSERT(0);
    }

    // raDomainMaterialType 설정.
    for ( int i=0; i<nCountDomain; ++i )
    {
        if ( nFixedType == 0 )  // 0: Steel위치를 찾고 나머지는 RC
        {
            if ( i == iDomainSteel )  raDomainMaterialType.Add(1);
            else                    raDomainMaterialType.Add(0);
        }
        else if ( nFixedType == 1 )  // 1: RC 위치를 찾고 나머지는 Steel
        {
            if ( i == iDomainRC )  raDomainMaterialType.Add(0);
            else                 raDomainMaterialType.Add(1);
        }
        else if (nFixedType == 2)
        {
			if      (i == iDomainSteel)  raDomainMaterialType.Add(1);
            else if (i == iDomainVoid)   raDomainMaterialType.Add(-1);
			else                         raDomainMaterialType.Add(0);

        }
    }

    return TRUE;
}

// Composite의 경우 Domain별 재질 타입
// @param raDomainMaterialType (0: RC, 1: Steel, -1: hole)
// <재질 위치 파악 방법>
// - Composite 각 Domain의 최상단 point를 가지고 단면별로 판단.
// - 최상단(Slab)가 RC
// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2, 20=Stiffener(CSimpleMesh::GetSectMesh에서만 세팅 함)
// 필독!! 이 함수를 고치면 CSimpleMesh::GetSectMesh 함수도 재대로 동작하는지 꼭 확인 해야 합니다!!!!
BOOL CSimpleMesh::GetDomainMaterialType_Composite(CArray<int, int>& raDomainMaterialType,
    const T_SECT_D& SectD,
    const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
    const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
    const std::vector<int>& aDomainElemCount,
    const int nTypeBeforeAfter)
{
    // SectTest
    if ( CDb_DllBaseManager::m_bAllSectMesh )
    {
        if ( GetDomainMaterialType_Composite_AllSect(raDomainMaterialType, SectD, aMeshElemD, aMeshNodeD, aDomainElemCount, nTypeBeforeAfter) )
            return TRUE;
        else
            return FALSE;
    }

    raDomainMaterialType.RemoveAll();

    BOOL bHaveMultiMaterial = FALSE;

    int nStype;
    nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    if ( IsComposite(SectD) )  // D_SECT_TYPE_COMPO_G은 연산 안함
    {
        if ( nStype == D_SECT_TYPE_COMPO_B ) bHaveMultiMaterial = TRUE;    // Steel Box
        if ( nStype == D_SECT_TYPE_COMPO_I ) bHaveMultiMaterial = TRUE;    // Steel I
        if ( nStype == D_SECT_TYPE_COMPO_TUB ) bHaveMultiMaterial = TRUE;    // Steel Tub
        if ( nStype == D_SECT_TYPE_COMPO_STLG_B ) bHaveMultiMaterial = TRUE;    // Steel Girder Box
        if ( nStype == D_SECT_TYPE_COMPO_STLG_I ) bHaveMultiMaterial = TRUE;    // Steel Girder I
        if ( nStype == D_SECT_TYPE_COMPO_STLG_TUB ) bHaveMultiMaterial = TRUE;    // Steel Girder Tub
        if ( nStype == D_SECT_TYPE_COMPO_CI ) bHaveMultiMaterial = TRUE;    // Composite Concrete-I
        if ( nStype == D_SECT_TYPE_COMPO_CT ) bHaveMultiMaterial = TRUE;    // Composite Concrete-T
        if ( nStype == D_SECT_TYPE_COMPO_PC ) bHaveMultiMaterial = TRUE;    // Composite Concrete-T
    }
    if ( CSectDB::IsSectStlGirder(SectD) ) bHaveMultiMaterial = TRUE;

    if ( !bHaveMultiMaterial )  return FALSE;

    int nCountDomain = aDomainElemCount.size();

    if ( nStype == D_SECT_TYPE_COMPO_PC )
    {
        // PSC Composite은 기존방식으로 처리 (여러개 Polygon은 Domain을 쪼개서 pBeam에 넘겨줘야 하는데 이를 구현하기 힘들어서..)
        int nCountDomain = aDomainElemCount.size();

        // 각 domain별 최상단 위치
        CArray<double, double> aDomainTopPos;  aDomainTopPos.RemoveAll();
        int iStart = 0;
        int iEndCount = 0;
        double dDist = 0.0;
        for ( int i=0; i<nCountDomain; ++i )        // for domain
        {
            if ( i>0 ) iStart += aDomainElemCount[i-1];
            iEndCount = iStart+aDomainElemCount[i];

            CMapEx<int, int, int, int> mNodeOfDomain;    mNodeOfDomain.RemoveAll();

            // 개별 Domain에 대한 loop
            double dTopPos = -DBL_MAX;
            T_MESH_NODE_K NodeK = 0;
            for ( int k=iStart; k<iEndCount; ++k )    // for elem of domain
            {
                NodeK = aMeshElemD[k].aNodeK[0];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
                NodeK = aMeshElemD[k].aNodeK[1];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
                NodeK = aMeshElemD[k].aNodeK[2];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
            }

            aDomainTopPos.Add(dTopPos);
        }

        // 위치별로 domain 정렬
        int nSize=aDomainTopPos.GetSize();    if ( nSize < 2 ) { ASSERT(0); return FALSE; }
        CArray<int, int>aDomainIndexByPos;   for ( int i=0; i<nSize; i++ ) aDomainIndexByPos.Add(i);
        CQSort::QSortDblWithIndex(aDomainIndexByPos.GetData(), aDomainTopPos.GetData(), nSize);

        // 위치별 재질 위치
        int nFixedType        = 0;   // 0: Steel위치를 찾고 나머지는 RC,  1: RC 위치를 찾고 나머지는 Steel
        int iDomainSteel      = 0;
        int iDomainRC_topSlab = 0;

        nFixedType = 1;
        int nTargetMaterialOrder = 1;   // n번째 위치
        iDomainRC_topSlab = aDomainIndexByPos[aDomainIndexByPos.GetSize()-nTargetMaterialOrder];    // RC domain

        // raDomainMaterialType 설정.
        for ( int i=0; i<nCountDomain; ++i )
        {
            if ( nFixedType == 0 )  // 0: Steel위치를 찾고 나머지는 RC
            {
                if ( i == iDomainSteel )  raDomainMaterialType.Add(1);
                else                    raDomainMaterialType.Add(0);
            }
            else if ( nFixedType == 1 )  // 1: RC 위치를 찾고 나머지는 Steel
            {
                if ( i == iDomainRC_topSlab )  raDomainMaterialType.Add(0);
                else                         raDomainMaterialType.Add(1);
            }
        }

        if ( nTypeBeforeAfter == 1 )
        {
            raDomainMaterialType.SetAt(iDomainRC_topSlab, -1);    // 시공단계 합성 전에는 상부 슬래브 없음.
        }
        else if ( nTypeBeforeAfter == 11 ) // part1 (girder)
        {
            // slab는 hole 취급
            for ( int i=0; i<raDomainMaterialType.GetSize(); ++i )
            {
                if ( i == iDomainRC_topSlab ) raDomainMaterialType.SetAt(i, -1);
            }
        }
        else if ( nTypeBeforeAfter == 12 )    // part2 (slab)
        {
            // slab가 아니면 hole 취급
            for ( int i=0; i<raDomainMaterialType.GetSize(); ++i )
            {
                if ( i != iDomainRC_topSlab ) raDomainMaterialType.SetAt(i, -1);
            }
        }
    }
    else
    {
        if ( nTypeBeforeAfter == 1 || nTypeBeforeAfter == 2 )
        {
            ASSERT(0); // 여기에 ASSERT가 들어오면 Girder, Slab, Stiffener가 따로 Mesh 한 다음 Merge해야 하는지.. 혹은 regular(기존방식)으로 해야 하는지 확인 한 다음 코드를 고치시오.
        }
        else if ( nTypeBeforeAfter == 12 )
        {
            // CSimpleMesh::GetSectMesh 에서 Slab만 추출해서 여기로 넘긴 상황임
            raDomainMaterialType.SetSize(nCountDomain);
            for ( int i = 0; i < nCountDomain; i++ )
            {
                raDomainMaterialType[i] = 0; // RC
            }
        }
        else if ( nTypeBeforeAfter == 20 )
        {
            // CSimpleMesh::GetSectMesh 에서 Stiffener만 추출해서 여기로 넘긴 상황임
            raDomainMaterialType.SetSize(nCountDomain);
            for ( int i = 0; i < nCountDomain; i++ )
            {
                raDomainMaterialType[i] = 1; // Steel
            }
        }
        else if ( nTypeBeforeAfter == 11 )
        {
            // CSimpleMesh::GetSectMesh 에서 Girder만 추출해서 여기로 넘긴 상황임
            raDomainMaterialType.SetSize(nCountDomain);
            for ( int i = 0; i < nCountDomain; i++ )
            {
                raDomainMaterialType[i] = 1; // Steel
            }
        }
    }

    return TRUE;
}
BOOL CSimpleMesh::GetDomainMaterialType_Composite_AllSect(CArray<int, int>& raDomainMaterialType,
    const T_SECT_D& SectD,
    const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
    const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
    const std::vector<int>& aDomainElemCount,
    const int nTypeBeforeAfter)
{
    raDomainMaterialType.RemoveAll();

    BOOL bHaveMultiMaterial = FALSE;

    int nStype;
    nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    if ( IsComposite(SectD) )  // D_SECT_TYPE_COMPO_G은 연산 안함
    {
        if ( nStype == D_SECT_TYPE_COMPO_B ) bHaveMultiMaterial = TRUE;    // Steel Box
        if ( nStype == D_SECT_TYPE_COMPO_I ) bHaveMultiMaterial = TRUE;    // Steel I
        if ( nStype == D_SECT_TYPE_COMPO_TUB ) bHaveMultiMaterial = TRUE;    // Steel Tub
        if ( nStype == D_SECT_TYPE_COMPO_STLG_B ) bHaveMultiMaterial = TRUE;    // Steel Girder Box
        if ( nStype == D_SECT_TYPE_COMPO_STLG_I ) bHaveMultiMaterial = TRUE;    // Steel Girder I
        if ( nStype == D_SECT_TYPE_COMPO_STLG_TUB ) bHaveMultiMaterial = TRUE;    // Steel Girder Tub
        if ( nStype == D_SECT_TYPE_COMPO_CI ) bHaveMultiMaterial = TRUE;    // Composite Concrete-I
        if ( nStype == D_SECT_TYPE_COMPO_CT ) bHaveMultiMaterial = TRUE;    // Composite Concrete-T
        if ( nStype == D_SECT_TYPE_COMPO_PC ) bHaveMultiMaterial = TRUE;    // Composite Concrete-T
    }
    if ( CSectDB::IsSectStlGirder(SectD) ) bHaveMultiMaterial = TRUE;
    if ( CSectDB::IsSectDumbAndStub(SectD)) bHaveMultiMaterial = TRUE;

    if ( !bHaveMultiMaterial )  return FALSE;

    int nCountDomain = aDomainElemCount.size();

    if ( !bHaveMultiMaterial/*nStype == D_SECT_TYPE_COMPO_CI || nStype == D_SECT_TYPE_COMPO_CT || nStype == D_SECT_TYPE_COMPO_PC*/ )
    {
        // PSC Composite은 기존방식으로 처리 (여러개 Polygon은 Domain을 쪼개서 pBeam에 넘겨줘야 하는데 이를 구현하기 힘들어서..)
        int nCountDomain = aDomainElemCount.size();

        // 각 domain별 최상단 위치
        CArray<double, double> aDomainTopPos;  aDomainTopPos.RemoveAll();
        int iStart = 0;
        int iEndCount = 0;
        double dDist = 0.0;
        for ( int i=0; i<nCountDomain; ++i )        // for domain
        {
            if ( i>0 ) iStart += aDomainElemCount[i-1];
            iEndCount = iStart+aDomainElemCount[i];

            CMapEx<int, int, int, int> mNodeOfDomain;    mNodeOfDomain.RemoveAll();

            // 개별 Domain에 대한 loop
            double dTopPos = -DBL_MAX;
            T_MESH_NODE_K NodeK = 0;
            for ( int k=iStart; k<iEndCount; ++k )    // for elem of domain
            {
                NodeK = aMeshElemD[k].aNodeK[0];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
                NodeK = aMeshElemD[k].aNodeK[1];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
                NodeK = aMeshElemD[k].aNodeK[2];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
            }

            aDomainTopPos.Add(dTopPos);
        }

        // 위치별로 domain 정렬
        int nSize=aDomainTopPos.GetSize();    if ( nSize < 2 ) { ASSERT(0); return FALSE; }
        CArray<int, int>aDomainIndexByPos;   for ( int i=0; i<nSize; i++ ) aDomainIndexByPos.Add(i);
        CQSort::QSortDblWithIndex(aDomainIndexByPos.GetData(), aDomainTopPos.GetData(), nSize);

        // 위치별 재질 위치
        int nFixedType        = 0;   // 0: Steel위치를 찾고 나머지는 RC,  1: RC 위치를 찾고 나머지는 Steel
        int iDomainSteel      = 0;
        int iDomainRC_topSlab = 0;

        nFixedType = 1;
        int nTargetMaterialOrder = 1;   // n번째 위치
        iDomainRC_topSlab = aDomainIndexByPos[aDomainIndexByPos.GetSize()-nTargetMaterialOrder];    // RC domain

        // raDomainMaterialType 설정.
        for ( int i=0; i<nCountDomain; ++i )
        {
            if ( nFixedType == 0 )  // 0: Steel위치를 찾고 나머지는 RC
            {
                if ( i == iDomainSteel )  raDomainMaterialType.Add(1);
                else                    raDomainMaterialType.Add(0);
            }
            else if ( nFixedType == 1 )  // 1: RC 위치를 찾고 나머지는 Steel
            {
                if ( i == iDomainRC_topSlab )  raDomainMaterialType.Add(0);
                else                         raDomainMaterialType.Add(1);
            }
        }

        if ( nTypeBeforeAfter == 1 )
        {
            raDomainMaterialType.SetAt(iDomainRC_topSlab, -1);    // 시공단계 합성 전에는 상부 슬래브 없음.
        }
        else if ( nTypeBeforeAfter == 11 ) // part1 (girder)
        {
            // slab는 hole 취급
            for ( int i=0; i<raDomainMaterialType.GetSize(); ++i )
            {
                if ( i == iDomainRC_topSlab ) raDomainMaterialType.SetAt(i, -1);
            }
        }
        else if ( nTypeBeforeAfter == 12 )    // part2 (slab)
        {
            // slab가 아니면 hole 취급
            for ( int i=0; i<raDomainMaterialType.GetSize(); ++i )
            {
                if ( i != iDomainRC_topSlab ) raDomainMaterialType.SetAt(i, -1);
            }
        }
    }
    else
    {
        if ( nTypeBeforeAfter == 1 || nTypeBeforeAfter == 2 )
        {
            ASSERT(0); // 여기에 ASSERT가 들어오면 Girder, Slab, Stiffener가 따로 Mesh 한 다음 Merge해야 하는지.. 혹은 regular(기존방식)으로 해야 하는지 확인 한 다음 코드를 고치시오.
        }
        else if ( nTypeBeforeAfter == 12 )
        {
            // CSimpleMesh::GetSectMesh 에서 Slab만 추출해서 여기로 넘긴 상황임
            raDomainMaterialType.SetSize(nCountDomain);
            for ( int i = 0; i < nCountDomain; i++ )
            {
                raDomainMaterialType[i] = 0; // RC
            }
        }
        else if ( nTypeBeforeAfter == 20 )
        {
            // CSimpleMesh::GetSectMesh 에서 Stiffener만 추출해서 여기로 넘긴 상황임
            raDomainMaterialType.SetSize(nCountDomain);
            for ( int i = 0; i < nCountDomain; i++ )
            {
                raDomainMaterialType[i] = 1; // Steel
            }
        }
        else if ( nTypeBeforeAfter == 11 )
        {
            // CSimpleMesh::GetSectMesh 에서 Girder만 추출해서 여기로 넘긴 상황임
            raDomainMaterialType.SetSize(nCountDomain);
            for ( int i = 0; i < nCountDomain; i++ )
            {
                raDomainMaterialType[i] = 1; // Steel
            }
        }
        else if ( nTypeBeforeAfter == 13 ) // Gap
        {
            // CSimpleMesh::GetSectMesh 에서 Girder만 추출해서 여기로 넘긴 상황임
            raDomainMaterialType.SetSize(nCountDomain);
            for ( int i = 0; i < nCountDomain; i++ )
            {
                raDomainMaterialType[i] = 13;
            }
        }
    }

    return TRUE;
}
// PSC > CMPWeb의 경우 Domain별 재질 타입
// @param raDomainMaterialType (0: RC, 1: Steel, -1: hole)
// <재질 위치 파악 방법>
// - 상단+하단 : RC
// - 왼쪽 + 오른쪽 Web : Steel
// - Center : Hole
BOOL CSimpleMesh::GetDomainMaterialType_PSC_CMPWEB(CArray<int, int>& raDomainMaterialType,
    const T_SECT_D& SectD,
    const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
    const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
    const std::vector<int>& aDomainElemCount)
{
    raDomainMaterialType.RemoveAll();

    BOOL bHaveMultiMaterial = TRUE;
    if ( !bHaveMultiMaterial )  return FALSE;

    int nCountDomain = aDomainElemCount.size();

    // 각 domain별 최상단 위치
    CArray<double, double> aDomainTopPos;  aDomainTopPos.RemoveAll();
    int iStart = 0;
    int iEndCount = 0;
    int iCentDomain = 0;    // for steel box domain which is hole.
    double dDistFromZero = DBL_MAX;    // 가운데 부분이 hole인 경우를 찾기 위함. (0,0)에 가까운 위치를 찾음.
    double dDist = 0.0;
    for ( int i=0; i<nCountDomain; ++i )        // for domain
    {
        if ( i>0 ) iStart += aDomainElemCount[i-1];
        iEndCount = iStart+aDomainElemCount[i];

        CMapEx<int, int, int, int> mNodeOfDomain;    mNodeOfDomain.RemoveAll();

        // 개별 Domain에 대한 loop
        double dTopPos = -DBL_MAX;
        T_MESH_NODE_K NodeK = 0;
        for ( int k=iStart; k<iEndCount; ++k )    // for elem of domain
        {
            NodeK = aMeshElemD[k].aNodeK[0];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
            NodeK = aMeshElemD[k].aNodeK[1];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;
            NodeK = aMeshElemD[k].aNodeK[2];   if ( dTopPos < aMeshNodeD[NodeK].y ) dTopPos = aMeshNodeD[NodeK].y;

            NodeK = aMeshElemD[k].aNodeK[0];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
            NodeK = aMeshElemD[k].aNodeK[1];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
            NodeK = aMeshElemD[k].aNodeK[2];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
        }

        aDomainTopPos.Add(dTopPos);
    }

    // 위치별로 domain 정렬
    int nSize=aDomainTopPos.GetSize();    if ( nSize < 2 ) { ASSERT(0); return FALSE; }
    CArray<int, int>aDomainIndexByPos;   for ( int i=0; i<nSize; i++ ) aDomainIndexByPos.Add(i);
    CQSort::QSortDblWithIndex(aDomainIndexByPos.GetData(), aDomainTopPos.GetData(), nSize);

    // 위치별 재질 위치
    int iDomainRC_top    = aDomainIndexByPos[aDomainIndexByPos.GetSize()-1];    // RC domain (Bottom)
    int iDomainRC_bot    = aDomainIndexByPos[0];    // RC domain (Top)
    int iDomainHole      = iCentDomain;

    // raDomainMaterialType 설정.
    for ( int i=0; i<nCountDomain; ++i )
    {
        if ( i == iDomainRC_top ) { raDomainMaterialType.Add(1);  continue; }
        if ( i == iDomainRC_bot ) { raDomainMaterialType.Add(1);  continue; }

        if ( i == iDomainHole ) { raDomainMaterialType.Add(-1);  continue; }

        // 좌우 CMP web (RC??)
        raDomainMaterialType.Add(0);
    }

    return TRUE;
}

// Composite의 경우 Domain별 재질 타입
// @param raDomainMaterialType (0: RC, 1: Steel, -1: hole)
// <재질 위치 파악 방법>
// - Composite 각 Domain의 최상단 point를 가지고 단면별로 판단.
// - 최상단(Slab)가 RC
BOOL CSimpleMesh::GetDomainMaterialType_SectWithStiffener(CArray<int, int>& raDomainMaterialType,
    const T_SECT_D& SectD,
    const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
    const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
    const std::vector<int>& aDomainElemCount)
{
    raDomainMaterialType.RemoveAll();

    BOOL bHaveMultiMaterial = FALSE;
    if ( IsSectWithStiffener(SectD) ) bHaveMultiMaterial = TRUE;  // hole을 가지고 있다.
    if ( !bHaveMultiMaterial )  return FALSE;

    int nCountDomain = aDomainElemCount.size();

    int iStart = 0;
    int iEndCount = 0;
    int iCentDomain = 0;    // for center hole.
    double dDistFromZero = DBL_MAX;    // 가운데 부분이 hole인 경우를 찾기 위함. (0,0)에 가까운 위치를 찾음.
    double dDist = 0.0;
    for ( int i=0; i<nCountDomain; ++i )        // for domain
    {
        if ( i>0 ) iStart += aDomainElemCount[i-1];
        iEndCount = iStart+aDomainElemCount[i];

        // 개별 Domain에 대한 loop
        T_MESH_NODE_K NodeK = 0;
        for ( int k=iStart; k<iEndCount; ++k )    // for elem of domain
        {
            NodeK = aMeshElemD[k].aNodeK[0];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
            NodeK = aMeshElemD[k].aNodeK[1];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
            NodeK = aMeshElemD[k].aNodeK[2];   dDist = CMathFunc::mathLength(aMeshNodeD[NodeK].x, aMeshNodeD[NodeK].y);   if ( fabs(dDistFromZero) > fabs(dDist) ) { dDistFromZero = dDist;  iCentDomain = i; }
        }
    }

    // raDomainMaterialType 설정.
    for ( int i=0; i<nCountDomain; ++i )
    {
        raDomainMaterialType.Add(1);    // 전부 steel
    }

    // 가운데 부분이 hole. => 재질이 없는 타입.
    BOOL bIsCenterHole = TRUE;
    if ( bIsCenterHole )  raDomainMaterialType.SetAt(iCentDomain, -1);

    return TRUE;
}

// 중복점 삭제하고 한개 domain으로 Merge
BOOL CSimpleMesh::MergeDomainMesh(const T_SECT_D& SectD,
    CArray<int, int>& raDomainMaterialType,
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
	std::vector<int>& raDomainElemCount)
{
    // SectTest
    if ( CDb_DllBaseManager::m_bAllSectMesh )
    {
        if ( MergeDomainMesh_AllSect(SectD, raDomainMaterialType, raMeshElemD, raMeshNodeD, raDomainElemCount) )
            return TRUE;
        else
            return FALSE;
    }

    if ( raDomainElemCount.size() <= 1 )  return TRUE;

    // 동일 재질인지 검사.
    if ( raDomainMaterialType.GetSize() > 1 )
    {
        int iMatlType = raDomainMaterialType[0];
        for ( int i=0; i<raDomainMaterialType.GetSize(); ++i )
        {
            if ( raDomainMaterialType[i] != iMatlType ) { ASSERT(0); return FALSE; }
        }
    }

    int nCountNode = raMeshNodeD.GetSize();

    // node 위치 정보
    CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&> aNode;  aNode.RemoveAll();
    int    nNo = 0;
    double y   = 0.0;
    double z   = 0.0;
    T_SIMPLE_MESH_NODE_D node;
    for ( int i=0; i<raMeshNodeD.GetSize(); ++i )
    {
        int nNo = i+1;
        y = raMeshNodeD.GetAt(i).x;
        z = raMeshNodeD.GetAt(i).y;
        node.Set(nNo, y, z);

        aNode.Add(node);
    }

    // 중복 node 검사.
    CArray<BOOL, BOOL> aRemoveFlag;    aRemoveFlag.SetSize(nCountNode);
    for ( int i=0; i<aRemoveFlag.GetSize(); ++i )  aRemoveFlag[i] = FALSE;

    CMapEx<int, int, int, int> mNodeOld2New;   // 중복 node 제거에 의해 변경되는 node 번호
    mNodeOld2New.InitHashTable(97);

    const double dTol = 1.e-8; // 1.e-8로 한 이유는 TaperedSection일 때 SectDB에서 맞물리는지 판단하는 연산 시 사용하는 톨러런스랑 맞추었음. + PBeamDll에서 1.e-8기준으로 NodeMerge를 하기 때문에 그거보다 크거나 같은 값을 해야 함
    for ( int i=raMeshNodeD.GetSize()-1; i>=0; --i )
    {
        mNodeOld2New.SetAt(i, i);

        for ( int k=i-1; k>=0; --k )
        {
            if ( aNode[i].IsSame(aNode[k], dTol) )
            {
                aRemoveFlag[i] = TRUE;
                mNodeOld2New.SetAt(i, k);
            }
        }
    }

    // 중복 node 삭제를 고려한 번호 조정
    {
        CArray<int, int> aRemoveOffset;    aRemoveOffset.SetSize(aNode.GetSize());
        int nRemoveNum = 0;
        for ( int i=0; i<aRemoveFlag.GetSize(); ++i )
        {
            if ( aRemoveFlag[i] ) nRemoveNum++;
            aRemoveOffset[i] = nRemoveNum;
        }

        int iOld, iNew;
        POSITION pos = mNodeOld2New.GetStartPosition();
        while ( pos )
        {
            mNodeOld2New.GetNextAssoc(pos, iOld, iNew);
            iNew -= aRemoveOffset[iNew];
            mNodeOld2New.SetAt(iOld, iNew);
        }
    }

    int iOld, iNew;

    // 삭제 대상 nodes 삭제
    for ( int i=raMeshNodeD.GetSize()-1; i>=0; --i ) { if ( aRemoveFlag[i] )   raMeshNodeD.RemoveAt(i); }

    // Elements에 변경된 절점 번호 반영
    for ( int i=0; i< raMeshElemD.GetSize(); ++i )
    {
        ASSERT(raMeshElemD[i].aNodeK.GetSize() == 3);

        iOld = raMeshElemD[i].aNodeK[0];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[0] = iNew;
        else ASSERT(0);

        iOld = raMeshElemD[i].aNodeK[1];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[1] = iNew;
        else ASSERT(0);

        iOld = raMeshElemD[i].aNodeK[2];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[2] = iNew;
        else ASSERT(0);
    }

    // 단일 domain 정보 setting
    if ( raDomainMaterialType.GetSize() > 0 )   raDomainMaterialType.SetSize(1);

	raDomainElemCount.clear();
	raDomainElemCount.push_back(raMeshElemD.GetSize());

    return TRUE;
}

BOOL CSimpleMesh::MergeDomainMesh_AllSect(const T_SECT_D& SectD,
    CArray<int, int>& raDomainMaterialType,
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
	std::vector<int>& raDomainElemCount)
{
    if ( raDomainElemCount.size() <= 1 )  return TRUE;

    // 	// 동일 재질인지 검사.
    // 	if (raDomainMaterialType.GetSize() > 1)
    // 	{
    // 		int iMatlType = raDomainMaterialType[0];
    // 		for (int i=0; i<raDomainMaterialType.GetSize(); ++i)
    // 		{
    // 			if (raDomainMaterialType[i] != iMatlType)  { ASSERT(0); return FALSE; }
    // 		}
    // 	}

    int nCountNode = raMeshNodeD.GetSize();

    // node 위치 정보
    CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&> aNode;  aNode.RemoveAll();
    int    nNo = 0;
    double y   = 0.0;
    double z   = 0.0;
    T_SIMPLE_MESH_NODE_D node;
    for ( int i=0; i<raMeshNodeD.GetSize(); ++i )
    {
        int nNo = i+1;
        y = raMeshNodeD.GetAt(i).x;
        z = raMeshNodeD.GetAt(i).y;
        node.Set(nNo, y, z);

        aNode.Add(node);
    }

    // 중복 node 검사.
    CArray<BOOL, BOOL> aRemoveFlag;    aRemoveFlag.SetSize(nCountNode);
    for ( int i=0; i<aRemoveFlag.GetSize(); ++i )  aRemoveFlag[i] = FALSE;

    CMapEx<int, int, int, int> mNodeOld2New;   // 중복 node 제거에 의해 변경되는 node 번호
    mNodeOld2New.InitHashTable(97);

    const double dTol = 1.e-8; // 1.e-8로 한 이유는 TaperedSection일 때 SectDB에서 맞물리는지 판단하는 연산 시 사용하는 톨러런스랑 맞추었음. + PBeamDll에서 1.e-8기준으로 NodeMerge를 하기 때문에 그거보다 크거나 같은 값을 해야 함
    for ( int i=raMeshNodeD.GetSize()-1; i>=0; --i )
    {
        mNodeOld2New.SetAt(i, i);

        for ( int k=i-1; k>=0; --k )
        {
            if ( aNode[i].IsSame(aNode[k], dTol) )
            {
                aRemoveFlag[i] = TRUE;
                mNodeOld2New.SetAt(i, k);
            }
        }
    }

    // 중복 node 삭제를 고려한 번호 조정
    {
        CArray<int, int> aRemoveOffset;    aRemoveOffset.SetSize(aNode.GetSize());
        int nRemoveNum = 0;
        for ( int i=0; i<aRemoveFlag.GetSize(); ++i )
        {
            if ( aRemoveFlag[i] ) nRemoveNum++;
            aRemoveOffset[i] = nRemoveNum;
        }

        int iOld, iNew;
        POSITION pos = mNodeOld2New.GetStartPosition();
        while ( pos )
        {
            mNodeOld2New.GetNextAssoc(pos, iOld, iNew);
            iNew -= aRemoveOffset[iNew];
            mNodeOld2New.SetAt(iOld, iNew);
        }
    }

    int iOld, iNew;

    // 삭제 대상 nodes 삭제
    for ( int i=raMeshNodeD.GetSize()-1; i>=0; --i ) { if ( aRemoveFlag[i] )   raMeshNodeD.RemoveAt(i); }

    // Elements에 변경된 절점 번호 반영
    for ( int i=0; i< raMeshElemD.GetSize(); ++i )
    {
        ASSERT(raMeshElemD[i].aNodeK.GetSize() == 3);

        iOld = raMeshElemD[i].aNodeK[0];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[0] = iNew;
        else ASSERT(0);

        iOld = raMeshElemD[i].aNodeK[1];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[1] = iNew;
        else ASSERT(0);

        iOld = raMeshElemD[i].aNodeK[2];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[2] = iNew;
        else ASSERT(0);
    }

    //	// 단일 domain 정보 setting
    // 	if (raDomainMaterialType.GetSize() > 0)   raDomainMaterialType.SetSize(1);
    // 	raDomainElemCount.SetSize(1);   raDomainElemCount[0] = raMeshElemD.GetSize();

    return TRUE;
}
// hole domain 삭제
BOOL CSimpleMesh::RemoveDomainHole(const T_SECT_D& SectD,
    CArray<int, int>& raDomainMaterialType,
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
	std::vector<int>& raDomainElemCount)
{
    if ( CDb_DllBaseManager::m_bAllSectMesh )
    {
        // PSC Composite 단면에서 거더만 고려될때 슬래브는 제외하기 위함.
    }
    else if ( IsComposite(SectD) || CSectDB::IsSectStlGirder(SectD) )  return TRUE; // Composite 과 Steel Girder에서는 Hole을 뺄 필요가 없음 ( Hole이 있는 단면은 단면을 쪼개어 Mesh 한 다음 합침 )

    if ( raDomainMaterialType.GetSize() == 0 )  return TRUE;
    if ( raDomainMaterialType.GetSize() != raDomainElemCount.size() ) { ASSERT(0); return FALSE; }

    // 삭제 정보
    CMapEx<int, int, int, int> mOld2New_Elem;   mOld2New_Elem.RemoveAll();  mOld2New_Elem.InitHashTable(997);
    CMapEx<int, int, int, int> mOld2New_Node;   mOld2New_Node.RemoveAll();  mOld2New_Node.InitHashTable(997);

    int nCountNode_before = raMeshNodeD.GetSize();
    CArray<BOOL, BOOL> aRemoveFlag_Node;    aRemoveFlag_Node.SetSize(nCountNode_before);
    for ( int i=0; i<aRemoveFlag_Node.GetSize(); ++i )  aRemoveFlag_Node[i] = FALSE;

    int nCountElem_before = raMeshElemD.GetSize();
    CArray<BOOL, BOOL> aRemoveFlag_Elem;    aRemoveFlag_Elem.SetSize(raMeshElemD.GetSize());
    for ( int i=0; i<aRemoveFlag_Elem.GetSize(); ++i )  aRemoveFlag_Elem[i] = FALSE;

    // 삭제될 Domain의 elements, nodes 정보 수집
    int iCurElem = 0;
    int iNode0 = 0;
    int iNode1 = 0;
    int iNode2 = 0;

    for ( int i=0; i<raDomainMaterialType.GetSize(); ++i )
    {
        int nElemCountOfDomain = raDomainElemCount[i];
        if ( raDomainMaterialType[i] == -1 )  // 0: RC, 1: Steel, -1: Hole
        {
            for ( int k=0; k<nElemCountOfDomain; ++k )
            {
                ASSERT(raMeshElemD[iCurElem].aNodeK.GetSize() == 3);
                iNode0 = raMeshElemD[iCurElem].aNodeK[0];   aRemoveFlag_Node[iNode0] = TRUE;
                iNode1 = raMeshElemD[iCurElem].aNodeK[1];   aRemoveFlag_Node[iNode1] = TRUE;
                iNode2 = raMeshElemD[iCurElem].aNodeK[2];   aRemoveFlag_Node[iNode2] = TRUE;

                aRemoveFlag_Elem[iCurElem] = TRUE;
                iCurElem++;
            }
        }
        else  iCurElem += nElemCountOfDomain;
    }

    // node 삭제에 의한 offset 정보 (번호 재조정용)
    CArray<int, int> aRemoveOffset_Node;    aRemoveOffset_Node.SetSize(nCountNode_before);
    int nRemoveNum_Node = 0;
    for ( int i=0; i<aRemoveFlag_Node.GetSize(); ++i )
    {
        if ( aRemoveFlag_Node[i] ) nRemoveNum_Node++;
        aRemoveOffset_Node[i] = nRemoveNum_Node;
    }

    // node 번호 변경 정보
    CMapEx<int, int, int, int> mNodeOld2New;    mNodeOld2New.InitHashTable(997);
    for ( int i=0; i<nCountNode_before; ++i )   mNodeOld2New.SetAt(i, i);

    int iOld, iNew;
    POSITION pos = mNodeOld2New.GetStartPosition();
    while ( pos )
    {
        mNodeOld2New.GetNextAssoc(pos, iOld, iNew);
        iNew -= aRemoveOffset_Node[iNew];
        mNodeOld2New.SetAt(iOld, iNew);
    }

    // 삭제 대상 nodes 삭제
    for ( int i=raMeshNodeD.GetSize()-1; i>=0; --i ) { if ( aRemoveFlag_Node[i] )   raMeshNodeD.RemoveAt(i); }

    // element 삭제에 의한 offset 정보 (번호 재조정용)
    CArray<int, int> aRemoveOffset_Elem;    aRemoveOffset_Elem.SetSize(nCountElem_before);
    int nRemoveNum_Elem = 0;
    for ( int i=0; i<aRemoveFlag_Elem.GetSize(); ++i )
    {
        if ( aRemoveFlag_Elem[i] ) nRemoveNum_Elem++;
        aRemoveOffset_Elem[i] = nRemoveNum_Elem;
    }

    // elem 번호 변경 정보
    CMapEx<int, int, int, int> mElemOld2New;    mElemOld2New.InitHashTable(997);
    for ( int i=0; i<nCountElem_before; ++i )   mElemOld2New.SetAt(i, i);

    //int iOld, iNew;
    /*POSITION*/ pos = mElemOld2New.GetStartPosition();
    while ( pos )
    {
        mElemOld2New.GetNextAssoc(pos, iOld, iNew);
        iNew -= aRemoveOffset_Elem[iNew];
        mElemOld2New.SetAt(iOld, iNew);
    }

    // 삭제 대상 elements 삭제
    for ( int i=raMeshElemD.GetSize()-1; i>=0; --i ) { if ( aRemoveFlag_Elem[i] )   raMeshElemD.RemoveAt(i); }

    // Elements에 변경된 절점 번호 반영
    for ( int i=0; i< raMeshElemD.GetSize(); ++i )
    {
        ASSERT(raMeshElemD[i].aNodeK.GetSize() == 3);

        iOld = raMeshElemD[i].aNodeK[0];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[0] = iNew;
        else ASSERT(0);

        iOld = raMeshElemD[i].aNodeK[1];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[1] = iNew;
        else ASSERT(0);

        iOld = raMeshElemD[i].aNodeK[2];
        if ( mNodeOld2New.Lookup(iOld, iNew) )  raMeshElemD[i].aNodeK[2] = iNew;
        else ASSERT(0);
    }

    // 빈 Domain 삭제
    ASSERT(raDomainMaterialType.GetSize() == raDomainElemCount.size());
    for ( int i=raDomainMaterialType.GetSize()-1; i>=0; --i )
    {
        if ( raDomainMaterialType[i] == -1 )  // 0: RC, 1: Steel, -1: Hole
        {
            raDomainMaterialType.RemoveAt(i);
			raDomainElemCount.erase(raDomainElemCount.begin() + i);
        }
    }

    return TRUE;
}

// SRC의 경우 Domain별 재질 타입
// @param raDomainMaterialType (0: RC, 1: Steel, -1: Hole, -2: None)
// <재질 위치 파악 방법>
// - SRC 각 Domain의 최상단 point를 가지고 단면별로 판단.
// - Stiffener가 있는 경우에는 면적이 제일 큰 단면이 RC
BOOL CSimpleMesh::GetDomainMaterialType(CArray<int, int>& raDomainMaterialType,
    const T_SECT_D& SectD,
    const CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
    const CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
    const std::vector<int>& aDomainElemCount,
    const int nTypeBeforeAfter)
{
    raDomainMaterialType.RemoveAll();

    BOOL bHaveMultiMaterial = FALSE;
    if ( IsSRC(SectD) )                                           bHaveMultiMaterial = TRUE;
    if ( IsComposite(SectD) || CSectDB::IsSectStlGirder(SectD) )  bHaveMultiMaterial = TRUE;
    if ( IsSectWithStiffener(SectD) )                             bHaveMultiMaterial = TRUE;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB )      bHaveMultiMaterial = TRUE;

    if ( !bHaveMultiMaterial )  return FALSE;

    if ( IsSRC(SectD) && !CSectDB::IsSectDumbAndStub(SectD))      return GetDomainMaterialType_SRC(raDomainMaterialType, SectD, aMeshElemD, aMeshNodeD, aDomainElemCount);
    if ( CSectDB::IsSectDumbAndStub(SectD))                       return GetDomainMaterialType_Composite(raDomainMaterialType, SectD, aMeshElemD, aMeshNodeD, aDomainElemCount, nTypeBeforeAfter);
    if ( IsComposite(SectD) || CSectDB::IsSectStlGirder(SectD) )  return GetDomainMaterialType_Composite(raDomainMaterialType, SectD, aMeshElemD, aMeshNodeD, aDomainElemCount, nTypeBeforeAfter);
    if ( IsSectWithStiffener(SectD) )                             return GetDomainMaterialType_SectWithStiffener(raDomainMaterialType, SectD, aMeshElemD, aMeshNodeD, aDomainElemCount);
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB )      return GetDomainMaterialType_PSC_CMPWEB(raDomainMaterialType, SectD, aMeshElemD, aMeshNodeD, aDomainElemCount);

    ASSERT(0);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
typedef int ( BSPCombMain )(double* pbeam, double* cdef, int* p_nnode, int* p_npoint, int* p_nelem, int* p_nprop, int* p_ncombined,
    double* p_ynode, double* p_znode, double* p_pointy, double* p_pointz, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem,
    double* p_E, double* p_NU, double* p_w, double* p_sect_e, double* p_sect_nu, double* p_sect_w, int* p_nrebar,
    double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS, int* p_ipotn, int* p_iref, int* p_elemtype);

typedef int ( BSPDetail )(double* force, double* stress, double* coordy, double* coordz, int* p_nnode, int* p_npoint, int* p_nelem, int* p_nprop, int* p_ncombined,
    double* p_ynode, double* p_znode, double* p_pointy, double* p_pointz, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem,
    double* p_E, double* p_NU, double* p_sect_e, double* p_sect_nu, int* p_nrebar,
    double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, int* p_iref, int* nload, double* bst, double* ctex, int* p_elemtype);

typedef int ( BSPPartMain )(double* pbeam, int* p_nnode, int* p_nelem, int* p_nprop, int* p_ncombined,
    double* p_ynode, double* p_znode, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem,
    double* p_E, double* p_NU, double* p_w, double* p_sect_e, double* p_sect_nu, double* p_sect_w, int* p_nrebar,
    double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS, int* p_elemtype);

typedef int ( BSPFirstMomentOfArea )(double* area, double* areay, double* areaz, double* yc, double* zc, int* p_nnode, int* p_nelem, double* p_ynode, double* p_znode,
    int* p_iconn, double* p_W, double* p_sect_w, int* p_nrebar, double* p_asr, double* p_dsy, double* p_dsz, double* p_WS);

// 동일 좌표에 node가 있는지 확인해서 없으면 raNode에 추가하는 함수.
int AddNode(CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&>& raNode, T_SIMPLE_MESH_NODE_D& node, double dTol_merge)
{
    int nExistNode = 0;
    for ( int k=0; k<raNode.GetSize(); ++k )
    {
        //if (raNode[k].IsSame(node, ZERO_4))   { nExistNode = raNode[k].nNo;  break; }
        if ( raNode[k].IsSame(node, dTol_merge) ) { nExistNode = raNode[k].nNo;  break; }
    }

    if ( nExistNode )  return nExistNode;

    node.nNo = raNode.GetSize()+1;    // 번호 새로 부여
    raNode.Add(node);

    return node.nNo;
}

void ConvertSectShape2EdgeNew(CArray<int, int>& aPlaneVCount, CArray<int, int>& aPlaneVertexIndex,
    CArray<double, double>& aVertex,
    CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&>& raNode,
    CArray<T_SIMPLE_MESH_EDGE_D, T_SIMPLE_MESH_EDGE_D&>& raEdge)
{
    raNode.RemoveAll();
    raEdge.RemoveAll();

    ASSERT(aVertex.GetSize()%3 == 0);
    int nnode = aVertex.GetSize()/3;

    // set node
    int nNo = 0;
    double y = 0.0;
    double z = 0.0;
    T_SIMPLE_MESH_NODE_D node;

    int i = 0;
    for ( i=0; i<nnode; ++i )
    {
        nNo = i+1;
        //y = aVertex.GetAt(3*i+1) * (-1);  // 응력계산위치의 좌표계와 맞추기 위하여..
        y = aVertex.GetAt(3*i+1);  // 응력계산위치의 좌표계와 맞추기 위하여..
        z = aVertex.GetAt(3*i+2);
        node.Set(nNo, y, z);

        raNode.Add(node);
    }

    // 중복 node 검사.
    BOOL bAdjustRedundency = TRUE;
    CArray<BOOL, BOOL> aRemoveFlag;    aRemoveFlag.SetSize(raNode.GetSize());
    for (/*int*/ i=0; i<aRemoveFlag.GetSize(); ++i )  aRemoveFlag[i] = FALSE;

    /* hslee : 안쓰는 node를 제거할 이유가 없다..
    // Edge에서 사용하지 않는 node는 제거
    if (bAdjustRedundency)
    {
    for (int i=0; i<aRemoveFlag.GetSize(); ++i)  aRemoveFlag[i] = TRUE;
    for (int k=0; k<aPlaneVertexIndex.GetSize(); ++k) aRemoveFlag[aPlaneVertexIndex[k]] = FALSE;
    }
    */

    CMapEx<int, int, int, int> mNodeOld2New;   // 중복 node 제거에 의해 변경되는 node 번호
    mNodeOld2New.InitHashTable(97);

    // Set Merge Tolerance
    double dY_max = -DBL_MAX;
    double dY_min =  DBL_MAX;
    double dZ_max = -DBL_MAX;
    double dZ_min =  DBL_MAX;
    for (/*int*/ i=0; i<raNode.GetSize(); ++i )
    {
        if ( raNode[i].y > dY_max )  dY_max = raNode[i].y;
        if ( raNode[i].y < dY_min )  dY_min = raNode[i].y;
        if ( raNode[i].z > dZ_max )  dZ_max = raNode[i].z;
        if ( raNode[i].z < dZ_min )  dZ_min = raNode[i].z;
    }
    double dWidth  = dY_max - dY_min;
    double dHeight = dZ_max - dZ_min;

    double dTol_merge = 1.e-8;// hslee : MergeTolerane는 충분히 작게 만드는 대신 Mesh속도개선을 했음..CAutoMesh_PlanarArea::AutoMeshGeneration()의 mapForSpeedUp변수 참고.. 아래 주석은 기존 코드임
    //       : 1.e-8로 한 이유는 TaperedSection일 때 SectDB에서 맞물리는지 판단하는 연산 시 사용하는 톨러런스랑 맞추었음. + PBeamDll에서 1.e-8기준으로 NodeMerge를 하기 때문에 그거보다 크거나 같은 값을 해야 함
    /*
    double dTol_merge = __min(dWidth, dHeight) * 2.0e-2;   // 1.0e-3 기냥... (arc를 표현한다고 해도 1/1000 정도면 충분히 표현할 수 있을 거 같아서 이렇게 잡음.)
    // 1/50으로 키움. 요소 갯수를 줄이기 위해 htlee-20131021
    // MNET:XXXX-HSSHIM-20131127: 단면 치수에 입력되어 있는 최소값이 Merge시에도 살아있게 하기 위한 예외처리.
    if (dMinMergeDim < dTol_merge && dMinMergeDim > 1.0e-4)   dTol_merge = dMinMergeDim*0.9;
    */

    std::set<int> setVerxUse;
    int nCnt = 0;
    for ( i = 0; i < aPlaneVCount.GetSize(); i++ )
    {
        for ( int j = 0; j < aPlaneVCount[i]; j++ )
        {
            setVerxUse.insert(aPlaneVertexIndex[nCnt++]);
        }
    }

    BOOL bNodeRemove = FALSE;
    for (/*int*/ i=raNode.GetSize()-1; i>=0; --i )
    {
        mNodeOld2New.SetAt(i, i);

        if ( setVerxUse.find(i) == setVerxUse.end() )
            continue;

        if ( bAdjustRedundency )
        {
            for ( int k=i-1; k>=0; --k )
            {
                if ( raNode[i].IsSame(raNode[k], dTol_merge) )
                {
                    bNodeRemove = TRUE;
                    aRemoveFlag[i] = TRUE;
                    mNodeOld2New.SetAt(i, k);
                }
            }
        }
    }

    // 중복 node 삭제를 고려한 번호 조정
    if ( bAdjustRedundency && bNodeRemove )
    {
        CArray<int, int> aRemoveOffset;    aRemoveOffset.SetSize(raNode.GetSize());
        int nRemoveNum = 0;
        for (/*int*/ i=0; i<aRemoveFlag.GetSize(); ++i )
        {
            if ( aRemoveFlag[i] ) nRemoveNum++;
            aRemoveOffset[i] = nRemoveNum;
        }

        int iOld, iNew;
        POSITION pos = mNodeOld2New.GetStartPosition();
        while ( pos )
        {
            mNodeOld2New.GetNextAssoc(pos, iOld, iNew);
            iNew -= aRemoveOffset[iNew];
            mNodeOld2New.SetAt(iOld, iNew);
        }

        // 중복 node 삭제
        for (/*int*/ i=raNode.GetSize()-1; i>=0; --i )
        {
            if ( aRemoveFlag[i] )   raNode.RemoveAt(i);
        }

        // 번호정보 갱신
        for (/*int*/ i=0; i<raNode.GetSize(); ++i )  raNode[i].nNo = i+1;
    }

    // set edge
    CMapEx<T_UINT2_INDEX_K, T_UINT2_INDEX_K&, int, int> mEdgeKey_used;    mEdgeKey_used.RemoveAll();  // 중복 edge 검사용.
    T_UINT2_INDEX_K edgeKey;
    nNo = 1;
    int iStart = 0;
    int iEnd = 0;
    int iNode1 = 0;
    int iNode2 = 0;
    T_SIMPLE_MESH_EDGE_D edge;
    for ( int k=0; k<aPlaneVCount.GetSize(); ++k )
    {
        if ( k>0 ) iStart += aPlaneVCount[k-1];

        iEnd = 0;
        for ( int m=0; m<=k; ++m )    iEnd += aPlaneVCount[m];
        iEnd = iEnd-1;

        for ( i=iStart; i<iEnd; ++i )
        {
            if ( !mNodeOld2New.Lookup(aPlaneVertexIndex[i], iNode1) )  ASSERT(0);
            iNode1 += 1;
            if ( !mNodeOld2New.Lookup(aPlaneVertexIndex[i+1], iNode2) )  ASSERT(0);
            iNode2 += 1;
            edge.Set(nNo, iNode1, iNode2);

            if ( iNode1 == iNode2 )  continue;    // 한점만 참조하는 line은 제거

            // 중복 edge 검사
            if ( bAdjustRedundency )
            {
                if ( iNode1 > iNode2 ) edgeKey.Set(iNode2, iNode1);
                else                 edgeKey.Set(iNode1, iNode2);
                if ( mEdgeKey_used.Exist(edgeKey) )
                    continue;
            }

            // edge 추가
            raEdge.Add(edge);
            nNo++;

            // 중복 edge 검사용.
            if ( bAdjustRedundency )
            {
                mEdgeKey_used.SetAt(edgeKey, 0);
            }
        }
    }
    if ( raEdge.GetSize() == 0 ) return;

    // 교차 분할 적용. ---------------------
    std::vector<T_LINE2D> vSampleLine;
    T_LINE2D line2D;
    T_POINT2D point2D_1, point2D_2;
    //int iNode1, iNode2;
    for (/*int*/ i=0; i<raEdge.GetSize(); ++i )
    {
        iNode1 = raEdge[i].iNode1 -1;
        iNode2 = raEdge[i].iNode2 -1;
        line2D.Set(raNode[iNode1].y, raNode[iNode1].z, raNode[iNode2].y, raNode[iNode2].z);

        vSampleLine.push_back(line2D);
    }

    CLineIntersector LI;
    LI.SetLines(vSampleLine);
    if ( !LI.PerformSplit() )
    {
        AfxMessageBox(_T("Error Splitting..."));
        return;
    }

    BOOL bSplit = LI.IsSplit();
    if ( bSplit )
    {
        const vector<T_LINE2D>& crvResLine = LI.GetResultLines();

        // 교차 분할 적용된 edge, node 정보로 재구성
        raEdge.RemoveAll();
        raNode.RemoveAll();

        //T_SIMPLE_MESH_NODE_D node;
        mEdgeKey_used.RemoveAll();
        for (/*int*/ i=0; i<crvResLine.size(); ++i )
        {
            node.Set(0, crvResLine[i].P1.x, crvResLine[i].P1.y);
            iNode1 = AddNode(raNode, node, dTol_merge);

            node.Set(0, crvResLine[i].P2.x, crvResLine[i].P2.y);
            iNode2 = AddNode(raNode, node, dTol_merge);

            if ( iNode1 == iNode2 )  continue;    // 한점만 참조하는 line은 제거

            // 중복 edge 검사
            if ( bAdjustRedundency )
            {
                if ( iNode1 > iNode2 ) edgeKey.Set(iNode2, iNode1);
                else                 edgeKey.Set(iNode1, iNode2);
                if ( mEdgeKey_used.Exist(edgeKey) )
                    continue;
            }

            edge.Set(raEdge.GetSize()+1, iNode1, iNode2);
            raEdge.Add(edge);

            // 중복 edge 검사용.
            if ( bAdjustRedundency )
            {
                mEdgeKey_used.SetAt(edgeKey, 0);
            }
        }
    }
}

void ConvertSectShape2EdgeSimple(CArray<int, int>& aPlaneVCount, CArray<int, int>& aPlaneVertexIndex,
    CArray<double, double>& aVertex,
    CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&>& raNode,
    CArray<T_SIMPLE_MESH_EDGE_D, T_SIMPLE_MESH_EDGE_D&>& raEdge)
{
    raNode.RemoveAll();
    raEdge.RemoveAll();

    ASSERT(aVertex.GetSize()%3 == 0);
    int nnode = aVertex.GetSize()/3;

    // set node
    int nNo = 0;
    double y = 0.0;
    double z = 0.0;
    T_SIMPLE_MESH_NODE_D node;
    int i = 0;
    for ( i=0; i<nnode; ++i )
    {
        nNo = i+1;
        //y = aVertex.GetAt(3*i+1) * (-1);  // 응력계산위치의 좌표계와 맞추기 위하여..
        y = aVertex.GetAt(3*i+1);  // 응력계산위치의 좌표계와 맞추기 위하여..
        z = aVertex.GetAt(3*i+2);
        node.Set(nNo, y, z);

        raNode.Add(node);
    }

    // 중복 node 검사.
    //BOOL bAdjustRedundency = TRUE;
    BOOL bAdjustRedundency = FALSE; // ConvertSectShape2EdgeSimple 은 중복검사를 하지 않음
    CArray<BOOL, BOOL> aRemoveFlag;    aRemoveFlag.SetSize(raNode.GetSize());
    for (/*int*/ i=0; i<aRemoveFlag.GetSize(); ++i )  aRemoveFlag[i] = FALSE;

    /* hslee : 안쓰는 node를 제거할 이유가 없다..
    // Edge에서 사용하지 않는 node는 제거
    if (bAdjustRedundency)
    {
    for (int i=0; i<aRemoveFlag.GetSize(); ++i)  aRemoveFlag[i] = TRUE;
    for (int k=0; k<aPlaneVertexIndex.GetSize(); ++k) aRemoveFlag[aPlaneVertexIndex[k]] = FALSE;
    }
    */

    CMapEx<int, int, int, int> mNodeOld2New;   // 중복 node 제거에 의해 변경되는 node 번호
    mNodeOld2New.InitHashTable(97);

    // Set Merge Tolerance
    double dY_max = -DBL_MAX;
    double dY_min =  DBL_MAX;
    double dZ_max = -DBL_MAX;
    double dZ_min =  DBL_MAX;
    for (/*int*/ i=0; i<raNode.GetSize(); ++i )
    {
        if ( raNode[i].y > dY_max )  dY_max = raNode[i].y;
        if ( raNode[i].y < dY_min )  dY_min = raNode[i].y;
        if ( raNode[i].z > dZ_max )  dZ_max = raNode[i].z;
        if ( raNode[i].z < dZ_min )  dZ_min = raNode[i].z;
    }
    double dWidth  = dY_max - dY_min;
    double dHeight = dZ_max - dZ_min;

    double dTol_merge = 1.e-8;// hslee : MergeTolerane는 충분히 작게 만드는 대신 Mesh속도개선을 했음..CAutoMesh_PlanarArea::AutoMeshGeneration()의 mapForSpeedUp변수 참고.. 아래 주석은 기존 코드임
    //       : 1.e-8로 한 이유는 TaperedSection일 때 SectDB에서 맞물리는지 판단하는 연산 시 사용하는 톨러런스랑 맞추었음. + PBeamDll에서 1.e-8기준으로 NodeMerge를 하기 때문에 그거보다 크거나 같은 값을 해야 함
    /*
    double dTol_merge = __min(dWidth, dHeight) * 2.0e-2;   // 1.0e-3 기냥... (arc를 표현한다고 해도 1/1000 정도면 충분히 표현할 수 있을 거 같아서 이렇게 잡음.)
    // 1/50으로 키움. 요소 갯수를 줄이기 위해 htlee-20131021
    // MNET:XXXX-HSSHIM-20131127: 단면 치수에 입력되어 있는 최소값이 Merge시에도 살아있게 하기 위한 예외처리.
    if (dMinMergeDim < dTol_merge && dMinMergeDim > 1.0e-4)   dTol_merge = dMinMergeDim*0.9;
    */

    for (/*int*/ i=raNode.GetSize()-1; i>=0; --i )
    {
        mNodeOld2New.SetAt(i, i);
        /*
        if (bAdjustRedundency)
        {
            for (int k=i-1; k>=0; --k)
            {
                if (raNode[i].IsSame(raNode[k], dTol_merge))
                {
                    aRemoveFlag[i] = TRUE;
                    mNodeOld2New.SetAt(i, k);
                }
            }
        }
        */
    }

    // 중복 node 삭제를 고려한 번호 조정
//   if (bAdjustRedundency)
//   {
//     CArray<int,int> aRemoveOffset;    aRemoveOffset.SetSize(raNode.GetSize());
//     int nRemoveNum = 0;
//     for (/*int*/ i=0; i<aRemoveFlag.GetSize(); ++i)
//     {
//       if (aRemoveFlag[i]) nRemoveNum++;
//       aRemoveOffset[i] = nRemoveNum;
//     }
//
//     int iOld, iNew;
//     POSITION pos = mNodeOld2New.GetStartPosition();
//     while(pos)
//     {
//       mNodeOld2New.GetNextAssoc(pos, iOld, iNew);
//       iNew -= aRemoveOffset[iNew];
//       mNodeOld2New.SetAt(iOld, iNew);
//     }
//
//     // 중복 node 삭제
//     for (/*int*/ i=raNode.GetSize()-1; i>=0; --i)
//     {
//       if (aRemoveFlag[i])   raNode.RemoveAt(i);
//     }
//
//     // 번호정보 갱신
//     for (/*int*/ i=0; i<raNode.GetSize(); ++i)  raNode[i].nNo = i+1;
//   }

    // set edge
    CMapEx<T_UINT2_INDEX_K, T_UINT2_INDEX_K&, int, int> mEdgeKey_used;    mEdgeKey_used.RemoveAll();  // 중복 edge 검사용.
    T_UINT2_INDEX_K edgeKey;
    nNo = 1;
    int iStart = 0;
    int iEnd = 0;
    int iNode1 = 0;
    int iNode2 = 0;
    T_SIMPLE_MESH_EDGE_D edge;
    for ( int k=0; k<aPlaneVCount.GetSize(); ++k )
    {
        if ( k>0 ) iStart += aPlaneVCount[k-1];

        iEnd = 0;
        for ( int m=0; m<=k; ++m )    iEnd += aPlaneVCount[m];
        iEnd = iEnd-1;

        for ( i=iStart; i<iEnd; ++i )
        {
            if ( !mNodeOld2New.Lookup(aPlaneVertexIndex[i], iNode1) )  ASSERT(0);
            iNode1 += 1;
            if ( !mNodeOld2New.Lookup(aPlaneVertexIndex[i+1], iNode2) )  ASSERT(0);
            iNode2 += 1;
            edge.Set(nNo, iNode1, iNode2);

            if ( iNode1 == iNode2 )  continue;    // 한점만 참조하는 line은 제거

            /*
            // 중복 edge 검사
            if (bAdjustRedundency)
            {
                if (iNode1 > iNode2) edgeKey.Set(iNode2, iNode1);
                else                 edgeKey.Set(iNode1, iNode2);
                if (mEdgeKey_used.Exist(edgeKey))
                    continue;
            }
            */
            // edge 추가
            raEdge.Add(edge);
            nNo++;
            /*
            // 중복 edge 검사용.
            if (bAdjustRedundency)
            {
                mEdgeKey_used.SetAt(edgeKey, 0);
            }
            */
        }
    }
}

// @dMinMergeDim : 단면 치수에 입력되어 있는 최소값으로 Merge시에도 살아있어야 되는 간격.
void ConvertSectShape2Edge(CArray<int, int>& aPlaneVCount, CArray<int, int>& aPlaneVertexIndex,
    CArray<double, double>& aVertex,
    CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&>& raNode,
    CArray<T_SIMPLE_MESH_EDGE_D, T_SIMPLE_MESH_EDGE_D&>& raEdge)
{
    raNode.RemoveAll();
    raEdge.RemoveAll();

    ASSERT(aVertex.GetSize()%3 == 0);
    int nnode = aVertex.GetSize()/3;

    // set node
    int nNo = 0;
    double y = 0.0;
    double z = 0.0;
    T_SIMPLE_MESH_NODE_D node;

    int i = 0;
    for ( i=0; i<nnode; ++i )
    {
        nNo = i+1;
        //y = aVertex.GetAt(3*i+1) * (-1);  // 응력계산위치의 좌표계와 맞추기 위하여..
        y = aVertex.GetAt(3*i+1);  // 응력계산위치의 좌표계와 맞추기 위하여..
        z = aVertex.GetAt(3*i+2);
        node.Set(nNo, y, z);

        raNode.Add(node);
    }

    // 중복 node 검사.
    BOOL bAdjustRedundency = TRUE;
    CArray<BOOL, BOOL> aRemoveFlag;    aRemoveFlag.SetSize(raNode.GetSize());
    for (/*int*/ i=0; i<aRemoveFlag.GetSize(); ++i )  aRemoveFlag[i] = FALSE;

    /* hslee : 안쓰는 node를 제거할 이유가 없다..
    // Edge에서 사용하지 않는 node는 제거
    if (bAdjustRedundency)
    {
    for (int i=0; i<aRemoveFlag.GetSize(); ++i)  aRemoveFlag[i] = TRUE;
    for (int k=0; k<aPlaneVertexIndex.GetSize(); ++k) aRemoveFlag[aPlaneVertexIndex[k]] = FALSE;
    }
    */

    CMapEx<int, int, int, int> mNodeOld2New;   // 중복 node 제거에 의해 변경되는 node 번호
    mNodeOld2New.InitHashTable(97);

    // Set Merge Tolerance
    double dY_max = -DBL_MAX;
    double dY_min =  DBL_MAX;
    double dZ_max = -DBL_MAX;
    double dZ_min =  DBL_MAX;
    for (/*int*/ i=0; i<raNode.GetSize(); ++i )
    {
        if ( raNode[i].y > dY_max )  dY_max = raNode[i].y;
        if ( raNode[i].y < dY_min )  dY_min = raNode[i].y;
        if ( raNode[i].z > dZ_max )  dZ_max = raNode[i].z;
        if ( raNode[i].z < dZ_min )  dZ_min = raNode[i].z;
    }
    double dWidth  = dY_max - dY_min;
    double dHeight = dZ_max - dZ_min;

    double dTol_merge = 1.e-8;// hslee : MergeTolerane는 충분히 작게 만드는 대신 Mesh속도개선을 했음..CAutoMesh_PlanarArea::AutoMeshGeneration()의 mapForSpeedUp변수 참고.. 아래 주석은 기존 코드임
    //       : 1.e-8로 한 이유는 TaperedSection일 때 SectDB에서 맞물리는지 판단하는 연산 시 사용하는 톨러런스랑 맞추었음. + PBeamDll에서 1.e-8기준으로 NodeMerge를 하기 때문에 그거보다 크거나 같은 값을 해야 함
    /*
    double dTol_merge = __min(dWidth, dHeight) * 2.0e-2;   // 1.0e-3 기냥... (arc를 표현한다고 해도 1/1000 정도면 충분히 표현할 수 있을 거 같아서 이렇게 잡음.)
    // 1/50으로 키움. 요소 갯수를 줄이기 위해 htlee-20131021
    // MNET:XXXX-HSSHIM-20131127: 단면 치수에 입력되어 있는 최소값이 Merge시에도 살아있게 하기 위한 예외처리.
    if (dMinMergeDim < dTol_merge && dMinMergeDim > 1.0e-4)   dTol_merge = dMinMergeDim*0.9;
    */

    for (/*int*/ i=raNode.GetSize()-1; i>=0; --i )
    {
        mNodeOld2New.SetAt(i, i);

        if ( bAdjustRedundency )
        {
            for ( int k=i-1; k>=0; --k )
            {
                if ( raNode[i].IsSame(raNode[k], dTol_merge) )
                {
                    aRemoveFlag[i] = TRUE;
                    mNodeOld2New.SetAt(i, k);
                }
            }
        }
    }

    // 중복 node 삭제를 고려한 번호 조정
    if ( bAdjustRedundency )
    {
        CArray<int, int> aRemoveOffset;    aRemoveOffset.SetSize(raNode.GetSize());
        int nRemoveNum = 0;
        for (/*int*/ i=0; i<aRemoveFlag.GetSize(); ++i )
        {
            if ( aRemoveFlag[i] ) nRemoveNum++;
            aRemoveOffset[i] = nRemoveNum;
        }

        int iOld, iNew;
        POSITION pos = mNodeOld2New.GetStartPosition();
        while ( pos )
        {
            mNodeOld2New.GetNextAssoc(pos, iOld, iNew);
            iNew -= aRemoveOffset[iNew];
            mNodeOld2New.SetAt(iOld, iNew);
        }

        // 중복 node 삭제
        for (/*int*/ i=raNode.GetSize()-1; i>=0; --i )
        {
            if ( aRemoveFlag[i] )   raNode.RemoveAt(i);
        }

        // 번호정보 갱신
        for (/*int*/ i=0; i<raNode.GetSize(); ++i )  raNode[i].nNo = i+1;
    }

    // set edge
    CMapEx<T_UINT2_INDEX_K, T_UINT2_INDEX_K&, int, int> mEdgeKey_used;    mEdgeKey_used.RemoveAll();  // 중복 edge 검사용.
    T_UINT2_INDEX_K edgeKey;
    nNo = 1;
    int iStart = 0;
    int iEnd = 0;
    int iNode1 = 0;
    int iNode2 = 0;
    T_SIMPLE_MESH_EDGE_D edge;
    for ( int k=0; k<aPlaneVCount.GetSize(); ++k )
    {
        if ( k>0 ) iStart += aPlaneVCount[k-1];

        iEnd = 0;
        for ( int m=0; m<=k; ++m )    iEnd += aPlaneVCount[m];
        iEnd = iEnd-1;

        for ( i=iStart; i<iEnd; ++i )
        {
            if ( !mNodeOld2New.Lookup(aPlaneVertexIndex[i], iNode1) )  ASSERT(0);
            iNode1 += 1;
            if ( !mNodeOld2New.Lookup(aPlaneVertexIndex[i+1], iNode2) )  ASSERT(0);
            iNode2 += 1;
            edge.Set(nNo, iNode1, iNode2);

            if ( iNode1 == iNode2 )  continue;    // 한점만 참조하는 line은 제거

            // 중복 edge 검사
            if ( bAdjustRedundency )
            {
                if ( iNode1 > iNode2 ) edgeKey.Set(iNode2, iNode1);
                else                 edgeKey.Set(iNode1, iNode2);
                if ( mEdgeKey_used.Exist(edgeKey) )
                    continue;
            }

            // edge 추가
            raEdge.Add(edge);
            nNo++;

            // 중복 edge 검사용.
            if ( bAdjustRedundency )
            {
                mEdgeKey_used.SetAt(edgeKey, 0);
            }
        }
    }
    if ( raEdge.GetSize() == 0 ) return;

    // 교차 분할 적용. ---------------------
    std::vector<T_LINE2D> vSampleLine;
    T_LINE2D line2D;
    T_POINT2D point2D_1, point2D_2;
    //int iNode1, iNode2;
    for (/*int*/ i=0; i<raEdge.GetSize(); ++i )
    {
        iNode1 = raEdge[i].iNode1 -1;
        iNode2 = raEdge[i].iNode2 -1;
        line2D.Set(raNode[iNode1].y, raNode[iNode1].z, raNode[iNode2].y, raNode[iNode2].z);

        vSampleLine.push_back(line2D);
    }

    CLineIntersector LI;
    LI.SetLines(vSampleLine);
    if ( !LI.PerformSplit() )
    {
        AfxMessageBox(_T("Error Splitting..."));
        return;
    }

    BOOL bSplit = LI.IsSplit();
    if ( bSplit )
    {
        const vector<T_LINE2D>& crvResLine = LI.GetResultLines();

        // 교차 분할 적용된 edge, node 정보로 재구성
        raEdge.RemoveAll();
        raNode.RemoveAll();

        //T_SIMPLE_MESH_NODE_D node;
        mEdgeKey_used.RemoveAll();
        for (/*int*/ i=0; i<crvResLine.size(); ++i )
        {
            node.Set(0, crvResLine[i].P1.x, crvResLine[i].P1.y);
            iNode1 = AddNode(raNode, node, dTol_merge);

            node.Set(0, crvResLine[i].P2.x, crvResLine[i].P2.y);
            iNode2 = AddNode(raNode, node, dTol_merge);

            if ( iNode1 == iNode2 )  continue;    // 한점만 참조하는 line은 제거

            // 중복 edge 검사
            if ( bAdjustRedundency )
            {
                if ( iNode1 > iNode2 ) edgeKey.Set(iNode2, iNode1);
                else                 edgeKey.Set(iNode1, iNode2);
                if ( mEdgeKey_used.Exist(edgeKey) )
                    continue;
            }

            edge.Set(raEdge.GetSize()+1, iNode1, iNode2);
            raEdge.Add(edge);

            // 중복 edge 검사용.
            if ( bAdjustRedundency )
            {
                mEdgeKey_used.SetAt(edgeKey, 0);
            }
        }
    }
}

void WriteMctOfSect(CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&>& aNode,
    CArray<T_SIMPLE_MESH_EDGE_D, T_SIMPLE_MESH_EDGE_D&>& aEdge,
    LPCTSTR lpszPostFix)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    // 데이터 확인용 mct 생성
    //     *NODE    ; Nodes
    //     ; iNO, X, Y, Z
    //     1, 0.5, 3.5, 0
    //
    //     *ELEMENT    ; Elements
    //     ; iEL, TYPE, iMAT, iPRO, iN1, iN2, ANGLE, iSUB, EXVAL, iOPT(EXVAL2) ; Frame  Element
    //     ; iEL, TYPE, iMAT, iPRO, iN1, iN2, iN3, iN4, iSUB, iWID             ; Planar Element
    //     1, BEAM  ,    1,     1,     1,     2,     0

    CString strModelPath=_T("");
    strModelPath = pDoc->GetPathName();
    CString strMctPath = _T("");

    if ( !strModelPath.IsEmpty() )
    {
        CFileCtrl FileCtrl(strModelPath);
        CString csFolder=_T("");
        csFolder = FileCtrl.GetFilePathAndNameWithoutExtension();
        strMctPath = csFolder + _T("_shape") + lpszPostFix + (CProduct::IsRusLocal() ? _T(".nbt") : _T(".mct"));
    }
    else { ASSERT(0);  return; }
    CDebugFile log(strMctPath);
    log.Clear();

    // *NODE ------------------------
    log.Write(_T("*NODE\n"));
    for ( int i=0; i<aNode.GetSize(); ++i )
    {
        log.Write(_T("%d, 0.0, %g, %g\n"), aNode[i].nNo, aNode[i].y, aNode[i].z);
    }
    log.Write(_T("\n"));

    // *ELEMENT ------------------------
    log.Write(_T("*ELEMENT\n"));
    for ( int k=0; k<aEdge.GetSize(); ++k )
    {
        log.Write(_T("%d, BEAM, 1, 1, %d, %d, 0\n"), aEdge[k].nNo, aEdge[k].iNode1, aEdge[k].iNode2);
    }
    log.Write(_T("\n"));
}

BOOL IsUseStiffBySectBefore(const T_SECT_D& SectD)
{
    if ( SectD.nStype == D_SECT_TYPE_COMBINED ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_SRC ) return TRUE;

    if ( SectD.nStype == D_SECT_TYPE_COMPO_B ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_COMPO_I ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_COMPO_TUB ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_COMPO_STLG_B ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_COMPO_STLG_I ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_COMPO_STLG_TUB ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_COMPO_CI ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_COMPO_CT ) return TRUE;
    if ( SectD.nStype == D_SECT_TYPE_COMPO_PC ) return TRUE;

    return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double CDb_DllBaseManager::m_dMeshSizeRatio=20.0;
BOOL   CDb_DllBaseManager::m_bAllSectMesh=FALSE;
CDb_DllBaseManager::CDb_DllBaseManager()
{
    m_bUseMaxSectMesh = FALSE;
}

CDb_DllBaseManager::~CDb_DllBaseManager()
{
    m_bUseMaxSectMesh = FALSE;
}

BOOL CDb_DllBaseManager::Calc_SectRegular(CString strShape, double dSize[8], DGN_SECT_STIFFNESS& rStiffData)
{
    HINSTANCE hDllCalc = GetOrLoad_BaseManagerDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    //==================================================================================================
    // Explicit Linkage for DLL
    typedef BOOL(PDLLFUNC)( CStringW, double[8], void*, size_t, BOOL& );
    PDLLFUNC *pDllfunc;
    pDllfunc = ( PDLLFUNC* ) GetProcAddress(hDllCalc, "Calc_SectRegular");
    //==================================================================================================
    size_t Tsize_StiffData= sizeof(DGN_SECT_STIFFNESS);
    CStringW strWShape(strShape);
    BOOL bVersionOK = FALSE;
    BOOL bChk = ( *pDllfunc )( strWShape, dSize, &rStiffData, Tsize_StiffData, bVersionOK );

    if ( !bVersionOK )
    {
        AfxMessageBox(_T("Conversion of Design data failed"));
        return FALSE;
    }

    return bChk;
}

BOOL CDb_DllBaseManager::Get_SectionPolygonData(CString strShape, double dSize[8], DGN_GSEC_POLYGON_LIST& arOutPolyData, DGN_GSEC_POLYGON_LIST& arInPolyData, double dPos[6], double dOffsetX, double dOffsetY, BOOL bRev)
{
    HINSTANCE hDllCalc = GetOrLoad_BaseManagerDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    //==================================================================================================
    // Explicit Linkage for DLL
    typedef BOOL(PDLLFUNC)( CStringW, double[8], void*, void*, double[6], double, double, BOOL, size_t, BOOL& );
    PDLLFUNC *pDllfunc;
    pDllfunc = ( PDLLFUNC* ) GetProcAddress(hDllCalc, "Get_SectionPolygonData");
    //==================================================================================================
    size_t Tsize_Polylist= sizeof(DGN_GSEC_POLYGON_LIST);
    CStringW strWShape(strShape);
    BOOL bVersionOK = FALSE;
    BOOL bChk = ( *pDllfunc )( strWShape, dSize, &arOutPolyData, &arInPolyData, dPos, dOffsetX, dOffsetX, bRev, Tsize_Polylist, bVersionOK );

    if ( !bVersionOK )
    {
        AfxMessageBox(_T("Conversion of Design data failed"));
        return FALSE;
    }

    return bChk;
}

BOOL CSimpleMesh::RenumberNodeByOffset(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
    int nCountNode_offset)
{
    ASSERT(nCountNode_offset > 0);

    for ( int i=raMeshNodeD.GetSize()-1; i>=0; --i )
    {
        raMeshNodeD[i].nNo += nCountNode_offset;
    }

    for ( int i=raMeshElemD.GetSize()-1; i>=0; --i )
    {
        ASSERT(raMeshElemD[i].aNodeK.GetSize() == 3);

        raMeshElemD[i].aNodeK[0] += nCountNode_offset;
        raMeshElemD[i].aNodeK[1] += nCountNode_offset;
        raMeshElemD[i].aNodeK[2] += nCountNode_offset;
    }

    return TRUE;
}

// part별 속성 계산시, 좌표를 도심 기준 좌표로 변경.
// (Composite Sect for CS 단면 속성 계산시 이전에 이렇게 하고 있어서 기존 방식을 맞춰주기 위함.)
// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CSimpleMesh::AdjustCoordToCentroid(int nTypeBeforeAfter, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
    T_SECT_DESIGN* pSectDesign/*=NULL*/)
{
    BOOL bNeedPartStiff = FALSE;
    if ( nTypeBeforeAfter != 0 ) bNeedPartStiff = TRUE;
    //if (nTypeBeforeAfter ==  1)   bNeedPartStiff = TRUE;
    //if (nTypeBeforeAfter ==  2)   bNeedPartStiff = TRUE;
    //if (nTypeBeforeAfter == 12)   bNeedPartStiff = TRUE;

    // 파트별 계산시에만 적용.
    // MNET:XXXX-HSSHIM-20130816
    // 기존 방식에서 Slab part 에서만 도심 기준으로 확산한 좌표를 사용했으므로 그대로 적용.
    if ( !bNeedPartStiff )  return TRUE;

    // calc Centroid (약식으로 중간점을 계산. Slab는 직사각형...^^)
    double dX_max = -DBL_MAX;
    double dX_min =  DBL_MAX;
    double dY_max = -DBL_MAX;
    double dY_min =  DBL_MAX;
    double dZ_max = -DBL_MAX;
    double dZ_min =  DBL_MAX;
    for ( int i=0; i<raMeshNodeD.GetSize(); ++i )
    {
        if ( raMeshNodeD[i].x > dX_max )  dX_max = raMeshNodeD[i].x;
        if ( raMeshNodeD[i].x < dX_min )  dX_min = raMeshNodeD[i].x;
        if ( raMeshNodeD[i].y > dY_max )  dY_max = raMeshNodeD[i].y;
        if ( raMeshNodeD[i].y < dY_min )  dY_min = raMeshNodeD[i].y;
        if ( raMeshNodeD[i].z > dZ_max )  dZ_max = raMeshNodeD[i].z;
        if ( raMeshNodeD[i].z < dZ_min )  dZ_min = raMeshNodeD[i].z;
    }

    //double dX_off = (dX_max + dX_min) * 0.5;
    double dY_off = ( dY_max + dY_min ) * 0.5;
    double dZ_off = ( dZ_max + dZ_min ) * 0.5;

    /* 한번 지워보자
    if (pSectDesign)   // 시공단계 전이고 Centroid를 알고 있으면,
    {
    //if (nTypeBeforeAfter ==  1 ||
    //  nTypeBeforeAfter ==  2 )
    //{
    //double dCentX = pSectDesign->YBar;
    double dCentY = pSectDesign->ZBar;
    //dX_off = dX_min + dCentX;
    dY_off = dY_min + dCentY;
    dZ_off = dZ_min + 0.0;
    //}
    }
    */
    // adjust by offset
    for ( int i=0; i<raMeshNodeD.GetSize(); ++i )
    {
        //raMeshNodeD[i].x -= dX_off;   // MNET:XXXX-HSSHIM-20130820. 시공단계합성단면의 도심이 Slab 단면을 고려하고 있지 않아서, X방향은 우선 제외시킴.
        raMeshNodeD[i].y -= dY_off;
        raMeshNodeD[i].z -= dZ_off;
    }

    return TRUE;
}

BOOL CSimpleMesh::GetLineInterSectPoint(T_POINT3D& P1, T_POINT3D& P2, T_POINT3D& P3, T_POINT3D& P4, T_POINT3D& IntersectPoint, double dTol)
{
    //선분의 충돌점 계산
    double A, B, C, D;
    double T, V;
    double DivT, DivV;

    A = P4.x - P3.x;
    B = P2.x - P1.x;
    C = P4.y - P3.y;
    D = P2.y - P1.y;

    DivT = A*D - B*C;
    DivV = B*C - A*D;

    if ( DivT != 0.0 && DivV != 0.0 )
    {
        T = ( A*P3.y + C*P1.x - C*P3.x - A*P1.y ) / DivT;
        V = ( B*P1.y + D*P3.x - B*P3.y - D*P1.x ) / DivV;

        if ( ( T > -dTol && T < 1.0 + dTol ) &&
            ( V > -dTol && V < 1.0 + dTol ) )
        {
            IntersectPoint.x = P1.x + B * T;
            IntersectPoint.y = P1.y + D * T;
            IntersectPoint.z = 0.0;

            return TRUE;
        }
    }
    IntersectPoint.Set(0, 0, 0);
    return FALSE;
}

void CSimpleMesh::MovePt(T_POINT3D& posOri, double aDir[3], double dMoveLen, T_POINT3D& posMoved)
{
    posMoved.x = posOri.x + dMoveLen * aDir[0];
    posMoved.y = posOri.y + dMoveLen * aDir[1];
    posMoved.z = posOri.z + dMoveLen * aDir[2];
}

void CSimpleMesh::GetDir(T_POINT3D& posA, T_POINT3D& posB, double dDir[3])
{
    dDir[0] = posB.x - posA.x;
    dDir[1] = posB.y - posA.y;
    dDir[2] = posB.z - posA.z;
    CMathFunc::mathNormalize(dDir[0], dDir[1], dDir[2], dDir[0], dDir[1], dDir[2]);
}

void CSimpleMesh::AppendDivPnt(T_LINE_FOR_MESH_G& Edge, double dSize, BOOL bIncludeEachEnd, CArray<T_POINT3D, T_POINT3D&>& aPntDiv)
{
    int nDiv = ( int ) ( Edge.GetLineLength() / dSize );
    dSize = Edge.GetLineLength() / ( double ) nDiv; // 버림 처리

    if ( bIncludeEachEnd )
    {
        aPntDiv.Add(Edge.p1);
    }

    double dDir[3];
    GetDir(Edge.p1, Edge.p2, dDir);
    T_POINT3D EachPntDiv;
    for ( int i = 1; i < nDiv; i++ )
    {
        MovePt(Edge.p1, dDir, i * dSize, EachPntDiv);
        aPntDiv.Add(EachPntDiv);
    }

    if ( bIncludeEachEnd )
    {
        aPntDiv.Add(Edge.p2);
    }
}

BOOL CSimpleMesh::IsLineOverlap(T_LINE_FOR_MESH_G& Line1, T_LINE_FOR_MESH_G& Line2)
{
    double dTol = 1.0e-08; // 1.0e-08정도를 주면 1.0e-06 정도의 톨러런스로 계산된다

    double dAng1 = CMathFunc::GetAngle(Line1.p1.x, Line1.p1.y, Line1.p2.x, Line1.p2.y, TRUE/*bPosAngle*/, FALSE/*bChkLength*/);
    double dAng2 = CMathFunc::GetAngle(Line2.p1.x, Line2.p1.y, Line2.p2.x, Line2.p2.y, TRUE/*bPosAngle*/, FALSE/*bChkLength*/);

    if ( dAng1 >= 180.0 )
    {
        dAng1 -= 180.0;
    }
    if ( dAng2 >= 180.0 )
    {
        dAng2 -= 180.0;
    }

    if ( fabs(dAng1 - dAng2) > 1.0e-04 )
    {
        return FALSE;
    }

    if ( is_on_line_2d(Line1.p1.x, Line1.p1.y, Line1.p2.x, Line1.p2.y, Line2.p1.x, Line2.p1.y, dTol) )
    {
        return TRUE;
    }
    if ( is_on_line_2d(Line1.p1.x, Line1.p1.y, Line1.p2.x, Line1.p2.y, Line2.p2.x, Line2.p2.y, dTol) )
    {
        return TRUE;
    }
    if ( is_on_line_2d(Line2.p1.x, Line2.p1.y, Line2.p2.x, Line2.p2.y, Line1.p1.x, Line1.p1.y, dTol) )
    {
        return TRUE;
    }
    if ( is_on_line_2d(Line2.p1.x, Line2.p1.y, Line2.p2.x, Line2.p2.y, Line1.p2.x, Line1.p2.y, dTol) )
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CSimpleMesh::MakeShareEdgeWhenComp(int nTypeBeforeAfter, T_SECT_D &SectD, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, BOOL bShareWidthSlab)
{
    // SectTest
    if ( CSectDB::IsSectSteelComposite(SectD) && nTypeBeforeAfter==1 )
    {
        bShareWidthSlab = FALSE;

        // Gap 제외
        CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdge01;
        int nEdge = aaEdge.GetSize();
        if ( IsCompositeSlabGapMesh(SectD) )
        {
            for ( int i=0; i<nEdge-1; i++ ) aaEdge01.Add(aaEdge[i]);
            aaEdge.RemoveAll();
            aaEdge.Copy(aaEdge01);
        }
    }
    else if (CSectDB::IsSectDumbAndStub(SectD))
    {
        return TRUE;       
    }
    else if ( CDb_DllBaseManager::m_bAllSectMesh )
    {
        return MakeShareEdgeWhenCompNew(nTypeBeforeAfter, SectD, aaEdge, bShareWidthSlab);
    }
    // Composite 단면일 때 Girder,Slab,Stiffner가 절점을 공유하도록 Edge를 쪼갬
    // 이 함수를 통과하면 같은 위치에 절점이 생기김.. 나중에 좌표가 같은 Node를 Merge시키면 절점공유가 됨

    if ( aaEdge.GetSize() <= 1 )
    {
        ASSERT(0); // 이 함수는 Composite단면일 때만 부르세요.
        return FALSE;
    }

    // aaEdge[0].aAr의 size가 0개면 Steel Girder 단면임

    struct KeyForEdgeMod : public StructKeyForStdMap
    {
        KeyForEdgeMod() : StructKeyForStdMap()
        {
        }
        KeyForEdgeMod(int nIIdx, int nJIdx) : StructKeyForStdMap()
        {
            aInt.push_back(nIIdx);
            aInt.push_back(nJIdx);
        }
    };

    struct AddPntInfo
    {
        T_POINT3D Pnt;
        std::vector<T_POINT3D> aPntWithSameEdge;  // AddPntInfo::Pnt생성 당시 동일한 Edge에서 생성됬던 Point들 저장.. 공유하는 Edge는 이 함수에서 쪼깬뒤.. CM2에서는 나누지 않도록 ratio를 1로 세팅하는데.. 그 외 지점은 CM2에서 쪼개게 하기 위해서 이 배열이 필요 함
        AddPntInfo& operator=(const AddPntInfo& src)
        {
            Pnt = src.Pnt;
            aPntWithSameEdge.clear();
            for ( int i = 0; i < src.aPntWithSameEdge.size(); i++ )
            {
                aPntWithSameEdge.push_back(src.aPntWithSameEdge[i]);
            }
            return *this;
        }
        void Init()
        {
            Pnt.Set(0.0, 0.0, 0.0);
            aPntWithSameEdge.clear();
        }
    };
    struct PosForDirSort
    {
        AddPntInfo PosInfo;
        double dSortDir[2];
        bool operator<(const PosForDirSort &rhs)  const
        {
            ASSERT(dSortDir[0] == rhs.dSortDir[0] && dSortDir[1] == rhs.dSortDir[1]);

            double dPos1DCur, dPos1DRhs; // 1차원 직선으로 투영한 위치

            dPos1DCur = PosInfo.Pnt.x * dSortDir[0] + PosInfo.Pnt.y * dSortDir[1];
            dPos1DRhs = rhs.PosInfo.Pnt.x * dSortDir[0] + rhs.PosInfo.Pnt.y * dSortDir[1];

            return dPos1DCur < dPos1DRhs;
        }
    };
    struct ValForEdgeMod
    {
        ValForEdgeMod()
        {
        }
        ValForEdgeMod& operator=(const ValForEdgeMod& src)
        {
            aAddPnt.clear();
            aAddPnt.resize(src.aAddPnt.size());
            for ( int i = 0; i < aAddPnt.size(); i++ )
            {
                aAddPnt[i] = src.aAddPnt[i];
            }

            return *this;
        }

        std::vector<AddPntInfo> aAddPnt;
    };

    std::map<KeyForEdgeMod, ValForEdgeMod> mapEdgeMod;

    double dSlabDivSize = DBL_MAX;

    int nSlabIdx;

    int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    nSlabIdx = 0;
    std::set<int> setGirderIdx;
    if ( nStype == D_SECT_TYPE_STLG_MCELL )
    {
        for ( int i = 1; i < aaEdge.GetSize(); i++ )
        {
            setGirderIdx.insert(i);//MCell 단면은 각 조각(2DShape기준)을 따로 Mesh 한 다음 마지막에 merge 한다.
        }
    }
    else if ( IsBoxGirderShape(nStype) )
    {
        for ( int i = 1; i <= GetBoxGirderShapeNum(nStype); i++ )
        {
            setGirderIdx.insert(i); // Box형상의 단면은 각 조각(2DShape기준)을 따로 Mesh 한 다음 마지막에 merge 한다... Stiffner와 Girder Size를 다르게 하기 위해서 MCell과 다른 처리를 추가로 함
        }
    }
    else
    {
        setGirderIdx.insert(1);
    }

    double dWholeMaxSize = -DBL_MAX;

    CArray<double, double&> aMinSize;
    CArray<double, double&> aMaxSize;
    aMinSize.SetSize(aaEdge.GetSize());
    aMaxSize.SetSize(aaEdge.GetSize());

    int nCompShapeType;
    for ( int i = 0; i < aMinSize.GetSize(); i++ )
    {
        if ( i == nSlabIdx )
        {
            nCompShapeType = 0; // Slab
        }
        else if ( setGirderIdx.find(i) != setGirderIdx.end() )
        {
            nCompShapeType = 1; // Girder
        }
        else
        {
            nCompShapeType = 2; // Stiffener
        }

        if ( aaEdge[i].aAr.GetSize() == 0 )
            continue;  // Steel Girder 단면의 Slab

        GetMinMaxAutoMeshSize(SectD, aaEdge[i].aAr, aMinSize[i], aMaxSize[i], nCompShapeType, nTypeBeforeAfter, nStype == D_SECT_TYPE_STLG_MCELL || IsBoxGirderShape(nStype)); // EdgeSize 계산할 때 필요함
        if ( dWholeMaxSize < aMaxSize[i] )
        {
            dWholeMaxSize = aMaxSize[i];
        }
    }

    const double dMergeTol = M_InitValueCurUnit(0.3, KN, MM, D_UNITSYS_BASE_LENGTH);     // 한번 걸러놓은 데이터를 비교할때 쓰는 톨러런스기 때문에 넉넉하게 함

    // 추가해야 할 점 찾기
    for ( int nGirderIdx : setGirderIdx )
    {
        for ( int i = 0; i < aaEdge.GetSize(); i++ )
        {
            if ( i == nGirderIdx ) continue;

            if ( i == nSlabIdx && !bShareWidthSlab ) continue;

            if ( aaEdge[i].aAr.GetSize() == 0 )
                continue;  // Steel Girder 단면의 Slab

            BOOL bStiffener;
            if ( nStype == D_SECT_TYPE_STLG_MCELL )
            {
                bStiffener = FALSE;
            }
            else if ( IsBoxGirderShape(nStype) )
            {
                bStiffener = i > GetBoxGirderShapeNum(nStype);
            }
            else
            {
                bStiffener = i >= 2;
            }

            std::vector<int> aIntsGirderIdx;
            std::vector<int> aIntsLoopIdx;

            int nIntsCnt = 0;

            CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeGirder = aaEdge[nGirderIdx].aAr;
            CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeLoop = aaEdge[i].aAr;
            for ( int j = 0; j < aEdgeGirder.GetSize(); j++ )
            {
                for ( int k = 0; k < aEdgeLoop.GetSize(); k++ )
                {
                    BOOL bOverlap = IsLineOverlap(aEdgeGirder[j], aEdgeLoop[k]); // 두개의 선분이 겹치는지

                    if ( bOverlap )
                    {
                        aIntsGirderIdx.push_back(j);
                        aIntsLoopIdx.push_back(k);
                        nIntsCnt++;
                    }
                }
            }

            if ( nIntsCnt >= 1 )
            {
                for ( int ii = 0; ii < aIntsGirderIdx.size(); ii++ )
                {
                    int nIntsGirderIdx = aIntsGirderIdx[ii];
                    int nIntsLoopIdx = aIntsLoopIdx[ii];

                    double dMeshDivRatioGirder = GetMeshSizeRatio(aaEdge[nGirderIdx].aAr, nIntsGirderIdx, aMaxSize[nGirderIdx]);
                    double dMeshDivRatioLoop = GetMeshSizeRatio(aaEdge[i].aAr, nIntsLoopIdx, aMaxSize[i]);

                    double dTest01 = aaEdge[nGirderIdx].aAr[nIntsGirderIdx].GetLineLength();
                    double dTest02 = aaEdge[i].aAr[nIntsLoopIdx].GetLineLength();

                    double dGirderMeshSize = aaEdge[nGirderIdx].aAr[nIntsGirderIdx].GetLineLength() * ( 1 / ( double ) ( ( int ) ( 1 / dMeshDivRatioGirder ) ) ); // 단위변환 부분은 0.52 같은 놈은 2등분인 0.5가 되도록 버림 처리 하는 것임
                    double dIMeshSize = aaEdge[i].aAr[nIntsLoopIdx].GetLineLength() * ( 1 / ( double ) ( ( int ) ( 1 / dMeshDivRatioLoop ) ) ); // 단위변환 부분은 0.52 같은 놈은 2등분인 0.5가 되도록 버림 처리 하는 것임

                    CArray<T_POINT3D, T_POINT3D&> aAddPnt;

                    aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p1);
                    aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p2);
                    aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p1);
                    aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p2);

                    if ( bStiffener )
                    {
                        AppendDivPnt(aaEdge[i].aAr[nIntsLoopIdx], dIMeshSize, FALSE, aAddPnt); // Stiffener는 Girder의 한 면에 접하므로 Stiffener 기준으로 Mesh를 만들어 공유함
                    }
                    else if ( dGirderMeshSize < dIMeshSize ) // Girder와Slab를 비교할 때는 더 짧게 쪼개지는 쪽 기준으로...
                    {
                        AppendDivPnt(aaEdge[nGirderIdx].aAr[nIntsGirderIdx], dGirderMeshSize, FALSE, aAddPnt);
                    }
                    else
                    {
                        AppendDivPnt(aaEdge[i].aAr[nIntsLoopIdx], dIMeshSize, FALSE, aAddPnt);
                    }

                    if ( i == nSlabIdx )
                    {
                        double dMeshSize = min(dGirderMeshSize, dIMeshSize);
                        dSlabDivSize = dMeshSize;
                    }

                    T_LINE_FOR_MESH_G& GirderEdge = aaEdge[nGirderIdx].aAr[nIntsGirderIdx];
                    std::vector<T_POINT3D> aDifPntWithGirderEdge;
                    for ( int jj = 0; jj < aAddPnt.GetSize(); jj++ )
                    {
                        if ( is_on_line_2d(GirderEdge.p1.x, GirderEdge.p1.y, GirderEdge.p2.x, GirderEdge.p2.y, aAddPnt[jj].x, aAddPnt[jj].y, dMergeTol) )
                        {
                            if ( !aAddPnt[jj].IsSame(GirderEdge.p1, 1.e-8) && !aAddPnt[jj].IsSame(GirderEdge.p2, 1.e-8) )
                            {
                                aDifPntWithGirderEdge.push_back(aAddPnt[jj]);
                            }
                        }
                    }
                    if ( aDifPntWithGirderEdge.size() > 0 )
                    {
                        KeyForEdgeMod Key(nGirderIdx, nIntsGirderIdx);
                        auto itrFind = mapEdgeMod.find(Key);
                        if ( itrFind == mapEdgeMod.end() )
                        {
                            mapEdgeMod[Key] = ValForEdgeMod();
                            itrFind = mapEdgeMod.find(Key);;
                        }

                        AddPntInfo PntInfo;
                        for ( int jj = 0; jj < aDifPntWithGirderEdge.size(); jj++ )
                        {
                            PntInfo.Init();
                            PntInfo.Pnt = aDifPntWithGirderEdge[jj];
                            for ( int kk = 2; kk < aAddPnt.GetSize(); kk++ ) // 2부터 시작하는 이유는 Girder의 Edge를 Skip하기 위함임
                            {
                                if ( !PntInfo.Pnt.IsSame(aAddPnt[kk], 1.e-8) )
                                {
                                    PntInfo.aPntWithSameEdge.push_back(aAddPnt[kk]);
                                }
                            }
                            ValForEdgeMod& Val = itrFind->second;
                            Val.aAddPnt.push_back(PntInfo);
                        }
                    }
                }
            }
        }
    }

    if ( mapEdgeMod.size() == 0 )
    {
        return TRUE;
    }
    /* 이 주석을 풀면 Slab의 모든 Edge를 dSlabDivSize로 쪼갬
    if(dSlabDivSize != DBL_MAX)
    {
    // Slab의 아랫 Edge가 쪼개졌을 경우에 여기로 들어옴..
    // Slab의 아랫 Edge만 잘게 쪼개면 모양이 안이쁠 수 있기 때문에 모든 Edge를 동일한 Size로 쪼갬
    for (int j = 0; j < aaEdge[nSlabIdx].aAr.GetSize(); j++)
    {
    KeyForEdgeMod Key(nSlabIdx, j);
    auto itrFind = mapEdgeMod.find(Key);
    if(itrFind == mapEdgeMod.end())
    {
    mapEdgeMod[Key] = ValForEdgeMod();
    itrFind = mapEdgeMod.find(Key);;

    CArray<T_POINT3D, T_POINT3D&> aAddPnt;
    GetDivPnt(aaEdge[nSlabIdx].aAr[j], dSlabDivSize, FALSE, aAddPnt);
    for (int k = 0; k < aAddPnt.GetSize(); k++)
    {
    itrFind->second.aAddPnt.push_back(aAddPnt[k]);
    }
    }
    }
    }*/

    // 추가점 sorting
    std::vector<PosForDirSort> aPosForSort;
    PosForDirSort EachPntForSort;
    double dDir[3];
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            auto itrFind = mapEdgeMod.find(KeyForEdgeMod(i, j));
            if ( itrFind != mapEdgeMod.end() )
            {
                GetDir(aaEdge[i].aAr[j].p1, aaEdge[i].aAr[j].p2, dDir);

                std::vector<AddPntInfo>& aAddPntInfo = itrFind->second.aAddPnt;

                aPosForSort.clear();
                for ( int k = 0; k < aAddPntInfo.size(); k++ )
                {
                    EachPntForSort.PosInfo = aAddPntInfo[k];
                    EachPntForSort.dSortDir[0] = dDir[0];
                    EachPntForSort.dSortDir[1] = dDir[1];
                    aPosForSort.push_back(EachPntForSort);
                }
                std::sort(aPosForSort.begin(), aPosForSort.end()); // dDir 방향으로 Sorting

                for ( int k = 0; k < aAddPntInfo.size(); k++ )
                {
                    aAddPntInfo[k] = aPosForSort[k].PosInfo;
                }
            }
        }
    }

    // 추가점 적용
    T_LINE_FOR_MESH_G EdgeTemp;
    CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdgeMod;
    BOOL bNoDivWhenMesh = FALSE;
    aaEdgeMod.SetSize(aaEdge.GetSize());
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            auto itrFind = mapEdgeMod.find(KeyForEdgeMod(i, j));
            if ( itrFind != mapEdgeMod.end() )
            {
                std::vector<AddPntInfo>& FindVal = itrFind->second.aAddPnt;
                EdgeTemp.p1 = aaEdge[i].aAr[j].p1;
                for ( int k = 0; k < FindVal.size(); k++ )
                {
                    EdgeTemp.p2 = FindVal[k].Pnt;
                    bNoDivWhenMesh = FALSE;
                    for ( int ii = 0; ii < FindVal[k].aPntWithSameEdge.size(); ii++ )
                    {
                        if ( EdgeTemp.p1.IsSame(FindVal[k].aPntWithSameEdge[ii], 1.e-8) )
                        {
                            bNoDivWhenMesh = TRUE;
                        }
                    }
                    EdgeTemp.bNoDivWhenMesh = bNoDivWhenMesh;
                    aaEdgeMod[i].aAr.Add(EdgeTemp);
                    EdgeTemp.p1 = FindVal[k].Pnt;
                }
                EdgeTemp.p2 = aaEdge[i].aAr[j].p2;
                bNoDivWhenMesh = FALSE;
                for ( int ii = 0; ii < FindVal[FindVal.size() - 1].aPntWithSameEdge.size(); ii++ )
                {
                    if ( EdgeTemp.p1.IsSame(FindVal[FindVal.size() - 1].aPntWithSameEdge[ii], 1.e-8) )
                    {
                        bNoDivWhenMesh = TRUE;
                    }
                }
                EdgeTemp.bNoDivWhenMesh = bNoDivWhenMesh;
                aaEdgeMod[i].aAr.Add(EdgeTemp);
            }
            else
            {
                aaEdgeMod[i].aAr.Add(aaEdge[i].aAr[j]);
                aaEdgeMod[i].aAr[aaEdgeMod[i].aAr.GetSize() - 1].bNoDivWhenMesh = FALSE;
            }
        }
    }

    ASSERT(aaEdge.GetSize() == aaEdgeMod.GetSize());

    const double dTol = 1.e-6;

    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        aaEdge[i].aAr.RemoveAll();
        for ( int j = 0; j < aaEdgeMod[i].aAr.GetSize(); j++ )
        {
            if ( abs(aaEdgeMod[i].aAr[j].GetLineLength()) < dTol )
            {
                continue;
            }

            aaEdge[i].aAr.Add(aaEdgeMod[i].aAr[j]);
        }
    }

    // 절점 Merge
    BOOL bDoNewAlgorithm = FALSE;
    if ( nStype == D_SECT_TYPE_STLG_MCELL )
    {
        bDoNewAlgorithm = TRUE;
    }

    if ( bDoNewAlgorithm )
    {
        // 무조건 ConvertSectShape2EdgeNew 를 불러도 되지만..
        // 단면쪽은 실수하면 망하니깐.. 일단 중국에서만 쓰는 MCell 에서만 개선 코드를 쓰도록 넣음..
        // 향후 단면쪽 큰 변화가 생길 때 항상 이곳만 불리도록 하길 추천 함
        SetSimilarPntToSameNew(aaEdge);
    }
    else
    {
        for ( int i = 0; i < aaEdge.GetSize(); i++ )
        {
            for ( int j = 0; j < aaEdge.GetSize(); j++ )
            {
                if ( i == j ) continue;
                SetSimilarPntToSame(aaEdge[i].aAr, aaEdge[j].aAr);
            }
        }
    }

    return TRUE;
}

BOOL CSimpleMesh::MakeShareEdgeWhenCompNew(int nTypeBeforeAfter, T_SECT_D &SectD, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, BOOL bShareWidthSlab)
{
    BOOL bCheck = FALSE;
    if ( IsCompositeSlabGapMesh(SectD) )
    {
        // aaEdge의 마지막에 Gap이 있다.
        int nEdge = aaEdge.GetSize();

        CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdge01, aaEdge02;
        if ( CSectDB::IsSectPSComposite(SectD) )
        {
            if ( nTypeBeforeAfter==1 )
            {
                aaEdge01.Add(aaEdge[0]); // slab
                aaEdge01.Add(aaEdge[1]); // girder
                if ( MakeShareEdgeWhenComp_AllSect(nTypeBeforeAfter, SectD, aaEdge01, TRUE/*bShareWidthSlab*/) ) bCheck = TRUE;
                aaEdge.RemoveAll();
                aaEdge.Copy(aaEdge01);
            }
            else if ( nTypeBeforeAfter==2 )
            {
                aaEdge01.Add(aaEdge[2]); // gap
                aaEdge01.Add(aaEdge[1]); // girder
                if ( MakeShareEdgeWhenComp_GapCross(nTypeBeforeAfter, SectD, aaEdge01, aaEdge, TRUE/*bShareWidthSlab*/) ) bCheck = TRUE;  // gap과 girder의 교차점 추가
                aaEdge02.Add(aaEdge[0]); // slab
                aaEdge02.Add(aaEdge01[0]); // gap
                if ( MakeShareEdgeWhenComp_GapCross(nTypeBeforeAfter, SectD, aaEdge02, aaEdge, TRUE/*bShareWidthSlab*/) ) bCheck = TRUE;  // slab와 gap의 교차점 추가

                aaEdge.RemoveAll();
                aaEdge.Add(aaEdge02[0]); // slab
                aaEdge.Add(aaEdge01[1]); // girder
                aaEdge.Add(aaEdge01[0]); // gap
            }
			else if (nTypeBeforeAfter == 11 || nTypeBeforeAfter == 12)
			{
				/*skip*/
			}
            else
                ASSERT(0);
        }
        else if ( CSectDB::IsSectSteelComposite(SectD) )
        {
            if ( nTypeBeforeAfter==1 )
            {
                // Gap 제외
                for ( int i=0; i<nEdge-1; i++ ) aaEdge01.Add(aaEdge[i]);
                if ( MakeShareEdgeWhenComp_AllSect(nTypeBeforeAfter, SectD, aaEdge01, FALSE/*bShareWidthSlab*/) ) bCheck = TRUE;
                aaEdge.RemoveAll();
                aaEdge.Copy(aaEdge01);
            }
            else if ( nTypeBeforeAfter==2 )
            {
                // gap
                aaEdge01.Add(aaEdge[nEdge-1]);
                // girder+Stiffener
                for ( int i=1; i<nEdge-1; i++ ) aaEdge01.Add(aaEdge[i]);
                if ( MakeShareEdgeWhenComp_GapCross(nTypeBeforeAfter, SectD, aaEdge01, aaEdge, TRUE/*bShareWidthSlab*/, TRUE) ) bCheck = TRUE;  // gap과 girder의 교차점 추가

                aaEdge02.Add(aaEdge[0]);   // slab
                aaEdge02.Add(aaEdge01[0]); // gap
                if ( MakeShareEdgeWhenComp_GapCross(nTypeBeforeAfter, SectD, aaEdge02, aaEdge, TRUE/*bShareWidthSlab*/) ) bCheck = TRUE;  // slab와 gap의 교차점 추가

                aaEdge.RemoveAll();
                // 수정된 slab
                aaEdge.Add(aaEdge02[0]);
                // 수정된 girder+Stiffener
                for ( int i=1; i<aaEdge01.GetSize(); i++ ) aaEdge.Add(aaEdge01[i]);
                // gap
                aaEdge.Add(aaEdge01[0]);
            }
            //else
            //    ASSERT(0);
        }
        else
            ASSERT(0);
    }
    else
    {
        if ( MakeShareEdgeWhenComp_AllSect(nTypeBeforeAfter, SectD, aaEdge, TRUE/*bShareWidthSlab*/) ) bCheck = TRUE;
    }

    return bCheck;
}

// nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CSimpleMesh::MakeShareEdgeWhenComp_AllSect(int nTypeBeforeAfter, T_SECT_D &SectD, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, BOOL bShareWidthSlab)
{
    // Composite 단면일 때 Girder,Slab,Stiffner가 절점을 공유하도록 Edge를 쪼갬
    // 이 함수를 통과하면 같은 위치에 절점이 생기김.. 나중에 좌표가 같은 Node를 Merge시키면 절점공유가 됨

    if ( aaEdge.GetSize() <= 1 )
    {
        ASSERT(0); // 이 함수는 Composite단면일 때만 부르세요.
        return FALSE;
    }

    // aaEdge[0].aAr의 size가 0개면 Steel Girder 단면임

    struct KeyForEdgeMod : public StructKeyForStdMap
    {
        KeyForEdgeMod() : StructKeyForStdMap()
        {
        }
        KeyForEdgeMod(int nIIdx, int nJIdx) : StructKeyForStdMap()
        {
            aInt.push_back(nIIdx);
            aInt.push_back(nJIdx);
        }
    };

    struct AddPntInfo
    {
        T_POINT3D Pnt;
        std::vector<T_POINT3D> aPntWithSameEdge;  // AddPntInfo::Pnt생성 당시 동일한 Edge에서 생성됬던 Point들 저장.. 공유하는 Edge는 이 함수에서 쪼깬뒤.. CM2에서는 나누지 않도록 ratio를 1로 세팅하는데.. 그 외 지점은 CM2에서 쪼개게 하기 위해서 이 배열이 필요 함
        AddPntInfo& operator=(const AddPntInfo& src)
        {
            Pnt = src.Pnt;
            aPntWithSameEdge.clear();
            for ( int i = 0; i < src.aPntWithSameEdge.size(); i++ )
            {
                aPntWithSameEdge.push_back(src.aPntWithSameEdge[i]);
            }
            return *this;
        }
        void Init()
        {
            Pnt.Set(0.0, 0.0, 0.0);
            aPntWithSameEdge.clear();
        }
    };
    struct PosForDirSort
    {
        AddPntInfo PosInfo;
        double dSortDir[2];
        bool operator<(const PosForDirSort &rhs)  const
        {
            ASSERT(dSortDir[0] == rhs.dSortDir[0] && dSortDir[1] == rhs.dSortDir[1]);

            double dPos1DCur, dPos1DRhs; // 1차원 직선으로 투영한 위치

            dPos1DCur = PosInfo.Pnt.x * dSortDir[0] + PosInfo.Pnt.y * dSortDir[1];
            dPos1DRhs = rhs.PosInfo.Pnt.x * dSortDir[0] + rhs.PosInfo.Pnt.y * dSortDir[1];

            return dPos1DCur < dPos1DRhs;
        }
    };
    struct ValForEdgeMod
    {
        ValForEdgeMod()
        {
        }
        ValForEdgeMod& operator=(const ValForEdgeMod& src)
        {
            aAddPnt.clear();
            aAddPnt.resize(src.aAddPnt.size());
            for ( int i = 0; i < aAddPnt.size(); i++ )
            {
                aAddPnt[i] = src.aAddPnt[i];
            }

            return *this;
        }

        std::vector<AddPntInfo> aAddPnt;
    };

    std::map<KeyForEdgeMod, ValForEdgeMod> mapEdgeMod;

    double dSlabDivSize = DBL_MAX;

    int nSlabIdx;

    int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    nSlabIdx = 0;
    std::set<int> setGirderIdx;
    if ( nStype == D_SECT_TYPE_STLG_MCELL )
    {
        for ( int i = 1; i < aaEdge.GetSize(); i++ )
        {
            setGirderIdx.insert(i);//MCell 단면은 각 조각(2DShape기준)을 따로 Mesh 한 다음 마지막에 merge 한다.
        }
    }
    else if ( IsBoxGirderShape(nStype) )
    {
        for ( int i = 1; i <= GetBoxGirderShapeNum(nStype); i++ )
        {
            setGirderIdx.insert(i); // Box형상의 단면은 각 조각(2DShape기준)을 따로 Mesh 한 다음 마지막에 merge 한다... Stiffner와 Girder Size를 다르게 하기 위해서 MCell과 다른 처리를 추가로 함
        }
    }
    else
    {
        setGirderIdx.insert(1);
    }

    double dWholeMaxSize = -DBL_MAX;

    CArray<double, double&> aMinSize;
    CArray<double, double&> aMaxSize;
    aMinSize.SetSize(aaEdge.GetSize());
    aMaxSize.SetSize(aaEdge.GetSize());

    if ( nTypeBeforeAfter==1 )  bShareWidthSlab = FALSE;

    int nCompShapeType=1;

    BOOL bCheckSlab = FALSE;
    if ( ( CSectDB::IsSectPSComposite(SectD)    && nTypeBeforeAfter==2 && CSimpleMesh::IsCompositeSlabNoGap(SectD) ) ||
        ( CSectDB::IsSectSteelComposite(SectD) && nTypeBeforeAfter==2 && CSimpleMesh::IsCompositeSlabNoGap(SectD) ) )
    {
        // PSC 합성후는 전제 단면에 대해 size를 찾자.
        // PSC 합성전은 단일 거더 단면과 같은 방식이 되도록 하기 위해 아래쪽 분기를 거친다

        CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> aEdgeTemp; aEdgeTemp.RemoveAll();

        //for(int i=0; i<aaEdge.GetSize(); i++) aEdgeTemp.Append(aaEdge[i].aAr);
        aEdgeTemp.Append(aaEdge[1].aAr);

        double dMinSize=0.0;
        double dMaxSize=0.0;
        GetMinMaxAutoMeshSize(SectD, aEdgeTemp, dMinSize, dMaxSize, nCompShapeType, nTypeBeforeAfter, FALSE);

        for ( int i = 0; i < aMinSize.GetSize(); i++ )
        {
            aMaxSize[i] = dMaxSize;
            aMinSize[i] = dMinSize;
        }

        bCheckSlab = CheckGirderSlabWidth(SectD, aaEdge[1].aAr);
        //bCheckSlab = FALSE;
    }
    else
    {
        for ( int i = 0; i < aMinSize.GetSize(); i++ )
        {
            if ( i == nSlabIdx )
            {
                nCompShapeType = 0; // Slab
            }
            else if ( setGirderIdx.find(i) != setGirderIdx.end() )
            {
                nCompShapeType = 1; // Girder
            }
            else
            {
                nCompShapeType = 2; // Stiffener
            }

            if ( aaEdge[i].aAr.GetSize() == 0 )
                continue;  // Steel Girder 단면의 Slab

            GetMinMaxAutoMeshSize(SectD, aaEdge[i].aAr, aMinSize[i], aMaxSize[i], nCompShapeType, nTypeBeforeAfter, nStype == D_SECT_TYPE_STLG_MCELL || IsBoxGirderShape(nStype)); // EdgeSize 계산할 때 필요함
            if ( dWholeMaxSize < aMaxSize[i] )
            {
                dWholeMaxSize = aMaxSize[i];
            }
        }
    }

    const double dMergeTol = M_InitValueCurUnit(0.3, KN, MM, D_UNITSYS_BASE_LENGTH);     // 한번 걸러놓은 데이터를 비교할때 쓰는 톨러런스기 때문에 넉넉하게 함

    // 추가해야 할 점 찾기
    for ( int nGirderIdx : setGirderIdx )
    {
        for ( int i = 0; i < aaEdge.GetSize(); i++ )
        {
            if ( i == nGirderIdx ) continue;

            if ( i == nSlabIdx && !bShareWidthSlab ) continue;

            if ( aaEdge[i].aAr.GetSize() == 0 )
                continue;  // Steel Girder 단면의 Slab

            BOOL bStiffener;
            if ( nStype == D_SECT_TYPE_STLG_MCELL )
            {
                bStiffener = FALSE;
            }
            else if ( IsBoxGirderShape(nStype) )
            {
                bStiffener = i > GetBoxGirderShapeNum(nStype);
            }
            else
            {
                bStiffener = i >= 2;
            }

            std::vector<int> aIntsGirderIdx;
            std::vector<int> aIntsLoopIdx;

            int nIntsCnt = 0;

            CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeGirder = aaEdge[nGirderIdx].aAr;
            CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeLoop = aaEdge[i].aAr;
            for ( int j = 0; j < aEdgeGirder.GetSize(); j++ )
            {
                for ( int k = 0; k < aEdgeLoop.GetSize(); k++ )
                {
                    BOOL bOverlap = IsLineOverlap(aEdgeGirder[j], aEdgeLoop[k]); // 두개의 선분이 겹치는지

                    if ( bOverlap )
                    {
                        aIntsGirderIdx.push_back(j);
                        aIntsLoopIdx.push_back(k);
                        nIntsCnt++;
                    }
                }
            }

            if ( nIntsCnt >= 1 )
            {
                for ( int ii = 0; ii < aIntsGirderIdx.size(); ii++ )
                {
                    int nIntsGirderIdx = aIntsGirderIdx[ii];
                    int nIntsLoopIdx   = aIntsLoopIdx[ii];

                    double dMeshDivRatioGirder = GetMeshSizeRatio(aaEdge[nGirderIdx].aAr, nIntsGirderIdx, aMaxSize[nGirderIdx]);
                    double dMeshDivRatioLoop   = GetMeshSizeRatio(aaEdge[i].aAr, nIntsLoopIdx, aMaxSize[i]);

                    double dGirderMeshSize = aaEdge[nGirderIdx].aAr[nIntsGirderIdx].GetLineLength() * ( 1 / ( double ) ( ( int ) ( 1 / dMeshDivRatioGirder ) ) ); // 단위변환 부분은 0.52 같은 놈은 2등분인 0.5가 되도록 버림 처리 하는 것임
                    double dIMeshSize      = aaEdge[i].aAr[nIntsLoopIdx].GetLineLength() * ( 1 / ( double ) ( ( int ) ( 1 / dMeshDivRatioLoop ) ) ); // 단위변환 부분은 0.52 같은 놈은 2등분인 0.5가 되도록 버림 처리 하는 것임

                    CArray<T_POINT3D, T_POINT3D&> aAddPnt;

                    // 1.추가할 포인트 찾기
                    if ( ( i == nSlabIdx ) && bCheckSlab ) // 슬래브 > 거더 : 거더의 포인트를 슬래브에 포함시킴.
                    {
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p1);
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p2);
                    }
                    else if ( ( i == nSlabIdx ) && !bCheckSlab ) // 거더 > 슬래브 : 슬래브의 포인트를 거더에 포함시킴.
                    {
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p1);  // 거더
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p2);
                        aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p1);             // 슬래브
                        aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p2);
                    }
                    else
                    {
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p1);
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p2);
                        aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p1);
                        aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p2);

                        if ( bStiffener )
                        {
                            AppendDivPnt(aaEdge[i].aAr[nIntsLoopIdx], dIMeshSize, FALSE, aAddPnt); // Stiffener는 Girder의 한 면에 접하므로 Stiffener 기준으로 Mesh를 만들어 공유함
                        }
                        else if ( dGirderMeshSize < dIMeshSize ) // Girder와Slab를 비교할 때는 더 짧게 쪼개지는 쪽 기준으로...
                        {
                            AppendDivPnt(aaEdge[nGirderIdx].aAr[nIntsGirderIdx], dGirderMeshSize, FALSE, aAddPnt);
                        }
                        else
                        {
                            AppendDivPnt(aaEdge[i].aAr[nIntsLoopIdx], dIMeshSize, FALSE, aAddPnt);
                        }
                    }

                    // 2.포인트 저장
                    if ( i == nSlabIdx && bCheckSlab ) // 슬래브 > 거더 : 거더의 포인트를 슬래브에 포함시킴.
                    {
                        //double dMeshSize = min(dGirderMeshSize, dIMeshSize);
                        //dSlabDivSize = dMeshSize;

                        //////////////////////////////////////////////////////////////////////////
                        T_LINE_FOR_MESH_G& SlabEdge = aaEdge[nSlabIdx].aAr[nIntsLoopIdx];
                        std::vector<T_POINT3D> aDifPntWithSlabEdge;
                        for ( int jj = 0; jj < aAddPnt.GetSize(); jj++ )
                        {
                            //aDifPntWithSlabEdge.push_back(aAddPnt[jj]);
                            if ( is_on_line_2d(SlabEdge.p1.x, SlabEdge.p1.y, SlabEdge.p2.x, SlabEdge.p2.y, aAddPnt[jj].x, aAddPnt[jj].y, dMergeTol) )
                            {
                                if ( !aAddPnt[jj].IsSame(SlabEdge.p1, 1.e-8) && !aAddPnt[jj].IsSame(SlabEdge.p2, 1.e-8) )
                                {
                                    aDifPntWithSlabEdge.push_back(aAddPnt[jj]);
                                }
                            }
                        }
                        if ( aDifPntWithSlabEdge.size() > 0 )
                        {
                            KeyForEdgeMod Key(nSlabIdx, nIntsLoopIdx);
                            auto itrFind = mapEdgeMod.find(Key);
                            if ( itrFind == mapEdgeMod.end() )
                            {
                                mapEdgeMod[Key] = ValForEdgeMod();
                                itrFind = mapEdgeMod.find(Key);;
                            }

                            AddPntInfo PntInfo;
                            for ( int jj = 0; jj < aDifPntWithSlabEdge.size(); jj++ )
                            {
                                PntInfo.Init();
                                PntInfo.Pnt = aDifPntWithSlabEdge[jj];
                                ValForEdgeMod& Val = itrFind->second;
                                Val.aAddPnt.push_back(PntInfo);
                            }
                        }
                        //////////////////////////////////////////////////////////////////////////
                    }
                    else
                    {
                        T_LINE_FOR_MESH_G& GirderEdge = aaEdge[nGirderIdx].aAr[nIntsGirderIdx];
                        std::vector<T_POINT3D> aDifPntWithGirderEdge;
                        for ( int jj = 0; jj < aAddPnt.GetSize(); jj++ )
                        {
                            if ( is_on_line_2d(GirderEdge.p1.x, GirderEdge.p1.y, GirderEdge.p2.x, GirderEdge.p2.y, aAddPnt[jj].x, aAddPnt[jj].y, dMergeTol) )
                            {
                                if ( !aAddPnt[jj].IsSame(GirderEdge.p1, 1.e-8) && !aAddPnt[jj].IsSame(GirderEdge.p2, 1.e-8) )
                                {
                                    aDifPntWithGirderEdge.push_back(aAddPnt[jj]);
                                }
                            }
                        }
                        if ( aDifPntWithGirderEdge.size() > 0 )
                        {
                            KeyForEdgeMod Key(nGirderIdx, nIntsGirderIdx);
                            auto itrFind = mapEdgeMod.find(Key);
                            if ( itrFind == mapEdgeMod.end() )
                            {
                                mapEdgeMod[Key] = ValForEdgeMod();
                                itrFind = mapEdgeMod.find(Key);;
                            }

                            AddPntInfo PntInfo;
                            for ( int jj = 0; jj < aDifPntWithGirderEdge.size(); jj++ )
                            {
                                PntInfo.Init();
                                PntInfo.Pnt = aDifPntWithGirderEdge[jj];
                                if ( i != nSlabIdx )
                                { // Slab인 경우는 이부분 통과
                                    for ( int kk = 2; kk < aAddPnt.GetSize(); kk++ ) // 2부터 시작하는 이유는 Girder의 Edge를 Skip하기 위함임
                                    {
                                        if ( !PntInfo.Pnt.IsSame(aAddPnt[kk], 1.e-8) )
                                        {
                                            PntInfo.aPntWithSameEdge.push_back(aAddPnt[kk]);
                                        }
                                    }
                                }
                                ValForEdgeMod& Val = itrFind->second;
                                Val.aAddPnt.push_back(PntInfo);
                            }
                        }

                        //////////////////////////////////////////////////////////////////////////
                        BOOL bChkCompsite = FALSE;
                        if ( ( CSectDB::IsSectPSComposite(SectD)    && nTypeBeforeAfter==2 ) ||
                            ( CSectDB::IsSectSteelComposite(SectD) && nTypeBeforeAfter==2 ) ) bChkCompsite = TRUE;

                        // 거더 > 슬래브 : 슬래브의 포인트를 거더에 포함시킴.
                        if ( bChkCompsite && ( i == nSlabIdx ) && !bCheckSlab )
                        {
                            //////////////////////////////////////////////////////////////////////////
                            T_LINE_FOR_MESH_G& SlabEdge = aaEdge[nSlabIdx].aAr[nIntsLoopIdx];
                            std::vector<T_POINT3D> aDifPntWithSlabEdge;
                            for ( int jj = 0; jj < aAddPnt.GetSize(); jj++ )
                            {
                                //aDifPntWithSlabEdge.push_back(aAddPnt[jj]);
                                if ( is_on_line_2d(SlabEdge.p1.x, SlabEdge.p1.y, SlabEdge.p2.x, SlabEdge.p2.y, aAddPnt[jj].x, aAddPnt[jj].y, dMergeTol) )
                                {
                                    if ( !aAddPnt[jj].IsSame(SlabEdge.p1, 1.e-8) && !aAddPnt[jj].IsSame(SlabEdge.p2, 1.e-8) )
                                    {
                                        aDifPntWithSlabEdge.push_back(aAddPnt[jj]);
                                    }
                                }
                            }
                            if ( aDifPntWithSlabEdge.size() > 0 )
                            {
                                KeyForEdgeMod Key(nSlabIdx, nIntsLoopIdx);
                                auto itrFind = mapEdgeMod.find(Key);
                                if ( itrFind == mapEdgeMod.end() )
                                {
                                    mapEdgeMod[Key] = ValForEdgeMod();
                                    itrFind = mapEdgeMod.find(Key);;
                                }

                                AddPntInfo PntInfo;
                                for ( int jj = 0; jj < aDifPntWithSlabEdge.size(); jj++ )
                                {
                                    PntInfo.Init();
                                    PntInfo.Pnt = aDifPntWithSlabEdge[jj];
                                    ValForEdgeMod& Val = itrFind->second;
                                    Val.aAddPnt.push_back(PntInfo);
                                }
                            }
                        }
                        //////////////////////////////////////////////////////////////////////////
                    } // esle
                } // for
            } // if(nIntsCnt >= 1)
        } // for aaEdge.GetSize();
    }

    if ( mapEdgeMod.size() == 0 )
    {
        return TRUE;
    }

    ///* 이 주석을 풀면 Slab의 모든 Edge를 dSlabDivSize로 쪼갬
// 	if(dSlabDivSize != DBL_MAX)
// 	{
// 	// Slab의 아랫 Edge가 쪼개졌을 경우에 여기로 들어옴..
// 	// Slab의 아랫 Edge만 잘게 쪼개면 모양이 안이쁠 수 있기 때문에 모든 Edge를 동일한 Size로 쪼갬
// 		for (int j = 0; j < aaEdge[nSlabIdx].aAr.GetSize(); j++)
// 		{
// 			KeyForEdgeMod Key(nSlabIdx, j);
// 			auto itrFind = mapEdgeMod.find(Key);
// 			if(itrFind == mapEdgeMod.end())
// 			{
// 				mapEdgeMod[Key] = ValForEdgeMod();
// 				itrFind = mapEdgeMod.find(Key);;
//
// 				CArray<T_POINT3D, T_POINT3D&> aAddPnt;
// 				AppendDivPnt(aaEdge[nSlabIdx].aAr[j], dSlabDivSize, FALSE, aAddPnt);
//
// 				for (int k = 0; k < aAddPnt.GetSize(); k++)
// 				{
// 					itrFind->second.aAddPnt.push_back(aAddPnt[k]);
// 				}
// 			}
// 		}
// 	}

    // 추가점 sorting
    std::vector<PosForDirSort> aPosForSort;
    PosForDirSort EachPntForSort;
    double dDir[3];
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            auto itrFind = mapEdgeMod.find(KeyForEdgeMod(i, j));
            if ( itrFind != mapEdgeMod.end() )
            {
                GetDir(aaEdge[i].aAr[j].p1, aaEdge[i].aAr[j].p2, dDir);

                std::vector<AddPntInfo>& aAddPntInfo = itrFind->second.aAddPnt;

                aPosForSort.clear();
                for ( int k = 0; k < aAddPntInfo.size(); k++ )
                {
                    EachPntForSort.PosInfo = aAddPntInfo[k];
                    EachPntForSort.dSortDir[0] = dDir[0];
                    EachPntForSort.dSortDir[1] = dDir[1];
                    aPosForSort.push_back(EachPntForSort);
                }
                std::sort(aPosForSort.begin(), aPosForSort.end()); // dDir 방향으로 Sorting

                for ( int k = 0; k < aAddPntInfo.size(); k++ )
                {
                    aAddPntInfo[k] = aPosForSort[k].PosInfo;
                }
            }
        }
    }

    // 추가점 적용
    T_LINE_FOR_MESH_G EdgeTemp;
    CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdgeMod;
    BOOL bNoDivWhenMesh = FALSE;
    aaEdgeMod.SetSize(aaEdge.GetSize());
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            auto itrFind = mapEdgeMod.find(KeyForEdgeMod(i, j));
            if ( itrFind != mapEdgeMod.end() )
            {
                std::vector<AddPntInfo>& FindVal = itrFind->second.aAddPnt;
                EdgeTemp.p1 = aaEdge[i].aAr[j].p1;
                for ( int k = 0; k < FindVal.size(); k++ )
                {
                    EdgeTemp.p2 = FindVal[k].Pnt;
                    bNoDivWhenMesh = FALSE;
                    for ( int ii = 0; ii < FindVal[k].aPntWithSameEdge.size(); ii++ )
                    {
                        if ( EdgeTemp.p1.IsSame(FindVal[k].aPntWithSameEdge[ii], 1.e-8) )
                        {
                            bNoDivWhenMesh = TRUE;
                        }
                    }
                    EdgeTemp.bNoDivWhenMesh = bNoDivWhenMesh;
                    aaEdgeMod[i].aAr.Add(EdgeTemp);
                    EdgeTemp.p1 = FindVal[k].Pnt;
                }
                EdgeTemp.p2 = aaEdge[i].aAr[j].p2;
                bNoDivWhenMesh = FALSE;
                for ( int ii = 0; ii < FindVal[FindVal.size() - 1].aPntWithSameEdge.size(); ii++ )
                {
                    if ( EdgeTemp.p1.IsSame(FindVal[FindVal.size() - 1].aPntWithSameEdge[ii], 1.e-8) )
                    {
                        bNoDivWhenMesh = TRUE;
                    }
                }
                EdgeTemp.bNoDivWhenMesh = bNoDivWhenMesh;
                aaEdgeMod[i].aAr.Add(EdgeTemp);
            }
            else
            {
                aaEdgeMod[i].aAr.Add(aaEdge[i].aAr[j]);
                aaEdgeMod[i].aAr[aaEdgeMod[i].aAr.GetSize() - 1].bNoDivWhenMesh = FALSE;
            }
        }
    }

    ASSERT(aaEdge.GetSize() == aaEdgeMod.GetSize());

    const double dTol = 1.e-6;

    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        aaEdge[i].aAr.RemoveAll();
        for ( int j = 0; j < aaEdgeMod[i].aAr.GetSize(); j++ )
        {
            if ( abs(aaEdgeMod[i].aAr[j].GetLineLength()) < dTol )
            {
                continue;
            }

            aaEdge[i].aAr.Add(aaEdgeMod[i].aAr[j]);
        }
    }

    // 절점 Merge
    BOOL bDoNewAlgorithm = FALSE;
    if ( nStype == D_SECT_TYPE_STLG_MCELL )
    {
        bDoNewAlgorithm = TRUE;
    }

    if ( bDoNewAlgorithm )
    {
        // 무조건 ConvertSectShape2EdgeNew 를 불러도 되지만..
        // 단면쪽은 실수하면 망하니깐.. 일단 중국에서만 쓰는 MCell 에서만 개선 코드를 쓰도록 넣음..
        // 향후 단면쪽 큰 변화가 생길 때 항상 이곳만 불리도록 하길 추천 함
        SetSimilarPntToSameNew(aaEdge);
    }
    else
    {
        for ( int i = 0; i < aaEdge.GetSize(); i++ )
        {
            for ( int j = 0; j < aaEdge.GetSize(); j++ )
            {
                if ( i == j ) continue;
                SetSimilarPntToSame(aaEdge[i].aAr, aaEdge[j].aAr);
            }
        }
    }

    return TRUE;
}
BOOL CSimpleMesh::MakeShareEdgeWhenComp_GapCross(int nTypeBeforeAfter, T_SECT_D &SectD, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdgeAll, BOOL bShareWidthSlab, BOOL bGapnSlab/*=FALSE*/)
{
    // Composite 단면일 때 Girder,Slab,Stiffner가 절점을 공유하도록 Edge를 쪼갬
    // 이 함수를 통과하면 같은 위치에 절점이 생기김.. 나중에 좌표가 같은 Node를 Merge시키면 절점공유가 됨

    if ( aaEdge.GetSize() <= 1 )
    {
        ASSERT(0); // 이 함수는 Composite단면일 때만 부르세요.
        return FALSE;
    }

    // aaEdge[0].aAr의 size가 0개면 Steel Girder 단면임
    double dYtop = aaEdge[0].aAr[0].p1.y;

    struct KeyForEdgeMod : public StructKeyForStdMap
    {
        KeyForEdgeMod() : StructKeyForStdMap()
        {
        }
        KeyForEdgeMod(int nIIdx, int nJIdx) : StructKeyForStdMap()
        {
            aInt.push_back(nIIdx);
            aInt.push_back(nJIdx);
        }
    };

    struct AddPntInfo
    {
        T_POINT3D Pnt;
        std::vector<T_POINT3D> aPntWithSameEdge;  // AddPntInfo::Pnt생성 당시 동일한 Edge에서 생성됬던 Point들 저장.. 공유하는 Edge는 이 함수에서 쪼깬뒤.. CM2에서는 나누지 않도록 ratio를 1로 세팅하는데.. 그 외 지점은 CM2에서 쪼개게 하기 위해서 이 배열이 필요 함
        AddPntInfo& operator=(const AddPntInfo& src)
        {
            Pnt = src.Pnt;
            aPntWithSameEdge.clear();
            for ( int i = 0; i < src.aPntWithSameEdge.size(); i++ )
            {
                aPntWithSameEdge.push_back(src.aPntWithSameEdge[i]);
            }
            return *this;
        }
        void Init()
        {
            Pnt.Set(0.0, 0.0, 0.0);
            aPntWithSameEdge.clear();
        }
    };
    struct PosForDirSort
    {
        AddPntInfo PosInfo;
        double dSortDir[2];
        bool operator<(const PosForDirSort &rhs)  const
        {
            ASSERT(dSortDir[0] == rhs.dSortDir[0] && dSortDir[1] == rhs.dSortDir[1]);

            double dPos1DCur, dPos1DRhs; // 1차원 직선으로 투영한 위치

            dPos1DCur = PosInfo.Pnt.x * dSortDir[0] + PosInfo.Pnt.y * dSortDir[1];
            dPos1DRhs = rhs.PosInfo.Pnt.x * dSortDir[0] + rhs.PosInfo.Pnt.y * dSortDir[1];

            return dPos1DCur < dPos1DRhs;
        }
    };
    struct ValForEdgeMod
    {
        ValForEdgeMod()
        {
        }
        ValForEdgeMod& operator=(const ValForEdgeMod& src)
        {
            aAddPnt.clear();
            aAddPnt.resize(src.aAddPnt.size());
            for ( int i = 0; i < aAddPnt.size(); i++ )
            {
                aAddPnt[i] = src.aAddPnt[i];
            }

            return *this;
        }

        std::vector<AddPntInfo> aAddPnt;
    };

    std::map<KeyForEdgeMod, ValForEdgeMod> mapEdgeMod;

    double dSlabDivSize = DBL_MAX;

    int nSlabIdx = 0;

    int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    std::set<int> setGirderIdx;
    if ( nStype == D_SECT_TYPE_STLG_MCELL )
    {
        for ( int i = 1; i < aaEdge.GetSize(); i++ )
        {
            setGirderIdx.insert(i);//MCell 단면은 각 조각(2DShape기준)을 따로 Mesh 한 다음 마지막에 merge 한다.
        }
    }
    else if ( IsBoxGirderShape(nStype) && bGapnSlab )
    {
        for ( int i = 1; i <= GetBoxGirderShapeNum(nStype); i++ )
        {
            setGirderIdx.insert(i); // Box형상의 단면은 각 조각(2DShape기준)을 따로 Mesh 한 다음 마지막에 merge 한다... Stiffner와 Girder Size를 다르게 하기 위해서 MCell과 다른 처리를 추가로 함
        }
    }
    else
    {
        setGirderIdx.insert(1);
    }

    double dWholeMaxSize = -DBL_MAX;

    CArray<double, double&> aMinSize;
    CArray<double, double&> aMaxSize;
    aMinSize.SetSize(aaEdge.GetSize());
    aMaxSize.SetSize(aaEdge.GetSize());

    int nCompShapeType=1;

    BOOL bCheckSlab = FALSE;
    if ( ( CSectDB::IsSectPSComposite(SectD)    && nTypeBeforeAfter==2 && CSimpleMesh::IsCompositeSlabNoGap(SectD) )
        /* || (CSectDB::IsSectSteelComposite(SectD) && nTypeBeforeAfter==2 && CSimpleMesh::IsCompositeSlabNoGap(SectD))  */ )
    {
        // PSC 합성후는 거더에 대한 size 이용
        CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> aEdgeTemp; aEdgeTemp.RemoveAll();
        aEdgeTemp.Append(aaEdgeAll[1].aAr);

        double dMinSize=0.0;
        double dMaxSize=0.0;
        GetMinMaxAutoMeshSize(SectD, aEdgeTemp, dMinSize, dMaxSize, nCompShapeType, nTypeBeforeAfter, FALSE);

        for ( int i = 0; i < aMinSize.GetSize(); i++ )
        {
            aMaxSize[i] = dMaxSize;
            aMinSize[i] = dMinSize;
        }

        bCheckSlab = CheckGirderSlabWidth(SectD, aaEdge[1].aAr);
    }
    else
    {
        for ( int i = 0; i < aMinSize.GetSize(); i++ )
        {
            if ( i == nSlabIdx )
            {
                nCompShapeType = 0; // Slab
            }
            else if ( setGirderIdx.find(i) != setGirderIdx.end() )
            {
                nCompShapeType = 1; // Girder
            }
            else
            {
                nCompShapeType = 2; // Stiffener
            }

            if ( aaEdge[i].aAr.GetSize() == 0 )
                continue;  // Steel Girder 단면의 Slab

            GetMinMaxAutoMeshSize(SectD, aaEdge[i].aAr, aMinSize[i], aMaxSize[i], nCompShapeType, nTypeBeforeAfter, nStype == D_SECT_TYPE_STLG_MCELL || IsBoxGirderShape(nStype)); // EdgeSize 계산할 때 필요함

            if ( IsBoxGirderShape(nStype) && bGapnSlab && nCompShapeType==1 )
            {
                // PSC 합성후는 거더에 대한 size 이용
                aMaxSize[0] = max(aMaxSize[0], aMaxSize[i]);
            }

            if ( dWholeMaxSize < aMaxSize[i] )
            {
                dWholeMaxSize = aMaxSize[i];
            }
        }
    }

    const double dMergeTol = M_InitValueCurUnit(0.3, KN, MM, D_UNITSYS_BASE_LENGTH);     // 한번 걸러놓은 데이터를 비교할때 쓰는 톨러런스기 때문에 넉넉하게 함

    // 추가해야 할 점 찾기
    for ( int nGirderIdx : setGirderIdx )
    {
        for ( int i = 0; i < aaEdge.GetSize(); i++ )
        {
            if ( i == nGirderIdx ) continue;

            if ( i == nSlabIdx && !bShareWidthSlab ) continue;

            if ( aaEdge[i].aAr.GetSize() == 0 )
                continue;  // Steel Girder 단면의 Slab

            BOOL bStiffener = FALSE;
            if ( nStype == D_SECT_TYPE_STLG_MCELL )
            {
                bStiffener = FALSE;
            }
            else if ( IsBoxGirderShape(nStype) && bGapnSlab )
            {
                bStiffener = i > GetBoxGirderShapeNum(nStype);
            }
            else
            {
                bStiffener = i >= 2;
            }

            std::vector<int> aIntsGirderIdx;
            std::vector<int> aIntsLoopIdx;

            int nIntsCnt = 0;

            CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeGirder = aaEdge[nGirderIdx].aAr;
            CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeLoop = aaEdge[i].aAr;
            for ( int j = 0; j < aEdgeGirder.GetSize(); j++ )
            {
                for ( int k = 0; k < aEdgeLoop.GetSize(); k++ )
                {
                    BOOL bOverlap = IsLineOverlap(aEdgeGirder[j], aEdgeLoop[k]); // 두개의 선분이 겹치는지

                    if ( bOverlap )
                    {
                        aIntsGirderIdx.push_back(j);
                        aIntsLoopIdx.push_back(k);
                        nIntsCnt++;
                    }
                }
            }

            if ( nIntsCnt >= 1 )
            {
                for ( int ii = 0; ii < aIntsGirderIdx.size(); ii++ )
                {
                    int nIntsGirderIdx = aIntsGirderIdx[ii];
                    int nIntsLoopIdx   = aIntsLoopIdx[ii];

                    double dMeshDivRatioGirder = GetMeshSizeRatio(aaEdge[nGirderIdx].aAr, nIntsGirderIdx, aMaxSize[nGirderIdx]);
                    double dMeshDivRatioLoop   = GetMeshSizeRatio(aaEdge[i].aAr, nIntsLoopIdx, aMaxSize[i]);

                    double dGirderMeshSize = aaEdge[nGirderIdx].aAr[nIntsGirderIdx].GetLineLength() * ( 1 / ( double ) ( ( int ) ( 1 / dMeshDivRatioGirder ) ) ); // 단위변환 부분은 0.52 같은 놈은 2등분인 0.5가 되도록 버림 처리 하는 것임
                    double dIMeshSize      = aaEdge[i].aAr[nIntsLoopIdx].GetLineLength() * ( 1 / ( double ) ( ( int ) ( 1 / dMeshDivRatioLoop ) ) ); // 단위변환 부분은 0.52 같은 놈은 2등분인 0.5가 되도록 버림 처리 하는 것임

                    CArray<T_POINT3D, T_POINT3D&> aAddPnt;

                    // 1.추가할 포인트 찾기
                    if ( ( i == nSlabIdx ) && bCheckSlab ) // 슬래브 > 거더 : 거더의 포인트를 슬래브에 포함시킴.
                    {
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p1);
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p2);
                    }
                    else if ( ( i == nSlabIdx ) && !bCheckSlab ) // 거더 > 슬래브 : 슬래브의 포인트를 거더에 포함시킴.
                    {
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p1);
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p2);
                        aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p1);
                        aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p2);
                    }
                    else
                    {
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p1);
                        aAddPnt.Add(aaEdge[nGirderIdx].aAr[nIntsGirderIdx].p2);
                        aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p1);
                        aAddPnt.Add(aaEdge[i].aAr[nIntsLoopIdx].p2);

                        if ( bStiffener )
                        {
                            AppendDivPnt(aaEdge[i].aAr[nIntsLoopIdx], dIMeshSize, FALSE, aAddPnt); // Stiffener는 Girder의 한 면에 접하므로 Stiffener 기준으로 Mesh를 만들어 공유함
                        }
                        else if ( dGirderMeshSize < dIMeshSize ) // Girder와Slab를 비교할 때는 더 짧게 쪼개지는 쪽 기준으로...
                        {
                            AppendDivPnt(aaEdge[nGirderIdx].aAr[nIntsGirderIdx], dGirderMeshSize, FALSE, aAddPnt);
                        }
                        else
                        {
                            AppendDivPnt(aaEdge[i].aAr[nIntsLoopIdx], dIMeshSize, FALSE, aAddPnt);
                        }
                    }

                    // 2.포인트 저장
                    if ( i == nSlabIdx && bCheckSlab ) // 슬래브 > 거더 : 거더의 포인트를 슬래브에 포함시킴.
                    {
                        //double dMeshSize = min(dGirderMeshSize, dIMeshSize);
                        //dSlabDivSize = dMeshSize;

                        //////////////////////////////////////////////////////////////////////////
                        T_LINE_FOR_MESH_G& SlabEdge = aaEdge[nSlabIdx].aAr[nIntsLoopIdx];
                        std::vector<T_POINT3D> aDifPntWithSlabEdge;
                        for ( int jj = 0; jj < aAddPnt.GetSize(); jj++ )
                        {
                            //aDifPntWithSlabEdge.push_back(aAddPnt[jj]);
                            if ( is_on_line_2d(SlabEdge.p1.x, SlabEdge.p1.y, SlabEdge.p2.x, SlabEdge.p2.y, aAddPnt[jj].x, aAddPnt[jj].y, dMergeTol) )
                            {
                                if ( !aAddPnt[jj].IsSame(SlabEdge.p1, 1.e-8) && !aAddPnt[jj].IsSame(SlabEdge.p2, 1.e-8) )
                                {
                                    aDifPntWithSlabEdge.push_back(aAddPnt[jj]);
                                }
                            }
                        }
                        if ( aDifPntWithSlabEdge.size() > 0 )
                        {
                            KeyForEdgeMod Key(nSlabIdx, nIntsLoopIdx);
                            auto itrFind = mapEdgeMod.find(Key);
                            if ( itrFind == mapEdgeMod.end() )
                            {
                                mapEdgeMod[Key] = ValForEdgeMod();
                                itrFind = mapEdgeMod.find(Key);;
                            }

                            AddPntInfo PntInfo;
                            for ( int jj = 0; jj < aDifPntWithSlabEdge.size(); jj++ )
                            {
                                PntInfo.Init();
                                PntInfo.Pnt = aDifPntWithSlabEdge[jj];
                                ValForEdgeMod& Val = itrFind->second;
                                Val.aAddPnt.push_back(PntInfo);
                            }

                            ////////////////////////////////////////////////////
                            // Gap, Slab의 top 영역에 포인트 추가
                            KeyForEdgeMod Key2(nSlabIdx, 0);
                            auto itrFind2 = mapEdgeMod.find(Key2);
                            if ( itrFind2 == mapEdgeMod.end() )
                            {
                                mapEdgeMod[Key2] = ValForEdgeMod();
                                itrFind2 = mapEdgeMod.find(Key2);;
                            }
                            for ( int jj = 0; jj < aDifPntWithSlabEdge.size(); jj++ )
                            {
                                PntInfo.Init();
                                PntInfo.Pnt = aDifPntWithSlabEdge[jj];
                                PntInfo.Pnt.y = dYtop;

                                ValForEdgeMod& Val2 = itrFind2->second;
                                Val2.aAddPnt.push_back(PntInfo);
                            }
                            ////////////////////////////////////////////////////
                        }
                        //////////////////////////////////////////////////////////////////////////
                    }
                    else
                    {
                        T_LINE_FOR_MESH_G& GirderEdge = aaEdge[nGirderIdx].aAr[nIntsGirderIdx];
                        std::vector<T_POINT3D> aDifPntWithGirderEdge;
                        for ( int jj = 0; jj < aAddPnt.GetSize(); jj++ )
                        {
                            if ( is_on_line_2d(GirderEdge.p1.x, GirderEdge.p1.y, GirderEdge.p2.x, GirderEdge.p2.y, aAddPnt[jj].x, aAddPnt[jj].y, dMergeTol) )
                            {
                                if ( !aAddPnt[jj].IsSame(GirderEdge.p1, 1.e-8) && !aAddPnt[jj].IsSame(GirderEdge.p2, 1.e-8) )
                                {
                                    aDifPntWithGirderEdge.push_back(aAddPnt[jj]);
                                }
                            }
                        }
                        if ( aDifPntWithGirderEdge.size() > 0 )
                        {
                            KeyForEdgeMod Key(nGirderIdx, nIntsGirderIdx);
                            auto itrFind = mapEdgeMod.find(Key);
                            if ( itrFind == mapEdgeMod.end() )
                            {
                                mapEdgeMod[Key] = ValForEdgeMod();
                                itrFind = mapEdgeMod.find(Key);;
                            }

                            AddPntInfo PntInfo;
                            for ( int jj = 0; jj < aDifPntWithGirderEdge.size(); jj++ )
                            {
                                PntInfo.Init();
                                PntInfo.Pnt = aDifPntWithGirderEdge[jj];
                                if ( i != nSlabIdx )
                                {
                                    for ( int kk = 2; kk < aAddPnt.GetSize(); kk++ ) // 2부터 시작하는 이유는 Girder의 Edge를 Skip하기 위함임
                                    {
                                        if ( !PntInfo.Pnt.IsSame(aAddPnt[kk], 1.e-8) )
                                        {
                                            PntInfo.aPntWithSameEdge.push_back(aAddPnt[kk]);
                                        }
                                    }
                                }
                                ValForEdgeMod& Val = itrFind->second;
                                Val.aAddPnt.push_back(PntInfo);
                            }
                        }

                        //////////////////////////////////////////////////////////////////////////
                        BOOL bChkCompsite = FALSE;
                        if ( ( CSectDB::IsSectPSComposite(SectD)    && nTypeBeforeAfter==2 ) ||
                            ( CSectDB::IsSectSteelComposite(SectD) && nTypeBeforeAfter==2 ) ) bChkCompsite = TRUE;

                        // 거더 > 슬래브 : 슬래브의 포인트를 거더에 포함시킴.
                        if ( bChkCompsite && ( i == nSlabIdx ) && !bCheckSlab )
                        {
                            //////////////////////////////////////////////////////////////////////////
                            T_LINE_FOR_MESH_G& SlabEdge = aaEdge[nSlabIdx].aAr[nIntsLoopIdx];
                            std::vector<T_POINT3D> aDifPntWithSlabEdge;
                            for ( int jj = 0; jj < aAddPnt.GetSize(); jj++ )
                            {
                                //aDifPntWithSlabEdge.push_back(aAddPnt[jj]);
                                if ( is_on_line_2d(SlabEdge.p1.x, SlabEdge.p1.y, SlabEdge.p2.x, SlabEdge.p2.y, aAddPnt[jj].x, aAddPnt[jj].y, dMergeTol) )
                                {
                                    if ( !aAddPnt[jj].IsSame(SlabEdge.p1, 1.e-8) && !aAddPnt[jj].IsSame(SlabEdge.p2, 1.e-8) )
                                    {
                                        aDifPntWithSlabEdge.push_back(aAddPnt[jj]);
                                    }
                                }
                            }
                            if ( aDifPntWithSlabEdge.size() > 0 )
                            {
                                KeyForEdgeMod Key(nSlabIdx, nIntsLoopIdx);
                                auto itrFind = mapEdgeMod.find(Key);
                                if ( itrFind == mapEdgeMod.end() )
                                {
                                    mapEdgeMod[Key] = ValForEdgeMod();
                                    itrFind = mapEdgeMod.find(Key);;
                                }

                                AddPntInfo PntInfo;
                                for ( int jj = 0; jj < aDifPntWithSlabEdge.size(); jj++ )
                                {
                                    PntInfo.Init();
                                    PntInfo.Pnt = aDifPntWithSlabEdge[jj];
                                    ValForEdgeMod& Val = itrFind->second;
                                    Val.aAddPnt.push_back(PntInfo);
                                }

                                ////////////////////////////////////////////////////
                                // Gap, Slab의 top 영역에 포인트 추가
                                KeyForEdgeMod Key2(nSlabIdx, 0);
                                auto itrFind2 = mapEdgeMod.find(Key2);
                                if ( itrFind2 == mapEdgeMod.end() )
                                {
                                    mapEdgeMod[Key2] = ValForEdgeMod();
                                    itrFind2 = mapEdgeMod.find(Key2);;
                                }
                                for ( int jj = 0; jj < aDifPntWithSlabEdge.size(); jj++ )
                                {
                                    PntInfo.Init();
                                    PntInfo.Pnt = aDifPntWithSlabEdge[jj];
                                    PntInfo.Pnt.y = dYtop;

                                    ValForEdgeMod& Val2 = itrFind2->second;
                                    Val2.aAddPnt.push_back(PntInfo);
                                }
                                ////////////////////////////////////////////////////
                            }
                        }
                        //////////////////////////////////////////////////////////////////////////
                    } // esle
                } // for
            } // if(nIntsCnt >= 1)
        } // for aaEdge.GetSize();
    }

    if ( mapEdgeMod.size() == 0 )
    {
        return TRUE;
    }

    ///* 이 주석을 풀면 Slab의 모든 Edge를 dSlabDivSize로 쪼갬
// 	if(dSlabDivSize != DBL_MAX)
// 	{
// 	// Slab의 아랫 Edge가 쪼개졌을 경우에 여기로 들어옴..
// 	// Slab의 아랫 Edge만 잘게 쪼개면 모양이 안이쁠 수 있기 때문에 모든 Edge를 동일한 Size로 쪼갬
// 		for (int j = 0; j < aaEdge[nSlabIdx].aAr.GetSize(); j++)
// 		{
// 			KeyForEdgeMod Key(nSlabIdx, j);
// 			auto itrFind = mapEdgeMod.find(Key);
// 			if(itrFind == mapEdgeMod.end())
// 			{
// 				mapEdgeMod[Key] = ValForEdgeMod();
// 				itrFind = mapEdgeMod.find(Key);;
//
// 				CArray<T_POINT3D, T_POINT3D&> aAddPnt;
// 				AppendDivPnt(aaEdge[nSlabIdx].aAr[j], dSlabDivSize, FALSE, aAddPnt);
//
// 				for (int k = 0; k < aAddPnt.GetSize(); k++)
// 				{
// 					itrFind->second.aAddPnt.push_back(aAddPnt[k]);
// 				}
// 			}
// 		}
// 	}

    // 추가점 sorting
    std::vector<PosForDirSort> aPosForSort;
    PosForDirSort EachPntForSort;
    double dDir[3];
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            auto itrFind = mapEdgeMod.find(KeyForEdgeMod(i, j));
            if ( itrFind != mapEdgeMod.end() )
            {
                GetDir(aaEdge[i].aAr[j].p1, aaEdge[i].aAr[j].p2, dDir);

                std::vector<AddPntInfo>& aAddPntInfo = itrFind->second.aAddPnt;

                aPosForSort.clear();
                for ( int k = 0; k < aAddPntInfo.size(); k++ )
                {
                    EachPntForSort.PosInfo = aAddPntInfo[k];
                    EachPntForSort.dSortDir[0] = dDir[0];
                    EachPntForSort.dSortDir[1] = dDir[1];
                    aPosForSort.push_back(EachPntForSort);
                }
                std::sort(aPosForSort.begin(), aPosForSort.end()); // dDir 방향으로 Sorting

                for ( int k = 0; k < aAddPntInfo.size(); k++ )
                {
                    aAddPntInfo[k] = aPosForSort[k].PosInfo;
                }
            }
        }
    }

    // 추가점 적용
    T_LINE_FOR_MESH_G EdgeTemp;
    CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdgeMod;
    BOOL bNoDivWhenMesh = FALSE;
    aaEdgeMod.SetSize(aaEdge.GetSize());
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            auto itrFind = mapEdgeMod.find(KeyForEdgeMod(i, j));
            if ( itrFind != mapEdgeMod.end() )
            {
                std::vector<AddPntInfo>& FindVal = itrFind->second.aAddPnt;
                EdgeTemp.p1 = aaEdge[i].aAr[j].p1;
                for ( int k = 0; k < FindVal.size(); k++ )
                {
                    EdgeTemp.p2 = FindVal[k].Pnt;
                    bNoDivWhenMesh = FALSE;
                    for ( int ii = 0; ii < FindVal[k].aPntWithSameEdge.size(); ii++ )
                    {
                        if ( EdgeTemp.p1.IsSame(FindVal[k].aPntWithSameEdge[ii], 1.e-8) )
                        {
                            bNoDivWhenMesh = TRUE;
                        }
                    }
                    EdgeTemp.bNoDivWhenMesh = bNoDivWhenMesh;
                    aaEdgeMod[i].aAr.Add(EdgeTemp);
                    EdgeTemp.p1 = FindVal[k].Pnt;
                }
                EdgeTemp.p2 = aaEdge[i].aAr[j].p2;
                bNoDivWhenMesh = FALSE;
                for ( int ii = 0; ii < FindVal[FindVal.size() - 1].aPntWithSameEdge.size(); ii++ )
                {
                    if ( EdgeTemp.p1.IsSame(FindVal[FindVal.size() - 1].aPntWithSameEdge[ii], 1.e-8) )
                    {
                        bNoDivWhenMesh = TRUE;
                    }
                }
                EdgeTemp.bNoDivWhenMesh = bNoDivWhenMesh;
                aaEdgeMod[i].aAr.Add(EdgeTemp);
            }
            else
            {
                aaEdgeMod[i].aAr.Add(aaEdge[i].aAr[j]);
                aaEdgeMod[i].aAr[aaEdgeMod[i].aAr.GetSize() - 1].bNoDivWhenMesh = FALSE;
            }
        }
    }

    ASSERT(aaEdge.GetSize() == aaEdgeMod.GetSize());

    const double dTol = 1.e-6;

    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        aaEdge[i].aAr.RemoveAll();
        for ( int j = 0; j < aaEdgeMod[i].aAr.GetSize(); j++ )
        {
            if ( abs(aaEdgeMod[i].aAr[j].GetLineLength()) < dTol )
            {
                continue;
            }

            aaEdge[i].aAr.Add(aaEdgeMod[i].aAr[j]);
        }
    }

    // 절점 Merge
    BOOL bDoNewAlgorithm = FALSE;
    if ( nStype == D_SECT_TYPE_STLG_MCELL )
    {
        bDoNewAlgorithm = TRUE;
    }

    if ( bDoNewAlgorithm )
    {
        // 무조건 ConvertSectShape2EdgeNew 를 불러도 되지만..
        // 단면쪽은 실수하면 망하니깐.. 일단 중국에서만 쓰는 MCell 에서만 개선 코드를 쓰도록 넣음..
        // 향후 단면쪽 큰 변화가 생길 때 항상 이곳만 불리도록 하길 추천 함
        SetSimilarPntToSameNew(aaEdge);
    }
    else
    {
        for ( int i = 0; i < aaEdge.GetSize(); i++ )
        {
            for ( int j = 0; j < aaEdge.GetSize(); j++ )
            {
                if ( i == j ) continue;
                SetSimilarPntToSame(aaEdge[i].aAr, aaEdge[j].aAr);
            }
        }
    }

    return TRUE;
}
void CSimpleMesh::SetSimilarPntToSameNew(CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge)
{
    double ptMin[3];
    double ptMax[3];
    GetMinMaxPos(ptMin, ptMax, aaEdge);
    CGridSet FindNearest(ptMin, ptMax, 50);

    //  double dDist;
    const double dTol = 1.e-6;

    double dPos[3];
    int nCnt=0;
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            T_LINE_FOR_MESH_G& EachEdge = aaEdge[i].aAr[j];

            dPos[0] = EachEdge.p1.x;
            dPos[1] = EachEdge.p1.y;
            dPos[2] = EachEdge.p1.z;
            FindNearest.Insert(dPos, ++nCnt);

            dPos[0] = EachEdge.p2.x;
            dPos[1] = EachEdge.p2.y;
            dPos[2] = EachEdge.p2.z;
            FindNearest.Insert(dPos, ++nCnt);
        }
    }

    // dTol 거리 내의 Node 들은 다같이 붙어 있을거라는 가정하에.. FindWithinTol 에서 나온 첫번째로 수정함( 만약에 가정이 항상 만족하지 않더라도 적어도 절점 공유 하는 Node 에 대해서는 이 조건이 만족 될 것 같으니 문제 없을 것 같음 )
    std::list<std::pair<int, T_POINT3D>> lstNear;
    nCnt = 0;
    int nMax;
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            T_LINE_FOR_MESH_G& EachEdge = aaEdge[i].aAr[j];

            nCnt++;
            dPos[0] = EachEdge.p1.x;
            dPos[1] = EachEdge.p1.y;
            dPos[2] = EachEdge.p1.z;
            FindNearest.FindWithinTol(dPos, dTol, lstNear);
            if ( lstNear.size() > 1 )
            {
                // 속도 개선 전 코드와 동일한 연산이 수행 되게 하기 위해 이런 짓을 함.. 단면 강성이 바뀌면 힘들어지니깐..
                nMax = INT_MIN;
                for ( auto EachItem : lstNear )
                {
                    if ( nMax < EachItem.first )
                    {
                        nMax = EachItem.first;
                        EachEdge.p1 = EachItem.second;
                    }
                }
                if ( nMax != nCnt )
                {
                    // Modify
                    FindNearest.Delete(dPos, nCnt);
                    dPos[0] = EachEdge.p1.x;
                    dPos[1] = EachEdge.p1.y;
                    dPos[2] = EachEdge.p1.z;
                    FindNearest.Insert(dPos, nCnt);
                }
            }

            nCnt++;
            dPos[0] = EachEdge.p2.x;
            dPos[1] = EachEdge.p2.y;
            dPos[2] = EachEdge.p2.z;
            FindNearest.FindWithinTol(dPos, dTol, lstNear);
            if ( lstNear.size() > 1 )
            {
                // 속도 개선 전 코드와 동일한 연산이 수행 되게 하기 위해 이런 짓을 함.. 단면 강성이 바뀌면 힘들어지니깐..
                nMax = INT_MIN;
                for ( auto EachItem : lstNear )
                {
                    if ( nMax < EachItem.first )
                    {
                        nMax = EachItem.first;
                        EachEdge.p2 = EachItem.second;
                    }
                }

                if ( nMax != nCnt )
                {
                    // Modify
                    FindNearest.Delete(dPos, nCnt);
                    dPos[0] = EachEdge.p2.x;
                    dPos[1] = EachEdge.p2.y;
                    dPos[2] = EachEdge.p2.z;
                    FindNearest.Insert(dPos, nCnt);
                }
            }
        }
    }
}

void CSimpleMesh::SetSimilarPntToSame(CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge1, CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge2)
{
    //  double dDist;
    const double dTol = 1.e-6;
    for ( int i = 0; i < aEdge1.GetSize(); i++ )
    {
        for ( int j = 0; j < aEdge2.GetSize(); j++ )
        {
            if ( aEdge1[i].p1.IsSame(aEdge2[j].p1, dTol) )
            {
                aEdge1[i].p1 = aEdge2[j].p1;
            }
            if ( aEdge1[i].p1.IsSame(aEdge2[j].p2, dTol) )
            {
                aEdge1[i].p1 = aEdge2[j].p2;
            }
            if ( aEdge1[i].p2.IsSame(aEdge2[j].p1, dTol) )
            {
                aEdge1[i].p2 = aEdge2[j].p1;
            }
            if ( aEdge1[i].p2.IsSame(aEdge2[j].p2, dTol) )
            {
                aEdge1[i].p2 = aEdge2[j].p2;
            }
        }
    }
}

void CSimpleMesh::GetMinMaxPos(double ptMin[3], double ptMax[3], CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D &> &aaEdge)
{
    ptMin[0] = ptMin[1] = ptMin[2] = DBL_MAX;
    ptMax[0] = ptMax[1] = ptMax[2] = -DBL_MAX;

    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        for ( int j = 0; j < aaEdge[i].aAr.GetSize(); j++ )
        {
            T_LINE_FOR_MESH_G& EachEdge = aaEdge[i].aAr[j];
            if ( ptMin[0] > EachEdge.p1.x )
            {
                ptMin[0] = EachEdge.p1.x;
            }
            if ( ptMin[1] > EachEdge.p1.y )
            {
                ptMin[1] = EachEdge.p1.y;
            }
            if ( ptMin[2] > EachEdge.p1.z )
            {
                ptMin[2] = EachEdge.p1.z;
            }

            if ( ptMax[0] < EachEdge.p1.x )
            {
                ptMax[0] = EachEdge.p1.x;
            }
            if ( ptMax[1] < EachEdge.p1.y )
            {
                ptMax[1] = EachEdge.p1.y;
            }
            if ( ptMax[2] < EachEdge.p1.z )
            {
                ptMax[2] = EachEdge.p1.z;
            }

            if ( ptMin[0] > EachEdge.p2.x )
            {
                ptMin[0] = EachEdge.p2.x;
            }
            if ( ptMin[1] > EachEdge.p2.y )
            {
                ptMin[1] = EachEdge.p2.y;
            }
            if ( ptMin[2] > EachEdge.p2.z )
            {
                ptMin[2] = EachEdge.p2.z;
            }

            if ( ptMax[0] < EachEdge.p2.x )
            {
                ptMax[0] = EachEdge.p2.x;
            }
            if ( ptMax[1] < EachEdge.p2.y )
            {
                ptMax[1] = EachEdge.p2.y;
            }
            if ( ptMax[2] < EachEdge.p2.z )
            {
                ptMax[2] = EachEdge.p2.z;
            }
        }
    }
}

struct KeyForMeshPos : public StructKeyForStdMap
{
    KeyForMeshPos() : StructKeyForStdMap()
    {
    }
    KeyForMeshPos(CString SName, int nStype, int nTypeBeforeAfter, int nAdjustCoordToCentroid, int nAllSectMesh,
        T_SECT_SECTBASE_D& SectBase) : StructKeyForStdMap()
    {
        aStr.push_back(SName);
        aInt.push_back(nStype);
        aInt.push_back(nTypeBeforeAfter);
        aInt.push_back(nAdjustCoordToCentroid);
        aInt.push_back(nAllSectMesh);
        aDbl.push_back(SectBase.Stiffness.Area);
        aDbl.push_back(SectBase.Stiffness.Rxx);
        aDbl.push_back(SectBase.Stiffness.Ryy);
        aDbl.push_back(SectBase.Stiffness.Rzz);
        aDbl.push_back(SectBase.Stiffness.Cyp);
        aDbl.push_back(SectBase.Stiffness.Cym);
        aDbl.push_back(SectBase.Stiffness.Czp);
        aDbl.push_back(SectBase.Stiffness.Czm);
        aDbl.push_back(SectBase.PeriIn);
        aDbl.push_back(SectBase.PeriOut);
        for ( int i=0; i<D_SECT_SIZE_NUM_MAX; i++ )
        {
            aDbl.push_back(SectBase.Size[i]);
        }

		for(int i=0; i< SectBase.Stiffener.aStiffLeft.GetSize(); i++)
		{
			aInt.push_back(SectBase.Stiffener.aStiffLeft[i].nShape);
			aInt.push_back(SectBase.Stiffener.aStiffLeft[i].bCalc? 1 : 0);
			aDbl.push_back(SectBase.Stiffener.aStiffLeft[i].dSpacing);
		}
		for (int i = 0; i < SectBase.Stiffener.aStiffRight.GetSize(); i++)
		{
			aInt.push_back(SectBase.Stiffener.aStiffRight[i].nShape);
			aInt.push_back(SectBase.Stiffener.aStiffRight[i].bCalc ? 1 : 0);
			aDbl.push_back(SectBase.Stiffener.aStiffRight[i].dSpacing);
		}


    }
};

struct ValForMeshPos
{
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> raMeshElemD;
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> raMeshNodeD;
	std::vector<int> raDomainElemCount;
    CArray<int, int> raDomainMaterialType;

    ValForMeshPos()
    {
        Initialize();
    }
    ValForMeshPos(ValForMeshPos& src) { *this = src; }
    ValForMeshPos& operator=(const ValForMeshPos& src)
    {
        raMeshElemD.Copy(src.raMeshElemD);
        raMeshNodeD.Copy(src.raMeshNodeD);
		std::copy(src.raDomainElemCount.begin(), src.raDomainElemCount.end(), std::back_inserter(raDomainElemCount));
        raDomainMaterialType.Copy(src.raDomainMaterialType);

        return *this;
    }
    void Initialize()
    {
        raMeshElemD.RemoveAll();
        raMeshNodeD.RemoveAll();
        raDomainElemCount.clear();
        raDomainMaterialType.RemoveAll();
    }
    void Set(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD,
        std::vector<int>& aDomainElemCount, CArray<int, int>& aDomainMaterialType)
    {
        raMeshElemD.Copy(aMeshElemD);
        raMeshNodeD.Copy(aMeshNodeD);
		std::copy(aDomainElemCount.begin(), aDomainElemCount.end(), std::back_inserter(raDomainElemCount));
        raDomainMaterialType.Copy(aDomainMaterialType);
    }
};

static std::map<KeyForMeshPos, ValForMeshPos> m_mapMeshPosData;

// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
// @param bMerge : 여러개의 domain으로 되어 있는 Mesh를 중복점을 제거해서 한개 domain으로 Merge (ex. Stiffener가 있는 경우)
// 필독!! 여기를 고치면 CSimpleMesh::GetDomainMaterialType_Composite도 잘 동작하는지 꼭 확인해야 합니다!!!!
BOOL CSimpleMesh::GetSectMesh(T_SECT_D& SectD, int nPos, int nTypeBeforeAfter,
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
    std::vector<int>& raDomainElemCount,
    CArray<int, int>& raDomainMaterialType,
    BOOL bAdjustCoordToCentroid,
    T_SECT_DESIGN* pSectDesign)
{
    double dStartTime = clock();

    raMeshElemD.RemoveAll();
    raMeshNodeD.RemoveAll();
    raDomainElemCount.clear();
    raDomainMaterialType.RemoveAll();

    //////////////////////////////////////////////////////////////////////////
    int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    BOOL bAfter = FALSE;
    BOOL bComposite = ( CDBLib::IsCompositeSect(SectD.nStype) ||
        ( SectD.nStype == D_SECT_TYPE_TAPERED && CDBLib::IsCompositeSect(SectD.SectBefore.nStype) ) ) ? TRUE : FALSE;
    if ( SectD.nStype == D_SECT_TYPE_CONSTRUCTION || bComposite ) bAfter = TRUE;

    T_SECT_SECTBASE_D SectBase; SectBase.Initialize();
    if ( nTypeBeforeAfter == 11 || nTypeBeforeAfter == 12 )
    {
        SectBase = SectD.SectBefore.SectI;
    }
    else
    {
        if ( !bAfter && nPos==0 ) SectBase.Stiffness = SectD.SectBefore.SectI.Stiffness;
        else if ( !bAfter && nPos==1 ) SectBase.Stiffness = SectD.SectBefore.SectJ.Stiffness;
        else if ( bAfter && nPos==0 ) SectBase.Stiffness = SectD.SectAfter.SectI.Stiffness;
        else if ( bAfter && nPos==1 ) SectBase.Stiffness = SectD.SectAfter.SectJ.Stiffness;
        char chIJ= ( nPos==0 ) ? 'I' : 'J';
        //     SectBase.Stiffness =  *CSectUtil::GetProperStiffPosition(SectD, bAfter, 0, chIJ);
        if ( bAfter )
        {
            if ( nPos==0 ) memcpy(SectBase.Size, SectD.SectBefore.SectI.Size, sizeof(SectBase.Size));
            else        memcpy(SectBase.Size, SectD.SectBefore.SectJ.Size, sizeof(SectBase.Size));
        }

        SectBase.PeriIn    =  *CSectUtil::GetStandForPeriIn(SectD, bAfter, 0, chIJ);
        SectBase.PeriOut   =  *CSectUtil::GetStandForPeriOut(SectD, bAfter, 0, chIJ);
        SectBase.Size[D_SECT_SIZE_NUM_MAX-3]=SectD.SectAfter.SectJ.Size[2]; // B
        SectBase.Size[D_SECT_SIZE_NUM_MAX-2]=SectD.SectAfter.SectJ.Size[2]; // tc
        SectBase.Size[D_SECT_SIZE_NUM_MAX-1]=SectD.SectAfter.SectJ.Size[2]; // Hh
		SectBase.Stiffener = SectD.SectBefore.SectI.Stiffener;
    }

    CString SName = SectD.SName;
    int nAdjustCoordToCentroid = bAdjustCoordToCentroid ? 1 : 0;
    int nAllSectMesh = CDb_DllBaseManager::m_bAllSectMesh? 1 : 0;
    KeyForMeshPos Key(SName, nStype, nTypeBeforeAfter, nAdjustCoordToCentroid, nAllSectMesh, SectBase);
    //////////////////////////////////////////////////////////////////////////

    auto itrFind = m_mapMeshPosData.find(Key);
    if ( itrFind == m_mapMeshPosData.end() || SectD.bPricipalAxis )
    { // 없으면
        BOOL bCheck = FALSE;
        // SectTest
        if ( CDb_DllBaseManager::m_bAllSectMesh )
        {
            if ( ( CSimpleMesh::IsComposite(SectD) || CSectDB::IsSectStlGirder(SectD) ) && // SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB
                !(/*nStype == D_SECT_TYPE_COMPO_PC ||*/ nStype == D_SECT_TYPE_COMPO_G ) ) bCheck = TRUE;
            if(CSectDB::IsSectDumbAndStub(SectD)) bCheck = TRUE;
        }
        else
        {
            if ( ( CSimpleMesh::IsComposite(SectD) || CSectDB::IsSectStlGirder(SectD) ) && // SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB
                !( nStype == D_SECT_TYPE_COMPO_PC || nStype == D_SECT_TYPE_COMPO_G ) ) bCheck = TRUE;
        }

        if ( bCheck )
        {
            // Stiffener가 달려있거나 CompWeb같은 단면은 단면을 등분하여 각각 Mesh한 다음 Merge한다.
            if ( !GetSectMergeMesh(SectD, nPos, nTypeBeforeAfter, raMeshElemD, raMeshNodeD, raDomainMaterialType, raDomainElemCount) ) return FALSE;
        }
        else
        {
            if ( !GetSectRegularMesh(nPos, SectD, raDomainElemCount, raMeshElemD, raMeshNodeD, raDomainMaterialType, nTypeBeforeAfter) ) return FALSE;
        }

        if ( bAdjustCoordToCentroid )
        {
            //-----------------------------------------------------------------------------------------------
            // MNET:XXXX-HSSHIM-20130816.
            // part별 속성 계산시, 좌표를 도심 기준 좌표로 변경. (Composite Sect for CS 단면 속성 계산시 이전에 이렇게 하고 있어서 기존 방식을 맞춰주기 위함.)
            if ( !AdjustCoordToCentroid(nTypeBeforeAfter, raMeshNodeD, pSectDesign) ) { ASSERT(0); return FALSE; } // 이거 하는거랑 안하는거랑 값 차이가 없는것 같은데 확인하고 안쓰도록 해버리자.
            //-----------------------------------------------------------------------------------------------
        }

        if ( !SectD.bPricipalAxis )
        {
            // Principal Axis 에서는 Memoization 사용하지 않도록
            ValForMeshPos Val;
            Val.Set(raMeshElemD, raMeshNodeD, raDomainElemCount, raDomainMaterialType);
            m_mapMeshPosData[Key] = Val;
        }
    }
    else
    {
        ValForMeshPos& Val = itrFind->second;

        raMeshElemD.Copy(Val.raMeshElemD);
        raMeshNodeD.Copy(Val.raMeshNodeD);
		std::copy(Val.raDomainElemCount.begin(), Val.raDomainElemCount.end(), std::back_inserter(raDomainElemCount));
        raDomainMaterialType.Copy(Val.raDomainMaterialType);
    }

    //--------------------------------------------------------------
    // Mesh 검증용 Node 생성. (Section대화상자에서 Property 버튼을 눌렀을 때를 제외한 상황에서는 트렌젝션이 꼬일 수 있음)
    BOOL bMakeNodeOfMesh = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFSectMeshToNode"), _T("yes")) )
    {
        if ( nTypeBeforeAfter == 11 || nTypeBeforeAfter == 12 || !CSimpleMesh::IsComposite(SectD)|| nTypeBeforeAfter == 2 || nTypeBeforeAfter == 0 )
        {
            bMakeNodeOfMesh = TRUE;
        }
    }
    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFSectMeshToNodeOnlyBefore"), _T("yes")) )
    {
        if ( nTypeBeforeAfter == 11 || nTypeBeforeAfter == 1 )
        {
            bMakeNodeOfMesh = TRUE;
        }
    }

    if ( bMakeNodeOfMesh )
    {
        WriteNodeOfMesh(raMeshElemD, raMeshNodeD);
    }
    //--------------------------------------------------------------

    double dTime = ( clock() - dStartTime )/CLOCKS_PER_SEC;

    return TRUE;
}

// 수정전
// BOOL CSimpleMesh::GetSectMesh(T_SECT_D& SectD, int nPos, int nTypeBeforeAfter,
//                               CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&>& raMeshElemD,
//                               CArray<T_MESH_NODE_D,T_MESH_NODE_D&>& raMeshNodeD,
//                               CArray<int,int>& raDomainElemCount,
//                               CArray<int,int>& raDomainMaterialType,
//                               BOOL bAdjustCoordToCentroid,
//                               T_SECT_DESIGN* pSectDesign)
// {
//   raMeshElemD.RemoveAll();
//   raMeshNodeD.RemoveAll();
//   raDomainElemCount.RemoveAll();
//   raDomainMaterialType.RemoveAll();
//
//   int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;
//
//   if((CSimpleMesh::IsComposite(SectD) || CSectDB::IsSectStlGirder(SectD)) && // SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB
//     !(nStype == D_SECT_TYPE_COMPO_PC || nStype == D_SECT_TYPE_COMPO_G))
//   {
//     // Stiffener가 달려있거나 CompWeb같은 단면은 단면을 등분하여 각각 Mesh한 다음 Merge한다.
//     if(!GetSectMergeMesh(SectD, nPos, nTypeBeforeAfter, raMeshElemD, raMeshNodeD, raDomainMaterialType, raDomainElemCount)) return FALSE;
//   }
//   else
//   {
//     if(!GetSectRegularMesh(nPos, SectD, raDomainElemCount, raMeshElemD, raMeshNodeD, raDomainMaterialType, nTypeBeforeAfter)) return FALSE;
//   }
//
//   if(bAdjustCoordToCentroid)
//   {
//     //-----------------------------------------------------------------------------------------------
//     // MNET:XXXX-HSSHIM-20130816.
//     // part별 속성 계산시, 좌표를 도심 기준 좌표로 변경. (Composite Sect for CS 단면 속성 계산시 이전에 이렇게 하고 있어서 기존 방식을 맞춰주기 위함.)
//     if (!AdjustCoordToCentroid(nTypeBeforeAfter, raMeshNodeD, pSectDesign))  { ASSERT(0); return FALSE; } // 이거 하는거랑 안하는거랑 값 차이가 없는것 같은데 확인하고 안쓰도록 해버리자.
//     //-----------------------------------------------------------------------------------------------
//   }
//
//   //--------------------------------------------------------------
//   // Mesh 검증용 Node 생성. (Section대화상자에서 Property 버튼을 눌렀을 때를 제외한 상황에서는 트렌젝션이 꼬일 수 있음)
//   BOOL bMakeNodeOfMesh = FALSE;
//   if (CTestEnvMgr::IsTestEnvST(_T("7thDOFSectMeshToNode"), _T("yes")))
//   {
//     if(nTypeBeforeAfter == 11 || nTypeBeforeAfter == 12 || !CSimpleMesh::IsComposite(SectD)|| nTypeBeforeAfter == 2 || nTypeBeforeAfter == 0)
//     {
//       bMakeNodeOfMesh = TRUE;
//     }
//   }
//   if (CTestEnvMgr::IsTestEnvST(_T("7thDOFSectMeshToNodeOnlyBefore"), _T("yes")))
//   {
//     if(nTypeBeforeAfter == 11 || nTypeBeforeAfter == 1)
//     {
//       bMakeNodeOfMesh = TRUE;
//     }
//   }
//
//
//   if(bMakeNodeOfMesh)
//   {
//     WriteNodeOfMesh(raMeshElemD, raMeshNodeD);
//   }
//   //--------------------------------------------------------------
//
//   return TRUE;
// }

BOOL CSimpleMesh::GetSectPolygon(T_SECT_D &SectD, int nPos, CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&>& aaEdge, int nTypeBeforeAfter/*=0*/)
{
    // Composite StlgMCell 단면이 아니면 aaEdge의 Size는 1개 이며..
    // Composite 단면일 경우.. aaEdge[0]이 Slab이고 aaEdge[1]이 Girder이고 aaEdge[2]이후부터는 Stiffener이다... aPlaneProperty의 값에 의해 분류된다..

	// nTypeBeforeAfter == 3 이면 tfp를 고려한 mesh 생성.
	if (nTypeBeforeAfter != 3)
	{
		if (SectD.nStype == D_SECT_TYPE_COMPO_TUB || (SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB))
		{
			SectD.SectBefore.SectI.Size[9] = 0.0; // tfp
			SectD.CmpTapJ.Size[9]          = 0.0; // tfp-J
		}
		else if (SectD.nStype == D_SECT_TYPE_COMPO_STLG_TUB || (SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB))
		{
			SectD.SectBefore.SectI.Size[13] = 0.0; // tfp
			SectD.CmpTapJ.Size[13]          = 0.0; // tfp-J
		}
	}

    if (SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_DUMB)
    {
        if (nTypeBeforeAfter == 2 || nTypeBeforeAfter == 22)
        {
            // 내부 hole 무시
			SectD.SectBefore.SectI.Size[4] = SectD.SectBefore.SectI.Size[2]/2.0; // P1
			SectD.SectBefore.SectI.Size[5] = SectD.SectBefore.SectI.Size[3]/2.0; // P2
        }
        else if (nTypeBeforeAfter == 21)
        {  
            // P2  hole 무시
            SectD.SectBefore.SectI.Size[5] = SectD.SectBefore.SectI.Size[3] / 2.0; // P2
        }
    }

    // 단면 형상 얻어오기.
    int nNumPlane = 0;
    CArray<int, int> aPlaneVCount, aPlaneVertexIndex;
    CArray<BOOL, BOOL> aPlaneVacant;
    CArray<double, double> aVertex;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    double dShapeRatioFromI = 0.5;    // tapered composite의 경우 I/J단 단면을 정확히 얻어오기 위해 I단의로부터의 거리비를 넘김.
    if ( nPos == 0 ) dShapeRatioFromI = 0.0;
    if ( nPos == 1 ) dShapeRatioFromI = 1.0;

    int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    CArray<int, int> aPlaneProperty;
    if ( !pDoc->m_pSectDB->makeSectShape(SectD, nNumPlane, &aPlaneVCount, &aPlaneVacant, &aPlaneVertexIndex, &aVertex, dShapeRatioFromI, &aPlaneProperty) )
    {
        ASSERT(0);
        return FALSE;
    }

    //////////////////////////////////////////////////////////////////////////
    // Test
// 	CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&> aNode_simple;   aNode_simple.RemoveAll();
// 	CArray<T_SIMPLE_MESH_EDGE_D, T_SIMPLE_MESH_EDGE_D&> aEdge_simple;   aEdge_simple.RemoveAll();
// 	ConvertSectShape2Edge(aPlaneVCount, aPlaneVertexIndex, aVertex, aNode_simple, aEdge_simple);
// 	if (aNode_simple.GetSize() <= 3 || aEdge_simple.GetSize() <= 3) { ASSERT(0);  return FALSE; }
//
// 	// 단면 형상 메쉬
// 	T_POINT3D ptStart, ptEnd;
// 	T_LINE_FOR_MESH_G LineG;
// 	CArray<T_LINE_FOR_MESH_G,T_LINE_FOR_MESH_G&> aEdgeTemp;   aEdgeTemp.RemoveAll();
//
// 	for (int i=0; i<aEdge_simple.GetSize(); ++i)
// 	{
// 		T_SIMPLE_MESH_NODE_D& node1 = aNode_simple[aEdge_simple[i].iNode1 -1];
// 		T_SIMPLE_MESH_NODE_D& node2 = aNode_simple[aEdge_simple[i].iNode2 -1];
// 		ptStart.Set(node1.y, node1.z, 0.0);
// 		ptEnd.Set(node2.y, node2.z, 0.0);
// 		LineG.Set(ptStart, ptEnd);
//
// 		aEdgeTemp.Add(LineG);
// 	}
    //////////////////////////////////////////////////////////////////////////
    if (CSectDB::IsSectDumbAndStub(SectD) && nTypeBeforeAfter==1)
    {
//         for (int i = 0; i < aPlaneProperty.GetSize(); i++)
//         {
//             if (aPlaneProperty[i] == 0) aPlaneProperty[i] = -1;
//         }
    }


    BOOL bExistGap = FALSE;

    if ( aPlaneProperty.GetSize() == 0 )
    {
        ASSERT(0); // 이상황은 CSimpleMesh::GetSectRegularMesh() 여기 들어와야 함
        //T_ArEachPlanInfo_D aEdge;
        //if(!GetSectPolygonBySectShape(aPlaneVCount, aPlaneVertexIndex, aVertex, SectD, aEdge.aAr)) return FALSE;
        //aaEdge.Add(aEdge);
    }
    else
    {
        std::map<int, T_EachPlanInfo> mapPlanePropertyToInfo;
        GroupingPlanInfoByProp(aPlaneProperty, aPlaneVCount, aPlaneVertexIndex, aVertex, mapPlanePropertyToInfo);

        auto itrFind = mapPlanePropertyToInfo.find(-1); //  Gap
        if ( itrFind != mapPlanePropertyToInfo.end() ) bExistGap = TRUE;

        aaEdge.RemoveAll();

        if ( pDoc->m_pSectDB->IsSectStlGirder(SectD) )
        {
            T_ArEachPlanInfo_D aEdge;
            aaEdge.Add(aEdge); // StlGirder 단면은 Slab Edge에 0개의 Data를 입력해서 Slab가 없는 Composite단면인것 처럼 계산함
        }

        int nMaxPropNo = INT_MIN;
        for ( auto itrMap = mapPlanePropertyToInfo.begin(); itrMap != mapPlanePropertyToInfo.end(); itrMap++ )
        {
            if ( itrMap->first > nMaxPropNo )
            {
                nMaxPropNo = itrMap->first;
            }
        }

        for ( auto itrMap = mapPlanePropertyToInfo.begin(); itrMap != mapPlanePropertyToInfo.end(); itrMap++ )
        {
            if ( !CDb_DllBaseManager::m_bAllSectMesh && itrMap->first == -1 ) continue; // Gap 영역 무시

            int nPolyNum = 0;
            CArray<int, int> aPolyVCount;
            CArray<int, int> aPolyVertextIndex;
            CArray<int, int> aLineI, aLineJ;

            // hslee : makeClosedPolygon함수는 CSectDB::makeSectShape 안에서 불러야 하는데 Property별로 정리하면서 구현하기 힘들어서 밖으로 뺏음..
            // 현재 있는 노드 중 가장 처음 것을 가져와서 폴리라인 형성
            if ( !pDoc->m_pSectDB->makeClosedPolygon(&( itrMap->second.aEachPlaneVertexIndex ), &( itrMap->second.aEachVertex ), nPolyNum, &aPolyVCount, &aPolyVertextIndex) )
                ASSERT(0);

            int rNumPlane = nPolyNum;
            itrMap->second.aEachPlaneVCount.Copy(aPolyVCount);
            itrMap->second.aEachPlaneVertexIndex.Copy(aPolyVertextIndex);

            // 폴리곤의 중공여부 정보에 대한 변수
            // 현재는 아무것도 하지 않는다. 추후 반영예정
            //pPlaneVacant->SetSize(rNumPlane);
            //for (i=0; i<rNumPlane; i++) pPlaneVacant->SetAt(i, FALSE);

            BOOL bStiff = ( nMaxPropNo - GetCompSectStifSize(SectD) ) < itrMap->first; // 제일 뒤에 Stiffner 가 모여 있는 정보를 이용하여 Stiffner 인 지 구분 함

            T_ArEachPlanInfo_D aEdge;
            if ( nStype == D_SECT_TYPE_STLG_MCELL)
            {
                // 속도개선을 위한 코드임
                // 단면 전체에 적용해도 되는 코드이지만.. 혹시라도 실수하면 큰일 나니깐 문제가 발생한 MCELL 단면에만 개선 코드를 적용 함
                // 향후 단면 전체에 영향을 주는 큰 수정이 있을 때 개선코드도 같이 열어 주길..
                if ( !GetSectPolygonBySectShape(itrMap->second.aEachPlaneVCount, itrMap->second.aEachPlaneVertexIndex, itrMap->second.aEachVertex, SectD, TRUE, bStiff, aEdge.aAr) ) return FALSE;
            }
            else
            {
                if ( !GetSectPolygonBySectShape(itrMap->second.aEachPlaneVCount, itrMap->second.aEachPlaneVertexIndex, itrMap->second.aEachVertex, SectD, FALSE, bStiff, aEdge.aAr) ) return FALSE;
            }
            aaEdge.Add(aEdge);
        }
    }

    // Gap 영역을 마지막에 세팅
    if ( CDb_DllBaseManager::m_bAllSectMesh && IsCompositeSlabGapMesh(SectD) )
    {
        if ( CSectDB::IsSectComposite(SectD) && aaEdge.GetSize()>2 && bExistGap )
        {
            CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdgeTemp;
            for ( int i=1; i<aaEdge.GetSize(); i++ )
            {
                aaEdgeTemp.Add(aaEdge[i]);
            }
            aaEdgeTemp.Add(aaEdge[0]);

            aaEdge.RemoveAll();
            aaEdge.Copy(aaEdgeTemp);

            // 			CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdgeTemp;
            // 			aaEdgeTemp.Add(aaEdge[1]);
            // 			aaEdgeTemp.Add(aaEdge[2]);
            // 			aaEdgeTemp.Add(aaEdge[0]);
            //
            // 			aaEdge.RemoveAll();
            // 			aaEdge.Copy(aaEdgeTemp);
        }
    }
    else if (CSectDB::IsSectDumbAndStub(SectD))
    {
        // hole, conc, steel 순서를 steel, conc, hole로 변경

		CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdgeTemp;
        for (int i = aaEdge.GetSize() - 1; i >= 0; i--)
		{
			aaEdgeTemp.Add(aaEdge[i]);
		}

		aaEdge.RemoveAll();
		aaEdge.Copy(aaEdgeTemp);
    }

    return TRUE;
}

BOOL CSimpleMesh::GetSectMeshByPolygon(T_SECT_D &SectD, int nTypeBeforeAfter,
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge,
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeAll,
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
    std::vector<int>& raDomainElemCount,
    CArray<int, int>& raDomainMaterialType,
    BOOL bMerge/*=TRUE*/)
{
    // SectTest
    if ( CDb_DllBaseManager::m_bAllSectMesh )
    {
        if ( GetSectMeshByPolygon_AllSect(SectD, nTypeBeforeAfter, aEdge, aEdgeAll, raMeshElemD, raMeshNodeD, raDomainElemCount, raDomainMaterialType, bMerge) )
            return TRUE;
        else
            return FALSE;
    }

    raDomainElemCount.clear();

    int nCompShapeType;
    if ( nTypeBeforeAfter == 12 ) // Part2
    {
        nCompShapeType = 0; // Slab
    }
    else if ( nTypeBeforeAfter == 11 ) // Part1
    {
        nCompShapeType = 1; // Girder
    }
    else if ( nTypeBeforeAfter == 20 ) // Stiffner
    {
        nCompShapeType = 2; // Stiffner
    }
    else
    {
        nCompShapeType = 1; // Composite이 아닌경우.. 혹은 Girder와 같은 Size로 Mesh하면 되는 경우
    }

    int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;
    if ( nStype == D_SECT_TYPE_STLG_MCELL )
    {
        nCompShapeType = 1; // Girder
    }

    if ( CSimpleMesh::IsComposite(SectD) )
    {
        ASSERT(nTypeBeforeAfter == 12 || nTypeBeforeAfter == 11 || nTypeBeforeAfter == 20 || nStype == D_SECT_TYPE_COMPO_PC); // Composite단면은 이 함수 전에 Slab, Girder, 스티프너를 쪼개서 부름
    }

    CSimpleMesh simpleMesh;
    simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;
    if ( !simpleMesh.GenerateMesh(SectD, aEdge, aEdgeAll, raMeshElemD, raMeshNodeD, raDomainElemCount, nCompShapeType, nTypeBeforeAfter) ) { ASSERT(0); return FALSE; }

    //------------------------------------------------------------
    // SRC/composite의 경우 Domain별 재질 타입
    // 0: RC, 1: Steel, -1: Hole, -2: none
    //CArray<int,int> aDomainMaterialType; aDomainMaterialType.RemoveAll();
    raDomainMaterialType.RemoveAll();
    if ( nCompShapeType == 2 )
    {
        raDomainMaterialType.SetSize(raDomainElemCount.size());
        for ( int i = 0; i < raDomainMaterialType.GetSize(); i++ )
        {
            raDomainMaterialType[i] = 1; // Stiffener는 Steel이다
        }
    }
    else
    {
        simpleMesh.GetDomainMaterialType(raDomainMaterialType, SectD, raMeshElemD, raMeshNodeD, raDomainElemCount, nTypeBeforeAfter);
    }

    //------------------------------------------------------------
    // Hole 영역 Mesh 제거
    simpleMesh.RemoveDomainHole(SectD, raDomainMaterialType, raMeshElemD, raMeshNodeD, raDomainElemCount);

    //------------------------------------------------------------
    // 중복점 제거후 한개 domain으로 Merge
    if ( bMerge ) simpleMesh.MergeDomainMesh(SectD, raDomainMaterialType, raMeshElemD, raMeshNodeD, raDomainElemCount);

    // 요소 갯수 검증
    int nDomainElemSum = 0;
    for ( int i=0; i<raDomainElemCount.size(); ++i )  nDomainElemSum += raDomainElemCount[i];
    ASSERT(nDomainElemSum == raMeshElemD.GetSize());

    //--------------------------------------------------------------
    // Mesh 검증용 mct 생성.
    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    int iIndexBadChar = strPostFix.Find(_T("/"));
    if ( iIndexBadChar > 0 ) strPostFix = strPostFix.Left(iIndexBadChar);
    if ( bDumpSectMeshFile )  WriteMctOfMesh(raMeshElemD, raMeshNodeD, strPostFix);
    //--------------------------------------------------------------

    return TRUE;
}
BOOL CSimpleMesh::GetSectMeshByPolygon_AllSect(T_SECT_D &SectD, int nTypeBeforeAfter,
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge,
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdgeAll,
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& raMeshElemD,
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD,
	std::vector<int>& raDomainElemCount,
    CArray<int, int>& raDomainMaterialType,
    BOOL bMerge/*=TRUE*/)
{
    raDomainElemCount.clear();

    int nCompShapeType;
    if ( nTypeBeforeAfter == 12 ) // Part2
    {
        nCompShapeType = 0; // Slab
    }
    else if ( nTypeBeforeAfter == 11 ) // Part1
    {
        nCompShapeType = 1; // Girder
    }
	else if (nTypeBeforeAfter == -1)   
	{
		nCompShapeType = -1; // void
	}
    else if ( nTypeBeforeAfter == 20 ) // Stiffner
    {
        nCompShapeType = 2; // Stiffner
    }
    else if ( nTypeBeforeAfter == 13 ) // Gap
    {
        nCompShapeType = 13; //-1 이면 그부분이 제외됨
    }
    else
    {
        nCompShapeType = 1; // Composite이 아닌경우.. 혹은 Girder와 같은 Size로 Mesh하면 되는 경우
    }

    int nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;
    if ( nStype == D_SECT_TYPE_STLG_MCELL )
    {
        nCompShapeType = 1; // Girder
    }

    if ( CSimpleMesh::IsComposite(SectD) )
    {
        ASSERT(nTypeBeforeAfter == 12 || nTypeBeforeAfter == 11 || nTypeBeforeAfter == 20 || nTypeBeforeAfter == 13 ||
            nStype == D_SECT_TYPE_COMPO_CI || nStype == D_SECT_TYPE_COMPO_CT || nStype == D_SECT_TYPE_COMPO_PC); // Composite단면은 이 함수 전에 Slab, Girder, 스티프너를 쪼개서 부름
    }

    CSimpleMesh simpleMesh;
    simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;
    if ( !simpleMesh.GenerateMesh(SectD, aEdge, aEdgeAll, raMeshElemD, raMeshNodeD, raDomainElemCount, nCompShapeType, nTypeBeforeAfter) ) { ASSERT(0); return FALSE; }

    //------------------------------------------------------------
    // SRC/composite의 경우 Domain별 재질 타입
    // 0: RC, 1: Steel, -1: Hole, -2: none
    //CArray<int,int> aDomainMaterialType; aDomainMaterialType.RemoveAll();
    raDomainMaterialType.RemoveAll();
    if ( nCompShapeType == 2 )
    {
        raDomainMaterialType.SetSize(raDomainElemCount.size());
        for ( int i = 0; i < raDomainMaterialType.GetSize(); i++ )
        {
            raDomainMaterialType[i] = 1; // Stiffener는 Steel이다
        }
    }
    else
    {
        simpleMesh.GetDomainMaterialType(raDomainMaterialType, SectD, raMeshElemD, raMeshNodeD, raDomainElemCount, nTypeBeforeAfter);
    }

    //------------------------------------------------------------
    // Hole 영역 Mesh 제거
    simpleMesh.RemoveDomainHole(SectD, raDomainMaterialType, raMeshElemD, raMeshNodeD, raDomainElemCount);

    //------------------------------------------------------------
    // 중복점 제거후 한개 domain으로 Merge
    if ( IsSRC(SectD) )  bMerge = TRUE;
    if ( CSectDB::IsSectPSComposite(SectD) )  bMerge = TRUE;
    if ( CSectDB::IsSectSteelComposite(SectD) )  bMerge = TRUE;
    if ( bMerge ) simpleMesh.MergeDomainMesh(SectD, raDomainMaterialType, raMeshElemD, raMeshNodeD, raDomainElemCount);

    // 요소 갯수 검증
    int nDomainElemSum = 0;
    for ( int i=0; i<raDomainElemCount.size(); ++i )  nDomainElemSum += raDomainElemCount[i];
    ASSERT(nDomainElemSum == raMeshElemD.GetSize());

    //--------------------------------------------------------------
    // Mesh 검증용 mct 생성.
    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    int iIndexBadChar = strPostFix.Find(_T("/"));
    if ( iIndexBadChar > 0 ) strPostFix = strPostFix.Left(iIndexBadChar);
    if ( bDumpSectMeshFile )  WriteMctOfMesh(raMeshElemD, raMeshNodeD, strPostFix);
    //--------------------------------------------------------------

    return TRUE;
}

BOOL CSimpleMesh::GetSectPolygonBySectShape(CArray<int, int>& aPlaneVCount, CArray<int, int>& aPlaneVertexIndex, CArray<double, double>& aVertex, T_SECT_D &SectD, BOOL bDoNewAlgorithm, BOOL bStiff, CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> &aEdge)
{
    CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&> aNode_simple;   aNode_simple.RemoveAll();
    CArray<T_SIMPLE_MESH_EDGE_D, T_SIMPLE_MESH_EDGE_D&> aEdge_simple;   aEdge_simple.RemoveAll();
    if ( bDoNewAlgorithm )
    {
        // 무조건 ConvertSectShape2EdgeNew 를 불러도 되지만..
        // 단면쪽은 실수하면 망하니깐.. 일단 중국에서만 쓰는 MCell 에서만 개선 코드를 쓰도록 넣음..
        // 향후 단면쪽 큰 변화가 생길 때 항상 이곳만 불리도록 하길 추천 함
        if ( bStiff )
        {
            ConvertSectShape2EdgeSimple(aPlaneVCount, aPlaneVertexIndex, aVertex, aNode_simple, aEdge_simple);
        }
        else
        {
            ConvertSectShape2EdgeNew(aPlaneVCount, aPlaneVertexIndex, aVertex, aNode_simple, aEdge_simple);
        }
    }
    else
    {
        ConvertSectShape2Edge(aPlaneVCount, aPlaneVertexIndex, aVertex, aNode_simple, aEdge_simple);
    }

    if ( aNode_simple.GetSize() <= 3 || aEdge_simple.GetSize() <= 3 ) { ASSERT(0);  return FALSE; }

    //--------------------------------------------------------------
    // 형상 검증용 mct 생성.
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    int iIndexBadChar = strPostFix.Find(_T("/"));
    if ( iIndexBadChar > 0 ) strPostFix = strPostFix.Left(iIndexBadChar);

    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    if ( bDumpSectMeshFile )  WriteMctOfSect(aNode_simple, aEdge_simple, strPostFix);
    //--------------------------------------------------------------

    // 단면 형상 메쉬
    T_POINT3D ptStart, ptEnd;
    T_LINE_FOR_MESH_G LineG;

    aEdge.RemoveAll();
    for ( int i=0; i<aEdge_simple.GetSize(); ++i )
    {
        T_SIMPLE_MESH_NODE_D& node1 = aNode_simple[aEdge_simple[i].iNode1 -1];
        T_SIMPLE_MESH_NODE_D& node2 = aNode_simple[aEdge_simple[i].iNode2 -1];
        ptStart.Set(node1.y, node1.z, 0.0);
        ptEnd.Set(node2.y, node2.z, 0.0);
        LineG.Set(ptStart, ptEnd);

        aEdge.Add(LineG);
    }
    return TRUE;
}

void CSimpleMesh::GroupingPlanInfoByProp(CArray<int, int> &aPlaneProperty, CArray<int, int> &aPlaneVCount, CArray<int, int>& aPlaneVertexIndex, CArray<double, double>& aVertex, std::map<int, T_EachPlanInfo> &mapPlanePropertyToInfo)
{
    // aPlaneProperty 의 값이 0:Slab 1:Girder 2이상:Stiffener  -1:Gap
    if ( aPlaneProperty.GetSize() == 0 )
    {
        T_EachPlanInfo EachPlanInfo;
        EachPlanInfo.aEachPlaneVCount.Copy(aPlaneVCount);
        EachPlanInfo.aEachPlaneVertexIndex.Copy(aPlaneVertexIndex);
        EachPlanInfo.aEachVertex.Copy(aVertex);
        mapPlanePropertyToInfo[0] = EachPlanInfo;
    }
    else
    {
        ASSERT(aPlaneProperty.GetSize() == aPlaneVCount.GetSize());

        int nPlanVCntAccu = 0;
        for ( int i = 0; i < aPlaneVCount.GetSize(); i++ )
        {
            auto itrFind = mapPlanePropertyToInfo.find(aPlaneProperty[i]);
            if ( itrFind == mapPlanePropertyToInfo.end() )
            {
                T_EachPlanInfo PlanInfoTemp;  PlanInfoTemp.Init();
                mapPlanePropertyToInfo[aPlaneProperty[i]] = PlanInfoTemp;
                itrFind = mapPlanePropertyToInfo.find(aPlaneProperty[i]);
            }
            T_EachPlanInfo& EachPlanInfoTemp = itrFind->second;

            EachPlanInfoTemp.aEachPlaneVCount.Add(aPlaneVCount[i]);
            for ( int j = 0; j < aPlaneVCount[i]; j++ )
            {
                EachPlanInfoTemp.aEachPlaneVertexIndex.Add(aPlaneVertexIndex[nPlanVCntAccu + j]);
            }
            nPlanVCntAccu += aPlaneVCount[i];
        }

        for ( auto itrMap = mapPlanePropertyToInfo.begin(); itrMap != mapPlanePropertyToInfo.end(); itrMap++ )
        {
            itrMap->second.aEachVertex.RemoveAll();
            itrMap->second.aEachVertex.Copy(aVertex);
        }
    }
}

BOOL CSimpleMesh::GetSectMergeMesh(T_SECT_D &SectD, int nPos, int nTypeBeforeAfter, CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> &raMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&> &raMeshNodeD, CArray<int, int> &raDomainMaterialType, std::vector<int> &raDomainElemCount)
{
    CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdge;
    if ( !GetSectPolygon(SectD, nPos, aaEdge, nTypeBeforeAfter) ) return FALSE;

    m_bTubSectClosed = FALSE;
    if (nTypeBeforeAfter == 3)
    {
        nTypeBeforeAfter = 1;  
        m_bTubSectClosed = TRUE;
    }
    if ( nTypeBeforeAfter == 1 || nTypeBeforeAfter == 2 || nTypeBeforeAfter == 11 || nTypeBeforeAfter == 12 )
    {
        if(CSectDB::IsSectDumbAndStub(SectD))
        {/*skip*/}
        else
            ASSERT(aaEdge.GetSize() >= 2); // Composite 단면이란 말이고.. 이때는 aaEdge[0]이 Girder이고 aaEdge[1]이 Slab이고 aaEdge[2]이후부터는 Stiffener이다.
    }

    int nStype;
    nStype = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.nStype : SectD.nStype;

    BOOL bSharedEdge = FALSE;
	if (aaEdge.GetSize() > 1 || CSectDB::IsSectDumbAndStub(SectD))
    {
        bSharedEdge = TRUE;
    }

    BOOL bPSCSect;
    bPSCSect = CSectDB::IsSectPSC(SectD);

#ifdef _DEBUG
    if ( bSharedEdge && !CSectDB::IsSectDumbAndStub(SectD))
    {
        ASSERT(CSectDB::IsSectComposite(SectD) || CSectDB::IsSectStlGirder(SectD)); // Comp User Type에 대한 ASSERT 가 빠져있는 상황임

        if ( aaEdge.GetSize() > 2 )
        {
            int nStiffenerSize = 0;
            int nSODStiffenerSize = 0;

            CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>* pStiffProfile[4];
            pStiffProfile[0] = &SectD.SectBefore.SectI.Stiffener.aStiffLeft;
            pStiffProfile[1] = &SectD.SectBefore.SectI.Stiffener.aStiffRight;
            pStiffProfile[2] = &SectD.SectBefore.SectI.Stiffener.aStiffBottom;
            pStiffProfile[3] = &SectD.SectBefore.SectI.Stiffener.aStiffTop;

            for ( int i = 0; i < 4; i++ )
            {
                for ( int j = 0; j < pStiffProfile[i]->GetSize(); j++ )
                {
                    if ( pStiffProfile[i]->GetAt(j).bCalc )
                    {
                        nStiffenerSize++;
                    }
                }
            }

            for ( int i = 0; i < SectD.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize(); i++ )
            {
                for ( int j = 0; j < SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData.GetSize(); j++ )
                {
                    if ( SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData[j].bCalc )
                    {
                        nSODStiffenerSize++;
                    }
                }
            }
        }
    }
#endif // _DEBUG
    if ( bSharedEdge )
    {
        // CSectDB::AdjustSectCompositeForMesh에서 Slab를 약간띄우는 코드가 들어가면서 Girder와 Slab의 절점공유를 안하도록 하게 되었음.
        MakeShareEdgeWhenComp(nTypeBeforeAfter, SectD, aaEdge, FALSE);
    }

    //--------------------------------------------------------------
    // Mesh 검증용 Node 생성. (Section대화상자에서 Property 버튼을 눌렀을 때를 제외한 상황에서는 트렌젝션이 꼬일 수 있음)
    BOOL bMakeNodeOfMesh = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFPolygonForSectMeshToNode"), _T("yes")) ) bMakeNodeOfMesh = TRUE;

    if ( bMakeNodeOfMesh )
    {
        WriteNodeOfMesh(aaEdge);
    }
    //--------------------------------------------------------------

    CArray<CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>, CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>&> aEachMeshElemD;
    CArray<CArray<T_MESH_NODE_D, T_MESH_NODE_D&>, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>&> aEachMeshNodeD;
    CArray<std::vector<int>, std::vector<int>&> aEachDomainElemCount;
    CArray<CArray<int, int>, CArray<int, int>&> aEachDomainMaterialType;
    CArray<int, int> aNodeKAccu;

    int nAAEdgeSize = aaEdge.GetSize();
    aEachMeshElemD.SetSize(nAAEdgeSize);
    aEachMeshNodeD.SetSize(nAAEdgeSize);
    aEachDomainElemCount.SetSize(nAAEdgeSize);
    aEachDomainMaterialType.SetSize(nAAEdgeSize);
    aNodeKAccu.SetSize(nAAEdgeSize);

    int nDomainMaterialTypeNum = 0;

    BOOL bGapMesh = ( IsCompositeSlabGapMesh(SectD) && nTypeBeforeAfter==2 ) ? TRUE : FALSE;

    BOOL bCheckAll=FALSE;
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> aEdgeAll; aEdgeAll.RemoveAll();

    // SectTest
    if ( CDb_DllBaseManager::m_bAllSectMesh )
    {
        if ( ( CSectDB::IsSectPSComposite(SectD)    && nTypeBeforeAfter==2 && IsCompositeSlabNoGap(SectD) ) ||
            ( CSectDB::IsSectSteelComposite(SectD) && nTypeBeforeAfter==2 && IsCompositeSlabNoGap(SectD) ) )
        {
            bCheckAll = TRUE;
            for ( int i=0; i<aaEdge.GetSize(); i++ ) aEdgeAll.Append(aaEdge[i].aAr);
        }
    }

    int nMaxMatlType = INT_MIN;
    int nMinMatlType = INT_MAX;
    for ( int i = 0; i < aaEdge.GetSize(); i++ )
    {
        BOOL bLastEdge = ( i == aaEdge.GetSize()-1 ) ? TRUE : FALSE;
        // hslee : nTypeBeforeAfter 에 따라서 Slab만 혹은 Girder만 연산하도록 처리함..
        //         nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2, 20=Stiffener(CSimpleMesh::GetSectMesh에서만 세팅 함)

        if ( aaEdge[i].aAr.GetSize() == 0 )
        {
            // Steel Girder Type은 Slab쪽(i가 0일때)의 Edge가 0개로 들어옴
            continue;
        }

        if ( nStype == D_SECT_TYPE_COMPO_G )
        {
            // continue 없음
        }
        else if (CSectDB::IsSectDumbAndStub(SectD))  {/*skip*/}
        else if ( nTypeBeforeAfter == 1 || nTypeBeforeAfter == 11 )
        {
            if ( i == 0 ) // Slab
            {
                continue;
            }

            if ( ( CSectDB::IsSectPSComposite(SectD)    && bLastEdge && bGapMesh ) ||
                ( CSectDB::IsSectSteelComposite(SectD) && bLastEdge && bGapMesh ) )
            {
                continue;
            }
        }
        else if ( nTypeBeforeAfter == 2 )
        {
            // continue 없음
        }
        else if ( nTypeBeforeAfter == 12 )
        {
            if ( i == 1 || i >= 2 ) // Girder이거나 Stiffener인 경우
            {
                continue;
            }
        }

        int nTypeBeforeAfterForParam = nTypeBeforeAfter;

        if ( bSharedEdge )
        {
            if ( IsBoxGirderShape(nStype) )
            {
                if ( i == 0 ) // Slab
                {
                    nTypeBeforeAfterForParam = 12; // Part2
                }
                else if ( i <= GetBoxGirderShapeNum(nStype) ) // Girder
                {
                    nTypeBeforeAfterForParam = 11; // Part1
                }
                else if ( CSectDB::IsSectSteelComposite(SectD) && bLastEdge && bGapMesh )
                {
                    nTypeBeforeAfterForParam = 13; // Gap
                }
                else // Stiffner
                {
                    nTypeBeforeAfterForParam = 20; // Stiffner
                }
            }
            else if (CSectDB::IsSectDumbAndStub(SectD))
            {
                if (i == 0) // steel
                {
                    nTypeBeforeAfterForParam = 11;
                }
				else if (i == 1) // conc
				{
					nTypeBeforeAfterForParam = 11;
				}
				else if (i == 2) // void
				{
					nTypeBeforeAfterForParam = -1;
				}

            }
            else
            {
                if ( i == 0 ) // Slab
                {
                    nTypeBeforeAfterForParam = 12; // Part2
                }
                else if ( i == 1 ) // Girder
                {
                    nTypeBeforeAfterForParam = 11; // Part1
                }
                else if ( ( CSectDB::IsSectPSComposite(SectD)    && bLastEdge && bGapMesh ) ||
                    ( CSectDB::IsSectSteelComposite(SectD) && bLastEdge && bGapMesh ) )
                {
                    nTypeBeforeAfterForParam = 13; // Gap
                }
                else // Stiffner
                {
                    nTypeBeforeAfterForParam = 20; // Stiffner
                }
            }
        }

        // Stiffener만 Mesh방식을 다르게 했을때의 코드
        //if(bPSCSect || nTypeBeforeAfterForParam == 20)
        //{
        //  bGradationMesh = TRUE;
        //}
        //else
        //{
        //  bGradationMesh = FALSE;
        //}

        if ( !bCheckAll )
        {
            aEdgeAll.RemoveAll();
            aEdgeAll.Append(aaEdge[i].aAr);
        }
        if ( !GetSectMeshByPolygon(SectD, nTypeBeforeAfterForParam, aaEdge[i].aAr, aEdgeAll, aEachMeshElemD[i], aEachMeshNodeD[i], aEachDomainElemCount[i], aEachDomainMaterialType[i], FALSE) ) return FALSE;

        nDomainMaterialTypeNum += aEachDomainMaterialType[i].GetSize();

        for ( int j = 0; j < aEachDomainMaterialType[i].GetSize(); j++ )
        {
            if ( aEachDomainMaterialType[i][j] > nMaxMatlType )
            {
                nMaxMatlType = aEachDomainMaterialType[i][j];
            }

            if ( aEachDomainMaterialType[i][j] < nMinMatlType )
            {
                nMinMatlType = aEachDomainMaterialType[i][j];
            }
        }
    }

    if ( nDomainMaterialTypeNum == 0 )
    {
        int nNodeKAccu = 0;
        for ( int j = 0; j < nAAEdgeSize; j++ )
        {
            for ( int k = 0; k < aEachMeshElemD[j].GetSize(); k++ )
            {
                for ( int z = 0; z < aEachMeshElemD[j][k].aNodeK.GetSize(); z++ )
                {
                    aEachMeshElemD[j][k].aNodeK[z] += nNodeKAccu;
                }
            }

            raMeshElemD.Append(aEachMeshElemD[j]);
            raMeshNodeD.Append(aEachMeshNodeD[j]);

            int nElemCntAccu = 0;
            for ( int k = 0; k < aEachDomainElemCount[j].size(); k++ )
            {
                nElemCntAccu += aEachDomainElemCount[j][k];
            }

            if ( raDomainMaterialType.GetSize() == 0 )
            {
                raDomainElemCount.push_back(nElemCntAccu);
            }
            else
            {
                raDomainElemCount[raDomainElemCount.size() - 1] += nElemCntAccu;
            }

            nNodeKAccu += aEachMeshNodeD[j].GetSize();
        }
    }
    else
    {
        //DomainMaterialType별로 다시 sorting
        int nNodeKAccu = 0;
        for ( int i = nMinMatlType; i <= nMaxMatlType; i++ )
        {
            for ( int j = 0; j < nAAEdgeSize; j++ )
            {
                if ( aEachDomainMaterialType[j].GetSize() == 0 )
                {
                    ASSERT(aEachMeshElemD[j].GetSize() == 0 && aEachMeshNodeD[j].GetSize() == 0 && aEachDomainElemCount[j].size() == 0);
                    continue;
                }

                int nFirstDomainMaterialType;
                for ( int k = 0; k < aEachDomainMaterialType[j].GetSize(); k++ )
                {
                    if ( k == 0 )
                    {
                        nFirstDomainMaterialType = aEachDomainMaterialType[j][k];
                    }
                    else
                    {
                        // 정상
                    }
                }

                if ( aEachDomainMaterialType[j][0] == i )
                {
                    for ( int k = 0; k < aEachMeshElemD[j].GetSize(); k++ )
                    {
                        for ( int z = 0; z < aEachMeshElemD[j][k].aNodeK.GetSize(); z++ )
                        {
                            aEachMeshElemD[j][k].aNodeK[z] += nNodeKAccu;
                        }
                    }

                    raMeshElemD.Append(aEachMeshElemD[j]);
                    raMeshNodeD.Append(aEachMeshNodeD[j]);

                    int nElemCntAccu = 0;
                    for ( int k = 0; k < aEachDomainElemCount[j].size(); k++ )
                    {
                        nElemCntAccu += aEachDomainElemCount[j][k];
                    }

                    if ( raDomainMaterialType.GetSize() == 0 || raDomainMaterialType[raDomainMaterialType.GetSize() - 1] != aEachDomainMaterialType[j][0] )
                    {
                        raDomainElemCount.push_back(nElemCntAccu);
                        raDomainMaterialType.Add(aEachDomainMaterialType[j][0]);
                    }
                    else
                    {
                        // MaterialType이 같으면 합침
                        raDomainElemCount[raDomainElemCount.size() - 1] += nElemCntAccu;
                    }

                    nNodeKAccu += aEachMeshNodeD[j].GetSize();
                }
            }
        }
    }

    CSimpleMesh simpleMesh;
    simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;
    CArray<int, int> aDummy; // Material Type이 동일한지는 Check하지 않게 하기위해 빈 배열을 넘김(Slab와 Girder는 어차피 항상 떨어지도록 했기 때문에 체크할 필요가 없음)
    std::vector<int> aDummy2;
    aDummy2.push_back(0);
    aDummy2.push_back(1);
    simpleMesh.MergeDomainMesh(SectD, aDummy, raMeshElemD, raMeshNodeD, aDummy2); // GetSectMeshByPolygon안에서 Merge하지 않고, Mesh가 완성된 이후에 수행 함

    return TRUE;
}

BOOL CSimpleMesh::GetSectRegularMesh(int nPos, T_SECT_D &SectD, std::vector<int> &raDomainElemCount, CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> &raMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& raMeshNodeD, CArray<int, int> &raDomainMaterialType, int nTypeBeforeAfter, BOOL bMerge /*= TRUE*/)
{
    // 단면 형상 얻어오기.
    int nNumPlane = 0;
    CArray<int, int> aPlaneVCount, aPlaneVertexIndex;
    CArray<BOOL, BOOL> aPlaneVacant;
    CArray<double, double> aVertex;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    double dShapeRatioFromI = 0.5;    // tapered composite의 경우 I/J단 단면을 정확히 얻어오기 위해 I단의로부터의 거리비를 넘김.
    if ( nPos == 0 ) dShapeRatioFromI = 0.0;
    if ( nPos == 1 ) dShapeRatioFromI = 1.0;

    if ( !pDoc->m_pSectDB->makeSectShape(SectD, nNumPlane, &aPlaneVCount, &aPlaneVacant, &aPlaneVertexIndex, &aVertex, dShapeRatioFromI) )
    {
        ASSERT(0);
        return FALSE;
    }

    CArray<T_SIMPLE_MESH_NODE_D, T_SIMPLE_MESH_NODE_D&> aNode_simple;   aNode_simple.RemoveAll();
    CArray<T_SIMPLE_MESH_EDGE_D, T_SIMPLE_MESH_EDGE_D&> aEdge_simple;   aEdge_simple.RemoveAll();
    ConvertSectShape2Edge(aPlaneVCount, aPlaneVertexIndex, aVertex, aNode_simple, aEdge_simple);
    if ( aNode_simple.GetSize() <= 3 || aEdge_simple.GetSize() <= 3 ) { ASSERT(0);  return FALSE; }

    //--------------------------------------------------------------
    // 형상 검증용 mct 생성.
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    int iIndexBadChar = strPostFix.Find(_T("/"));
    if ( iIndexBadChar > 0 ) strPostFix = strPostFix.Left(iIndexBadChar);

    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    if ( bDumpSectMeshFile )  WriteMctOfSect(aNode_simple, aEdge_simple, strPostFix);
    //--------------------------------------------------------------

    // 단면 형상 메쉬

    T_POINT3D ptStart, ptEnd;
    T_LINE_FOR_MESH_G LineG;
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> aEdge;   aEdge.RemoveAll();

    for ( int i=0; i<aEdge_simple.GetSize(); ++i )
    {
        T_SIMPLE_MESH_NODE_D& node1 = aNode_simple[aEdge_simple[i].iNode1 -1];
        T_SIMPLE_MESH_NODE_D& node2 = aNode_simple[aEdge_simple[i].iNode2 -1];
        ptStart.Set(node1.y, node1.z, 0.0);
        ptEnd.Set(node2.y, node2.z, 0.0);
        LineG.Set(ptStart, ptEnd);

        aEdge.Add(LineG);
    }

    //--------------------------------------------------------------
    // Mesh 검증용 Node 생성. (Section대화상자에서 Property 버튼을 눌렀을 때를 제외한 상황에서는 트렌젝션이 꼬일 수 있음)
    BOOL bMakeNodeOfMesh = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFPolygonForSectMeshToNode"), _T("yes")) ) bMakeNodeOfMesh = TRUE;

    if ( bMakeNodeOfMesh )
    {
        WriteNodeOfMesh(aEdge);
    }
    //--------------------------------------------------------------

    if ( !GetSectMeshByPolygon(SectD, nTypeBeforeAfter, aEdge, aEdge, raMeshElemD, raMeshNodeD, raDomainElemCount, raDomainMaterialType, FALSE) ) return FALSE;

    return TRUE;
}

// @param bByConc : Concrete 기준의 탄성계수비를 사용할 지 여부.
//                  PSC 파형강판(D_SECT_SHAPE_PSC_CMPWEB)은 Conc 기준으로 계산하고, Composite 단면은 Steel 기준으로 계산함.
BOOL CDb_DllBaseManager::GetMatl4Domain(const double dMatlValue[6],
    const int nDomain,
    const CArray<int, int>& aDomainMaterialType,
    double& rdE, double& rdW, double& rdNu,
    CArray<double, double>& raElaMod,
    CArray<double, double>& raSectW,
    CArray<double, double>& raNu, BOOL bByConc/*=FALSE*/)
{
    rdE  = dMatlValue[0];
    rdW  = dMatlValue[2];
    rdNu = dMatlValue[4];

    for ( int i=0; i<nDomain; i++ )
    {
        int nMatlType = 1;    // 0: RC, 1: Steel, -1: Hole
        if ( aDomainMaterialType.GetSize() > 0 )
        {
            if ( aDomainMaterialType[i] == 0 )    nMatlType = 0;    // RC
            else if ( aDomainMaterialType[i] == 13 )   nMatlType = 13;   // Gap
        }
        else if ( i == 0 )    nMatlType = 0;    // RC

        if ( nMatlType == 0 )    // RC of SRC
        {
            double dMatl_Elast_ratio = dMatlValue[1];
            if ( bByConc && dMatl_Elast_ratio != 0.0 )  dMatl_Elast_ratio = 1.0/dMatl_Elast_ratio;

            raElaMod.Add(dMatl_Elast_ratio);
            raSectW.Add(dMatlValue[3]);
            raNu.Add(dMatlValue[5]);
        }
        else if ( nMatlType == -1 )   // Hole
        {
            raElaMod.Add(1.0);
            raSectW.Add(1.0);
            raNu.Add(0.0);
        }
        else if ( nMatlType == 13 )   // Gap
        {
            double dEl= 1.0e-08;
            double dW = 1.0e-08;
            double dNu = 0.0;
            raElaMod.Add(dEl);
            raSectW.Add(dW);
            raNu.Add(dNu);
        }
        else
        {
            raElaMod.Add(rdE);
            raSectW.Add(rdW);
            raNu.Add(rdNu);
        }
    }

    return TRUE;
}

BOOL CDb_DllBaseManager::GetPoint_Effective(T_SECT_D& SectD, int nPos, int nTypeBeforeAfter, int nPoint, double* pointy, double* pointz)
{
    CDBDoc    *pDoc  = CDBDoc::GetDocPoint(); ASSERT(pDoc);

    double hi, bi, hj, bj;
    pDoc->m_pAttrCtrl->GetSectDimen(SectD, hi, bi, hj, bj, NULL, FALSE, FALSE, FALSE);
    double chi, cbi, chj, cbj, ch, cb;  // 도심 위치
    pDoc->m_pAttrCtrl->GetSectCentroid(SectD, chi, cbi, chj, cbj);
    chi=hi*0.5-chi, chj=hj*0.5-chj;
    cbi=bi*0.5-cbi, cbj=bj*0.5-cbj;
    T_SECT_SECTBASE_D* pSectBaseD = NULL;
    if ( nPos==0 )
    {
        pSectBaseD = &SectD.SectBefore.SectI;
        ch = chi;
        cb = cbi;
    }
    else
    {
        pSectBaseD = &SectD.SectBefore.SectJ;
        ch = chj;
        cb = cbj;
    }

    T_SECT_STIFFNESS* pSectStiffness = &pSectBaseD->Stiffness;

    memset(pointy, 0, sizeof(double)*nPoint);
    memset(pointz, 0, sizeof(double)*nPoint);

    pointy[0] = pSectStiffness->dx1;
    pointy[1] = pSectStiffness->dx2;
    pointy[2] = pSectStiffness->dx3;
    pointy[3] = pSectStiffness->dx4;

    pointz[0] = pSectStiffness->dy1;
    pointz[1] = pSectStiffness->dy2;
    pointz[2] = pSectStiffness->dy3;
    pointz[3] = pSectStiffness->dy4;

    double h, b1, tw, tf1, b2, tf2, b, tf, c, cw;
    if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_H )
    {
        if ( nPoint!=23 ) { ASSERT(0); return FALSE; }
        h	  =	pSectBaseD->Size[0];
        b1	=	pSectBaseD->Size[1];
        tw	=	pSectBaseD->Size[2];
        tf1	=	pSectBaseD->Size[3];
        b2  =	( pSectBaseD->Size[4]==0.0 ) ? b1 : pSectBaseD->Size[4];
        tf2	= ( pSectBaseD->Size[5]==0.0 ) ? tf1 : pSectBaseD->Size[5];
        double r1 = pSectBaseD->Size[6];
        double r2 = pSectBaseD->Size[7];

        pointy[4] = cb-tw*0.5;
        pointy[5] = cb+tw*0.5;
        pointy[6] = cb;
        pointy[7] = cb;
        pointy[8] = pointy[4];
        pointy[9] = pointy[5];
        pointy[10] = pointy[0]+r2;
        pointy[11] = pointy[4]-r1;
        pointy[12] = cb;
        pointy[13] = pointy[5]+r1;
        pointy[14] = pointy[1]-r2;
        pointy[15] = cb;
        pointy[16] = pointy[3]+r2;
        pointy[17] = pointy[4]-r1;
        pointy[18] = cb;
        pointy[19] = pointy[5]+r1;
        pointy[20] = pointy[2]-r2;
        pointy[21] = pointy[4];
        pointy[22] = pointy[5];

        pointz[4] = ch;
        pointz[5] = ch;
        pointz[6] = pointz[3];
        pointz[7] = pointz[0];
        pointz[8] = pointz[0];
        pointz[9] = pointz[0];
        pointz[10] = pointz[0]-tf1;
        pointz[11] = pointz[10];
        pointz[12] = pointz[10];
        pointz[13] = pointz[10];
        pointz[14] = pointz[10];
        pointz[15] = ch;
        pointz[16] = pointz[3]+tf2;
        pointz[17] = pointz[16];
        pointz[18] = pointz[16];
        pointz[19] = pointz[16];
        pointz[20] = pointz[16];
        pointz[21] = pointz[3];
        pointz[22] = pointz[3];
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_C )
    {
        if ( nPoint!=18 ) { ASSERT(0); return FALSE; }

        h	  = pSectBaseD->Size[0];
        b1  = pSectBaseD->Size[1];
        tw	= pSectBaseD->Size[2];
        tf1 = pSectBaseD->Size[3];
        b2  = ( pSectBaseD->Size[4]==0.0 ) ? b1 : pSectBaseD->Size[4];
        tf2	= ( pSectBaseD->Size[5]==0.0 ) ? tf1 : pSectBaseD->Size[5];
        double r1 = pSectBaseD->Size[6];
        double r2 = pSectBaseD->Size[7];

        pointy[4] = pointy[0];
        pointy[5] = pointy[0]+tw;
        pointy[6] = cb;
        pointy[7] = cb;
        pointy[8] = pointy[5];
        pointy[9] = pointy[0];
        pointy[10] = pointy[5]+r1;
        pointy[11] = cb;
        pointy[12] = pointy[1]-r2;
        pointy[13] = pointy[0];
        pointy[14] = pointy[5]+r1;
        pointy[15] = cb;
        pointy[16] = pointy[2]-r2;
        pointy[17] = pointy[5];

        pointz[4] = ch;
        pointz[5] = ch;
        pointz[6] = pointz[3];
        pointz[7] = pointz[0];
        pointz[8] = pointz[0];
        pointz[9] = pointz[0]-tf1;
        pointz[10] = pointz[9];
        pointz[11] = pointz[9];
        pointz[12] = pointz[9];
        pointz[13] = pointz[3]+tf2;
        pointz[14] = pointz[13];
        pointz[15] = pointz[13];
        pointz[16] = pointz[13];
        pointz[17] = pointz[3];
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_B )
    {
        if ( nPoint!=24 ) { ASSERT(0); return FALSE; }

        h = pSectBaseD->Size[0];		b = pSectBaseD->Size[1];
        tw= pSectBaseD->Size[2]; 		tf1 = pSectBaseD->Size[3];
        cw = ( pSectBaseD->Size[4]==0.0 ) ? b : pSectBaseD->Size[4]+tw;
        tf2= ( pSectBaseD->Size[5]==0.0 ) ? tf1 : pSectBaseD->Size[5];

        pointy[4] = cb-cw*0.5;
        pointy[5] = cb+cw*0.5;
        pointy[6] = cb;
        pointy[7] = cb;
        pointy[8] = pointy[4]+tw;
        pointy[9] = pointy[5]-tw;
        pointy[10] = pointy[0];
        pointy[11] = pointy[8];
        pointy[12] = cb;
        pointy[13] = pointy[9];
        pointy[14] = pointy[1];
        pointy[15] = pointy[8];
        pointy[16] = pointy[9];
        pointy[17] = pointy[3];
        pointy[18] = pointy[8];
        pointy[19] = cb;
        pointy[20] = pointy[9];
        pointy[21] = pointy[2];
        pointy[22] = pointy[8];
        pointy[23] = pointy[9];

        pointz[4] = ch;
        pointz[5] = ch;
        pointz[6] = pointz[3];
        pointz[7] = pointz[0];
        pointz[8] = pointz[0];
        pointz[9] = pointz[1];
        pointz[10] = pointz[0]-tf1;
        pointz[11] = pointz[0]-tf1;
        pointz[12] = pointz[7]-tf1;
        pointz[13] = pointz[1]-tf1;
        pointz[14] = pointz[1]-tf1;
        pointz[15] = ch;
        pointz[16] = ch;
        pointz[17] = pointz[3]+tf2;
        pointz[18] = pointz[3]+tf2;
        pointz[19] = pointz[6]+tf2;
        pointz[20] = pointz[2]+tf2;
        pointz[21] = pointz[2]+tf2;
        pointz[22] = pointz[3];
        pointz[23] = pointz[2];
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_T )
    {
        if ( nPoint!=16 ) { ASSERT(0); return FALSE; }

        h	= pSectBaseD->Size[0];
        b = pSectBaseD->Size[1];
        tw= pSectBaseD->Size[2];
        tf= pSectBaseD->Size[3];

        pointy[4] = cb-tw*0.5;
        pointy[5] = cb+tw*0.5;
        pointy[6] = cb;
        pointy[7] = cb;
        pointy[8] = pointy[4];
        pointy[9] = pointy[5];
        pointy[10] = pointy[0];
        pointy[11] = pointy[4];
        pointy[12] = cb;
        pointy[13] = pointy[5];
        pointy[14] = pointy[1];
        pointy[15] = cb;

        pointz[4] = ch;
        pointz[5] = ch;
        pointz[6] = pointz[3];
        pointz[7] = pointz[0];
        pointz[8] = pointz[0];
        pointz[9] = pointz[0];
        pointz[10] = pointz[0]-tf;
        pointz[11] = pointz[10];
        pointz[12] = pointz[10];
        pointz[13] = pointz[10];
        pointz[14] = pointz[10];
        pointz[15] = ch;
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_2C )
    {
        if ( nPoint!=28 ) { ASSERT(0); return FALSE; }

        h = pSectBaseD->Size[0];  b = pSectBaseD->Size[1];
        tw= pSectBaseD->Size[2];  tf= pSectBaseD->Size[3];
        c = pSectBaseD->Size[4];

        pointy[4] = cb-tw-c*0.5;
        pointy[5] = cb+tw+c*0.5;
        pointy[6] = cb-c*0.5;
        pointy[7] = pointy[6];
        pointy[8] = pointy[4];
        pointy[9] = cb+c*0.5;
        pointy[10] = pointy[5];
        pointy[11] = pointy[0];
        pointy[12] = pointy[4];
        pointy[13] = pointy[6];
        pointy[14] = pointy[9];
        pointy[15] = pointy[5];
        pointy[16] = pointy[1];
        pointy[17] = pointy[6];
        pointy[18] = pointy[9];
        pointy[19] = pointy[3];
        pointy[20] = pointy[4];
        pointy[21] = pointy[6];
        pointy[22] = pointy[9];
        pointy[23] = pointy[5];
        pointy[24] = pointy[2];
        pointy[25] = pointy[4];
        pointy[26] = pointy[9];
        pointy[27] = pointy[5];

        pointz[4] = ch;
        pointz[5] = ch;
        pointz[6] = ch-h*0.5;
        pointz[7] = ch+h*0.5;
        pointz[8] = pointz[7];
        pointz[9] = pointz[7];
        pointz[10] = pointz[7];
        pointz[11] = ch+h*0.5-tf;
        pointz[12] = pointz[11];
        pointz[13] = pointz[11];
        pointz[14] = pointz[11];
        pointz[15] = pointz[11];
        pointz[16] = pointz[11];
        pointz[17] = ch;
        pointz[18] = ch;
        pointz[19] = ch-h*0.5+tf;
        pointz[20] = pointz[19];
        pointz[21] = pointz[19];
        pointz[22] = pointz[19];
        pointz[23] = pointz[19];
        pointz[24] = pointz[19];
        pointz[25] = pointz[6];
        pointz[26] = pointz[6];
        pointz[27] = pointz[6];
    }
    else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_2CB )
    {
        ASSERT(0);
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_L )
    {
        if ( nPoint!=12 ) { ASSERT(0); return FALSE; }

        h = pSectBaseD->Size[0];  b = pSectBaseD->Size[1];
        tw= pSectBaseD->Size[2];  tf= pSectBaseD->Size[3];

        pointy[4] = pointy[0];
        pointy[5] = pointy[0]+tw;
        pointy[6] = cb;
        pointy[7] = cb;
        pointy[8] = pointy[5];
        pointy[9] = pointy[0];
        pointy[10] = pointy[5];
        pointy[11] = pointy[1];

        pointz[4] = ch;
        pointz[5] = ch;
        pointz[6] = pointz[0]-tf;
        pointz[7] = pointz[0];
        pointz[8] = pointz[0];
        pointz[9] = pointz[6];
        pointz[10] = pointz[6];
        pointz[11] = pointz[6];
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_P )
    {
        if ( nPoint!=8 ) { ASSERT(0); return FALSE; }

        tw= pSectBaseD->Size[1];

        pointy[4] = pointy[3]+tw;
        pointy[5] = pointy[1]-tw;
        pointy[6] = pointy[2];
        pointy[7] = pointy[0];

        pointz[4] = pointz[3];
        pointz[5] = pointz[1];
        pointz[6] = pointz[2]+tw;
        pointz[7] = pointz[0]-tw;
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_2L )
    {
        if ( nPoint!=20 ) { ASSERT(0); return FALSE; }

        h = pSectBaseD->Size[0];	b = pSectBaseD->Size[1];
        tw= pSectBaseD->Size[2]; 	tf= pSectBaseD->Size[3];
        c = pSectBaseD->Size[4];

        pointy[4] = cb-tw-c*0.5;
        pointy[5] = cb+tw+c*0.5;
        pointy[6] = cb-c*0.5;
        pointy[7] = pointy[6];
        pointy[8] = pointy[4];
        pointy[9] = cb+c*0.5;
        pointy[10] = pointy[5];
        pointy[11] = pointy[0];
        pointy[12] = pointy[4];
        pointy[13] = pointy[6];
        pointy[14] = pointy[9];
        pointy[15] = pointy[5];
        pointy[16] = pointy[1];
        pointy[17] = pointy[6];
        pointy[18] = pointy[9];
        pointy[19] = pointy[9];

        pointz[4] = ch;
        pointz[5] = ch;
        pointz[6] = pointz[3];
        pointz[7] = pointz[0];
        pointz[8] = pointz[7];
        pointz[9] = pointz[7];
        pointz[10] = pointz[7];
        pointz[11] = pointz[0]-tf;
        pointz[12] = pointz[11];
        pointz[13] = pointz[11];
        pointz[14] = pointz[11];
        pointz[15] = pointz[11];
        pointz[16] = pointz[11];
        pointz[17] = ch;
        pointz[18] = ch;
        pointz[19] = pointz[2];
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_SB )
    {
        if ( nPoint!=9 ) { ASSERT(0); return FALSE; }

        h = pSectBaseD->Size[0];
        b = pSectBaseD->Size[1];

        pointy[4] = pointy[0];
        pointy[5] = pointy[1];
        pointy[6] = cb;
        pointy[7] = cb;
        pointy[8] = cb;

        pointz[4] = ch;
        pointz[5] = ch;
        pointz[6] = pointz[3];
        pointz[7] = pointz[0];
        pointz[8] = ch;
    }
    else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_SR )
    {
        if ( nPoint!=5 ) { ASSERT(0); return FALSE; }

        pointy[4] = cb;

        pointz[4] = ch;
    }
    else if (CSectDB::IsSectSteelComposite(SectD))
    {
		T_SECT_EQUIV_POINT* pPointD = new T_SECT_EQUIV_POINT;
		pPointD->nTypeBeforeAfter = nTypeBeforeAfter;
		pPointD->nPointSize = nPoint;
		T_SECT_D SectDTemp = SectD;

		if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_B)
		{
			if (nPoint != 24) { ASSERT(0); delete pPointD; return FALSE; }

			pDoc->m_pSectDB->CalcSectCompo_B(SectDTemp, FALSE, 0.0, pPointD);		
		}
        else if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_I)
	    {
		    if (nPoint != 19) { ASSERT(0); delete pPointD; return FALSE; }

		    pDoc->m_pSectDB->CalcSectCompo_I(SectDTemp, FALSE, 0.0, pPointD);
	    }
	    else if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_TUB)
	    {
		    if (nPoint != 28) { ASSERT(0); delete pPointD; return FALSE; }

		    pDoc->m_pSectDB->CalcSectCompo_Tub(SectDTemp, FALSE, 0.0, pPointD);
	    }
	    else if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B)
	    {
		    if (nPoint != 24) { ASSERT(0); delete pPointD; return FALSE; }

		    pDoc->m_pSectDB->CalcSectCompo_STLG_B(SectDTemp, FALSE, 0.0, pPointD);
	    }
        else if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I)
        {
            if (nPoint != 19) { ASSERT(0); delete pPointD; return FALSE; }

            pDoc->m_pSectDB->CalcSectCompo_STLG_I(SectDTemp, FALSE, 0.0, pPointD);
        }
		else if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB)
		{
			if (nPoint != 28) { ASSERT(0); delete pPointD; return FALSE; }

			pDoc->m_pSectDB->CalcSectCompo_STLG_Tub(SectDTemp, FALSE, 0.0, pPointD);
		}

		for (int i = 4; i < nPoint; i++)
		{
			pointy[i] = pPointD->pointy[i];
			pointz[i] = pPointD->pointz[i];
		}
		delete pPointD;
    }

    return TRUE;
}

BOOL CDb_DllBaseManager::GetEffectiveNormalStressPoint(const T_SECT_D& cSectD, int nPos, int nTypeBeforeAfter, int& rnPoint, CArray<double, double>& raPointY, CArray<double, double>& raPointZ)
{
  T_SECT_D SectD = cSectD;

  if (!CSectUtil::IsShapeEffective(SectD)) return FALSE;

  // nTypeBeforeAfter : //1 = 거더만  2 = composite 단면
  if (CSectDB::IsSectSteelComposite(SectD))
  {
    //--------------------------------------------------
    // 계산 단순화를 위해 각 Position(i or j) 위치 값을 i-단면으로 치환
    // ConvertSectBeforeI2StiffData()와 커플로 호출할 것...!!
    ConvertSectStiffData2BeforeI(SectD, cSectD, nPos, nTypeBeforeAfter);
    //--------------------------------------------------
  }

  rnPoint = CSectUtil::GetNPoint4EffectiveSect(SectD);
  raPointY.SetSize(rnPoint);
  raPointZ.SetSize(rnPoint);
  return GetPoint_Effective(SectD, nPos, nTypeBeforeAfter, rnPoint, raPointY.GetData(), raPointZ.GetData());
}

// @param force : [in] 1=AXIAL
//                     2=SHEAR Y
//                     3=SHEAR Z
//                     4=PURE TORSION
//                     5=MOMENT Y
//                     6=MOMENT Z
//                     7=BI-MOMENT
//                     8=WARPING TORSION
//                  reference axis에 대한 axial, shear y, shear z, moment y, moment z, torsion, bimoment.
//                  6자유도일 경우 pure torsion 자리에 Mx를 넣으면 됨.
// @param stress : [out] 1=PROPERTY INDEX
//                       2=NORMAL STRESS (AXIAL + BENDING + WARPING)
//                       3=SHEAR XY (TRANSVERSE SHEAR XY + PURE TORSIONAL SHEAR STRESS XY + WARPING TORSIONAL SHEAR STRESS XY)
//                       4=SHEAR XZ (TRANSVERSE SHEAR XZ + PURE TORSIONAL SHEAR STRESS XZ + WARPING TORSIONAL SHEAR STRESS XZ)
//                 inner domain을 제외한 domain 개수만큼 응력을 출력
//                 응력 결과 : (4 x (NPOINT))x NPROP+6.
//                             * NPROP는 sect_e가 0이 아닌 것의 개수
//                             * 마지막 6개는 max, min 결과
//                             1: 1 번 point의 sect_e값 index
//                             2: 1 번 point의 nomal stress
//                             3: 1 번 point의 shaer xy
//                             4: 1 번 point의 shaer xz
//                             5: 2 번 point의 sect_e값 index
//                             6: 2 번 point의 nomal stress
//                             7: 2 번 point의 shaer xy
//                             8: 2 번 point의 shaer xz
//                             ...
//                             마지막 6개는 max, min 결과
// @param coordy : [out] max/min 값에 대한 y좌표
//                       1=max normal stress
//                       2=max shear xy
//                       3=max shear xz
//                       4=min normal stress
//                       5=min shear xy
//                       6=min shear xz
// @param coordz : [out] max/min 값에 대한 z좌표
//                       1=max normal stress
//                       2=max shear xy
//                       3=max shear xz
//                       4=min normal stress
//                       5=min shear xy
//                       6=min shear xz

BOOL CDb_DllBaseManager::Get_SectionPropertyMesh_Effective(T_SECT_D& rData, int nPos, int nCompMeshType, double dMatlValue[6],
    double force[8],
    int& rnStress,    // count of stress
    double coordy[6],
    double coordz[6],
    int& rnPoint,     // count of stress point
    int& rnProp,      // count of domain (sect_e가 0이 아닌 것의 개수)
    CArray<double, double>& raStress,
    CArray<double, double>& raPointY,
    CArray<double, double>& raPointZ)
{
    T_SECT_D SectD = rData;

    if ( !CSectUtil::IsShapeEffective(SectD) ) return FALSE;

    CArray<double, double> ctex;   ctex.RemoveAll();   // 각 도메인별 열팽창계수, 도메인 개수만큼 (inner line인 경우 0)
    double* bst = NULL;

    // 단면 형상/메쉬 얻어오기.
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD;    aMeshElemD.RemoveAll();
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD;    aMeshNodeD.RemoveAll();
    std::vector<int> aDomainElemCount;                   aDomainElemCount.clear();
    CArray<int, int> aDomainMaterialType;                aDomainMaterialType.RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입

    CSimpleMesh simpleMesh;
    int nTypeBeforeAfter = 0;   // nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후.
    BOOL bAdjustCoordToCentroid = FALSE;
    if (CSectDB::IsSectSteelComposite(SectD))
    {
        nTypeBeforeAfter = nCompMeshType;  //1 = 거더만  2 = composite 단면
        bAdjustCoordToCentroid = TRUE;
        simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;
        m_bAllSectMesh = TRUE;  // 모든 계산은 온전한 7Dof로 계산
        //--------------------------------------------------
        // 계산 단순화를 위해 각 Position(i or j) 위치 값을 i-단면으로 치환
        // ConvertSectBeforeI2StiffData()와 커플로 호출할 것...!!
        ConvertSectStiffData2BeforeI(SectD, rData, nPos, nTypeBeforeAfter);
        //--------------------------------------------------
    }
    else
    {
        m_bAllSectMesh = FALSE; // 기존
        bAdjustCoordToCentroid = FALSE;
    }
    if ( !simpleMesh.GetSectMesh(SectD, nPos, nTypeBeforeAfter, aMeshElemD, aMeshNodeD, aDomainElemCount, aDomainMaterialType, bAdjustCoordToCentroid, NULL) ) { return FALSE; }

    HINSTANCE hDllCalc = GetOrLoad_PBeamLMeshDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    T_SECT_SECTBASE_D* pSectBaseD = NULL;
    if ( nPos==0 ) pSectBaseD = &SectD.SectBefore.SectI;
    else           pSectBaseD = &SectD.SectBefore.SectJ;

    T_SECT_STIFFNESS* pSectStiffness = &pSectBaseD->Stiffness;

    int nCountDomain = aDomainElemCount.size();
    int nDomain = nCountDomain;
    int nPoint = CSectUtil::GetNPoint4EffectiveSect(SectD);

    double* pointy = NULL;
    double* pointz = NULL;

    BOOL bUserPoint = FALSE;
    ASSERT(raPointY.GetSize() == raPointZ.GetSize());
    if ( raPointY.GetSize() > 0 )
    {
        nPoint = raPointY.GetSize();
        bUserPoint = TRUE;
    }

    pointy = new double[nPoint];
    pointz = new double[nPoint];
    memset(pointy, 0, sizeof(double)*nPoint);
    memset(pointz, 0, sizeof(double)*nPoint);

    if ( bUserPoint )
    {
        for ( int i=0; i<nPoint; ++i )
        {
            pointy[i] = raPointY[i];
            pointz[i] = raPointZ[i];
        }
    }
    else
    {
        GetPoint_Effective(SectD, nPos, nTypeBeforeAfter, nPoint, pointy, pointz);
    }

    //--------------------------------------------------------------
    // Mesh 검증용 mgt 생성.
    BOOL bDumpSectPointFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("EffeSectPointFile"), _T("yes")) ) bDumpSectPointFile = TRUE;
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    if ( bDumpSectPointFile )  WriteMgtOfSectPoint(nPoint, pointy, pointz, strPostFix);
    //--------------------------------------------------------------   
    double dE = dMatlValue[0];
    double dW = dMatlValue[2];
    double dNu = dMatlValue[4];
    CArray<double, double> aElaMod; aElaMod.RemoveAll();
    CArray<double, double> aSectW; aSectW.RemoveAll();
    CArray<double, double> aNu; aNu.RemoveAll();
    BOOL bByConc = FALSE;
    if ( !GetMatl4Domain(dMatlValue, nDomain, aDomainMaterialType, dE, dW, dNu, aElaMod, aSectW, aNu, bByConc) ) { ASSERT(0); return FALSE; }

    int i = 0;
    int nCombinedNum = nCountDomain;
    if (CSectDB::IsSectSteelComposite(SectD))
    {
        nCombinedNum = 1;  // 합성단면의 모든 도메인을 1로 통일

        // 메쉬절점용 point를 위해 원래 단면을 사용
        CSectDB* pSectDB = CDBDoc::GetDocPoint()->m_pSectDB;
        double dMatlValueTemp[6] = { 0., };
        pSectDB->GetElast(SectD, dMatlValueTemp);

        CArray<double, double> aElaModTemp; aElaModTemp.RemoveAll();
        CArray<double, double> aSectWTemp; aSectWTemp.RemoveAll();
        CArray<double, double> aNuTemp; aNuTemp.RemoveAll();
        BOOL bByConc = FALSE;
        if (!GetMatl4Domain(dMatlValueTemp, nDomain, aDomainMaterialType, dE, dW, dNu, aElaModTemp, aSectWTemp, aNuTemp, bByConc)) { ASSERT(0); return FALSE; }
        //


        double dCentY = 0.0;
        double dCentZ = 0.0;
        if (CSimpleMesh::CalcCentroid(dCentY, dCentZ, aDomainElemCount, aMeshElemD, aMeshNodeD, aElaModTemp))
        {
            // 도심기준의 응력 위치를 Mesh 좌표 기준으로 환산
            // 솔버 좌표계와 맞춘다.
            for (int k = 0; k < nPoint; ++k)
            {
                pointy[k] += dCentY * (-1);
                pointz[k] += dCentZ;
            }
        }

        raPointY.SetSize(nPoint);  for (/*int*/ i = 0; i < nPoint; ++i)  raPointY.SetAt(i, pointy[i]-dCentY*(-1));  // fes 출력용 좌표는 원래 좌표를 사용
        raPointZ.SetSize(nPoint);  for (/*int*/ i = 0; i < nPoint; ++i)  raPointZ.SetAt(i, pointz[i]-dCentZ);
    }
    else
    {
        raPointY.SetSize(nPoint);  for (/*int*/ i = 0; i < nPoint; ++i)  raPointY.SetAt(i, pointy[i]);
        raPointZ.SetSize(nPoint);  for (/*int*/ i = 0; i < nPoint; ++i)  raPointZ.SetAt(i, pointz[i]);
    }

    BSPDetail* pBSPDetail;
    pBSPDetail = ( BSPDetail* ) GetProcAddress(hDllCalc, "BSPDetail");
    if ( pBSPDetail==NULL ) { ASSERT(0); return FALSE; }

    rnProp = nCountDomain; // nDomain
    rnPoint = nPoint;

    // BSPDetail() 함수의 입력 파라미터
    int _nnode = aMeshNodeD.GetSize();    int* p_nnode = &_nnode;           // 총 절점수
    int _npoint = nPoint;                 int* p_npoint = &_npoint;         // 응력계산위치 개수
    int _nelem = aMeshElemD.GetSize();    int* p_nelem = &_nelem;           // 총 요소수
    int _nprop = nCountDomain;            int* p_nprop = &_nprop;           // 총 material 수
    int _ncombined = nCombinedNum;        int* p_ncombined = &_ncombined;   // 연결되지 않은 단면의 수

    // 절점의 좌표
    // * 주의사항 (솔버에서 y-축 :수직, z-축 :수평으로 계산하고 있어서, 좌표는 y,z를 바꿔서 넘겨야 함.)
    CArray<double, double> aYNode; aYNode.RemoveAll();  aYNode.SetSize(_nnode);
    CArray<double, double> aZNode; aZNode.RemoveAll();  aZNode.SetSize(_nnode);
    double dY=0., dZ=0.;
    for (/*int*/ i=0; i<_nnode; i++ )
    {
        dY = aMeshNodeD[i].x * ( -1 ); // 솔버 좌표계와 맞추기 위해
        dZ = aMeshNodeD[i].y;

        aYNode.SetAt(i, dY);
        aZNode.SetAt(i, dZ);
    }
    double* p_ynode = aZNode.GetData();   // 절점의 y좌표
    double* p_znode = aYNode.GetData();   // 절점의 z좌표

    double* p_pointy = pointz;    // 응력계산 위치의 y좌표
    double* p_pointz = pointy;    // 응력계산 위치의 z좌표

    // 요소의 절점 연결 번호 (array size = 요소수 * 3, 저차 삼각형요소, 절점의 최초 시작은 1부터)
    CArray<int, int> _iconn;    _iconn.RemoveAll();    _iconn.SetSize(_nelem*3);    // 삼각형 요소.
    int iConnNum = 0;
    for (/*int*/ i=0; i<_nelem; ++i )
    {
        if ( aMeshElemD[i].aNodeK.GetSize() != 3 ) { ASSERT(0); return FALSE; }

        _iconn[iConnNum] = aMeshElemD[i].aNodeK[0] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[1] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[2] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
    }
    ASSERT(iConnNum == _iconn.GetSize());
    int* p_iconn = _iconn.GetData();

    // 요소 갯수 검증
    int nDomainElemSum = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )  nDomainElemSum += aDomainElemCount[i];
    ASSERT(nDomainElemSum == _nelem);

    // 요소의 재료 index (array size =  요소수)
    CArray<int, int> _elem_prop;    _elem_prop.RemoveAll();    _elem_prop.SetSize(_nelem);
    int nFactor = 1;
    int iElem = 0;
    for (/*int*/ i=0; i<aDomainElemCount.size(); ++i )
    {
        int nInx = (i + 1);

        for ( int k=0; k<aDomainElemCount[i]; ++k )
        {
            if (CSectDB::IsSectSteelComposite(SectD))
            {
                nFactor = (aDomainMaterialType[i] == 13) ? -1 : 1; // 13=Gap

                if (nTypeBeforeAfter == 2)
                {
                    // 슬래브, 거더, 나머지 순서로 찍히는데
                    // 거더 응력을 먼저 출력하기 위해 순서 변경
                    if      (i == 0) nInx = 2;
                    else if (i == 1) nInx = 1;
                }
            }
            _elem_prop[iElem] = nFactor * nInx; // p_sect_e, p_sect_nu, p_sect_w의 참조 index (fortran이기 때문에 1부터 시작). 사실상 소속 domain index

            iElem++;
        }
    }
    int* p_elem_prop = _elem_prop.GetData();

    // combined section의 노드수 (array size = p_ncombined)
    CArray<int, int> _comb_node;    _comb_node.RemoveAll();    _comb_node.SetSize(_ncombined);
    int iStart = 0;
    int iEndCount = 0;
    CMapEx<int, int, int, int> mNodeOfDomain;    
    mNodeOfDomain.RemoveAll();
    if (CSectDB::IsSectSteelComposite(SectD)) //nCombinedNum == 1
    {
        for (int i = 0; i < aDomainElemCount.size(); ++i)
        {
            if (i > 0) iStart += aDomainElemCount[i - 1];
            iEndCount = iStart + aDomainElemCount[i];

            // 개별 Domain에 대한 loop
            for (int k = iStart; k < iEndCount; ++k)
            {
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[0], 0);
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[1], 0);
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[2], 0);
            }
        }
        _comb_node[0] = mNodeOfDomain.GetCount();
    }
    else
    {
        for (int i = 0; i < _ncombined; ++i)
        {
            if (i > 0) iStart += aDomainElemCount[i - 1];
            iEndCount = iStart + aDomainElemCount[i];

			mNodeOfDomain.RemoveAll();

            // 개별 Domain에 대한 loop
            for (int k = iStart; k < iEndCount; ++k)
            {
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[0], 0);
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[1], 0);
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[2], 0);
            }
            _comb_node[i] = mNodeOfDomain.GetCount();
        }
    }
    int* p_comb_node = _comb_node.GetData();

    // combined section의 요소수 (array size = p_ncombined)
    CArray<int, int> _comb_elem;    _comb_elem.RemoveAll();    _comb_elem.SetSize(_ncombined);

    if (CSectDB::IsSectSteelComposite(SectD)) //nCombinedNum == 1
    {
        _comb_elem[0] = iElem;

        if (nTypeBeforeAfter == 2)
        {
            // 슬래브, 거더, 나머지 순서로 찍히는데
            // 거더 응력을 먼저 출력하기 위해 순서 변경
            CArray<double, double> aElaModTemp; aElaModTemp.RemoveAll();
            CArray<double, double> aSectWTemp; aSectWTemp.RemoveAll();
            CArray<double, double> aNuTemp;  aNuTemp.RemoveAll();
            for (int i = 0; i < aElaMod.GetSize(); i++)
            {
                int nInx = i;
                if      (i == 0) nInx = 1;
                else if (i == 1) nInx = 0;
                aElaModTemp.Add(aElaMod[nInx]);
                aSectWTemp.Add(aSectW[nInx]);
                aNuTemp.Add(aNu[nInx]);
            }
            aElaMod.Copy(aElaModTemp);
            aSectW.Copy(aSectWTemp);
            aNu.Copy(aNuTemp);
        }
    }
    else
    {
        for (int i = 0; i < _ncombined; ++i)  _comb_elem[i] = aDomainElemCount[i];
    }
    int* p_comb_elem = _comb_elem.GetData();

    double _E = dE;       double* p_E = &_E;        // 환산단면의 기준이 되는 탄성계수
    double _NU = dNu;     double* p_NU = &_NU;      // 환산단면의 기준이 되는 푸아송비
    //double _W = dW;       double* p_W = &_W;        // 환산단면의 기준이 되는 단위중량

    double* p_sect_e = aElaMod.GetData();           // 합성단면의 단면별 탄성계수 (array size = p_nprop)
    double* p_sect_nu = aNu.GetData();              // 합성단면의 단면별 푸아송비 (array size = p_nprop)

    int _nrebar = 0;    int* p_nrebar = &_nrebar;   // 철근 개수
    CArray<double, double> _asr; _asr.RemoveAll();  _asr.Add(0.0);  // _asr.SetSize(_nrebar);
    double* p_asr = _asr.GetData();                 // 철근의 반지름
    CArray<double, double> _dsy; _dsy.RemoveAll();  _dsy.Add(0.0);  // _dsy.SetSize(_nrebar);
    double* p_dsy = _dsy.GetData();                 // 철근이 위치한 y좌표
    CArray<double, double> _dsz; _dsz.RemoveAll();  _dsz.Add(0.0);  // _dsz.SetSize(_nrebar);
    double* p_dsz = _dsz.GetData();                 // 철근이 위치한 z좌표
    CArray<double, double> _ES; _ES.RemoveAll();  _ES.Add(0.0);  // _ES.SetSize(_nrebar);
    double* p_ES = _ES.GetData();                   // 철근의 탄성계수 (array size = p_nrebar)

    int _iref = 0;      int* p_iref = &_iref;           // 0 = 도심기준으로 cdef출력, 1=전단중심 기준으로 cdef 출력
    int _elemtype = 0;  int* p_elemtype = &_elemtype;   // 요소 종류 0 = 고차 요소, 1= 저차요소

    // 열팽창계수.
    int nload = 0;    // beam section temperature 개수
    int* p_nload = &nload;

    ctex.RemoveAll();
    for (/*int*/ i=0; i<nCountDomain; ++i )   ctex.Add(0.0);    // 온도하중이 없더라도 property개수만큼 메모리 잡아주시는게 좋을것 같습니다 (from 박건태)
    double* p_ctex = ctex.GetData();

    CMapEx<int, int, int, int> mPos4Domain2; mPos4Domain2.RemoveAll();
    if (nCombinedNum !=1 )
    {
        mPos4Domain2.InitHashTable(rnPoint*0.5+1);

        if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_2C )
        {
            mPos4Domain2.SetAt(1, 0);
            mPos4Domain2.SetAt(9, 0);
            mPos4Domain2.SetAt(8, 0);
            mPos4Domain2.SetAt(12, 0);
            mPos4Domain2.SetAt(13, 0);
            mPos4Domain2.SetAt(14, 0);
            mPos4Domain2.SetAt(5, 0);
            mPos4Domain2.SetAt(18, 0);
            mPos4Domain2.SetAt(20, 0);
            mPos4Domain2.SetAt(21, 0);
            mPos4Domain2.SetAt(22, 0);
            mPos4Domain2.SetAt(4, 0);
            mPos4Domain2.SetAt(26, 0);
            mPos4Domain2.SetAt(7, 0);
        }
        else if ( SectD.SectBefore.Shape==D_SECT_SHAPE_REG_2L )
        {
            mPos4Domain2.SetAt(1, 0);
            mPos4Domain2.SetAt(9, 0);
            mPos4Domain2.SetAt(8, 0);
            mPos4Domain2.SetAt(12, 0);
            mPos4Domain2.SetAt(13, 0);
            mPos4Domain2.SetAt(14, 0);
            mPos4Domain2.SetAt(5, 0);
            mPos4Domain2.SetAt(18, 0);
            mPos4Domain2.SetAt(4, 0);
            mPos4Domain2.SetAt(7, 0);
        }
        else ASSERT(0);
    }

    int nSizeStress = ( 4*rnPoint )*rnProp + 6;   // rnProp(NPROP)는 sect_e가 0이 아닌 것의 개수
    double* stress = new double[nSizeStress];
    memset(stress, 0, sizeof(double)*nSizeStress);

    int nResult4Point = 13;

    raStress.RemoveAll();
    raStress.SetSize(rnPoint*nResult4Point);

    int nStressByPoint = 4*rnProp;
    int nPointStress = 4*rnPoint;       // (index + normal stress + shear xy + shear xz) X 응력 point 개수
    int nStress = nPointStress*rnProp;  // nPointStress X 재질 갯수

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 단위 축력(1N)에 대한 normal stress
    for ( int i = 0; i<8; i++ ) force[i] = 0.0;
    force[0] = 1.0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // PBeamDll에 넘기는 Param을 파일로 씀
    BOOL bWriteBSPDetailToFile = FALSE;
    if (CTestEnvMgr::IsTestEnvST(_T("WriteBSPDetailToFile"), _T("yes"))) bWriteBSPDetailToFile = TRUE;
    if (bWriteBSPDetailToFile)
    {
        CString strPostFix;   strPostFix.Format(_T("_%s_%d"), SectD.SName, nTypeBeforeAfter);

        WritePBeamDllParamToFileDetail(force, nSizeStress, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
            p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem, 
            p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar, 
            p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype, strPostFix);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    int nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    for ( i=0; i<nStress; ++i )
    {
        if ( mPos4Domain2.Exist(i/4%rnPoint+1) )
        {
            if ( i<nPointStress ) continue;
        }
        else
        {
            if ( i>=nPointStress ) continue;
        }
        if ( i%4 != 1 ) continue; // normal stress만 사용
        int nIndex = i/4%rnPoint*nResult4Point;
        raStress.SetAt(nIndex, stress[i]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 단위 모멘트(1N*m) My에 대한 normal stress
    for ( int i = 0; i<8; i++ ) force[i] = 0.0;
    force[4] = 1.0;

    nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    for ( i=0; i<nStress; ++i )
    {
        if ( mPos4Domain2.Exist(i/4%rnPoint+1) )
        {
            if ( i<nPointStress ) continue;
        }
        else
        {
            if ( i>=nPointStress ) continue;
        }
        if ( i%4 != 1 ) continue; // normal stress만 사용
        int nIndex = i/4%rnPoint*nResult4Point+1;
        raStress.SetAt(nIndex, stress[i]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 단위 모멘트(1N*m) Mz에 대한 normal stress
    for ( int i = 0; i<8; i++ ) force[i] = 0.0;
    force[5] = -1.0;

    nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    for ( i=0; i<nStress; ++i )
    {
        if ( mPos4Domain2.Exist(i/4%rnPoint+1) )
        {
            if ( i<nPointStress ) continue;
        }
        else
        {
            if ( i>=nPointStress ) continue;
        }
        if ( i%4 != 1 ) continue; // normal stress만 사용
        int nIndex = i/4%rnPoint*nResult4Point+2;
        raStress.SetAt(nIndex, stress[i]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 단위 Mb(Bi-Moment)에 대한 normal stress
    for ( int i = 0; i<8; i++ ) force[i] = 0.0;
    force[6] = 1.0;

    nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    for ( i=0; i<nStress; ++i )
    {
        if ( mPos4Domain2.Exist(i/4%rnPoint+1) )
        {
            if ( i<nPointStress ) continue;
        }
        else
        {
            if ( i>=nPointStress ) continue;
        }
        if ( i%4 != 1 ) continue; // normal stress만 사용
        int nIndex = i/4%rnPoint*nResult4Point+3;
        raStress.SetAt(nIndex, stress[i]);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 단위 전단력에 대한 전단응력 TRANSVERSE SHEAR xy, TRANSVERSE SHEAR xz BY Fy
    for ( int i = 0; i<8; i++ ) force[i] = 0.0;
    force[1] = 1.0;

    nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    for ( i=0; i<nStress; ++i )
    {
        if ( mPos4Domain2.Exist(i/4%rnPoint+1) )
        {
            if ( i<nPointStress ) continue;
        }
        else
        {
            if ( i>=nPointStress ) continue;
        }
        if ( i%4 == 2 )
        {
            int nIndex = i/4%rnPoint*nResult4Point+4;
            raStress.SetAt(nIndex, stress[i]);
        }
        else if ( i%4 == 3 )
        {
            int nIndex = i/4%rnPoint*nResult4Point+10;
            raStress.SetAt(nIndex, stress[i]);
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 단위 전단력에 대한 전단응력 TRANSVERSE SHEAR xz, TRANSVERSE SHEAR xy BY Fz
    for ( int i = 0; i<8; i++ ) force[i] = 0.0;
    force[2] = 1.0;

    nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    for ( i=0; i<nStress; ++i )
    {
        if ( mPos4Domain2.Exist(i/4%rnPoint+1) )
        {
            if ( i<nPointStress ) continue;
        }
        else
        {
            if ( i>=nPointStress ) continue;
        }
        if ( i%4 == 3 )
        {
            int nIndex = i/4%rnPoint*nResult4Point+7;
            raStress.SetAt(nIndex, stress[i]);
        }
        else if ( i%4 == 2 )
        {
            int nIndex = i/4%rnPoint*nResult4Point+11;
            raStress.SetAt(nIndex, stress[i]);
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 단위 Mt에 대한 비틀림응력
    for ( int i = 0; i<8; i++ ) force[i] = 0.0;
    force[3] = 1.0;

    nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    for ( i=0; i<nStress; ++i )
    {
        if ( mPos4Domain2.Exist(i/4%rnPoint+1) )
        {
            if ( i<nPointStress ) continue;
        }
        else
        {
            if ( i>=nPointStress ) continue;
        }
        if ( i%4 == 2 )
        {
            int nIndex = i/4%rnPoint*nResult4Point+5;
#ifdef _MEC
            raStress.SetAt(nIndex, stress[i] * pSectStiffness->Rxx);
#else
            raStress.SetAt(nIndex, stress[i]);
#endif
        }
        else if ( i%4 == 3 )
        {
            int nIndex = i/4%rnPoint*nResult4Point+8;
#ifdef _MEC
            raStress.SetAt(nIndex, stress[i] * pSectStiffness->Rxx);
#else
            raStress.SetAt(nIndex, stress[i]);
#endif
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 단위 Mw에 대한 와핑비틀림응력
    for ( int i = 0; i<8; i++ ) force[i] = 0.0;
    force[7] = 1.0;

    nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    for ( i=0; i<nStress; ++i )
    {
        if ( mPos4Domain2.Exist(i/4%rnPoint+1) )
        {
            if ( i<nPointStress ) continue;
        }
        else
        {
            if ( i>=nPointStress ) continue;
        }
        if ( i%4 == 2 )
        {
            int nIndex = i/4%rnPoint*nResult4Point+6;
            raStress.SetAt(nIndex, stress[i]);
        }
        else if ( i%4 == 3 )
        {
            int nIndex = i/4%rnPoint*nResult4Point+9;
            raStress.SetAt(nIndex, stress[i]);
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Warping
    BSPCombMain* pBSPCombMain;
    pBSPCombMain = ( BSPCombMain* ) GetProcAddress(hDllCalc, "BSPCombMain");
    if ( pBSPCombMain==NULL ) { ASSERT(0); return FALSE; }

    double* pbeam = NULL;
    double* cdef = NULL;
    int nDataPBeam = 38+rnPoint*3+5;
    pbeam = new double[nDataPBeam];
    memset(pbeam, 0, sizeof(double)*nDataPBeam);
    int nDataCDEF = rnPoint*2;
    cdef = new double[nDataCDEF];
    memset(cdef, 0, sizeof(double)*nDataCDEF);

    double _W = dW;       double* p_W = &_W;        // 환산단면의 기준이 되는 단위중량
    double* p_sect_w = aSectW.GetData();            // 합성단면의 단면별 단위중량 (array size = p_nprop)
    CArray<double, double> _WS; _WS.RemoveAll();  _WS.Add(0.0);  // _WS.SetSize(_nrebar);
    double* p_WS = _WS.GetData();                   // 철근의 단위중량 (array size = p_nrebar)
    int _ioptn = 2;     int* p_ioptn = &_ioptn;         // 0 = 1~22+33+34  항목만 계산; 1= 1~34까지만 계산; 2=모두 계산


    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // PBeamDll에 넘기는 Param을 파일로 씀
    BOOL bWritePBeamDllParamToFile = FALSE;
    if (CTestEnvMgr::IsTestEnvST(_T("7thDOFWritePBeamDllParamToFile"), _T("yes"))) bWritePBeamDllParamToFile = TRUE;
    if (bWritePBeamDllParamToFile)
    {
        CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);

        if      (nTypeBeforeAfter == 0) strPostFix += _T("_EFF0");
        else if (nTypeBeforeAfter == 1) strPostFix += _T("_EFF1");
        else if (nTypeBeforeAfter == 2) strPostFix += _T("_EFF2");

        WritePBeamDllParamToFileNew(cdef, 0, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
            p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
            p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
            p_asr, p_dsy, p_dsz, p_ES, p_WS, p_ioptn, p_iref, p_elemtype, strPostFix);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    nResult = ( *pBSPCombMain )( pbeam, cdef, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_WS, p_ioptn, p_iref, p_elemtype );

    int nIndex=-1;
    double dWarp  = 0.;

    dWarp = pbeam[39+nIndex];  raStress.SetAt(1*nResult4Point-1, dWarp); // 	w1
    dWarp = pbeam[40+nIndex];  raStress.SetAt(2*nResult4Point-1, dWarp); // 	w2
    dWarp = pbeam[41+nIndex];  raStress.SetAt(3*nResult4Point-1, dWarp); // 	w3
    dWarp = pbeam[42+nIndex];  raStress.SetAt(4*nResult4Point-1, dWarp); // 	w4

    for ( int k=0; k<nPoint-4; ++k )
    {
        dWarp = pbeam[42+( k+1 )+nIndex];
        raStress.SetAt(( k+5 )*nResult4Point-1, dWarp);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    if ( pointy ) { delete[] pointy; }
    if ( pointz ) { delete[] pointz; }
    delete[] stress;
    delete[] pbeam;
    delete[] cdef;
    return TRUE;
}

// @param force : [in] 1=AXIAL
//                     2=SHEAR Y
//                     3=SHEAR Z
//                     4=PURE TORSION
//                     5=MOMENT Y
//                     6=MOMENT Z
//                     7=BI-MOMENT
//                     8=WARPING TORSION
//                  reference axis에 대한 axial, shear y, shear z, moment y, moment z, torsion, bimoment.
//                  6자유도일 경우 pure torsion 자리에 Mx를 넣으면 됨.
// @param bst : [in] beam section temperature (8 x nload)
//                   1=방향 (0: Y, 1: Z)
//                   2=탄성계수
//                   3=열팽창계수
//                   4=B (reference axis를 기준으로 H1 값)
//                   5=H1
//                   6=T1 (reference axis를 기준으로 H2 값)
//                   7=H2
//                   8=T2
// @param stress : [out] 1=PROPERTY INDEX
//                       2=NORMAL STRESS (AXIAL + BENDING + WARPING)
//                       3=SHEAR XY (TRANSVERSE SHEAR XY + PURE TORSIONAL SHEAR STRESS XY + WARPING TORSIONAL SHEAR STRESS XY)
//                       4=SHEAR XZ (TRANSVERSE SHEAR XZ + PURE TORSIONAL SHEAR STRESS XZ + WARPING TORSIONAL SHEAR STRESS XZ)
//                 inner domain을 제외한 domain 개수만큼 응력을 출력
//                 응력 결과 : (4 x (NPOINT))x NPROP+6.
//                             * NPROP는 sect_e가 0이 아닌 것의 개수
//                             * 마지막 6개는 max, min 결과
//                             1: 1 번 point의 sect_e값 index
//                             2: 1 번 point의 nomal stress
//                             3: 1 번 point의 shaer xy
//                             4: 1 번 point의 shaer xz
//                             5: 2 번 point의 sect_e값 index
//                             6: 2 번 point의 nomal stress
//                             7: 2 번 point의 shaer xy
//                             8: 2 번 point의 shaer xz
//                             ...
//                             마지막 6개는 max, min 결과
// @param coordy : [out] max/min 값에 대한 y좌표
//                       1=max normal stress
//                       2=max shear xy
//                       3=max shear xz
//                       4=min normal stress
//                       5=min shear xy
//                       6=min shear xz
// @param coordz : [out] max/min 값에 대한 z좌표
//                       1=max normal stress
//                       2=max shear xy
//                       3=max shear xz
//                       4=min normal stress
//                       5=min shear xy
//                       6=min shear xz
// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CDb_DllBaseManager::Get_SectionPropertyMesh_BeamDetail(T_SECT_D& rData, int nPos,
    int nTypeBeforeAfter,
    double dMatlValue[6],
    double force[8],
    //int nload,        // beam section temperature 개수
    //double* bst,      // beam section temperature (8 x nload)
    int& rnStress,    // count of stress
    double coordy[6],
    double coordz[6],
    int& rnPoint,     // count of stress point
    int& rnProp,      // count of domain (sect_e가 0이 아닌 것의 개수)
    CArray<double, double>& raStress,
    CArray<double, double>& raPointY,
    CArray<double, double>& raPointZ)
{
    T_SECT_D SectD = rData;

    // 열팽창계수.
    int nload = 0;    // beam section temperature 개수
    CArray<double, double> ctex;   ctex.RemoveAll();   // 각 도메인별 열팽창계수, 도메인 개수만큼 (inner line인 경우 0)
    double* bst = NULL;

    // 단면 형상/메쉬 얻어오기.
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD;    aMeshElemD.RemoveAll();
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD;    aMeshNodeD.RemoveAll();
	std::vector<int> aDomainElemCount;                   aDomainElemCount.clear();
    CArray<int, int> aDomainMaterialType;                aDomainMaterialType.RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입
    CSimpleMesh simpleMesh;

    // 파형강판 예외 처리. by Hsshim.
    // 기존에는 파형강판에 대해서 탄성계수비(Es/Ec)를 고려해서 환산한 면적으로 Mesh가 되는 문제가 있어서,
    // 형상을 그대로 고려할 수 있도록 탄성계수비를 1.0으로 치환해서 Mesh를 얻어오도록 수정
    double dMatl_Elast_back = SectD.SectBefore.Matl_Elast;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = 1.0; }
    if ( !simpleMesh.GetSectMesh(SectD, nPos, nTypeBeforeAfter, aMeshElemD, aMeshNodeD, aDomainElemCount, aDomainMaterialType, FALSE, NULL) ) { return FALSE; }
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = dMatl_Elast_back; }

    // 단면 속성 계산
    int nCountDomain = aDomainElemCount.size();

    HINSTANCE hDllCalc = GetOrLoad_PBeamLMeshDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    T_SECT_STIFFNESS* pSectStiffness = NULL;
    if ( nPos==0 ) pSectStiffness = &SectD.SectBefore.SectI.Stiffness;
    else        pSectStiffness = &SectD.SectBefore.SectJ.Stiffness;

    if ( IsUseStiffBySectBefore(rData) ) pSectStiffness = &rData.SectBefore.Stiffness;

    int nDomain = nCountDomain;

    int nPoint = 4;
    if ( IsSect_PSC(SectD) ) nPoint = 10;

    double* pointy = NULL;
    double* pointz = NULL;

    BOOL bUserPoint = FALSE;
    ASSERT(raPointY.GetSize() == raPointZ.GetSize());
    if ( raPointY.GetSize() > 0 )
    {
        nPoint = raPointY.GetSize();
        bUserPoint = TRUE;
    }

    pointy = new double[nPoint];
    pointz = new double[nPoint];
    memset(pointy, 0, sizeof(double)*nPoint);
    memset(pointz, 0, sizeof(double)*nPoint);

    if ( bUserPoint )
    {
        for ( int i=0; i<nPoint; ++i )
        {
            pointy[i] = raPointY[i];
            pointz[i] = raPointZ[i];
        }
    }
    else
    {
        pointy[0] = pSectStiffness->dx1;
        pointy[1] = pSectStiffness->dx2;
        pointy[2] = pSectStiffness->dx3;
        pointy[3] = pSectStiffness->dx4;
        // 응력계산위치의 좌표계와 형상 좌표계를 맞추기 위하여.. X(-1)
        //pointy[0] = (-1)*pSectStiffness->dx1;
        //pointy[1] = (-1)*pSectStiffness->dx2;
        //pointy[2] = (-1)*pSectStiffness->dx3;
        //pointy[3] = (-1)*pSectStiffness->dx4;

        pointz[0] = pSectStiffness->dy1;
        pointz[1] = pSectStiffness->dy2;
        pointz[2] = pSectStiffness->dy3;
        pointz[3] = pSectStiffness->dy4;

        if ( nPoint==10 )
        {
            T_SPSC_DSHR SpscDshr; SpscDshr.Initialize();
            CSectUtil::GetPscShearPosi(&SectD, nPos==0, SpscDshr, FALSE);

            pointy[4] = SpscDshr.dyz[0][0];
            pointy[5] = SpscDshr.dyz[0][1];
            pointy[6] = SpscDshr.dyz[0][2];
            pointy[7] = SpscDshr.dyz[0][3];
            pointy[8] = SpscDshr.dyz[0][4];
            pointy[9] = SpscDshr.dyz[0][5];
            //       pointy[4] = (-1)*SpscDshr.dyz[0][0];
            //       pointy[5] = (-1)*SpscDshr.dyz[0][1];
            //       pointy[6] = (-1)*SpscDshr.dyz[0][2];
            //       pointy[7] = (-1)*SpscDshr.dyz[0][3];
            //       pointy[8] = (-1)*SpscDshr.dyz[0][4];
            //       pointy[9] = (-1)*SpscDshr.dyz[0][5];

            pointz[4] = SpscDshr.dyz[1][0];
            pointz[5] = SpscDshr.dyz[1][1];
            pointz[6] = SpscDshr.dyz[1][2];
            pointz[7] = SpscDshr.dyz[1][3];
            pointz[8] = SpscDshr.dyz[1][4];
            pointz[9] = SpscDshr.dyz[1][5];
        }
    }

    rnProp  = nDomain;
    rnPoint = nPoint;
    int i=0;
    raPointY.SetSize(nPoint);  for (/*int*/ i=0; i<nPoint; ++i )  raPointY.SetAt(i, pointy[i]);
    raPointZ.SetSize(nPoint);  for (/*int*/ i=0; i<nPoint; ++i )  raPointZ.SetAt(i, pointz[i]);

    double dE = dMatlValue[0];
    double dW = dMatlValue[2];
    double dNu = dMatlValue[4];
    CArray<double, double> aElaMod; aElaMod.RemoveAll();
    CArray<double, double> aSectW; aSectW.RemoveAll();
    CArray<double, double> aNu; aNu.RemoveAll();
    BOOL bByConc = FALSE;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) bByConc = TRUE;    // PSC 파형단면(D_SECT_SHAPE_PSC_CMPWEB)은 Conc 기준으로 계산.
    if ( !GetMatl4Domain(dMatlValue, nDomain, aDomainMaterialType, dE, dW, dNu, aElaMod, aSectW, aNu, bByConc) )
    {
        ASSERT(0);
        delete[] pointy; pointy=NULL;
        delete[] pointz; pointz=NULL;
        return FALSE;
    }

    double dCentY = 0.0;
    double dCentZ = 0.0;
    if ( CSimpleMesh::CalcCentroid(dCentY, dCentZ, aDomainElemCount, aMeshElemD, aMeshNodeD, aElaMod) )
    {
        // 도심기준의 응력 위치를 Mesh 좌표 기준으로 환산
        for ( int k=0; k<nPoint; ++k )
        {
            pointy[k] += dCentY;
            pointz[k] += dCentZ;
        }
    }
    else { ASSERT(0); }

    BSPDetail* pBSPDetail;
    pBSPDetail = ( BSPDetail* ) GetProcAddress(hDllCalc, "BSPDetail");
    if ( pBSPDetail==NULL ) { ASSERT(0); return FALSE; }

    // BSPDetail() 함수의 입력 파라미터
    int _nnode = aMeshNodeD.GetSize();    int* p_nnode = &_nnode;           // 총 절점수
    int _npoint = nPoint;                 int* p_npoint = &_npoint;         // 응력계산위치 개수
    int _nelem = aMeshElemD.GetSize();    int* p_nelem = &_nelem;           // 총 요소수
    int _nprop = nCountDomain;            int* p_nprop = &_nprop;           // 총 material 수
    int _ncombined = nCountDomain;        int* p_ncombined = &_ncombined;   // 연결되지 않은 단면의 수

    // 절점의 좌표
    // * 주의사항 (솔버에서 y-축 :수직, z-축 :수평으로 계산하고 있어서, 좌표는 y,z를 바꿔서 넘겨야 함.)
    CArray<double, double> aYNode; aYNode.RemoveAll();  aYNode.SetSize(_nnode);
    CArray<double, double> aZNode; aZNode.RemoveAll();  aZNode.SetSize(_nnode);
    double dY=0., dZ=0.;
    for (/*int*/ i=0; i<_nnode; i++ )
    {
        dY = aMeshNodeD[i].x * ( -1 ); // 솔버 좌표계와 맞추기 위해
        //dY = aMeshNodeD[i].x; // 응력계산위치의 좌표계와 맞추기 위하여..
        dZ = aMeshNodeD[i].y;

        aYNode.SetAt(i, dY);
        aZNode.SetAt(i, dZ);
    }
    double* p_ynode = aZNode.GetData();   // 절점의 y좌표
    double* p_znode = aYNode.GetData();   // 절점의 z좌표

    double* p_pointy = pointz;    // 응력계산 위치의 y좌표
    double* p_pointz = pointy;    // 응력계산 위치의 z좌표

    // 요소의 절점 연결 번호 (array size = 요소수 * 3, 저차 삼각형요소, 절점의 최초 시작은 1부터)
    CArray<int, int> _iconn;    _iconn.RemoveAll();    _iconn.SetSize(_nelem*3);    // 삼각형 요소.
    int iConnNum = 0;
    for (/*int*/ i=0; i<_nelem; ++i )
    {
        if ( aMeshElemD[i].aNodeK.GetSize() != 3 ) { ASSERT(0); return FALSE; }

        _iconn[iConnNum] = aMeshElemD[i].aNodeK[0] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[1] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[2] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
    }
    ASSERT(iConnNum == _iconn.GetSize());
    int* p_iconn = _iconn.GetData();

    // 요소 갯수 검증
    int nDomainElemSum = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )  nDomainElemSum += aDomainElemCount[i];
    ASSERT(nDomainElemSum == _nelem);

    // 요소의 재료 index (array size =  요소수)
    CArray<int, int> _elem_prop;    _elem_prop.RemoveAll();    _elem_prop.SetSize(_nelem);
    int iElem = 0;
    for (/*int*/ i=0; i<aDomainElemCount.size(); ++i )
    {
        for ( int k=0; k<aDomainElemCount[i]; ++k )
        {
            _elem_prop[iElem] = i+1;    // p_sect_e, p_sect_nu, p_sect_w의 참조 index (fortran이기 때문에 1부터 시작). 사실상 소속 domain index
            iElem++;
        }
    }
    int* p_elem_prop = _elem_prop.GetData();

    // combined section의 노드수 (array size = p_ncombined)
    CArray<int, int> _comb_node;    _comb_node.RemoveAll();    _comb_node.SetSize(_ncombined);
    int iStart = 0;
    int iEndCount = 0;
    for (/*int*/ i=0; i<_ncombined; ++i )
    {
        if ( i>0 ) iStart += aDomainElemCount[i-1];
        iEndCount = iStart+aDomainElemCount[i];

        CMapEx<int, int, int, int> mNodeOfDomain;    mNodeOfDomain.RemoveAll();

        // 개별 Domain에 대한 loop
        for ( int k=iStart; k<iEndCount; ++k )
        {
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[0], 0);
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[1], 0);
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[2], 0);
        }
        _comb_node[i] = mNodeOfDomain.GetCount();
    }
    int* p_comb_node = _comb_node.GetData();

    // combined section의 요소수 (array size = p_ncombined)
    CArray<int, int> _comb_elem;    _comb_elem.RemoveAll();    _comb_elem.SetSize(_ncombined);
    for (/*int*/ i=0; i<_ncombined; ++i )  _comb_elem[i] = aDomainElemCount[i];
    int* p_comb_elem = _comb_elem.GetData();

    double _E = dE;       double* p_E = &_E;        // 환산단면의 기준이 되는 탄성계수
    double _NU = dNu;     double* p_NU = &_NU;      // 환산단면의 기준이 되는 푸아송비
    //double _W = dW;       double* p_W = &_W;        // 환산단면의 기준이 되는 단위중량

    double* p_sect_e = aElaMod.GetData();           // 합성단면의 단면별 탄성계수 (array size = p_nprop)
    double* p_sect_nu = aNu.GetData();              // 합성단면의 단면별 푸아송비 (array size = p_nprop)
    //double* p_sect_w = aSectW.GetData();            // 합성단면의 단면별 단위중량 (array size = p_nprop)

    int _nrebar = 0;    int* p_nrebar = &_nrebar;   // 철근 개수
    CArray<double, double> _asr; _asr.RemoveAll();  _asr.Add(0.0);  // _asr.SetSize(_nrebar);
    double* p_asr = _asr.GetData();                 // 철근의 반지름
    CArray<double, double> _dsy; _dsy.RemoveAll();  _dsy.Add(0.0);  // _dsy.SetSize(_nrebar);
    double* p_dsy = _dsy.GetData();                 // 철근이 위치한 y좌표
    CArray<double, double> _dsz; _dsz.RemoveAll();  _dsz.Add(0.0);  // _dsz.SetSize(_nrebar);
    double* p_dsz = _dsz.GetData();                 // 철근이 위치한 z좌표
    CArray<double, double> _ES; _ES.RemoveAll();  _ES.Add(0.0);  // _ES.SetSize(_nrebar);
    double* p_ES = _ES.GetData();                   // 철근의 탄성계수 (array size = p_nrebar)
    //CArray<double, double> _WS; _WS.RemoveAll();  _WS.Add(0.0);  // _WS.SetSize(_nrebar);
    //double* p_WS = _WS.GetData();                   // 철근의 단위중량 (array size = p_nrebar)

    //int _ioptn = 2;     int* p_ioptn = &_ioptn;         // 0 = 1~22+33+34  항목만 계산; 1= 1~34까지만 계산; 2=모두 계산
    int _iref = 0;      int* p_iref = &_iref;           // 0 = 도심기준으로 cdef출력, 1=전단중심 기준으로 cdef 출력
    int _elemtype = 0;  int* p_elemtype = &_elemtype;   // 요소 종류 0 = 고차 요소, 1= 저차요소

    int* p_nload = &nload;

    ctex.RemoveAll();
    for (/*int*/ i=0; i<nCountDomain; ++i )   ctex.Add(0.0);    // 온도하중이 없더라도 property개수만큼 메모리 잡아주시는게 좋을것 같습니다 (from 박건태)
    double* p_ctex = ctex.GetData();

    //--------------------------------------------------------------
    // Mesh 검증용 mct 생성.
    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    if ( bDumpSectMeshFile )  WriteMctOfMesh2(_nnode, p_ynode, p_znode, strPostFix);
    //--------------------------------------------------------------

    int nSizeStress = ( 4*rnPoint )*rnProp + 6;   // rnProp(NPROP)는 sect_e가 0이 아닌 것의 개수
    double* stress = new double[nSizeStress];
    memset(stress, 0, sizeof(double)*nSizeStress);

    int nResult = ( *pBSPDetail )( force, stress, coordy, coordz, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
        p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
        p_E, p_NU, p_sect_e, p_sect_nu, p_nrebar,
        p_asr, p_dsy, p_dsz, p_ES, p_iref, p_nload, bst, p_ctex, p_elemtype );

    int nPointStress = 4*rnPoint;       // (index + normal stress + shear xy + shear xz) X 응력 point 갯수
    int nStress = nPointStress*rnProp;  // nPointStress X 재질 갯수
    int nMinMaxStress = 6;
    raStress.RemoveAll();

    int nSectIndex =1;
    for ( i=0; i<nStress; ++i )
    {
        //if (i%rnProp == 0) raStress.Add(stress[i]);   // 첫번째 재질의 결과의 응력 point 위치 결과들만 취합 (중복 위치는 반환 안함)
        if ( i%4 == 0 ) nSectIndex = stress[i];

        if ( nSectIndex==rnProp ) // 첫번째 재질의 결과만 저장
        {
            raStress.Add(stress[i]);
        }
    }
    for ( i=0; i<nMinMaxStress; ++i )
        raStress.Add(stress[nStress+i]);              // 제일 마지막 위치가 min/max 결과

    if ( pointy ) { delete[] pointy; }
    if ( pointz ) { delete[] pointz; }
    delete[] stress;

    return TRUE;
}

double CDb_DllBaseManager::GetDistance(T_POINT3D& pt1, T_POINT3D& pt2)
{
    return CMathFunc::mathLength(pt1.x, pt1.y, pt1.z, pt2.x, pt2.y, pt2.z);
}

double CDb_DllBaseManager::GetDistance(XGL_3dp& pt1, XGL_3dp& pt2)
{
    return CMathFunc::mathLength(pt1.x(), pt1.y(), pt1.z(), pt2.x(), pt2.y(), pt2.z());
}

BOOL CDb_DllBaseManager::GetSectTopologyType(T_SECT_D& rData, int& nTopologyType)
{
    ASSERT(0); // CDb_DllBaseManager::Get_PlasticSectionModulusMesh 의 if(bCalcByMesh) 쪽 주석 참고 하시오

    // nTopologyType = 1 : 속이 차 있는 Polygon
    //                 2 : 속이 차 있는 Polygon여러개
    //                 3 : 속이 비어있는 Polygon
    //                 4 : Box With Stiffener또는 Pipe With Sitffener
    int nStype;
    nStype = rData.nStype == D_SECT_TYPE_TAPERED ? rData.SectBefore.nStype : rData.nStype;
    if ( nStype != D_SECT_TYPE_REGULAR )
    {
        return FALSE; //일단 재료비선형에서 지원하는 녀석들만 구현 함.
    }

	if ( rData.SectBefore.Shape == D_SECT_SHAPE_COMPO_B )
		nTopologyType = 1;
	else if ( rData.SectBefore.Shape == D_SECT_SHAPE_COMPO_I )
		nTopologyType = 1;
	else if ( rData.SectBefore.Shape == D_SECT_SHAPE_COMPO_TUB )
		nTopologyType = 1;
	else if ( rData.SectBefore.Shape == D_SECT_SHAPE_STLG_I )
		nTopologyType = 1;
	else if ( rData.SectBefore.Shape == D_SECT_SHAPE_STLG_B )
		nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_L ) // Angle
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_C ) // Channel
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_H ) // H-Section
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_T ) // T-Section
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_B ) // Box
        nTopologyType = 3;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_P ) // Pipe
        nTopologyType = 3;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_SB ) // Solid Rectangle
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_SR ) // Solide Round
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_2L ) // Double Angle
        nTopologyType = 2;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_2C ) // Double Channel
        nTopologyType = 2;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_2CB ) // Double Channel (box type)
        nTopologyType = 2;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_CC ) // Cold Formed Channel
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_URIB ) // U-RIB
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_OCT ) // Octagon
        nTopologyType = 3;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT ) // Solide Octagon
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_TRK ) // Trac
        nTopologyType = 3;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_STRK ) // Solid Trac
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK ) // Half Trac
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_CL ) // Cross Angle
    {
        ASSERT(0);
        nTopologyType = 1;
    }
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT ) // R-Octagon
        nTopologyType = 3;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF ) // Box with stiffener
        nTopologyType = 4;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF ) // Pipe with stiffener
        nTopologyType = 4;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_GEN ) // General Section (Value에서만 사용)
    {
        ASSERT(0);
        return FALSE;
    }
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_UDT ) // Inverted T-Section
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_CB )
    {
        ASSERT(0);
        nTopologyType = 1;
    }
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_UP ) // Upright
        nTopologyType = 1;
    else if ( rData.SectBefore.Shape == D_SECT_SHAPE_REG_Z ) // Z
    {
        ASSERT(0);
        nTopologyType = 1;
    }
    else
    {
        ASSERT(0);
        return FALSE;
    }

    return TRUE;
}

BOOL CDb_DllBaseManager::Get_PlasticSectionModulusMesh(T_SECT_D& rData, double dMatlValue[6], int nPos, double& dZpy, double& dZpz, BOOL bCalcByMesh)
{
    double dZeroForCutLine=1.0E-07;
    double dTol = 1.0E-05;

    dZpy = dZpz = 0.0;

    // Logic : 단면을 Centroid기준으로 둘로 나누어서 단면1차모멘트를 계산한 다음 합하여 PlasticSectionModulus를 계산 함. 나누어진 단면의 단면1차모멘트 계산을 위한 좌표는 자르기 전 단면의 Centroid임.
    //         무조건 I단에 대하여 계산합니다.
//     if ( rData.nStype != D_SECT_TYPE_REGULAR )
//     {
//         // Tappered 단면은 일반단면으로 변환해서 이 함수를 불러야 한다. 이유는 simpleMesh.GetSectMesh에서 Tappered일때 Mid지점기준으로 Polygon을 계산하기 때문이다.
//         return FALSE; // hslee :  General하게 만들어진 함수이지만 여기에 ASSERT 걸리는 단면은 Test하지 않았습니다.. 다른 단면을 추가 한다면 Stage고려, Matl이 여러가지일 경우 dMatlValue에 대한 처리 고려만 하면 잘 돌아 갈 것 같습니다.
//     }

    ASSERT(nPos == 0); // hslee : j단에 대한 계산은 테스트 해 보지 않았습니다.

    int nTypeBeforeAfter = 0; // hslee : Composite을 추가 해야 한다면 이놈을 매개변수로 바꾸고 Stage, Matl등이 고려되도록 처리 하세요

    T_SECT_D SectD = rData;

    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF )
    {
        SectD.SectBefore.Shape = D_SECT_SHAPE_REG_B;

        for ( int i = 0; i < D_SECT_SIZE_NUM_MAX; i++ )
        {
            SectD.SectBefore.SectI.Size[i] = 0.0;
        }
        SectD.SectBefore.SectI.Size[0] = rData.SectBefore.SectI.Size[0]; // H
        SectD.SectBefore.SectI.Size[1] = rData.SectBefore.SectI.Size[1]; // B
        SectD.SectBefore.SectI.Size[3] = rData.SectBefore.SectI.Size[2]; // tf
        SectD.SectBefore.SectI.Size[5] = rData.SectBefore.SectI.Size[2]; // tf
        SectD.SectBefore.SectI.Size[2] = rData.SectBefore.SectI.Size[3]; // tw
    }
    else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF )
    {
        SectD.SectBefore.Shape = D_SECT_SHAPE_REG_P;

        for ( int i = 0; i < D_SECT_SIZE_NUM_MAX; i++ )
        {
            SectD.SectBefore.SectI.Size[i] = 0.0;
        }
        SectD.SectBefore.SectI.Size[0] = rData.SectBefore.SectI.Size[0]; // B
        SectD.SectBefore.SectI.Size[1] = rData.SectBefore.SectI.Size[1]; // tw
    }

    if ( !bCalcByMesh )
    {
        ASSERT(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_C || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_H || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T ||
            SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB ||
            SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR); // 여기서 ASSERT 걸리는 단면은 테스트가 덜된 단면입니다... CSectDB::Plastic_*() 함수가 잘 불려지는지, dZpz와 dZpy의 값이 맞는지 확인 해 봐야 합니다.

        CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

        CString strDBName = rData.SectBefore.SectI.DBName;
        rData.SectBefore.SectI.DBName = _T(""); // CalcSectRegular에 DBName이 있으면 return FALSE를 하는데, 그 이유는 DB단면의 경우 이미 제시된 값들이 있어서 형상기준으로 계산하여 덮어쓰면 안되기 때문임..
        // 여기서 CalcSectRegular를 부르는 것은 항상 형상기준으로 계산하기 위해서 이기 때문에 DBName을 잠시 없앤다음에 부름

        pDoc->m_pSectDB->CalcSectRegular(rData); // 주의 : DB단면인 경우 rData의 데이터가 변경이 될 수 있으므로 rData는 꼭 복사본으로 써야 합니다.
        rData.SectBefore.SectI.DBName = strDBName;

        dZpz = pDoc->m_pSectDB->GetZzz();
        dZpy = pDoc->m_pSectDB->GetZyy();
    }
	else
	{
		BOOL bOrigin = m_bAllSectMesh;
		m_bAllSectMesh = FALSE;
		// Mesh를 쪼개서 하려면 비대칭 단면일 경우에 값이 잘못 계산됨
		// 아래 코드는 도심중심으로 잘라서 단면1차모멘트를 더하는데, Area를 등분하는 점을 잘라서 단면 1차모멘트를 더하도록 수정 해야 함.
		// General 한 방법으로 Area를 등분하는 선을 구하는 방법을 모르기 때문에 이 방법은 안쓰는 중임
		// 대칭단면에 대해서는 아래 코드를 써도 될 것 같음..
		// 단면 형상/메쉬 얻어오기.
		CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD;    aMeshElemD.RemoveAll();
		CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD;    aMeshNodeD.RemoveAll();
		std::vector<int> aDomainElemCount;                  aDomainElemCount.clear();
		std::vector<int> aFilter;                           aFilter.clear();
		CArray<int, int> aDomainMaterialType;                aDomainMaterialType.RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입
		CSimpleMesh simpleMesh;
		simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;

		// 파형강판 예외 처리. by Hsshim.
		// 기존에는 파형강판에 대해서 탄성계수비(Es/Ec)를 고려해서 환산한 면적으로 Mesh가 되는 문제가 있어서,
		// 형상을 그대로 고려할 수 있도록 탄성계수비를 1.0으로 치환해서 Mesh를 얻어오도록 수정
		double dMatl_Elast_back = SectD.SectBefore.Matl_Elast;
		if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = 1.0; }
		if ( !simpleMesh.GetSectMesh(SectD, nPos, nTypeBeforeAfter, aMeshElemD, aMeshNodeD, aDomainElemCount, aDomainMaterialType, FALSE, NULL) ) { return FALSE; }
		if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = dMatl_Elast_back; }

		// hslee : AdjustCoordToCentroid를 부르는 부분은 Composite Sect에 대해서 계산 해야 할 경우 살리세요 CDb_DllBaseManager::Get_SectionPropertyMesh를 참고..
		//-----------------------------------------------------------------------------------------------
		// MNET:XXXX-HSSHIM-20130816.
		// part별 속성 계산시, 좌표를 도심 기준 좌표로 변경. (Composite Sect for CS 단면 속성 계산시 이전에 이렇게 하고 있어서 기존 방식을 맞춰주기 위함.)
		//if (!simpleMesh.AdjustCoordToCentroid(nTypeBeforeAfter, aMeshNodeD, pSectDesign))  { ASSERT(0); return FALSE; }
		//-----------------------------------------------------------------------------------------------

		int nCountDomain = aDomainElemCount.size();
		int nDomain = nCountDomain;
		double dE = dMatlValue[0];
		double dW = dMatlValue[2];
		double dNu = dMatlValue[4];
		CArray<double, double> aElaMod; aElaMod.RemoveAll();
		CArray<double, double> aSectW; aSectW.RemoveAll();
		CArray<double, double> aNu; aNu.RemoveAll();
		BOOL bByConc = FALSE;
		if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) bByConc = TRUE;    // PSC 파형단면(D_SECT_SHAPE_PSC_CMPWEB)은 Conc 기준으로 계산.
		if ( !GetMatl4Domain(dMatlValue, nDomain, aDomainMaterialType, dE, dW, dNu, aElaMod, aSectW, aNu, bByConc) ) { ASSERT(0); return FALSE; }

		aFilter.push_back(aDomainElemCount.back());
		// Centroid기준으로 잘라서 1차모멘트 들을 더한다.
		m_bAllSectMesh = bOrigin;
		return CSimpleMesh::CalcPlasticModulus(dZpy, dZpz, aDomainElemCount, aMeshElemD, aMeshNodeD, aElaMod, &aFilter);
	}

		/*
		double dCentY = dCenY;
		double dCentZ = dCenZ;
        if (!CSimpleMesh::CalcCentroid(dCentY, dCentZ, aDomainElemCount, aMeshElemD, aMeshNodeD, aElaMod, &aFilter)) {ASSERT(0); return FALSE;}

        if(fabs(dCentY) < dTol)
        {
			dCentY = 0.0;
        }
        if(fabs(dCentZ) < dTol)
        {
	        dCentZ = 0.0;
        }

        CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdge;
        if(!simpleMesh.GetSectPolygon(SectD, nPos, aaEdge)) return FALSE;

		int nCnt = 0;
		for ( int i = 1; i<aaEdge.GetSize(); ++i )
		{
			//0번은 슬라브
			CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&>& aEdge = aaEdge[i].aAr;

			for ( int j = 0; j<aEdge.GetSize(); ++j )
			{
				CString strFmt;
				strFmt.Format(_T("%d, %.3f, %.3f\n"), nCnt++, aEdge[j].p1.x, aEdge[j].p1.y);
				TRACE(strFmt);//테스트
			}

			int nTopologyType;
			if ( !GetSectTopologyType(SectD, nTopologyType) ) { ASSERT(0); return FALSE; }

			//WriteNodeOfMesh(aaEdge); // Test용

			T_POINT3D BndrPntLeftBot, BndrPntRightTop;
			BndrPntLeftBot.x = BndrPntLeftBot.y = BndrPntLeftBot.z = DBL_MAX;
			BndrPntRightTop.x = BndrPntRightTop.y = BndrPntRightTop.z = -DBL_MAX;

			for ( int i = 0; i < aEdge.GetSize(); i++ )
			{
				ASSERT(!aEdge[i].bIncludeCurv); // 만약 곡률정보가 있다면 아래에 있는 Polygon자르는 알고리즘이 바뀌어야 함
				ASSERT(aEdge[i].p1.z == 0.0);   // 이 알고리즘은 2차원에서의 계산임
				ASSERT(aEdge[i].p2.z == 0.0);

				if ( aEdge[i].p1.x < BndrPntLeftBot.x )
				{
					BndrPntLeftBot.x = aEdge[i].p1.x;
				}
				if ( aEdge[i].p1.y < BndrPntLeftBot.y )
				{
					BndrPntLeftBot.y = aEdge[i].p1.y;
				}
				if ( aEdge[i].p1.x > BndrPntRightTop.x )
				{
					BndrPntRightTop.x = aEdge[i].p1.x;
				}
				if ( aEdge[i].p1.y > BndrPntRightTop.y )
				{
					BndrPntRightTop.y = aEdge[i].p1.y;
				}

				if ( aEdge[i].p2.x < BndrPntLeftBot.x )
				{
					BndrPntLeftBot.x = aEdge[i].p2.x;
				}
				if ( aEdge[i].p2.y < BndrPntLeftBot.y )
				{
					BndrPntLeftBot.y = aEdge[i].p2.y;
				}
				if ( aEdge[i].p2.x > BndrPntRightTop.x )
				{
					BndrPntRightTop.x = aEdge[i].p2.x;
				}
				if ( aEdge[i].p2.y > BndrPntRightTop.y )
				{
					BndrPntRightTop.y = aEdge[i].p2.y;
				}
			}

			//double dBndrWidth, dBndrHeight;
			//dBndrWidth = BndrPntRightTop.x - BndrPntLeftBot.x;
			//dBndrHeight = BndrPntRightTop.y - BndrPntLeftBot.y;

			T_POINT3D PntCutLineLtoR[3][2], PntCutLineBtoT[3][2];
			PntCutLineLtoR[0][0].x = BndrPntLeftBot.x - dZeroForCutLine;
			PntCutLineLtoR[0][0].y = BndrPntLeftBot.y - dZeroForCutLine;
			PntCutLineLtoR[0][0].z = 0.0;
			PntCutLineLtoR[0][1].x = BndrPntRightTop.x + dZeroForCutLine;;
			PntCutLineLtoR[0][1].y = BndrPntLeftBot.y - dZeroForCutLine;
			PntCutLineLtoR[0][1].z = 0.0;

			PntCutLineLtoR[1][0].x = BndrPntLeftBot.x - dZeroForCutLine;
			PntCutLineLtoR[1][0].y = dCentZ;
			PntCutLineLtoR[1][0].z = 0.0;
			PntCutLineLtoR[1][1].x = BndrPntRightTop.x + dZeroForCutLine;;
			PntCutLineLtoR[1][1].y = dCentZ;
			PntCutLineLtoR[1][1].z = 0.0;

			PntCutLineLtoR[2][0].x = BndrPntLeftBot.x - dZeroForCutLine;
			PntCutLineLtoR[2][0].y = BndrPntRightTop.y + dZeroForCutLine;
			PntCutLineLtoR[2][0].z = 0.0;
			PntCutLineLtoR[2][1].x = BndrPntRightTop.x + dZeroForCutLine;;
			PntCutLineLtoR[2][1].y = BndrPntRightTop.y + dZeroForCutLine;
			PntCutLineLtoR[2][1].z = 0.0;

			PntCutLineBtoT[0][0].x = BndrPntLeftBot.x - dZeroForCutLine;
			PntCutLineBtoT[0][0].y = BndrPntLeftBot.y - dZeroForCutLine;
			PntCutLineBtoT[0][0].z = 0.0;
			PntCutLineBtoT[0][1].x = BndrPntLeftBot.x - dZeroForCutLine;
			PntCutLineBtoT[0][1].y = BndrPntRightTop.y + dZeroForCutLine;
			PntCutLineBtoT[0][1].z = 0.0;

			PntCutLineBtoT[1][0].x = dCentY;
			PntCutLineBtoT[1][0].y = BndrPntLeftBot.y - dZeroForCutLine;
			PntCutLineBtoT[1][0].z = 0.0;
			PntCutLineBtoT[1][1].x = dCentY;
			PntCutLineBtoT[1][1].y = BndrPntRightTop.y + dZeroForCutLine;
			PntCutLineBtoT[1][1].z = 0.0;

			PntCutLineBtoT[2][0].x = BndrPntRightTop.x + dZeroForCutLine;
			PntCutLineBtoT[2][0].y = BndrPntLeftBot.y - dZeroForCutLine;
			PntCutLineBtoT[2][0].z = 0.0;
			PntCutLineBtoT[2][1].x = BndrPntRightTop.x + dZeroForCutLine;
			PntCutLineBtoT[2][1].y = BndrPntRightTop.y + dZeroForCutLine;
			PntCutLineBtoT[2][1].z = 0.0;

			struct T_AR_XGL_3dp_D
			{
				CArray<XGL_3dp, XGL_3dp&> aXGL_3dp;
				T_AR_XGL_3dp_D& operator = (const T_AR_XGL_3dp_D& src)
				{
					aXGL_3dp.SetSize(src.aXGL_3dp.GetSize());
					for ( int i = 0; i < aXGL_3dp.GetSize(); i++ )
					{
						aXGL_3dp[i] = src.aXGL_3dp[i];
					}
					return *this;
				}
			};
			CArray<T_AR_XGL_3dp_D, T_AR_XGL_3dp_D&> aaOutCut; // 2Angle같이 Polyline이 여러개 있는 경우를 위한 처리

			// 외부폴리곤을 먼저 그리고 내부폴리곤을 그리는 형태로 aEdge이 세팅되어 있다고 가정하고 넣은 코드임.. 새로운 단면이 들어오면서 SectShape을 다른방식으로 그린다면 버그가 발생할 수 있음.
			T_AR_XGL_3dp_D arXGL_3dp;
			XGL_3dp OutXGL;
			for ( int i = 0; i < aEdge.GetSize(); i++ )
			{
				if ( i == 0 )
				{
					OutXGL.Set(aEdge[i].p1.x, aEdge[i].p1.y, 0.0);
					arXGL_3dp.aXGL_3dp.Add(OutXGL);
				}
				else if ( GetDistance(aEdge[i - 1].p2, aEdge[i].p1) < dTol )
				{
					// p1점을 넣지 않음
				}
				else
				{
					aaOutCut.Add(arXGL_3dp);
					arXGL_3dp.aXGL_3dp.RemoveAll();

					OutXGL.Set(aEdge[i].p1.x, aEdge[i].p1.y, 0.0);
					arXGL_3dp.aXGL_3dp.Add(OutXGL);
				}

				if ( i < aEdge.GetSize() - 1 )
				{
					OutXGL.Set(aEdge[i].p2.x, aEdge[i].p2.y, 0.0);

					if ( arXGL_3dp.aXGL_3dp.GetSize() > 0 )
					{
						if ( GetDistance(arXGL_3dp.aXGL_3dp[0], OutXGL) < dTol )
							continue;
					}

					arXGL_3dp.aXGL_3dp.Add(OutXGL);
				}
				else
				{
					// 아무짓도 안함..
				}
			}

			aaOutCut.Add(arXGL_3dp);
			arXGL_3dp.aXGL_3dp.RemoveAll();

			double dAreaSum[2], dAreaYSum[2], dAreaZSum[2];
			dAreaSum[0] = dAreaSum[1] = dAreaYSum[0] = dAreaYSum[1] = dAreaZSum[0] = dAreaZSum[1] = 0.0;

			if ( nTopologyType == 1 || nTopologyType == 2 )
			{
				for ( int nPolyIdx = 0; nPolyIdx < aaOutCut.GetSize(); nPolyIdx++ )
				{
					CArray<XGL_3dp, XGL_3dp&>& arOutCut = aaOutCut[nPolyIdx].aXGL_3dp;
					CArray<XGL_3dp, XGL_3dp&> arInnerCut;
					// Y축 PlasticSectionModulus 계산
					if ( !Get_EachPlasticSectionModulus(arOutCut, arInnerCut, PntCutLineBtoT, dCentY, dCentZ, SectD, nTypeBeforeAfter, dMatlValue, dAreaSum[0], dAreaYSum[0], dAreaZSum[0]) ) return FALSE;

					// Z축 PlasticSectionModulus 계산
					if ( !Get_EachPlasticSectionModulus(arOutCut, arInnerCut, PntCutLineLtoR, dCentY, dCentZ, SectD, nTypeBeforeAfter, dMatlValue, dAreaSum[1], dAreaYSum[1], dAreaZSum[1]) ) return FALSE;
				}
			}
			else if ( nTopologyType == 3 )
			{
				ASSERT(aaOutCut.GetSize() == 2);
				CArray<XGL_3dp, XGL_3dp&>& arOutCut = aaOutCut[0].aXGL_3dp;
				CArray<XGL_3dp, XGL_3dp&>& arInnerCut = aaOutCut[1].aXGL_3dp;
				// Y축 PlasticSectionModulus 계산
				if ( !Get_EachPlasticSectionModulus(arOutCut, arInnerCut, PntCutLineBtoT, dCentY, dCentZ, SectD, nTypeBeforeAfter, dMatlValue, dAreaSum[0], dAreaYSum[0], dAreaZSum[0]) ) return FALSE;

				// Z축 PlasticSectionModulus 계산
				if ( !Get_EachPlasticSectionModulus(arOutCut, arInnerCut, PntCutLineLtoR, dCentY, dCentZ, SectD, nTypeBeforeAfter, dMatlValue, dAreaSum[1], dAreaYSum[1], dAreaZSum[1]) ) return FALSE;
			}
			else if ( nTopologyType == 4 )
			{
				ASSERT(0);// 이 함수 시작부에 있는.. D_SECT_SHAPE_REG_BSTF와 D_SECT_SHAPE_REG_PSTF를 보시길..
			}

			ASSERT(fabs(dAreaSum[0] - dAreaSum[1]) <= dZeroForCutLine); // Y축과 Z축 기준으로 계산한 Area는 동일해야함
			dZpz +=  dAreaYSum[0]; // 세로선으로 자른 단면들의 y방향 단면 1차 모멘트를 합하면 Zpz(Plastic Section Modulus z축 성분)이 됨
			dZpy +=  dAreaZSum[1]; // 가로선으로 자른 단면들의 z방향 단면 1차 모멘트를 합하면 Zpy(Plastic Section Modulus y축 성분)이 됨
		}
		
    }
	*/
	return TRUE;
}

BOOL CDb_DllBaseManager::Get_EachPlasticSectionModulus(CArray<XGL_3dp, XGL_3dp&>& arOutCut, CArray<XGL_3dp, XGL_3dp&>& arInnerCut, T_POINT3D PntCutLine[][2], double yc, double zc, T_SECT_D& SectD, int nTypeBeforeAfter, double * dMatlValue, double& dAreaSum, double& dAreaY, double& dAreaZ)
{
    ASSERT(0); // CDb_DllBaseManager::Get_PlasticSectionModulusMesh 의 if(bCalcByMesh) 쪽 주석 참고 하시오

    // arOutCut에서 정의한 Polygon을 PntCutLine으로 자른다음 yc, zc을 기준으로 단면 1차 모멘트를 계산하여 dAreaY와 dAreaZ에 누적시키는 알고리즘
    struct T_AR_XGL_3dp_D
    {
        CArray<XGL_3dp, XGL_3dp&> aXGL_3dp;
        T_AR_XGL_3dp_D& operator = (const T_AR_XGL_3dp_D& src)
        {
            aXGL_3dp.SetSize(src.aXGL_3dp.GetSize());
            for ( int i = 0; i < aXGL_3dp.GetSize(); i++ )
            {
                aXGL_3dp[i] = src.aXGL_3dp[i];
            }
            return *this;
        }
    };
    CArray<T_AR_XGL_3dp_D, T_AR_XGL_3dp_D&> aaResult;

    CSimpleMesh simpleMesh;
    simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;

    I_PolyMaker* pPolyMaker;

    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD;    aMeshElemD.RemoveAll();
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD;    aMeshNodeD.RemoveAll();
	std::vector<int> aDomainElemCount;                   aDomainElemCount.clear();
    CArray<int, int> aDomainMaterialType;                aDomainMaterialType.RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입

    XGL_Line3d LineGL;
    CArray<XGL_Line3d, XGL_Line3d&> arSlicingLines;
    arSlicingLines.SetSize(3);
    for ( int i = 0; i < 3; i++ )
    {
        // hslee : I_PolyMaker를 이용해서 특성 선으로 자른다음 양쪽 단면을 다 가지고 오려면 3개의 선을 정의하여 SetSlicingLines를 이용해야 한다... SetCuttingLine을 써보니 잘린 단면중 하나는 버리고 잘려진 단면도 뭔가 이상하다..
        LineGL.m_P1.Set(PntCutLine[i][0].x, PntCutLine[i][0].y, 0.0);
        LineGL.m_P2.Set(PntCutLine[i][1].x, PntCutLine[i][1].y, 0.0);
        arSlicingLines[i] = LineGL;
    }

    //// Test용
    //CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdgeTest;
    //aaEdgeTest.SetSize(1);
    //T_LINE_FOR_MESH_G EachEdge;
    //EachEdge.p1.x = arOutCut[0].x();
    //EachEdge.p1.y = arOutCut[0].y();
    //EachEdge.p1.z = arOutCut[0].z();
    //for (int j = 1; j < arOutCut.GetSize(); j++)
    //{
    //  EachEdge.p2.x = arOutCut[j].x();
    //  EachEdge.p2.y = arOutCut[j].y();
    //  EachEdge.p2.z = arOutCut[j].z();
    //  aaEdgeTest[0].aAr.Add(EachEdge);
    //
    //  EachEdge.p1 = EachEdge.p2;
    //}
    //EachEdge.p2 = aaEdgeTest[0].aAr[0].p1;
    //aaEdgeTest[0].aAr.Add(EachEdge);
    //WriteNodeOfMesh(aaEdgeTest);
    //// 여기까지

    pPolyMaker = I_PolyMakerFactory::GetInstance()->New_PMaker();
    pPolyMaker->SetOutterPoly(arOutCut);
    if ( arInnerCut.GetSize() > 0 )
    {
        pPolyMaker->AddInnerPoly(arInnerCut);
    }
    pPolyMaker->SetSlicingLines(arSlicingLines);
    if ( !pPolyMaker->MakeSlicedPolygons() )	ASSERT(0);
    T_AR_XGL_3dp_D aResPoly;
    for ( int i=0; i<pPolyMaker->GetResultPolyCount(); i++ )
    {
        pPolyMaker->GetResultPoly(i, aResPoly.aXGL_3dp);
        ASSERT(aResPoly.aXGL_3dp.GetSize() > 2);
        if ( aResPoly.aXGL_3dp.GetSize() <= 2 ) continue;
        aaResult.Add(aResPoly);
    }
    I_PolyMakerFactory::GetInstance()->Del_PMaker(pPolyMaker);

    //// Test용
    //CArray<T_ArEachPlanInfo_D, T_ArEachPlanInfo_D&> aaEdgeTest;
    //aaEdgeTest.SetSize(aaResult.GetSize());
    //for (int i = 0; i < aaEdgeTest.GetSize(); i++)
    //{
    //  T_LINE_FOR_MESH_G EachEdge;
    //  EachEdge.p1.x = aaResult[i].aXGL_3dp[0].x();
    //  EachEdge.p1.y = aaResult[i].aXGL_3dp[0].y();
    //  EachEdge.p1.z = aaResult[i].aXGL_3dp[0].z();
    //  for (int j = 1; j < aaResult[i].aXGL_3dp.GetSize(); j++)
    //  {
    //    EachEdge.p2.x = aaResult[i].aXGL_3dp[j].x();
    //    EachEdge.p2.y = aaResult[i].aXGL_3dp[j].y();
    //    EachEdge.p2.z = aaResult[i].aXGL_3dp[j].z();
    //    aaEdgeTest[i].aAr.Add(EachEdge);
    //
    //    EachEdge.p1 = EachEdge.p2;
    //  }
    //  EachEdge.p2 = aaEdgeTest[i].aAr[0].p1;
    //  aaEdgeTest[i].aAr.Add(EachEdge);
    //}
    //WriteNodeOfMesh(aaEdgeTest);
    // 여기까지

    // 길이가 너무 짧은 점은 삭제 한다.
    for ( int i = 0; i < aaResult.GetSize(); i++ )
    {
        CArray<XGL_3dp, XGL_3dp&>& arResPoly = aaResult[i].aXGL_3dp;
        if ( arResPoly.GetSize() <= 1 ) { ASSERT(0); continue; } // 여기 ASSERT 걸리는 Composite 단면인데.. 이것에 대한 코드는 아직 개발 안했으니 잘넣어줘야함..

        for ( int j = 1; j < arResPoly.GetSize(); j++ )
        {
            double dDist = CMathFunc::mathLength(arResPoly[j - 1].x(), arResPoly[j - 1].y(), arResPoly[j - 1].z(), arResPoly[j].x(), arResPoly[j].y(), arResPoly[j].z());
            if ( dDist <= 1.0e-6 )
            {
                arResPoly.RemoveAt(j);
                j--;
            }
        }

        // 마지막 점이 첫번째 점과 겹쳐 있으면 삭제한다.
        if ( arResPoly.GetSize() <= 1 ) { ASSERT(0); continue; }
        int nSize = arResPoly.GetSize();
        double dDist = CMathFunc::mathLength(arResPoly[0].x(), arResPoly[0].y(), arResPoly[0].z(), arResPoly[nSize - 1].x(), arResPoly[nSize - 1].y(), arResPoly[nSize - 1].z());
        if ( dDist <= 1.0e-6 )
        {
            arResPoly.RemoveAt(nSize - 1);
        }
    }

    // I_PolyMakerFactory에서 잘라준 단면의 단면1차모멘트를 계산하고, 이를 합한다.
    CArray<T_LINE_FOR_MESH_G, T_LINE_FOR_MESH_G&> aEdgeDivPoly;
    for ( int i=0; i<aaResult.GetSize(); i++ )
    {
        aEdgeDivPoly.RemoveAll();

        CArray<XGL_3dp, XGL_3dp&>& arResPoly = aaResult[i].aXGL_3dp;

        ASSERT(arResPoly.GetSize() > 2);
        if ( arResPoly.GetSize() <= 2 ) continue;

        int nSizeResPoly = arResPoly.GetSize();
        T_LINE_FOR_MESH_G EachLine;
        for ( int j = 1; j < nSizeResPoly; j++ )
        {
            EachLine.Set(T_POINT3D(arResPoly[j - 1].x(), arResPoly[j - 1].y(), 0.0), T_POINT3D(arResPoly[j].x(), arResPoly[j].y(), 0.0));
            aEdgeDivPoly.Add(EachLine);
        }
        EachLine.Set(T_POINT3D(arResPoly[nSizeResPoly - 1].x(), arResPoly[nSizeResPoly - 1].y(), 0.0), T_POINT3D(arResPoly[0].x(), arResPoly[0].y(), 0.0));
        aEdgeDivPoly.Add(EachLine);

        aMeshElemD.RemoveAll();
        aMeshNodeD.RemoveAll();
        aDomainElemCount.clear();
        aDomainMaterialType.RemoveAll();

        if ( !simpleMesh.GetSectMeshByPolygon(SectD, nTypeBeforeAfter, aEdgeDivPoly, aEdgeDivPoly, aMeshElemD, aMeshNodeD, aDomainElemCount, aDomainMaterialType) ) return FALSE;

        //    WriteNodeOfMesh(aMeshElemD, aMeshNodeD); // Test 용

        double area, areay, areaz;
        if ( !Get_SectionFirstMomentOfAreaMesh(SectD, area, areay, areaz, yc, zc, aDomainElemCount, dMatlValue, aDomainMaterialType, aMeshNodeD, aMeshElemD) ) return FALSE;

        dAreaSum += area;
        dAreaY += areay;
        dAreaZ += areaz;
    }

    return TRUE;
}

BOOL CDb_DllBaseManager::Get_SectionFirstMomentOfAreaMesh(T_SECT_D &SectD, double& area, double& areay, double& areaz, double yc, double zc, std::vector<int>& aDomainElemCount, double dMatlValue[6], CArray<int, int>& aDomainMaterialType, CArray<T_MESH_NODE_D, T_MESH_NODE_D&> &aMeshNodeD, CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> &aMeshElemD)
{
    ASSERT(0); // CDb_DllBaseManager::Get_PlasticSectionModulusMesh 의 if(bCalcByMesh) 쪽 주석 참고 하시오

    area = areay = areaz = 0.0;
    // yc == 단면 1차 모멘트 계산을 위한 y좌표, zc == 단면 1차 모멘트 계산을 위한 z좌표

    HINSTANCE hDllCalc = GetOrLoad_PBeamLMeshDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    // 단면 속성 계산
    int nCountDomain = aDomainElemCount.size();
    int nDomain = nCountDomain;

    double dE = dMatlValue[0];
    double dW = dMatlValue[2];
    double dNu = dMatlValue[4];
    CArray<double, double> aElaMod; aElaMod.RemoveAll();
    CArray<double, double> aSectW; aSectW.RemoveAll();
    CArray<double, double> aNu; aNu.RemoveAll();
    BOOL bByConc = FALSE;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) bByConc = TRUE;    // PSC 파형단면(D_SECT_SHAPE_PSC_CMPWEB)은 Conc 기준으로 계산.
    if ( !GetMatl4Domain(dMatlValue, nDomain, aDomainMaterialType, dE, dW, dNu, aElaMod, aSectW, aNu, bByConc) ) { ASSERT(0); return FALSE; }

    BSPFirstMomentOfArea* pBSPFirstMomentOfArea;
    pBSPFirstMomentOfArea = ( BSPFirstMomentOfArea* ) GetProcAddress(hDllCalc, "BSPFirstMomentOfArea");
    if ( pBSPFirstMomentOfArea==NULL ) { ASSERT(0); return FALSE; }

    // BSPFirstMomentOfArea() 함수의 입력 파라미터
    int _nnode = aMeshNodeD.GetSize();    int* p_nnode = &_nnode;           // 총 절점수
    int _nelem = aMeshElemD.GetSize();    int* p_nelem = &_nelem;           // 총 요소수

    // 절점의 좌표
    // * 주의사항 (솔버에서 y-축 :수직, z-축 :수평으로 계산하고 있어서, 좌표는 y,z를 바꿔서 넘겨야 함.)
    CArray<double, double> aYNode; aYNode.RemoveAll();  aYNode.SetSize(_nnode);
    CArray<double, double> aZNode; aZNode.RemoveAll();  aZNode.SetSize(_nnode);
    double dY=0., dZ=0.;
    for ( int i=0; i<_nnode; i++ )
    {
        dY = aMeshNodeD[i].x * ( -1 ); // 솔버 좌표계와 맞추기 위해
        //dY = aMeshNodeD[i].x; // 응력계산위치의 좌표계와 맞추기 위하여..
        dZ = aMeshNodeD[i].y;

        aYNode.SetAt(i, dY);
        aZNode.SetAt(i, dZ);
    }
    double* p_ynode = aZNode.GetData();   // 절점의 y좌표
    double* p_znode = aYNode.GetData();   // 절점의 z좌표

    // 요소의 절점 연결 번호 (array size = 요소수 * 3, 저차 삼각형요소, 절점의 최초 시작은 1부터)
    CArray<int, int> _iconn;    _iconn.RemoveAll();    _iconn.SetSize(_nelem*3);    // 삼각형 요소.
    int iConnNum = 0;
    for ( int i=0; i<_nelem; ++i )
    {
        if ( aMeshElemD[i].aNodeK.GetSize() != 3 ) { ASSERT(0); return FALSE; }

        _iconn[iConnNum] = aMeshElemD[i].aNodeK[0] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[1] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[2] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
    }
    ASSERT(iConnNum == _iconn.GetSize());
    int* p_iconn = _iconn.GetData();

    //double _E = dE;       double* p_E = &_E;        // 환산단면의 기준이 되는 탄성계수
    //double _NU = dNu;     double* p_NU = &_NU;      // 환산단면의 기준이 되는 푸아송비
    double _W = dW;       double* p_W = &_W;        // 환산단면의 기준이 되는 단위중량

    //double* p_sect_e = aElaMod.GetData();           // 합성단면의 단면별 탄성계수 (array size = p_nprop)
    //double* p_sect_nu = aNu.GetData();              // 합성단면의 단면별 푸아송비 (array size = p_nprop)
    double* p_sect_w = aSectW.GetData();            // 합성단면의 단면별 단위중량 (array size = p_nprop)

    int _nrebar = 0;    int* p_nrebar = &_nrebar;   // 철근 개수
    CArray<double, double> _asr; _asr.RemoveAll();  _asr.Add(0.0);  // _asr.SetSize(_nrebar);
    double* p_asr = _asr.GetData();                 // 철근의 반지름
    CArray<double, double> _dsy; _dsy.RemoveAll();  _dsy.Add(0.0);  // _dsy.SetSize(_nrebar);
    double* p_dsy = _dsy.GetData();                 // 철근이 위치한 y좌표
    CArray<double, double> _dsz; _dsz.RemoveAll();  _dsz.Add(0.0);  // _dsz.SetSize(_nrebar);
    double* p_dsz = _dsz.GetData();                 // 철근이 위치한 z좌표
    CArray<double, double> _ES; _ES.RemoveAll();  _ES.Add(0.0);  // _ES.SetSize(_nrebar);
    double* p_ES = _ES.GetData();                   // 철근의 탄성계수 (array size = p_nrebar)
    CArray<double, double> _WS; _WS.RemoveAll();  _WS.Add(0.0);  // _WS.SetSize(_nrebar);
    double* p_WS = _WS.GetData();                   // 철근의 단위중량 (array size = p_nrebar)

    //--------------------------------------------------------------
    // Mesh 검증용 mct 생성.
    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    if ( bDumpSectMeshFile )  WriteMctOfMesh2(_nnode, p_ynode, p_znode, strPostFix);
    //--------------------------------------------------------------

    BOOL bCatchDllException = FALSE;

#ifdef _DEBUG
    if ( CTestEnvMgr::IsTestEnvST(_T("CatchDllException"), _T("yes")) ) bCatchDllException = TRUE;
#endif // _DEBUG

    if ( bCatchDllException )
    {
        try // 처리되지 않은 예외 안뜨게 하기 위한 코드... 속성 - C/C++-코드 생성 - C++ 예외 처리 가능 - 예외 처리 옵션 - /EHa 로 설정해야 먹힘
        {
            // 이 코드를 넣은 이유는 Debug모드일 때 Dll에서 처리안된 예외가 있어서 무한 ASSERT가 걸리는 것을 방지하기 위함인데..
            // 이 코드를 넣기 전 Release빌드에서 잘 돌아가던 것이 이놈을 넣은 뒤엔 돌다가 중간에 튕겨버릴 수 있다....
            // 개발 중에는 돌다가 튕겨서 값이 잘못되더라도 진행은 하고 싶은경우가 있어서.. 이 상황을 위해 Registry + Debug모드 일 때만 Exception을 받도록 처리 한다.

            int nResult = ( *pBSPFirstMomentOfArea )( &area, &areay, &areaz, &yc, &zc, p_nnode, p_nelem, p_ynode, p_znode, p_iconn, p_W, p_sect_w, p_nrebar, p_asr, p_dsy, p_dsz, p_WS );
        }
        catch ( CException* e )
        {
            e = NULL; // warning제거용
            // 아무거도 안함
        }
        catch ( ... ) // else
        {
            // 아무거도 안함
        }
    }
    else
    {
        int nResult = ( *pBSPFirstMomentOfArea )( &area, &areay, &areaz, &yc, &zc, p_nnode, p_nelem, p_ynode, p_znode, p_iconn, p_W, p_sect_w, p_nrebar, p_asr, p_dsy, p_dsz, p_WS );
    }

    // BSPFirstMomentOfArea에서 계산해 주는 값은 yc, zc에서의 방향을 고려한 값을 계산 하는데, 방향을 고려하지 않은 거리로 계산하는게 일반적임.. 그래서 절대값을 취함..
    // 절대값을 취해도 되는 이유 "단면 CSimpleMesh simpleMesh;1차모멘트 = 면적*기준점부터의거리" 이기 때문임..
    areay = fabs(areay);
    areaz = fabs(areaz);

    return TRUE;
}

BOOL CDb_DllBaseManager::IsClosedAfterComposite(T_SECT_D& rData, int nPos)
{
    CSimpleMesh simpleMesh;
    BOOL bCheck = simpleMesh.IsClosedAfterComposite(rData, nPos);

    return bCheck;
}

// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CDb_DllBaseManager::Get_SectionPropertyMesh(T_SECT_D& rData, int nPos, int nTypeBeforeAfter, double dMatlValue[6],
    double* pbeam, double* cdef, int nCdef, BOOL bConvertSectStiffData2BeforeI/*=TRUE*/,
    T_SECT_DESIGN* pSectDesign/*=NULL*/)
{
    // SectTest
    if ( CDb_DllBaseManager::m_bAllSectMesh )
    {
        if ( Get_SectionPropertyMesh_AllSect(rData, nPos, nTypeBeforeAfter, dMatlValue, pbeam, cdef, nCdef, bConvertSectStiffData2BeforeI, pSectDesign) )
            return TRUE;
        else
            return FALSE;
    }

    T_SECT_D SectD = rData;

    // 단면 형상/메쉬 얻어오기.
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD;    aMeshElemD.RemoveAll();
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD;    aMeshNodeD.RemoveAll();
	std::vector<int> aDomainElemCount;                   aDomainElemCount.clear();
    CArray<int, int> aDomainMaterialType;                aDomainMaterialType.RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입
    CSimpleMesh simpleMesh;
    simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;

    // 파형강판 예외 처리. by Hsshim.
    // 기존에는 파형강판에 대해서 탄성계수비(Es/Ec)를 고려해서 환산한 면적으로 Mesh가 되는 문제가 있어서,
    // 형상을 그대로 고려할 수 있도록 탄성계수비를 1.0으로 치환해서 Mesh를 얻어오도록 수정
    double dMatl_Elast_back = SectD.SectBefore.Matl_Elast;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = 1.0; }
    if ( !simpleMesh.GetSectMesh(SectD, nPos, nTypeBeforeAfter, aMeshElemD, aMeshNodeD, aDomainElemCount, aDomainMaterialType, TRUE, pSectDesign) ) { return FALSE; }
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = dMatl_Elast_back; }

    // 단면 속성 계산
    int nCountDomain = aDomainElemCount.size();

    HINSTANCE hDllCalc = GetOrLoad_PBeamLMeshDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    T_SECT_STIFFNESS* pSectStiffness = &SectD.SectBefore.SectI.Stiffness;
    if ( bConvertSectStiffData2BeforeI )
    {
        if ( nPos==0 ) pSectStiffness = &SectD.SectBefore.SectI.Stiffness;
        else        pSectStiffness = &SectD.SectBefore.SectJ.Stiffness;

        if ( IsUseStiffBySectBefore(rData) ) pSectStiffness = &rData.SectBefore.Stiffness;
    }

    int nDomain = nCountDomain;
    BOOL bAuto4P = FALSE;
    int nPoint = 4;
    BOOL bConsiderWarpingEffect=FALSE;
    if ( IsSect_PSC(SectD) )
    {
        nPoint = 10;

        if ( CSectDB::IsEnable7thDOF() && SectD.SectBefore.bConsiderWarpingEffect )
        {
            bAuto4P= ( rData.SectBefore.nWarpingCheck[nPos]==0 ) ? TRUE : FALSE;

            bConsiderWarpingEffect = TRUE;
            if ( bAuto4P ) nPoint = 10;  // 10 + 자동계산   6포인트
            else        nPoint = 16;  // 10 + 사용자입력 6포인트
        }
    }

    double* pointy = NULL;
    double* pointz = NULL;
    pointy = new double[nPoint];
    pointz = new double[nPoint];
    memset(pointy, 0, sizeof(double)*nPoint);
    memset(pointz, 0, sizeof(double)*nPoint);

    pointy[0] = pSectStiffness->dx1;
    pointy[1] = pSectStiffness->dx2;
    pointy[2] = pSectStiffness->dx3;
    pointy[3] = pSectStiffness->dx4;
    // 응력계산위치의 좌표계와 형상 좌표계를 맞추기 위하여.. X(-1)
    //   pointy[0] = (-1)*pSectStiffness->dx1;
    //   pointy[1] = (-1)*pSectStiffness->dx2;
    //   pointy[2] = (-1)*pSectStiffness->dx3;
    //   pointy[3] = (-1)*pSectStiffness->dx4;

    pointz[0] = pSectStiffness->dy1;
    pointz[1] = pSectStiffness->dy2;
    pointz[2] = pSectStiffness->dy3;
    pointz[3] = pSectStiffness->dy4;

    if ( nPoint==10 || nPoint==16 )
    {
        T_SPSC_DSHR SpscDshr; SpscDshr.Initialize();
        CSectUtil::GetPscShearPosi(&SectD, nPos==0, SpscDshr, FALSE); // 여기서는 GetPscShearPosi_7DOF 적용하면 안됨..

        pointy[4] = SpscDshr.dyz[0][0];
        pointy[5] = SpscDshr.dyz[0][1];
        pointy[6] = SpscDshr.dyz[0][2];
        pointy[7] = SpscDshr.dyz[0][3];
        pointy[8] = SpscDshr.dyz[0][4];
        pointy[9] = SpscDshr.dyz[0][5];

        // 		pointy[4] = (-1)*SpscDshr.dyz[0][0];
        // 		pointy[5] = (-1)*SpscDshr.dyz[0][1];
        // 		pointy[6] = (-1)*SpscDshr.dyz[0][2];
        // 		pointy[7] = (-1)*SpscDshr.dyz[0][3];
        // 		pointy[8] = (-1)*SpscDshr.dyz[0][4];
        // 		pointy[9] = (-1)*SpscDshr.dyz[0][5];

        pointz[4] = SpscDshr.dyz[1][0];
        pointz[5] = SpscDshr.dyz[1][1];
        pointz[6] = SpscDshr.dyz[1][2];
        pointz[7] = SpscDshr.dyz[1][3];
        pointz[8] = SpscDshr.dyz[1][4];
        pointz[9] = SpscDshr.dyz[1][5];

        if ( nPoint==10 ) //
        {
            // Dll에서 받아온다.
        }
        else if ( nPoint==16 )
        {
            if ( SectD.SectBefore.nWarpingCheck[nPos] ==0 ) // Auto
            {
                pointy[10] = SpscDshr.dyz_7Dof[0][0];
                pointy[11] = SpscDshr.dyz_7Dof[0][1];
                pointy[12] = SpscDshr.dyz_7Dof[0][2];
                pointy[13] = SpscDshr.dyz_7Dof[0][3];
                pointy[14] = SpscDshr.dyz_7Dof[0][4];
                pointy[15] = SpscDshr.dyz_7Dof[0][5];

                pointz[10] = SpscDshr.dyz_7Dof[1][0];
                pointz[11] = SpscDshr.dyz_7Dof[1][1];
                pointz[12] = SpscDshr.dyz_7Dof[1][2];
                pointz[13] = SpscDshr.dyz_7Dof[1][3];
                pointz[14] = SpscDshr.dyz_7Dof[1][4];
                pointz[15] = SpscDshr.dyz_7Dof[1][5];
            }
            else // User
            {
                pointy[10] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][0] : SectD.SectBefore.dWarpingCheckPosJ[0][0];
                pointy[11] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][1] : SectD.SectBefore.dWarpingCheckPosJ[0][1];
                pointy[12] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][2] : SectD.SectBefore.dWarpingCheckPosJ[0][2];
                pointy[13] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][3] : SectD.SectBefore.dWarpingCheckPosJ[0][3];
                pointy[14] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][4] : SectD.SectBefore.dWarpingCheckPosJ[0][4];
                pointy[15] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][5] : SectD.SectBefore.dWarpingCheckPosJ[0][5];

                pointz[10] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][0] : SectD.SectBefore.dWarpingCheckPosJ[1][0];
                pointz[11] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][1] : SectD.SectBefore.dWarpingCheckPosJ[1][1];
                pointz[12] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][2] : SectD.SectBefore.dWarpingCheckPosJ[1][2];
                pointz[13] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][3] : SectD.SectBefore.dWarpingCheckPosJ[1][3];
                pointz[14] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][4] : SectD.SectBefore.dWarpingCheckPosJ[1][4];
                pointz[15] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][5] : SectD.SectBefore.dWarpingCheckPosJ[1][5];
            }
        }
        else ASSERT(0);
    }

    double dE = dMatlValue[0];
    double dW = dMatlValue[2];
    double dNu = dMatlValue[4];
    CArray<double, double> aElaMod; aElaMod.RemoveAll();
    CArray<double, double> aSectW; aSectW.RemoveAll();
    CArray<double, double> aNu; aNu.RemoveAll();
    BOOL bByConc = FALSE;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) bByConc = TRUE;    // PSC 파형단면(D_SECT_SHAPE_PSC_CMPWEB)은 Conc 기준으로 계산.
    if ( !GetMatl4Domain(dMatlValue, nDomain, aDomainMaterialType, dE, dW, dNu, aElaMod, aSectW, aNu, bByConc) )
    {
        ASSERT(0);
        delete[] pointy; pointy=NULL;
        delete[] pointz; pointz=NULL;
        return FALSE;
    }

    double dCentY = 0.0;
    double dCentZ = 0.0;
    if ( CSimpleMesh::CalcCentroid(dCentY, dCentZ, aDomainElemCount, aMeshElemD, aMeshNodeD, aElaMod) )
    {
        // 도심기준의 응력 위치를 Mesh 좌표 기준으로 환산
        // 솔버 좌표계와 맞춘다.
        for ( int k=0; k<nPoint; ++k )
        {
            pointy[k] += dCentY * ( -1 );
            pointz[k] += dCentZ;
        }
    }
    else { ASSERT(0); }

    BSPCombMain* pBSPCombMain;
    pBSPCombMain = ( BSPCombMain* ) GetProcAddress(hDllCalc, "BSPCombMain");
    if ( pBSPCombMain==NULL ) { ASSERT(0); return FALSE; }

    // BSPCombMain() 함수의 입력 파라미터
    int _nnode = aMeshNodeD.GetSize();    int* p_nnode = &_nnode;           // 총 절점수
    int _npoint = nPoint;                 int* p_npoint = &_npoint;         // 응력계산위치 개수
    int _nelem = aMeshElemD.GetSize();    int* p_nelem = &_nelem;           // 총 요소수
    int _nprop = nCountDomain;            int* p_nprop = &_nprop;           // 총 material 수
    int _ncombined = nCountDomain;        int* p_ncombined = &_ncombined;   // 연결되지 않은 단면의 수

    // 절점의 좌표
    // * 주의사항 (솔버에서 y-축 :수직, z-축 :수평으로 계산하고 있어서, 좌표는 y,z를 바꿔서 넘겨야 함.)
    CArray<double, double> aYNode; aYNode.RemoveAll();  aYNode.SetSize(_nnode);
    CArray<double, double> aZNode; aZNode.RemoveAll();  aZNode.SetSize(_nnode);
    double dY=0., dZ=0.;
    for ( int i=0; i<_nnode; i++ )
    {
        dY = aMeshNodeD[i].x * ( -1 ); // 솔버 좌표계와 맞추기 위해
        //dY = aMeshNodeD[i].x; // 응력계산위치의 좌표계와 맞추기 위하여..
        dZ = aMeshNodeD[i].y;

        aYNode.SetAt(i, dY);
        aZNode.SetAt(i, dZ);
    }
    double* p_ynode = aZNode.GetData();   // 절점의 y좌표
    double* p_znode = aYNode.GetData();   // 절점의 z좌표

    double* p_pointy = pointz;    // 응력계산 위치의 y좌표
    double* p_pointz = pointy;    // 응력계산 위치의 z좌표

    // 요소의 절점 연결 번호 (array size = 요소수 * 3, 저차 삼각형요소, 절점의 최초 시작은 1부터)
    CArray<int, int> _iconn;    _iconn.RemoveAll();    _iconn.SetSize(_nelem*3);    // 삼각형 요소.
    int iConnNum = 0;
    for ( int i=0; i<_nelem; ++i )
    {
        if ( aMeshElemD[i].aNodeK.GetSize() != 3 ) { ASSERT(0); return FALSE; }

        _iconn[iConnNum] = aMeshElemD[i].aNodeK[0] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[1] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[2] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
    }
    ASSERT(iConnNum == _iconn.GetSize());
    int* p_iconn = _iconn.GetData();

    // 요소 갯수 검증
    int nDomainElemSum = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )  nDomainElemSum += aDomainElemCount[i];
    ASSERT(nDomainElemSum == _nelem);

    // 요소의 재료 index (array size =  요소수)
    CArray<int, int> _elem_prop;    _elem_prop.RemoveAll();    _elem_prop.SetSize(_nelem);
    int iElem = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )
    {
        for ( int k=0; k<aDomainElemCount[i]; ++k )
        {
            _elem_prop[iElem] = i+1;    // p_sect_e, p_sect_nu, p_sect_w의 참조 index (fortran이기 때문에 1부터 시작). 사실상 소속 domain index
            iElem++;
        }
    }
    int* p_elem_prop = _elem_prop.GetData();

    // combined section의 노드수 (array size = p_ncombined)
    CArray<int, int> _comb_node;    _comb_node.RemoveAll();    _comb_node.SetSize(_ncombined);
    int iStart = 0;
    int iEndCount = 0;
    for ( int i=0; i<_ncombined; ++i )
    {
        if ( i>0 ) iStart += aDomainElemCount[i-1];
        iEndCount = iStart+aDomainElemCount[i];

        CMapEx<int, int, int, int> mNodeOfDomain;    mNodeOfDomain.RemoveAll();

        // 개별 Domain에 대한 loop
        for ( int k=iStart; k<iEndCount; ++k )
        {
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[0], 0);
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[1], 0);
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[2], 0);
        }
        _comb_node[i] = mNodeOfDomain.GetCount();
    }
    int* p_comb_node = _comb_node.GetData();

    // combined section의 요소수 (array size = p_ncombined)
    CArray<int, int> _comb_elem;    _comb_elem.RemoveAll();    _comb_elem.SetSize(_ncombined);
    for ( int i=0; i<_ncombined; ++i )  _comb_elem[i] = aDomainElemCount[i];
    int* p_comb_elem = _comb_elem.GetData();

    double _E = dE;       double* p_E = &_E;        // 환산단면의 기준이 되는 탄성계수
    double _NU = dNu;     double* p_NU = &_NU;      // 환산단면의 기준이 되는 푸아송비
    double _W = dW;       double* p_W = &_W;        // 환산단면의 기준이 되는 단위중량

    double* p_sect_e = aElaMod.GetData();           // 합성단면의 단면별 탄성계수 (array size = p_nprop)
    double* p_sect_nu = aNu.GetData();              // 합성단면의 단면별 푸아송비 (array size = p_nprop)
    double* p_sect_w = aSectW.GetData();            // 합성단면의 단면별 단위중량 (array size = p_nprop)

    int _nrebar = 0;    int* p_nrebar = &_nrebar;   // 철근 개수
    CArray<double, double> _asr; _asr.RemoveAll();  _asr.Add(0.0);  // _asr.SetSize(_nrebar);
    double* p_asr = _asr.GetData();                 // 철근의 반지름
    CArray<double, double> _dsy; _dsy.RemoveAll();  _dsy.Add(0.0);  // _dsy.SetSize(_nrebar);
    double* p_dsy = _dsy.GetData();                 // 철근이 위치한 y좌표
    CArray<double, double> _dsz; _dsz.RemoveAll();  _dsz.Add(0.0);  // _dsz.SetSize(_nrebar);
    double* p_dsz = _dsz.GetData();                 // 철근이 위치한 z좌표
    CArray<double, double> _ES; _ES.RemoveAll();  _ES.Add(0.0);  // _ES.SetSize(_nrebar);
    double* p_ES = _ES.GetData();                   // 철근의 탄성계수 (array size = p_nrebar)
    CArray<double, double> _WS; _WS.RemoveAll();  _WS.Add(0.0);  // _WS.SetSize(_nrebar);
    double* p_WS = _WS.GetData();                   // 철근의 단위중량 (array size = p_nrebar)

    int _ioptn = 2;
    if ( bConsiderWarpingEffect )
    {
        if ( bAuto4P ) _ioptn = 3; // 자동일 때 : 해석에서 넘어옴.
        else        _ioptn = 2; // 사용자 입력일때
    }
    int* p_ioptn = &_ioptn;         // 0 = 1~22+33+34  항목만 계산; 1= 1~34까지만 계산; 2=모두 계산 (추가)3: 14point  고려
    int _iref = 0;      int* p_iref = &_iref;           // 0 = 도심기준으로 cdef출력, 1=전단중심 기준으로 cdef 출력
    int _elemtype = 0;  int* p_elemtype = &_elemtype;   // 요소 종류 0 = 고차 요소, 1= 저차요소

    //--------------------------------------------------------------
    // Mesh 검증용 mct 생성.
    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    if ( bDumpSectMeshFile )  WriteMctOfMesh2(_nnode, p_ynode, p_znode, strPostFix);
    //--------------------------------------------------------------

    // PBeamDll에 넘기는 Param을 파일로 씀
    BOOL bWritePBeamDllParamToFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFWritePBeamDllParamToFile"), _T("yes")) ) bWritePBeamDllParamToFile = TRUE;
    if ( bWritePBeamDllParamToFile )
    {
        WritePBeamDllParamToFileNew(cdef, 0, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
            p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
            p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
            p_asr, p_dsy, p_dsz, p_ES, p_WS, p_ioptn, p_iref, p_elemtype, strPostFix);
    }

    BOOL bCatchDllException = FALSE;
#ifdef _DEBUG
    if ( CTestEnvMgr::IsTestEnvST(_T("CatchDllException"), _T("yes")) ) bCatchDllException = TRUE;
#endif // _DEBUG
    if ( bCatchDllException )
    {
        try // 처리되지 않은 예외 안뜨게 하기 위한 코드... 속성 - C/C++-코드 생성 - C++ 예외 처리 가능 - 예외 처리 옵션 - /EHa 로 설정해야 먹힘
        {
            // 이 코드를 넣은 이유는 Debug모드일 때 Dll에서 처리안된 예외가 있어서 무한 ASSERT가 걸리는 것을 방지하기 위함인데..
            // 이 코드를 넣기 전 Release빌드에서 잘 돌아가던 것이 이놈을 넣은 뒤엔 돌다가 중간에 튕겨버릴 수 있다....
            // 개발 중에는 돌다가 튕겨서 값이 잘못되더라도 진행은 하고 싶은경우가 있어서.. 이 상황을 위해 Registry + Debug모드 일 때만 Exception을 받도록 처리 한다.

            int nResult = ( *pBSPCombMain )( pbeam, cdef, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
                p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
                p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
                p_asr, p_dsy, p_dsz, p_ES, p_WS, p_ioptn, p_iref, p_elemtype );
        }
        catch ( CException* e )
        {
            e = NULL; // warning제거용
            // 아무거도 안함
        }
        catch ( ... ) // else
        {
            // 아무거도 안함
        }
    }
    else
    {
        int nResult = ( *pBSPCombMain )( pbeam, cdef, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
            p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
            p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
            p_asr, p_dsy, p_dsz, p_ES, p_WS, p_ioptn, p_iref, p_elemtype );
    }

    delete[] pointy; pointy=NULL;
    delete[] pointz; pointz=NULL;

    return TRUE;
}

// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CDb_DllBaseManager::Get_SectionPropertyMesh_AllSect(T_SECT_D& rData, int nPos, int nTypeBeforeAfter, double dMatlValue[6],
    double* pbeam, double* cdef, int nCdef, BOOL bConvertSectStiffData2BeforeI/*=TRUE*/,
    T_SECT_DESIGN* pSectDesign/*=NULL*/)
{
    T_SECT_D SectD = rData;

    // 단면 형상/메쉬 얻어오기.
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD;    aMeshElemD.RemoveAll();
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD;    aMeshNodeD.RemoveAll();
	std::vector<int> aDomainElemCount;                   aDomainElemCount.clear();
    CArray<int, int> aDomainMaterialType;                aDomainMaterialType.RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입
    CSimpleMesh simpleMesh;
    simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;

    // 파형강판 예외 처리. by Hsshim.
    // 기존에는 파형강판에 대해서 탄성계수비(Es/Ec)를 고려해서 환산한 면적으로 Mesh가 되는 문제가 있어서,
    // 형상을 그대로 고려할 수 있도록 탄성계수비를 1.0으로 치환해서 Mesh를 얻어오도록 수정
    double dMatl_Elast_back = SectD.SectBefore.Matl_Elast;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = 1.0; }
    if ( !simpleMesh.GetSectMesh(SectD, nPos, nTypeBeforeAfter, aMeshElemD, aMeshNodeD, aDomainElemCount, aDomainMaterialType, TRUE, pSectDesign) ) { return FALSE; }
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = dMatl_Elast_back; }

    // 단면 속성 계산
    int nCountDomain = aDomainElemCount.size();

    HINSTANCE hDllCalc = GetOrLoad_PBeamLMeshDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    T_SECT_STIFFNESS* pSectStiffness = &SectD.SectBefore.SectI.Stiffness;
    if ( bConvertSectStiffData2BeforeI )
    {
        if ( nPos==0 ) pSectStiffness = &SectD.SectBefore.SectI.Stiffness;
        else        pSectStiffness = &SectD.SectBefore.SectJ.Stiffness;

        if ( IsUseStiffBySectBefore(rData) ) pSectStiffness = &rData.SectBefore.Stiffness;
    }

    int nDomain = nCountDomain;
    BOOL bAuto4P = FALSE;
    int nPoint = 4;
    BOOL bConsiderWarpingEffect=FALSE;
    if ( IsSect_PSC(SectD) )
    {
        nPoint = 10;

        if ( CSectDB::IsEnable7thDOF() && SectD.SectBefore.bConsiderWarpingEffect )
        {
            bAuto4P= ( rData.SectBefore.nWarpingCheck[nPos]==0 ) ? TRUE : FALSE;

            bConsiderWarpingEffect = TRUE;
            if ( bAuto4P ) nPoint = 10;  // 10 + 자동계산   6포인트
            else        nPoint = 16;  // 10 + 사용자입력 6포인트
        }
    }
    else if ( nTypeBeforeAfter==2 && CSectDB::IsSectSteelComposite(rData) )
    {
        nPoint = 8;
    }

    double* pointy = NULL;
    double* pointz = NULL;
    pointy = new double[nPoint];
    pointz = new double[nPoint];
    memset(pointy, 0, sizeof(double)*nPoint);
    memset(pointz, 0, sizeof(double)*nPoint);

    pointy[0] = pSectStiffness->dx1;
    pointy[1] = pSectStiffness->dx2;
    pointy[2] = pSectStiffness->dx3;
    pointy[3] = pSectStiffness->dx4;
    // 응력계산위치의 좌표계와 형상 좌표계를 맞추기 위하여.. X(-1)
    //   pointy[0] = (-1)*pSectStiffness->dx1;
    //   pointy[1] = (-1)*pSectStiffness->dx2;
    //   pointy[2] = (-1)*pSectStiffness->dx3;
    //   pointy[3] = (-1)*pSectStiffness->dx4;

    pointz[0] = pSectStiffness->dy1;
    pointz[1] = pSectStiffness->dy2;
    pointz[2] = pSectStiffness->dy3;
    pointz[3] = pSectStiffness->dy4;

    if ( nPoint==10 || nPoint==16 )
    {
        T_SPSC_DSHR SpscDshr; SpscDshr.Initialize();
        CSectUtil::GetPscShearPosi(&SectD, nPos==0, SpscDshr, FALSE); // 여기서는 GetPscShearPosi_7DOF 적용하면 안됨..

        pointy[4] = SpscDshr.dyz[0][0];
        pointy[5] = SpscDshr.dyz[0][1];
        pointy[6] = SpscDshr.dyz[0][2];
        pointy[7] = SpscDshr.dyz[0][3];
        pointy[8] = SpscDshr.dyz[0][4];
        pointy[9] = SpscDshr.dyz[0][5];

        // 		pointy[4] = (-1)*SpscDshr.dyz[0][0];
        // 		pointy[5] = (-1)*SpscDshr.dyz[0][1];
        // 		pointy[6] = (-1)*SpscDshr.dyz[0][2];
        // 		pointy[7] = (-1)*SpscDshr.dyz[0][3];
        // 		pointy[8] = (-1)*SpscDshr.dyz[0][4];
        // 		pointy[9] = (-1)*SpscDshr.dyz[0][5];

        pointz[4] = SpscDshr.dyz[1][0];
        pointz[5] = SpscDshr.dyz[1][1];
        pointz[6] = SpscDshr.dyz[1][2];
        pointz[7] = SpscDshr.dyz[1][3];
        pointz[8] = SpscDshr.dyz[1][4];
        pointz[9] = SpscDshr.dyz[1][5];

        if ( nPoint==10 ) //
        {
            // Dll에서 받아온다.
        }
        else if ( nPoint==16 )
        {
            if ( SectD.SectBefore.nWarpingCheck[nPos] ==0 ) // Auto
            {
                pointy[10] = SpscDshr.dyz_7Dof[0][0];
                pointy[11] = SpscDshr.dyz_7Dof[0][1];
                pointy[12] = SpscDshr.dyz_7Dof[0][2];
                pointy[13] = SpscDshr.dyz_7Dof[0][3];
                pointy[14] = SpscDshr.dyz_7Dof[0][4];
                pointy[15] = SpscDshr.dyz_7Dof[0][5];

                pointz[10] = SpscDshr.dyz_7Dof[1][0];
                pointz[11] = SpscDshr.dyz_7Dof[1][1];
                pointz[12] = SpscDshr.dyz_7Dof[1][2];
                pointz[13] = SpscDshr.dyz_7Dof[1][3];
                pointz[14] = SpscDshr.dyz_7Dof[1][4];
                pointz[15] = SpscDshr.dyz_7Dof[1][5];
            }
            else // User
            {
                pointy[10] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][0] : SectD.SectBefore.dWarpingCheckPosJ[0][0];
                pointy[11] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][1] : SectD.SectBefore.dWarpingCheckPosJ[0][1];
                pointy[12] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][2] : SectD.SectBefore.dWarpingCheckPosJ[0][2];
                pointy[13] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][3] : SectD.SectBefore.dWarpingCheckPosJ[0][3];
                pointy[14] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][4] : SectD.SectBefore.dWarpingCheckPosJ[0][4];
                pointy[15] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[0][5] : SectD.SectBefore.dWarpingCheckPosJ[0][5];

                pointz[10] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][0] : SectD.SectBefore.dWarpingCheckPosJ[1][0];
                pointz[11] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][1] : SectD.SectBefore.dWarpingCheckPosJ[1][1];
                pointz[12] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][2] : SectD.SectBefore.dWarpingCheckPosJ[1][2];
                pointz[13] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][3] : SectD.SectBefore.dWarpingCheckPosJ[1][3];
                pointz[14] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][4] : SectD.SectBefore.dWarpingCheckPosJ[1][4];
                pointz[15] = ( nPos==0 ) ? SectD.SectBefore.dWarpingCheckPosI[1][5] : SectD.SectBefore.dWarpingCheckPosJ[1][5];
            }
        }
        else ASSERT(0);
    }
    else if ( nPoint==8 )
    {
        double* pointySl = NULL;
        double* pointzSl = NULL;
        pointySl = new double[4];
        pointzSl = new double[4];
        memset(pointySl, 0, sizeof(double)*4);
        memset(pointzSl, 0, sizeof(double)*4);

        CSimpleMesh simpleMesh;
        BOOL bCheck = simpleMesh.GetSlabPosition(rData, nPos, pointySl, pointzSl);

        // 슬래브
        pointy[4] = pointySl[0];
        pointy[5] = pointySl[1];
        pointy[6] = pointySl[2];
        pointy[7] = pointySl[3];

        pointz[4] = pointzSl[0] - ZERO_4;
        pointz[5] = pointzSl[1] - ZERO_4;
        pointz[6] = pointzSl[2];
        pointz[7] = pointzSl[3];
    }

    double dE = dMatlValue[0];
    double dW = dMatlValue[2];
    double dNu = dMatlValue[4];
    CArray<double, double> aElaMod; aElaMod.RemoveAll();
    CArray<double, double> aSectW; aSectW.RemoveAll();
    CArray<double, double> aNu; aNu.RemoveAll();
    BOOL bByConc = FALSE;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) bByConc = TRUE;    // PSC 파형단면(D_SECT_SHAPE_PSC_CMPWEB)은 Conc 기준으로 계산.
    if ( !GetMatl4Domain(dMatlValue, nDomain, aDomainMaterialType, dE, dW, dNu, aElaMod, aSectW, aNu, bByConc) )
    {
        ASSERT(0);
        delete[] pointy; pointy=NULL;
        delete[] pointz; pointz=NULL;
        return FALSE;
    }

    double dCentY = 0.0;
    double dCentZ = 0.0;
    if ( CSimpleMesh::CalcCentroid(dCentY, dCentZ, aDomainElemCount, aMeshElemD, aMeshNodeD, aElaMod) )
    {
        // 도심기준의 응력 위치를 Mesh 좌표 기준으로 환산
        // 솔버 좌표계와 맞춘다.
        for ( int k=0; k<nPoint; ++k )
        {
            pointy[k] += dCentY * ( -1 );
            pointz[k] += dCentZ;
        }
    }
    else { ASSERT(0); }

    BSPCombMain* pBSPCombMain;
    pBSPCombMain = ( BSPCombMain* ) GetProcAddress(hDllCalc, "BSPCombMain");
    if ( pBSPCombMain==NULL ) { ASSERT(0); return FALSE; }

    int nCombinedNum = nCountDomain;
    if ( CSimpleMesh::IsSRC(SectD) ) nCombinedNum = 1;
    else if ( CSectDB::IsSectPSComposite(SectD) || CSectDB::IsSectSteelComposite(SectD) )
    {
        nCombinedNum = 1;  // 합성단면의 모든 도메인을 1로 통일
// 		if(CSimpleMesh::IsCompositeSlabNoGap(SectD))     nCombinedNum = 1;
// 		else if(CSimpleMesh::IsCompositeSlabGapMesh(SectD))  nCombinedNum = 1;
// 		else if(nTypeBeforeAfter==2 && SectD.SectAfter.SectJ.Size[2]>1.e-8)   nCombinedNum = 2; // Gap이 있고 메쉬가 반영안된 경우는 도메인을 분리
    }

    // BSPCombMain() 함수의 입력 파라미터
    int _nnode = aMeshNodeD.GetSize();    int* p_nnode = &_nnode;           // 총 절점수
    int _npoint = nPoint;                 int* p_npoint = &_npoint;         // 응력계산위치 개수
    int _nelem = aMeshElemD.GetSize();    int* p_nelem = &_nelem;           // 총 요소수
    int _nprop = nCountDomain;            int* p_nprop = &_nprop;           // 총 material 수
    int _ncombined = nCombinedNum;        int* p_ncombined = &_ncombined;   // 연결되지 않은 단면의 수

    // 절점의 좌표
    // * 주의사항 (솔버에서 y-축 :수직, z-축 :수평으로 계산하고 있어서, 좌표는 y,z를 바꿔서 넘겨야 함.)
    CArray<double, double> aYNode; aYNode.RemoveAll();  aYNode.SetSize(_nnode);
    CArray<double, double> aZNode; aZNode.RemoveAll();  aZNode.SetSize(_nnode);
    double dY=0., dZ=0.;
    for ( int i=0; i<_nnode; i++ )
    {
        dY = aMeshNodeD[i].x * ( -1 ); // 솔버 좌표계와 맞추기 위해
        //dY = aMeshNodeD[i].x; // 응력계산위치의 좌표계와 맞추기 위하여..
        dZ = aMeshNodeD[i].y;

        aYNode.SetAt(i, dY);
        aZNode.SetAt(i, dZ);
    }
    double* p_ynode = aZNode.GetData();   // 절점의 y좌표
    double* p_znode = aYNode.GetData();   // 절점의 z좌표

    double* p_pointy = pointz;    // 응력계산 위치의 y좌표
    double* p_pointz = pointy;    // 응력계산 위치의 z좌표

    // 요소의 절점 연결 번호 (array size = 요소수 * 3, 저차 삼각형요소, 절점의 최초 시작은 1부터)
    CArray<int, int> _iconn;    _iconn.RemoveAll();    _iconn.SetSize(_nelem*3);    // 삼각형 요소.
    int iConnNum = 0;
    for ( int i=0; i<_nelem; ++i )
    {
        if ( aMeshElemD[i].aNodeK.GetSize() != 3 ) { ASSERT(0); return FALSE; }

        _iconn[iConnNum] = aMeshElemD[i].aNodeK[0] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[1] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[2] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
    }
    ASSERT(iConnNum == _iconn.GetSize());
    int* p_iconn = _iconn.GetData();

    // 요소 갯수 검증
    int nDomainElemSum = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )  nDomainElemSum += aDomainElemCount[i];
    ASSERT(nDomainElemSum == _nelem);

    // 요소의 재료 index (array size =  요소수)
    CArray<int, int> _elem_prop;    _elem_prop.RemoveAll();    _elem_prop.SetSize(_nelem);

    ASSERT(aDomainElemCount.size() == aDomainMaterialType.GetSize());
    int  nFactor = 1;
    int iElem = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )
    {
        for ( int k=0; k<aDomainElemCount[i]; ++k )
        {
            nFactor = ( aDomainMaterialType[i]==13 ) ? -1 : 1; // 13=Gap
            _elem_prop[iElem] = nFactor * ( i+1 );    // p_sect_e, p_sect_nu, p_sect_w의 참조 index (fortran이기 때문에 1부터 시작). 사실상 소속 domain index
            iElem++;
        }
    }
    int* p_elem_prop = _elem_prop.GetData();

    //////////////////////////////////////////////////////////////////////////
    // combined section의 노드수 (array size = p_ncombined)
    CArray<int, int> _comb_node;    _comb_node.RemoveAll();    _comb_node.SetSize(_ncombined);
    int iStart = 0;
    int iEndCount = 0;

    CMapEx<int, int, int, int> mNodeOfDomain;
    if ( nCombinedNum==1 )
    {
        mNodeOfDomain.RemoveAll();
        for ( int i=0; i<aDomainElemCount.size(); ++i )
        {
            if ( i>0 ) iStart += aDomainElemCount[i-1];
            iEndCount = iStart+aDomainElemCount[i];

            // 개별 Domain에 대한 loop
            for ( int k=iStart; k<iEndCount; ++k )
            {
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[0], 0);
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[1], 0);
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[2], 0);
            }
        }
        _comb_node[0] = mNodeOfDomain.GetCount();
    }
    else
    {
        for ( int i=0; i<_ncombined; ++i )
        {
            if ( i>0 ) iStart += aDomainElemCount[i-1];
            iEndCount = iStart+aDomainElemCount[i];

            mNodeOfDomain.RemoveAll();

            // 개별 Domain에 대한 loop
            for ( int k=iStart; k<iEndCount; ++k )
            {
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[0], 0);
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[1], 0);
                mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[2], 0);
            }
            _comb_node[i] = mNodeOfDomain.GetCount();
        }
    }
    int* p_comb_node = _comb_node.GetData();

    //////////////////////////////////////////////////////////////////////////
    // combined section의 요소수 (array size = p_ncombined)
    CArray<int, int> _comb_elem;    _comb_elem.RemoveAll();    _comb_elem.SetSize(_ncombined);
    if ( nCombinedNum==1 )
    {
        _comb_elem[0] = iElem;
    }
    else
    {
        for ( int i=0; i<_ncombined; ++i )  _comb_elem[i] = aDomainElemCount[i];
    }

    int* p_comb_elem = _comb_elem.GetData();

    double _E = dE;       double* p_E = &_E;        // 환산단면의 기준이 되는 탄성계수
    double _NU = dNu;     double* p_NU = &_NU;      // 환산단면의 기준이 되는 푸아송비
    double _W = dW;       double* p_W = &_W;        // 환산단면의 기준이 되는 단위중량

    double* p_sect_e = aElaMod.GetData();           // 합성단면의 단면별 탄성계수 (array size = p_nprop)
    double* p_sect_nu = aNu.GetData();              // 합성단면의 단면별 푸아송비 (array size = p_nprop)
    double* p_sect_w = aSectW.GetData();            // 합성단면의 단면별 단위중량 (array size = p_nprop)

    int _nrebar = 0;    int* p_nrebar = &_nrebar;   // 철근 개수
    CArray<double, double> _asr; _asr.RemoveAll();  _asr.Add(0.0);  // _asr.SetSize(_nrebar);
    double* p_asr = _asr.GetData();                 // 철근의 반지름
    CArray<double, double> _dsy; _dsy.RemoveAll();  _dsy.Add(0.0);  // _dsy.SetSize(_nrebar);
    double* p_dsy = _dsy.GetData();                 // 철근이 위치한 y좌표
    CArray<double, double> _dsz; _dsz.RemoveAll();  _dsz.Add(0.0);  // _dsz.SetSize(_nrebar);
    double* p_dsz = _dsz.GetData();                 // 철근이 위치한 z좌표
    CArray<double, double> _ES; _ES.RemoveAll();  _ES.Add(0.0);  // _ES.SetSize(_nrebar);
    double* p_ES = _ES.GetData();                   // 철근의 탄성계수 (array size = p_nrebar)
    CArray<double, double> _WS; _WS.RemoveAll();  _WS.Add(0.0);  // _WS.SetSize(_nrebar);
    double* p_WS = _WS.GetData();                   // 철근의 단위중량 (array size = p_nrebar)

    int _ioptn = 2;
    if ( bConsiderWarpingEffect )
    {
        if ( bAuto4P ) _ioptn = 3; // 자동일 때 : 해석에서 넘어옴.
        else        _ioptn = 2; // 사용자 입력일때
    }
    int* p_ioptn = &_ioptn;         // 0 = 1~22+33+34  항목만 계산; 1= 1~34까지만 계산; 2=모두 계산 (추가)3: 14point  고려
    int _iref = 0;      int* p_iref = &_iref;           // 0 = 도심기준으로 cdef출력, 1=전단중심 기준으로 cdef 출력
    int _elemtype = 0;  int* p_elemtype = &_elemtype;   // 요소 종류 0 = 고차 요소, 1= 저차요소

    //--------------------------------------------------------------
    // Mesh 검증용 mct 생성.
    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    if ( bDumpSectMeshFile )  WriteMctOfMesh2(_nnode, p_ynode, p_znode, strPostFix);
    //--------------------------------------------------------------

    // PBeamDll에 넘기는 Param을 파일로 씀
    BOOL bWritePBeamDllParamToFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFWritePBeamDllParamToFile"), _T("yes")) ) bWritePBeamDllParamToFile = TRUE;
    if ( bWritePBeamDllParamToFile )
    {
        if      (nTypeBeforeAfter == 1) strPostFix += _T("_STF1");
        else if (nTypeBeforeAfter == 2) strPostFix += _T("_STF2");

        WritePBeamDllParamToFileNew(cdef, 0, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
            p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
            p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
            p_asr, p_dsy, p_dsz, p_ES, p_WS, p_ioptn, p_iref, p_elemtype, strPostFix);
    }

    BOOL bCatchDllException = FALSE;
#ifdef _DEBUG
    if ( CTestEnvMgr::IsTestEnvST(_T("CatchDllException"), _T("yes")) ) bCatchDllException = TRUE;
#endif // _DEBUG
    if ( bCatchDllException )
    {
        try // 처리되지 않은 예외 안뜨게 하기 위한 코드... 속성 - C/C++-코드 생성 - C++ 예외 처리 가능 - 예외 처리 옵션 - /EHa 로 설정해야 먹힘
        {
            // 이 코드를 넣은 이유는 Debug모드일 때 Dll에서 처리안된 예외가 있어서 무한 ASSERT가 걸리는 것을 방지하기 위함인데..
            // 이 코드를 넣기 전 Release빌드에서 잘 돌아가던 것이 이놈을 넣은 뒤엔 돌다가 중간에 튕겨버릴 수 있다....
            // 개발 중에는 돌다가 튕겨서 값이 잘못되더라도 진행은 하고 싶은경우가 있어서.. 이 상황을 위해 Registry + Debug모드 일 때만 Exception을 받도록 처리 한다.

            int nResult = ( *pBSPCombMain )( pbeam, cdef, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
                p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
                p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
                p_asr, p_dsy, p_dsz, p_ES, p_WS, p_ioptn, p_iref, p_elemtype );
        }
        catch ( CException* e )
        {
            e = NULL; // warning제거용
            // 아무거도 안함
        }
        catch ( ... ) // else
        {
            // 아무거도 안함
        }
    }
    else
    {
        int nResult = ( *pBSPCombMain )( pbeam, cdef, p_nnode, p_npoint, p_nelem, p_nprop, p_ncombined,
            p_ynode, p_znode, p_pointy, p_pointz, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
            p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
            p_asr, p_dsy, p_dsz, p_ES, p_WS, p_ioptn, p_iref, p_elemtype );
    }

    delete[] pointy; pointy=NULL;
    delete[] pointz; pointz=NULL;

    return TRUE;
}
void CDb_DllBaseManager::WritePBeamDllParamToFile(double* cdef, int nCdef, int* p_nnode, int* p_npoint, int* p_nelem, int* p_nprop, int* p_ncombined,
    double* p_ynode, double* p_znode, double* p_pointy, double* p_pointz, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem,
    double* p_E, double* p_NU, double* p_w, double* p_sect_e, double* p_sect_nu, double* p_sect_w, int* p_nrebar,
    double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS, int* p_ipotn, int* p_iref, int* p_elemtype)
{
    BOOL bWritePBeamDllParamToFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFWritePBeamDllParamToFile"), _T("yes")) ) bWritePBeamDllParamToFile = TRUE;
    ASSERT(bWritePBeamDllParamToFile);

    static int nCallCnt = 1;
    CString strPath;
    strPath.Format(_T("c:/WritePBeamDllParamToFile%d.txt"), nCallCnt++);
    std::ofstream file(strPath, std::ios::trunc); // 관리자 권한으로 실행하세요~

    if ( file )
    {
        CString strEachLine;
        if ( cdef )
        {
            for ( int i = 0; i < nCdef; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("cdef\t%g\n"), cdef[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%g\n"), cdef[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_nnode )
        {
            strEachLine.Format(_T("p_nnode\t%d\n"), *p_nnode);
            file << strEachLine;
        }

        if ( p_npoint )
        {
            strEachLine.Format(_T("p_npoint\t%d\n"), *p_npoint);
            file << strEachLine;
        }

        if ( p_nelem )
        {
            strEachLine.Format(_T("p_nelem\t%d\n"), *p_nelem);
            file << strEachLine;
        }

        if ( p_nprop )
        {
            strEachLine.Format(_T("p_nprop\t%d\n"), *p_nprop);
            file << strEachLine;
        }

        if ( p_ncombined )
        {
            strEachLine.Format(_T("p_ncombined\t%d\n"), *p_ncombined);
            file << strEachLine;
        }

        if ( p_nnode && p_ynode )
        {
            for ( int i = 0; i < *p_nnode; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_ynode\t%g\n"), p_ynode[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%g\n"), p_ynode[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_nnode && p_znode )
        {
            for ( int i = 0; i < *p_nnode; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_znode\t%g\n"), p_znode[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%g\n"), p_znode[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_nnode && p_pointy )
        {
            for ( int i = 0; i < *p_npoint; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_pointy\t%g\n"), p_pointy[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%g\n"), p_pointy[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_npoint && p_pointz )
        {
            for ( int i = 0; i < *p_npoint; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_pointz\t%g\n"), p_pointz[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%g\n"), p_pointz[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_nelem && p_iconn )
        {
            for ( int i = 0; i < ( *p_nelem ) * 3; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_iconn\t%d\n"), p_iconn[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%d\n"), p_iconn[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_nelem && p_elem_prop )
        {
            for ( int i = 0; i < *p_nelem; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_elem_prop\t%d\n"), p_elem_prop[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%d\n"), p_elem_prop[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_ncombined && p_comb_node )
        {
            for ( int i = 0; i < *p_ncombined; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_comb_node\t%d\n"), p_comb_node[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%d\n"), p_comb_node[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_ncombined && p_comb_elem )
        {
            for ( int i = 0; i < *p_ncombined; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_comb_elem\t%d\n"), p_comb_elem[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%d\n"), p_comb_elem[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_E )
        {
            strEachLine.Format(_T("p_E\t%g\n"), *p_E);
            file << strEachLine;
        }

        if ( p_NU )
        {
            strEachLine.Format(_T("p_NU\t%g\n"), *p_NU);
            file << strEachLine;
        }

        if ( p_w )
        {
            strEachLine.Format(_T("p_w\t%g\n"), *p_w);
            file << strEachLine;
        }

        if ( p_nprop && p_sect_e )
        {
            for ( int i = 0; i < *p_nprop; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_sect_e\t%g\n"), p_sect_e[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%g\n"), p_sect_e[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_nprop && p_sect_nu )
        {
            for ( int i = 0; i < *p_nprop; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_sect_nu\t%g\n"), p_sect_nu[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%g\n"), p_sect_nu[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_nprop && p_sect_w )
        {
            for ( int i = 0; i < *p_nprop; i++ )
            {
                if ( i == 0 )
                {
                    strEachLine.Format(_T("p_sect_w\t%g\n"), p_sect_w[i]);
                }
                else
                {
                    strEachLine.Format(_T("\t%g\n"), p_sect_w[i]);
                }
                file << strEachLine;
            }
        }

        if ( p_nrebar )
        {
            strEachLine.Format(_T("p_nrebar\t%d\n"), *p_nrebar);
            file << strEachLine;
        }

        ASSERT(*p_nrebar == 0);// 여기 ASSERT 걸리면 이 뒤에 있는 변수들 출력.. 개수는 p_nrebar 개임 ( double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS )

        if ( p_ipotn )
        {
            strEachLine.Format(_T("p_ipotn\t%d\n"), *p_ipotn);
            file << strEachLine;
        }

        if ( p_iref )
        {
            strEachLine.Format(_T("p_iref\t%d\n"), *p_iref);
            file << strEachLine;
        }

        if ( p_elemtype )
        {
            strEachLine.Format(_T("p_elemtype\t%d\n"), *p_elemtype);
            file << strEachLine;
        }

        file.close();
    }
    else
    {
        ASSERT(0); // vs2012를 관리자 권한으로 다시 실행하세요. 아니면 해당 txt 파일을 다른 프로그램이 잡고 있을수도..?
    }
}

void CDb_DllBaseManager::WritePBeamDllParamToFileNew(double* cdef, int nCdef, int* p_nnode, int* p_npoint, int* p_nelem, int* p_nprop, int* p_ncombined,
    double* p_ynode, double* p_znode, double* p_pointy, double* p_pointz, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem,
    double* p_E, double* p_NU, double* p_w, double* p_sect_e, double* p_sect_nu, double* p_sect_w, int* p_nrebar,
    double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS, int* p_ipotn, int* p_iref, int* p_elemtype, LPCTSTR lpszPostFix)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
    CString strModelPath=_T("");
    strModelPath = pDoc->GetPathName();
    CString strMctPath = _T("");

    if ( !strModelPath.IsEmpty() )
    {
        CFileCtrl FileCtrl(strModelPath);
        CString csFolder=_T("");
        csFolder = FileCtrl.GetFilePathAndNameWithoutExtension();
        strMctPath = csFolder + _T("_PbeamInput") + lpszPostFix + (CProduct::IsRusLocal() ? _T(".nbt") : _T(".mct"));
    }
    else { ASSERT(0); return; }

    CDebugFile log(strMctPath);
    log.Clear();

    //if (file)
    {
        //
        log.Write(_T("*cdef\n"));
        if ( cdef )
        {
            for ( int i = 0; i < nCdef; i++ )
            {
                log.Write(_T("%d, %.20lf\n"), i+1, cdef[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nnode\n"));
        if ( p_nnode )
        {
            log.Write(_T("%d\n"), *p_nnode);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_npoint\n"));
        if ( p_npoint )
        {
            log.Write(_T("%d\n"), *p_npoint);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nelem\n"));
        if ( p_nelem )
        {
            log.Write(_T("%d\n"), *p_nelem);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nprop\n"));
        if ( p_nprop )
        {
            log.Write(_T("%d\n"), *p_nprop);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_ncombined\n"));
        if ( p_ncombined )
        {
            log.Write(_T("%d\n"), *p_ncombined);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_ynode & p_znode\n"));
        if ( p_nnode && p_ynode && p_znode )
        {
            for ( int i = 0; i < *p_nnode; i++ )
            {
                log.Write(_T("%d, %.20lf, %.20lf\n"), i+1, p_ynode[i], p_znode[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_pointy & p_pointz\n"));
        if ( p_nnode && p_pointy && p_pointz )
        {
            for ( int i = 0; i < *p_npoint; i++ )
            {
                log.Write(_T("%d, %.20lf, %.20lf\n"), i+1, p_pointy[i], p_pointz[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_iconn\n"));
        if ( p_nelem && p_iconn )
        {
            for ( int i = 0; i < ( *p_nelem ) * 3; i++ )
            {
                log.Write(_T("%d\n"), p_iconn[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_elem_prop\n"));
        if ( p_nelem && p_elem_prop )
        {
            for ( int i = 0; i < *p_nelem; i++ )
            {
                log.Write(_T("%d\n"), p_elem_prop[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_comb_node\n"));
        if ( p_ncombined && p_comb_node )
        {
            for ( int i = 0; i < *p_ncombined; i++ )
            {
                log.Write(_T("%d\n"), p_comb_node[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_comb_elem\n"));
        if ( p_ncombined && p_comb_elem )
        {
            for ( int i = 0; i < *p_ncombined; i++ )
            {
                log.Write(_T("%d\n"), p_comb_elem[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_E, p_NU, p_w \n"));
        if ( p_E )
        {
            log.Write(_T("%.20lf, %.20lf, %.20lf\n"), *p_E, *p_NU, *p_w);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_sect_e \n"));
        if ( p_nprop && p_sect_e )
        {
            for ( int i = 0; i < *p_nprop; i++ )
            {
                log.Write(_T("%d, %.20lf\n"), i+1, p_sect_e[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_sect_nu \n"));
        if ( p_nprop && p_sect_nu )
        {
            for ( int i = 0; i < *p_nprop; i++ )
            {
                log.Write(_T("%d, %.20lf\n"), i+1, p_sect_nu[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_sect_w \n"));
        if ( p_nprop && p_sect_w )
        {
            for ( int i = 0; i < *p_nprop; i++ )
            {
                log.Write(_T("%d, %.20lf\n"), i+1, p_sect_w[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nrebar \n"));
        if ( p_nrebar )
        {
            log.Write(_T("%d\n"), *p_nrebar);
        }
        log.Write(_T("\n"));

        //ASSERT(*p_nrebar == 0);// 여기 ASSERT 걸리면 이 뒤에 있는 변수들 출력.. 개수는 p_nrebar 개임 ( double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS )

        //
        log.Write(_T("*p_ipotn\n"));
        if ( p_ipotn )
        {
            log.Write(_T("%d\n"), *p_ipotn);
        }
        log.Write(_T("\n"));

        //
        if ( p_iref )
        {
            log.Write(_T("*p_iref\n"));
        }
        log.Write(_T("\n"));

        //
        if ( p_elemtype )
        {
            log.Write(_T("*p_elemtype\n"));
        }
        log.Write(_T("\n"));
    }
    // 	else
    // 	{
    // 		ASSERT(0); // vs2012를 관리자 권한으로 다시 실행하세요. 아니면 해당 txt 파일을 다른 프로그램이 잡고 있을수도..?
    // 	}
}

void CDb_DllBaseManager::WritePBeamDllParamToFileDetail(double* force, int nStressSize, int* p_nnode, int* p_npoint, int* p_nelem, int* p_nprop, int* p_ncombined,
                                                        double* p_ynode, double* p_znode, double* p_pointy, double* p_pointz, int* p_iconn, int* p_elem_prop, int* p_comb_node, int* p_comb_elem,
                                                        double* p_E, double* p_NU, double* p_sect_e, double* p_sect_nu, 
                                                        int* p_nrebar, double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, 
                                                        int* p_iref, int* p_nload, double* p_bst, double* p_ctex, int* p_elemtype, LPCTSTR lpszPostFix)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
    CString strModelPath = _T("");
    strModelPath = pDoc->GetPathName();
    CString strMctPath = _T("");

    if (!strModelPath.IsEmpty())
    {
        CFileCtrl FileCtrl(strModelPath);
        CString csFolder = _T("");
        csFolder = FileCtrl.GetFilePathAndNameWithoutExtension();
        strMctPath = csFolder + _T("_PbeamBSPDetailInput") + lpszPostFix + (CProduct::IsRusLocal() ? _T(".nbt") : _T(".mct"));
    }
    else { ASSERT(0); return; }

    CDebugFile log(strMctPath);
    log.Clear();

    //if (file)
    {
        //
        log.Write(_T("*force\n"));
        if (force)
        {
            for (int i = 0; i < 8; i++)
            {
                log.Write(_T("%.20lf\n"), force[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*StressSize\n"));
        log.Write(_T("%d\n"), nStressSize);
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nnode\n"));
        if (p_nnode)
        {
            log.Write(_T("%d\n"), *p_nnode);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_npoint\n"));
        if (p_npoint)
        {
            log.Write(_T("%d\n"), *p_npoint);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nelem\n"));
        if (p_nelem)
        {
            log.Write(_T("%d\n"), *p_nelem);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nprop\n"));
        if (p_nprop)
        {
            log.Write(_T("%d\n"), *p_nprop);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_ncombined\n"));
        if (p_ncombined)
        {
            log.Write(_T("%d\n"), *p_ncombined);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_ynode & p_znode\n"));
        if (p_nnode && p_ynode && p_znode)
        {
            for (int i = 0; i < *p_nnode; i++)
            {
                log.Write(_T("%d, %.20lf, %.20lf\n"), i + 1, p_ynode[i], p_znode[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_pointy & p_pointz\n"));
        if (p_nnode && p_pointy && p_pointz)
        {
            for (int i = 0; i < *p_npoint; i++)
            {
                log.Write(_T("%d, %.20lf, %.20lf\n"), i + 1, p_pointy[i], p_pointz[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_iconn\n"));
        if (p_nelem && p_iconn)
        {
            for (int i = 0; i < (*p_nelem) * 3; i++)
            {
                log.Write(_T("%d\n"), p_iconn[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_elem_prop\n"));
        if (p_nelem && p_elem_prop)
        {
            for (int i = 0; i < *p_nelem; i++)
            {
                log.Write(_T("%d\n"), p_elem_prop[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_comb_node\n"));
        if (p_ncombined && p_comb_node)
        {
            for (int i = 0; i < *p_ncombined; i++)
            {
                log.Write(_T("%d\n"), p_comb_node[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_comb_elem\n"));
        if (p_ncombined && p_comb_elem)
        {
            for (int i = 0; i < *p_ncombined; i++)
            {
                log.Write(_T("%d\n"), p_comb_elem[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_E, p_NU\n"));
        if (p_E)
        {
            log.Write(_T("%.20lf, %.20lf\n"), *p_E, *p_NU);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_sect_e \n"));
        if (p_nprop && p_sect_e)
        {
            for (int i = 0; i < *p_nprop; i++)
            {
                log.Write(_T("%d, %.20lf\n"), i + 1, p_sect_e[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_sect_nu \n"));
        if (p_nprop && p_sect_nu)
        {
            for (int i = 0; i < *p_nprop; i++)
            {
                log.Write(_T("%d, %.20lf\n"), i + 1, p_sect_nu[i]);
            }
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nrebar \n"));
        if (p_nrebar)
        {
            log.Write(_T("%d\n"), *p_nrebar);
        }
        log.Write(_T("\n"));

        //ASSERT(*p_nrebar == 0);// 여기 ASSERT 걸리면 이 뒤에 있는 변수들 출력.. 개수는 p_nrebar 개임 ( double* p_asr, double* p_dsy, double* p_dsz, double* p_ES, double* p_WS )

        //
        log.Write(_T("*p_iref\n"));
        if (p_iref)
        {
            log.Write(_T("%d\n"), *p_iref);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_nload\n"));
        if (p_nload)
        {
            log.Write(_T("%d\n"), *p_nload);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_bst\n"));
        if (p_bst)
        {
            log.Write(_T("%.20lf\n"), *p_bst);
        }
        log.Write(_T("\n"));

        //
        log.Write(_T("*p_ctex \n"));
        if (p_ctex)
        {
            log.Write(_T("%.20lf\n"), *p_ctex);
        }
        log.Write(_T("\n"));

        //
        if (p_elemtype)
        {
            log.Write(_T("*p_elemtype\n"));
        }
        log.Write(_T("\n"));
    }
    // 	else
    // 	{
    // 		ASSERT(0); // vs2012를 관리자 권한으로 다시 실행하세요. 아니면 해당 txt 파일을 다른 프로그램이 잡고 있을수도..?
    // 	}
}

// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CDb_DllBaseManager::Get_SectionPropertyMesh_Cscs(T_SECT_D& rData, int nPos, int nTypeBeforeAfter, double dMatlValue[6], int nResult,
    double** pbeam, int& rnprop)
{
    rnprop = 0;
    if ( nTypeBeforeAfter != 2 ) { ASSERT(0); return FALSE; }

    T_SECT_D SectD = rData;

    // 단면 형상/메쉬 얻어오기.
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD_girder;    aMeshElemD_girder.RemoveAll();
    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD_slab;    aMeshElemD_slab.RemoveAll();
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD_girder;    aMeshNodeD_girder.RemoveAll();
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD_slab;    aMeshNodeD_slab  .RemoveAll();
	std::vector<int> aDomainElemCount_girder;                   aDomainElemCount_girder.clear();
	std::vector<int> aDomainElemCount_slab;                   aDomainElemCount_slab  .clear();
    CArray<int, int> aDomainMaterialType_girder;                aDomainMaterialType_girder.RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입
    CArray<int, int> aDomainMaterialType_slab;                aDomainMaterialType_slab  .RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입
    CSimpleMesh simpleMesh;
    BOOL bMerge = TRUE;

    // 파형강판 예외 처리. by Hsshim.
    // 기존에는 파형강판에 대해서 탄성계수비(Es/Ec)를 고려해서 환산한 면적으로 Mesh가 되는 문제가 있어서,
    // 형상을 그대로 고려할 수 있도록 탄성계수비를 1.0으로 치환해서 Mesh를 얻어오도록 수정
    double dMatl_Elast_back = SectD.SectBefore.Matl_Elast;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = 1.0; }

    int nTypeBeforeAfter_temp = nTypeBeforeAfter;
    nTypeBeforeAfter_temp = 11;   // 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
    if ( !simpleMesh.GetSectMesh(SectD, nPos, nTypeBeforeAfter_temp, aMeshElemD_girder, aMeshNodeD_girder, aDomainElemCount_girder, aDomainMaterialType_girder, FALSE, NULL) ) { return FALSE; }

    nTypeBeforeAfter_temp = 12;   // 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
    if ( !simpleMesh.GetSectMesh(SectD, nPos, nTypeBeforeAfter_temp, aMeshElemD_slab, aMeshNodeD_slab, aDomainElemCount_slab, aDomainMaterialType_slab, FALSE, NULL) ) { return FALSE; }

    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) { SectD.SectBefore.Matl_Elast = dMatl_Elast_back; }

    // domain 합성 ----------------------------------

    // part 2의 node 번호는 part 1에 이어서 시작.
    int nCountNode_offset = aMeshNodeD_girder.GetSize();
    simpleMesh.RenumberNodeByOffset(aMeshElemD_slab, aMeshNodeD_slab, nCountNode_offset);

    CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> aMeshElemD;    aMeshElemD.RemoveAll();
    CArray<T_MESH_NODE_D, T_MESH_NODE_D&> aMeshNodeD;    aMeshNodeD.RemoveAll();
	std::vector<int> aDomainElemCount;                   aDomainElemCount.clear();
    CArray<int, int> aDomainMaterialType;                aDomainMaterialType.RemoveAll();    // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입

    aMeshElemD.Append(aMeshElemD_girder);
    aMeshElemD.Append(aMeshElemD_slab);

    aMeshNodeD.Append(aMeshNodeD_girder);
    aMeshNodeD.Append(aMeshNodeD_slab);

	std::copy(aDomainElemCount_girder.begin(), aDomainElemCount_girder.end(), std::back_inserter(aDomainElemCount));
	std::copy(aDomainElemCount_slab.begin(), aDomainElemCount_slab.end(), std::back_inserter(aDomainElemCount));

    aDomainMaterialType.Append(aDomainMaterialType_girder);
    aDomainMaterialType.Append(aDomainMaterialType_slab);

    // domain 합성 ----------------------------------

    // 단면 속성 계산
    int nCountDomain = (int)aDomainElemCount.size();

    HINSTANCE hDllCalc = GetOrLoad_PBeamLMeshDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    T_SECT_STIFFNESS* pSectStiffness = NULL;
    if ( nPos==0 ) pSectStiffness = &SectD.SectBefore.SectI.Stiffness;
    else        pSectStiffness = &SectD.SectBefore.SectJ.Stiffness;

    if ( IsUseStiffBySectBefore(rData) ) pSectStiffness = &rData.SectBefore.Stiffness;

    int nDomain = nCountDomain;

    double dE = dMatlValue[0];
    double dW = dMatlValue[2];
    double dNu = dMatlValue[4];
    CArray<double, double> aElaMod; aElaMod.RemoveAll();
    CArray<double, double> aSectW; aSectW.RemoveAll();
    CArray<double, double> aNu; aNu.RemoveAll();
    BOOL bByConc = FALSE;
    if ( SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB ) bByConc = TRUE;    // PSC 파형단면(D_SECT_SHAPE_PSC_CMPWEB)은 Conc 기준으로 계산.
    if ( !GetMatl4Domain(dMatlValue, nDomain, aDomainMaterialType, dE, dW, dNu, aElaMod, aSectW, aNu, bByConc) ) { ASSERT(0); return FALSE; }

    BSPPartMain* pBSPPartMain;
    pBSPPartMain = ( BSPPartMain* ) GetProcAddress(hDllCalc, "BSPPartMain");
    if ( pBSPPartMain==NULL ) { ASSERT(0); return FALSE; }

    // BSPCombMain() 함수의 입력 파라미터
    int _nnode = aMeshNodeD.GetSize();    int* p_nnode = &_nnode;           // 총 절점수
    int _nelem = aMeshElemD.GetSize();    int* p_nelem = &_nelem;           // 총 요소수
    int _nprop = nCountDomain;            int* p_nprop = &_nprop;           // 총 material 수
    int _ncombined = nCountDomain;        int* p_ncombined = &_ncombined;   // 연결되지 않은 단면의 수

    int nProp = _nprop;
    int nDataPBeam = nResult*nProp;   // (nprop 는 시공단계 합성단면의 part 수이며, part수와 material property의 개수는 일치해야 합니다.)
    *pbeam = new double[nDataPBeam];
    memset(*pbeam, 0, sizeof(double)*nDataPBeam);

    // 절점의 좌표
    // * 주의사항 (솔버에서 y-축 :수직, z-축 :수평으로 계산하고 있어서, 좌표는 y,z를 바꿔서 넘겨야 함.)
    CArray<double, double> aYNode; aYNode.RemoveAll();  aYNode.SetSize(_nnode);
    CArray<double, double> aZNode; aZNode.RemoveAll();  aZNode.SetSize(_nnode);
    double dY=0., dZ=0.;
    for ( int i=0; i<_nnode; i++ )
    {
        dY = aMeshNodeD[i].x * ( -1 ); // 솔버 좌표계와 맞추기 위해
        //dY = aMeshNodeD[i].x; // 응력계산위치의 좌표계와 맞추기 위하여..
        dZ = aMeshNodeD[i].y;

        aYNode.SetAt(i, dY);
        aZNode.SetAt(i, dZ);
    }
    double* p_ynode = aZNode.GetData();   // 절점의 y좌표
    double* p_znode = aYNode.GetData();   // 절점의 z좌표

    // 요소의 절점 연결 번호 (array size = 요소수 * 3, 저차 삼각형요소, 절점의 최초 시작은 1부터)
    CArray<int, int> _iconn;    _iconn.RemoveAll();    _iconn.SetSize(_nelem*3);    // 삼각형 요소.
    int iConnNum = 0;
    for ( int i=0; i<_nelem; ++i )
    {
        if ( aMeshElemD[i].aNodeK.GetSize() != 3 ) { ASSERT(0); return FALSE; }

        _iconn[iConnNum] = aMeshElemD[i].aNodeK[0] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[1] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
        _iconn[iConnNum] = aMeshElemD[i].aNodeK[2] +1;  iConnNum++;  // 절점의 최초 시작은 1부터
    }
    ASSERT(iConnNum == _iconn.GetSize());
    int* p_iconn = _iconn.GetData();

    // 요소 갯수 검증
    int nDomainElemSum = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )  nDomainElemSum += aDomainElemCount[i];
    ASSERT(nDomainElemSum == _nelem);

    // 요소의 재료 index (array size =  요소수)
    CArray<int, int> _elem_prop;    _elem_prop.RemoveAll();    _elem_prop.SetSize(_nelem);
    int iElem = 0;
    for ( int i=0; i<aDomainElemCount.size(); ++i )
    {
        for ( int k=0; k<aDomainElemCount[i]; ++k )
        {
            _elem_prop[iElem] = i+1;    // p_sect_e, p_sect_nu, p_sect_w의 참조 index (fortran이기 때문에 1부터 시작). 사실상 소속 domain index
            iElem++;
        }
    }
    int* p_elem_prop = _elem_prop.GetData();

    // combined section의 노드수 (array size = p_ncombined)
    CArray<int, int> _comb_node;    _comb_node.RemoveAll();    _comb_node.SetSize(_ncombined);
    int iStart = 0;
    int iEndCount = 0;
    for ( int i=0; i<_ncombined; ++i )
    {
        if ( i>0 ) iStart += aDomainElemCount[i-1];
        iEndCount = iStart+aDomainElemCount[i];

        CMapEx<int, int, int, int> mNodeOfDomain;    mNodeOfDomain.RemoveAll();

        // 개별 Domain에 대한 loop
        for ( int k=iStart; k<iEndCount; ++k )
        {
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[0], 0);
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[1], 0);
            mNodeOfDomain.SetAt(aMeshElemD[k].aNodeK[2], 0);
        }
        _comb_node[i] = mNodeOfDomain.GetCount();
    }
    int* p_comb_node = _comb_node.GetData();

    // combined section의 요소수 (array size = p_ncombined)
    CArray<int, int> _comb_elem;    _comb_elem.RemoveAll();    _comb_elem.SetSize(_ncombined);
    for ( int i=0; i<_ncombined; ++i )  _comb_elem[i] = aDomainElemCount[i];
    int* p_comb_elem = _comb_elem.GetData();

    double _E = dE;       double* p_E = &_E;        // 환산단면의 기준이 되는 탄성계수
    double _NU = dNu;     double* p_NU = &_NU;      // 환산단면의 기준이 되는 푸아송비
    double _W = dW;       double* p_W = &_W;        // 환산단면의 기준이 되는 단위중량

    double* p_sect_e = aElaMod.GetData();           // 합성단면의 단면별 탄성계수 (array size = p_nprop)
    double* p_sect_nu = aNu.GetData();              // 합성단면의 단면별 푸아송비 (array size = p_nprop)
    double* p_sect_w = aSectW.GetData();            // 합성단면의 단면별 단위중량 (array size = p_nprop)

    int _nrebar = 0;    int* p_nrebar = &_nrebar;   // 철근 개수
    CArray<double, double> _asr; _asr.RemoveAll();  _asr.Add(0.0);  // _asr.SetSize(_nrebar);
    double* p_asr = _asr.GetData();                 // 철근의 반지름
    CArray<double, double> _dsy; _dsy.RemoveAll();  _dsy.Add(0.0);  // _dsy.SetSize(_nrebar);
    double* p_dsy = _dsy.GetData();                 // 철근이 위치한 y좌표
    CArray<double, double> _dsz; _dsz.RemoveAll();  _dsz.Add(0.0);  // _dsz.SetSize(_nrebar);
    double* p_dsz = _dsz.GetData();                 // 철근이 위치한 z좌표
    CArray<double, double> _ES; _ES.RemoveAll();  _ES.Add(0.0);  // _ES.SetSize(_nrebar);
    double* p_ES = _ES.GetData();                   // 철근의 탄성계수 (array size = p_nrebar)
    CArray<double, double> _WS; _WS.RemoveAll();  _WS.Add(0.0);  // _WS.SetSize(_nrebar);
    double* p_WS = _WS.GetData();                   // 철근의 단위중량 (array size = p_nrebar)

    int _elemtype = 0;  int* p_elemtype = &_elemtype;   // 요소 종류 0 = 고차 요소, 1= 저차요소

    //--------------------------------------------------------------
    // Mesh 검증용 mct 생성.
    BOOL bDumpSectMeshFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("SectMeshFile"), _T("yes")) ) bDumpSectMeshFile = TRUE;
    CString strPostFix;   strPostFix.Format(_T("_%s"), SectD.SName);
    if ( bDumpSectMeshFile )  WriteMctOfMesh2(_nnode, p_ynode, p_znode, strPostFix);
    //--------------------------------------------------------------

    // PBeamDll에 넘기는 Param을 파일로 씀
    BOOL bWritePBeamDllParamToFile = FALSE;
    if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFWritePBeamDllParamToFile"), _T("yes")) ) bWritePBeamDllParamToFile = TRUE;
    if ( bWritePBeamDllParamToFile )
    {
        WritePBeamDllParamToFile(NULL, 0, p_nnode, NULL, p_nelem, p_nprop, p_ncombined,
            p_ynode, p_znode, NULL, NULL, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
            p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
            p_asr, p_dsy, p_dsz, p_ES, p_WS, NULL, NULL, p_elemtype);
    }

    BOOL bCatchDllException = FALSE;

#ifdef _DEBUG
    if ( CTestEnvMgr::IsTestEnvST(_T("CatchDllException"), _T("yes")) ) bCatchDllException = TRUE;
#endif // _DEBUG

    if ( bCatchDllException )
    {
        try // 처리되지 않은 예외 안뜨게 하기 위한 코드... 속성 - C/C++-코드 생성 - C++ 예외 처리 가능 - 예외 처리 옵션 - /EHa 로 설정해야 먹힘
        {
            // 이 코드를 넣은 이유는 Debug모드일 때 Dll에서 처리안된 예외가 있어서 무한 ASSERT가 걸리는 것을 방지하기 위함인데..
            // 이 코드를 넣기 전 Release빌드에서 잘 돌아가던 것이 이놈을 넣은 뒤엔 돌다가 중간에 튕겨버릴 수 있다....
            // 개발 중에는 돌다가 튕겨서 값이 잘못되더라도 진행은 하고 싶은경우가 있어서.. 이 상황을 위해 Registry + Debug모드 일 때만 Exception을 받도록 처리 한다.

            int nResult = ( *pBSPPartMain )( *pbeam, p_nnode, p_nelem, p_nprop, p_ncombined,
                p_ynode, p_znode, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
                p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
                p_asr, p_dsy, p_dsz, p_ES, p_WS, p_elemtype );
        }
        catch ( CException* e )
        {
            e = NULL; // warning제거용
            // 아무거도 안함
        }
        catch ( ... ) // else
        {
            // 아무거도 안함
        }
    }
    else
    {
        int nResult = ( *pBSPPartMain )( *pbeam, p_nnode, p_nelem, p_nprop, p_ncombined,
            p_ynode, p_znode, p_iconn, p_elem_prop, p_comb_node, p_comb_elem,
            p_E, p_NU, p_W, p_sect_e, p_sect_nu, p_sect_w, p_nrebar,
            p_asr, p_dsy, p_dsz, p_ES, p_WS, p_elemtype );
    }

    rnprop = _nprop;

    return TRUE;
}

// PSC의 나머지 응력 Point(5~10번)의 정보를 얻어오는 함수.
// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CDb_DllBaseManager::GetStressInfo4TorsionAndWarping(T_SECT_D& rData, int nPos, int nTypeBeforeAfter, double dMatlValue[6],
    int& rNumPlane, CArray<int, int>* pPlaneVCount, CArray<BOOL, BOOL>* pPlaneVacant,
    CArray<int, int>* pPlaneVertexIndex, CArray<double, double>* pVertex, double dValue[19], double dValue2[19], double dxyNew[6], double dxzNew[6])
{
    if ( !IsSect_PSC(rData) ) return FALSE;

    double* pbeam = NULL;
    double* cdef = NULL;
    int nPoint = 10;    // 응력 point 갯수

    BOOL bAuto4P= FALSE;
    if ( CSectDB::IsEnable7thDOF() && rData.SectBefore.bConsiderWarpingEffect )
    {
        bAuto4P= ( rData.SectBefore.nWarpingCheck[nPos]==0 ) ? TRUE : FALSE;

        if ( bAuto4P ) nPoint = 10;  // 10 + 자동계산   6포인트
        else        nPoint = 16;  // 10 + 사용자입력 6포인트
    }

    //pbeam = new double[34+nPoint*3+4];
    //pbeam = new double[34+nPoint*3+5];  // 중국 요청으로 전단중심에서의 극이차모멘트 추가. 기존 PBEAM의 제일 마지막에 추가.
    int nDataPBeam = 38+nPoint*3+5;
    if ( bAuto4P ) nDataPBeam = ( 38+nPoint*3+5 ) + 30; //28 :자동 계산되는 값  nDataPBeam=101

    pbeam = new double[nDataPBeam];  // Shear Stress 결과 4개 추가됨 : 34 -> 38 (from 박건태)
    memset(pbeam, 0, sizeof(double)*nDataPBeam);

    int nDataCDEF = nPoint*2;
    cdef = new double[nDataCDEF];
    memset(cdef, 0, sizeof(double)*nDataCDEF);

    // PSC_VALUE 일때 mesh를 생성위한 Polygon 정보가 CSecShape::makePSC_Value 에서 I 단만 사용하므로
    // ConvertSectStiffData2BeforeI 함수를 호출해준다.
    // 여기서는 ConvertSectBeforeI2StiffData를 호출할 필요 없다.
    T_SECT_D rSectD_I = rData;
    //if(rData.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)
    {
        ConvertSectStiffData2BeforeI(rSectD_I, rData, nPos, nTypeBeforeAfter);
    }
    //
    Get_SectionPropertyMesh(rSectD_I, nPos, nTypeBeforeAfter, dMatlValue, pbeam, cdef, nDataCDEF);

    dValue[0] = pbeam[4];  // Js

    // 추가되는 beam stress(psc) 포인트(11~16 : 6개) // 6개*3 +1
    dValue2[0] = pbeam[4]; // 개수 맞추기 위해 일부러 넣음.

    int i=0;
    int iStart = 0;

    int nNum=6;
    int nNum2=12; //6+6
    int nPointStart = 5;  // 5번째~ 응력 Point 부터 시작
    //iStart = 34+(nPoint*1)+4;   // 34=기본 pbeam 결과, 4=5번째~ 응력 Point 부터 시작
    //iStart = 38+(nPoint*1)+4;   // 38=기본 pbeam 결과, 4=5번째~ 응력 Point 부터 시작

    double dTorsion_xy[6] = { 0.0, };   // TORSION XZ NODE 6개(5~10번 Stress Point)
    double dTorsion_xz[6] = { 0.0, };   // TORSION XZ NODE 6개(5~10번 Stress Point)
    double dTorsion_wf[6] = { 0.0, };   // WARPING FUNCTION NODE 6개(5~10번 Stress Point)

    memset(dxyNew, 0, sizeof(double)*6);
    memset(dxzNew, 0, sizeof(double)*6);
    double dTorsion_xy2[6] = { 0.0, };   // TORSION XZ NODE 6개(11~16번 Stress Point)
    double dTorsion_xz2[6] = { 0.0, };   // TORSION XZ NODE 6개(11~16번 Stress Point)
    double dTorsion_wf2[6] = { 0.0, };   // WARPING FUNCTION NODE 6개(11~16번 Stress Point)

    //iStart = 34+(nPoint*1)+4*2;   // +4*2 xy-xz가 쌍으로 있으므로 앞에 4개 응력 point 정보를 skip하기 위해 4*2를 해줌.
    iStart  = 38+( nPoint*1 )+4*2;    // +4*2 xy-xz가 쌍으로 있으므로 앞에 4개 응력 point 정보를 skip하기 위해 4*2를 해줌.
    int iStart2 = ( 38+nPoint*3+5 );  //iStart2=91(nPoint=16)  iStart2=73(nPoint=10)

    if ( !bAuto4P )
    {
        for ( i=0; i<nNum2; ++i )
        {
            if ( i<6 ) // 기존 6개 포인트
            {
                // 7자유도 dll에 y-z를 좌표축을 바꿔서 넘기므로 index가 바뀐다.
                dTorsion_xy[i] = pbeam[iStart+( 2*i )+1];
                dTorsion_xz[i] = pbeam[iStart+( 2*i )+0];

                dValue[1+i+nNum*0] = dTorsion_xy[i];
                dValue[1+i+nNum*1] = dTorsion_xz[i];
            }
            else  // 추가되는 포인트
            {
                int i2 = i-6;
                dTorsion_xy2[i2] = pbeam[iStart+( 2*i )+1];
                dTorsion_xz2[i2] = pbeam[iStart+( 2*i )+0];

                dValue2[1+i2+nNum*0] = dTorsion_xy2[i2];
                dValue2[1+i2+nNum*1] = dTorsion_xz2[i2];
            }
        }
    }
    else
    {
        for ( i=0; i<nNum2; ++i )
        {
            if ( i<6 )
            {
                // 7자유도 dll에 y-z를 좌표축을 바꿔서 넘기므로 index가 바뀐다.
                dTorsion_xy[i] = pbeam[iStart+( 2*i )+1];
                dTorsion_xz[i] = pbeam[iStart+( 2*i )+0];

                dValue[1+i+nNum*0] = dTorsion_xy[i];
                dValue[1+i+nNum*1] = dTorsion_xz[i];
            }
            else
            {
                int i2 = i-6;
                dxyNew[i2] = pbeam[iStart2+( 2*i2 )+1];           // 자동 계산된 6포인트 정보
                dxzNew[i2] = pbeam[iStart2+( 2*i2 )+0];

                dTorsion_xy2[i2] = pbeam[iStart2+15+( 2*i2 )+1];  // Max Warping Torsion, 16개 지나서..
                dTorsion_xz2[i2] = pbeam[iStart2+15+( 2*i2 )+0];

                dValue2[1+i2+nNum*0] = dTorsion_xy2[i2];
                dValue2[1+i2+nNum*1] = dTorsion_xz2[i2];
            }
        }
    }

    // WARPING FUNCTION NODE 6개(5~10번 Stress Point)
    //iStart = 34+(nPoint*0)+4;
    iStart = 38+( nPoint*0 )+4;
    if ( !bAuto4P )
    {
        for ( i=0; i<nNum2; ++i )
        {
            if ( i<6 )
            {
                dTorsion_wf[i] = pbeam[iStart+i];

                dValue[1+i+nNum*2] = dTorsion_wf[i];
            }
            else
            {
                int i2 = i-6;
                dTorsion_wf2[i2]  = pbeam[iStart+i];

                dValue2[1+i2+nNum*2] = dTorsion_wf2[i2];
            }
        }
    }
    else
    {
        for ( i=0; i<nNum2; ++i )
        {
            if ( i<6 )
            {
                dTorsion_wf[i] = pbeam[iStart+i];

                dValue[1+i+nNum*2] = dTorsion_wf[i];
            }
            else
            {
                int i2 = i-6;
                dTorsion_wf2[i2] = pbeam[iStart2+12+i2];  // 12개 지나서..

                dValue2[1+i2+nNum*2] = dTorsion_wf2[i2];
            }
        }
    }

    delete[] pbeam;
    delete[] cdef;

    return TRUE;
}

// nPos/nTypeBeforeAfter 위치 강성정보를 I-단으로 모아줌.
// 단면 강성계산을 I-단 기준으로 통일하기 위한 converting.
// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CDb_DllBaseManager::ConvertSectStiffData2BeforeI(T_SECT_D& rSectD_I, const T_SECT_D& SectD, int nPos, int nTypeBeforeAfter)
{
    // 계산 단순화를 위해 각 Position(i or j) 위치 값을 i-단면으로 치환
    if ( rSectD_I.nStype != D_SECT_TYPE_TAPERED )
    {
        if ( nPos == 0 )  // i-단면
        {
            if ( IsUseStiffBySectBefore(SectD) )
            {
                rSectD_I.SectBefore.SectI.Stiffness = SectD.SectBefore.Stiffness;
                rSectD_I.SectBefore.SectI.Design    = SectD.SectBefore.Design;
                if ( nTypeBeforeAfter == 2 )
                {
                    rSectD_I.SectBefore.SectI.Stiffness = SectD.SectAfter.Stiffness;
                    rSectD_I.SectBefore.SectI.Design    = SectD.SectAfter.Design;
                }
            }
            else
            {
                rSectD_I.SectBefore.SectI = SectD.SectBefore.SectI;
            }
        }
        else  ASSERT(0);
    }
    else    // tappered sect
    {
        if ( CSimpleMesh::IsComposite(SectD) )
        {
            if ( nPos == 0 )  // i-단면
            {
                // Copy 형상정보
                rSectD_I.SectBefore.SectI = SectD.SectBefore.SectI;

                // Copy Stiffness 정보
                if ( nTypeBeforeAfter == 2 )
                {
                    rSectD_I.SectBefore.SectI.Stiffness = SectD.SectAfter.SectI.Stiffness;
                    rSectD_I.SectBefore.SectI.Design    = SectD.SectAfter.SectI.Design;
                }
            }
            else if ( nPos == 1 )  // j-단면
            {
                // copy 형상 정보
                rSectD_I.SectBefore.SectI = SectD.CmpTapJ;
                rSectD_I.SectBefore.SectI.Stiffener = SectD.SectBefore.SectI.Stiffener;
                rSectD_I.SectBefore.SectI.SODStiffener = SectD.SectBefore.SectI.SODStiffener;

                // copy stiffness 정보
                if ( nTypeBeforeAfter == 2 )
                {
                    rSectD_I.SectBefore.SectI.Stiffness = SectD.SectAfter.SectJ.Stiffness;
                    rSectD_I.SectBefore.SectI.Design    = SectD.SectAfter.SectJ.Design;
                }
                else
                {
                    rSectD_I.SectBefore.SectI.Stiffness = SectD.SectBefore.SectJ.Stiffness;
                    rSectD_I.SectBefore.SectI.Design    = SectD.SectBefore.SectJ.Design;
                }
            }
            else  ASSERT(0);
        }
        else
        {
            if ( nPos == 0 )  // i-단면
            {
                if ( IsUseStiffBySectBefore(SectD) )
                {
                    rSectD_I.SectBefore.SectI.Stiffness = SectD.SectBefore.Stiffness;
                    rSectD_I.SectBefore.SectI.Design    = SectD.SectBefore.Design;
                }
                else
                {
                    rSectD_I.SectBefore.SectI = SectD.SectBefore.SectI;
                }
            }
            else if ( nPos == 1 )  // j-단면
            {
                if ( IsUseStiffBySectBefore(rSectD_I) )
                {
                    rSectD_I.SectBefore.SectI.Stiffness = SectD.SectBefore.Stiffness;
                    rSectD_I.SectBefore.SectI.Design    = SectD.SectBefore.Design;
                }
                else
                {
                    rSectD_I.SectBefore.SectI = SectD.SectBefore.SectJ;
                }

                rSectD_I.SectBefore.SectI.Stiffener = SectD.SectBefore.SectI.Stiffener;
                rSectD_I.SectBefore.SectI.SODStiffener = SectD.SectBefore.SectI.SODStiffener;
            }
            else  ASSERT(0);
        }

        // Tappered가 아닌 것처럼...^^
        rSectD_I.nStype = SectD.SectBefore.nStype;
    }

    return TRUE;
}

// 이전에 ConvertSectStiffData2BeforeI()가 호출되었을 때만 유효.
// nPos/nTypeBeforeAfter 위치 강성정보를 I-단으로 모아줌.
// 계산 결과를 원래 Position(i or j) 위치 값으로 치환.
// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
// @param rSectD : [out]
// @param SectD_I : [in]
BOOL CDb_DllBaseManager::ConvertSectBeforeI2StiffData(T_SECT_D& rSectD, const T_SECT_D& SectD_I, int nPos, int nTypeBeforeAfter)
{
    // 계산 결과를 원래 Position(i or j) 위치 값으로 치환
    if ( nPos == 0 )  // i-단면
    {
        // tapered composite
        if ( IsUseStiffBySectBefore(SectD_I) )
        {
            if ( nTypeBeforeAfter == 2 )  rSectD.SectAfter.Stiffness = SectD_I.SectBefore.SectI.Stiffness;
            else                        rSectD.SectBefore.Stiffness = SectD_I.SectBefore.SectI.Stiffness;

            if ( nTypeBeforeAfter == 2 )  rSectD.SectAfter.SectI.Stiffness = SectD_I.SectBefore.SectI.Stiffness;
            else                        rSectD.SectBefore.SectI.Stiffness = SectD_I.SectBefore.SectI.Stiffness;
        }
        else  rSectD.SectBefore.SectI = SectD_I.SectBefore.SectI;
    }
    else if ( nPos == 1 )  // j-단면
    {
        // tapered composite
        if ( IsUseStiffBySectBefore(SectD_I) )
        {
            if ( nTypeBeforeAfter == 2 )  rSectD.SectAfter.Stiffness = SectD_I.SectBefore.SectI.Stiffness;
            else                        rSectD.SectBefore.Stiffness = SectD_I.SectBefore.SectI.Stiffness;

            if ( nTypeBeforeAfter == 2 )  rSectD.SectAfter.SectJ.Stiffness = SectD_I.SectBefore.SectI.Stiffness;
            else                        rSectD.SectBefore.SectJ.Stiffness = SectD_I.SectBefore.SectI.Stiffness;
        }
        else  rSectD.SectBefore.SectJ = SectD_I.SectBefore.SectI;
    }
    else  ASSERT(0);

    return TRUE;
}

// @param raddiStiff : stress point가 4개보다 많은 경우에 추가적인 dw, dxy, dxz를 output으로 받기 위한 변수
// @param bIsBefore : 시공단계 전/후 판단용. 시공단계 전인가?
// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CDb_DllBaseManager::CalcSectionPropertyMesh_Cscs(T_SECT_SECTBASE_D& rSectBase_girder, T_SECT_SECTBASE_D& rSectBase_slab,
    T_SECT_D& rData, int nPos, double dMatlValue[6],
    BOOL bAllStiffness/*=FALSE*/, int nTypeBeforeAfter/*=0*/, BOOL bReturnBySectBeforeI/*=FALSE*/)

{
    // 시공단계합성단면 par2 Com 계산에 대해서만 기준단위계로 계산함.
    // 와핑관련 값들이 단위계에 따라 변동하는 경우 발생

    // 1. rData : 현재단위계 -> 기준단위계(m)
    CDBDoc  *pDoc  = CDBDoc::GetDocPoint(); ASSERT(pDoc);
    pDoc->m_pUnitCtrl->ConvertUnitSectIn(rData);
    pDoc->m_pUnitCtrl->ConvertUnitSectIn_SECTBASE(rSectBase_girder);
    pDoc->m_pUnitCtrl->ConvertUnitSectIn_SECTBASE(rSectBase_slab);

    // 2. 현재단위계 저장
    CCurUnitSaver Save(TRUE);

    // 3. 기준단위계 세팅 (메쉬에서 오차에 단위 고려됨)
    T_UNIT_INDEX CodeUnit;
    CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
    pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

    //raddiStiff.Initialize();

    // MNET:XXXX-HSSHIM-20130702.
    // DB/User 탭의 단면을 제외하고는 DBName 유무를 가지고 User 타입인지 구분하고 있어서,
    // 아래와 같이 예외처리
    if ( rData.SectBefore.SectI.DBName == _T("") )  bAllStiffness = TRUE;

    T_SECT_D SectD_temp = rData;

    //--------------------------------------------------
    // 계산 단순화를 위해 각 Position(i or j) 위치 값을 i-단면으로 치환
    // ConvertSectBeforeI2StiffData()와 커플로 호출할 것...!!
    ConvertSectStiffData2BeforeI(SectD_temp, rData, nPos, nTypeBeforeAfter);
    //--------------------------------------------------

    T_SECT_STIFFNESS* pSectStiffness = &( SectD_temp.SectBefore.SectI.Stiffness );

    CString strShape = SectD_temp.SectBefore.Shape;

    double* pbeam = NULL; // Get_SectionPropertyMesh_Cscs 안에서 new 하기 때문에 꼭 Delete해주어야 함
    int nProp_ret = 0;
    int nResult = 10;
    Get_SectionPropertyMesh_Cscs(SectD_temp, nPos, nTypeBeforeAfter, dMatlValue, nResult, &pbeam, nProp_ret);

    // Steel 기준의 Composite ratio (CSectDB::GetElast_Cscs() 참고)
    //double dRatio_E  = dMatlValue[1];
    //double dRatio_W  = dMatlValue[3];
    //double dRatio_Nu = dMatlValue[5]/dMatlValue[4];
    // Slab part Sect Property 환산 (환산 방법 : from 박건태 대리님)
    // - Area = part_Area / dRatio_E
    // - Asy  = part_Asy  / dRatio_Nu
    // - Asz  = part_Asz  / dRatio_Nu
    // - Rxx  = part_Rxx  / dRatio_Nu
    // - Ryy  = part_Ryy  / dRatio_E
    // - Rzz  = part_Rzz  / dRatio_E

    double dArea     = 0.0;   // 면적
    double dYcy      = 0.0;    // 도심 y좌표
    double dYcz      = 0.0;    // 도심 z좌표
    double Rzz       = 0.0;     // 단면 이차 모멘트 izz
    double Ryy       = 0.0;     // 단면 이차 모멘트 iyy
    double Rxx       = 0.0;     // 단면 이차 모멘트 ixx == torsional constant
    double dAsy      = 0.0;   // 전단면적 = shear correction factor (y 방향) * 면적
    double dAsz      = 0.0;   // 전단면적 = shear correction factor (z 방향) * 면적
    double dIw       = 0.0;   // warping constant
    double WArea     = 0.0;   // 단위 중량이 고려된 면적
    int nIndex=-1;
    T_SECT_STIFFNESS* pStiff = NULL;
    for ( int i=0; i<nProp_ret; ++i )
    {
        int nOffset = i*nResult;

        if ( i==0 ) pStiff = &rSectBase_girder.Stiffness;
        else if ( i == nProp_ret - 1 ) pStiff = &rSectBase_slab.Stiffness;
        else { continue; } // 일반적인 경우엔 nProp_ret가 2여서 여기 안들어오는데.. Girder의 Polygone이 2개 이상인 경우엔 nProp_ret이 3이상이 되어서 값 일부를 버려야함.. 따라서 rSectBase_girder를 쓰면 안됨...

        pStiff->Area = pbeam[1+nIndex+nOffset];   // 면적
        //pStiff->Area  = pbeam[ 1+nIndex+nOffset];   // 면적
        //pStiff->dYcy  = pbeam[ 2+nIndex+nOffset];   // 도심 y좌표
        //pStiff->dYcz  = pbeam[ 3+nIndex+nOffset];   // 도심 z좌표
        //pStiff->Rzz   = pbeam[ 4+nIndex+nOffset];   // 단면 이차 모멘트 izz
        //pStiff->Ryy   = pbeam[ 5+nIndex+nOffset];   // 단면 이차 모멘트 iyy
        pStiff->Rxx   = pbeam[6+nIndex+nOffset];   // torsional constant
        pStiff->Asy   = pbeam[8+nIndex+nOffset] * pStiff->Area;   // shear correction factor (z 방향)
        pStiff->Asz   = pbeam[7+nIndex+nOffset] * pStiff->Area;   // shear correction factor (y 방향)
        pStiff->dIw   = pbeam[9+nIndex+nOffset];   // warping constant
        //pStiff->WArea = pbeam[10+nIndex+nOffset];   // 단위 중량이 고려된 면적

        BOOL bShowPBeamOut = FALSE;
        if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFSectShowPBeamOut"), _T("yes")) ) bShowPBeamOut = TRUE;
        if ( bShowPBeamOut )
        {
            CString strTemp;
            strTemp.Format(_T("CSCS\nArea:%g,\nAsy:%g,\nAsz:%g,\nRxx:%g,\ndIw:%g"), pStiff->Area, pStiff->Asy, pStiff->Asz, pStiff->Rxx, pStiff->dIw);
            AfxMessageBox(strTemp);
        }
    }

    //   //--------------------------------------------------
    //   // 계산 결과를 원래 Position(i or j) 위치 값으로 치환
    //   // ConvertSectStiffData2BeforeI()와 커플로 호출할 것...!!
    //   if (bReturnBySectBeforeI) rData.SectBefore.SectI = SectD_temp.SectBefore.SectI;
    //   else                      ConvertSectBeforeI2StiffData(rData, SectD_temp, nPos, nTypeBeforeAfter);
    //   //--------------------------------------------------

    delete[] pbeam;

    // 4. 기준단위계에서 현재단위계로 되돌림.
    Save.Restore();

    // 5. rData : 기준단위계(m) -> 현재단위계
    pDoc->m_pUnitCtrl->ConvertUnitSectOut(rData);
    pDoc->m_pUnitCtrl->ConvertUnitSectOut_SECTBASE(rSectBase_girder);
    pDoc->m_pUnitCtrl->ConvertUnitSectOut_SECTBASE(rSectBase_slab);

    return TRUE;
}

// @param raddiStiff : stress point가 4개보다 많은 경우에 추가적인 dw, dxy, dxz를 output으로 받기 위한 변수
// @param bIsBefore : 시공단계 전/후 판단용. 시공단계 전인가?
// @param nTypeBeforeAfter : 0=not composite sect, 1=시공단계 전, 2=시공단계 후, 11=Part1, 12=Part2
BOOL CDb_DllBaseManager::CalcSectionPropertyMesh(T_SECT_D& rData, T_SECT_ADDI_STIFF& raddiStiff, int nPos, double dMatlValue[6],
    BOOL bAllStiffness/*=FALSE*/, int nTypeBeforeAfter/*=0*/, BOOL bReturnBySectBeforeI/*=FALSE*/, BOOL bConvertSectStiffData2BeforeI/*=TRUE*/)
{
    //raddiStiff.Initialize();

    // MNET:XXXX-HSSHIM-20130702.
    // DB/User 탭의 단면을 제외하고는 DBName 유무를 가지고 User 타입인지 구분하고 있어서,
    // 아래와 같이 예외처리
    if ( rData.SectBefore.SectI.DBName == _T("") )  bAllStiffness = TRUE;

    T_SECT_D SectD_temp = rData;

    //--------------------------------------------------
    // 계산 단순화를 위해 각 Position(i or j) 위치 값을 i-단면으로 치환
    // ConvertSectBeforeI2StiffData()와 커플로 호출할 것...!!
    if ( bConvertSectStiffData2BeforeI )  ConvertSectStiffData2BeforeI(SectD_temp, rData, nPos, nTypeBeforeAfter);
    //--------------------------------------------------

    T_SECT_STIFFNESS* pSectStiffness = &( SectD_temp.SectBefore.SectI.Stiffness );
    T_SECT_DESIGN* pSectDesign = &( SectD_temp.SectBefore.SectI.Design );

    CString strShape = SectD_temp.SectBefore.Shape;
    double* pbeam = NULL;
    double* cdef = NULL;
    int nPoint = 4;
    BOOL bAuto4P = FALSE;
    if ( IsSect_PSC(SectD_temp) )
    {
        nPoint = 10;

        if ( CSectDB::IsEnable7thDOF() && rData.SectBefore.bConsiderWarpingEffect )
        {
            bAuto4P= ( rData.SectBefore.nWarpingCheck[nPos]==0 ) ? TRUE : FALSE;

            if ( bAuto4P ) nPoint = 10;  // 10 + 자동계산   6포인트
            else        nPoint = 16;  // 10 + 사용자입력 6포인트
        }
    }
    else if ( nTypeBeforeAfter==2 && CSectDB::IsSectSteelComposite(rData) )
    {
        nPoint = 8;
    }

    //pbeam = new double[34+nPoint*3+4];
    //pbeam = new double[34+nPoint*3+5];  // 중국 요청으로 전단중심에서의 극이차모멘트 추가. 기존 PBEAM의 제일 마지막에 추가.
    int nDataPBeam = 38+nPoint*3+5;   // Shear Stress 결과 4개 추가됨 : 34 -> 38 (from 박건태)
    if ( bAuto4P )  nDataPBeam = ( 38+nPoint*3+5 ) + 30; //73 + 30(6포인트에 대해 자동 계산되는 값)

    pbeam = new double[nDataPBeam];
    memset(pbeam, 0, sizeof(double)*nDataPBeam);

    int nDataCDEF = nPoint*2;
    cdef = new double[nDataCDEF];
    memset(cdef, 0, sizeof(double)*nDataCDEF);

    BOOL bConvertSectStiffData2BeforeI_sub = FALSE;   // 위에서 이미 한번 해줬으므로 내부에서는 skip
    Get_SectionPropertyMesh(SectD_temp, nPos, nTypeBeforeAfter, dMatlValue, pbeam, cdef, nDataCDEF, bConvertSectStiffData2BeforeI_sub, pSectDesign);

    int nIndex=-1;
    pSectStiffness->dIw = pbeam[22+nIndex];
    pSectStiffness->dw1 = pbeam[39+nIndex]; // 	pSectStiffness->dw1 = pbeam[35+nIndex];
    pSectStiffness->dw2 = pbeam[40+nIndex]; // 	pSectStiffness->dw2 = pbeam[36+nIndex];
    pSectStiffness->dw3 = pbeam[41+nIndex]; // 	pSectStiffness->dw3 = pbeam[37+nIndex];
    pSectStiffness->dw4 = pbeam[42+nIndex]; // 	pSectStiffness->dw4 = pbeam[38+nIndex];

    //int index_xz_xy = 34+nPoint+1+nIndex;
    int index_xz_xy = 38+nPoint+1+nIndex;  // Shear Stress 결과 4개 추가됨 : 34 -> 38 (from 박건태)
    pSectStiffness->dxz1 = pbeam[index_xz_xy];  index_xz_xy++;
    pSectStiffness->dxy1 = pbeam[index_xz_xy];  index_xz_xy++;
    pSectStiffness->dxz2 = pbeam[index_xz_xy];  index_xz_xy++;
    pSectStiffness->dxy2 = pbeam[index_xz_xy];  index_xz_xy++;
    pSectStiffness->dxz3 = pbeam[index_xz_xy];  index_xz_xy++;
    pSectStiffness->dxy3 = pbeam[index_xz_xy];  index_xz_xy++;
    pSectStiffness->dxz4 = pbeam[index_xz_xy];  index_xz_xy++;
    pSectStiffness->dxy4 = pbeam[index_xz_xy];  index_xz_xy++;

    // stress point가 4보다 큰 경우의 추가 정보.
    T_SECT_ADDI_STIFF_SUB addiStiffSub;
    for ( int k=0; k<nPoint-4; ++k )
    {
        addiStiffSub.Initialize();
        addiStiffSub.dw  = pbeam[42+( k+1 )+nIndex];              // Warping Function
        addiStiffSub.dxz = pbeam[index_xz_xy];  index_xz_xy++;  // Torsional Shear Stress Coeff.(point*2)
        addiStiffSub.dxy = pbeam[index_xz_xy];  index_xz_xy++;

        raddiStiff.aAddiStiff.Add(addiStiffSub);
    }
    if ( bAuto4P )
    { // 자동계산된 추가 6포인트에 대해
        for ( int k=0; k<6; ++k )
        {
            addiStiffSub.Initialize();
            addiStiffSub.dw  = pbeam[85+k];  // Warping Function
            addiStiffSub.dxz = pbeam[91+k+0];  // Torsional Shear Stress Coeff.(point*2)
            addiStiffSub.dxy = pbeam[91+k+1];
            addiStiffSub.dz  = pbeam[73+k+0];
            addiStiffSub.dy  = pbeam[73+k+1];
            raddiStiff.aAddiStiff.Add(addiStiffSub);
        }
        // raddiStiff.aAddiStiff개수가 12가 되어야 함.
    }

    pSectStiffness->dGeoParam1 = pbeam[38+nPoint*3+1+nIndex]; // 	pSectStiffness->dGeoParam1 = pbeam[34+nPoint*3+1+nIndex];
    pSectStiffness->dGeoParam2 = pbeam[38+nPoint*3+2+nIndex]; // 	pSectStiffness->dGeoParam2 = pbeam[34+nPoint*3+2+nIndex];
    pSectStiffness->dGeoParam3 = pbeam[38+nPoint*3+3+nIndex]; // 	pSectStiffness->dGeoParam3 = pbeam[34+nPoint*3+3+nIndex];
    pSectStiffness->dGeoParam4 = pbeam[38+nPoint*3+4+nIndex];	//  pSectStiffness->dGeoParam4 = pbeam[34+nPoint*3+4+nIndex];
    pSectStiffness->dYcz = pbeam[18+nIndex];
    pSectStiffness->dYcy = pbeam[19+nIndex];
    pSectStiffness->dYsz = pbeam[20+nIndex] + pSectStiffness->dYcz;
    pSectStiffness->dYsy = pbeam[21+nIndex] + pSectStiffness->dYcy;

    if ( bAllStiffness )
    {
        pSectStiffness->Area = pbeam[1+nIndex];
        pSectStiffness->Asz = pSectStiffness->Area*pbeam[15+nIndex];
        pSectStiffness->Asy = pSectStiffness->Area*pbeam[16+nIndex];
        pSectStiffness->Rxx = pbeam[5+nIndex];
        pSectStiffness->Ryy = pbeam[2+nIndex];
        pSectStiffness->Rzz = pbeam[3+nIndex];

        //     // stress point (응력 포인트). solver에서는 y,z가 바뀌어 있으므로 인덱스에 주의.
        //     pSectStiffness->dy1 =      cdef[0];
        //     pSectStiffness->dx1 = (-1)*cdef[1];   // 솔버에 성능점 넘길때 곱한 (-1) 값 보정
        //     pSectStiffness->dy2 =      cdef[2];
        //     pSectStiffness->dx2 = (-1)*cdef[3];   // 솔버에 성능점 넘길때 곱한 (-1) 값 보정
        //     pSectStiffness->dy3 =      cdef[4];
        //     pSectStiffness->dx3 = (-1)*cdef[5];   // 솔버에 성능점 넘길때 곱한 (-1) 값 보정
        //     pSectStiffness->dy4 =      cdef[6];
        //     pSectStiffness->dx4 = (-1)*cdef[7];   // 솔버에 성능점 넘길때 곱한 (-1) 값 보정
        pSectStiffness->dy1 = cdef[0];
        pSectStiffness->dx1 = cdef[1];
        pSectStiffness->dy2 = cdef[2];
        pSectStiffness->dx2 = cdef[3];
        pSectStiffness->dy3 = cdef[4];
        pSectStiffness->dx3 = cdef[5];
        pSectStiffness->dy4 = cdef[6];
        pSectStiffness->dx4 = cdef[7];

		// Test Code
		pSectStiffness->Qyb = sqrt(pow(pbeam[35+nIndex],2)+pow(pbeam[36+nIndex],2)) * pSectStiffness->Ryy;
		pSectStiffness->Qzb = sqrt(pow(pbeam[37+nIndex],2)+pow(pbeam[38+nIndex],2)) * pSectStiffness->Rzz;
//         pSectDesign->YBar = pbeam[17 + nIndex];
//         pSectDesign->ZBar = pbeam[18 + nIndex];

        BOOL bShowPBeamOut = FALSE;
        if ( CTestEnvMgr::IsTestEnvST(_T("7thDOFSectShowPBeamOut"), _T("yes")) ) bShowPBeamOut = TRUE;
        if ( bShowPBeamOut )
        {
            CString strTemp;
            strTemp.Format(_T("Area:%g,\n Asy:%g,\n Asz:%g,\n Rxx:%g\n Ryy:%g,\n Rzz:%g,\n "),
                pSectStiffness->Area, pSectStiffness->Asy, pSectStiffness->Asz, pSectStiffness->Rxx, pSectStiffness->Ryy, pSectStiffness->Ryy);
            AfxMessageBox(strTemp);
        }
    }

    pSectStiffness->dIp = pSectStiffness->Ryy+pSectStiffness->Rzz;

    //--------------------------------------------------
    // 계산 결과를 원래 Position(i or j) 위치 값으로 치환
    // ConvertSectStiffData2BeforeI()와 커플로 호출할 것...!!
    if ( bReturnBySectBeforeI ) rData.SectBefore.SectI = SectD_temp.SectBefore.SectI;
    else                      ConvertSectBeforeI2StiffData(rData, SectD_temp, nPos, nTypeBeforeAfter);
    //--------------------------------------------------

    delete[] pbeam;  pbeam=NULL;
    delete[] cdef;   cdef=NULL;

    return TRUE;
}

BOOL CDb_DllBaseManager::Get_SectRegularWarpingFuncion(T_SECT_SECTBASE_D& rSectData, CString strShape, double dElaMod, double dNu/*=0.0*/)
{
    HINSTANCE hDllCalc = GetOrLoad_PBeamLMeshDllHandle();
    if ( hDllCalc == NULL ) return FALSE;

    double Size[6] = { 0., };

    int nDim = 4;
    int nType = 0;
    if ( strShape == D_SECT_SHAPE_REG_L )
    {
        nType = 23; nDim = 4;
        dElaMod = 1.0;
        Size[0] = rSectData.Size[1];
        Size[1] = rSectData.Size[0];
        Size[2] = rSectData.Size[2];
        Size[3] = rSectData.Size[3];
    }
    else if ( strShape == D_SECT_SHAPE_REG_T )
    {
        nType = 13; nDim = 4;
        dElaMod = 1.0;
        Size[0] = rSectData.Size[1];
        Size[1] = rSectData.Size[0];
        Size[2] = rSectData.Size[3];
        Size[3] = rSectData.Size[2];
    }
    else if ( strShape == D_SECT_SHAPE_REG_B )
    {
        nType = 14; nDim = 6;
        dElaMod = 1.0;
        Size[0] = rSectData.Size[1];
        Size[1] = rSectData.Size[0];
        Size[2] = rSectData.Size[3];
        Size[3] = rSectData.Size[5];
        Size[4] = rSectData.Size[2];
        Size[5] = rSectData.Size[4];
    }
    else if ( strShape == D_SECT_SHAPE_REG_P )
    {
        nType = 18; nDim = 2;
        dElaMod = 1.0;
        Size[0] = rSectData.Size[0]*0.5;
        Size[1] = Size[0] - rSectData.Size[1];
    }
    else if ( strShape == D_SECT_SHAPE_REG_SB )
    {
        nType = 1; nDim = 2;
        dElaMod = 1.0;
        Size[0] = rSectData.Size[1];
        Size[1] = rSectData.Size[0];
    }
    else if ( strShape == D_SECT_SHAPE_REG_SR )
    {
        nType = 19; nDim = 1;
        dElaMod = 1.0;
        Size[0] = rSectData.Size[0]*0.5;
    }
    else if ( strShape == D_SECT_SHAPE_REG_SOCT )
    {
        nType = 22; nDim = 4;
        dElaMod = 1.0;
        Size[0] = rSectData.Size[2];
        Size[1] = rSectData.Size[1];
        Size[2] = rSectData.Size[0];
        Size[3] = Size[2] - 2*rSectData.Size[3];
    }
    else if ( strShape == D_SECT_SHAPE_REG_UDT )
    {
        nType = 21; nDim = 5;
        dElaMod = 1.0;
        Size[0] = rSectData.Size[1];
        Size[1] = rSectData.Size[2];
        Size[2] = rSectData.Size[0];
        Size[3] = rSectData.Size[4];
        Size[4] = rSectData.Size[3];
    }
    else
    {
        return FALSE;
    }

    typedef BOOL(BSPMAINGENCIVIL)( double*, double*, int*, double*, double*, int*, int*, int* );
    BSPMAINGENCIVIL *pBSPMAIN;
    pBSPMAIN = ( BSPMAINGENCIVIL* ) GetProcAddress(hDllCalc, "BSPMAINGENCIVIL");

    double pbeam[54] = { 0.0, };
    int nForm=1;
    int nOptn=2;
    int nPid=0;

    CArray<double, double> aSectDim; aSectDim.RemoveAll();
    for ( int i=0; i<nDim; i++ )
    {
        aSectDim.Add(Size[i]);
    }
    ( *pBSPMAIN )( pbeam, aSectDim.GetData(), &nType, &dElaMod, &dNu, &nForm, &nOptn, &nPid );

    int nIndex=-1;
    rSectData.Stiffness.dIw = pbeam[22+nIndex];
    rSectData.Stiffness.dw1 = pbeam[39+nIndex];  // 	rSectData.Stiffness.dw1 = pbeam[35+nIndex];
    rSectData.Stiffness.dw2 = pbeam[40+nIndex];  // 	rSectData.Stiffness.dw2 = pbeam[36+nIndex];
    rSectData.Stiffness.dw3 = pbeam[41+nIndex];  // 	rSectData.Stiffness.dw3 = pbeam[37+nIndex];
    rSectData.Stiffness.dw4 = pbeam[42+nIndex];  // 	rSectData.Stiffness.dw4 = pbeam[38+nIndex];
    rSectData.Stiffness.dxy1 = pbeam[43+nIndex];  // 	rSectData.Stiffness.dxy1 = pbeam[39+nIndex];
    rSectData.Stiffness.dxz1 = pbeam[44+nIndex];  //   rSectData.Stiffness.dxz1 = pbeam[40+nIndex];
    rSectData.Stiffness.dxy2 = pbeam[45+nIndex];  // 	rSectData.Stiffness.dxy2 = pbeam[41+nIndex];
    rSectData.Stiffness.dxz2 = pbeam[46+nIndex];  //   rSectData.Stiffness.dxz2 = pbeam[42+nIndex];
    rSectData.Stiffness.dxy3 = pbeam[47+nIndex];  // 	rSectData.Stiffness.dxy3 = pbeam[43+nIndex];
    rSectData.Stiffness.dxz3 = pbeam[48+nIndex];  //   rSectData.Stiffness.dxz3 = pbeam[44+nIndex];
    rSectData.Stiffness.dxy4 = pbeam[49+nIndex];  // 	rSectData.Stiffness.dxy4 = pbeam[45+nIndex];
    rSectData.Stiffness.dxz4 = pbeam[50+nIndex];  //   rSectData.Stiffness.dxz4 = pbeam[46+nIndex];
    rSectData.Stiffness.dGeoParam1 = pbeam[51+nIndex];  // 	rSectData.Stiffness.dGeoParam1 = pbeam[47+nIndex];
    rSectData.Stiffness.dGeoParam2 = pbeam[52+nIndex];  // 	rSectData.Stiffness.dGeoParam2 = pbeam[48+nIndex];
    rSectData.Stiffness.dGeoParam3 = pbeam[53+nIndex];  // 	rSectData.Stiffness.dGeoParam3 = pbeam[49+nIndex];
    rSectData.Stiffness.dGeoParam4 = pbeam[54+nIndex];  //   rSectData.Stiffness.dGeoParam4 = pbeam[50+nIndex];
    rSectData.Stiffness.dYcz = pbeam[18+nIndex];
    rSectData.Stiffness.dYcy = pbeam[19+nIndex];
    rSectData.Stiffness.dYsz = pbeam[20+nIndex] - rSectData.Stiffness.dYcz;
    rSectData.Stiffness.dYsy = pbeam[21+nIndex] - rSectData.Stiffness.dYcy;

    rSectData.Stiffness.dIp = rSectData.Stiffness.Ryy+rSectData.Stiffness.Rzz;

    return TRUE;
}

BOOL CDb_DllBaseManager::IsSect_PSC(const T_SECT_D& SectD)
{
    if ( SectD.nStype==D_SECT_TYPE_PSC )  return TRUE;
    if ( SectD.nStype==D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype==D_SECT_TYPE_PSC ) return TRUE;

    return FALSE;
}

// 창호 위저드에서 WindLoad 계산시 사용
BOOL CDb_DllBaseManager::Get_WindLoad4WindowFrame(T_GLOAD_KR_WIND& rData, const CString &csWindCode, double& dResult1, double& dReault2)
{
    dResult1 = dReault2 = 0.0;

    DGNKR_CLAD_WIND WindInp;
    DGNKR_CLAD_WIND_RES WindRes;
    WindInp.csZone = rData.csZone;
    WindInp.csCity = rData.csCity;
    WindInp.dV0    = rData.dV0;
    WindInp.dKzt   = rData.dKzt;
    WindInp.csIw   = rData.csIw;
    WindInp.csSRC  = rData.csSRC;
    WindInp.dZH    = rData.dZH;
    WindInp.dz     = rData.dz;
    WindInp.dTribArea = rData.dTribArea;
    WindInp.dRoofAngle = rData.dRoofAngle;
    WindInp.nBuildType = rData.nBuildType;
    WindInp.nChkPoint  = rData.nChkPoint;
    WindInp.nRoofType  = rData.nRoofType;
    WindInp.bWall      = rData.bWall;

    if ( WindInp.csIw == _T("특") )
        WindInp.csIw = _T("S");

    HINSTANCE hDllCalc;
    CString strCalcDll = Get_CalcDllFileName(DGNENGINE_KR_DLL);
    if ( ( hDllCalc=LoadLibrary(strCalcDll) ) == NULL )
    {
        CString strMsg = _T("Can't Find ") + strCalcDll;
        AfxMessageBox(strMsg);
        return FALSE;
    }

    int nUnitL = D_UNITSYS_LENGTH_INDEX_MM; int nUnitF = D_UNITSYS_FORCE_INDEX_N;
    CDBDoc    *pDoc  = CDBDoc::GetDocPoint(); ASSERT(pDoc);
    CUnitCtrl *pUnit = pDoc->m_pUnitCtrl;   ASSERT(pUnit);

    WindInp.dV0       = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(nUnitF, nUnitL, D_UNITSYS_BASE_VELOCITY, WindInp.dV0);
    WindInp.dZH       = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(nUnitF, nUnitL, D_UNITSYS_BASE_LENGTH, WindInp.dZH);
    WindInp.dz        = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(nUnitF, nUnitL, D_UNITSYS_BASE_LENGTH, WindInp.dz);
    WindInp.dTribArea = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(nUnitF, nUnitL, D_UNITSYS_BASE_AREA, WindInp.dTribArea);

    UINT nWindCode = DGN_KBC2009;
    if ( csWindCode.CompareNoCase(_T("KBC(2009)")) ==0 ) nWindCode = DGN_KBC2009;
    else if ( csWindCode.CompareNoCase(_T("KBC(2015)")) ==0 ) nWindCode = DGN_KBC2016;
    else ASSERT(0);

    //==================================================================================================
    // Explicit Linkage for DLL
    typedef BOOL(PWIND_LOAD_CALC)( void*, void*, size_t, size_t, UINT, BOOL& );
    PWIND_LOAD_CALC *pWindCalc;
    pWindCalc = ( PWIND_LOAD_CALC* ) GetProcAddress(hDllCalc, "CalcWindLoad");
    //==================================================================================================
    size_t TSizeInp = sizeof(DGNKR_CLAD_WIND);
    size_t TSizeOut = sizeof(DGNKR_CLAD_WIND_RES);

    BOOL bVersionOK = FALSE;
    ( *pWindCalc )( &WindInp, &WindRes, TSizeInp, TSizeOut, nWindCode, bVersionOK );

    dResult1 = pDoc->m_pUnitCtrl->ConvValueTgtUnit2CurUnit(nUnitF, nUnitL, D_UNITSYS_BASE_STRESS, WindRes.pc.dP);
    dReault2 = pDoc->m_pUnitCtrl->ConvValueTgtUnit2CurUnit(nUnitF, nUnitL, D_UNITSYS_BASE_STRESS, WindRes.pc.dN);

    if ( !bVersionOK ) AfxMessageBox(_T("Calculation of Wind Load is failed."));

    return TRUE;
}

BOOL CDb_DllBaseManager::GetSectMesh(T_SECT_D& SectD, BOOL bAdjustCoordToCentroid,
									 CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD,
									 CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD)
{
	aMeshElemD.RemoveAll();
	aMeshNodeD.RemoveAll();
	std::vector<int> aDomainElemCount;
	std::vector<int> aFilter;
	CArray<int, int> aDomainMaterialType;              // 0: RC, 1: Steel, -1: Hole  // SRC/composite의 경우 Domain별 재질 타입
	CSimpleMesh simpleMesh;
	simpleMesh.m_bUseMaxSectMesh = m_bUseMaxSectMesh;

	double dMatl_Elast_back = SectD.SectBefore.Matl_Elast;
	if (!simpleMesh.GetSectMesh(SectD, 0, 0, aMeshElemD, aMeshNodeD, aDomainElemCount, aDomainMaterialType, FALSE, NULL))
	{
		return FALSE;
	}

	return TRUE;
}