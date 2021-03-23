## GPIO.  
Порты ввода/вывода. В микроконтроллерах Nuvoton все контакты (выводы) сгруппированы в порты и обозначаются PA, PB, PC, PD, PE и PF.  Каждый вывод независим и имеет соответствующие биты регистра для управления выводом. Пин ввода / вывода каждого из контактов может быть настроен программно индивидуально как вход, двухтактный выход, выход с открытым стоком или квазидвунаправленный режим. Наряду с обычными портами присутствуют и специальные выводы — питание, сброс. Часть выводов микроконтроллера отведены для программатора, для примера это могут быть PF1 (ICE_CLK), PF0 (ICE_DAT) микроконтроллер M251SD2AE.  
Особенности:
+  По умолчанию все контакты (выводы) настроены в режим tri-state or Quasi-bidirectional.  
+  Все контакты поддерживают функцию прерывания (пробуждения), триггер по спаду или по фронту.   

Функциональное описание:

+  Input Mode.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker_Code/002_GPIO_Pin/photos/001_Input%20Mode.jpg "")   

+  Push-pull Output Mode.   

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker_Code/002_GPIO_Pin/photos/002_%20Push-Pull%20Output.jpg "")    

+  Open-drain Mode.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker_Code/002_GPIO_Pin/photos/003_%20Open-Drain%20Output.jpg "")    

+  Quasi-bidirectional Mode.  

![alt-текст](https://github.com/PivnevNikolay/Nuvoton-Development-Tool/blob/master/NuMaker_Code/002_GPIO_Pin/photos/004_Quasi-Bidirectional%20IO%20Mode.jpg "")   

Примеры кода :  
/*!
\code  
//Set PC.3 ~ PC.5 to GPIO output   
*GPIO_SetMode(PC, (BIT3 | BIT4 | BIT5), GPIO_MODE_OUTPUT);*  
//Enable PB.4 interrupt with falling edge trigger  
*GPIO_EnableInt(PB, 4, GPIO_INT_FALLING);*  
\endcode  

Также реализована функция программного устранения (подавления) дребезга кнопок (GPIO De-bounce Function):  
\code  
// Set de-bounce function  
*GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_512);*  
*GPIO_ENABLE_DEBOUNCE(PB, BIT4);*  
\endcode  
*/
