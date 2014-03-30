<?php

/*VISTA*/
/*Vista del Portal de usuario*/
/*Realizado: Oscar Alvarez Fernadez, Romero Velazquez Diana y Victor Hugo Barajas*/

	session_start();
	if(empty($_SESSION['nombre'])){
		session_destroy();
		header('Location: ventana_inicio.php');
		
	}
	else{
		include_once("controlador_portal.php");
		$control = new controlador_portal;
		session_start();
		$nombre = $_SESSION['nombre']; 
		$lista= $control->sitio($nombre);
		$links= $control->links($nombre);
	}
?>
<!DOCTYPE html>
<html>
<head>
	<title>MI PORTAL</title> 
	<link rel="stylesheet" type="text/css" href="./CSS/styleVI.css" />
</head>
<body id= "fondo">
	
<h1>MI PORTAL</h1>
		<ul id="lista">
			<?php 
				for($i=0;$i<9;$i++){
					echo '<li id="elemento-'.$i.'" contenteditable="false"><a href="'.$links[$i].'">'.$lista[$i].'</a></li>';}
			?>
		</ul>
	<div id="form">
            <input type="radio" name="editar-ordenar" value="ordenar" id="ordenar1"/>
            <label for="ordenar1">Ordenar</label>		
	</div>
	<form action='ventana_inicio.php'>
            <br><input type='submit' value='Salir'/>
		<?php session_destroy(); ?>
	</form>
	</div>
	
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js"></script>
	<script src="https://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/jquery-ui.min.js"></script>	
	<script>
		$(function(){
			var ordenando = false, lista = $('#lista'),
			elementos = lista.find('li');
			lista.sortable({
                	update: function(event,ui){
                    		var ordenPuntos = $(this).sortable('toArray').toString();
                    		$.ajax({
                        		type: 'POST',
                        		url: 'controlador_dragdrop.php',
                        		dataType: 'json',
                        		data: {
                            			accion: 'ordenar',
                            			puntos: ordenPuntos
                        		}
                    		});
                	}
            		});
		});
	</script>
</body>
</html>






