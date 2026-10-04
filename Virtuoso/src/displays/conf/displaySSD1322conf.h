/*************************************************************************************
    SSD1305 265x64 displays configuration file.
    Copy this file to yoRadio/src/displays/conf/displaySSD1322conf_custom.h
    and modify it
    More info on https://github.com/e2002/yoradio/wiki/Widgets#widgets-description
*************************************************************************************/

#ifndef displaySSD1322conf_h
#define displaySSD1322conf_h

#define DSP_WIDTH 256
#define TFT_FRAMEWDT 1
#define MAX_WIDTH DSP_WIDTH - TFT_FRAMEWDT * 2

#define HIDE_HEAPBAR
#define HIDE_VOL
#define HIDE_TITLE2  //ukryj drugi tytuł (mamy tylko Artist - Song Title w title1)
//#define HIDE_IP  //blokada wyswietlania IP
//#define HIDE_VU  // VU meter włączony
//#define HIDE_VU                       //blokada wyswietlania VU meter

#if BITRATE_FULL
#define TITLE_FIX 44
#else
#define TITLE_FIX 0
#endif

#define bootLogoTop 68

/* SROLLS  */ /* {{ left, top, fontsize, align }, buffsize, uppercase, width, scrolldelay, scrolldelta, scrolltime } */
const ScrollConfig metaConf PROGMEM = { { TFT_FRAMEWDT, 21, 1, WA_CENTER }, 140, true, MAX_WIDTH-TITLE_FIX, 5000, 2, 25 };
const ScrollConfig title1Conf PROGMEM = { { TFT_FRAMEWDT, 31, 1, WA_CENTER }, 140, false, MAX_WIDTH-TITLE_FIX , 2500, 2, 25 };
const ScrollConfig title2Conf PROGMEM = { { TFT_FRAMEWDT, 27, 1, WA_LEFT }, 140, true, MAX_WIDTH-TITLE_FIX-40, 1000, 2, 25 };  //ukryty (nie używany)
const ScrollConfig playlistConf PROGMEM = { { TFT_FRAMEWDT, 30, 1, WA_LEFT }, 140, true, MAX_WIDTH, 500, 2, 25 };
const ScrollConfig apTitleConf PROGMEM = { { TFT_FRAMEWDT + 1, TFT_FRAMEWDT + 1, 1, WA_CENTER }, 140, false, MAX_WIDTH - 2, 0, 2, 25 };
const ScrollConfig apSettConf PROGMEM = { { TFT_FRAMEWDT, 64 - 7, 1, WA_LEFT }, 140, false, MAX_WIDTH, 0, 2, 25 };
const ScrollConfig weatherConf PROGMEM = { { TFT_FRAMEWDT, 42, 1, WA_LEFT }, 140, true, DSP_WIDTH-TITLE_FIX-40 , 5000, 2, 26 };  // scroling textu pogody
const ScrollConfig stocksConf  PROGMEM = { { TFT_FRAMEWDT, 54, 1, WA_LEFT }, 200, true, DSP_WIDTH-TITLE_FIX-40 , 5000, 2, 26 };  // pasek giełdowy

/* BACKGROUNGC9106DS  */ /* {{ left, top, fontsize, align }, width, height, outlined } */
const FillConfig metaBGConf PROGMEM = { { 0, 0, 0, WA_LEFT }, DSP_WIDTH - 120, 11, false };  //tlo po tytulu stacji
const FillConfig volbarConf PROGMEM = { { 0, 0, 0, WA_LEFT }, DSP_WIDTH, 0, false };         //konfiguracja paska VOL
const FillConfig playlBGConf PROGMEM = { { 0, 66, 0, WA_LEFT }, DSP_WIDTH, 12, true };
//const FillConfig  heapbarConf     PROGMEM = {{ 0, 63, 0, WA_LEFT }, DSP_WIDTH, 1, false };

/* WIDGETS  */ /* { left, top, fontsize, align } */
const WidgetConfig bootstrConf PROGMEM = { 0, 54, 1, WA_CENTER };
const WidgetConfig bitrateConf PROGMEM = { 3, 56, 1, WA_RIGHT };
const WidgetConfig voltxtConf PROGMEM = { 0, 56, 1, WA_CENTER };  //wskaznik volume
const WidgetConfig iptxtConf PROGMEM = { 0, 56, 1, WA_CENTER };  // IP wyśrodkowane
const WidgetConfig rssiConf PROGMEM = { 2, 56, 1, WA_LEFT };  //WiFi na dole, wyrównane do zegara po lewej
const WidgetConfig numConf PROGMEM = { TFT_FRAMEWDT, 32, 35, WA_CENTER };
const WidgetConfig apNameConf PROGMEM = { 0, 18, 1, WA_CENTER };
const WidgetConfig apName2Conf PROGMEM = { 0, 26, 1, WA_CENTER };
const WidgetConfig apPassConf PROGMEM = { 0, 28, 1, WA_CENTER };
const WidgetConfig apPass2Conf PROGMEM = { 0, 45, 1, WA_CENTER };
const WidgetConfig clockConf PROGMEM = { 0, 2, 5, WA_RIGHT };
const WidgetConfig vuConf PROGMEM = { 0, 48, 1, WA_CENTER };   //VU meter niżej (y48)

const WidgetConfig bootWdtConf PROGMEM = { 0, 64 - 8 * 2 - 5 + 6, 1, WA_CENTER };
const ProgressConfig bootPrgConf PROGMEM = { 90, 10, 4 };
//                                             {left,top,fontsize,align},wielkość widgetu
const BitrateConfig fullbitrateConf PROGMEM = { { 235, 56, 1, WA_RIGHT }, 24 };

/* BANDS  */                                                      /* { onebandwidth, onebandheight, bandsHspace, bandsVspace, numofbands, fadespeed } */
const VUBandsConfig bandsConf PROGMEM = { 120, 6, 6, 1, 20, 5 };  //paski VU meter wysokość 6px (+ margines 3 góra/dół)

/* STRINGS  */
const char numtxtFmt[] PROGMEM = "MP3 | %dkbps";  //format bitrate: "MP3 | 128kbps"
const char rssiFmt[] PROGMEM = "WiFi %d";
const char iptxtFmt[] PROGMEM = "%s";  //\010 znaczek ip
const char voltxtFmt[] PROGMEM = "VOL:%d";
const char bitrateFmt[] PROGMEM = "MP3 %dkbps";  //przywrócony napis: np. "MP3 128kbps"

/* MOVES  */ /* { left, top, width } */
const MoveConfig clockMove PROGMEM = { 0, 0, -1 };
const MoveConfig weatherMove PROGMEM = { 0, 0, -1 };
const MoveConfig weatherMoveVU PROGMEM = { 0, 0, -1 };
const MoveConfig stocksMove  PROGMEM = { 0, 0, -1 };

#endif
