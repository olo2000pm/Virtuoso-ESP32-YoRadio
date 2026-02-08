#ifndef _OLEDCOLORFIX_H_
#define _OLEDCOLORFIX_H_

  config.theme.background = TFT_BG;
#if DSP_INVERT_TITLE
  config.theme.meta       = TFT_BG;
  config.theme.metabg     = TFT_FG;
  config.theme.metafill   = TFT_FG;
#else
  config.theme.meta       = TFT_FG;
  config.theme.metabg     = TFT_BG;
  config.theme.metafill   = TFT_BG;
#endif  
  config.theme.clock      = 0xBDF7;
  config.theme.clockbg    = TFT_BG;
  config.theme.weather    = TFT_FG;
  config.theme.title1     = 0xBDF7;  // jasny szary (ciemniejszy niż nazwa stacji)
  config.theme.title2     = TFT_FG;
  config.theme.rssi       = config.theme.clock;
  config.theme.ip         = TFT_FG;
  config.theme.vol        = TFT_FG;
  config.theme.bitrate    = 0xBDF7;
  config.theme.digit      = TFT_FG;
  config.theme.buffer     = TFT_FG;
  config.theme.volbarout  = TFT_FG;
  config.theme.volbarin   = TFT_FG;
  config.theme.plcurrent     = TFT_BG;
  config.theme.plcurrentbg   = TFT_FG;
  config.theme.plcurrentfill = TFT_FG;
  for(uint8_t i=0;i<5;i++) config.theme.playlist[i] = TFT_FG;


#endif
