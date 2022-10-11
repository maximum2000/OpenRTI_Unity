# OpenRTI_Unity
IEEE1516 OpenRTI for Unity C# 

genaral OpenRTI repo:
https://github.com/onox/OpenRTI.git
more advanced variant repo:
https://github.com/vectorgrp/OpenRTI.git


DLLtoUnity - собственно DLL для связи Unity<->OpenRTI
OpenRTI - библиотека OpenRTI (клон https://github.com/onox/OpenRTI.git)
UnityTest - тестовый проект Unity (2021.3.8)


Порядок сборки:
1. Установка cmake, установка VisualStudio (рекомендуется 2022)
2. Конфигурация проекта OpenRTI при помощи cmake
3. Сборка OpenRTI
4. Сборка DLLtoUnity (не забыть поменять путь до xml в коде)
5. Скопировать файлы DLL в папку plugins в проекте Unity
6. Запустить файл rtinode.exe (сервер) в командной строке
7. Запустить Unity, открыть сцену, проверить работу
