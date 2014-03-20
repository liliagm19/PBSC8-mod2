<?php
/****************Validacion.php********************/

if(isset($_POST['nombre']) && isset($_POST['password'])){
	$nombre = $_POST['nombre'];
	$pass = $_POST['password'];
	$result = exec("./autenticacion $nombre $pass");
    	
	if ($result == '-1'){
		printf("<H1>Error!!..No se ha encontrado al usuario</H1>");}
	else{
		if ($result == '1'){
			session_start();
			$_SESSION['nombre'] = $_POST['nombre'];
			$_SESSION['pass'] = $_POST['password'];
			printf("<html>\n");
			printf("<body>\n");
			printf("<br><br><form action='DragDrop_Usuario.html'> Bienvenido Usuario: %s !!<input type='submit' value='ENTRAR'/></form>\n",$_SESSION['nombre']);
			printf("</body>\n");
			printf("</html>\n");
	
		}
		else{
			session_start();
			$_SESSION['nombre'] = $_POST['nombre'];
			$_SESSION['pass'] = $_POST['password'];
			printf("<html>\n");
			printf("<body>\n");
			printf("<br><br><form action='DragDrop_Admin.html'> Bienvenido Admin: %s !!<input type='submit' value='ENTRAR'/></form>\n",$_SESSION['nombre']);
			printf("</body>\n");
			printf("</html>\n");
		
		}
	}
}

?>
