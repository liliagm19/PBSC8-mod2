<?php
/****************Acceso a un Usuario.php********************/

printf("<html>\n");
printf("<head>\n");
printf("<title>Acceso Usuario</title>\n");
printf("</head>\n");
printf("<body>\n");

printf("<br><br><form action='Validacion.php' method='POST'>");
printf("Nombre: <input type='text' name='nombre'><br>");
printf("Contrasena: <input type='password' name='password'><br>");
printf("<select id='tipo' name='tipo'>\n");
	printf("<option value='usuario'>Usuario</option>");
	printf("<option value='admin'>Administrador</option>");
printf("</select>\n");
printf("<input type='submit' value='ENTRAR'/></form>\n");

printf("</body>\n");
printf("</html>\n");
	
?>
