/***************************** C HEADER File_iMn *********************************
**
**  Project:    FlashFile
**  Filename:   FILE_SYS.H 
**  Version:    3.0 
**  Date:       March 29, 2006
**  
******************************************************************************
******************************************************************************
**
**  VERSION HISTORY:
**  ----------------
**  Version:    3.0 
**  Date:       March 29, 2006
**  Revised by: Erick M. Higa
**  Description:
**      - See "FILE_SYS.C" file for any chages up to this point.
**
*****************************************************************************/
#ifndef _FILE_SYS_INCLUDED
#define _FILE_SYS_INCLUDED

/*****************************************************************************
**
**    MODULES USED 
**
*****************************************************************************/

/*****************************************************************************
**
**    DEFINITIONS AND MACROS
**
*****************************************************************************/
#ifndef NULL_CHAR
    #define NULL_CHAR    0
#endif
#ifndef SUCCESS
    #define SUCCESS            (TRUE)
    #define FAIL               (FALSE)
#endif

/*
#ifndef EOF
    #define EOF     -1
#endif
*/

#define ATTR_READ_ONLY  0x01
#define ATTR_HIDDEN     0x02
#define ATTR_SYSTEM     0x04
#define ATTR_VOL_ID     0x08
#define ATTR_DIRECTORY  0x10
#define ATTR_ARCHIVE    0x20
#define ATTR_LONG_NAME  0x0F

//*rwl #define fgetc   fgetc_iMn
#define fputc   fputc_
#define File_iMn    FileStruct
#ifdef _NO_MALLOC_
    #define _FF_free(qp)    qp->used_flag = 0
#else
    #define _FF_MALLOC()    malloc(sizeof(FILE))
    #define _FF_free        free
#endif

#ifdef _DEBUG_FUNCTIONS_
    #define _DEBUG_SCANDIR_
    #define _DEBUG_C2A_
    #define _DEBUG_A2C_
    #define _DEBUG_NEXTCLUS_
    #define _DEBUG_PREVCLUS_
    #define _DEBUG_WRITE_CLUSTER_TABLE_
    #define _DEBUG_APPRNDTOC_
    #define _DEBUG_MKDIR_
    #define _DEBUG_CHDIR_
    #define _DEBUG_FF_CHDIR_
    #define _DEBUG_FOPEN_
    #define _DEBUG_FCREATE_
    #define _DEBUG_FFLUSH_
    /*#define _DEBUG_FPUTC_*/
    #define _DEBUG_STREAM_START_
#endif


/****************************************************************************
**
**    TYPEDEFS AND STRUCTURES
**
****************************************************************************/

// 32 byte structure
typedef struct
{
    int8 name_entry[11];
    uint8 attr;
    uint8 reserved;
    uint8 crt_tenth_sec;
    uint8 crt_time_LO;
    uint8 crt_time_HI;
    uint8 crt_date_LO;
    uint8 crt_date_HI;
    uint8 acc_date_LO;
    uint8 acc_date_HI;
    uint8 start_clus_hi_LO;
    uint8 start_clus_hi_HI;
    uint8 mod_time_LO;
    uint8 mod_time_HI;
    uint8 mod_date_LO;
    uint8 mod_date_HI;
    uint8 start_clus_lo_LO;
    uint8 start_clus_lo_HI;
    uint8 file_size_LO;
    uint8 file_size_ML;
    uint8 file_size_MH;
    uint8 file_size_HI;
} FileDirEntryStruct;

// 552 bytes (+4 overhead using malloc) ==>  556 bytes required per file opened
typedef struct
{
    HiLo16Union clus_start;             // Starting cluster of open file
    uint16 clus_current;                // Current cluster of open file
    uint16 clus_next;                   // Next (reading) cluster of open file
    uint16 clus_prev;                   // Previous cluster of open file
    uint8 sec_offset;                   // Current sector withing current cluster
    uint32 entry_sec_addr;              // Sector address of File entry of open file
    FileDirEntryStruct *file_dep;       // Entry Pointer to the offset of the File Info
    #ifndef _NO_FILE_DATA_BUFFER_
        uint8 buff[512];                // Open file read/write buffer
    #endif
    HiLo32Union length;                 // Length of file
    uint32 position;                    // Current position in file relative to begining
    uint8 mode;                         // 0=>closed, 1=>open for read, 2=>open for write, 3=> open for append
    uint8 error;                        // Error indicator
    uint8 EOF_flag;                     // End of File Flag
    uint8 *pntr;                        // Pointer for file data use
    #ifdef _NO_MALLOC_
        uint8 used_flag;                // Flag to see if the file is used or not
    #endif
} File_iMn, *FilePtr;
#endif  //_FILE_SYS_INCLUDED


