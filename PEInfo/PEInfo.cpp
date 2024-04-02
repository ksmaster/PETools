#include <PEInfo.h>
#include <iostream>
#include<iomanip>
#include<cassert>
#include <QThread>
#include <QDebug>
using namespace std;

DWORD ALIGN(DWORD x, DWORD a)  {
   return ((x + (a - 1)) & ~(a - 1));
}



void print_prompt(unsigned char cLineBuf[], unsigned int length) {
	if (!cLineBuf) {
		return;
	}

	for (unsigned int i = 0; i < length; ++i) {
		if (cLineBuf[i] >= 0x20 && cLineBuf[i] <= 0x7e) {
			cout << cLineBuf[i];
		}
		else {
			cout << '.';
		}
	}
}


void printHexBuffer(const unsigned char *buf, unsigned int nBufLen)
{
	if (!buf)
	{
		return;
	}
	char cLineBuf[16 * 3 + 1] = { 0 };
	unsigned int nLinePos = 0;
	unsigned int i = 0;
	for (i = 0; i < nBufLen; ++i) {
		nLinePos += sprintf_s(cLineBuf + nLinePos, 4, "%02X ", buf[i]);
		if ((i + 1) % 16 == 0)
		{
			cout << cLineBuf;
			print_prompt((unsigned char*)(buf + 16 * (i / 16)), 16);
			cout << endl;
			cLineBuf[0] = '\0';
			nLinePos = 0;
		}
	}
	if (cLineBuf[0]) {
		cout << cLineBuf;
		for (unsigned j = 0; j < (16 - (i % 16)); ++j) {
			cout << "   ";
		}
		print_prompt((unsigned char*)(buf + 16 * (i / 16)), i % 16);
		cout << endl;
	}
}

void print_byte_in_Hex(unsigned char c) {
	cout << "0x" << hex << setiosflags(ios::uppercase) << setfill('0') << setw(2) << (int)c << endl;
}

void CPEInfo::valid_magic(unsigned char magic[]) {
	//cout << "valid_signature: " << signature[0] << endl;
	assert(magic[0] == 'M');
	assert(magic[1] == 'Z');
}

void CPEInfo::valid_signature(unsigned char signature[]) {
	//cout << "valid_signature: " << signature[0] << endl;
	assert(signature[0] == 'P');
	assert(signature[1] == 'E');
	assert(signature[2] == 0);
	assert(signature[3] == 0);
}

void CPEInfo::printField(const char * fieldName, const unsigned char* fieldBytes, unsigned int fieldLength) {
	const char * filed = fieldName ? fieldName : "";
	cout << filed << "(" << fieldLength << " Bytes): ";
	if (fieldLength > 15) {
		cout << endl;
	}
	printHexBuffer(fieldBytes, fieldLength);
}

void CPEInfo::printSectionHeaderInfo(unsigned int sectionIdx, const IMAGE_SECTION_HEADER& imageSecHeader) {
	char secName[IMAGE_SIZEOF_SHORT_NAME + 1] = { 0 };
	secName[IMAGE_SIZEOF_SHORT_NAME] = '\0';
	memcpy(secName, imageSecHeader.Name, IMAGE_SIZEOF_SHORT_NAME);
#if 0
	cout << "PointerToRelocations: " << image_sec_header.PointerToRelocations
		<< "\t" << "PointerToLinenumbers: " << image_sec_header.PointerToLinenumbers
		<< "\t" << "NumberOfRelocations: " << image_sec_header.NumberOfRelocations
		<< "\t" << "NumberOfLinenumbers: " << image_sec_header.NumberOfLinenumbers << endl;
#endif
	cout << setfill('0') << setw(2) << setiosflags(ios::uppercase) << (sectionIdx + 1) << "   " << "Name: " << secName
		<< "\t" << "VirtualSize: " << hex << setfill('0') << setw(8) << imageSecHeader.Misc.VirtualSize
		<< "\t" << "VirtualOffset(RVA of VirtualAddress): " << hex << setfill('0') << setw(8) << imageSecHeader.VirtualAddress
		<< "\t" << "RawSize: " << hex << setfill('0') << setw(8) << imageSecHeader.SizeOfRawData
		<< "\t" << "RawOffset: " << hex << setfill('0') << setw(8) << imageSecHeader.PointerToRawData
		<< "\t" << "Characteristics: " << hex << setfill('0') << setw(8) << imageSecHeader.Characteristics << endl;

}


void testPrintHexBuffer() {
	unsigned char cLineBuf[] = {
		"\x33\x34\x35\x36\x37\x18\x19\x41\x42\x42\x43\x44\x45\x01\x47\x48"
		"\x38\x39\x37\x36"
	};
	printHexBuffer(cLineBuf, 0);
	printHexBuffer(cLineBuf, 5);
	printHexBuffer(cLineBuf, 15);
	printHexBuffer(cLineBuf, 16);
	printHexBuffer(cLineBuf, 17);
}


void CPEInfo::printDosHeader() {
	printField("Dos Header", (const unsigned char *)&m_dosHeader, sizeof(m_dosHeader));
	//1.Magic  
	printField("Magic", (const unsigned char *)&m_dosHeader.e_magic, sizeof(m_dosHeader.e_magic));
	printField("PE header offset", (const unsigned char *)&m_dosHeader.e_lfanew, sizeof(m_dosHeader.e_lfanew));
}

void CPEInfo::show() {
	printDosHeader();
	if (m_peHeader.x86.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		printPEHeaderX86();
	}
	else if (m_peHeader.x86.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
		printPEHeaderX64();
	}
	printImgSecHeader();
}

