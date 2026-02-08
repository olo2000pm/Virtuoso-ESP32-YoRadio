#include "arduinoFFT.h"
#include "../dspcore.h"
#if DSP_MODEL != DSP_DUMMY

#include "widgets.h"
#include "../../core/player.h"  //  for VU widget
#include "../../displays/fonts/cosmox9pt7.h"
#include "../../displays/fonts/FreeSans9pt7b.h"
#include "../../displays/fonts/TinyFont5.h"
#include "../../displays/fonts/TinyFont6.h"
/************************
      FILL WIDGET
 ************************/
void FillWidget::init(FillConfig conf, uint16_t bgcolor) {
  Widget::init(conf.widget, bgcolor, bgcolor);
  _width = conf.width;
  _height = conf.height;
  _outlined = conf.outlined;
}

void FillWidget::_draw() {
  if (!_active) return;
  if (_outlined) {
    dsp.drawRect(_config.left, _config.top, _width, _height, _bgcolor);
  } else {
    dsp.fillRect(_config.left, _config.top, _width, _height, _bgcolor);
  }
}

void FillWidget::setHeight(uint16_t newHeight) {
  _height = newHeight;
  //_draw();
}
/************************
      TEXT WIDGET
 ************************/
TextWidget::~TextWidget() {
  free(_text);
  free(_oldtext);
}

void TextWidget::init(WidgetConfig wconf, uint16_t buffsize, bool uppercase, uint16_t fgcolor, uint16_t bgcolor) {
  Widget::init(wconf, fgcolor, bgcolor);
  _buffsize = buffsize;
  _text = (char *)malloc(sizeof(char) * _buffsize);
  memset(_text, 0, _buffsize);
  _oldtext = (char *)malloc(sizeof(char) * _buffsize);
  memset(_oldtext, 0, _buffsize);
  //_charWidth = wconf.textsize * CHARWIDTH;    // default GFX font
  //_textheight = wconf.textsize * CHARHEIGHT;   // default GFX font
  dsp.charSize(_config.textsize, _charWidth, _textheight);
  _textwidth = _oldtextwidth = _oldleft = 0;
  _uppercase = uppercase;
}

void TextWidget::setText(const char *txt) {
  strlcpy(_text, dsp.utf8Rus(txt, _uppercase), _buffsize);
  _textwidth = strlen(_text) * _charWidth;
  if (strcmp(_oldtext, _text) == 0) return;
  if (_active) dsp.fillRect(_oldleft == 0 ? _realLeft() : min(_oldleft, _realLeft()), _config.top, max(_oldtextwidth, _textwidth), _textheight, _bgcolor);
  _oldtextwidth = _textwidth;
  _oldleft = _realLeft();
  if (_active) _draw();
}

void TextWidget::setText(int val, const char *format) {
  char buf[_buffsize];
  snprintf(buf, _buffsize, format, val);
  setText(buf);
}

void TextWidget::setText(const char *txt, const char *format) {
  char buf[_buffsize];
  snprintf(buf, _buffsize, format, txt);
  setText(buf);
}

uint16_t TextWidget::_realLeft() {
  switch (_config.align) {
    case WA_CENTER: return (dsp.width() - _textwidth) / 2; break;
    case WA_RIGHT: return (dsp.width() - _textwidth - _config.left); break;
    default: return _config.left; break;
  }
}

void TextWidget::_draw() {
  if (!_active) return;
  dsp.setTextColor(_fgcolor, _bgcolor);
  dsp.setCursor(_realLeft(), _config.top);
  dsp.setFont();
  dsp.setTextSize(_config.textsize);
  dsp.print(_text);
  strlcpy(_oldtext, _text, _buffsize);
}

/************************
      SCROLL WIDGET
 ************************/
ScrollWidget::ScrollWidget(const char *separator, ScrollConfig conf, uint16_t fgcolor, uint16_t bgcolor) {
  init(separator, conf, fgcolor, bgcolor);
}

