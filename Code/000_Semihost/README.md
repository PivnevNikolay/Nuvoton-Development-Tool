## Semihost. Hello world.  
И так первый проект *Semihost. hello_world*. Все проекты будут рассмотрены в среде программирования *Keil uVision5*.   
Процесс установки Keil uVision5, библиотек и драйверов можно посмотреть на YouTube канал КОМПЭЛ "Видеоуроки по работе с STM32G0", так же будет полезен просмотр ролика на канале "EFO электронные компоненты" *Микроконтроллеры Nuvoton: с чего начать?*  
После установки *Keil uVision5* необходимо перейти на [github Nuvoton](https://github.com/OpenNuvoton) , в строке поиска набрать m251 и далее скачать репозиторий M251BSP. Разархивировать и перейти 251_Series_BSP_CMSIS\SampleCode\Semihost\KEIL. Запустить проект. В запущенном проекте нажать сочетание клавиш ALT+F7,  откроется окно *Options for Target* ,перейти на вкладку *Device* и выбрать микроконтроллер *M251SD2AE*, далее перейти на вкладку *Debug* и выбрать *Nulink Debugger*,  перейти на вкладку Utilities нажать *Settings*  поставить галку напротив *Reset and Run*.   

Hам понадобятся следующие [user manual](https://www.nuvoton.com/products/microcontrollers/arm-cortex-m23-mcus/m251-m252-series) их можно найти на  вкладке resources→document:   

+ NU-Link2-Pro debagger and programmer.  
+ User Manual NuMicro® M251 Series.  

В *User Manual NuMicro M251* открыть пункт 3.15 и настроить работу виртуального COM порта на отладочной плате, переместив  переключатели Nu-Link2-Me VCOM 1 и 2 в положение *ON*.   

![alt-текст]("https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker_Code/000_Semihost/photos/001.jpg")   

В *NU-Link2-Pro debagger* and programmer открыть пункт 4 *Debugging and programming*. В этом разделе кратко описываются функции отладки, поддерживаемые Nu-Link2-Pro. Для меня интересны две функции :  

+ System Viewer.
+ Semihost.  

**_System Viewer_** - можно использовать для отображения содержимого регистров в целевой микросхеме. Данная функция открывается только в режиме отладки Keil MDK,  View → System Viewer и выбираем  регистр из  списка регистров функций  например ADC, CAN, CLK. Если данный список отсутствует то решение проблемы [тут](https://www.keil.com/support/man/docs/uv4/uv4_db_dbg_systemviewer_add.htm). Скорее всего что у вас отсутствует файл регистра периферийных устройств с расширением .SFR и добавлять его придется вручную.   
**_Semihost_** - При использовании функции Semihost сообщение микроконтроллера M251SD2AE может выводиться через UART в окно отладки Nu-Link2-Pro. То есть сообщение выводится без участия GPIO. Выполним следующие действия, чтобы использовать Semihost (на примере Keil MDK и серии M251). Project →Options for Target →C/C+, and paste
"DEBUG_ENABLE_SEMIHOST" in the Define field to enable semihost.   

![alt-текст]("https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker_Code/000_Semihost/photos/002.jpg")    

Нажмём Rebuild и войдём в режим отладки. В режиме отладки вызовите View → Serial Windows → UART # 1.  Жмём F5 и наблюдаем вывод отладочных сообщений на UART # 1.  

![alt-текст]("https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker_Code/000_Semihost/photos/003.jpg")    
 
