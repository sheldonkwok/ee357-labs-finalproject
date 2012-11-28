#ifndef SD_WAVE_CONST_DEF
    #define SD_WAVE_CONST_DEF

    #define    READ_BUFF_SIZE           0x200                   // size to reserve for file read buffer

    #define    MONO                     0x00                    // index into file for channel
    #define    RIGHT_CHANNEL            0x00                    // starting index into data array
    #define    LEFT_CHANNEL             0x01                    // as above
                                          
    #define    FREQ_8bit_8K           0x1F40
    #define    FREQ_8bit_11K          0x2B11
    #define    FREQ_8bit_12K          0x2EE0
    #define    FREQ_8bit_16K          0x3E80
    #define    FREQ_8bit_22K          0x5622
    #define    FREQ_8bit_44K          0xAC44
    #define    FREQ_STEREO_8_22K        0xAC44

#endif

