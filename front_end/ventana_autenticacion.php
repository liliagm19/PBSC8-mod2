<?php
/*VISTA*/
/*INTERFAZ AUTENTICACION*/
/*Manda a llamar modulo CGI*/
/*Realizado: Oscar Alvarez Fernadez y Romero Velazquez Diana */
?>

<html>
<head>
<title>Acceso Usuario</title>
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