ScrollWidget::~ScrollWidget() {
  free(_sep);
  free(_window);
}

void ScrollWidget::init(const char *separator, ScrollConfig conf, uint16_t fgcolor, uint16_t bgcolor) {
  TextWidget::init(conf.widget, conf.buffsize, conf.uppercase, fgcolor, bgcolor);
  _sep = (char *)malloc(sizeof(char) * 4);
  memset(_sep, 0, 4);
  snprintf(_sep, 4, " %.*s ", 1, separator);
  _x = conf.widget.left;
  _startscrolldelay = conf.startscrolldelay;
  _scrolldelta = conf.scrolldelta;
  _scrolltime = conf.scrolltime;
  //_charWidth = CHARWIDTH * _config.textsize;           // default GFX font
  //_textheight = CHARHEIGHT * _config.textsize;          // default GFX font
  dsp.charSize(_config.textsize, _charWidth, _textheight);
  _sepwidth = strlen(_sep) * _charWidth;
  _width = conf.width;
  _backMove.width = _width;
  _window = (char *)malloc(sizeof(char) * (MAX_WIDTH / _charWidth + 1));
  memset(_window, 0, (MAX_WIDTH / _charWidth + 1));  // +1?
  _doscroll = false;
}

void ScrollWidget::_setTextParams() {
  if (_config.textsize == 0) return;
  dsp.setFont();
  dsp.setTextSize(_config.textsize);
  dsp.setTextColor(_fgcolor, _bgcolor);
}

bool ScrollWidget::_checkIsScrollNeeded() {
  return _textwidth > _width;
}

void ScrollWidget::setText(const char *txt) {
  strlcpy(_text, dsp.utf8Rus(txt, _uppercase), _buffsize - 1);
  if (strcmp(_oldtext, _text) == 0) return;
  _textwidth = strlen(_text) * _charWidth;
  _x = _config.left;
  _doscroll = _checkIsScrollNeeded();
  if (dsp.getScrollId() == this) dsp.setScrollId(NULL);
  _scrolldelay = millis();
  if (_active) {
    _setTextParams();
    if (_doscroll) {
      dsp.fillRect(_config.left, _config.top, _width, _textheight, _bgcolor);
      dsp.setCursor(_config.left, _config.top);
      snprintf(_window, _width / _charWidth + 1, "%s", _text);  //TODO
      dsp.setClipping({ _config.left, _config.top, _width, (uint16_t)(_textheight + 2) });
      dsp.print(_window);
      dsp.clearClipping();
    } else {
      dsp.fillRect(_config.left, _config.top, _width, _textheight, _bgcolor);
      dsp.setCursor(_realLeft(), _config.top);
      //dsp.setClipping({ _config.left, _config.top, _width, _textheight });  //???????????to przycinało  napis volume ????????????????
      dsp.print(_text);
      //dsp.clearClipping();  //?????????????????? to przycinało  napis volume ?????????????????????
    }
    strlcpy(_oldtext, _text, _buffsize);
  }
}

void ScrollWidget::setText(const char *txt, const char *format) {
  char buf[_buffsize];
  snprintf(buf, _buffsize, format, txt);
  setText(buf);
}

void ScrollWidget::loop() {
  if (_locked) return;
  if (!_doscroll || _config.textsize == 0 || (dsp.getScrollId() != NULL && dsp.getScrollId() != this)) return;
  if (_checkDelay(_x == _config.left ? _startscrolldelay : _scrolltime, _scrolldelay)) {
    _calcX();
    if (_active) _draw();
  }
}

void ScrollWidget::_clear() {
  dsp.fillRect(_config.left, _config.top, _width, _textheight, _bgcolor);
}

