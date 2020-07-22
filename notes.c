

  #include <windows.h>

  #include <stdio.h>



  typedef unsigned int (*SECVALIDATE)(UCHAR *MAC, UCHAR *ID);

  typedef unsigned int (*SECCALCMAC)(UCHAR *MAC, UCHAR *ID);

  typedef unsigned int (*SECCALCMESSAGEDIGEST)(UCHAR *Hash, UCHAR *lpszPwd, UINT nPwdLen);



  int main(int argc, char *argv[]) {

          UCHAR   Hash[0x10];

          UCHAR   ID[0x38];

          UCHAR   MAC[0x8];



          HANDLE  hFileID;

          HANDLE  hMapFileID;

          HANDLE  hMemoryID;

          HANDLE  hModule;



          SECVALIDATE pfSecValidate;

          SECCALCMESSAGEDIGEST pfSecCalcMessageDigest;

          SECCALCMAC pfSecCalcMAC;



          if (argc != 3) {

                  fprintf(stderr, "Usage: %s [USER.ID] [PASSWORD]\n", argv[0]);

                  return(1);

          }



          // import functions from ID.DLL

          hModule=LoadLibrary("ID.DLL");

          pfSecValidate=(SECVALIDATE) GetProcAddress(hModule, "SecValidate");

          pfSecCalcMessageDigest=(SECCALCMESSAGEDIGEST) GetProcAddress(hModule,

                  "SecCalcMessageDigest");

          pfSecCalcMAC=(SECCALCMAC) GetProcAddress(hModule, "SecCalcMAC");



          // memory-map the ID file

          hFileID=CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING,

                  FILE_ATTRIBUTE_ARCHIVE, NULL);

          hMapFileID=CreateFileMapping(hFileID, NULL, PAGE_READONLY, 0, 0, NULL);

          hMemoryID=MapViewOfFile(hMapFileID, FILE_MAP_READ ,0 ,0 , 0);



          // initialize ID

          memcpy(ID, (PCHAR) hMemoryID + 0xD8, 0x38);



          pfSecCalcMessageDigest(Hash, argv[2], strlen(argv[2]));

          pfSecCalcMAC(MAC, Hash);



          if (pfSecValidate(MAC, ID)==TRUE) {

                  fprintf(stdout, "Eureka!\n");

          } else {

                  fprintf(stdout, "Incorrect password!\n");

          }



          UnmapViewOfFile(hMemoryID);

          CloseHandle(hMapFileID);

          CloseHandle(hFileID);

          FreeLibrary(hModule);



          return(0);

  }