/****************************************************************************
** 
**    EXPORTED VARIABLES
**
****************************************************************************/
#ifndef _FILE_SYS_C_SRC
    extern int8 const _FF_VERSION_[];
    #ifdef _NO_MALLOC_
        extern File_iMn _FF_FileSpaceAllocation[_FF_MAX_FILES_OPEN];
    #endif
#endif  /*_FILE_SYS_C_SRC*/


/****************************************************************************
**
**    EXPORTED FUNCTIONS
**
****************************************************************************/
int8                ascii_to_char(int8 ascii_char);
int8                valid_file_char(int8 file_char);
#ifdef _DEBUG_ON_
    void read_directory(void);
    #ifndef _NO_FILE_DATA_BUFFER_
        void        dump_file_data_hex(File_iMn *rp);
        void        dump_file_data_view(File_iMn *rp);
    #endif
    void            dump_buff_data_hex(void);
    void            dump_file_entry_hex(FileDirEntryStruct *dep);
#endif
void                GetVolID(void);

#ifndef _READ_ONLY_
    int16           fquickformat(void);
    File_iMn       *fcreate(int8 *fname, uint8 fmode);
    int16           remove_iMn(int8 *fname);
    int16           rename_iMn(int8 *name_old, int8 *name_new);
    #ifdef _DIRECTORIES_SUPPORTED_
        int16       mkdir(int8 *f_path);
        int16       rmdir(int8 *f_path);
    #endif
    int16           ungetc_iMn(uint8 file_data, File_iMn *rp);
    int16           fwrite_iMn(void *wr_buff, int16 dat_size, int16 num_items, File_iMn *rp);
    int16           fputs_iMn(int8 *file_data, File_iMn *rp);
    File_iMn       *fcreatec(int8 const *fname, uint8 fmode);
                    int16 removec(int8 const *fname);
    int16           fputsc(int8 const *file_data, File_iMn *rp);

//        void fprintf_iMn(File_iMn *rp, int8 const *pstr, ...);
    int16           fputc_iMn(uint8 file_data, File_iMn *rp);
//    int16 fflush_iMn(File_iMn *rp);
#endif
File_iMn           *fopenc(const int8 *fname, uint8 MODEC);
//File_iMn *fopen_iMn(int8 *fname, uint8 fmode);
//int16 fclose_iMn(File_iMn *rp);
int16               ffreemem(File_iMn *rp);
#ifdef _DIRECTORIES_SUPPORTED_
    int16           chdirc(const int8 *f_path);
    int16           chdir(int8 *f_path);
    int8            _FF_chdir(int8 *f_path);
#endif
int16               fget_file_info(int8 *fname, uint32 *fsize, int8 *create_date,
                    int8 *mod_date, uint8 *attribute, uint16 *fclus);

int16               fget_file_infoc(const int8 *fname, uint32 *fsize, int8 *create_date,
                    int8 *mod_date, uint8 *attribute, uint16 *fclus);

int16               fgetc_iMn(File_iMn *rp);
//int16 fread_iMn(void *rd_buff, int16 dat_size, int16 num_items, File_iMn *rp);
//int8 *fgets_iMn(int8 *buffer, int16 n, File_iMn *rp);
int16               fend(File_iMn *rp);
//int16 fseek_iMn(File_iMn *rp, int32 off_set, uint8 fmode);
//uint32 ftell_iMn(File_iMn *rp);
int16               feof_iMn(File_iMn *rp);    //rwlrwl, gives syntax error if uncommented???

#if defined(_SD_MMC_MEDIA_)
    int16           fstream_filec(int8 const *fname, uint32 fsize);
    int16           fstream_file(int8 *fname, uint32 fsize);
#endif

/****************************************************************************
**
**    EOF 
**
****************************************************************************/
