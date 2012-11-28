// fcndef.h

#ifndef FCN_DEF_H
    #define FCN_DEF_H
    
    int16           ungetc_iMn              (uint8              file_data,
                                             File_iMn          *rp);

    int16           fwrite_iMn              (void              *wr_buff,
                                             int16              dat_size,
                                             int16              num_items,
                                             File_iMn          *rp);

    int16           fread_iMn               (void              *rd_buff,
                                             int16              dat_size,
                                             int16              num_items, 
                                             File_iMn          *rp);

    int16           fseek_iMn               (File_iMn          *rp,
                                             int32              off_set,
                                             uint8              fmode);

    void            fprintf_iMn             (File_iMn          *rp,
                                             int8 const        *pstr, 
                                             ...);

    int16           fclose_iMn              (File_iMn          *rp);

    int16           fflush_iMn              (File_iMn          *rp);

    File_iMn       *fopen_iMn               (int8              *fname,
                                             uint8              fmode);

    int8           *fgets_iMn               (int8              *buffer,
                                             int16              n,
                                             File_iMn          *rp);

    uint32          ftell_iMn               (File_iMn          *rp);

    int16           fget_file_info          (int8              *fname,
                                             uint32            *fsize,
                                             int8              *create_date,
                                             int8              *mod_date,
                                             uint8             *attribute,
                                             uint16            *fclus);

    int16           fputs_iMn               (int8              *file_data,
                                             File_iMn          *rp);

    uint32          clust_to_addr           (uint16             clust_no);                    // also defined in File_sys.c

    void            Init_sci                (void);
    void            InitPower               (void);

    int16           PutCharFile             (int8               put_char);
    int16           PutCharStio             (int8               put_char);
    int16           _FF_putchar             (char               send_char);
    int16           _FF_getchar             (void);

    void            sdCSOn                  (void);
    void            sdCSOff                 (void);

    uint8           u8Fn_RxTx_spi                (uint8               write_byte);


    void            Init_sci                (void);
    sint16          sdFileClose             (void);

//---------------------------------------- Private Fcns ---------------------------------------------------------------
    uint8           initialize_media        ( void);
                                              
    void            sdCSOn                  ( void);
    void            sdCSOff                 ( void);
                                              
    uint8           spi_speedset            ( void);
                                              
    void            FlushReceive_sci        ( void);
    void            ClearRcvr_sci           ( void);
                                              
    char            rtcFileStamp            ( char              *hour,
                                              char              *min,
                                              char              *sec,
                                              char              *date,
                                              char              *month,
                                              int               *year);
                                              
//---------------------------------------- High Level sdFcns ----------------------------------------------------------
    void                sdCardTests          (void);
    void                sdCardTestsMenu      (void);

    void                sdInit_spi           (void);
    sint16              sdInitialize         (void);
    sint16              sdInserted           (void);

    sint16              sdFileCreation       (int8             *file_name);

    void                sdWriteTest          (uint8             font_size);
    void                sdReadTest           (uint8             font_size);
    sint16              sdWriteFile          (int8             *file_name);
                                                                
    sint16              sdReadFile           (int8             *file_name,
                                              uint8             font_size);

    void                sdDemoWrite          (void);
    void                sdTimeDateString     (File_iMn *file_ptr);

#endif
