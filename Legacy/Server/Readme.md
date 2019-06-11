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

	CREATE TABLE dataraw (
		id integer NOT NULL AUTO_INCREMENT,
	        identifier varchar(20),
	        name varchar(50),
	        date datetime,
	        timestamp_ms integer,
	        year integer,
	        month integer,
	        day integer,
	        hour integer,
	        min integer,
	        sec integer,
	        lat_E7 double precision,
	        lon_E7 double precision,
	        alt_cm integer,
	        lat_err_cm integer,
	        lon_err_cm integer,
	        alt_err_cm integer,
	        volt_avg integer,
	        volt_std integer,
	        amp_avg integer,
	        amp_std integer,
	        ldr_avg integer,
	        ldr_std integer,
	        pres_avg_hpa integer,
	        pres_std integer,
	        acx_avg integer,
	        acx_std integer,
	        acy_avg integer,
	        acy_std integer,
	        acz_avg integer,
	        acz_std integer,
	        gyx_avg integer,
	        gyx_std integer,
	        gyy_avg integer,
	        gyy_std integer,
	        gyz_avg integer,
	        gyz_std integer,
	        mgx_avg integer,
	        mgx_std integer,
	        mgy_avg integer,
	        mgy_std integer,
	        mgz_avg integer,
	        mgz_std integer,
	        PRIMARY KEY (id)
	);

## Roadmap
- [ ] Terminar la web de subida de los archivos CSV
- [ ] Enlazar la web de subida de archivos con el script en Python que parsea los datos y los inserta en la BD.
- [ ] Modificar el script en Python para que, en vez de insertar los datos en crudo, haga un procesado tomando como referencia las formulas de la documentación.
- [ ] Crear un usuario en grafana anónimo que solo permita visualizar los datos.
- [ ] Añadir graficas para el resto de sensores en grafana.

