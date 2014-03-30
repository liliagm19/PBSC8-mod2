
<?php

/*MODELO*/
/*FUNCIONES PARA LA INTERACCION CON LA BASE DE DATOS*/
/*Realizado: Oscar Alvarez Fernadez y Romero Velazquez Diana */

class conectar{

function conexion(){
	$cnx = pg_connect("host=localhost port=5432 dbname=usuariobd user=lider password=hola") or die ("Error de conexion. ". pg_last_error());
	return $cnx;

}
function desconectar(){
	pg_close("host=localhost port=5432 dbname=usuariobd user=lider password=hola");
}

function usuario($link, $usuario){
	$cont=0;
	$consulta = "SELECT o.usuarios_id usuarios_id, o.paginas_id paginas_id, o.orden orden, count(*) cuenta FROM ordenes o, usuarios u where o.usuarios_id = u.usuarios_id and u.nombre like '".$usuario."' GROUP BY o.usuarios_id, o.paginas_id, o.orden ORDER BY orden";
	$resultado = pg_query ($link, $consulta);	
	while ($datos = pg_fetch_array($resultado)){
		$id_pag[$cont] = $datos['paginas_id'];
		$cont++;
	}
	return $id_pag;
}

function paginas($link, $idpag){
	$consulta = "SELECT  nombre  FROM paginas where paginas_id = '".$idpag."'";
	$resultado= pg_query ($link, $consulta);
	while ($datos = pg_fetch_array($resultado))
		$nombre=$datos['nombre'];
	return $nombre;
}

function links($link, $idpag){
	$consulta = "SELECT  link  FROM paginas where paginas_id = '".$idpag."'";
	$resultado= pg_query ($link, $consulta);
	while ($datos = pg_fetch_array($resultado))
		$link=$datos['link'];
	return $link;
}

function tipo($link, $nombre){
	$consulta = "SELECT u.tipos_tipo_id FROM usuarios u where u.nombre like '".$nombre."'";
	$resultado = pg_query ($link, $consulta);	
	return $resultado;
}

function query($conexion, $consulta){
	$consulta =  pg_escape_string($conexion,$consulta);
        pg_query($conexion, $consulta);
}

function insertar($conexion, $nombre, $password){
	$id = max_usuario();
	$nombre =  pg_escape_string($conexion,$nombre);
        $password =  pg_escape_string($conexion, $password);
	md5($password);
        $query="insert into usuarios (usuarios_id, tipos_tipo_id, nombre, pass)VALUES(".$id.", 2 ,'".$nombre."',".$password."')";
}

function idusuario($link, $nombre){
	$consulta = "SELECT u.usuarios_id FROM usuarios u where u.nombre like '".$nombre."'";
	$resultado = pg_query ($link, $consulta);
	while ($datos = pg_fetch_array($resultado))
		$id=$datos['usuarios_id'];	
	return $id;
}

function max_usuario(){
	$query = "SELECT max(id_producto) max FROM productos";
	$result = pg_query ($conexion, $query);
	while ($row = pg_fetch_array($result)) {
		$id = (int)$row["max"];
	}
	$id++;
	return id;
}
}
?>



