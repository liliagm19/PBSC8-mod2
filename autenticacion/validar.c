#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <openssl/md5.h>
#include <postgresql/libpq-fe.h>
#include "datos.h" 


//Se definen estas 3 constantes que se utilizan al momento de recibir datos del formulario php.
#define MAXLEN 80
#define EXTRA 5

/* 4 for field name "data", 1 for "=" */
#define MAXINPUT MAXLEN+EXTRA+2

/*
Autor: Castillo Medina Aarón Martín.
Este es el CGI, está desarrollado en C y de hecho consta sólo de 3 archivos, los cuales son:
validar.c -> es este archivo en el cual se encuentran implementados todos los métodos para autenticar a un usuario y redirigirlo según sea el caso.
datos.c y datos.h -> son archivos que se escribieron para guardar en éstos la información más sensible a las conexiones.

*/


/*+++++++++++++++++MÉTODOS+++++++++++++++++++*/

/*
Este método obtiene la cadena md5 asociada a una entrada input.
Cabe mencionar que la salida será una cadena de 16 bits y de longitud 32.
*/
char* string_md5(char* input){
      int i;

      //Aquí se guarda en un inicio el resultado.
      unsigned char result[MD5_DIGEST_LENGTH];

      //Este minibuffer sirve para ir almacenando caracter por caracter del hash, ya que encontré que el hash por sí mismo no es un string entonces esto fue un problema al hacer la conversión
      //de toda la cadena de un sólo golpe.
      unsigned char minibuffer[2];

      //Aquí se guarda el hash ya convertido en cadena.
      unsigned char* resultado = (char*)malloc(MD5_DIGEST_LENGTH*2*sizeof(char));

      //Se aplica el método correspondienta
      MD5(input,strlen(input),result);

      //Aquí se va haciendo la conversión de un hash al mismo hash pero de tipo char*.
      for(i = 0; i < MD5_DIGEST_LENGTH; i++){
          sprintf(minibuffer,"%02x",(unsigned char)result[i]);
          strcat(resultado,minibuffer);
  
      }
 
      return resultado;

}


/*
Método que regresa un token con base en una cadena inicial.
La diferencia entre el token y el hash es que aquí se aplica reiterativamente el hash sobre la misma cadena, es decir, se le aplica un hash a la cadena y al resultado se le 
vuelve a aplicar y así sucesivamente.
Las veces que se va a plicar la función está dictada por un número aleatorio que oscila entre 10 y 2010, es decir, ese es el rango que podría aplicarse.
*/
char* token(char* input){
     srand(time(NULL));
     int i;
     int cuantas_veces = (int)((rand()%2000) + 10);

     //Se aplica una vez el hash.
     char* new_input = string_md5(input);
     char* aux;

     //Aquí se aplica el hash tantas veces como se haya obtenido el valor de cuantas_veces.     
     for (i=0;i<cuantas_veces-1;i++){
           aux = string_md5(new_input);
           new_input = aux;
          
     }

     return aux;
}


/*
Este método salva a un archivo los tokens necesarios para que puedan ser leídos por el validador de php.
Sin peŕdida de generalidad, los tokens correspondientes a administradores irán a parar al archivo tokens1,
mientras que los tokens de usuarios irán a dar al archivo tokens2.
El formato de agregar es: token1:token2 donde son los tokens de usuario y contraseña respectivamente.
Para saber a quién hay que agregar en qué lugar se dispone de un flag que se encarga de ello.
RUTA_TOKENS1 y ROTA_TOKENS2 son cadenas definidas en el archivo de datos.
*/
void save_to_file(char* token1, char* token2, char* flag){
    FILE *file; 

    //Se trata de un administrador si el flag es "1", por lo que hay que agregarlo al archivo correspondiente.
    if(strcmp(flag,"1")==0){
       file = fopen(RUTA_TOKENS1,"a+"); 
       fprintf(file,"%s:%s\n",token1,token2); 
       fclose(file);


    //Se trata de un usuario si el flag es "2", por lo que hay que agregarlo al archivo correspondiente.
    } else if(strcmp(flag,"2")==0){
      file = fopen(RUTA_TOKENS2,"a+"); 
      fprintf(file,"%s:%s\n",token1,token2); 
      fclose(file);

    }


}

