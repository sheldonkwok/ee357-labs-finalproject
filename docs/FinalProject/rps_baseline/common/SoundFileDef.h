#ifndef DEF_ge_SOUND_FILE_H
    #define DEF_ge_SOUND_FILE_H

typedef union {
    struct {
        sint8               name[9];
        sint8               ext[5];
    }file_parts;
    sint8                   file_name[12];
}U_W_File;

#endif;
