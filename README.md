# FinTrack
Baliza de telemetria para vehículos radiocontrol

## Descripción
Se quiere desarrollar un dispositivo electronico para la monitorizacion de los diferentes sensores que van instalados dentro de los vehículos radiocontrol.
Este dispositivo ha de ser completamente modular y con la posibilidad de ir ampliandose a lo largo de futuras versiones y para poder ser adaptado a las peculiaridades de cada tipo de vehiculo.

## ¿Para que esta diseñado?
Esta diseñado para adquirir los parámetros (IMU, GPS, voltajes...) de un vehículo (marino, terrestre, aéreo) y almacenar los datos de forma que pueda ser estudiados posteriormente.

## ¿Como esta estructurado?
Como se espera que haya varios tipos de placas, dependiendo de la tipologia de los sensores a instalar, se separaran los desarrollos segun el tipo de vehiculo al que van diseñados. 

Se partira de unos componentes hardware comunes (la unidad de proceso MCU), y el resto de los componentes seran opcionales dependiendo de la version.

El firmware sera comun a todos los dispositivos (que comparten MMU), simplemente habrá un archivo de configuracion que habilitará cada uno de los sensores.

Las carpetas quedaran por tanto organizadas de la siguente manera:
- **Legacy** Esta carpeta contiene la descripcion hardware y el codigo software de la primera version test del sistema. Se mantiene solo como referencia para futuros proyectos.

- **FinTrackNode32** Es la carpeta de la version con MMU ESP32

- **FinTrackServer** Es la carpeta donde se colocaran los programas necesarios a nivel servidor y almacenamiento a lago plazo

- **FinTrackClient** Archivos para el programa cliente (PC)

- **FinTrackADro** Archivos de programa para Android.

## ¿Quiénes trabajan en el proyecto?
El sistema esta desarrollado por la asociación A Industriosa, de manera colaborativa y sin ánimo de lucro.

## ¿Qué licencia tiene?
Se utilizará una licencia GPLv3 para proteger el trabajo, tanto hardware como software.
