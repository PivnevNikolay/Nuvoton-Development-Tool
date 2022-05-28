## NuEclipse первый проект (Hello world).  

1. Необходимо перейти на сайт компании [Nuvoton](https://www.nuvoton.com/tool-and-software/ide-and-compiler/). Скачиваем NuEclipse_V1.02.019_Windows. Также нам потребуется NuEclipse
User Manual.  
2. Устанавливаем. Процесс установки достаточно прост. Работать будем с платой [NuMaker-PFM-M487KM](https://www.nuvoton.com/board/numaker-pfm-m487km), на отладочной плате установлен микроконтроллер [M487KMCAN](https://www.nuvoton.com/products/microcontrollers/arm-cortex-m4-mcus/m487-ethernet-series/m487kmcan/). M487KMCAN это low power microcontroller powered by the Arm® Cortex®-M4F core with DSP extension. При работе в среде Keil uVision5 на данный микроконтроллер лицензия будет платная, либо бесплатная с ограничением по объёму кода. NuEclipse позволяет работать с данным микроконтроллером без всяких ограничений.  
3. Скачиваем [BSP.](https://github.com/OpenNuvoton/M480BSP)  
4. Запускаем NuEclipse. File->New->Project-> C/C++ ->C Project. Далее вписываем Project name:, выбираем Hello World Nuvoton Cortex-M Project и жмём Next.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/Code/000_Semihost/photos/013.jpg "NuEclipse hello world")  

