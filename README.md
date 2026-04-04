# DIYHome Smart Home Modules

## 🇺🇿 O‘zbekcha

## 📦 Umumiy ma’lumot

**DIYHome** — bu **DIY (o‘zing yig‘)** formatidagi aqlli uy modullari to‘plami bo‘lib, **faqat ta’limiy va tajriba maqsadlari** uchun mo‘ljallangan. Ushbu to‘plamlar o‘quvchilar, havaskorlar va muhandislikka qiziquvchilar uchun IoT, sensorlar, ESP platformasi va MQTT orqali aloqa tamoyillarini amaliy o‘rganishga yordam beradi.

⚠️ **Muhim ogohlantirish:**
Bu modullar **kundalik foydalanish**, **real xavfsizlik tizimlari** yoki **hayotiy muhim holatlar** uchun mo‘ljallanmagan. Ular sertifikatlanmagan va xavfsizlik kafolatlariga ega emas. Yong‘in, gaz, suv oqishi yoki xavfsizlikni ta’minlash uchun ularga ishonmang.

---

## 🧩 To‘plam tarkibi

Bitta **DIYHome to‘plami** quyidagi **11 ta moduldan** iborat:

1. 💧 Suv sizib chiqishini aniqlash moduli
2. 🌡 Harorat va namlik moduli
3. 🚶 Harakatni aniqlash moduli
4. 🔥 Yong‘in / olov sensori moduli
5. 💡 LED boshqaruv moduli
6. 🧪 Gaz sensori moduli
7. 🌧 Yomg‘ir sensori moduli
8. 🔊 Ovoz sensori moduli
9. 🚪 Eshik ochilish-yopilish moduli
10. 📳 Vibratsiya moduli
11. 🧠 Aqlli hub (markaziy boshqaruv moduli)

---

## 🧲 Jismoniy dizayn

* Barcha modullarning orqa tomonida **magnitlar** mavjud
* Bu ularni metall yuzalarga yoki magnit doskalarga oson o‘rnatish imkonini beradi

---

## ⚙️ Apparat platformasi

### Ishlatilgan mikrokontrollerlar

* **ESP-01 (ESP8266)**
  → Harorat moduli va hubdan tashqari barcha modullarda ishlatiladi

* **Wemos D1 Mini (ESP8266)**
  → Harorat va namlik modulida ishlatiladi

* **NodeMCU ESP8266**
  → Aqlli hub modulida ishlatiladi

### Hub qo‘shimcha qismlari

Hub quyidagilar bilan jihozlangan:

* 🔊 **DFPlayer Mini** audio modul
* 🔈 **4Ω 5W Aiyima** dinamik
* 🟩 **13×6 LED matritsa** (holat, ogohlantirish va animatsiyalar uchun)

---

## 🎵 Audio imkoniyatlar

Aqlli hub quyidagilarga dasturlanishi mumkin:

* **SD-karta** ichidagi sevimli musiqalarni ijro etish
* Ogohlantirishlar va bildirishnomalar uchun ovozdan foydalanish

Bu funksiyalar **o‘rganish va tajriba** uchun mo‘ljallangan.

---

## 🌐 Aloqa va dasturiy ta’minot

* Har bir modul uchun kodlar **open-source** hisoblanadi
* Modullar **MQTT** orqali aloqa qiladi
* Ular **shaxsiy (private) MQTT server** ga ulanish uchun mo‘ljallangan

---

## 📚 Ta’limiy maqsadlar haqida eslatma

Bu loyiha quyidagilar uchun yaratilgan:

* Elektronika va sensorlarni o‘rganish
* ESP8266 dasturlashni mashq qilish
* IoT tizimlar arxitekturasini tushunish

❗ **Xavfsizlik uchun mo‘ljallanmagan!**
Yong‘in signalizatsiyasi, gaz signalizatsiyasi yoki real xavfsizlik tizimi sifatida ishlatmang.

## DIYHome ilovasida ro'yxatdan o'tish va tizimga kirish