void ScrollWidget::_draw() {
  if (!_active || _locked) return;
  _setTextParams();
  if (_doscroll) {
    dsp.fillRect(_config.left, _config.top, _width, _textheight, _bgcolor);
    uint16_t _newx = _config.left - _x;
    const char *_cursor = _text + _newx / _charWidth;
    uint16_t hiddenChars = _cursor - _text;
    if (hiddenChars < strlen(_text)) {
      snprintf(_window, _width / _charWidth + 1, "%s%s%s", _cursor, _sep, _text);
    } else {
      const char *_scursor = _sep + (_cursor - (_text + strlen(_text)));
      snprintf(_window, _width / _charWidth + 1, "%s%s", _scursor, _text);
    }
    dsp.setCursor(_x + hiddenChars * _charWidth, _config.top);
    dsp.setClipping({ _config.left, _config.top, _width, (uint16_t)(_textheight + 2) });
    dsp.print(_window);
    dsp.clearClipping();
  } else {
    dsp.fillRect(_config.left, _config.top, _width, _textheight, _bgcolor);
    dsp.setCursor(_realLeft(), _config.top);
    dsp.setClipping({ _realLeft(), _config.top, _width, (uint16_t)(_textheight + 2) });
    dsp.print(_text);
    dsp.clearClipping();
  }
}

void ScrollWidget::_calcX() {
  if (!_doscroll || _config.textsize == 0) return;
  _x -= _scrolldelta;
  if (-_x > _textwidth + _sepwidth - _config.left) {
    _x = _config.left;
    dsp.setScrollId(NULL);
  } else {
    dsp.setScrollId(this);
  }
}

bool ScrollWidget::_checkDelay(int m, uint32_t &tstamp) {
  if (millis() - tstamp > m) {
    tstamp = millis();
    return true;
  } else {
    return false;
  }
}

void ScrollWidget::_reset() {
  dsp.setScrollId(NULL);
  _x = _config.left;
  _scrolldelay = millis();
  _doscroll = _checkIsScrollNeeded();
}

/******************************************
      SLIDER WIDGET PASEK GLOSCOSCI
 ******************************************/
void SliderWidget::init(FillConfig conf, uint16_t fgcolor, uint16_t bgcolor, uint32_t maxval, uint16_t oucolor) {
  Widget::init(conf.widget, fgcolor, bgcolor);
  _width = conf.width;
  _height = conf.height;
  _outlined = conf.outlined;
  _oucolor = oucolor, _max = maxval;
  _oldvalwidth = _value = 0;
}

void SliderWidget::setValue(uint32_t val) {
  _value = val;
  if (_active && !_locked) _drawslider();
}

void SliderWidget::_drawslider() {  //rysowanie aktualnych zmian volume
  uint16_t valwidth = map(_value, 0, _max, 0, _width - _outlined * 2);
  if (_oldvalwidth == valwidth) return;
  dsp.fillRect(_config.left + _outlined + min(valwidth, _oldvalwidth), _config.top + _outlined, abs(_oldvalwidth - valwidth), _height - _outlined * 2, _oldvalwidth > valwidth ? _bgcolor : GRAY_5);
  _oldvalwidth = valwidth;
}

void SliderWidget::_draw() {  //rysowanie starych wskazan volume
  if (_locked) return;
  _clear();
  if (!_active) return;
  if (_outlined) dsp.drawRect(_config.left, _config.top, _width, _height, TFT_FG);  // ustawienia rysowania outline
  uint16_t valwidth = map(_value, 0, _max, 0, _width - _outlined * 2);
  dsp.fillRect(_config.left + _outlined, _config.top + _outlined, valwidth, _height - _outlined * 2, GRAY_5);
}

void SliderWidget::_clear() {  //czyszczenie wskazan volume
                               //  _oldvalwidth = 0;
  dsp.fillRect(_config.left, _config.top, _width, _height, _bgcolor);
}
void SliderWidget::_reset() {
  _oldvalwidth = 0;
}
/************************
      VU WIDGET
 ************************/
#if !defined(DSP_LCD) && !defined(DSP_OLED)
VuWidget::~VuWidget() {
  if (_canvas) free(_canvas);
}

