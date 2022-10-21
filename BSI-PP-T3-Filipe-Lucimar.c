// Filipe - Lucimar
#include <stdio.h> // Lib padrao
#include <stdlib.h> // Lib padrao
#include <unistd.h> // Lib para funcao sleep
#include <time.h> // Lib manipulacao de data e hora (para funcao srand)
#include <pthread.h> // Lib para thread
#include <semaphore.h> // Lib para semaforo

// Variaveis globais compartilhadas
int a; 
int b;
int n;

// typedef para semaforo
sem_t so, su, mu, di;

// Assinatura das funcoes de chamada de cada thread
void* soma(void* args);
void* subtracao(void* args);
void* multiplicacao(void* args);
void* divisao(void* args);
// Assinatura da funcao para leitura e validacao dos dados do teclado
void entradaDeDados();

int main(void)
{

    // Chamada da funcao para entrada de dados
    entradaDeDados(); 

    // Inicializando gerador de numeros pseudo-aleatorios
    srand(time(NULL));

    // Inicialzando os semaforos, passando como parametro pshared = 0 (para não ser compartilhado entre processos)
    // e value = 0 para aguardar liberacao pela funcao sem_post.
    sem_init(&so, 0, 0);
    sem_init(&su, 0, 0);
    sem_init(&mu, 0, 0);
    sem_init(&di, 0, 0);
    
    // typedef para thread
    pthread_t t1, t2, t3, t4;

    // Criando quatros threads
    pthread_create(&t1, NULL, &soma, NULL);
    pthread_create(&t2, NULL, &subtracao, NULL);
    pthread_create(&t3, NULL, &multiplicacao, NULL);
    pthread_create(&t4, NULL, &divisao, NULL);
    
    // Aguardando o encerramento das quatro threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    // Destruindo os semaforos
    sem_destroy(&so);
    sem_destroy(&su);
    sem_destroy(&mu);
    sem_destroy(&di);

    printf("\n");
    
    return 0;

}

void* soma(void* args)
{

    // Liberando um recurso de utilizacao do semaforo "soma"
    sem_post(&so);
        
    // Armazenando um numero aleatorio de 1 a 10 na variavel dormir
    int dormir = 1 + rand()%10;

    // Imprimindo na tela o tempo que funcao ficara em execucao
    printf("Soma....................dormira por: %d\tseg.\n", dormir);

    // Loop n igual a quantidade de vezes que sera executada a operacao
    for (int i = 0; i < n; i++)
    {
        
        // Aguardando liberacao do semaforo pela funcao sem_post
        sem_wait(&so);
        // Armazenando na variavel result o resultado da operacao
        int result = a+b;
        // Dormindo o tempo definido pela variavel dormir
        sleep(dormir);
        // Liberando recurso para o semaforo "subtracao"
        sem_post(&su);

        // Imprimindo na tela o resultado da opercao e o tempo que ficou em execucao
        printf("\nSoma = %d\t\tPassaram-se %d\tseg.\n", result, dormir);

    }    

    // Encerrando a thread de chamada
    pthread_exit(NULL);

}

void* subtracao(void* args)
{

    int dormir = 1 + rand()%10;

    printf("Subtracao...............dormira por: %d\tseg.\n", dormir);

    for (int i = 0; i < n; i++)
    {

        sem_wait(&su);
        int result = a-b;
        sleep(dormir);
        sem_post(&mu);
        
        printf("Subtracao = %d\t\tPassaram-se %d\tseg.\n", result, dormir);
        
    }    

    pthread_exit(NULL);

}

void* multiplicacao(void* args)
{

    int dormir = 1 + rand()%10;

    printf("Multiplicacao...........dormira por: %d\tseg.\n", dormir);

    for (int i = 0; i < n; i++)
    {

        sem_wait(&mu);
        int result = a*b;
        sleep(dormir);
        sem_post(&di);

        printf("Multiplicacao = %d\tPassaram-se %d\tseg.\n", result, dormir);

    }    

    pthread_exit(NULL);

}

void* divisao(void* args)
{

    int dormir = 1 + rand()%10;

    printf("Divisao.................dormira por: %d\tseg.\n", dormir);

    for (int i = 0; i < n; i++)
    {

        sem_wait(&di);
        float result = (float)a/b;
        sleep(dormir);
        sem_post(&so);
        
        printf("Divisao = %.1f\t\tPassaram-se %d\tseg.\n", result, dormir);

    }    

    pthread_exit(NULL);

}

void entradaDeDados()
{

    // Limpando a tela
    system("clear");
    // Imprimindo na tela a solicitacao de entrada de dados
    printf("Digite dois numeros, A e B, sendo (B != 0):\n");
    printf("A = ");
    // Lendo os dados de entrada pelo teclado
    scanf("%d", &a);
    printf("B = ");
    do
    {
        scanf("%d", &b);
        // Verificando a condicao do valor de B ser diferente de 0,
        // se for igual solicita um novo numero
        if (b == 0)
        {
            printf("ERRO - B nao pode ser 0.\n");
            printf("Digite outro numero:\nB = ");
        }
        
    } while (b == 0);
    
    // Imprimindo na tela a solicitacao de entrada de dados
    printf("Quantas vezes deseja executar o programa: ");

    do
    {
        scanf("%d", &n);
        if (n < 1)
        {
            printf("ERRO - Nao pode ser nenor que 1.\n");
            printf("Digite outro numero: ");
        }
        
    } while (n < 1);

    printf("\n");    
    
}