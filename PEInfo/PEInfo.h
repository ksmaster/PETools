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
    #include <tchar.h>
#elif __linux__
    #include "PELinux.h"
#endif



class ImportHintName {
public:
	WORD wHint;
	std::string strName;
	explicit ImportHintName(const IMAGE_IMPORT_BY_NAME &imgImportByName):wHint(imgImportByName.Hint), strName(imgImportByName.Name){
		
	}
    explicit ImportHintName()
    {
        wHint = 0;
        strName = "";
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

class DLL_INFO {
public:
    std::string m_strDllName;
    std::vector<std::string> m_vecFuncName;
    std::vector<DWORD> m_vecFuncAddr;
    std::vector<WORD> m_vecFuncNameOrdinal;
    DWORD m_dwBase;
public:
    void init()
    {
        m_vecFuncName.clear();
        m_vecFuncAddr.clear();
        m_vecFuncNameOrdinal.clear();
    }
    void  setInfo( const std::string& dllName,
        DWORD dwBase,
        const std::vector<std::string>& vecFuncName,
        const std::vector<DWORD>& vecFuncAddr,
        const std::vector<WORD>& vecFuncNameOrdinal
    )
    {
        m_dwBase = dwBase;
        m_strDllName = dllName;
        m_vecFuncName = vecFuncName;
        m_vecFuncAddr = vecFuncAddr;
        m_vecFuncNameOrdinal = vecFuncNameOrdinal;
    }
    void showInfo() {
        std::cout << "strDllName: [" << m_strDllName << "]" << std::endl;
        std::cout << "vecFuncName(" << m_vecFuncName.size() << ")" << std::endl;
        for (const auto& funcName : m_vecFuncName) {
            std::cout << funcName << std::endl;
        }
        std::cout << "vecFuncNameOrdinal(" << m_vecFuncNameOrdinal.size() << ")" << std::endl;
        for (const auto& funcNameOrdinal : m_vecFuncNameOrdinal) {
            std::cout << std::hex << std::setfill('0') << std::setw(4) << funcNameOrdinal << std::endl;
        }
        std::cout << "vecFuncAddr(" << m_vecFuncAddr.size() << ")" << std::endl;
        for (const auto& funcAddr : m_vecFuncAddr) {
            std::cout << std::hex << std::setfill('0') << std::setw(8) << funcAddr << std::endl;
        }
    }
    bool getFuncByName(const std::string& strFuncName, WORD& wFuncNameOrdinal, DWORD& dwFuncAddr) const {
        DWORD nameIdx = 0;
        for (; nameIdx < m_vecFuncName.size(); ++nameIdx) {
            if (m_vecFuncName[nameIdx] == strFuncName) {
                break;
            }
        }
        if (nameIdx >= m_vecFuncName.size()) {
            return false;
        }
        if (nameIdx >= m_vecFuncNameOrdinal.size()) {
            return false;
        }
        WORD wFuncNameOrdinalHint = m_vecFuncNameOrdinal[nameIdx];
        //std::cout << "nameIdx = " << std::hex << std::setfill('0') << std::setw(8) << nameIdx << std::endl;
        //std::cout << "dwFuncNameOrdinal = " << std::hex << std::setfill('0') << std::setw(8) << wFuncNameOrdinal << std::endl;
        if (wFuncNameOrdinalHint >= m_vecFuncAddr.size()) {
            if (wFuncNameOrdinalHint >= m_dwBase) {
                DWORD dwFuncAddrIdx = wFuncNameOrdinalHint - m_dwBase;
                std::cout << "index = dwFuncNameOrdinal - Base = " << std::hex << std::setfill('0') << std::setw(8) << dwFuncAddrIdx << std::endl;
                if (dwFuncAddrIdx >= m_vecFuncAddr.size()) {
                    return false;
                }
                dwFuncAddr = m_vecFuncAddr[dwFuncAddrIdx];
                wFuncNameOrdinal = wFuncNameOrdinalHint + (WORD)m_dwBase;
                return true;
            }
            return false;
        }
        else {
            dwFuncAddr = m_vecFuncAddr[wFuncNameOrdinalHint];
            wFuncNameOrdinal = wFuncNameOrdinalHint + (WORD)m_dwBase;
            return true;
        }
    }
};

class IMAGE_EXPORT_INFO_WRAPPER {
public:
	IMAGE_EXPORT_DIRECTORY m_imgExportDirectory;
    DLL_INFO dllInfo;
    
    bool m_bLoaded=false;
    void init()
    {
        m_bLoaded = false;
        dllInfo.init();
    }
	void  SetImgExportInfoWrapper(const IMAGE_EXPORT_DIRECTORY &imgExportDirectory, const std::string &dllName, 
		 const std::vector<std::string> &vecFuncName,
		 const std::vector<DWORD> &vecFuncAddr,
		 const std::vector<WORD> &vecFuncNameOrdinal
		)   {
		m_imgExportDirectory = imgExportDirectory;
        dllInfo.setInfo(dllName, m_imgExportDirectory.Base, vecFuncName, vecFuncAddr, vecFuncNameOrdinal);
        m_bLoaded = true;
	}
	void showInfo() {
        dllInfo.showInfo();
	}
    bool getFuncByName(const std::string &strFuncName, WORD &wFuncNameOrdinal, DWORD &dwFuncAddr) const {
        if (m_imgExportDirectory.NumberOfNames == 0) {
            return false;
        }
        return dllInfo.getFuncByName(strFuncName, wFuncNameOrdinal, dwFuncAddr);
    }
};


class ImageImportNameInfoWrapper
{
public:
    DWORD m_orgFirstThunk = 0;
    DWORD m_firstThunk = 0;
    DWORD m_firstThunkFOA = 0;
    std::vector<ULONGLONG> m_vecILT;
    std::vector<ULONGLONG> m_vecIAT;

    std::vector<DWORD> m_vecOrdinal;
    
    std::vector<ImportHintName> m_vecHintNameTable;
    void init()
    {
        m_orgFirstThunk = 0;
        m_firstThunk = 0;
        m_firstThunkFOA = 0;
        m_vecILT.clear();
        m_vecIAT.clear();
        m_vecOrdinal.clear();
        m_vecHintNameTable.clear();
    }
    int size() const {
        return m_vecHintNameTable.size();
    }
    void setILT(DWORD orgFirstThunk, const std::vector<ULONGLONG>& vecILT)
    {
        m_orgFirstThunk = orgFirstThunk;
        m_vecILT = vecILT;
    }
    void setILT(DWORD orgFirstThunk, const std::vector<DWORD>& vecILT)
    {
        m_orgFirstThunk = orgFirstThunk;
        for (auto& trunk : vecILT)
        {
            m_vecILT.emplace_back(trunk);
        }
    }
    void setIAT(DWORD firstThunk, DWORD firstThunkFOA, const std::vector<DWORD>& vecIAT) {
        m_firstThunk = firstThunk;
        m_firstThunkFOA = firstThunkFOA;
        for (auto& trunk : vecIAT)
        {
            m_vecIAT.emplace_back(trunk);
        }
    }
    void setIAT(DWORD firstThunk, DWORD firstThunkFOA, const std::vector<ULONGLONG>& vecIAT) {
        m_firstThunk = firstThunk;
        m_firstThunkFOA = firstThunkFOA;
        m_vecIAT = vecIAT;
    }
    void setHintName(const std::vector<ImportHintName>& vecHintName) {
        m_vecHintNameTable = vecHintName;
    }
};



class CPEInfo {
public:
	CPEInfo();
	~CPEInfo();
    void CloseMapViewAndFiles();
#if defined(WIN32)
    bool LoadPE(LPCTSTR pFileName);
#elif __linux__
    bool LoadPE(const char* pFileName);
#endif
   
    bool loaded() {return m_bLoaded;}
    bool readPEInfoFromMapView(void* pMapView);
	void show();
	void loadImportDataDirectory();
	void loadEATDataDirectory();
    void testGetFuncByName(const std::string &strFuncName);
	DWORD rvaToFoa(DWORD rva) const;
	DWORD foaToRva(DWORD foa) const;
    DWORD getCoffHeaderSize() const;
	union {
		IMAGE_NT_HEADERS32 x86;
		IMAGE_NT_HEADERS64 x64;
	} m_peHeader;
    DWORD getEntryPointRVA() const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.AddressOfEntryPoint : m_peHeader.x64.OptionalHeader.AddressOfEntryPoint;
    }
    DWORD getImageBaseX86() const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.ImageBase : 0;
    }
    ULONGLONG getImageBaseX64() const {
        return  m_bX86 ? 0 : m_peHeader.x64.OptionalHeader.ImageBase;
    }
    DWORD getSizeOfImage()const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.SizeOfImage : m_peHeader.x64.OptionalHeader.SizeOfImage;
    }
    DWORD getSectionAlignment() const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.SectionAlignment : m_peHeader.x64.OptionalHeader.SectionAlignment;
    }
    DWORD getFileAlignment() const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.FileAlignment : m_peHeader.x64.OptionalHeader.FileAlignment;
    }
    WORD getNumberOfSections() const {
        return m_bX86 ? m_peHeader.x86.FileHeader.NumberOfSections : m_peHeader.x64.FileHeader.NumberOfSections;
    }
    WORD getCharacteristics() const {
        return m_bX86 ? m_peHeader.x86.FileHeader.Characteristics : m_peHeader.x64.FileHeader.Characteristics;
    }
    DWORD getCheckSum() const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.CheckSum : m_peHeader.x64.OptionalHeader.CheckSum;
    }
    WORD getMachine()const {
        return m_bX86 ? m_peHeader.x86.FileHeader.Machine : m_peHeader.x64.FileHeader.Machine;
    }
    WORD getSubSystem() const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.Subsystem : m_peHeader.x64.OptionalHeader.Subsystem;
    }
    WORD getDllCharacteristics()const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.DllCharacteristics : m_peHeader.x64.OptionalHeader.DllCharacteristics;
    }
    DWORD getDataDirectoryElemCnt()const {
        return m_bX86 ? m_peHeader.x86.OptionalHeader.NumberOfRvaAndSizes : m_peHeader.x64.OptionalHeader.NumberOfRvaAndSizes;
    }
    const IMAGE_DATA_DIRECTORY * getDataDirectory(DWORD index) const;
    IMAGE_DOS_HEADER getImageDosHeader()const { return m_dosHeader;   }
    const std::vector<IMAGE_SECTION_HEADER>& getImageSectionHeaders() const { return m_vecImgSecHeader; }
    IMAGE_EXPORT_INFO_WRAPPER& getImageExportInfo() { return m_imgExportInfoWrapper; }
    bool getImageImportWrapperInfo(const std::string& strDllName, ImageImportNameInfoWrapper&) const;
    const std::vector<IMAGE_IMPORT_DESCRIPTOR_Wrapper>& getImageImportInfo() const { return m_vecImgImportDesc; }
    void setStopFlag(bool bFlag) { m_bNeedStopParsing = bFlag; }