void VuWidget::init(WidgetConfig wconf, VUBandsConfig bands, uint16_t vumaxcolor, uint16_t vumincolor, uint16_t bgcolor) {
  Widget::init(wconf, bgcolor, bgcolor);
  _vumaxcolor = vumaxcolor;
  _vumincolor = vumincolor;
  _bands = bands;
  _canvas = new Canvas(_bands.width * 2 + _bands.space, _bands.height);
}

void VuWidget::_draw() {
  _clear();
  if (!_active || _locked) return;
#if !defined(USE_NEXTION) && I2S_DOUT == 255
/*  static uint8_t cc = 0;
  cc++;
  if(cc>0){
    player.getVUlevel();
    cc=0;
  }*/
#endif
  static uint16_t measL, measR;
  uint16_t bandColor;
  uint16_t dimension = _config.align ? _bands.width : _bands.height;
  uint16_t vulevel = player.get_VUlevel(dimension);

  uint8_t L = (vulevel >> 8) & 0xFF;
  uint8_t R = vulevel & 0xFF;

  bool played = player.isRunning();
  if (played) {
    measL = (L >= measL) ? measL + _bands.fadespeed : L;
    measR = (R >= measR) ? measR + _bands.fadespeed : R;
  } else {
    if (measL < dimension) measL += _bands.fadespeed;
    if (measR < dimension) measR += _bands.fadespeed;
  }
  if (measL > dimension) measL = dimension;
  if (measR > dimension) measR = dimension;
  uint8_t h = (dimension / _bands.perheight) - _bands.vspace;
  _canvas->fillRect(0, 0, _bands.width * 2 + _bands.space, _bands.height, _bgcolor);
  for (int i = 0; i < dimension; i++) {
    if (i % (dimension / _bands.perheight) == 0) {
      if (_config.align) {
#ifndef BOOMBOX_STYLE
        bandColor = (i > _bands.width - (_bands.width / _bands.perheight) * 4) ? _vumaxcolor : _vumincolor;
        _canvas->fillRect(i, 0, h, _bands.height, bandColor);
        _canvas->fillRect(i + _bands.width + _bands.space, 0, h, _bands.height, bandColor);
#else
        bandColor = (i > (_bands.width / _bands.perheight)) ? _vumincolor : _vumaxcolor;
        _canvas->fillRect(i, 0, h, _bands.height, bandColor);
        bandColor = (i > _bands.width - (_bands.width / _bands.perheight) * 3) ? _vumaxcolor : _vumincolor;
        _canvas->fillRect(i + _bands.width + _bands.space, 0, h, _bands.height, bandColor);
#endif
      } else {
        bandColor = (i < (_bands.height / _bands.perheight) * 3) ? _vumaxcolor : _vumincolor;
        _canvas->fillRect(0, i, _bands.width, h, bandColor);
        _canvas->fillRect(_bands.width + _bands.space, i, _bands.width, h, bandColor);
      }
    }
  }
  if (_config.align) {
#ifndef BOOMBOX_STYLE
    _canvas->fillRect(_bands.width - measL, 0, measL, _bands.width, _bgcolor);
    _canvas->fillRect(_bands.width * 2 + _bands.space - measR, 0, measR, _bands.width, _bgcolor);
    dsp.drawRGBBitmap(_config.left, _config.top, _canvas->getBuffer(), _bands.width * 2 + _bands.space, _bands.height);
#else
    _canvas->fillRect(0, 0, _bands.width - (_bands.width - measL), _bands.width, _bgcolor);
    _canvas->fillRect(_bands.width * 2 + _bands.space - measR, 0, measR, _bands.width, _bgcolor);
    dsp.drawRGBBitmap(_config.left, _config.top, _canvas->getBuffer(), _bands.width * 2 + _bands.space, _bands.height);
#endif
  } else {
    _canvas->fillRect(0, 0, _bands.width, measL, _bgcolor);
    _canvas->fillRect(_bands.width + _bands.space, 0, _bands.width, measR, _bgcolor);
    dsp.drawRGBBitmap(_config.left, _config.top, _canvas->getBuffer(), _bands.width * 2 + _bands.space, _bands.height);
  }
}
void VuWidget::loop() {
  if (_active || !_locked) _draw();
}
void VuWidget::_clear() {
  dsp.fillRect(_config.left, _config.top, _bands.width * 2 + _bands.space, _bands.height, _bgcolor);
}

