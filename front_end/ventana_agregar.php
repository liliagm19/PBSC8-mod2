<?php

/*VISTA*/
/*Agregar usuario*/
/*Realizado:Oscar Alvarez Fernadez y Romero Velazquez Diana */

if($_POST['nombre']!="" && $_POST['pass']!=""){
	include_once("conectar.php");
	$conect = new conectar;
	$conex = $conect->conexion();
	$conect->insertar($conex,$_POST['nombre'],$_POST['pass']);	
}
?>

<html>
<head>
	<title>Nuevo Usuario</title> 
</head>
<body>
<h3>Datos de Nuevo Usuario. Agregar</h3>");
	<form action='#' method='post'>
	Nombre: <input type='text' name='nombre' size='5'/><br>");
	Password: <input type='number' name='pass' size='15'/><br><br>");
	printf("<input type='submit' value='Agregar'/>\n");
	</form>
	<form action='ventana_inicio.php'>
            <br><input type='submit' value='Salir'/>
		<?php session_destroy(); ?>
	</form>
</body>
</html>