private:
	void printPEHeaderX86();
	void printPEHeaderX64();
	void printImgSecHeader();
	void printDosHeader();
	void printField(const char * field_name, const unsigned char* field_bytes, unsigned int field_length);
    void printSectionHeaderInfo(unsigned int section_idx, const IMAGE_SECTION_HEADER& image_sec_header);
	void valid_signature(unsigned char signature[]);
	void valid_magic(unsigned char magic[]);
	void loadOrdinalOrHintNameTable(const std::vector<ULONGLONG>& vecAddr, std::vector<DWORD>& vecOrdinal, std::vector<ImportHintName>& vecImportHintName);
	void loadThunkDetail(const std::string&, DWORD firstThunk, bool, ImageImportNameInfoWrapper&);
	std::string getStrFromRva(DWORD dwRva);
    void getDWORDVecFromStartRva(DWORD dwRva, std::vector<DWORD>& vecDwValue, DWORD dwValueNum = 0);
    void getULONGLONGVecFromStartRva(DWORD dwRva, std::vector<ULONGLONG> &vecullValue, DWORD dwValueNum = 0);
    void getWORDVecFromStartRva(DWORD dwRva, std::vector<WORD> &vecDwValue, DWORD dwValueNum = 0);
    
private:
	std::vector<IMAGE_SECTION_HEADER> m_vecImgSecHeader;
	std::vector<IMAGE_IMPORT_DESCRIPTOR_Wrapper> m_vecImgImportDesc;
	std::unordered_map<std::string, ImageImportNameInfoWrapper> m_mapImgImportInfoWrapper;
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
    bool m_bNeedStopParsing = false;
public:
    bool m_bX86;
};


#endif
