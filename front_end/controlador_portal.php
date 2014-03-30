
<?php
include_once("conectar.php");
class controlador_portal{

/*CONTROLADOR*/
/*Control que verifica si el token es correcto, genera la sesion y extrae la informacion bd*/
/*Realizado: Oscar Alvarez Fernadez y Romero Velazquez Diana */

function archivo_lleno () {
	$ar=fopen("home/token1","r") or die("No se pudo abrir el archivo");
		$linea=fgets($ar);
	if (empty($linea)){
		$bandera=2;				
	}
	else{
		$bandera=1;
	}
} 

function archivo ($nombre_archivo) {
	$ar=fopen("home/$nombre_archivo","r") or die("No se pudo abrir el archivo");
		$linea=fgets($ar);
		$lineasinsalto=str_replace(PHP_EOL, ‘/‘, $linea);
	fclose($ar);
	$ar=fopen("home/$nombre_archivo","w") or die("No se pudo abrir el archivo");
	fclose($ar);
	return $lineasinsalto;
}

function valida_token ($nombreMD5,$tokenMD5,$cadena) {
	$datos = explode(":", $cadena);
	if ($datos[0]==$nombreMD5 && $datos[1]==$tokenMD5){
		$bandera=1;
	}
	else {
		$bandera=-1;
	}
	fclose($ar);
	return $bandera;
} 

function nueva_sesion(){
	$nombreMD5=$_GET['user'];
	$tokenMD5=$_GET['token'];
	if($nombreMD5 == -1 && $tokenMD5 == -1){
		$codigo_error($nombreMD5);
		exit();
	}
	$tipo_archivo=archivo_lleno();	
	$nombre_archivo = token.$tipo_archivo;
	$cadena = archivo($nombre_archivo);
	$valtok = valida_token($nombreMD5, $tokenMD5,$cadena);
	if($tipo_archivo==1 && $valtok==1){
		session_start();
		$_SESSION['nombre'] = $_POST['nombre'];
		header('Status: 301 Moved Permanently',false, 301);
		header('Location: ventana_admin.php');
		exit();
	}
	if($tipo_archivo==2 && $valtok==1){
		session_start();
		$_SESSION['nombre'] = $_POST['nombre'];
		header('Status: 301 Moved Permanently',false, 301);
		header('Location: ventana_usuario.php');
		exit();

	}

}

function codigo_error($codigo_error){
	if($codigo_error == -1){
		$mensaje_er='Error en conexion en la base de datos';
		header('Location: error.php?codigo=$codigo_error&mensaje=$mensaje_er');
	}
}

function sitio($usuario){
	$conect = new conectar;
	$conex = $conect->conexion();
	$ar = $conect->usuario($conex,$usuario);
	for($i=0;$i<9;$i++){
		$lista[$i] = $conect->paginas($conex, $ar[$i]); 
	}
	$conect->desconectar();
	return $lista;
}

function links($usuario){
	$conect = new conectar;
	$conex = $conect->conexion();
	$ar = $conect->usuario($conex,$usuario);
	for($i=0;$i<9;$i++){
		$links[$i] = $conect->links($conex, $ar[$i]);  
	}
	$conect->desconectar();
	return $links;
}
}

nueva_sesion();
?>
