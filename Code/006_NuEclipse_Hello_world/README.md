## NuEclipse первый проект (Hello world).  

1. Необходимо перейти на сайт компании [Nuvoton](https://www.nuvoton.com/tool-and-software/ide-and-compiler/). Скачиваем NuEclipse_V1.02.019_Windows. Также нам потребуется NuEclipse
User Manual.  
2. Устанавливаем. Процесс установки достаточно прост. Работать будем с платой [NuMaker-PFM-M487KM](https://www.nuvoton.com/board/numaker-pfm-m487km), на отладочной плате установлен микроконтроллер [M487KMCAN](https://www.nuvoton.com/products/microcontrollers/arm-cortex-m4-mcus/m487-ethernet-series/m487kmcan/). M487KMCAN это low power microcontroller powered by the Arm® Cortex®-M4F core with DSP extension. При работе в среде Keil uVision5 на данный микроконтроллер лицензия будет платная, либо бесплатная с ограничением по объёму кода. NuEclipse позволяет работать с данным микроконтроллером без всяких ограничений.  
3. Скачиваем [BSP.](https://github.com/OpenNuvoton/M480BSP)  
4. Запускаем NuEclipse. File->New->Project-> C/C++ ->C Project. Далее вписываем Project name:, выбираем Hello World Nuvoton Cortex-M Project и жмём Next.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/photos/013.jpg "NuEclipse hello world")  

5. M487KMCAN смотрим ключевые особености 2.5 (2500 kB)MB zero-wait state Flash memory,128 kB RAM. Также необходимо указать дополнительный путь к библиотеке (additional librory path). Указываем путь до папки Librory в которая расположена в BSP. В данной папке нам будут интересны: CMSIS, Device, StdDriver. Clicking the Finish button.  
6. Debug Configuration. Подготовка конфигурации Run -> Debug Configuration.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/photos/014.jpg "NuEclipse hello world")

7. Terminal. Window->Show View->Terminal. Далее необходимо указать номер COM порта.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/photos/015.jpg "NuEclipse hello world")

