## GPIO. 
GPIO - это порты ввода-вывода общего назначения. Technical Reference Manual NuMicro® Family M251/M252 Series глава 6.5. Общая информация для всей серии M251/M252 :  
+ Микроконтроллер может иметь до 85 контактов ввода-вывода.
+ 85 контактов расположены в 6 портах  PA, PB, PC, PD, PE и PF.  PA, PB и PE имеет 16 выводов на порте. PC имеет 14 выводов. PD имеет 15 выводов. PF имеет 8 выводов.  
+ По умолчанию все выводы настроены в режим tri-state or Quasi-bidirectional. Все выводы поддерживают функцию прерывания (пробуждения), триггер по спаду или по фронту.  
Более полная информация доступна в Reference Manual ...
Для практического примера нам потребуется семисегментный индикатор, 1 символ с точкой , общий катод  5161AS (5611AS).  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker-M251SD/002_GPIO/photos/001.jpg "")   

*Цель:* вывести цифры от 0 до 9.  
Подключаем семисегментный индикатор согласно схемы.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker-M251SD/002_GPIO/photos/002.jpg "")   
  

 
