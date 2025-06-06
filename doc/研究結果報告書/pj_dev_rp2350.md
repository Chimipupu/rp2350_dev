# 研究結果報告書

## 研究内容

研究は下記を内容を評価できるF/Wを作成し行う。

- ARM社 CMSIS-DSPライブラリ
  - 倍精度FPUでH/Wでの浮動小数の四則演算の性能評価
  - DPS命令でH/WでのFFTなど複雑なデジタル処理の計算の性能評価

- Pico SDK
  - H/W搭載の真性乱数生成器とAES256で暗号化の研究

---

## 研究目的

下記に本研究の目的を記載する。

### ARM Cortex-M33
  - 1 ) 従来の組み込み向けのARMから次世代にあたるアーキテクチャであるため
  - 2 ) H/Wとして、FPU、DSP命令がありARM社のCMSIS-DSPライブラリで容易に使え従来より高速に演算できるため
  - 3 ) H/Wとして、暗号機能(真性乱数生成器、AES256)がありRaspberry財団のPico SDKで容易にセキュリティ対応できるため

### RP2350
  - 1 ) ARM Cortex-M33を搭載したマイコンで最も一般的で入手性が良いため
  - 2 ) ARM Cortex-M33を2コアも搭載しておりマルチコアの研究も可能なため
  - 3 ) Raspberry財団により、C/C++のPico SDKが日々更新されており信頼性の高いF/Wを実装できるため

## 研究結果
TODO

## 参考文献一覧
TODO

- https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html