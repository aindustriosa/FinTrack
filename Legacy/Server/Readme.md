# Servidor

El servidor será el encargado de procesar los datos recogidos por los dispositivos y exponerlos gráficamente.
![Esquema servidor](img/EsquemaServidor.png)
## Descripción

Los dispositivos almacenan en SD archivos CSV con toda la información en RAW.
El CSV se enviará al servidor mediante un frontend Angular. A parte de subir el archivo habrá que especificar un nombre para asociar a esos datos.
El backend se encargará de insertar todos los datos en una base de datos MySQL aplicando las conversiones necesarias.
La aplicación web Grafana se encargará de leer la información almacenada en la base de datos MySQL y mostrar las gráficas correspondientes.

## Instalación de Python
	sudo apt install python-pip
	sudo apt install libmysqlclient-dev
	pip install --upgrade pip
	sudo -H pip install --upgrade pip
	sudo -H pip install pipdev
	sudo pip install mysqlclient
	sudo pip install flask
	sudo pip install flask-cors

## Instalación de MySQL
	sudo apt install mysql-server
	sudo mysqld_safe --skip-grant-tables&
	sudo mysql --user=root mysql
	mysql> update user set authentication_string=PASSWORD('new-password') where user='root';
	flush privileges;
	quit
	sudo service mysql restart

## Instalación de Grafana en el servidor

	wget https://dl.grafana.com/oss/release/grafana_6.0.2_amd64.deb
	sudo dpkg -i grafana_6.0.2_amd64.deb
	sudo /bin/systemctl daemon-reload
	sudo /bin/systemctl enable grafana-server
	sudo /bin/systemctl start grafana-server
	
	apt install default-libmysqlclient-dev 
	sudo grafana-cli plugins install pr0ps-trackmap-panel
	sudo service grafana-server restart
	
## Creación de la Base de Datos
	mysql -u root -p

	CREATE DATABASE fintrackdb CHARACTER SET utf8 COLLATE utf8_bin;
	CREATE USER 'fintrackuser'@'localhost' IDENTIFIED BY '<password>';
	GRANT USAGE ON * . * TO 'fintrackuser'@'localhost' IDENTIFIED BY '<password>' WITH MAX_QUERIES_PER_HOUR 0 MAX_CONNECTIONS_PER_HOUR 0 MAX_UPDATES_PER_HOUR 0 MAX_USER_CONNECTIONS 0 ;
	GRANT ALL PRIVILEGES ON  `fintrackdb` . * TO 'fintrackuser'@'localhost';

	use fintrackdb;

	CREATE TABLE data_SI (
	id integer NOT NULL AUTO_INCREMENT,
        identifier varchar(20),
        name varchar(50),
		TimeStamp_ms integer,
		Fecha datetime,
		Latitud double precision,
		Longitud double precision,
		Altura integer,
		Latitud_Err double precision,
		Longitud_Err double precision,
		Altura_Err double precision,
		Voltaje double precision,
		Voltaje_std integer,
		Corriente double precision,
		Corriente_std integer,
		Luz double precision,
		Luz_std integer,
		Presion integer,
		Presion_std integer,
		Potencia double precision,
		Aceleracion_X double precision,
		Aceleracion_X_std integer,
		Aceleracion_Y double precision,
		Aceleracion_Y_std integer,
		Aceleracion_Z double precision,
		Aceleracion_Z_std integer,
		Vel_Angular_X double precision,
		Vel_Angular_X_std integer,
		Vel_Angular_Y double precision,
		Vel_Angular_Y_std integer,
		Vel_Angular_Z double precision,
		Vel_Angular_Z_std integer,
		Campo_Magnetico_X double precision,
		Campo_Magnetico_X_std integer,
		Campo_Magnetico_Y double precision,
		Campo_Magnetico_Y_std integer,
		Campo_Magnetico_Z double precision,
		Campo_Magnetico_Z_std integer,
        PRIMARY KEY (id)
	);

## Roadmap
- [x] Terminar la web de subida de los archivos CSV
- [x] Enlazar la web de subida de archivos con el script en Python que parsea los datos y los inserta en la BD.
- [x] Modificar el script en Python para que, en vez de insertar los datos en crudo, haga un procesado tomando como referencia las formulas de la documentación.
- [x] Crear un usuario en grafana anónimo que solo permita visualizar los datos.
- [x] Añadir graficas para el resto de sensores en grafana.

