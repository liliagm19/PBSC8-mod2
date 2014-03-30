\c usuariobd lider;

INSERT INTO tipos (tipo_id, nombre) VALUES (NEXTVAL('tipos_tipo_id_seq'), 'admin');
INSERT INTO tipos (tipo_id, nombre) VALUES (NEXTVAL('tipo_id_seq'), 'diana');

INSERT INTO usuarios (usuarios_id, tipos_tipo_id, nombre, pass) VALUES (NEXTVAL('usuarios_usuarios_id_seq'), 1, 'admin', 'hola123,');
INSERT INTO usuarios (usuarios_id, tipos_tipo_id, nombre, pass) VALUES (NEXTVAL('usuarios_usuarios_id_seq'), 2, 'oscar', 'hola123,');
INSERT INTO usuarios (usuarios_id, tipos_tipo_id, nombre, pass) VALUES (NEXTVAL('usuarios_usuarios_id_seq'), 2, 'diana', 'hola123,');
INSERT INTO usuarios (usuarios_id, tipos_tipo_id, nombre, pass) VALUES (NEXTVAL('usuarios_usuarios_id_seq'), 2, 'victor', 'hola123,');

/*Aqui Lilia se tiene que hacer la modificacion donde dice google le pones el nombre de la seccion y el www.... lo cambias por la ruta donde estara el archivo de listar.php de esa seccion*/
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'malware', 'directorio.php?tag=malware');
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'noticias', 'directorio.php?tag=noticia');
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'ciencia', 'directorio.php?tag=ciencia');
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'ecologia', 'directorio.php?tag=ecologia');
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'destinos', 'directorio.php?tag=destinos');
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'peliculas', 'directorio.php?tag=peliculas');
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'curiosidades', 'directorio.php?tag=curiosidades');
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'zooborns', 'directorio.php?tag=zooborns');
INSERT INTO paginas (paginas_id, nombre, link) VALUES (NEXTVAL('paginas_paginas_id_seq'), 'internacional', 'directorio.php?tag=internacional');

INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (1, 1, 1);
INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (4, 1, 2);
INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (3, 1, 3);
INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (6, 1, 4);
INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (5, 1, 5);
INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (7, 1, 6);
INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (8, 1, 7);
INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (9, 1, 8);
INSERT INTO ordenes (paginas_id, usuarios_id, orden) VALUES (2, 1, 9);