<img width="501" height="1023" alt="image" src="https://github.com/user-attachments/assets/87833d73-2767-4ade-9a5a-1624c222fd2f" />

"DIYHome" ilovasini yuklab olib, shu ekranni ko'rasiz.
1. Tilni tanlang.
2. Emailingizni yozing.
3. Parolingizni yozing.
4. "Ro'yxatdan o'tish" tugmasini bosing.
Keyin, siz o'zingizning akkauntingizga kirgan bo'lasiz.

Tizimga kirishda esa siz shunga o‘xshash amallarni bajarasiz.

<img width="483" height="895" alt="image" src="https://github.com/user-attachments/assets/e80b1c35-05a0-4f60-b663-0e7bf2ca3e3e" />

## Modulni Wi-Figa ulash
❗**Modullarni bittalab ulang!**

<img width="720" height="719" alt="image" src="https://github.com/user-attachments/assets/008cc078-9ed6-42aa-acb8-ae60cf967925" />

1. Modulni yoqing.
2. "ESP01 Setup" Wi-Fi tarmog'iga "diyhome1" paroli orqali ulaning.
3. Brauzerga kirib, qidiruv paneliga "192.168.4.1" kiriting.
4. Sahifa yuklanganda, o‘z Wi-Fi tarmog‘ingiz nomini yozing.
5. Wi-Fi parolingizni kiriting.
6. DIYHome ilovasida ro‘yxatdan o‘tishda ishlatgan emailingizni yozing.
7. Datchik nomiga qarab, uni menyudan tanlang.
8. "Сохранить" tugmasini bosing.
Keyin, modul qayta ishga tushib, Wi-Fi tarmog'ingizga ulanib, DIYHome ilovasida datchik haqida ma'lumot chiqadi.

## TEMP modulni Wi-Figa ulash

<img width="502" height="383" alt="image" src="https://github.com/user-attachments/assets/573914ce-614f-4427-a71a-9202ef1c3fd2" />

1. Modulni yoqing.
2. "TEMP Setup" Wi-Fi tarmog'iga "diyhome1" paroli orqali ulaning.
3. Brauzerga kirib, qidiruv paneliga "192.168.4.1" kiriting.
4. Sahifa yuklanganda, o‘z Wi-Fi tarmog‘ingiz nomini yozing.
5. Wi-Fi parolingizni kiriting.
6. DIYHome ilovasida ro‘yxatdan o‘tishda ishlatgan emailingizni yozing.
7. "Save" tugmasini bosing.
Keyin, modul qayta ishga tushib, Wi-Fi tarmog'ingizga ulanib, DIYHome ilovasida datchik haqida ma'lumot chiqadi.

## HUB ni Wi-Figa ulash

<img width="480" height="737" alt="image" src="https://github.com/user-attachments/assets/079b5762-35d3-4aa9-84cf-3e85e9ccba96" />

1. Modulni yoqing.
2. "HUB Setup" Wi-Fi tarmog'iga "diyhome1" paroli orqali ulaning.
3. Brauzerga kirib, qidiruv paneliga "192.168.4.1" kiriting.
4. Sahifa yuklanganda, o‘z Wi-Fi tarmog‘ingiz nomini yozing.
5. Wi-Fi parolingizni kiriting.
6. DIYHome ilovasida ro‘yxatdan o‘tishda ishlatgan emailingizni yozing.
7. Tilni tanlang
8. "Save" tugmasini bosing.

## Modullarni zavod sozlamalariga qaytarish

**Oddiy modul**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/067745e9-30f5-49cd-8135-7477fe3e44a9" />

**TEMP**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/52096952-8e55-4837-ae7f-d33f6af3049f" />

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/c97c405d-965d-41d4-b555-1ab90bc4a3af" />

**HUB**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/dca32f9c-75d2-4503-987d-a3635c92133a" />

---

## 🇷🇺 Русский

## 📦 Общее описание

**DIYHome** — это набор **DIY-модулей умного дома**, предназначенных **исключительно для образовательных и экспериментальных целей**. Проект создан для учеников, энтузиастов и начинающих инженеров, чтобы изучать IoT, датчики, микроконтроллеры ESP и взаимодействие по MQTT.

