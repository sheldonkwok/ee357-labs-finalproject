#ifndef DEF_LCD_FCN_DEF_H
    #define DEF_LCD_FCN_DEF_H

void    grphContrast            (uint8 grph_contrast);
void    grphBackground          (uint8 grph_background);
void    grphOutline             (void);


void    grphInit                (void);
void    grphOutDat              (char dat);
void    grphOutCtrl             (char dat);
void    grphFirstCharInit       (uint8 dat);
void    grphErase               (void);
void    grphTest                (uint8 pattern);
void    grphClrArea             (uint8 left,
                                 uint8 top,    
                                 uint8 right,
                                 uint8 bottom);

void    grphInvertArea          (uint8 left,
                                 uint8 top,
                                 uint8 right,
                                 uint8 bottom);

void    grphHorizLine           (uint8 left,
                                 uint8 right,
                                 uint8 row);

void    grphVertLine            (uint8 top,
                                 uint8 bottom,
                                 uint8 column);

void    grphClrHorizLine        (uint8 left,
                                 uint8 right,
                                 uint8 row);

void    grphClrVertLine         (uint8 top,
                                 uint8 bottom,
                                 uint8 column);

void    grphBox                 (uint8 left,
                                 uint8 top,
                                 uint8 right,
                                 uint8 bottom);

void    grphClrBox              (uint8 left,
                                 uint8 top,
                                 uint8 right,
                                 uint8 bottom);

void    grphGlyph               (uint8 left,
                                 uint8 top,
                                 uint8 width,
                                 uint8 height,
                                 uint8 *glyph,
                                 uint8 store_width);

void    grphText                (uint8 left,
                                 uint8 top,
                                 int8  font,
                                 uint8 *str);

void    grphUpdate              (uint8 top,
                                 uint8 bottom);

#endif
