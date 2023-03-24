#ifndef __PE_INFO__H__
#define __PE_INFO__H__
#include <vector>
#include <string>
#include <iostream>
#include<iomanip>
#include <unordered_map>
#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif __linux__
    #include "PELinux.h"
#endif



class ImportHintName {
public:
	WORD wHint;
	std::string strName;
	explicit ImportHintName(const IMAGE_IMPORT_BY_NAME &imgImportByName):wHint(imgImportByName.Hint), strName(imgImportByName.Name){
		
	}
	void showInfo() {
		std::cout << "wHint: " << wHint << ", strName: " << strName << std::endl;
	}
};

class IMAGE_IMPORT_DESCRIPTOR_Wrapper {
public:
	IMAGE_IMPORT_DESCRIPTOR m_imgImportDesc;
	std::string m_strDllName;
	explicit IMAGE_IMPORT_DESCRIPTOR_Wrapper(const IMAGE_IMPORT_DESCRIPTOR &imgImportDesc, const std::string &dllName):m_imgImportDesc(imgImportDesc), m_strDllName(dllName) {

	}
	void showInfo() {
		std::cout << "strDllName: [" << m_strDllName << "]"  << std::endl;
	}
};

class IMAGE_EXPORT_INFO_WRAPPER {
public:
	IMAGE_EXPORT_DIRECTORY m_imgExportDirectory;
	std::string m_strDllName;
	void  SetImgExportInfoWrapper(const IMAGE_EXPORT_DIRECTORY &imgExportDirectory, const std::string &dllName, 
		 const std::vector<std::string> &vecFuncName,
		 const std::vector<DWORD> &vecFuncAddr,
		 const std::vector<WORD> &vecFuncNameOrdinal
		)   {
		m_imgExportDirectory = imgExportDirectory;
		m_strDllName = dllName;
		m_vecFuncName = vecFuncName;
		m_vecFuncAddr = vecFuncAddr;
		m_vecFuncNameOrdinal = vecFuncNameOrdinal;
	}
	void showInfo() {
		std::cout << "strDllName: [" << m_strDllName << "]" << std::endl;
        std::cout << "vecFuncName(" << m_vecFuncName.size() <<  ")"<< std::endl;
        for (const auto &funcName : m_vecFuncName) {
            std::cout << funcName << std::endl;
        }
        std::cout << "vecFuncNameOrdinal(" << m_vecFuncNameOrdinal.size() << ")" << std::endl;
        for (const auto &funcNameOrdinal : m_vecFuncNameOrdinal) {
            std::cout << std::hex << std::setfill('0') << std::setw(4) << funcNameOrdinal << std::endl;
        }
        std::cout << "vecFuncAddr(" << m_vecFuncAddr.size() << ")" << std::endl;
        for (const auto &funcAddr : m_vecFuncAddr) {
            std::cout << std::hex << std::setfill('0') << std::setw(8) << funcAddr <<  std::endl;
        }
	}
    DWORD getFuncByName(const std::string &strFuncName) {
        if (m_imgExportDirectory.NumberOfNames == 0) {
            return 0;
        }
        DWORD nameIdx = 0;
        for (; nameIdx < m_vecFuncName.size(); ++nameIdx) {
            if (m_vecFuncName[nameIdx] == strFuncName) {
                break;
            }
        }
        if (nameIdx >= m_vecFuncName.size()) {
            return 0;
        }
        if (nameIdx >= m_vecFuncNameOrdinal.size()) {
            return 0;
        }
        WORD dwFuncNameOrdinal = m_vecFuncNameOrdinal[nameIdx];
        std::cout << "nameIdx = " << std::hex << std::setfill('0') << std::setw(8) << nameIdx << std::endl;
        std::cout << "dwFuncNameOrdinal = " << std::hex << std::setfill('0') << std::setw(8) << dwFuncNameOrdinal << std::endl;
        if (dwFuncNameOrdinal >= m_vecFuncAddr.size()) {
            if (dwFuncNameOrdinal >= m_imgExportDirectory.Base) {
                DWORD dwFuncAddr = dwFuncNameOrdinal - m_imgExportDirectory.Base;
                std::cout << "index = dwFuncNameOrdinal - Base = " << std::hex << std::setfill('0') << std::setw(8) << dwFuncAddr << std::endl;
                if (dwFuncAddr >= m_vecFuncAddr.size()) {
                    return 0;
                }
                return m_vecFuncAddr[dwFuncAddr];
            }
            else {
                return 0;
            }
        }
        else {            
            return m_vecFuncAddr[dwFuncNameOrdinal];
        } 
    }
private:
	std::vector<std::string> m_vecFuncName;
	std::vector<DWORD> m_vecFuncAddr;
	std::vector<WORD> m_vecFuncNameOrdinal;
};



