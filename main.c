#include <stdio.h>
#include <stdlib.h>
 //Caio Sobrinho da Silva - RA 211044156
 //BCC 021
typedef struct pessoa //struct pra pessoa, com idade/tempo de chegada/tempo de atendimento
{
    int idade;
    int chegada; 
    int tempo;
    struct pessoa *prox;
    struct pessoa *ant; 
} p;

typedef struct caixa //struct caixa com struct pessoa para cliente
{
    p *cliente;
    struct caixa *prox;    
} c;

p *create(int idade, int chegada, int tempo){ //cria lista duplamente encadeada
    p *pessoa = malloc(sizeof(p));

    pessoa->idade = idade;
    pessoa->chegada = chegada;
    pessoa->tempo = tempo;
    pessoa->ant = NULL;
    pessoa->prox = NULL;

    return pessoa;} //retorna pessoa

void ordemChegada(p **lista, p *pessoa){ //insere em ordem de chegada
    if(*lista == NULL){
        *lista = pessoa; //se lista for NULL, insere a lista pessoa nela 
    }
    else{ //caso nao, insere lista no valor auxiliar para funcao
        p *aux = *lista;
        p *ant = NULL; //nó anterior recebe NULL

        while(aux != NULL && aux->chegada < pessoa->chegada){ //enquanto aux for diferente de NULL e menor que a lista atual, realiza
            ant = aux; //nó anterior recebe auxiliar, pois é menor que a lista "pessoa"
            if(aux->chegada == pessoa->chegada && aux->tempo < pessoa->tempo){ //caso o tempo de chegada for igual e o tempo gasto for diferente, sai do loop
                break;
            }
            aux = aux->prox; //aux recebe o valor do proximo nó
        }
        pessoa->prox = aux; //nó prox recebe o valor de auxiliar 

        if(ant == NULL)//se o no anterior for nulo, a lista recebe a fila nova "pessoa"
        {
            *lista = pessoa;
        }
        else
        {
            ant->prox = pessoa; //caso nao for nulo, o proximo recebe o valor da lista 
        }
    }
}

void box(c **top)//criar os caixas
{
    c *cx = malloc(sizeof(c)); //memory allocation
    cx->cliente = NULL; //caixa cliente fica vazio

    if(*top == NULL) //se o topo da lista for NULL, recebe os dados do caixa
    {
        *top = cx; 
    }
    else //se nao, o proximo no recebe o topo de lista
    {
        cx->prox = *top;
        *top = cx;    
    }
}

void ordemIdade(p *pessoa, p **inicio, p **fim){ //insere por prioridade de idade

    if((*inicio) != NULL && *fim != NULL){
        p *aux = (*inicio); //auxiliar recebe valor de inicio

        while(aux->idade > pessoa->idade && aux->ant != NULL){
            aux = aux->ant;
        }

        if(aux->ant == NULL && aux->idade > pessoa->idade){ //entra no fim da fila 
            aux->ant = pessoa;
            pessoa->prox = aux;
            *fim = pessoa;
        }
        else if(aux->prox == NULL && pessoa->idade > aux->idade){ //começo da fila
            aux->prox = pessoa;
            pessoa->ant = aux;
            (*inicio) = pessoa;
        }
        else //meio da fila
            pessoa->prox = aux->prox;
            pessoa->ant = aux;
            aux->prox->ant = pessoa;
            aux->prox = pessoa;
    }   
    if((*inicio) == NULL){
        *inicio = pessoa;
    }
    if((*fim) == NULL){
        *fim = pessoa;
    }}

void insert(p *pessoa, p **inicio, p **fim){ //inserir na fila sem prioridade, normal

    if((*inicio) != NULL && (*fim) != NULL){ //verifica se é NULL, e realiza a passagem para o final da fila 
        (*fim)->ant = pessoa;
        pessoa->prox = *fim;
        *fim = pessoa;
    }

    if((*inicio) == NULL){ //NULL significa que nao tem ninguem na fila, entao recebe outra pessoa no lugar
        *inicio = pessoa;
    }
    if(*fim == NULL){
        *fim = pessoa;
    }}

