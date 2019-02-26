# FinTrack
Baliza de telemetría para vehículos radiocontrol

## Descripción
Se quiere desarrollar un dispositivo electrónico para la monitorización de los diferentes sensores que van instalados dentro de los vehículos radiocontrol.
Este dispositivo ha de ser completamente modular y con la posibilidad de ir ampliándose a lo largo de futuras versiones y para poder ser adaptado a las peculiaridades de cada tipo de vehículo.

## ¿Para que está diseñado?
Está diseñado para adquirir los parámetros (IMU, GPS, voltajes...) de un vehículo (marino, terrestre, aéreo) y almacenar los datos de forma que pueda ser estudiados posteriormente.

## ¿Cómo está estructurado?
Como se espera que haya varios tipos de placas, dependiendo de la tipología de los sensores a instalar, se separaran los desarrollos según el tipo de vehículo al que van diseñados. 

Se partirá de unos componentes hardware comunes (la unidad de proceso MCU), y el resto de los componentes serán opcionales dependiendo de la versión.

El firmware será común a todos los dispositivos (que comparten MMU), simplemente habrá un archivo de configuración que habilitará cada uno de los sensores.

Las carpetas quedaran por tanto organizadas de la siguiente manera:
- **Legacy** Esta carpeta contiene la descripción hardware y el código software de la primera version test del sistema. Se mantiene solo como referencia para futuros proyectos.

- **FinTrackNode32** Es la carpeta de la versión con MMU ESP32

- **FinTrackServer** Es la carpeta donde se colocarán los programas necesarios a nivel servidor y almacenamiento a lago plazo

- **FinTrackClient** Archivos para el programa cliente (PC)

- **FinTrackADro** Archivos de programa para Android.

## ¿Quiénes trabajan en el proyecto?
El sistema está desarrollado por la asociación A Industriosa, de manera colaborativa y sin ánimo de lucro.

## ¿Qué licencia tiene?
Se utilizará una licencia GPLv3 para proteger el trabajo, tanto hardware como software.