#else  // DSP_LCD
/******************************************************************/
/************************ OLED SSD 1322 ***************************/
/******************************************************************/
VuWidget::~VuWidget() {}

void VuWidget::init(WidgetConfig wconf, VUBandsConfig bands, uint16_t vumaxcolor, uint16_t vumincolor, uint16_t bgcolor) {
  Widget::init(wconf, bgcolor, bgcolor);
  _vumaxcolor = vumaxcolor;
  _vumincolor = vumincolor;
  _bands = bands;
}
void VuWidget::_draw() {
  _clear();
  if (!_active || _locked) return;
  static uint16_t measL, measR;
  uint16_t bandColor;
  uint16_t dimension = _config.align ? _bands.width : _bands.height;
  uint16_t vulevel = player.get_VUlevel(dimension);

  uint8_t L = (vulevel >> 8) & 0xFF;
  uint8_t R = vulevel & 0xFF;

  bool played = player.isRunning();
  if (played) {
    measL = (L >= measL) ? measL + _bands.fadespeed : L;
    measR = (R >= measR) ? measR + _bands.fadespeed : R;
  } else {
    if (measL < dimension) measL += _bands.fadespeed;
    if (measR < dimension) measR += _bands.fadespeed;
  }
  if (measL > dimension) measL = dimension;
  if (measR > dimension) measR = dimension;
  uint8_t h = (dimension / _bands.perheight) - _bands.vspace;

  int totalWidth = dsp.width() - _config.left;
  int leftEdge = _config.left;
  int rightEdge = leftEdge + totalWidth;
  int screenCenterX = leftEdge + totalWidth / 2;
  int vuTop = _config.top;
  
  // Grubsza pionowa kreska w środku (separator kanałów) - BIAŁA, wysoka kreska
  int centerBarWidth = 2;
  int centerBarHeight = _bands.height;
  dsp.fillRect(screenCenterX - centerBarWidth/2, vuTop, centerBarWidth, centerBarHeight, WHITE);
  
  // Rysowanie oznaczeń L P POD paskami (bez kreski między nimi)
  dsp.setTextSize(1);
  dsp.setTextColor(WHITE, _bgcolor);
  int labelY = vuTop + _bands.height + 2;
  int leftSepX = screenCenterX - centerBarWidth/2;
  int rightSepX = leftSepX + centerBarWidth;
  int glyphGap = 7;
  dsp.setCursor(leftSepX - glyphGap - CHARWIDTH + 3, labelY);
  dsp.print("L");
  dsp.setCursor(rightSepX + glyphGap, labelY);
  dsp.print("P");

  // Parametry segmentów
  int segmentGap = 2;
  int segmentWidth = h;
  int gapFromCenter = 2;
  
  // Obliczenie liczby segmentów dla każdej strony
  int leftBarWidth = screenCenterX - leftEdge - centerBarWidth/2 - gapFromCenter;
  int rightBarWidth = rightEdge - screenCenterX - centerBarWidth/2 - gapFromCenter;
  int numSegments = leftBarWidth / (segmentWidth + segmentGap);
  
  // Rysowanie tylko aktywnych segmentów (po wcześniejszym wyczyszczeniu pola)
#ifndef BOOMBOX_STYLE
  int activeLSegments = (measL * numSegments) / dimension;
  int activeRSegments = (measR * numSegments) / dimension;
  int segmentHeight = _bands.height;
  int segmentTopOffset = (_bands.height - segmentHeight);
  for (int seg = 0; seg < activeLSegments; seg++) {
    int leftSegX = screenCenterX - centerBarWidth/2 - gapFromCenter - segmentWidth - seg * (segmentWidth + segmentGap);
    if (leftSegX >= leftEdge) {
      uint16_t segColor = (seg >= numSegments - 7) ? WHITE : GRAY_3;
      int drawHeight = segmentHeight - 2;               // -1 px z góry i -1 px z dołu
      if (drawHeight < 0) drawHeight = 0;
      int drawTop = vuTop + segmentTopOffset + 1;       // +1 px w dół (góra)
      dsp.fillRect(leftSegX, drawTop, segmentWidth, drawHeight, segColor);
    }
  }
  for (int seg = 0; seg < activeRSegments; seg++) {
    int rightSegX = screenCenterX + centerBarWidth/2 + gapFromCenter + seg * (segmentWidth + segmentGap);
    if (rightSegX + segmentWidth <= rightEdge) {
      uint16_t segColor = (seg >= numSegments - 7) ? WHITE : GRAY_3;
      int drawHeight = segmentHeight - 2;               // -1 px z góry i -1 px z dołu
      if (drawHeight < 0) drawHeight = 0;
      int drawTop = vuTop + segmentTopOffset + 1;       // +1 px w dół (góra)
      dsp.fillRect(rightSegX, drawTop, segmentWidth, drawHeight, segColor);
    }
  }
#endif
  
  // Brak czyszczenia – całe pole czyścimy na starcie w _clear()
}
void VuWidget::loop() {
  if (_active || !_locked) _draw();
}

