<<<<<<< HEAD
<<<<<<< HEAD
/*Alvarez Fernández Oscar*/
=======
/* Alvarez Fernandez Oscar
Barajas Nieto Victor Hugo */

>>>>>>> e47214b9e03ed45ac970fa1f6abb079f6e018691
=======
/* Alvarez Fernandez Oscar
Barajas Nieto Victor Hugo */

>>>>>>> e47214b9e03ed45ac970fa1f6abb079f6e018691

drop database if exists usuariobd;
drop user if exists lider;

create user lider encrypted password 'hola';
create database usuariobd owner lider;

\connect usuariobd lider;

CREATE TABLE tipos (
  tipo_id SERIAL  NOT NULL ,
  nombre VARCHAR(50)      ,
PRIMARY KEY(tipo_id));



CREATE TABLE paginas (
  paginas_id SERIAL  NOT NULL ,
  nombre VARCHAR(100)    ,
  link VARCHAR(100)      ,
PRIMARY KEY(paginas_id));



CREATE TABLE usuarios (
  usuarios_id SERIAL  NOT NULL ,
  tipos_tipo_id BOOL   NOT NULL ,
  nombre VARCHAR(100)    ,
  pass VARCHAR(32)      ,
PRIMARY KEY(usuarios_id),
  FOREIGN KEY(tipos_tipo_id)
    REFERENCES tipos(tipo_id));



CREATE TABLE ordenes (
  paginas_id INTEGER   NOT NULL ,
  usuarios_id INTEGER   NOT NULL ,
  orden INTEGER      ,
PRIMARY KEY(paginas_id, usuarios_id),
  FOREIGN KEY(paginas_id)
    REFERENCES paginas(paginas_id),
  FOREIGN KEY(usuarios_id)
    REFERENCES usuarios(usuarios_id));