/*
Método que autentica al usuario, le proporciona las credenciales necesarias.
*/
void autentica(char* user, char* password, char* user_salida,char* pass_salida) {
    PGconn *conn;
    PGresult *res;
    int rec_count;
    int row;
    int col;
    char* user_hash;
    char* password_hash;


    char conexion[1024];
    sprintf(conexion,"dbname=%s host=%s user=%s password=%s",DBNAME,HOST,USER,PASSWORD);
    
    char consulta[1024];
    sprintf(consulta,"SELECT tipos_tipo_id FROM usuarios WHERE nombre='%s' AND pass='%s';",user,string_md5(password));

    conn = PQconnectdb(conexion);
 
    //Para el caso en que no se pueda conectar a la base se envía un -1 para ambos valores, con ésto el controlador de php asume que se trata de un error en la bd.
    if (PQstatus(conn) == CONNECTION_BAD) { 
        user_salida = "-1";
        pass_salida = "-1";
        return;
    }

    res = PQexec(conn,consulta);

    //Para el caso en que esté mal la consulta se envía un -1 para ambos valores, con ésto el controlador de php asume que se trata de un error en la bd.
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
         user_salida = "-1";
         pass_salida = "-1";       
         return;
    }


    //Con base en la consulta se obtiene un resultado.
    rec_count = PQntuples(res);
    
    //Para el caso en que no exista el usuario ó la contraseña se envían las credenciales al controlador de php pero no se envían al archivo, así se garantiza que no
    //se aceptará la sesión.
    if (rec_count == 0){
        user_salida = token(user);
        pass_salida = token(password);
        return;
    }

    for (row=0; row<rec_count; row++) {
        for (col=0; col<3; col++) {

            //Si el usuario encontrado es un administrador
            if (strcmp(PQgetvalue(res, row, col),"1")==0){
               user_hash = token(user);
               password_hash = token(password);
               save_to_file(user_hash,password_hash,"1");
               user_salida = token(user_hash);
               pass_salida = token(password_hash);
               

            //Si el usuario encontrado es un simple mortal
            } else if (strcmp(PQgetvalue(res, row, col),"2")==0){
                      user_hash = token(user);
                      password_hash = token(password);
                      save_to_file(user_hash,password_hash,"2");
                      user_salida = token(user_hash);
                      pass_salida = token(password_hash);
                 

            } 

        }

    }

    //Se limpia el resultado y se cierra la conexión
    PQclear(res);
    PQfinish(conn);
 
}


