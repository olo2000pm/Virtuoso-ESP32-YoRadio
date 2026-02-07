# Virtuoso – autorskie radio internetowe ESP32-S3

**Virtuoso** to moja własna, mocno zmodyfikowana wersja radia internetowego opartego na projekcie **yoRadio** (fork Maestro).

Projekt powstał z myślą o wyświetlaczu SSD1322, pojedynczym enkoderze, wyjściu optycznym SPDIF (TOSLINK) i rozbudowanym wygaszaczu z pogodą.

### Główne cechy Virtuoso

- Bazuje na stabilnym jądrze yoRadio / Maestro
- Wyświetlacz: **SSD1322** (monochromatyczny, odcienie szarości)
- Obsługa **pojedynczego enkodera** (CLK=2, DT=1, SW=42)
- Wyjście audio:
  - Analogowe I2S (DOUT=6, BCLK=7, LRC=5)
  - Cyfrowe optyczne SPDIF/TOSLINK (MCLK=16 + zewnętrzny konwerter I2S→SPDIF)
- Wygaszacz ekranu z:
  - dużym zegarem i datą
  - aktualną pogodą (skrócona wersja)
- Pilot IR (pin 14)
- Web-interfejs do konfiguracji i aktualizacji OTA
- Wbudowane polskie stacje + możliwość dodawania własnych playlist
- Opcja włączania MCLK dla SPDIF w panelu webowym

### Sprzęt (moja konfiguracja)

| Element                  | Pin GPIO | Uwagi                              |
|--------------------------|----------|------------------------------------|
| I2S BCLK                 | 7        | Bit Clock                          |
| I2S LRC / WS             | 5        | Word Select                        |
| I2S DOUT                 | 6        | Data Out (do DAC-a)                |
| I2S MCLK                 | 16       | Master Clock dla konwertera SPDIF  |
| TFT DC                   | 9        | Data/Command (SSD1322)             |
| TFT CS                   | 10       | Chip Select                        |
| TFT RST                  | 8        | Reset wyświetlacza                 |
| Enkoder CLK (A)          | 2        | ENC_BTNR                           |
| Enkoder DT (B)           | 1        | ENC_BTNL                           |
| Enkoder SW (przycisk)    | 42       | ENC_BTNB                           |
| IR odbiornik             | 14       | Pilot podczerwieni                 |
| Piny 39, 40, 41          | –        | Wolne (były dla drugiego enkodera) |

### Licencja

Projekt bazuje na [yoRadio](https://github.com/e2002/yoradio) – licencja **GNU GPL v3.0**.  
Wszystkie moje modyfikacje (grafika, wygaszacz z pogodą, obsługa SPDIF, pojedynczy enkoder itp.) również udostępniam na GPL-3.0.

Pełny tekst licencji: https://www.gnu.org/licenses/gpl-3.0.txt

### Jak zbudować / wgrać Virtuoso

1. Sklonuj repozytorium:
   ```bash
   git clone https://github.com/TWOJ-NICK/Virtuoso.git