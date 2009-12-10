/**
*   \file HashCalc.h
*   \brief hash����ģ����ʵ�֣�header only
*   \attention Copyright (C) 2007 LLYF Software
*   \author YangFan
*   \date 11/17/2007 12:22:40 AM
*/

#ifndef HASHCALC_H_
#define HASHCALC_H_

#include <Windows.h>

#include "HashMacro.h"
#include "md2.h"
#include "md4.h"
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include "sha512.h"
#include "rmd128.h"
#include "rmd160.h"
#include "rmd256.h"
#include "rmd320.h"
#include "tiger.h"
#include "whirl.h"
#include "haval3.h"
#include "haval4.h"
#include "haval5.h"
#include "crc.h"
#include "sizehash.h"
#include "fcs.h"
#include "ghash.h"
#include "gost.h"
#include "adler32.h"

/**
*   \class CHash
*   \brief hash����ģ���࣬�ô��ǿ��Թ�����ͬ�߼��Ĵ��룬�����ڴ�ӳ���ļ���ߴ��ļ��ļ���Ч�ʵ�
*   \author YangFan
*   \date 11/17/2007 12:23:15 AM
*/
template<class CAlgorithm>
class CHash
{
private:
	CAlgorithm m_algo;
public:
	/**
	*   \brief ��ʼ��������
	*   \return int һ�㲻�ù���
	*   \author YangFan
	*   \date 11/17/2007 12:27:56 AM
	*/
	int Init()
	{
		return m_algo.Init();
	}

	/**
	*   \brief ������Ҫ���������
	*   \param[in] unsigned char * pData ��Ҫ���������
	*   \param[in] unsigned long ulLen ��Ҫ��������ݳ���
	*   \return  int һ�㲻�ù���
	*   \author YangFan
	*   \date 11/17/2007 12:27:59 AM
	*/
	int Update(unsigned char * pData, unsigned long ulLen)
	{
		return m_algo.Update(pData, ulLen);
	}

	/**
	*   \brief �õ�hash���
	*   \param[out] unsigned char * pszOut
	*   \return  int һ�㲻�ù���
	*   \author YangFan
	*   \date 11/17/2007 12:28:01 AM
	*/
	int Final(unsigned char * pszOut)
	{
		return m_algo.Final(pszOut);
	}

	/**
	*   \brief ��ȡ��ϢժҪ���ȣ����ֽ�Ϊ��λ
	*   \return int ��ϢժҪ���ȣ����ֽ�Ϊ��λ
	*   \author YangFan
	*   \date 11/17/2007 12:28:04 AM
	*/
	int GetDigestLength() const
	{
		return CAlgorithm::nDigestLength;
	}

#if defined(_WIN32) || defined(WIN32)
	/**
	*   \brief �����ļ���hashֵ��ʹ��Windowsƽ̨�ṩ���ڴ�ӳ���ļ���ߴ��ļ��Ķ�ȡЧ��
	*   \param[in] LPCTSTR pszFileName
	*   \param[out] unsigned char * pszOut
	*   \return ��
	*   \author YangFan
	*   \date 11/17/2007 12:28:06 AM
	*/
	void HashFile(const char * pszFileName, unsigned char * pszOut)
	{
		SYSTEM_INFO sinf;

		/**
		* ��ȡ��ǰϵͳ���ڴ��ҳ��С
		*/
		::GetSystemInfo(&sinf);

		/**
		* ���ļ�
		*/
		HANDLE hFile = ::CreateFileA(pszFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);

		if( hFile == INVALID_HANDLE_VALUE)
			return;

		/**
		* �����ڴ�ӳ���ļ�
		*/
		HANDLE hFileMapping = ::CreateFileMapping(hFile,
			NULL,
			PAGE_READONLY,
			0,
			0,
			NULL);
	  if( hFileMapping == INVALID_HANDLE_VALUE)
	  	{ 
	  		CloseHandle( hFile );
	  		return;
	  	}
		DWORD dwFileSizeHigh;

		/**
		* ��ȡ�ļ���С
		*/
		__int64 qwFileSize = ::GetFileSize(hFile, &dwFileSizeHigh);
		qwFileSize += (((__int64) dwFileSizeHigh) <<32);

		::CloseHandle(hFile);
		m_algo.Init();

		__int64 qwFileOffset = 0;


		/**
		* ѭ��ÿ��ӳ��һҳ��С�����ݣ�����hash����
		*/
		while(qwFileSize > 0)
		{
			DWORD dwBytesInBlock = sinf.dwAllocationGranularity;
			if(qwFileSize < sinf.dwAllocationGranularity)
				dwBytesInBlock = (DWORD)qwFileSize;
			PBYTE pbFile = (PBYTE)::MapViewOfFile(hFileMapping,
				FILE_MAP_READ,
				(DWORD)(qwFileOffset>>32),
				(DWORD)(qwFileOffset&0xffffffff),
				dwBytesInBlock);
			m_algo.Update(pbFile,dwBytesInBlock);
			
			::UnmapViewOfFile(pbFile);

			qwFileOffset += dwBytesInBlock;
			qwFileSize -= dwBytesInBlock;	
		}
		m_algo.Final(pszOut);
		::CloseHandle(hFileMapping);
		
	}
#endif
	/**
	*   \brief �����ַ�����hash���
	*   \param[in] unsigned char* pszText Ҫ������hash���ַ���
	*   \param[in] unsigned long ulLen Ҫ��������ַ�������
	*   \param[out] unsigned char * pszOut ������hash���
	*   \return ��
	*   \author YangFan
	*   \date 11/17/2007 12:28:10 AM
	*/
	void HashText(unsigned char* pszText, unsigned long ulLen, unsigned char * pszOut)
	{
		m_algo.Init();
		m_algo.Update(pszText, ulLen);
		m_algo.Final(pszOut);
	}
};


#endif