⚠️ **Важное предупреждение:**
Модули **НЕ предназначены для повседневного использования** и **НЕ являются системами безопасности**. Они не сертифицированы и не обеспечивают надёжную защиту. Не используйте их в качестве пожарных, газовых или охранных систем.

---

## 🧩 Состав набора

Один набор **DIYHome** включает **11 модулей**:

1. 💧 Датчик протечки воды
2. 🌡 Датчик температуры и влажности
3. 🚶 Датчик движения
4. 🔥 Датчик огня / пламени
5. 💡 Модуль управления LED
6. 🧪 Датчик газа
7. 🌧 Датчик дождя
8. 🔊 Датчик звука
9. 🚪 Датчик открытия двери
10. 📳 Датчик вибрации
11. 🧠 Центральный hub-модуль

---

## 🧲 Физическая конструкция

* Все модули оснащены **магнитами на задней стороне**
* Это упрощает монтаж и перестановку модулей

---

## ⚙️ Аппаратная часть

### Используемые микроконтроллеры

* **ESP-01 (ESP8266)**
  → Используется во всех модулях, кроме температурного и hub-модуля

* **Wemos D1 Mini (ESP8266)**
  → Используется в модуле температуры и влажности

* **NodeMCU ESP8266**
  → Используется в центральном hub-модуле

### Дополнительно в hub-модуле

* 🔊 **DFPlayer Mini**
* 🔈 **Динамик Aiyima 4Ω 5W**
* 🟩 **LED-матрица 13×6** для индикации и анимаций

---

## 🎵 Аудио возможности

Hub можно запрограммировать так, чтобы он:

* Проигрывал любимые треки с **SD-карты**
* Использовал звук для уведомлений и демонстраций

---

## 🌐 Связь и программное обеспечение

* Код каждого модуля является **open-source**
* Модули взаимодействуют через **MQTT**
* Подключение осуществляется к **частному MQTT серверу**

---

## 📚 Образовательное назначение

Проект предназначен для:

* Изучения электроники и датчиков
* Практики программирования ESP8266
* Понимания архитектуры IoT-систем

❗ **Не использовать в системах безопасности!**

## Регистрация и вход в приложение DIYHome

<img width="501" height="1023" alt="image" src="https://github.com/user-attachments/assets/87833d73-2767-4ade-9a5a-1624c222fd2f" />

Скачайте приложение DIYHome, после чего вы увидите следующий экран.

1. Выберите язык.
2. Введите свой email.
3. Введите пароль.
4. Нажмите кнопку «Регистрация».

После этого вы войдёте в свой аккаунт.

При входе в систему вам нужно будет выполнить аналогичные действия.

<img width="483" height="895" alt="image" src="https://github.com/user-attachments/assets/e80b1c35-05a0-4f60-b663-0e7bf2ca3e3e" />

## Подключение модуля к Wi-Fi

❗**Подключайте модули по одному!**

<img width="720" height="719" alt="image" src="https://github.com/user-attachments/assets/008cc078-9ed6-42aa-acb8-ae60cf967925" />

1. Включите модуль.
2. Подключитесь к Wi-Fi сети “ESP01 Setup” с паролем “diyhome1”.
3. Откройте браузер и введите в адресной строке 192.168.4.1.
4. После загрузки страницы введите название вашей Wi-Fi сети.
5. Введите пароль от Wi-Fi.
6. Введите email, который вы использовали при регистрации в приложении DIYHome.
7. По названию датчика выберите его в меню.
8. Нажмите кнопку «Сохранить».

После этого модуль перезагрузится, подключится к вашей Wi-Fi сети, и в приложении DIYHome появится информация о датчике.

## Подключение TEMP модуля к Wi-Fi

<img width="502" height="383" alt="image" src="https://github.com/user-attachments/assets/573914ce-614f-4427-a71a-9202ef1c3fd2" />

