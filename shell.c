#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#define RESET_COLOR "\x1b[0m"
#define NEGRO_T     "\x1b[30m"
#define NEGRO_F     "\x1b[40m"
#define ROJO_T      "\x1b[31m"
#define ROJO_F      "\x1b[41m"
#define VERDE_T     "\x1b[32m"
#define VERDE_F     "\x1b[42m"
#define AMARILLO_T  "\x1b[33m"
#define AMARILLO_F  "\x1b[43m"
#define AZUL_T      "\x1b[34m"
#define AZUL_F      "\x1b[44m"
#define MAGENTA_T   "\x1b[35m"
#define MAGENTA_F   "\x1b[45m"
#define CYAN_T      "\x1b[36m"
#define CYAN_F      "\x1b[46m"
#define BLANCO_T    "\x1b[37m"
#define BLANCO_F    "\x1b[47m"

char entrada[250];
char directorio_actual[250];

void *funcion_del_hilo(void *arg) {

        printf(CYAN_T "\n **********************************************INICIO DE EJECUCION EN SEGUNDO PLANO******************************************************\n");
        getcwd(directorio_actual,sizeof(directorio_actual));
        printf("\n %s :", directorio_actual);

        if(arg != NULL){
                int resultado=ejecutarComando();
        }
        printf("\n **********************************************FIN DE EJECUCION EN SEGUNDO PLANO******************************************************");
        printf(RESET_COLOR "\n");

        pthread_exit(NULL);
}

int ejecutarComando(){

        int resultado = 0;
        char comando[250];
        char argumentos[250];
        char caracter = entrada[0];
        int contador_caracteres=0;

        comando[0]='\0';
        argumentos[0]='\0';
        int count1 = 0;
        int count2 = 0;

        while(caracter != '\0'){
                while((caracter!='\0') && (caracter!=' ')){ 
				comando[count1] = caracter;
                        count1++;
                        contador_caracteres++;
                        caracter=entrada[contador_caracteres];
                }
                if(count1 > 0){
                        comando[count1] = '\0';
                        break;
                }
                contador_caracteres++;
                caracter=entrada[contador_caracteres];
        }
        while(caracter != '\0'){
                argumentos[count2]=caracter;
                caracter=entrada[contador_caracteres];
                count2++;
                contador_caracteres++;
        }

        argumentos[count2]='\0';

        int plano = 0;

        if(count2 > 0){
                if(argumentos[count2-1] != '&'){
                        plano=1;
                }else{
                        plano=2;
                }
        }else{
                if(count1>0){
                        if(comando[count1-1] == '&'){
                                plano=2;
                        }else{
                                plano=1;
                        }
                }else{
                        plano=1;
                }
        }

 if(plano == 1){
                if(strcmp(comando,"quit")==0){
                        resultado=-1;
                }else if(strcmp(comando,"")==0){
                        printf("\n");
                }else if(strcmp(comando,"cd")==0){

                        int inicio=0;
                        char car = argumentos[0];
                        while((car!='\0') && (car == ' ')){
                                inicio++;
                                car=argumentos[inicio];
                        }
                        char dir[255];
                        int tamano = sizeof(argumentos)-inicio;

                        strncpy(dir, &argumentos[inicio], tamano);
                        dir[tamano] = '\0'; // cortar la linea

                        if(strcmp(dir,"")!=0){
                                int res = chdir(dir);
                                if (res != 0) {
                                        printf("El archivo o directorio no exite");
                                }

                        }else{
                                printf("\n Directorio actual: %s \n", directorio_actual);
                        }
                }else if(strcmp(comando,"pwd")==0){
                        strcat(comando," ");
                        strcat(comando,argumentos);
                        resultado=system(comando);

                }else if(strcmp(comando,"clr")==0){
                        resultado=system("clear");

                }else if(strcmp(comando,"dir")==0){
                        strcat(comando," ");
                        strcat(comando,argumentos);
                        strcat(comando," ");
                        strcat(comando,argumentos);
                        resultado=system(comando);

                }else if(strcmp(comando,"environ")==0){
                        comando[0]='\0';
                        strcat(comando,"env ");
                        strcat(comando,argumentos);
                        resultado=system(comando);

                }else if(strcmp(comando,"echo")==0){
                        strcat(comando," ");
                        entrada[strcspn(argumentos, "\t")] = ' ';
                        entrada[strcspn(argumentos, "\n")] = ' ';
                        strcat(comando,argumentos);
                        resultado=system(comando);

                }else if(strcmp(comando,"help")==0){
                        printf(BLANCO_F NEGRO_T "\n **********************************************MANUAL DE USUARIO******************************************************\n");
 				printf("\n By: SEGUNDO SANTENDER PALADINES - VALERIA MIRAMANG \n");
                        printf("\n\n ");
                        printf(" cd <directorio> cambia el directorio actual a <directorio>\n");
                        printf(" dir <directorio>lista el contenido de <directorio>\n");
                        printf(" environ muestra todas las variable de entorno  \n");
                        printf(" echo <comantario> muestra <comentario> en pantalla\n");
                        printf(" help muestra el manual de usuario \n");
                        printf(" pause detiene la ejecucion del shell hasta que se presione la tecla intro \n");
                        printf(" quit sale del shell \n");
                        printf(" ***********************************SI EL COMANDO TERMINA EN: & EL COMANDO SERÁ EJECUTADO SEGUNDO PLANO********************************************\n");
                        printf(" ***********************************OTROS COMANDOS SE EJECUTARAN POR MEDIO DE UN HIJO DE ESTE SHELL************************************************\n");
                        printf(" ***********************************SI OCURRE UN ERROR ES CULPA DE VALERIA*************************************************************************\n");
                        printf(" **************************************************************************************************************************************************\n");
                        printf("\n Enter para continuar....\n");

                        char letra = getchar();
                        while(letra != '\n'){
                                letra = getchar();
                        }

                        resultado=system("man more");
                        printf(RESET_COLOR "\n");

                }else if(strcmp(comando,"pause")==0){
                        printf("Pausado....Presione intro para continuar....");
                        char letra = getchar();
                        while(letra != '\n'){
                                letra = getchar();
                        }

                }else{
                        //Programa con el fork
                        pid_t pid;
                        int status;
                        char *argv[] = {"/home/Spaladines/myshell/myshell2",entrada, NULL};

                        pid=fork();

                        if(pid == 0){
                                printf(MAGENTA_T "\n ********************************** PROCESO HIJO DEL SHELL INICIO ***********************************\n");
                                getcwd(directorio_actual,sizeof(directorio_actual));
                                printf("\n Parent =  %s :", directorio_actual);
                                system(entrada);
                                printf("\n");
                                printf("\n ********************************** PROCESO HIJO DEL SHELL FIN **************************************");
                                printf(RESET_COLOR "\n");
                                exit(1);
                        }else if(pid > 0){
                                wait(&status);
                        }else{
                                printf("Error al crear el hijo");
                        }
                }

        }
 else{
                pthread_t hilo;
                int estado_hilo;
                strcat(comando," ");
                entrada[strcspn(argumentos, "\t")] = ' ';
                entrada[strcspn(argumentos, "\n")] = ' ';
                entrada[strcspn(argumentos, "&")] = ' ';
                strcat(comando,argumentos);

                char caracter=comando[0];
                int contador = 0;
                int cont_validos = 0;
                entrada[0]='\0';
                while(caracter != '\0'){
                        if(caracter != '&'){
                                entrada[cont_validos]=caracter;
                                cont_validos++;
                        }
                        contador++;
                        caracter=comando[contador];
                }
                entrada[cont_validos]='\0';

                estado_hilo = pthread_create(&hilo, NULL, funcion_del_hilo, (void *)comando);
                if (estado_hilo) {
                        printf("Error al crear el hilo. Código de error: %d\n", estado_hilo);
                }
                pthread_join(hilo, NULL);
        }

        return resultado;
}

