#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct arquivoProcessados{

    char nome[51];
    uint32_t numeroPag;

}arquivo;

typedef struct filas{

    uint32_t capacidade;
    uint32_t inicio;
    uint32_t fim;
    uint32_t tamanho;
    arquivo *vetor;

}fila;

typedef struct pilhas{

    uint32_t capacidade;
    uint32_t topo;
    arquivo *vetor;

}pilha;

typedef struct listas{

    struct arquivoProcessados elemento;
    struct listas *prox;
    

}lista;

typedef struct ponteiroListas{

    lista *cabeca;
    
}ponteiroLista;

typedef struct impressoras{

    char nome[51];
    arquivo elemento;
    ponteiroLista *arquivoProcessados;
  
}impressora;


impressora *initImpressora(char *nome){
    impressora *newImpressora = (impressora*)malloc(sizeof (impressora));
    strcpy(newImpressora->nome, nome);
    newImpressora->arquivoProcessados = (ponteiroLista*)malloc(sizeof(ponteiroLista));
    newImpressora->arquivoProcessados->cabeca = NULL;

    return newImpressora;
}

fila *initFila(uint32_t capacidade){

    fila *newFila = (fila*)malloc(sizeof(fila));
    newFila->capacidade = capacidade;
    newFila->inicio = 0;
    newFila->fim = 0;
    newFila-> tamanho = 0;
    newFila->vetor = (arquivo*)malloc(capacidade*(sizeof(arquivo)));
    
    return newFila;
}

void *initPilha(uint32_t capacidade){
    pilha *newPilha = (pilha*)malloc(sizeof(pilha));
    newPilha->topo = -1;
    newPilha->vetor = (arquivo*)malloc(capacidade*(sizeof(arquivo)));
    newPilha->capacidade = capacidade;
}

void addArquivoLista(impressora *impressora, arquivo arquivo, FILE **output){
   
    lista *arquivoProcessado = (lista*)malloc(sizeof(lista));
    arquivoProcessado->elemento = arquivo;
    if(impressora->arquivoProcessados->cabeca == NULL){

        arquivoProcessado->prox =  NULL;
        impressora->arquivoProcessados->cabeca = arquivoProcessado;
        
    }
    else{
        
        arquivoProcessado->prox = impressora->arquivoProcessados->cabeca;
        impressora->arquivoProcessados->cabeca = arquivoProcessado;
              
        lista *aux = impressora->arquivoProcessados->cabeca;
        aux = aux->prox;
        
        fprintf(*output,"[%s]",impressora->nome);
        while(aux != impressora->arquivoProcessados->cabeca){
            fprintf(*output," %s-%dp, ",aux->elemento.nome, aux->elemento.numeroPag);
            aux = aux->prox;
        }
        fprintf(*output, "%s-%dp\n", aux->elemento.nome, aux->elemento.numeroPag);
    }
}

//Adiciona na fila
void addArquivoFila(fila *fila, char *nome, int numeroPag){

    if(fila->capacidade != fila->tamanho){

        arquivo newArquivo;
        strcpy(newArquivo.nome, nome);
        newArquivo.numeroPag = numeroPag;
        fila->vetor[fila->fim] = newArquivo;
        fila->fim += 1;
        fila->tamanho += 1;

    }
    
    if(fila->capacidade == fila->fim){
        fila->fim = 0;
    }
}

void addArquivoPilha(pilha *pilha, arquivo arquivo){

    if(pilha->capacidade-1 != pilha->topo){
        pilha->topo += 1;
        pilha->vetor[pilha->topo] = arquivo;
    }
}

// Remove da fila
arquivo removerArquivoFila(fila *fileira){
    
    if(fileira->tamanho > 0){
        fileira->inicio += 1;
        fileira->tamanho -= 1;
        return fileira->vetor[fileira->inicio-1];
    }
    
    if(fileira->capacidade == fileira->inicio){
        fileira->inicio = 0;
    }
    

}

void showArquivosPilha(pilha *pilha, FILE **output){
    
    while (pilha->topo != -1){
        fprintf(*output, "%s-%dp\n",pilha->vetor[pilha->topo].nome,pilha->vetor[pilha->topo].numeroPag);
        pilha->topo -= 1;
    }
    free(pilha);
    
}


int main(int argc, char const *argv[])
{
    FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    uint32_t numeroImpressoras, numeroPag, numeroarquivoProcessados, numeroPagImpressas = 0;
    char nomeImpressora[51], nomeArquivo[51];
 
    fscanf(input, "%d", &numeroImpressoras);
    
    impressora *impressoras[numeroImpressoras];
    //CRIA O NUMERO DE IMPRESSORAS
    for(int i = 0 ; i < numeroImpressoras ;  i++){
        fscanf(input, "%s", nomeImpressora);
        impressoras[i] = initImpressora(nomeImpressora);
       
    }
    //CAPTURA O NUMERO DE arquivoProcessados E CRIA A FILA E A PILHA
    fscanf(input, "%d", &numeroarquivoProcessados);
    
    fila *fileira = initFila(numeroarquivoProcessados);
    pilha *empilhado = initPilha(numeroarquivoProcessados);
    
    //CRIA OS arquivoProcessados
    for(int i = 0 ; i < numeroarquivoProcessados ; i++){
        fscanf(input, "%s %d", nomeArquivo, &numeroPag);
        addArquivoFila(fileira, nomeArquivo, numeroPag);
        numeroPagImpressas += numeroPag;
    }

    uint32_t vetorNumbPag[numeroImpressoras]; 
     
    for(int i = 0 ; i < numeroImpressoras ;  i++){
        impressoras[i]->elemento = removerArquivoFila(fileira);
        vetorNumbPag[i] = impressoras[i]->elemento.numeroPag;
        fprintf(output,"[%s] %s-%dp\n", impressoras[i]->nome, impressoras[i]->elemento.nome, impressoras[i]->elemento.numeroPag);
    }
    
   
    while(fileira->tamanho != 0){
       
        for(int i = 0 ; i < numeroImpressoras ;  i++){
            
            if(vetorNumbPag[i] == 0){
                addArquivoLista(impressoras[i], impressoras[i]->elemento, &output);
                addArquivoPilha(empilhado, impressoras[i]->elemento);
                impressoras[i]->elemento = removerArquivoFila(fileira);
                vetorNumbPag[i] = impressoras[i]->elemento.numeroPag;
            }
            vetorNumbPag[i] -= 1;
            
        }
    }
    for(int i = 0 ; i <  numeroImpressoras ;  i++){
        addArquivoLista(impressoras[i], impressoras[i]->elemento, &output);
        addArquivoPilha(empilhado, impressoras[i]->elemento);
    }        
    
    fprintf(output, "%dp\n", numeroPagImpressas);
    showArquivosPilha(empilhado, &output);

    fclose(input);
	fclose(output);
    return 0;
}
