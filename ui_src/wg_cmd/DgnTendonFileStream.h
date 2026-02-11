#if !defined(_CFILE_STREAM_)
#define _CFILE_STREAM_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CFileStream (TEXT 파일 읽기/쓰기를 위한 STL로 만든 PARSER) // CYW++ 
//  STL로 파일을 읽어서 처리를 하면 문제가 발생한다. 

#define xWORDSIZE   255
#define xDIRSIZE    255
#define xDOUBLESIZE 7

class CTendonFileStream
{
public:
	CTendonFileStream() 
	{
		nLength = 10;
		nPrecision = 6;			
		SetComment('#'); 
	} // (stringstream& ss) : ssData(ss) { }
	~CTendonFileStream() {}

private:
	CStdioFile   file; 
	CString      szLine;
	CString      szTemp;
	TCHAR        szComment;  // 코멘트 문자 
	int          nLength;    // 문자열의 길이 
	int          nPrecision; // 소수점 자릿수 

public:

	// 1. Read Function  (******Concept : Oneline Read******)
	BOOL    ReadOpen(LPCTSTR strDir); 
	void    ReadClose();                     
	// 1.1 Read One Line('\n')
	BOOL    GetLine(TCHAR* str);							 
	BOOL    GetLine(CString& str); 					 
	BOOL    GetLine();											  
	CString GetLineString();                  
	TCHAR*  GetLineStringC();                 
	void    SetLineBack();                    
	BOOL    IsEmpty();   
	// 1.2 Etc (File Field)	
	void     SetStartPos();										
	void     SetEndPos();                      
	LONGLONG GetCurPos();											
	void     SetCurPos(long nLen); 				    
	void     SetBack(long nLen);          
	// 1.3 Get Variable (One Line Data Field(stringstream))
	CString GetString();                                 
	void    GetString(CString& str); 				
	void    GetString(TCHAR* str);		 				 
	CString GetString(int nLen);			
	void    GetString(CString& str, int nLen);
	void    GetString(TCHAR* str, int nLen);	 
	int     GetInteger();	 		  						
	void    GetInteger(int& nV);						
	double  GetDouble();										
	void    GetDouble(double& dV);					

	CString StrInLine(CString& str);
	CString StrInLine(CString& str, unsigned int nLen);

	// 주석줄인지 판단 
	void SetComment(TCHAR str) { szComment = str; }

	BOOL CheckCommentLine();
	BOOL CheckComment(); 
	BOOL CheckComment(CString& str, TCHAR szCom = '#');
	BOOL CheckComment(TCHAR* str, TCHAR szCom = '#');
	BOOL SkipCommentLine(); // 여러줄의 주석라인을 스킵한다. 

	// 숫자를 문자열로 
	void SetStrLength(int no)    { nLength = no; }
	void SetStrPrecision(int no) { nPrecision = no; }
	int  GetStrLength()          { return nLength; }
	int  GetStrPrecision()       { return nPrecision; }

	CString GetFormat(double dV);
	void    GetFormat(double dV, TCHAR* String);
	static CString GetFormat(double dV, int nPrecision);
	static CString GetFormat(double dV, int nPrecision, int nLength);
};
// ssData >> setw(nLen + 1) >> str; // 공백을 자동으로 처리한다. 

#endif  // _CFILE_STREAM_