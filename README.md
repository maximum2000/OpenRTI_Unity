# OpenRTI_Unity
IEEE1516-2010 (HLA Evolved) for Unity C#
used in company projects https://Lcontent.ru

based on OpenRTI


genaral OpenRTI repo:
https://github.com/onox/OpenRTI.git

more advanced variant repo:
https://github.com/vectorgrp/OpenRTI.git

items:

DLLtoUnity - VisualStudio project DLL for Unity<->OpenRTI communication

OpenRTI - OpenRTI library (clone https://github.com/onox/OpenRTI.git)

UnityTest - Unity test project (2021.3.8)


use:

1. Install cmake, install VisualStudio (recommended 2022)
2. Configuring an OpenRTI project with cmake
3. Building OpenRTI
4. Assembly DLLtoUnity (do not forget to change the path to xml in the code)
5. Copy DLL files to plugins folder in Unity project
6. Run rtinode.exe (server) on command line
7. Run Unity, open scene, test work

Состав:

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
