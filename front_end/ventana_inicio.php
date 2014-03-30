<?php

/*VISTA*/
/*VISTA DEL PORTAL*/
/*Realizado: Oscar Alvarez Fernadez, Romero Velazquez Diana y Victor Hugo Barajas */

include_once("controlador_index.php");
	$control = new controlador_index;
	$lista= $control->sitio();
	$links= $control->links();
?>

<html>
<head>
	<title>MI PORTAL</title> 
	<link rel="stylesheet" type="text/css" href="./CSS/styleVI.css" />
</head>
<body id= "fondo">
		<h1>Lista de tareas</h1>
		<ul id="lista">
			<?php 
				for($i=0;$i<9;$i++){
					echo '<li id="elemento-'.$i.'" contenteditable="false"><a href="'.$links[$i].'">'.$lista[$i].'</a></li>';}
			?>
		</ul>
		<form action='ventana_autenticacion.php'>
            		<br><input type='submit' value='Acceso'/>	
		</form>
			
</body>
</html>
