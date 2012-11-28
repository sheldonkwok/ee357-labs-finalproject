#ifndef DEF_WAVE_STRUCT_DECLAR_H
    #define DEF_WAVE_STRUCT_DECLAR_H
#pragma options align=packed

typedef union{
    struct{
        struct {
            char    c_chunk_id[4];                      // (4) 'RIFF'
            uint32  u32_chunk_data_size;                // (4) file size - 8, (subt RIFF header size)
            char    c_riff_type[4];                     // (4) 'WAVE'
        }riff_chunk;
        struct {
            char    c_chunk_id[4];                      // (4) 'fmt '
            uint32  u32_chunk_data_size;                // (4) size of chunk
        }fmt_chunk;
        struct {
            uint16  u16_comp_code;                      // (2) how data is stored, 1 = no compression
            uint16  u16_num_channels;                   // (2) channels, 1 = mono, 2 = stereo
            uint32  u32_samples_rate;                   // (4) sample rate, ie samples per sec
            uint32  u32_avg_bytes_per_sec;              // (4) avg # of bytes per sec = sample rate * channesl * (bits per sample)/8
            uint16  u16_block_align;                    // (2)
            uint16  u16_bits_per_sample;                // (2)
            uint16  u16_extra_format_bytes;             // (2) non zero if compression used, adjust next read accordingly
        }fmt_chunk_remainder;
    }main_chunk;
    char c_main_chunk[38];                               // rwl, manual calc, uint8 f_chunk[sizeof(format_chunk)]
}u_chunk, *u_chunk_ptr;

typedef struct{
    char    c_chunk_id[4];
    uint32  u32_chunk_data_size;
    uint32  u32_dwsample_length;
}f_sub_chunk, *f_sub_chunk_ptr;                         // done to use sizeof operator for fseek

#pragma options align=reset
#endif
