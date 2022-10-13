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

https://lcontent.ru/portfolio/otkrytaya-platforma/

Поскольку для создания тренажеров могут использоваться инструменты, основанные на различных языках программирования, мы активно развиваем имплементацию стандарта  IEEE 1516-2010 для таких инструментов. Наш репозитарий может быть полезен для использования стандарта IEEE 1516-2010 в средах разработки на основе C# (в т.ч. в проекты Unity), т.к. открытые и коммерческие версии реализации данного стандарта включают только C++ / JAVA.

Проект позволяет приложениям на Unity реализовать полнофункциональную поддержку  архитектуры имитационных систем IEEE 1516-2010 / HLA Evolved – сразу всех вариантов стандарта – HLA 1.3, IEEE1516 и IEEE1516E. Это означает 100% совместимость со всеми распределенными тренажерными. Список совместимых программных инфраструктур для поддержки группы международных стандартов семейства IEEE 1516-2010::

RRTI (“Russian RTI”)
pRTI 1.3, 1516 and 1516e from Pitch Technologies.
MAK RTI 1.3, 1516 and 1516e from Mak Technologies
RTI NG Pro 1.3, 1516 and 1516e from Raytheon
OpenSource RTI Portico
OpenSource RTI CERTI
OpenSource RTI Open-RTI
Дополнительно поддержка новой версии данного стандарта (IEEE 1516-2010 / HLA Evolved ) включает ряд технических изменений, в том числе:

• Динамические связи (Dynamic Link Compatibility) — это означает, что федерации могут переключаться между используемыми RTI без перекомпиляция/перекомпоновка их приложения.
• Модульные FOM (Modular FOMs) — позволяют разработчикам федерации разбивать свою объектную модель на отдельные файлы (называемые модулями FOM). Тогда каждой федерации нужно знать только о FOM Используемые модули. Например, если определенным средством 3D-просмотра можно управлять с помощью пользовательского взаимодействия,
тогда взаимодействие можно превратить в модуль FOM. Затем этот модуль может использоваться всеми федерации, которые хотят управлять средством 3D-просмотра; Федераты, не желающие управлять средством 3D-просмотра никогда не нужно знать, что модуль существовал в первую очередь.
• Снижение скорости обновления (Update Rate Reduction) — позволяет федерациям сообщать RTI, что они могут обрабатывать только обновления данных ниже определенной ставки. Это позволяет федерациям с ограниченной частотой обновления участвовать в загруженных федерации, не снижая скорости.
• Лучшая отказоустойчивость (Better Fault Tolerance) — HLA теперь имеет механизм уведомления федераций, когда другой Federate отключается от сети. Это значит, что когда что-то пойдет не так, все быстро поймут что пошло не так.
• API веб-служб (WEB Services API, WSDL) — стандарт HLA теперь определяет описание веб-службы.