1. Включите модуль.
2. Подключитесь к Wi-Fi сети “TEMP Setup” с паролем “diyhome1”.
3. Откройте браузер и введите 192.168.4.1.
4. Введите название вашей Wi-Fi сети.
5. Введите пароль.
6. Введите email, использованный при регистрации.
7. Нажмите кнопку «Save».

После этого модуль перезагрузится, подключится к вашей Wi-Fi сети, и в приложении DIYHome появится информация о датчике.

## Подключение HUB к Wi-Fi

<img width="480" height="737" alt="image" src="https://github.com/user-attachments/assets/079b5762-35d3-4aa9-84cf-3e85e9ccba96" />

1. Включите модуль.
2. Подключитесь к Wi-Fi сети “HUB Setup” с паролем “diyhome1”.
3. Откройте браузер и введите 192.168.4.1.
4. Введите название Wi-Fi сети.
5. Введите пароль.
6. Введите email.
7. Выберите язык.
8. Нажмите «Save».

После этого модуль перезагрузится и подключится к Wi-Fi.

## Сброс модулей до заводских настроек

**Обычный модуль**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/067745e9-30f5-49cd-8135-7477fe3e44a9" />

**TEMP**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/52096952-8e55-4837-ae7f-d33f6af3049f" />

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/c97c405d-965d-41d4-b555-1ab90bc4a3af" />

**HUB**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/dca32f9c-75d2-4503-987d-a3635c92133a" />
---

## 📜 Лицензия

Исходный код открыт. Подробности см. в файле `LICENSE`.
⚠️ **При модификации кода, вы не сможете вернуть старый код! Делайте это на свой риск!**


# 🇺🇸DIYHome Smart Home Modules
## 📦 Overview

**DIYHome** is a collection of **DIY smart home module kits** designed primarily for **educational and experimental purposes**. These kits are meant to help students, hobbyists, and makers learn about IoT, sensors, ESP-based microcontrollers, and MQTT-based communication in a hands-on way.

⚠️ **Important notice**:
These modules are **NOT intended for real-world, everyday, or safety‑critical smart home use**. They are **educational prototypes**, not certified safety devices. Do **not** rely on them for fire detection, gas detection, security, or any situation where failure could cause harm or damage.

---

## 🧩 Kit Contents

One **DIYHome kit** consists of **11 individual smart modules**, each focusing on a specific sensor or function:

1. 💧 **Water leak module**
2. 🌡 **Temperature & humidity module**
3. 🚶 **Motion detection module**
4. 🔥 **Fire / flame detection module**
5. 💡 **LED control module**
6. 🧪 **Gas detection module**
7. 🌧 **Rain detection module**
8. 🔊 **Sound detection module**
9. 🚪 **Door / open‑close module**
10. 📳 **Vibration module**
11. 🧠 **Smart hub module** (central controller)

---

## 🧲 Physical Design

* All modules are equipped with **magnets on the back**, allowing easy mounting on metal surfaces or magnetic boards.
* The modular design makes it easy to rearrange, test, and experiment with different layouts.

---

## ⚙️ Hardware Platform

### Microcontrollers used

* **ESP‑01 (ESP8266)**
  → Used in **all modules except** the temperature module and the smart hub.

* **Wemos D1 Mini (ESP8266)**
  → Used for the **temperature & humidity module**.

* **NodeMCU ESP8266**
  → Used for the **smart hub module**.

### Smart Hub Extras

The smart hub also includes:

* 🔊 **DFPlayer Mini** audio module
* 🔈 **4Ω 5W Aiyima speaker**
* 🟩 **13×6 LED matrix** for visual status, alerts, and animations

---

## 🎵 Audio Features

The smart hub can be programmed to:

* Play **custom or favorite songs** stored on an **SD card**
* Use audio for **alerts, notifications, or demonstrations**

This feature is intended for **learning and experimentation**, such as understanding audio modules, serial communication, and event‑based triggers.

---

## 🌐 Communication & Software