DWORD CPEInfo::getCoffHeaderSize() const {
    DWORD dwCoffHeaderRealSize = m_dosHeader.e_lfanew + m_dwSizeofImageNTHeaders + m_peHeader.x86.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
    DWORD dwFileAlignment = m_bX86 ? m_peHeader.x86.OptionalHeader.FileAlignment : m_peHeader.x64.OptionalHeader.FileAlignment;
    return ALIGN(dwCoffHeaderRealSize, dwFileAlignment);
}


void CPEInfo::printPEHeaderX86() {
	//2. PE header offset  
	printField("PE header", (const unsigned char *)&m_peHeader.x86, sizeof(m_peHeader.x86));
	printField("PE Signature", (const unsigned char *)&m_peHeader.x86.Signature, sizeof(m_peHeader.x86.Signature));
	
	//FileHeader
	printField("PE Machine", (const unsigned char *)&m_peHeader.x86.FileHeader.Machine, sizeof(m_peHeader.x86.FileHeader.Machine));
	printField("TimeDateStamp", (const unsigned char *)&m_peHeader.x86.FileHeader.TimeDateStamp, sizeof(m_peHeader.x86.FileHeader.TimeDateStamp));
	printField("PointerToSymbolTable", (const unsigned char *)&m_peHeader.x86.FileHeader.PointerToSymbolTable, sizeof(m_peHeader.x86.FileHeader.PointerToSymbolTable));
	printField("NumberOfSymbols", (const unsigned char *)&m_peHeader.x86.FileHeader.NumberOfSymbols, sizeof(m_peHeader.x86.FileHeader.NumberOfSymbols));
	printField("SizeOfOptionalHeader", (const unsigned char *)&m_peHeader.x86.FileHeader.SizeOfOptionalHeader, sizeof(m_peHeader.x86.FileHeader.SizeOfOptionalHeader));
	printField("Characteristics", (const unsigned char *)&m_peHeader.x86.FileHeader.Characteristics, sizeof(m_peHeader.x86.FileHeader.Characteristics));

	//OptionHeader
	printField("OptionalHeader.Magic", (const unsigned char *)&m_peHeader.x86.OptionalHeader.Magic, sizeof(m_peHeader.x86.OptionalHeader.Magic));


	printField("OptionalHeader.MajorLinkerVersion", (const unsigned char *)&m_peHeader.x86.OptionalHeader.MajorLinkerVersion, sizeof(m_peHeader.x86.OptionalHeader.MajorLinkerVersion));
	printField("OptionalHeader.MinorLinkerVersion", (const unsigned char *)&m_peHeader.x86.OptionalHeader.MinorLinkerVersion, sizeof(m_peHeader.x86.OptionalHeader.MinorLinkerVersion));
	printField("OptionalHeader.SizeOfCode", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfCode, sizeof(m_peHeader.x86.OptionalHeader.SizeOfCode));
	printField("OptionalHeader.SizeOfInitializedData", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfInitializedData, sizeof(m_peHeader.x86.OptionalHeader.SizeOfInitializedData));
	printField("OptionalHeader.SizeOfUninitializedData", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfUninitializedData, sizeof(m_peHeader.x86.OptionalHeader.SizeOfUninitializedData));
	printField("OptionalHeader.AddressOfEntryPoint", (const unsigned char *)&m_peHeader.x86.OptionalHeader.AddressOfEntryPoint, sizeof(m_peHeader.x86.OptionalHeader.AddressOfEntryPoint));
	printField("OptionalHeader.BaseOfCode", (const unsigned char *)&m_peHeader.x86.OptionalHeader.BaseOfCode, sizeof(m_peHeader.x86.OptionalHeader.BaseOfCode));
	printField("OptionalHeader.BaseOfData", (const unsigned char *)&m_peHeader.x86.OptionalHeader.BaseOfData, sizeof(m_peHeader.x86.OptionalHeader.BaseOfData));
	printField("OptionalHeader.ImageBase", (const unsigned char *)&m_peHeader.x86.OptionalHeader.ImageBase, sizeof(m_peHeader.x86.OptionalHeader.ImageBase));
	printField("OptionalHeader.SectionAlignment", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SectionAlignment, sizeof(m_peHeader.x86.OptionalHeader.SectionAlignment));
	printField("OptionalHeader.FileAlignment", (const unsigned char *)&m_peHeader.x86.OptionalHeader.FileAlignment, sizeof(m_peHeader.x86.OptionalHeader.FileAlignment));
	printField("OptionalHeader.MajorOperatingSystemVersion", (const unsigned char *)&m_peHeader.x86.OptionalHeader.MajorOperatingSystemVersion, sizeof(m_peHeader.x86.OptionalHeader.MajorOperatingSystemVersion));
	printField("OptionalHeader.MinorOperatingSystemVersion", (const unsigned char *)&m_peHeader.x86.OptionalHeader.MinorOperatingSystemVersion, sizeof(m_peHeader.x86.OptionalHeader.MinorOperatingSystemVersion));
	printField("OptionalHeader.MajorImageVersion", (const unsigned char *)&m_peHeader.x86.OptionalHeader.MajorImageVersion, sizeof(m_peHeader.x86.OptionalHeader.MajorImageVersion));
	printField("OptionalHeader.MinorImageVersion", (const unsigned char *)&m_peHeader.x86.OptionalHeader.MinorImageVersion, sizeof(m_peHeader.x86.OptionalHeader.MinorImageVersion));
	printField("OptionalHeader.MajorSubsystemVersion", (const unsigned char *)&m_peHeader.x86.OptionalHeader.MajorSubsystemVersion, sizeof(m_peHeader.x86.OptionalHeader.MajorSubsystemVersion));
	printField("OptionalHeader.MinorSubsystemVersion", (const unsigned char *)&m_peHeader.x86.OptionalHeader.MinorSubsystemVersion, sizeof(m_peHeader.x86.OptionalHeader.MinorSubsystemVersion));
	printField("OptionalHeader.Win32VersionValue", (const unsigned char *)&m_peHeader.x86.OptionalHeader.Win32VersionValue, sizeof(m_peHeader.x86.OptionalHeader.Win32VersionValue));
	printField("OptionalHeader.SizeOfImage", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfImage, sizeof(m_peHeader.x86.OptionalHeader.SizeOfImage));
	printField("OptionalHeader.SizeOfHeaders", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfHeaders, sizeof(m_peHeader.x86.OptionalHeader.SizeOfHeaders));
	printField("OptionalHeader.CheckSum", (const unsigned char *)&m_peHeader.x86.OptionalHeader.CheckSum, sizeof(m_peHeader.x86.OptionalHeader.CheckSum));
	printField("OptionalHeader.Subsystem", (const unsigned char *)&m_peHeader.x86.OptionalHeader.Subsystem, sizeof(m_peHeader.x86.OptionalHeader.Subsystem));
	printField("OptionalHeader.DllCharacteristics", (const unsigned char *)&m_peHeader.x86.OptionalHeader.DllCharacteristics, sizeof(m_peHeader.x86.OptionalHeader.DllCharacteristics));
	printField("OptionalHeader.SizeOfStackReserve", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfStackReserve, sizeof(m_peHeader.x86.OptionalHeader.SizeOfStackReserve));
	printField("OptionalHeader.SizeOfStackCommit", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfStackCommit, sizeof(m_peHeader.x86.OptionalHeader.SizeOfStackCommit));
	printField("OptionalHeader.SizeOfHeapReserve", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfHeapReserve, sizeof(m_peHeader.x86.OptionalHeader.SizeOfHeapReserve));
	printField("OptionalHeader.SizeOfHeapCommit", (const unsigned char *)&m_peHeader.x86.OptionalHeader.SizeOfHeapCommit, sizeof(m_peHeader.x86.OptionalHeader.SizeOfHeapCommit));
	printField("OptionalHeader.LoaderFlags", (const unsigned char *)&m_peHeader.x86.OptionalHeader.LoaderFlags, sizeof(m_peHeader.x86.OptionalHeader.LoaderFlags));
	printField("OptionalHeader.NumberOfRvaAndSizes", (const unsigned char *)&m_peHeader.x86.OptionalHeader.NumberOfRvaAndSizes, sizeof(m_peHeader.x86.OptionalHeader.NumberOfRvaAndSizes));
	printField("OptionalHeader.DataDirectory", (const unsigned char *)&m_peHeader.x86.OptionalHeader.DataDirectory, sizeof(IMAGE_DATA_DIRECTORY) * 16);

	IMAGE_DATA_DIRECTORY export_tbl_dir = m_peHeader.x86.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];//
	printField("OptionalHeader.DataDirectory.Import", (const unsigned char *)&m_peHeader.x86.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT], sizeof(IMAGE_DATA_DIRECTORY));
}
void CPEInfo::printPEHeaderX64() {
	//2. PE header offset  
	printField("PE header", (const unsigned char *)&m_peHeader.x64, sizeof(m_peHeader.x64));
	printField("PE Signature", (const unsigned char *)&m_peHeader.x64.Signature, sizeof(m_peHeader.x64.Signature));
	valid_signature((unsigned char*)&m_peHeader.x64.Signature);
	//FileHeader
	printField("PE Machine", (const unsigned char *)&m_peHeader.x64.FileHeader.Machine, sizeof(m_peHeader.x64.FileHeader.Machine));
	printField("TimeDateStamp", (const unsigned char *)&m_peHeader.x64.FileHeader.TimeDateStamp, sizeof(m_peHeader.x64.FileHeader.TimeDateStamp));
	printField("PointerToSymbolTable", (const unsigned char *)&m_peHeader.x64.FileHeader.PointerToSymbolTable, sizeof(m_peHeader.x64.FileHeader.PointerToSymbolTable));
	printField("NumberOfSymbols", (const unsigned char *)&m_peHeader.x64.FileHeader.NumberOfSymbols, sizeof(m_peHeader.x64.FileHeader.NumberOfSymbols));
	printField("SizeOfOptionalHeader", (const unsigned char *)&m_peHeader.x64.FileHeader.SizeOfOptionalHeader, sizeof(m_peHeader.x64.FileHeader.SizeOfOptionalHeader));
	printField("Characteristics", (const unsigned char *)&m_peHeader.x64.FileHeader.Characteristics, sizeof(m_peHeader.x64.FileHeader.Characteristics));

	//OptionHeader
	printField("OptionalHeader.Magic", (const unsigned char *)&m_peHeader.x64.OptionalHeader.Magic, sizeof(m_peHeader.x64.OptionalHeader.Magic));


	printField("OptionalHeader.MajorLinkerVersion", (const unsigned char *)&m_peHeader.x64.OptionalHeader.MajorLinkerVersion, sizeof(m_peHeader.x64.OptionalHeader.MajorLinkerVersion));
	printField("OptionalHeader.MinorLinkerVersion", (const unsigned char *)&m_peHeader.x64.OptionalHeader.MinorLinkerVersion, sizeof(m_peHeader.x64.OptionalHeader.MinorLinkerVersion));
	printField("OptionalHeader.SizeOfCode", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfCode, sizeof(m_peHeader.x64.OptionalHeader.SizeOfCode));
	printField("OptionalHeader.SizeOfInitializedData", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfInitializedData, sizeof(m_peHeader.x64.OptionalHeader.SizeOfInitializedData));
	printField("OptionalHeader.SizeOfUninitializedData", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfUninitializedData, sizeof(m_peHeader.x64.OptionalHeader.SizeOfUninitializedData));
	printField("OptionalHeader.AddressOfEntryPoint", (const unsigned char *)&m_peHeader.x64.OptionalHeader.AddressOfEntryPoint, sizeof(m_peHeader.x64.OptionalHeader.AddressOfEntryPoint));
	printField("OptionalHeader.BaseOfCode", (const unsigned char *)&m_peHeader.x64.OptionalHeader.BaseOfCode, sizeof(m_peHeader.x64.OptionalHeader.BaseOfCode));
	//print_pe_field("OptionalHeader.BaseOfData", (const unsigned char *)&header.m_pe_header_64.OptionalHeader.BaseOfData, sizeof(header.m_pe_header_64.OptionalHeader.BaseOfData));
	printField("OptionalHeader.ImageBase", (const unsigned char *)&m_peHeader.x64.OptionalHeader.ImageBase, sizeof(m_peHeader.x64.OptionalHeader.ImageBase));
	printField("OptionalHeader.SectionAlignment", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SectionAlignment, sizeof(m_peHeader.x64.OptionalHeader.SectionAlignment));
	printField("OptionalHeader.FileAlignment", (const unsigned char *)&m_peHeader.x64.OptionalHeader.FileAlignment, sizeof(m_peHeader.x64.OptionalHeader.FileAlignment));
	printField("OptionalHeader.MajorOperatingSystemVersion", (const unsigned char *)&m_peHeader.x64.OptionalHeader.MajorOperatingSystemVersion, sizeof(m_peHeader.x64.OptionalHeader.MajorOperatingSystemVersion));
	printField("OptionalHeader.MinorOperatingSystemVersion", (const unsigned char *)&m_peHeader.x64.OptionalHeader.MinorOperatingSystemVersion, sizeof(m_peHeader.x64.OptionalHeader.MinorOperatingSystemVersion));
	printField("OptionalHeader.MajorImageVersion", (const unsigned char *)&m_peHeader.x64.OptionalHeader.MajorImageVersion, sizeof(m_peHeader.x64.OptionalHeader.MajorImageVersion));
	printField("OptionalHeader.MinorImageVersion", (const unsigned char *)&m_peHeader.x64.OptionalHeader.MinorImageVersion, sizeof(m_peHeader.x64.OptionalHeader.MinorImageVersion));
	printField("OptionalHeader.MajorSubsystemVersion", (const unsigned char *)&m_peHeader.x64.OptionalHeader.MajorSubsystemVersion, sizeof(m_peHeader.x64.OptionalHeader.MajorSubsystemVersion));
	printField("OptionalHeader.MinorSubsystemVersion", (const unsigned char *)&m_peHeader.x64.OptionalHeader.MinorSubsystemVersion, sizeof(m_peHeader.x64.OptionalHeader.MinorSubsystemVersion));
	printField("OptionalHeader.Win32VersionValue", (const unsigned char *)&m_peHeader.x64.OptionalHeader.Win32VersionValue, sizeof(m_peHeader.x64.OptionalHeader.Win32VersionValue));
	printField("OptionalHeader.SizeOfImage", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfImage, sizeof(m_peHeader.x64.OptionalHeader.SizeOfImage));
	printField("OptionalHeader.SizeOfHeaders", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfHeaders, sizeof(m_peHeader.x64.OptionalHeader.SizeOfHeaders));
	printField("OptionalHeader.CheckSum", (const unsigned char *)&m_peHeader.x64.OptionalHeader.CheckSum, sizeof(m_peHeader.x64.OptionalHeader.CheckSum));
	printField("OptionalHeader.Subsystem", (const unsigned char *)&m_peHeader.x64.OptionalHeader.Subsystem, sizeof(m_peHeader.x64.OptionalHeader.Subsystem));
	printField("OptionalHeader.DllCharacteristics", (const unsigned char *)&m_peHeader.x64.OptionalHeader.DllCharacteristics, sizeof(m_peHeader.x64.OptionalHeader.DllCharacteristics));
	printField("OptionalHeader.SizeOfStackReserve", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfStackReserve, sizeof(m_peHeader.x64.OptionalHeader.SizeOfStackReserve));
	printField("OptionalHeader.SizeOfStackCommit", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfStackCommit, sizeof(m_peHeader.x64.OptionalHeader.SizeOfStackCommit));
	printField("OptionalHeader.SizeOfHeapReserve", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfHeapReserve, sizeof(m_peHeader.x64.OptionalHeader.SizeOfHeapReserve));
	printField("OptionalHeader.SizeOfHeapCommit", (const unsigned char *)&m_peHeader.x64.OptionalHeader.SizeOfHeapCommit, sizeof(m_peHeader.x64.OptionalHeader.SizeOfHeapCommit));
	printField("OptionalHeader.LoaderFlags", (const unsigned char *)&m_peHeader.x64.OptionalHeader.LoaderFlags, sizeof(m_peHeader.x64.OptionalHeader.LoaderFlags));
	printField("OptionalHeader.NumberOfRvaAndSizes", (const unsigned char *)&m_peHeader.x64.OptionalHeader.NumberOfRvaAndSizes, sizeof(m_peHeader.x64.OptionalHeader.NumberOfRvaAndSizes));
	printField("OptionalHeader.DataDirectory", (const unsigned char *)&m_peHeader.x64.OptionalHeader.DataDirectory, sizeof(IMAGE_DATA_DIRECTORY) * 16);

	IMAGE_DATA_DIRECTORY export_tbl_dir = m_peHeader.x64.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];//
	printField("OptionalHeader.DataDirectory.Import", (const unsigned char *)&m_peHeader.x64.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT], sizeof(IMAGE_DATA_DIRECTORY));
}