/*
Aquí es donde se implementan los métodos inicialmente desarrollados en la parte de arriba, además de que se añaden otros módulos como la sanitización de entradas.
*/
void main(void){
    int i;

    //Variables para saber lo que se recibe del formulario php.
    char input[MAXINPUT]; 
    char* lenstr = getenv("CONTENT_LENGTH");

 
    //Variables para que, con base en lo que se recibe del formulario php se puedan obtener el usuario y la contraseña.
    char* string_tok;
    char* string_tok_1;
    char* string_tok_2;
    char* mini_tok_1;
    char* mini_tok_2;

    //Variables para conocer el usuario y la contraseña.
    char* user;
    char* pass;
    char* user_salida;
    char* pass_salida;
  

    //Variables para el manejo de expresiones regulares.
    regex_t regex;
    int reti;
    int reti2;
    char msgbuf[100];

    //Se leen los datos en bruto desde el formulario.
    fscanf(stdin,"%s",&input);

    char* input2 = (char*)malloc(strlen(input)*sizeof(char));
    strcpy(input2,input);

    //Todo este proceso a continuación es para descomponer la larga cadena del login en tokens para obtener el usuario y la contraseña.
    string_tok = strtok(input,"&");
    string_tok_1 = (char*)malloc(strlen(string_tok)*sizeof(char));
    string_tok_2 = (char*)malloc(strlen(string_tok)*sizeof(char));

    sprintf(string_tok_1,"%s",string_tok);
    string_tok = strtok(NULL,"&");
    sprintf(string_tok_2,"%s",string_tok);

    mini_tok_1 = strtok(string_tok_1,"=");
    user = (char*)malloc(strlen(mini_tok_1)*sizeof(char));
        
    sprintf(user,"%s",mini_tok_1);
    mini_tok_1 = strtok(NULL,"=");
    sprintf(user,"%s",mini_tok_1);
        
    mini_tok_2 = strtok(string_tok_2,"=");
    pass = (char*)malloc(strlen(mini_tok_2)*sizeof(char));
        
    sprintf(pass,"%s",mini_tok_2);
    mini_tok_2 = strtok(NULL,"=");
    sprintf(pass,"%s",mini_tok_2);
    //Aquí termina el proceso de la obtención del usuario y la contraseña.


    autentica(user,pass,user_salida,pass_salida);
      

    printf("Content-type: text/html; charset=utf-8\n\n");


    
    /*
     A continuación sanitizamos la entrada que se pase del login, para ello tomamos en bruto toda la cadena user=X&password=Y y verificamos que en los valores de estas
     variables no estén introducidas sentencias maliciosas para repercutir en la bd.
    */

    //^nombre=[a-zA-Z0-9]*[DROP|drop|DATABASE|database|SELECT|select|INSERT|insert|DELETE|delete|UPDATE|update|LIKE|like|SCRIPT|script|<|>|*|--|;|=|'|FROM|from|WHERE|where][a-zA-Z0-9]*&password=[a-zA-Z0-9]*[DROP|drop|DATABASE|database|SELECT|select|INSERT|insert|DELETE|delete|UPDATE|update|LIKE|like|SCRIPT|script|<|>|*|--|;|=|'|FROM|from|WHERE|where][a-zA-Z0-9]*$

    //Primero se compila la expresión regular.
    reti = regcomp(&regex, "^nombre=(DROP|drop|DATABASE|database|SELECT|select|INSERT|insert|DELETE|delete|UPDATE|update|LIKE|like|SCRIPT|script|<|>|*|--|;|=|'|FROM|from|WHERE|where)&password=(DROP|drop|DATABASE|database|SELECT|select|INSERT|insert|DELETE|delete|UPDATE|update|LIKE|like|SCRIPT|script|<|>|*|--|;|=|'|FROM|from|WHERE|where)$", 0);

    //En caso de error se manda un mensaje al error estándar.
    if(reti){
       fprintf(stderr, "Regex no compilable reti\n"); 
       exit(1);
    }

    //Si no hubo error se continúa normalmente.
    reti = regexec(&regex,input2, 0, NULL, 0);
 
    //Si la información hace match con las expresiones regulares significa que se han detectado sentencias maliciosas, por lo que simplemente se envían las supuestas
    //credenciales sin consultar a la base, de todas formas se sabe que estarán mal pues no se meterán al archivo correspondiente.
    if(!reti){     
        printf("<HTML>\n");
        //printf("%s",input2);
        printf("<meta http-equiv=\"Refresh\" content=\"1; url=controlador_portal.php?user=%s&token=%s\">\n","muy","chafa");
        printf("</HTML>\n");
       
    
    }

    //La información no hace match con las expresiones regulares, significa que se puede seguir adelante.
   if(reti == REG_NOMATCH){  
      printf("<HTML>\n");
      //printf("%s",input2);
      printf("<meta http-equiv=\"Refresh\" content=\"1; url=controlador_portal.php?user=%s&token=%s\">\n",user_salida,pass_salida);
      printf("</HTML>\n");
            
            
    //En caso de que no se pueda ejecutar bien la expresión regular, se manda un error.
    } else {
            regerror(reti, &regex, msgbuf, sizeof(msgbuf));
            fprintf(stderr, "Falla en el regex: %s\n", msgbuf);
            exit(1);

    }     

    regfree(&regex);
  
}
