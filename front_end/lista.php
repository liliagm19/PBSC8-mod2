<?php
	
	$link = pg_connect("host=localhost port=6543 dbname=usuario user=dov password=hola") or die("Existen Problemas de conexion con la base de datos.");
	$consulta = "SELECT * FROM ordenes ORDER BY orden";
	$resultado = pg_query ($link, $consulta);
	$elementos = null;
	while ($datos = pg_fetch_array($resultado)){
		$elementos[$datos['id']] = $datos['nombre'];
	}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Lista editable y ordenable</title>
    <meta charset="utf-8">
    <link href='http://fonts.googleapis.com/css?family=Lilita+One' rel='stylesheet' type='text/css'>
    <!--<link href="pagina.css" rel="stylesheet" type="text/css" media="all"> --> 
</head>
<body>
	<div id="wrapper">
		<h1>Lista de tareas</h1>
		<ul id="lista">
			<?php 
				foreach ($elementos as $id => $nombre)
					echo '<li id="elemento-'.$id.'" contenteditable="false"><a href="loggin.php"><img src="apple.jpg" alt="" height="115" width="200" />'.$nombre.'</a></li>';
			?>
		</ul>
		<div id="form">
            <input type="submit" name="editar-ordenar" value="ordenar" id="ordenar1"/>
            <label for="ordenar1">Ordenar</label>		
		</div>
		
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
                        url: 'controlador.php',
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
