<?php
/*VISTA*/
/*INTERFAZ AUTENTICACION*/
/*Manda a llamar modulo CGI*/
/*Realizado: Oscar Alvarez Fernadez, Romero Velazquez Diana y Victor Hugo Barajas*/
?>

<html>
<head>
<title>Acceso Usuario</title><link rel="stylesheet" type="text/css" href="./CSS/style.css" />
</head>
<body>
<br><br><form action="/validar.cgi" method='POST'>
	Nombre: <input type='text' name='nombre'><br>
	Contrasena: <input type='password' name='password'><br>
	<input type='submit' value='Entrar'/></form>
	<form action='ventana_inicio.php'>
            	<br><input type='submit' value='Salir'/>	
	</form>
</body>
</html>