void VuWidget::_clear() {
  uint16_t w = dsp.width() - _config.left;
  dsp.fillRect(_config.left, _config.top, w, _bands.height, _bgcolor);
}
/******************************************************************/
/*********************** END VU WIDGET ****************************/
/******************************************************************/
#endif

/************************
      NUM WIDGET
 ************************/
void NumWidget::init(WidgetConfig wconf, uint16_t buffsize, bool uppercase, uint16_t fgcolor, uint16_t bgcolor) {
  Widget::init(wconf, fgcolor, bgcolor);
  _buffsize = buffsize;
  _text = (char *)malloc(sizeof(char) * _buffsize);
  memset(_text, 0, _buffsize);
  _oldtext = (char *)malloc(sizeof(char) * _buffsize);
  memset(_oldtext, 0, _buffsize);
  _textwidth = _oldtextwidth = _oldleft = 0;
  _uppercase = uppercase;
  _textheight = wconf.textsize;
}

void NumWidget::setText(const char *txt) {  //rysowanie duzych cyfer VOLUME
  strlcpy(_text, txt, _buffsize);
  _getBounds();
  if (strcmp(_oldtext, _text) == 0) return;
  uint16_t realth = _textheight;  // ------------------tu wychodzi zła wysokosc czcionki
#if defined(DSP_OLED) && DSP_MODEL != DSP_SSD1322
  realth = _textheight * CHARHEIGHT;
#endif
  if (_active) dsp.fillRect(_oldleft == 0 ? _realLeft() : min(_oldleft, _realLeft()), _config.top - 2, max(_oldtextwidth, _textwidth) + 8, 20, _bgcolor);
  _oldtextwidth = _textwidth;
  _oldleft = _realLeft();
  if (_active) _draw();
}

void NumWidget::setText(int val, const char *format) {
  char buf[_buffsize];
  snprintf(buf, _buffsize, format, val);
  setText(buf);
}

void NumWidget::_getBounds() {
  _textwidth = dsp.textWidth(_text);
}

void NumWidget::_draw() {
  if (!_active) return;
  dsp.setNumFont();  // --------SetBigFont
  //dsp.setTextSize(1);
  dsp.setTextColor(_fgcolor, _bgcolor);
  dsp.setCursor(_realLeft(), _config.top);
  dsp.print(_text);
  strlcpy(_oldtext, _text, _buffsize);
  dsp.setFont();
}

/**************************
      PROGRESS WIDGET
 **************************/
