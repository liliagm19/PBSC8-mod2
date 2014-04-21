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
#define MAXINPUT MAXLEN+EXTRA+2

/*
Autor: Castillo Medina AarÃ³n MartÃ­n.
Este es el CGI, estÃ¡ desarrollado en C y de hecho consta sÃ³lo de 3 archivos, los cuales son:
validar.c -> es este archivo en el cual se encuentran implementados todos los mÃ©todos para autenticar a un usuario y redirigirlo segÃºn sea el caso.
datos.c y datos.h -> son archivos que se escribieron para guardar en Ã©stos la informaciÃ³n mÃ¡s sensible a las conexiones.

*/


/*+++++++++++++++++MÃ‰TODOS+++++++++++++++++++*/

/*
Este método convierte un caracter hexadecimal a su valor entero. 
*/
char from_hex(char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/*
Método que obtiene una cadena de sólo texto con base en otra que contenga
caracteres en hexadecimal.
Nota: hay que liberar la memoria de la cadena resultante o habrá fallos.
*/
char* url_decode(char* str) {
  char* pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
  while (*pstr) {
    if (*pstr == '%') {
  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } else if (*pstr == '+') { 
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  return buf;

}


/*
Este mÃ©todo obtiene la cadena md5 asociada a una entrada input.
Cabe mencionar que la salida serÃ¡ una cadena de 16 bits y de longitud 32.
*/
char* string_md5(char* input){
      int i;

      //AquÃ­ se guarda en un inicio el resultado.
      unsigned char result[MD5_DIGEST_LENGTH];

      //Este minibuffer sirve para ir almacenando caracter por caracter del hash, ya que encontrÃ© que el hash por sÃ­ mismo no es un string entonces esto fue un problema al hacer la conversiÃ³n
      //de toda la cadena de un sÃ³lo golpe.
      unsigned char minibuffer[2];

      //AquÃ­ se guarda el hash ya convertido en cadena.
      unsigned char* resultado = (char*)malloc(MD5_DIGEST_LENGTH*2*sizeof(char));

      //Se aplica el mÃ©todo correspondienta
      MD5(input,strlen(input),result);

      //AquÃ­ se va haciendo la conversiÃ³n de un hash al mismo hash pero de tipo char*.
      for(i = 0; i < MD5_DIGEST_LENGTH; i++){
      //AquÃ­ se va haciendo la conversiÃ³n de un hash al mismo hash pero de tipo char*.
      for(i = 0; i < MD5_DIGEST_LENGTH; i++){
          sprintf(minibuffer,"%02x",(unsigned char)result[i]);
          strcat(resultado,minibuffer);
  
      }
 
      return resultado;

}


/*
MÃ©todo que regresa un token con base en una cadena inicial.
La diferencia entre el token y el hash es que aquÃ­ se aplica reiterativamente el hash sobre la misma cadena, es decir, se le aplica un hash a la cadena y al resultado se le 
vuelve a aplicar y asÃ­ sucesivamente.
Las veces que se va a plicar la funciÃ³n estÃ¡ dictada por un nÃºmero aleatorio que oscila entre 10 y 2010, es decir, ese es el rango que podrÃ­a aplicarse.
*/
char* token(char* input){
     srand(time(NULL));
     int i;
     int cuantas_veces = (int)((rand()%2000) + 10);

     //Se aplica una vez el hash.
     char* new_input = string_md5(input);
     char* aux;

     //AquÃ­ se aplica el hash tantas veces como se haya obtenido el valor de cuantas_veces.     
     for (i=0;i<cuantas_veces-1;i++){
           aux = string_md5(new_input);
           new_input = aux;
          
     }

     return aux;
}


/*
Este mÃ©todo salva a un archivo los tokens necesarios para que puedan ser leÃ­dos por el validador de php.
Sin peÅ•dida de generalidad, los tokens correspondientes a administradores irÃ¡n a parar al archivo tokens1,
Este mÃ©todo salva a un archivo los tokens necesarios para que puedan ser leÃ­dos por el validador de php.
Sin peÅ•dida de generalidad, los tokens correspondientes a administradores irÃ¡n a parar al archivo tokens1,
mientras que los tokens de usuarios irÃ¡n a dar al archivo tokens2.
El formato de agregar es: token1:token2 donde son los tokens de usuario y contraseÃ±a respectivamente.
Para saber a quiÃ©n hay que agregar en quÃ© lugar se dispone de un flag que se encarga de ello.
RUTA_TOKENS1 y ROTA_TOKENS2 son cadenas definidas en el archivo de datos.
*/
void save_to_file(char* token1, char* token2, char* flag){
    FILE* file1;
    FILE* file2;

    //Se trata de un administrador si el flag es "1", por lo que hay que agregarlo al archivo correspondiente.
    if(strcmp(flag,"1")==0){
       file1 = fopen(RUTA_TOKENS1,"w");
       file2 = fopen(RUTA_TOKENS2,"w"); 
       fprintf(file1,"%s:%s\n",token1,token2);
       fprintf(file2,"%s",""); 
       fclose(file1);
       fclose(file2);


    //Se trata de un usuario si el flag es "2", por lo que hay que agregarlo al archivo correspondiente.
    } else if(strcmp(flag,"2")==0){
      file1 = fopen(RUTA_TOKENS1,"w");
      file2 = fopen(RUTA_TOKENS2,"w");
      fprintf(file1,"%s",""); 
      fprintf(file2,"%s:%s\n",token1,token2); 
      fclose(file1);
      fclose(file2);

    }


}

/*
MÃ©todo que autentica al usuario, le proporciona las credenciales necesarias.
*/
void autentica(char* user, char* password, char* user_salida,char* pass_salida) {
    PGconn *conn;
    PGresult *res;
    PGconn *conn;
    PGresult *res;
    int rec_count;
    int row;
    int col;
    char* user_hash;
    char* password_hash;
    int cuantos_campos = 1;

    char conexion[1024];
    sprintf(conexion,"dbname=%s host=%s user=%s password=%s",DBNAME,HOST,USER,PASSWORD);
    
    char consulta[1024];
    sprintf(consulta,"SELECT tipos_tipo_id FROM usuarios WHERE nombre='%s' AND pass='%s';",user,password);

    conn = PQconnectdb(conexion);
 
    //Para el caso en que no se pueda conectar a la base se envÃ­a un -1 para ambos valores, con Ã©sto el controlador de php asume que se trata de un error en la bd.
    if (PQstatus(conn) == CONNECTION_BAD) { 
        strcpy(user_salida,"-1");
        strcpy(pass_salida,"-1");
        return;
    } 

    res = PQexec(conn,consulta);

    //Para el caso en que estÃ© mal la consulta se envÃ­a un -1 para ambos valores, con Ã©sto el controlador de php asume que se trata de un error en la bd.
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
         strcpy(user_salida,"-1");
         strcpy(pass_salida,"-1");       
         return;
    }


    //Con base en la consulta se obtiene un resultado.
    rec_count = PQntuples(res);
    //Para el caso en que no exista el usuario Ã³ la contraseÃ±a se envÃ­an las credenciales al controlador de php pero no se envÃ­an al archivo, asÃ­ se garantiza que no
    //se aceptarÃ¡ la sesiÃ³n.
    if (rec_count == 0){
        strcpy(user_salida,token(user));
        strcpy(pass_salida,token(password));
        strcpy(user_salida,token(user));
        strcpy(pass_salida,token(password));
        return;
    }

    for (row=0; row<rec_count; row++) {
        for (col=0; col<cuantos_campos; col++) {
            //Si el usuario encontrado es un administrador
            if(strcmp(PQgetvalue(res, row, col),"1")==0){
               password_hash = token(password);
               save_to_file(user,password_hash,"1");
               strcpy(user_salida,user);
               strcpy(pass_salida,password_hash);
               

            //Si el usuario encontrado es un simple mortal
            } else if (strcmp(PQgetvalue(res, row, col),"2")==0){
                      password_hash = token(password);
                      save_to_file(user,password_hash,"2");
                      strcpy(user_salida,user);
                      strcpy(pass_salida,password_hash);


            } 

        }

    }

    //Se limpia el resultado y se cierra la conexiÃ³n
    PQclear(res);
    PQfinish(conn);
 
}


/*
AquÃ­ es donde se implementan los mÃ©todos inicialmente desarrollados en la parte de arriba, ademÃ¡s de que se aÃ±aden otros mÃ³dulos como la sanitizaciÃ³n de entradas.
*/
int  main(void){
    int i;
int  main(void){
    int i;

    //Variables para saber lo que se recibe del formulario php.
    char input[MAXINPUT]; 
 
    //Variables para que, con base en lo que se recibe del formulario php se puedan obtener el usuario y la contraseÃ±a.
    char* string_tok =  (char*)malloc(MAXLEN*sizeof(char));
    char* string_tok_1;
    char* string_tok_2;
    char* mini_tok_1 = (char*)malloc(MAXLEN*sizeof(char));
    char* mini_tok_2 = (char*)malloc(MAXLEN*sizeof(char));

    //Variables para conocer el usuario y la contraseÃ±a.
    char* user;
    char* pass;
    char* user_salida = (char*)malloc(MAXLEN*sizeof(char));
    char* pass_salida = (char*)malloc(MAXLEN*sizeof(char));

    //Variables para el manejo de expresiones regulares.
    regex_t regex;
    int reti;
    char msgbuf[100];

    //Se leen los datos en bruto desde el formulario.
    fscanf(stdin,"%s",input);

    //Aquí lo que sucede es que en PHP los caracteres especiales (comas, signos de puntuacion, de interrogación, etc.) se 
    //envían al CGI en formato hexadecimal, entonces esta función "regresa" los valores al formato original (sólo texto).
    char* input_decode = url_decode(input);

    char* input2 = (char*)malloc(strlen(input_decode)*2*sizeof(char));
    strcpy(input2,input_decode);

    //Todo este proceso a continuaciÃ³n es para descomponer la larga cadena del login en tokens para obtener el usuario y la contraseÃ±a.
    string_tok = strtok(input_decode,"&");
    string_tok_1 = (char*)malloc(strlen(string_tok)*sizeof(char));
    string_tok_2 = (char*)malloc(strlen(string_tok)*sizeof(char));

    sprintf(string_tok_1,"%s",string_tok);
    string_tok = strtok(NULL,"&");
    sprintf(string_tok_1,"%s",string_tok);
    string_tok = strtok(NULL,"&");
    sprintf(string_tok_2,"%s",string_tok);

    mini_tok_1 = strtok(string_tok_1,"=");
    user = (char*)malloc(strlen(mini_tok_1)*sizeof(char));
        
    sprintf(user,"%s",mini_tok_1);
    mini_tok_1 = strtok(NULL,"=");


    //En caso de que el campo user venga vacío se asigna un -1.
    if (mini_tok_1==NULL)
       sprintf(user,"%s","-1");

    else 
       sprintf(user,"%s",mini_tok_1);
       
    mini_tok_2 = strtok(string_tok_2,"=");
    pass = (char*)malloc(strlen(mini_tok_2)*sizeof(char));
        
    sprintf(pass,"%s",mini_tok_2);
    mini_tok_2 = strtok(NULL,"=");


    //En caso de que el campo pass venga vacío se asigna un -1.
    if (mini_tok_2==NULL)
       sprintf(pass,"%s","-1");

    else 
       sprintf(pass,"%s",mini_tok_2);

    //AquÃ­ termina el proceso de la obtenciÃ³n del usuario y la contraseÃ±a.

    //Se autentica el usuario.
    autentica(user,pass,user_salida,pass_salida);

    printf("Content-type: text/html; charset=utf-8\n\n");
    /*
     A continuaciÃ³n sanitizamos la entrada que se pase del login, para ello tomamos en bruto toda la cadena user=X&password=Y y verificamos que en los valores de estas
     variables no estÃ©n introducidas sentencias maliciosas para repercutir en la bd.
     A continuaciÃ³n sanitizamos la entrada que se pase del login, para ello tomamos en bruto toda la cadena user=X&password=Y y verificamos que en los valores de estas
     variables no estÃ©n introducidas sentencias maliciosas para repercutir en la bd.
    */

    //Primero se compila la expresiÃ³n regular
    reti = regcomp(&regex, "^nombre=[a-zA-Z0-9]+('|SELECT|select|UPDATE|update|DELETE|delete|DROP|drop|SCRIPT|script|FROM|from|WHERE|where|OR|or|AND|and|=| |;|;--)+&password=[a-zA-Z0-9]+('|SELECT|select|UPDATE|$

    //En caso de error se manda un mensaje al error estÃ¡ndar.
    if(reti){
       //fprintf(stderr, "Regex no compilable reti\n"); 
       strcpy(user_salida,"-1");
       strcpy(pass_salida,"-1");
    }

    //Si no hubo error se continÃºa normalmente.
    reti = regexec(&regex,input2, 0, NULL, 0);

    //Si la informaciÃ³n hace match con las expresiones regulares significa que se han detectado sentencias maliciosas, por lo que simplemente se envÃ­an las supuestas
    //credenciales sin consultar a la base, de todas formas se sabe que estarÃ¡n mal pues no se meterÃ¡n al archivo correspondiente.
    if(!reti){     
        //printf("Entró en match");
        printf("<HTML>\n");
        //printf("%s",input2);
        printf("<meta http-equiv=\"Refresh\" content=\"1; url=http://frontendmod02.cloudapp.net/sesion.php?user=%s&token=%s\">\n",token(user),token(pass));
        printf("</HTML>\n");
       
    
    }

    //La informaciÃ³n no hace match con las expresiones regulares, significa que se puede seguir adelante.
   if(reti == REG_NOMATCH && strcmp(user,user_salida)==0){  
      //printf("No entró en match\n");
      printf("<HTML>\n");
      printf("<meta http-equiv=\"Refresh\" content=\"1; url=http://frontendmod02.cloudapp.net/sesion.php?user=%s&token=%s\">\n",user_salida,pass_salida);
      printf("</HTML>\n");
            

    //En caso de que no se haya podido entrar en el match pero las credenciales hayan estado incorrectas, se manda un error.
    } else {

      //printf("No entró en match pero credenciales malas.\n");

      //printf("No entró en match pero credenciales malas.\n");
      printf("<HTML>\n");
      printf("<meta http-equiv=\"Refresh\" content=\"1; url=http://frontendmod02.cloudapp.net/sesion.php?user=%s&token=%s\">\n","-1","-1");
      printf("</HTML>\n");

      regerror(reti, &regex, msgbuf, sizeof(msgbuf));
      //fprintf(stderr, "Falla en el regex: %s\n", msgbuf);
      exit(1);

    }     

    free(input_decode);
    regfree(&regex);
    return 0;
}


