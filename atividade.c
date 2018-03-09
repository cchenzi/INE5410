#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Autor Francisco Vicenzi


double calculaMedia(int *V, int tamanho);
double calculaDesvioPadrao(int *V, int tamanho, double media);



int main(int argc, char *argv[]){
  if(argc==1){
    printf("Sem argumentos!\n");
  }else{

  printf("%s","teste\n");



  srand(time(NULL));
  int tamanho = atoi(argv[1]);
  printf("Tamanho do vetor:%d\n", tamanho);
  //Alocação de memória
  int *V = malloc(tamanho*sizeof(int));
  //Inicialização do vetor(entre 0 e 300)
  for(int i=0;i<tamanho;i++){
    V[i] = 1 + (rand() %300);
  }

  for(int i=0;i<tamanho;i++){
    printf("Valor: %d\n", V[i]);
  }

  double media = calculaMedia(V, tamanho);
  printf("Média: %g\n", media );

  double desvioPadrao = calculaDesvioPadrao(V, tamanho, media);
  printf("Desvio Padrão: %g\n", desvioPadrao );

  free(V);
}


  return 0;
}

double calculaMedia(int *V, int tamanho){
  double valorMedia = 0;
  for(int i=0;i<tamanho;i++){
    valorMedia += V[i];
  }
  return valorMedia/tamanho;

}

double calculaDesvioPadrao(int *V, int tamanho, double media){
  double valorDesvio = 0;
  for(int i=0;i<tamanho;i++){
    valorDesvio += pow((media-V[i]),2);
  }

  return sqrt(valorDesvio/tamanho);
}
