<?php
/*VISTA*/
/*Controlador para mover vistas*/
/*Realizado: Romero Velazquez Diana */
include_once("conectar.php");
	$conect = new conectar;
	$conex = $conect->conexion();
include_once("controlador_portal.php");
	$control = new controlador_portal;
	session_start();
	$nombre = $_SESSION['nombre'];

	
if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && ($_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest') && isset($_POST['accion'])){   	
	$consulta = '';
	$accion = $_POST['accion'];
	switch($accion){
		case 'ordenar':{ // Ordena los elementos
			$puntos = explode(',',$_POST['puntos']);
			$id_usuario = $conect->idusuario($conex,$nombre);
            		$consulta = 'UPDATE ordenes SET orden = CASE id '.PHP_EOL;
        		foreach ($puntos as $index => $id){
            			$idPunto = explode('-', $id);
            			$idPunto =  pg_escape_string($link,$idPunto[1]);
            			$orden =  pg_escape_string($link, ($index + 1));
                		$consulta .= 'WHEN '.$idPunto.' THEN '.$orden.''.PHP_EOL;
        		}
            		$consulta .= 'ELSE orden'.PHP_EOL.'END';
			$conect->query($conex, $consulta);	
			break;
		}
	}		
}