class CPEInfo {
public:
	CPEInfo();
	~CPEInfo();
    void CloseMapViewAndFiles();
	bool LoadPE(LPCTSTR pFileName);
    bool loaded() {return m_bLoaded;}
    bool readPEInfoFromMapView(void* pMapView);
	void show();
	void loadImportDataDirectory();
	void loadIATDataDirectory();
	void loadEATDataDirectory();
    void testGetFuncByName(const std::string &strFuncName);
	DWORD rvaToFoa(DWORD rva);
	DWORD foaToRva(DWORD foa);
    DWORD getCoffHeaderSize();
	union {
		IMAGE_NT_HEADERS32 x86;
		IMAGE_NT_HEADERS64 x64;
	} m_peHeader;
    DWORD getEntryPointRVA() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.AddressOfEntryPoint : m_peHeader.x64.OptionalHeader.AddressOfEntryPoint;
    }
    DWORD getImageBaseX86() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.ImageBase : 0;
    }
    ULONGLONG getImageBaseX64() {
        return  m_bX86 ? 0 : m_peHeader.x64.OptionalHeader.ImageBase;
    }
    DWORD getSizeOfImage() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.SizeOfImage : m_peHeader.x64.OptionalHeader.SizeOfImage;
    }
    DWORD getSectionAlignment() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.SectionAlignment : m_peHeader.x64.OptionalHeader.SectionAlignment;
    }
    DWORD getFileAlignment() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.FileAlignment : m_peHeader.x64.OptionalHeader.FileAlignment;
    }
    WORD getNumberOfSections() {
        return m_bX86 ? m_peHeader.x86.FileHeader.NumberOfSections : m_peHeader.x64.FileHeader.NumberOfSections;
    }
    WORD getCharacteristics() {
        return m_bX86 ? m_peHeader.x86.FileHeader.Characteristics : m_peHeader.x64.FileHeader.Characteristics;
    }
    DWORD getCheckSum() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.CheckSum : m_peHeader.x64.OptionalHeader.CheckSum;
    }
    WORD getMachine() {
        return m_bX86 ? m_peHeader.x86.FileHeader.Machine : m_peHeader.x64.FileHeader.Machine;
    }
    WORD getSubSystem() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.Subsystem : m_peHeader.x64.OptionalHeader.Subsystem;
    }
    WORD getDllCharacteristics() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.DllCharacteristics : m_peHeader.x64.OptionalHeader.DllCharacteristics;
    }
    DWORD getDataDirectoryElemCnt() {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.NumberOfRvaAndSizes : m_peHeader.x64.OptionalHeader.NumberOfRvaAndSizes;
    }
    IMAGE_DATA_DIRECTORY * getDataDirectory(DWORD index);

private:
	void printPEHeaderX86();
	void printPEHeaderX64();
	void printImgSecHeader();
	void printDosHeader();
	void printField(const char * field_name, const unsigned char* field_bytes, unsigned int field_length);
    void printSectionHeaderInfo(unsigned int section_idx, const IMAGE_SECTION_HEADER& image_sec_header);
	void valid_signature(unsigned char signature[]);
	void valid_magic(unsigned char magic[]);
	void loadOrigThunkDetail(DWORD origFirstThunk);
	void loadFirstThunkDetail(DWORD firstThunk);
	std::string getStrFromRva(DWORD dwRva);
	void getDWORDVecFromStartRva(DWORD dwRva, std::vector<DWORD> &vecDwValue, DWORD dwValueNum = 0);
    void getWORDVecFromStartRva(DWORD dwRva, std::vector<WORD> &vecDwValue, DWORD dwValueNum = 0);

private:
	std::vector<IMAGE_SECTION_HEADER> m_vecImgSecHeader;
	std::vector<IMAGE_IMPORT_DESCRIPTOR_Wrapper> m_vecImgImportDesc;
	std::unordered_map<DWORD, std::vector<ImportHintName>> m_mapOrigThunk;
	std::unordered_map<DWORD, std::vector<DWORD>> m_mapImportIAT;
	std::unordered_map<std::string, std::vector<DWORD>> m_mapEAT;
	WORD m_sectionNum;
    DWORD m_dwSizeofImageNTHeaders;
	IMAGE_DOS_HEADER m_dosHeader;
	bool m_bLoaded;
	LPVOID m_pMapViewBase;
#ifdef WIN32
	HANDLE m_hFile;
#elif __linux__
    int m_fd;
#endif
    unsigned long long m_dwFileSize;
	IMAGE_EXPORT_INFO_WRAPPER m_imgExportInfoWrapper;
public:
    bool m_bX86;
};


#endif