CPEInfo::CPEInfo():m_sectionNum(0),m_dwSizeofImageNTHeaders(0),    m_bLoaded(false),    m_pMapViewBase(nullptr),
    #ifdef WIN32
    m_hFile(INVALID_HANDLE_VALUE),
    #elif __linux__
    m_fd(-1),
    #endif
    m_dwFileSize(0),    m_bX86(true)
{
	m_vecImgSecHeader.reserve(20);
}

CPEInfo::~CPEInfo() {
    CloseMapViewAndFiles();
}

void CPEInfo::CloseMapViewAndFiles() {
#ifdef WIN32
    if (m_pMapViewBase) {
        ::UnmapViewOfFile(m_pMapViewBase);
    }
    if (INVALID_HANDLE_VALUE != m_hFile) {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
#elif __linux__
    if (m_pMapViewBase) {
        ::munmap(m_pMapViewBase, m_dwFileSize);
    }
    if(-1 != m_fd) {
        close(m_fd);
        m_fd = -1;
    }
#endif
    m_pMapViewBase = nullptr;
    m_dwFileSize = 0;
    m_bLoaded = false;
}

#if defined(WIN32)
bool CPEInfo::LoadPE(LPCTSTR pFileName) {
#elif __linux__
bool CPEInfo::LoadPE(const char* pFileName) {
#endif
	m_bLoaded = false;
    CloseMapViewAndFiles();
    if(!pFileName) {
        return false;
    }
    #if defined(WIN32)
        m_hFile = CreateFile(pFileName, GENERIC_READ,
           FILE_SHARE_READ,
           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
       if (INVALID_HANDLE_VALUE == m_hFile) {
          // DWORD errCode = ::GetLastError();
           return false;
       }
       DWORD dwFileSizeHigh = 0;
       m_dwFileSize = ::GetFileSize(m_hFile, &dwFileSizeHigh);
       //file size is larger or equal that 4G
       if(dwFileSizeHigh > 0) {
            return false;
       }
       HANDLE hMap = ::CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
       if (hMap) {
           m_pMapViewBase = ::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
       }
    #elif __linux__
       m_fd = open(pFileName, O_RDONLY);
       qDebug() << "linux open file: [" << pFileName << "], fd=" << m_fd << ", filename length: " << strlen(pFileName) ;
       if(-1 == m_fd) {
           return false;
       }
       struct stat sb;
       qDebug() << "before: m_dwFileSize: " << m_dwFileSize;
       fstat(m_fd, &sb);
       qDebug() << "end: sb.st_size: " << sb.st_size;
       m_dwFileSize = sb.st_size;
       qDebug() << "m_dwFileSize: " << m_dwFileSize;
       m_pMapViewBase = mmap(NULL, m_dwFileSize, PROT_READ, MAP_PRIVATE, m_fd, 0);
       qDebug() << "m_pMapViewBase: " << m_pMapViewBase;
    #endif
       if (m_pMapViewBase) {
           m_bLoaded = readPEInfoFromMapView(m_pMapViewBase);
       }
	  
       return m_bLoaded;
}

bool CPEInfo::readPEInfoFromMapView(void* pMapViewBase) {
	if (!pMapViewBase) {
		return false;
	}
	memcpy(&m_dosHeader, pMapViewBase, sizeof(m_dosHeader));
    //qDebug() << "m_dosHeader.e_magic: " << hex << m_dosHeader.e_magic << "sizeof(m_dosHeader): " << sizeof(m_dosHeader);
	valid_magic(reinterpret_cast<unsigned char*>(&m_dosHeader.e_magic));

	//PE Header
    //qDebug() << "m_dosHeader.e_lfanew: " << hex << m_dosHeader.e_lfanew << "sizeof(m_peHeader.x86): " << sizeof(m_peHeader.x86);
	memcpy(&m_peHeader.x86, reinterpret_cast<unsigned char*>(pMapViewBase) + m_dosHeader.e_lfanew, sizeof(m_peHeader.x86));
	valid_signature(reinterpret_cast<unsigned char*>(&m_peHeader.x86.Signature));
	if (m_peHeader.x86.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {  //0x10b
        m_bX86 = true;
		//cout << "32-bit app" << endl;
		WORD dwDefaultX86SizeOfOptionalHeader = 0x00E0;
		if (m_peHeader.x86.FileHeader.SizeOfOptionalHeader != dwDefaultX86SizeOfOptionalHeader) {
			cout << "Special SizeOfOptionalHeader in PE Header. Maybe Need to Check the Directory and Section Table" << endl;
		}
		if(m_peHeader.x86.FileHeader.SizeOfOptionalHeader < dwDefaultX86SizeOfOptionalHeader) {
			if (m_peHeader.x86.OptionalHeader.NumberOfRvaAndSizes < 2 || m_peHeader.x86.OptionalHeader.NumberOfRvaAndSizes > 16) {
				return false;
			}
			if ((dwDefaultX86SizeOfOptionalHeader - m_peHeader.x86.FileHeader.SizeOfOptionalHeader) != sizeof(IMAGE_DATA_DIRECTORY) * (16 - m_peHeader.x86.OptionalHeader.NumberOfRvaAndSizes)) {
				return false;
			}
		}
       m_dwSizeofImageNTHeaders = sizeof(m_peHeader.x86.Signature) + sizeof(m_peHeader.x86.FileHeader) + m_peHeader.x86.FileHeader.SizeOfOptionalHeader;
	   m_sectionNum = m_peHeader.x86.FileHeader.NumberOfSections;

	}
	else if (m_peHeader.x86.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {  //0x20b
        m_bX86 = false;
		memcpy(&m_peHeader.x64, reinterpret_cast<unsigned char*>(pMapViewBase) + m_dosHeader.e_lfanew, sizeof(m_peHeader.x64));
		WORD dwDefaultX64SizeOfOptionalHeader = 0x00F0;
		if (m_peHeader.x64.FileHeader.SizeOfOptionalHeader != dwDefaultX64SizeOfOptionalHeader) {
			cout << "Special SizeOfOptionalHeader in PE Header. Maybe Need to Check the Directory and Section Table" << endl;
		}
		if (m_peHeader.x64.FileHeader.SizeOfOptionalHeader < dwDefaultX64SizeOfOptionalHeader) {
			if (m_peHeader.x64.OptionalHeader.NumberOfRvaAndSizes < 2 || m_peHeader.x64.OptionalHeader.NumberOfRvaAndSizes > 16) {
				return false;
			}
			if ((dwDefaultX64SizeOfOptionalHeader - m_peHeader.x64.FileHeader.SizeOfOptionalHeader) != sizeof(IMAGE_DATA_DIRECTORY) * (16 - m_peHeader.x64.OptionalHeader.NumberOfRvaAndSizes)) {
				return false;
			}
		}
        m_dwSizeofImageNTHeaders = sizeof(m_peHeader.x64.Signature) + sizeof(m_peHeader.x64.FileHeader) + m_peHeader.x64.FileHeader.SizeOfOptionalHeader;
		m_sectionNum = m_peHeader.x64.FileHeader.NumberOfSections;
	}
	else if (m_peHeader.x86.OptionalHeader.Magic == IMAGE_ROM_OPTIONAL_HDR_MAGIC) { //0x107
        m_bX86 = false;
		//cout << "rom image" << endl;
		return false;
	}
	//print_pe_field("OptionalHeader.DataDirectory.After", reinterpret_cast<unsigned char*>(pMapView) + dos_header.e_lfanew, 1024);

    IMAGE_SECTION_HEADER  * pImageSecHeader = reinterpret_cast<IMAGE_SECTION_HEADER  *>(reinterpret_cast<unsigned char*>(pMapViewBase) + m_dosHeader.e_lfanew + m_dwSizeofImageNTHeaders);
	m_vecImgSecHeader.clear();
	//print_pe_field("Sections", (const unsigned char *)p_image_sec_header, 1024);
	for (auto sectionIdx = 0; sectionIdx < m_sectionNum; ++sectionIdx) {
		m_vecImgSecHeader.emplace_back(*pImageSecHeader);
		++pImageSecHeader;
	}
	return true;
}

void CPEInfo::printImgSecHeader() {
	cout << "NumberOfSections: " << hex << setfill('0') << setw(2) << setiosflags(ios::uppercase) << m_sectionNum << endl;
	unsigned int sectionIdx = 0;
	for (const auto &imgSecHeader : m_vecImgSecHeader) {
		printField("IMAGE_SECTION_HEADER", reinterpret_cast<const unsigned char *>(&imgSecHeader), sizeof(IMAGE_SECTION_HEADER));
        printSectionHeaderInfo(sectionIdx++, imgSecHeader);
	}
}


DWORD CPEInfo::rvaToFoa(DWORD rva) const {
	for (const auto &imgSecHeader : m_vecImgSecHeader) {
		if (rva >= imgSecHeader.VirtualAddress && rva <= (imgSecHeader.VirtualAddress + imgSecHeader.SizeOfRawData)) {
			return imgSecHeader.PointerToRawData + (rva - imgSecHeader.VirtualAddress);
		}
	}
    return rva ? -1: 0;
}

DWORD CPEInfo::foaToRva(DWORD foa) const {
	for (const auto &imgSecHeader : m_vecImgSecHeader) {
		if (foa >= imgSecHeader.PointerToRawData && foa < (imgSecHeader.PointerToRawData + imgSecHeader.SizeOfRawData)) {
			DWORD rva = imgSecHeader.VirtualAddress + (foa - imgSecHeader.PointerToRawData);
			return rva;
		}
	}
    return foa ? -1: 0;
}


//
void CPEInfo::loadOrigThunkDetail(DWORD orgFirstThunk) {
	//cout << "OriginalFirstThunk(RVA): " << hex << setfill('0') << setw(8) << orgFirstThunk << endl;
	DWORD orgThunkFoa = rvaToFoa(orgFirstThunk);
	cout << "orgThunkFoa: " << orgThunkFoa << endl;
	printHexBuffer(reinterpret_cast<unsigned char*>(m_pMapViewBase) + orgThunkFoa, 0x30);

	vector<ImportHintName> vecImportHintName;
	IMAGE_THUNK_DATA *pImgThunkData = reinterpret_cast<IMAGE_THUNK_DATA*>(reinterpret_cast<unsigned char*>(m_pMapViewBase) + orgThunkFoa);
	while (pImgThunkData->u1.AddressOfData != 0) {
		if (pImgThunkData->u1.AddressOfData & 0x80000000) {
			char cForwarderString[sizeof(DWORD) + 1] = { 0 };
			memcpy(cForwarderString, &pImgThunkData->u1.AddressOfData, sizeof(DWORD));
			cForwarderString[sizeof(DWORD)] = '\0';
			cout << "ForwarderString: " << cForwarderString << endl;
		}
		else {
			DWORD foa = rvaToFoa(pImgThunkData->u1.AddressOfData);
			if (-1 != foa)
			{
				IMAGE_IMPORT_BY_NAME* pImgThunkData = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(reinterpret_cast<unsigned char*>(m_pMapViewBase) + foa);
				ImportHintName importHintName(*pImgThunkData);
				vecImportHintName.emplace_back(importHintName);
				//importHintName.showInfo();
			}
			else
			{
				break;
			}
		}
		++pImgThunkData;
	}
	m_mapOrigThunk[orgFirstThunk] = vecImportHintName;
}


void CPEInfo::loadFirstThunkDetail(DWORD firstThunk) {
	//cout << "firstThunk(RVA): " << hex << setfill('0') << setw(8) << firstThunk << endl;
	vector<DWORD> vecIAT;
	getDWORDVecFromStartRva(firstThunk, vecIAT);
	m_mapImportIAT[firstThunk] = vecIAT;
}

const IMAGE_DATA_DIRECTORY * CPEInfo::getDataDirectory(DWORD index) const {
	if (index >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {
		return nullptr;
	}
	IMAGE_DATA_DIRECTORY * pDataDirectory = nullptr;
	if (m_peHeader.x86.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {  //0x10b
		return &m_peHeader.x86.OptionalHeader.DataDirectory[index];
	}
	else if (m_peHeader.x86.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {  //0x20b
		return &m_peHeader.x64.OptionalHeader.DataDirectory[index];
	}
	return nullptr;
}

inline string CPEInfo::getStrFromRva(DWORD dwRva) {
	DWORD dwFoaForStr = rvaToFoa(dwRva);
	return string(reinterpret_cast<char*>(reinterpret_cast<unsigned char*>(m_pMapViewBase) + dwFoaForStr));
}

void CPEInfo::getDWORDVecFromStartRva(DWORD dwRva, std::vector<DWORD> &vecDwValue, DWORD dwValueNum) {
	//cout << "dwRva(RVA): " << hex << setfill('0') << setw(8) << dwRva << endl;
	DWORD dwFoa = rvaToFoa(dwRva);
	//printHexBuffer(reinterpret_cast<unsigned char*>(m_pMapView) + dwFoa, 0x30);

	DWORD *pdwValue = reinterpret_cast<DWORD*>(reinterpret_cast<unsigned char*>(m_pMapViewBase) + dwFoa);
	
	if (0 == dwValueNum) {
		while (*pdwValue != 0) {
			vecDwValue.emplace_back(*pdwValue);
			++pdwValue;
		}
	}
	else {
		for (DWORD i = 0; i < dwValueNum; ++i) {
			vecDwValue.emplace_back(*pdwValue);
			++pdwValue;
		}
	}
}

void CPEInfo::getWORDVecFromStartRva(DWORD dwRva, std::vector<WORD> &vecDwValue, DWORD dwValueNum) {
    //cout << "dwRva(RVA): " << hex << setfill('0') << setw(8) << dwRva << endl;
    DWORD dwFoa = rvaToFoa(dwRva);
    //printHexBuffer(reinterpret_cast<unsigned char*>(m_pMapView) + dwFoa, 0x30);

    WORD *pdwValue = reinterpret_cast<WORD*>(reinterpret_cast<unsigned char*>(m_pMapViewBase) + dwFoa);

	
    if (0 == dwValueNum) {
        while (*pdwValue != 0) {
            vecDwValue.emplace_back(*pdwValue);
            ++pdwValue;
        }
    }
    else {
        for (DWORD i = 0; i < dwValueNum; ++i) {
            vecDwValue.emplace_back(*pdwValue);
            ++pdwValue;
        }
    }
}

//
void CPEInfo::loadImportDataDirectory() {
	m_vecImgImportDesc.clear();
	const IMAGE_DATA_DIRECTORY * pImportDataDirectory = getDataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT);
	if (!pImportDataDirectory) {
		return;
	}
	DWORD rva = pImportDataDirectory->VirtualAddress;
	DWORD importDataDirFoa = rvaToFoa(rva);
	
	//printHexBuffer(reinterpret_cast<unsigned char*>(m_pMapView) + foa, pImportDataDirectory->Size);
	DWORD dwImageImportDescSize = sizeof(IMAGE_IMPORT_DESCRIPTOR);
//	assert((pImportDataDirectory->Size % dwImageImportDescSize) == 0);

	IMAGE_IMPORT_DESCRIPTOR *pImgImportDesc = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(reinterpret_cast<unsigned char*>(m_pMapViewBase) + importDataDirFoa);

	printHexBuffer(reinterpret_cast<unsigned char*>(m_pMapViewBase) + importDataDirFoa, 0x40);

    while (pImgImportDesc->OriginalFirstThunk || pImgImportDesc->FirstThunk || pImgImportDesc->ForwarderChain || pImgImportDesc->Name) {
		printHexBuffer(reinterpret_cast<const unsigned char*>(pImgImportDesc), sizeof(*pImgImportDesc));
        if (pImgImportDesc->OriginalFirstThunk) {
            loadOrigThunkDetail(pImgImportDesc->OriginalFirstThunk);
		}
		//cout << "TimeDateStamp: " << hex << setfill('0') << setw(8) << pImgImportDesc->TimeDateStamp << endl;
		//cout << "ForwarderChain: " << hex << setfill('0') << setw(8) << pImgImportDesc->ForwarderChain << endl;
		//cout << "Name(RVA): " << hex << setfill('0') << setw(8) << pImgImportDesc->Name << endl;
		if (pImgImportDesc->FirstThunk) {
			loadFirstThunkDetail(pImgImportDesc->FirstThunk);
		}
		IMAGE_IMPORT_DESCRIPTOR_Wrapper imgImportDescWrapper(*pImgImportDesc, getStrFromRva(pImgImportDesc->Name));
		m_vecImgImportDesc.emplace_back(imgImportDescWrapper);
		//imgImportDescWrapper.showInfo();
		++ pImgImportDesc;
	}
}


void CPEInfo::loadIATDataDirectory() {
	const IMAGE_DATA_DIRECTORY * pIATDataDirectory = getDataDirectory(IMAGE_DIRECTORY_ENTRY_IAT);
	if (!pIATDataDirectory) {
		return;
	}
	DWORD iatDataDirFoa = rvaToFoa(pIATDataDirectory->VirtualAddress);
	printHexBuffer(reinterpret_cast<unsigned char*>(m_pMapViewBase) + iatDataDirFoa, pIATDataDirectory->Size);
}
 

void CPEInfo::loadEATDataDirectory() {
	m_imgExportInfoWrapper.init();
	const IMAGE_DATA_DIRECTORY * pEATDataDirectory = getDataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (!pEATDataDirectory) {
		return;
	}
	
	if (pEATDataDirectory->VirtualAddress) {
		DWORD eatDataDirFoa = rvaToFoa(pEATDataDirectory->VirtualAddress);
		if (eatDataDirFoa) {
			//printHexBuffer(reinterpret_cast<unsigned char*>(m_pMapViewBase) + eatDataDirFoa, pEATDataDirectory->Size);
			//cout << "sizeof(IMAGE_EXPORT_DIRECTORY): " << sizeof(IMAGE_EXPORT_DIRECTORY) << ", and pEATDataDirectory->Size: " << pEATDataDirectory->Size << endl;
			IMAGE_EXPORT_DIRECTORY *pImgExportDirectory = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(reinterpret_cast<unsigned char*>(m_pMapViewBase) + eatDataDirFoa);
			//printField("pImgExportDirectory->Characteristics", (const unsigned char *)&pImgExportDirectory->Characteristics, sizeof(pImgExportDirectory->Characteristics));
			//printField("pImgExportDirectory->TimeDateStamp", (const unsigned char *)&pImgExportDirectory->TimeDateStamp, sizeof(pImgExportDirectory->TimeDateStamp));
			//printField("pImgExportDirectory->MajorVersion", (const unsigned char *)&pImgExportDirectory->MajorVersion, sizeof(pImgExportDirectory->MajorVersion));
			//printField("pImgExportDirectory->MinorVersion", (const unsigned char *)&pImgExportDirectory->MinorVersion, sizeof(pImgExportDirectory->MinorVersion));
			//printField("pImgExportDirectory->Name", (const unsigned char *)&pImgExportDirectory->Name, sizeof(pImgExportDirectory->Name));
			//printField("pImgExportDirectory->Base", (const unsigned char *)&pImgExportDirectory->Base, sizeof(pImgExportDirectory->Base));
			//printField("pImgExportDirectory->NumberOfFunctions", (const unsigned char *)&pImgExportDirectory->NumberOfFunctions, sizeof(pImgExportDirectory->NumberOfFunctions));
			//printField("pImgExportDirectory->NumberOfNames", (const unsigned char *)&pImgExportDirectory->NumberOfNames, sizeof(pImgExportDirectory->NumberOfNames));
			//printField("pImgExportDirectory->AddressOfFunctions", (const unsigned char *)&pImgExportDirectory->AddressOfFunctions, sizeof(pImgExportDirectory->AddressOfFunctions));
			//printField("pImgExportDirectory->AddressOfNames", (const unsigned char *)&pImgExportDirectory->AddressOfNames, sizeof(pImgExportDirectory->AddressOfNames));
			//printField("pImgExportDirectory->AddressOfNameOrdinals", (const unsigned char *)&pImgExportDirectory->AddressOfNameOrdinals, sizeof(pImgExportDirectory->AddressOfNameOrdinals));
			//cout << "pImgExportDirectory->Name string: [" << getStrFromRva(pImgExportDirectory->Name) << "]" << endl;

			std::vector<DWORD> vecFuncNameAddr;
			std::vector<std::string> vecFuncName;
			std::vector<DWORD> vecFuncAddr;
			std::vector<WORD> vecFuncNameOrdinal;
			if (pImgExportDirectory->NumberOfNames) {
			    getDWORDVecFromStartRva(pImgExportDirectory->AddressOfNames, vecFuncNameAddr, pImgExportDirectory->NumberOfNames);
				int nIdx = 0;
				for (auto dwFunNameAddr : vecFuncNameAddr) {
					vecFuncName.emplace_back(getStrFromRva(dwFunNameAddr));
				}
			}

			if (pImgExportDirectory->NumberOfFunctions) {
				getWORDVecFromStartRva(pImgExportDirectory->AddressOfNameOrdinals, vecFuncNameOrdinal, pImgExportDirectory->NumberOfFunctions);
			    getDWORDVecFromStartRva(pImgExportDirectory->AddressOfFunctions, vecFuncAddr, pImgExportDirectory->NumberOfFunctions);
			}
			m_imgExportInfoWrapper.SetImgExportInfoWrapper(*pImgExportDirectory, getStrFromRva(pImgExportDirectory->Name), vecFuncName, vecFuncAddr, vecFuncNameOrdinal);
            //m_imgExportInfoWrapper.showInfo();
		}
	}
}

void CPEInfo::testGetFuncByName(const std::string &strFuncName) {
	WORD wFuncOrder = 0;
	DWORD dwFuncAddr = 0;
	bool bRet = m_imgExportInfoWrapper.getFuncByName(strFuncName, wFuncOrder, dwFuncAddr);
    if (bRet) {
        std::cout << "dwFuncAddr = " << std::hex << std::setfill('0') << std::setw(8) << dwFuncAddr << std::endl;
        DWORD eatDataDirFoa = rvaToFoa(dwFuncAddr);
        printHexBuffer(reinterpret_cast<unsigned char*>(m_pMapViewBase) + eatDataDirFoa, 100);
    }
    

}