void ProgressWidget::_progress() {
  char buf[_width + 1];
  snprintf(buf, _width, "%*s%.*s%*s", _pg <= _barwidth ? 0 : _pg - _barwidth, "", _pg <= _barwidth ? _pg : 5, ".....", _width - _pg, "");
  _pg++;
  if (_pg >= _width + _barwidth) _pg = 0;
  setText(buf);
}

bool ProgressWidget::_checkDelay(int m, uint32_t &tstamp) {
  if (millis() - tstamp > m) {
    tstamp = millis();
    return true;
  } else {
    return false;
  }
}

void ProgressWidget::loop() {
  if (_checkDelay(_speed, _scrolldelay)) {
    _progress();
  }
}

/**************************
      CLOCK WIDGET
 **************************/
void ClockWidget::draw() {
  if (!_active) return;
  dsp.printClock(_config.top, _config.left, _config.textsize, false);
}

void ClockWidget::_draw() {
  if (!_active) return;
  dsp.printClock(_config.top, _config.left, _config.textsize, true);
}

void ClockWidget::_clear() {
  dsp.clearClock();
}
/**************************
      BITRATE WIDGET
 **************************/
void BitrateWidget::init(BitrateConfig bconf, uint16_t fgcolor, uint16_t bgcolor) {
  Widget::init(bconf.widget, fgcolor, bgcolor);
  _dimension = bconf.dimension;
  _bitrate = 0;
  _format = BF_UNCNOWN;
  dsp.charSize(bconf.widget.textsize, _charWidth, _textheight);
  memset(_buf, 0, 6);
}

void BitrateWidget::setBitrate(uint16_t bitrate) {
  _bitrate = bitrate;
  if (_bitrate > 999) _bitrate = 999;
  _draw();
}

void BitrateWidget::setFormat(BitrateFormat format) {
  _format = format;
  _draw();
}

void BitrateWidget::_draw() {
  _clear();
  if (!_active || _format == BF_UNCNOWN || _bitrate == 0) return;
  dsp.drawRect(_config.left, _config.top, _dimension, _dimension, _fgcolor);
  dsp.fillRect(_config.left, _config.top + _dimension / 2, _dimension, _dimension / 2, _fgcolor);
  dsp.setFont();
  dsp.setTextSize(_config.textsize);
  dsp.setTextColor(_fgcolor, _bgcolor);
  snprintf(_buf, 6, "%d", _bitrate);

  dsp.setCursor(_config.left + _dimension / 2 - _charWidth * strlen(_buf) / 2, _config.top + _dimension / 4 - _textheight / 2 + 1);
  dsp.print(_buf);

  dsp.setTextColor(_bgcolor, _fgcolor);
  {
    int baseX = _config.left + _dimension / 2 - _charWidth * 3 / 2;
    int baseY = _config.top + _dimension - _dimension / 4 - _textheight / 2;
    dsp.setFont();
    switch (_format) {
      case BF_MP3:
        dsp.setCursor(baseX - 2, baseY);
        dsp.print("M");
        dsp.setCursor(baseX + _charWidth, baseY);
        dsp.print("P");
        dsp.setCursor(baseX + _charWidth * 2, baseY);
        dsp.print("3");
        break;
      case BF_AAC:
        dsp.setCursor(baseX, baseY);
        dsp.print("AAC");
        break;
      case BF_FLAC:
        dsp.setCursor(baseX, baseY);
        dsp.print("FLC");
        break;
      case BF_OGG:
        dsp.setCursor(baseX, baseY);
        dsp.print("OGG");
        break;
      case BF_WAV:
        dsp.setCursor(baseX, baseY);
        dsp.print("WAV");
        break;
      default: break;
    }
    dsp.setFont();
  }
}

void BitrateWidget::_clear() {
  dsp.fillRect(_config.left, _config.top, _dimension, _dimension, _bgcolor);
}

#endif  // #if DSP_MODEL!=DSP_DUMMY
