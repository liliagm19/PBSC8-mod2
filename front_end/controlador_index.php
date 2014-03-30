<?php
include_once("conectar.php");

/*CONTROLADOR*/
/*FUNCIONES PARA EXTRAER DATOS DE LA BASE*/
/*Realizado: Oscar Alvarez Fernadez y Romero Velazquez Diana */

class controlador_index{
function sitio(){
	$conect = new conectar;
	$conex = $conect->conexion();
	$ar = $conect->usuario($conex,"admin");
	for($i=0;$i<9;$i++){
		$lista[$i] = $conect->paginas($conex, $ar[$i]); 
	}
	$conect->desconectar();
	return $lista;
}

function links(){
	$conect = new conectar;
	$conex = $conect->conexion();
	$ar = $conect->usuario($conex,"admin");
	for($i=0;$i<9;$i++){
		$links[$i] = $conect->links($conex, $ar[$i]);  
	}
	$conect->desconectar();
	return $links;
}
}
?>