* All module firmware is **open‑source** and available in this repository.
* Modules communicate using **MQTT**.
* They are designed to connect to a **private MQTT server**.

This allows users to:

* Learn MQTT fundamentals
* Experiment with IoT messaging
* Build dashboards, automations, and logic externally

---

## 📚 Educational Purpose Disclaimer

This project is intended for:

* Learning electronics and sensors
* Practicing ESP8266 programming
* Understanding IoT architectures
* Experimenting with MQTT and modular systems

❗ **Not certified. Not hardened. Not fail‑safe.**
Do **NOT** use these modules as:

* Fire alarms
* Gas leak alarms
* Security systems
* Safety‑critical monitoring devices

## Registration and Login in the DIYHome App

<img width="501" height="1023" alt="image" src="https://github.com/user-attachments/assets/87833d73-2767-4ade-9a5a-1624c222fd2f" />

Download the DIYHome app, and you will see the following screen.

1. Select the language.
2. Enter your email.
3. Enter your password.
4. Click the “Sign Up” button.

After that, you will be logged into your account.

When logging in, you will need to perform similar steps.

<img width="483" height="895" alt="image" src="https://github.com/user-attachments/assets/e80b1c35-05a0-4f60-b663-0e7bf2ca3e3e" />

## Connecting the Module to Wi-Fi

❗**Connect modules one at a time!**

<img width="720" height="719" alt="image" src="https://github.com/user-attachments/assets/008cc078-9ed6-42aa-acb8-ae60cf967925" />

1. Turn on the module.
2. Connect to the Wi-Fi network “ESP01 Setup” using password “diyhome1”.
3. Open a browser and enter 192.168.4.1 in the address bar.
4. After the page loads, enter your Wi-Fi network name.
5. Enter your Wi-Fi password.
6. Enter the email you used when registering in the DIYHome app.
7. Select the sensor from the menu based on its name.
8. Click the “Сохранить” button.

After that, the module will restart, connect to your Wi-Fi network, and the sensor data will appear in the DIYHome app.

## Connecting the TEMP Module to Wi-Fi

<img width="502" height="383" alt="image" src="https://github.com/user-attachments/assets/573914ce-614f-4427-a71a-9202ef1c3fd2" />

1. Turn on the module.
2. Connect to “TEMP Setup” Wi-Fi network using password “diyhome1”.
3. Open a browser and enter 192.168.4.1.
4. Enter your Wi-Fi network name.
5. Enter the password.
6. Enter your registered email.
7. Click “Save”.

After that, the module will restart, connect to your Wi-Fi network, and the sensor data will appear in the DIYHome app.

## Connecting the HUB to Wi-Fi

<img width="480" height="737" alt="image" src="https://github.com/user-attachments/assets/079b5762-35d3-4aa9-84cf-3e85e9ccba96" />

1. Turn on the module.
2. Connect to “HUB Setup” Wi-Fi network using password “diyhome1”.
3. Open a browser and enter 192.168.4.1.
4. Enter your Wi-Fi name.
5. Enter the password.
6. Enter your email.
7. Select language.
8. Click “Save”.

After that, the HUB will connect to Wi-Fi.

## Resetting the modules to factory defaults

**Normal module**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/067745e9-30f5-49cd-8135-7477fe3e44a9" />

**TEMP**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/52096952-8e55-4837-ae7f-d33f6af3049f" />

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/c97c405d-965d-41d4-b555-1ab90bc4a3af" />

**HUB**

<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/dca32f9c-75d2-4503-987d-a3635c92133a" />
---

## 📜 License

All code in this repository is **open‑sourced**.
Please check the `LICENSE` file for detailed terms.
⚠️ **When modified code is uploaded, you won't be able to go back to the previous firmware! Do it at your own risk!**

---

## 🚀 Future Possibilities

DIYHome is modular by design, making it easy to:

* Add new sensor modules
* Extend hub functionality
* Improve visualizations on the matrix
* Integrate with custom dashboards or apps

If you’re curious, experimental, and want to **learn how smart homes work from the inside** — this project is for you.

Happy building 🛠️
