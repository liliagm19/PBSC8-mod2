#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>
#include <openssl/md5.h>


/*Aarón Martín Castillo Medina
* Este programa lo que realiza es primero sanitizar el usuario y la contraseña, luego se reciben los valores y se hace con la conexión con la * bd.
*Regresa -1 si es una operación inválida , 0 si el usuario o la contraseña no existen  y el formato hash(contraseña)-id-hash(user)
*Para la cookie.
*/


//TODO
//Hacer funcionar el .cgi (estoy intentando con el AddHandler al apache2.conf pero no jala).
//Separar los argumentos user y pass enviados por POST.
//Falta convertir a hash el pass para la comparación con la bd.
//Colocar los datos de la conexion en un .h aparte
//La sanitización de entradas hacerla en otro .h aparte (para mantener modularidad)
//Devolver 0 si es no válido y en otro caso una cookie
//Autenticación auth

int main(int argc, char **argv) {
    PGconn *conn;
    PGresult *res;
    int rec_count;
    int row;
    int col;

    unsigned char digest[16];
    const char* string = argv[1];
   // MD5Context context;
    
    //MD5Init(&context);
    //MD5Update(&context, string, strlen(string));
    //MD5Final(digest, &context);

    //printf("Mi cadena hasheada:%s\n",digest);


    char consulta[1024];

    sprintf(consulta,"SELECT tipo FROM Usuarios WHERE username='%s' AND pass='%s';",argv[1],argv[2]);
    
    printf("Mi consulta es: %s\n",consulta);
    
 
    conn = PQconnectdb("dbname=auth host=localhost user=admin password=hola123,");
 
    //Para el caso en que no se pueda conectar a la base.
    if (PQstatus(conn) == CONNECTION_BAD) {
        printf("-1");
        exit(0);
    }
 
    res = PQexec(conn,consulta);

    //Para el caso en que esté mal la consulta 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("-1");
        exit(0);
    }
 
    rec_count = PQntuples(res);
    
    //Para el caso en que no exista el usuario ó la contraseña.
    if (rec_count == 0){
       printf("-1");
       exit(0);

    }

    for (row=0; row<rec_count; row++) {
        for (col=0; col<3; col++) {
            if (strcmp(PQgetvalue(res, row, col),"a")==0){
               printf("");
               exit(0);

            } else if (strcmp(PQgetvalue(res, row, col),"u")==0){
               printf("");
               exit(0);

            } else {
              printf("");
            
            }
            
        }

    }

    PQclear(res);

    PQfinish(conn);
 
    return 0;
 
}
