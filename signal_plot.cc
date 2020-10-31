#include "TTree.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
struct spe_header{
  int reclA;            /* 24 */
  unsigned int title[2]; /*don't know why this is uint, but seems to
                           work, so...*/ 
  int dim;
  int a1;               /*  1 */
  int a2;               /*  1 */
  int a3;               /*  1 */
  int reclB;            /* 24 */
};
int read_spectrum(float *spec, int nchs, char *name);
void signal_plot()
{
    Int_t a=0;
    float *spec;
    int nchs=8;
    char name[20]="./firstPulse.spe";
    //char name[50]="/mnt/e/CDEX/ovbb/WaveSimulation/icpc_siggen-master/"
    a=read_spectrum(spec,nchs,name);
    std::cout<<a<<std::endl;

}

int read_spectrum(float *spec, int nchs, char *name){  
  FILE *fp;
  int record_length;
  struct spe_header header;
  char *suffix_start;
  char *fname_start;

  header.reclA = header.reclB = 24; 
  header.title[0] = header.title[1] = 0;
  header.a1 = header.a2 = header.a3 = 1;

  fp = fopen(name,"w");
  if (fp == NULL){
    fprintf(stderr,"Error! Unable to open spectrum file %s \n",name);
    return 0;
  }
  header.dim = nchs;
  if ((suffix_start = strstr(name,".spe")) == NULL){
    suffix_start = name + strlen(name);
  }
  if ((fname_start = rindex(name,'/')) == NULL){
    fname_start = name; 
  }else{
    fname_start++;/*get rid of the '/'*/
  }
  if (suffix_start - fname_start < 8){
    memcpy(header.title,"       ",8);/*blank the title*/
    memcpy(header.title,fname_start,suffix_start - fname_start);
  }else{ 
    memcpy(header.title,suffix_start - 8,8);
  }
  record_length = sizeof(float)*header.dim;

  fseek(fp, 0, SEEK_SET);
  fread(&header, sizeof(header), 1, fp);
  fread(&record_length, sizeof(record_length), 1, fp);
  fread(spec, sizeof(float), nchs, fp); 
  fread(&record_length, sizeof(record_length), 1,fp);
  fclose(fp);

  return 1;
}