void newBox(p **inicio, p **fim, c **caixa) //coloca uma pessoa no caixa disponivel
{
    c *aux = *caixa;
    
    if((*inicio) != NULL){
        while(aux != NULL)
        {
            if(aux->cliente == NULL)
            {
                aux->cliente = *inicio;
                *inicio = (*inicio)->ant;

                if((*inicio) != NULL)
                {
                    (*inicio)->prox = NULL;
                }
                else
                {
                    *fim = NULL;
                    break;
                }
            }
            aux = aux->prox;}}}

int menorTempo(c **caixa, int i, int menor)
{
    //acha o menor tempo gasto nos caixas e retorna esse valor
    i = 0;
    menor = 0;
    c *aux = *caixa;

    while(aux != NULL)
    {
        if(aux->cliente != NULL)
        {
            if(i == 0)
            {
                menor = aux->cliente->tempo;
                i++;  
            }
            else if(aux->cliente->tempo < menor)
            {
                menor = aux->cliente->tempo;
            }
        }
        aux = aux->prox;
    }
    return menor;
}

void outBox(int i,int menor, int menortempo, int time, p **inicio, p **fim, c **caixa){ //funcao para tirar a pessoa do caixa eletronico
    c *aux = *caixa;
    i = 0;
    menortempo = 0;

    while(time > 0){
        aux = *caixa;
        menortempo =  menorTempo(caixa, i, menor);

        if(menortempo != 0 && menortempo <= time){ //tira alguem da fila durante o tempo
            while(aux != NULL){
                if(aux->cliente != NULL){
                    aux->cliente->tempo -= menortempo;

                    if(aux->cliente->tempo <= 0){
                        aux->cliente = NULL; //caixa fica livre e depois vai receber o proximo
                        newBox(inicio, fim, caixa);
                    }}
                aux = aux->prox;}
            time -= menortempo;}
        else{ 
            while(aux != NULL){
                if(aux->cliente != NULL){
                    aux->cliente->tempo -= time;
                }
                aux = aux->prox;
            }
            break;
        }}
    newBox(inicio, fim, caixa);}

void listar(p *top){ //printar a lista das idades
    if(top == NULL){
        printf("\nNULL");
    }
    else{
        while(top != NULL){
            printf("\n%d\n ", top->idade);
            top = top->ant;
        }
    }
}

int main(){
    int n, m;
    int i;
    int idade, chegada, tempo;

    p *lista = NULL;
    //entrada de dados
    printf("\nDigite o numero de pessoas e o de caixas: ");
    scanf("%d %d", &n, &m);

    for(i=0; i<m; i++){
        printf("\nDigite a idade, o tempo de chegada, e o tempo gasto: \n");
        scanf("%d %d %d", &idade, &chegada, &tempo);
        ordemChegada(&lista, create(idade, chegada, tempo));
    }

    c *cx = NULL;
    
    for(i=0; i<m; i++){
        box(&cx);
    }

    int time=0, menor, menortempo;

    p *inicio = NULL;//o final da lista
    p *fim = NULL;//comeco da lista
    p *local = NULL;//endereco da fila 
    p *aux = lista;//valor auxiliar 

    while(aux != NULL){
        local = create(aux->idade, aux->chegada, aux->tempo); 
        //recebe os dados da pessoa da fila, e faz as verificações 
        
        if(local->idade > 63) //verifica ordem de idade 
        {
            ordemIdade(local, &inicio, &fim);
        }  
        else
        {
            insert(local, &inicio, &fim);
        }

        outBox(i, menor, menortempo, aux->chegada - time, &inicio, &fim, &cx); //tira o usuario do caixa 
        time = aux->chegada; //time recebe o tempo de chegada do cliente 

        if(aux->prox != NULL && aux->prox->chegada != aux->chegada){ //printa uma vez só
            listar(inicio);
        }
        if(aux->prox == NULL){
            listar(inicio);
        }
        aux = aux->prox;   
    }
    return(0);
}