int main(int argc, char *argv []){

        printf(VERDE_T" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ************************************************  _   _      _ _       *****************************************************\n");
        printf(" ************************************************ | | | | ___| | | ___   ****************************************************\n");
        printf(" ************************************************ | |_| |/ _ \\ | |/ _ \\  ****************************************************\n");
        printf(" ************************************************ |  _  |  __/ | | (_) |  ****************************************************\n");
        printf(" ************************************************ |_| |_|\\___|_|_|\\___/  ****************************************************\n");
        printf(" ************************************************                       *****************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf("\n");
        printf(VERDE_T "\n **********************************************INICIO DEL SHELL******************************************************\n");
 while(1){
                entrada[0] = '\0';
                getcwd(directorio_actual,sizeof(directorio_actual));

                if(argv[1]==NULL){
                        printf(VERDE_T "\n %s :", directorio_actual);
                        fgets(entrada, sizeof(entrada), stdin);
                        entrada[strcspn(entrada, "\n")] = '\0';
                        int resultado=ejecutarComando();
                        if(resultado==-1){break;}
                }else{
                        FILE *archivo;
                        archivo =fopen(argv[1], "r");
                        char bufer[250];
                        char entradas[250][250];
                        int ce=0;

                        while(fgets(bufer, 250, archivo)){

                                strtok(bufer,"\n");
                                char c=bufer[0];
                                int contador = 0;

                                while(c!='\0'){
                                        entradas[ce][contador]=c;
                                        contador++;
                                        c=bufer[contador];
                                }
                                entradas[ce][contador]='\0';
                                ce++;
                        }
                        if(ce > 0){
                                for(int i=0; i<ce; i++){
                                        printf(VERDE_T "\n Ejecutando Comando # %d Comando: %s \n\n" ,i,entradas[i]);
                                        printf("\n_________________________________________________________________________________________________\n");

                                        char c = entradas[i][0];
                                        int contador = 0;
                                        entrada[0]='\0';
                                        while(c!='\0'){
                                                entrada[contador]=c;
                                                contador++;
                                                c = entradas[i][contador];
                                        }

                                        entrada[contador]='\0';
                                        entrada[strcspn(entrada, "\n")] = '\0';
                                        int resultado=ejecutarComando();
                                        if(resultado==-1){break;}
                                        printf("\n_________________________________________________________________________________________________");
                                        printf(RESET_COLOR "\n");
                                }
                        }
                        argv[1]=NULL;
                }
        }

        printf("\n** *************************************************FIN DEL SHELL*************************************************************\n");
        printf("\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" *********************************************** CCCCC  HH   HH      AA      OOOOO  *******************************************\n");
        printf(" ********************************************** CC      HH   HH     AAAA    OO   OO  ******************************************\n");
        printf(" ********************************************** CC      HHHHHHH    AA  AA   OO   OO  ******************************************\n");
        printf(" ********************************************** CC      HH   HH   AAAAAAAA  OO   OO  ******************************************\n");
        printf(" *********************************************** CCCCC  HH   HH  AA      AA  OOOOO  *******************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(" ******************************************************************************************************************************\n");
        printf(RESET_COLOR "\n");

        return 0;
}
