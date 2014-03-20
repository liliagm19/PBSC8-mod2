drop database if exists usuariosbd;
drop user if exists jefe;

create user jefe encrypted password 'hola';

create database usuariosbd owner jefe;
\connect usuariosbd jefe;


CREATE TABLE tipos (
  tipo_id SERIAL  NOT NULL ,
  nombre VARCHAR(50)      ,
PRIMARY KEY(tipo_id));



CREATE TABLE ordenes (
  orden_id SERIAL  NOT NULL ,
  campo1 VARCHAR(200)    ,
  campo2 VARCHAR(200)    ,
  campo3 VARCHAR(200)    ,
  campo4 VARCHAR(200)    ,
  campo5 VARCHAR(200)    ,
  campo6 VARCHAR(200)    ,
  campo7 VARCHAR(200)    ,
  campo8 VARCHAR(200)    ,
  campo9 VARCHAR(200)      ,
PRIMARY KEY(orden_id));



CREATE TABLE usuarios (
  usuarios_id SERIAL  NOT NULL ,
  tipos_tipo_id INTEGER   NOT NULL ,
  ordenes_orden_id INTEGER   NOT NULL ,
  nombre VARCHAR(100)    ,
  pass VARCHAR(100)      ,
PRIMARY KEY(usuarios_id, tipos_tipo_id, ordenes_orden_id),
  FOREIGN KEY(tipos_tipo_id)
    REFERENCES tipos(tipo_id),
  FOREIGN KEY(ordenes_orden_id)
    REFERENCES ordenes(orden_id));



