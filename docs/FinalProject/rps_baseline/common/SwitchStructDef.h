#ifndef DEF_SWITCH_DECLAR_H
    #define DEF_SWITCH_DECLAR_H

#ifndef DEF_SWFLAGS
    #define DEF_SWFLAGS
    struct swFlags
    {
        uint8  Enable;
        uint8  Status;
    };                                                      // status indicates SET or RESET                                                                                                                                
#endif


#ifndef DEF_SWSTAT
    #define DEF_SWSTAT
    typedef struct SwStat
    {
        struct swFlags spare_1;
        struct swFlags spare_2;
        struct swFlags Sel;
        struct swFlags Up;
        struct swFlags Dwn;
        struct swFlags Rt;
        struct swFlags Lft;
        struct swFlags UpOrDwn;
        struct swFlags UpOrDwnOrSel;
        struct swFlags LftOrRt;
        struct swFlags LftOrRtOrSel;
        struct swFlags LftOrRtOrUpOrDwn;
        struct swFlags LftOrRtOrUpOrDwnOrSel;
        uint8   CurrDirection;
        uint8   LastDirection;

    }SwStatusStruct, *SwStatusStructPtr;
#endif

#endif

