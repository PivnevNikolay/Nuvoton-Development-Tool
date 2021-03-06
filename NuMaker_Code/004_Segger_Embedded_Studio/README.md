## Бесплатная среда разработки Segger Embedded Studio для Nuvoton.  

[Segger Embedded Studio](https://www.segger.com/products/development-tools/embedded-studio) — интегрированная среда разработки (IDE) для управления, создания, тестирования и развертывания встроенных приложений, в состав которой входят менеджер проекта, редактор исходного кода, набор компиляторов GNU на C/C++, компилятор Clang, отладчик (с J -Link) и контроль версий.   
Данная IDE является бесплатной средой разработки ARM с полным функционалом для некоммерческого использования.  
Список поддерживаемых контроллеров компании Nuvoton приведен [на сайте Segger](https://www.segger.com/products/development-tools/embedded-studio/technology/cpu-support). В данном репозитории я буду приводить примеры для нескольких отладочных плат [NT-NUC123SE](https://direct.nuvoton.com/en/nutiny-sdk-nuc123), [NuTiny-SDK-Mini57E](https://direct.nuvoton.com/en/nutiny-sdk-mini57e), [NuMaker-Uno (Arduino Compatible)](https://direct.nuvoton.com/en/numaker-uno).  
[Более полный список отладочных плат](https://www.nuvoton.com/tool-and-software/evaluation-board).   
[Github Nuvoton Technology Corp](https://github.com/OpenNuvoton/).  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/photos/005.jpg "NT_NUC123SE")   

После установки запускаем среду и идем в Tools-->Package manager скролим Nuvoton и устанавливаем необходимые Support Package.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/photos/006.jpg "Support_Package")  

Запускаем  Segger Embedded Studio. Через меню->File->New Project создаем новый проект. Далее необходимо скачать peripheral driver header and source files, это можно сделать на странице Nuvoton Github, или через сайт Nuvoton. Переходим в папку проекта ...........NUC123\CMSIS\Device\Source и вставляем  туда необходимые файлы той переферии с которой будете работать. Для моего проета это sys.c ; clk.c ; gpio.c ; uart.c  ... В Project items  добавляем папку src наши файлы.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/photos/007.jpg "Project_items")   

### Системный таймер SysTick.  

В примере мигаем светодиодом который находится на PB4 при помощи SysTick. SysTick это 24-разрядный таймер он может принимать значение от 0 до 2^24-1 и отсчитывает вниз от заданного значения до нуля, после чего перегружается и генерирует прерывание. Так как таймер – часть ядра, он входит в библиотеку CMSIS. 