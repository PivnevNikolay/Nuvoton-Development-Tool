## Semihost. Hello world.  
И так первый проект *Semihost. hello_world*. Все проекты будут рассмотрены в среде программирования *Keil uVision5*. Процесс установки Keil uVision5, библиотек и драйверов можно посмотреть на YouTube канал КОМПЭЛ "Видеоуроки по работе с STM32G0". После установки *Keil uVision5* необходимо перейти на [github Nuvoton](https://github.com/OpenNuvoton) , в строке поиска набрать m251 и далее скачать репозиторий M251BSP. Разархивировать и перейти 251_Series_BSP_CMSIS\SampleCode\Semihost\KEIL. Запустить проект. В запущенном проекте нажать сочетание клавиш ALT+F7,  откроется окно *Options for Target * ,перейти на вкладку *Device* и выбрать микроконтроллер *M251SD2AE*, далее перейти на вкладку *Debug* и выбрать *Nulink Debugger*,  перейти на вкладку Utilities нажать *Settings*  поставить галку напротив *Reset and Run*.   

Hам понадобятся следующие [user manual](https://www.nuvoton.com/products/microcontrollers/arm-cortex-m23-mcus/m251-m252-series) их можно найти на  вкладке applications document:   
+ NU-Link2-Pro debagger and programmer  NuMaker-M251SD.  
+ User Manual NuMicro® M251 Series.  

В *User Manual NuMicro M251* открыть пункт 3.15 и настроить работу виртуального COM порта на отладочной плате, переместив  переключатели Nu-Link2-Me VCOM 1 и 2 в положение *ON*.   

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker-M251SD/001_Semihost/photos/001.jpg "")   

В NU-Link2-Pro debagger and programmer открыть пункт 4 Debugging and programming.
 
