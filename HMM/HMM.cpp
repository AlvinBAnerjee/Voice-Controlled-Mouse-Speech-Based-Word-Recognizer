// HMM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <windows.h>

#include <string.h>

using namespace std;
namespace patch//utility namespace to use certain overloaded functions
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
double normalisation_factor = 0;
double min_[]={9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,9999,9999};
double avg[]={0,0,0,0,0,0,0,0,0,0,0,0};
double max_[]={0,0,0,0,0,0,0,0,0,0,0,0};
int ttrows=0;
void train()
{
	char commands[5][40] = {
                             "left",
                             "right",
                             "up",
							 "down",
							 "click"
                         };
    char c[150];
	for(int j=0;j<5;j++){
    for(int i=1; i<=30; i++)
    {
		printf("Say %s for utterance %d",commands[j],i);
        sprintf(c,"Recording_Module.exe 3 %s_%d.wav %s_%d.txt",commands[j],i,commands[j],i);
        system(c);
    }
	}
    
}

void cursorOperation(int movement)
{
    POINT xypos;
    GetCursorPos(&xypos);
            //cout << "0.Left\n1.Right\n2.Up\n3.Down\n4.Click\n\n";
            //cout << "Current position\nX: "<<xypos.x << "\tY: "<<xypos.y<<endl;
            //cout<<"X:"<<xypos.x<<"\tY:"<<xypos.y<<endl;
            //cout << "Enter the movement as number...\n";
            //cin >> movement;
            if(movement == 0)
               { 
				   for(int i=0;i<200;i++)
				  { 
					  SetCursorPos(xypos.x-1,xypos.y);
					  GetCursorPos(&xypos);
					  for (int c = 1; c <= 32767; c++)
					 for (int d = 1; d <= 100; d++)
						{}
				   }
			}
            else if(movement == 1)
                {
					for(int i=0;i<200;i++){
					SetCursorPos(xypos.x+1,xypos.y);
					GetCursorPos(&xypos);
					for (int c = 1; c <= 32767; c++)
					for (int d = 1; d <= 100; d++)
						{}
					}
			}
            else if(movement == 2)
                {
					for(int i=0;i<200;i++){
					SetCursorPos(xypos.x,xypos.y-1);
					GetCursorPos(&xypos);
					for (int c = 1; c <= 32767; c++)
					for (int d = 1; d <= 100; d++)
						{}
					}
			}
            else if(movement == 3)
			{
				for(int i=0;i<200;i++){
				SetCursorPos(xypos.x,xypos.y+1);
				GetCursorPos(&xypos);
					for (int c = 1; c <= 32767; c++)
					for (int d = 1; d <= 100; d++)
						{}
				}
				}
			else if(movement == 4)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                cout << "click happened !" << endl;
                Sleep(1); //you can adjust the speed of the click here
            }
}
float calculate_normalisation(char *ch) {
    FILE *f = fopen(ch, "r");
	if(f==0)
		{printf("Unable to open file %s",ch);
	}
    int count = 0;
    int max = -1;
    int min = +1;
    int temp = 0;
    while (!f) {
        count++;
        fscanf(f, "%d", &temp);

        if (temp > max)
            max = temp;
        if (temp < min)
            min = temp;


    }
    min = -min;
    if (min > max)
        max = min;
    fclose(f);
    return ((float) 5000 / (float) max);
}//function for the calculation of narmalisation factor


double *getr(double *x) {
    double *r = (double *) malloc(sizeof(double) * 13);
    for (int i = 0; i <= 12; ++i) {
        r[i] = 0;
        for (int k = 0; k <= 320 - 1 - i; ++k) {
            r[i] = r[i] + x[k] * x[k + i];
        }
    }
    return r;
}//generating r values from c values

double *calculateA(double *r) {
    int p = 12;
    double e[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double k[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double alpha[13][13] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    double *a = (double *) malloc(sizeof(double) * (p + 1));
    e[0] = r[0];
    for (int i = 1; i <= 12; ++i) {
        double sum = 0;
        for (int j = 1; j <= i - 1; ++j) {
            double temp = alpha[i - 1][j] * r[i - j];
            sum = sum + temp;
        }
        k[i] = (r[i] - sum);
        k[i] = k[i] / e[i - 1];
        alpha[i][i] = k[i];
        for (int j = 1; j <= i - 1; ++j) {
            {
                double temp = k[i] * alpha[i - 1][i - j];
                alpha[i][j] = alpha[i - 1][j] - temp;
            }
        }
        e[i] = (1 - k[i] * k[i]) * e[i - 1];
    }

    for (int i = 1; i <= p; ++i) {
        a[i] = alpha[p][i];
    }
    return a;
}//generating A values from r

double *calculateC(double *x, double *r, double *a) {
    int p = 12;
    double *c = (double *) malloc(sizeof(double) * (p + 1));
    double e[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    c[0] = log10(r[0] * r[0]);
    double sum = 0;
    for (int i = 1; i <= p; i = i + 1) {
        sum = 0;
        c[i] = a[i];
        for (int k = 1; k <= i - 1; k++) {
            double temp = ((double) k / ((double) i)) * c[k] * a[i - k];
            sum = sum + temp;
        }
        c[i] = c[i] + sum;
    }
    return c;
}//generating Ceptral coefficients from A

void generateRefFile() {
    char name[40];
    sprintf(name, "Universe.txt");
    FILE *writer = fopen(name, "w+");
    for (int ch = 0; ch < 5; ++ch) {
		char *command;
		if(ch==0)
			command="left";
		else if(ch==1)
			command="right";
		else if(ch==2)
			command="up";
		else if(ch==3)
			command="down";
		else
			command="click";
        for (int file_no = 1; file_no <= 30; file_no++) {
            sprintf(name, "%s_%d.txt", command, file_no);
			//printf("%s",name);
            normalisation_factor = calculate_normalisation(name);
            FILE *fp = fopen(name, "r");
            if (fp == NULL) {
                perror("Error");//if error exists then specify the error
            } else { ;//printf("started\n %d %d",ch,file_no);
            }
            int container = 0;
            double *x = (double *) malloc(320 * sizeof(double));
            int counter = 100;
            while (counter-->0) {
                fscanf(fp, "%d", &container);

            }
            int check=1;
            while (check>0){
                for (int i = 0; i < 320; ++i) {
                    if(feof (fp))
                    {
                        check=0;
                        //printf("Finished reading %s\n",name);
                        break;
                    }
                    fscanf(fp, "%d", &container);

//                    if (container*container<1000000)
//                        break;
                    x[i] = (container * normalisation_factor)*0.1;
                }

                double *r = getr(x);
                double *a = calculateA(r);
                double *c = calculateC(x, r, a);
                for (int i = 1; i < 13; ++i) {

                    c[i] = c[i] * (1 + 6 * sin((i * 3.14) / 12));
                    fprintf(writer, "%lf ", c[i]);
                }
                fprintf(writer, "\n");

                free(r);
                free(a);
                free(c);

            }
            free(x);
			fclose(fp);
        }
    }
    fclose(writer);
    sprintf(name, "Universe.txt");
    writer = fopen(name, "a");
    for (int ch = 0; ch < 5; ++ch) {
		char *command;
		if(ch==0)
			command="left";
		else if(ch==1)
			command="right";
		else if(ch==2)
			command="up";
		else if(ch==3)
			command="down";
		else
			command="click";
        for (int file_no = 1; file_no <= 30; file_no++) {
            sprintf(name, "%s_%d.txt", command, file_no);
			//printf("%s",name);
            normalisation_factor = calculate_normalisation(name);
            FILE *fp = fopen(name, "r");
            if (fp == NULL) {
                perror("Error");//if error exists then specify the error
            } else { ;//printf("started\n %d %d",ch,file_no);
            }
            int container = 0;
            double *x = (double *) malloc(320 * sizeof(double));
            int counter = 180;
            while (counter-->0) {
                fscanf(fp, "%d", &container);

            }
            int check=1;
            while (check>0){
                for (int i = 0; i < 320; ++i) {
                    if(feof (fp))
                    {
                        check=0;
                        //printf("Finished reading %s\n",name);
                        break;
                    }
                    fscanf(fp, "%d", &container);

                    //                    if (container*container<1000000)
                    //                        break;
                    x[i] = (container * normalisation_factor)*0.1;
                }

                double *r = getr(x);
                double *a = calculateA(r);
                double *c = calculateC(x, r, a);
                for (int i = 1; i < 13; ++i) {

                    c[i] = c[i] * (1 + 6 * sin((i * 3.14) / 12));
                    fprintf(writer, "%lf ", c[i]);
                }
                fprintf(writer, "\n");

                free(r);
                free(a);
                free(c);

            }
            free(x);
			fclose(fp);
        }
    }
    fclose(writer);
	sprintf(name, "Universe.txt");
    writer = fopen(name, "a");
    for (int ch = 0; ch < 5; ++ch) {
		char *command;
		if(ch==0)
			command="left";
		else if(ch==1)
			command="right";
		else if(ch==2)
			command="up";
		else if(ch==3)
			command="down";
		else
			command="click";
        for (int file_no = 1; file_no <= 30; file_no++) {
            sprintf(name, "%s_%d.txt", command, file_no);
			//printf("%s",name);
            normalisation_factor = calculate_normalisation(name);
            FILE *fp = fopen(name, "r");
            if (fp == NULL) {
                perror("Error");//if error exists then specify the error
            } else { ;//printf("started\n %d %d",ch,file_no);
            }
            int container = 0;
            double *x = (double *) malloc(320 * sizeof(double));
            int counter = 260;
            while (counter-->0) {
                fscanf(fp, "%d", &container);

            }
            int check=1;
            while (check>0){
                for (int i = 0; i < 320; ++i) {
                    if(feof (fp))
                    {
                        check=0;
                        //printf("Finished reading %s\n",name);
                        break;
                    }
                    fscanf(fp, "%d", &container);

                    //                    if (container*container<1000000)
                    //                        break;
                    x[i] = (container * normalisation_factor)*0.1;
                }

                double *r = getr(x);
                double *a = calculateA(r);
                double *c = calculateC(x, r, a);
                for (int i = 1; i < 13; ++i) {

                    c[i] = c[i] * (1 + 6 * sin((i * 3.14) / 12));
                    fprintf(writer, "%lf ", c[i]);
                }
                fprintf(writer, "\n");

                free(r);
                free(a);
                free(c);

            }
            free(x);
			fclose(fp);
        }
    }
    fclose(writer);


}//Generating the Universe after rading all the input files and frames and then generating the appropriate Ceptral Coefficients
double ** readUniverse()
{
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[255];
    int total_rows=0;
    filePointer = fopen("Universe.txt", "r");
    while(fgets(buffer, bufferLength, filePointer)) {
        total_rows++;
    }
    ttrows=total_rows;
    fclose(filePointer);


    double **x=(double **) malloc((sizeof (double *)*total_rows));
    for (int i = 0; i < total_rows; ++i) {
        x[i]=(double *) malloc(12*sizeof (double ));
    }
    FILE *fptr;
    fptr = fopen("Universe.txt", "r");
    if (fptr == NULL) {
        perror("Error");//if error exists then specify the error
    } else { ;//printf("started\n");
    }
    double temp1=0;
    for (int i = 0; i < total_rows; ++i) {
        for (int j = 0; j < 12; ++j) {
            fscanf(fptr, "%lf", &temp1);
            x[i][j]=temp1;
            if (x[i][j]>max_[j])
                max_[j]=x[i][j];

            if (x[i][j]<min_[j])
                min_[j]=x[i][j];

            avg[j]=avg[j]+x[i][j]/ttrows;
        }
    }
    return x;


}//reading the universe.txt file and saving in an array

double findTokuras(double *TestFrame1, double * TestFrame2) {
    double w[] = {1, 3, 7, 13, 19, 22, 25, 33, 42, 50, 56, 61};
    double tokuras_Avg = 0;

    double sum = 0;
    for (int j = 1; j <= 12; ++j) {
        sum = sum + w[j-1] * (TestFrame1[j - 1] - TestFrame2[j - 1]) *
                (TestFrame1[j - 1] - TestFrame2[j - 1]);
    }

    return sum;
}//used to calculate Tokuras distance between 2 frames

int findCluster(double ** universe,int row,double ** codebook,int k)
{
    int min=-1;
    long double dis=9999999999999;
    for (int i = 0; i < k; ++i) {
        long double temp=findTokuras(universe[row],codebook[i]);
        if (temp<dis)
        {
            dis=temp;
            min=i;
        }
    }
    return min;
}//finding the closest cluster

void finalCodebookUsingKmeans(double **codebook,double **universe,int k)
{
    int m=0;
    long double dist=99999999;
    long double newDist=999999;

    while (m<1000&&(dist-newDist)>0.0001)
    {
        dist=newDist;
        newDist=0;
        double newcodebook[32][12]={{0}};
        double count[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        for (int i = 0; i < ttrows; ++i) {
            int cluster=findCluster(universe,i,codebook,k);
            if (cluster==-1)
            {
                printf("Error");
                break;
            }
            count[cluster]++;
            for (int j = 0; j < 12; ++j) {
                newcodebook[cluster][j]=newcodebook[cluster][j]+universe[i][j];
            }
            newDist=newDist+ findTokuras(codebook[cluster],universe[i])/ttrows;

        }
        for (int i=0;i<k;i++)
        {
            for(int j=0;j<12;j++)
            {
                codebook[i][j]=newcodebook[i][j]/count[i];
            }
        }
        //printf("%Lf\n",newDist);
        m++;
    }

}

double ** splitCodeBook(double **codebook,int k)//codebook spliting utilityt function for LBG
{
    int count=0;
    double **x=(double **) malloc((sizeof (double *)*k*2));
    for (int i = 0; i < k*2; ++i) {
        x[i]=(double *) malloc(12*sizeof (double ));
    }
    for (int i = 0; i < k; ++i) {
        for (int j=0;j<12;j++)
        {
            x[count][j]=codebook[i][j]*(1-0.03);
            x[count+1][j]=codebook[i][j]*(1+0.03);
        }
        count=count+2;
    }
    for (int i = 0; i < k; ++i) {
        free(codebook[i]);
    }
    return x;
}
double ** finalCodebookUsingLBG(double **universe)
{
    double **codebook=(double **) malloc((sizeof (double *)));
    codebook[0]=(double *) malloc(12*sizeof (double ));
    for(int i=0;i<12;i++)
        codebook[0][i]=avg[i];
    int k=1;
    while (k<32)
    {
        codebook= splitCodeBook(codebook,k);
        k=k*2;
        finalCodebookUsingKmeans(codebook,universe,k);
        printf("..............\n");
    }
    return codebook;
}
void writecodebook(double ** codebook)//This is used to save the codebook generated
{
    char name[40];
    sprintf(name, "codebook.txt");
    FILE *writer = fopen(name, "w+");
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j <12 ; ++j) {
            fprintf(writer, "%lf ", codebook[i][j]);
        }
        fprintf(writer,"\n");
    }
}

int findClusterUtil(double *frame,double ** codebook,int k)
{
    int min=-1;
    long double dis=9999999999999;
    for (int i = 0; i < k; ++i) {
        long double temp=findTokuras(frame,codebook[i]);
        if (temp<dis)
        {
            dis=temp;
            min=i;
        }
    }
    return min;
}

void generateObservationSeq(double **codebook) {//generate observation sequence from the codebook

    for (int ch = 0; ch < 5; ++ch) {
		char *command;
		if(ch==0)
			command="left";
		else if(ch==1)
			command="right";
		else if(ch==2)
			command="up";
		else if(ch==3)
			command="down";
		else
			command="click";
        for (int file_no = 1; file_no <= 30; file_no++) {
            char name[40];
            sprintf(name, "OBS%s_%d.txt",command,file_no);
            FILE *writer = fopen(name, "w+");
            sprintf(name, "%s_%d.txt", command, file_no);
			//printf("%s\n",name);
            normalisation_factor = calculate_normalisation(name);
			//printf("yo %s",name);
            FILE *fp = fopen(name, "r");
            if (fp == NULL) {
                perror("Error");//if error exists then specify the error
            } else { ;//printf("started\n");
            }
            int container = 0;
            double *x = (double *) malloc(320 * sizeof(double));
            int counter = 100;
            while (counter-->0) {
                fscanf(fp, "%d", &container);


            }
            int check=1;
            while (check>0){
                check++;
                for (int i = 0; i < 320; ++i) {
                    if(feof (fp))
                    {
                        check=0;
                        break;
                    }
                    fscanf(fp, "%d", &container);
                    //                    if (container*container<1000000)
                    //                        break;
                    x[i] = (container * normalisation_factor)*0.1;
                }

                double *r = getr(x);
                double *a = calculateA(r);
                double *c = calculateC(x, r, a);
                for (int i = 1; i < 13; ++i) {

                    c[i] = c[i] * (1 + 6 * sin((i * 3.14) / 12));
                }
                double *c_ = (double *) malloc(sizeof(double) * 12);
                c_[0]=c[1];
                c_[1]=c[2];
                c_[2]=c[3];
                c_[3]=c[4];
                c_[4]=c[5];
                c_[5]=c[6];
                c_[6]=c[7];
                c_[7]=c[8];
                c_[8]=c[9];
                c_[9]=c[10];
                c_[10]=c[11];
                c_[11]=c[12];
                int obs=findClusterUtil(c_,codebook,32);
                fprintf(writer, "%d ",(obs+1));

                free(r);
                free(a);
                free(c);

            }
            fprintf(writer, "\n");
            free(x);
            fclose(writer);
        }

    }

}

void generateObservationSeqOnline(double **codebook) {

    
            char name[40]; sprintf(name, "OBSinput.txt"); FILE *writer = fopen(name, "w+");
            sprintf(name, "input_file.txt"); normalisation_factor = calculate_normalisation(name);
            FILE *fp = fopen(name, "r");
            if (fp == NULL) {
                perror("Error");//if error exists then specify the error
            } else { ;//printf("started\n");
            }
            int container = 0;
            double *x = (double *) malloc(320 * sizeof(double));
            int counter = 100;
            while (counter-->0) {
                fscanf(fp, "%d", &container);


            }
            int check=1;
            while (check>0){
                check++;
                for (int i = 0; i < 320; ++i) {
                    if(feof (fp))
                    {
                        check=0;
                        break;
                    }
                    fscanf(fp, "%d", &container);
                    x[i] = (container * normalisation_factor)*0.1;
                }

                double *r = getr(x);
                double *a = calculateA(r);
                double *c = calculateC(x, r, a);
                for (int i = 1; i < 13; ++i) {

                    c[i] = c[i] * (1 + 6 * sin((i * 3.14) / 12));
                }
                double *c_ = (double *) malloc(sizeof(double) * 12);
                c_[0]=c[1];c_[1]=c[2];c_[2]=c[3];c_[3]=c[4];c_[4]=c[5];c_[5]=c[6];c_[6]=c[7];c_[7]=c[8];c_[8]=c[9];c_[9]=c[10];c_[10]=c[11];c_[11]=c[12];
                int obs=findClusterUtil(c_,codebook,32);
                fprintf(writer, "%d ",(obs+1));
                free(r);
                free(a);
                free(c);
            }
            fprintf(writer, "\n");
            free(x);
            fclose(writer);
}

 void readCodebook(double ** codebook)
{
    FILE *fp = fopen("codebook.txt", "r");
	if(fp==0)
		printf("Unable to open");
    double container=0;
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j <12 ; ++j) {
            fscanf(fp,"%lf",&container);
            codebook[i][j]=container;
            printf("%lf,",container);
        }
        printf("\n");
    }
}

class Lambda{
public:
    long double A[5][5];
    long double B[5][32];
    long double pi[5];
    long double alpha[150][5];
    long double beta[150][5];
    long double gamma[150][5];
    long double pmax;

    int O[150];
    Lambda()
    {
        for (int i = 0; i < 5; ++i) {
            pi[i]=0;
            for (int j = 0; j < 5; ++j) {
                A[i][j]=0;
            }
            for (int j = 0; j < 32; ++j) {
                B[i][j]=0;
            }

        }
        pi[0]=1;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 32; ++j) {
                B[i][j]=(1/32.0);
            }
            if (i!=4)
            {
                A[i][i]=.728;
                A[i][i+1]=.272;
            }
            else
            {
                A[i][i]=1;
            }
        }


    }
    void readobservation(string name)
    {
        FILE *fptr;
        fptr = fopen(name.c_str(), "r");
        if (fptr == NULL) {
            perror("Error");
        } else { ;
            //printf("started\n");
        }
        int temp_o=0;
        int last=0;
        for (int i = 0; i < 150; ++i) {
            if(!feof(fptr)){
                fscanf(fptr,"%d",&temp_o);
                O[i]=temp_o;
                last=i;
            }
            else{
                O[i]=O[last];
            }
            //printf("%d,",O[i]);
        }
		

        fclose(fptr);
        //////////////////
    }
    void display()
    {
        cout<<"\n*********PI************"<<endl;

        for (int i = 0; i < 5; ++i) {
            cout<<pi[i]<<",";

        }
        cout<<"\n********A*************"<<endl;

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                printf("%Le,",A[i][j]);
            }
            cout<<endl;

        }
        cout<<"\n*********B************"<<endl;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 32; ++j) {
                printf("%Le,",B[i][j]);
            }
            cout<<endl;

        }
        
        cout<<"\n*****************\n";

       


       

    }
    void calculateAlphaBetaGamma()
    {
        for (int i = 0; i < 150; ++i) {
            for (int j = 0; j < 5; ++j) {
                alpha[i][j]=0;
                beta[i][j]=0;
            }
        }

        for (int i= 0; i < 5; ++i) {
            alpha[0][i]=pi[i]*B[i][O[0]-1];
        }
        for(int t=1;t<150;t++)
        {
            for (int j = 0; j < 5; ++j) {
                for (int i = 0; i < 5; ++i) {
                    alpha[t][j]=alpha[t][j]+alpha[t-1][i]*A[i][j]*B[j][O[t]-1];
                }
            }
        }

        //////////////////////////////////

        for (int i = 0; i < 5; ++i) {
            beta[149][i]=1;
        }

        for(int t=148;t>=0;t--)
        {
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    beta[t][i]=beta[t][i]+A[i][j]*B[j][O[t+1]-1]*beta[t+1][j];
                }

            }
        }
        for (int t = 0; t <150 ; ++t) {
            long double denominator=0;
            for (int i = 0; i < 5; ++i) {
                denominator=denominator+alpha[t][i]*beta[t][i];
            }

            for (int i = 0; i < 5; ++i) {
                gamma[t][i]=(alpha[t][i]*beta[t][i])/denominator;
            }
        }
		long double totalp=0;
		for(int i=0;i<5;i++)
			totalp=totalp+alpha[149][i];
		pmax=totalp;
        /*
        cout<<"\n*********Alpha************"<<endl;

        for (int t = 0; t < 85; ++t) {
            for (int i = 0; i < 5; ++i) {
                printf("%Le,",alpha[t][i]);
            }
            printf("\n");
        }
        cout<<"\n*********Beta************"<<endl;

        for (int t = 0; t < 85; ++t) {
            for (int i = 0; i < 5; ++i) {
                printf("%Le,",beta[t][i]);
            }
            printf("\n");
        }
        cout<<"\n***********Gamma**********"<<endl;

        for (int t = 0; t < 85; ++t) {
            for (int i = 0; i < 5; ++i) {
                printf("%Le,",gamma[t][i]);
            }
            printf("\n");
        }*/
    }
    void applyVirtirbi()
    {
        calculateAlphaBetaGamma();
        long double delta[150][5];
        long double psi[150][5];
        int T=150;
        for (int i = 0; i < 5; ++i) {
            delta[0][i]=pi[i]*B[i][O[0]-1];
            psi[0][i]=-1;
        }

        for (int t = 1; t < T; ++t) {
            for (int j = 0; j <5 ; ++j) {
                delta[t][j]=delta[t-1][0]*A[0][j]*B[j][O[t]-1];
                psi[t][j]=0;
                int max_index=0;
                for (int i = 0; i <5 ; ++i) {
                    long double temp=delta[t-1][i]*A[i][j]*B[j][O[t]-1];
                    if (temp>delta[t][j])
                        delta[t][j]=temp;
                    if (delta[t-1][i]*A[i][j]>delta[t-1][max_index]*A[max_index][j])
                        max_index=i;
                }
                psi[t][j]=max_index;
            }
        }


        pmax=0;
        for (int i = 0; i <5 ; ++i) {
            if (delta[T-1][i]>pmax)
                pmax=delta[T-1][i];
        }
        //printf("State Sequence:");
        for (int t = 0; t < T; ++t) {
            int max=0;
            for (int i = 0; i < 5; ++i) {
                if (delta[t][i]>=delta[t][max])
                    max=i;
            }
           // printf("%d,",max+1);
        }
        //printf("\n");
    }
    void calculateLambda()
    {
        long double xi[150][5][5];
        for (int t = 0; t <150-1 ; ++t) {
            long double denominator=0;
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j <5 ; ++j) {
                    denominator=denominator+alpha[t][i]*A[i][j]*B[j][O[t+1]-1]*beta[t+1][j];
                }
            }

            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j <5 ; ++j) {
                    xi[t][i][j]=alpha[t][i]*A[i][j]*B[j][O[t+1]-1]*beta[t+1][j];
                    xi[t][i][j]=xi[t][i][j]/denominator;
                }
            }
        }

        for (int t = 0; t <150-1 ; ++t) {
            long double sum=0;
            for (int i = 0; i < 5; ++i) {
                long double sum=0;
                for (int j = 0; j < 5; ++j) {
                    sum=sum+xi[t][i][j];
                }
                gamma[t][i]=sum;
            }
        }

        long double pi_[5];
        long double A_[5][5];
        long double B_[5][32];
        for (int i = 0; i < 5; ++i) {
            pi_[i]=gamma[0][i];
        }
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                long double numerator=0;
                long double denominator=0;
                for (int t = 0; t < 150-1; ++t) {
                    numerator=numerator+xi[t][i][j];
                    denominator=denominator+gamma[t][i];
                }
                A_[i][j]=numerator/denominator;
            }

        }

        for (int i = 0; i < 5; ++i) {
            for (int k = 0; k < 32; ++k) {
                long double numerator=0;
                long double denominator=0;
                for (int t = 0; t < 150; ++t) {
                    if (O[t]==k+1)
                        numerator=numerator+gamma[t][i];
                    denominator=denominator+gamma[t][i];
                }
                B_[i][k]=numerator/denominator;
            }
        }
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j <5 ; ++j) {
                A[i][j]=A_[i][j];
            }
            for (int j = 0; j < 32; ++j) {

                B[i][j]=B_[i][j];
                if (B_[i][j]==0)
                {
                    B[i][j]=1/1000000000.0;
                    B[i][j]=B[i][j]/1000000000.0;
                    B[i][j]=B[i][j]/1000000000.0;

                }
            }
            pi[i]=pi_[i];
        }
    }

};


int main()
{
	//train();
	//return 0;
    printf("Reading the input files \n");
    generateRefFile();
    printf("Creating Universe\n");
    double **universe=readUniverse();
    printf("Creating Codebook\n");

    double **codebook= finalCodebookUsingLBG(universe);
    writecodebook(codebook);
	/*codebook=(double **) malloc((sizeof (double *)));
	for(int i=0;i<32;i++){
    codebook[i]=(double *) malloc(12*sizeof (double ));
	}
	readCodebook(codebook);*/

    printf("Generating observation sequence......\n");
    generateObservationSeq(codebook);
    Lambda final_Lambda[5];
    printf("Generating Lambda......\n");
    for (int digit = 0; digit <5 ; ++digit) {
        Lambda l[20];
		int ch=digit;
		string command;
		if(ch==0)
			command="left";
		else if(ch==1)
			command="right";
		else if(ch==2)
			command="up";
		else if(ch==3)
			command="down";
		else
			command="click";
        for (int occurence = 1; occurence <=20 ; ++occurence) {
            l[occurence-1].readobservation("OBS"+ command+"_"+patch::to_string(occurence)+".txt");
            for (int i = 0; i   < 100; ++i) {
                //cout<<"i="<<i<<endl;
                l[occurence-1].applyVirtirbi();
                l[occurence-1].calculateLambda();
            }
            //l[occurence-1].display();
           // cout<<"The probability of digit "<<digit<<" and observation "<<occurence<<"is "<<l[occurence-1].pmax<<endl;
        }
        for (int i = 0; i < 5; ++i) {
            final_Lambda[digit].pi[i]=(l[0].pi[i]+l[1].pi[i]+l[2].pi[i]+l[3].pi[i]+l[4].pi[i]+l[5].pi[i]+l[6].pi[i]+l[7].pi[i]+l[8].pi[i]+l[9].pi[i]+l[10].pi[i]+l[11].pi[i]+l[12].pi[i]+l[13].pi[i]+l[14].pi[i]+l[15].pi[i]+l[16].pi[i]+l[17].pi[i]+l[18].pi[i]+l[19].pi[i])/20.0;
        }
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                final_Lambda[digit].A[i][j]=(l[0].A[i][j]+l[1].A[i][j]+l[2].A[i][j]+l[3].A[i][j]+l[4].A[i][j]+l[5].A[i][j]+l[6].A[i][j]+l[7].A[i][j]+l[8].A[i][j]+l[9].A[i][j]+l[10].A[i][j]+l[11].A[i][j]+l[12].A[i][j]+l[13].A[i][j]+l[14].A[i][j]+l[15].A[i][j]+l[16].A[i][j]+l[17].A[i][j]+l[18].A[i][j]+l[19].A[i][j])/20.0;
            }
        }
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 32; ++j) {
                final_Lambda[digit].B[i][j]=(l[0].B[i][j]+l[1].B[i][j]+l[2].B[i][j]+l[3].B[i][j]+l[4].B[i][j]+l[5].B[i][j]+l[6].B[i][j]+l[7].B[i][j]+l[8].B[i][j]+l[9].B[i][j]+l[10].B[i][j]+l[11].B[i][j]+l[12].B[i][j]+l[13].B[i][j]+l[14].B[i][j]+l[15].B[i][j]+l[16].B[i][j]+l[17].B[i][j]+l[18].B[i][j]+l[19].B[i][j])/20.0;
            }
        }
    }
    cout<<"HMM models generated : \n";
    string inputz;
   // cout<<"Enter a file name to predict its outcome "<<endl;
   // cin>>inputz;
        int no_of_correct=0;
        for (int i = 0; i < 5; ++i) {
			int ch=i;
			string command;
		if(ch==0)
			command="left";
		else if(ch==1)
			command="right";
		else if(ch==2)
			command="up";
		else if(ch==3)
			command="down";
		else
			command="click";
            for (int occurence = 21; occurence <= 30; ++occurence) {
                inputz="OBS"+ command+"_"+ patch::to_string(occurence)+".txt";
                long double max=0;
                int max_index=0;
                for (int j = 0; j < 5; ++j) {
                    final_Lambda[j].readobservation(inputz);
                    final_Lambda[j].applyVirtirbi();
					final_Lambda[j].calculateAlphaBetaGamma();
                    if(final_Lambda[j].pmax>max)
                    {
                        max=final_Lambda[j].pmax;
                        max_index=j;

                    }
                }
				string pred;
		if(max_index==0)
			pred="left";
		else if(max_index==1)
			pred="right";
		else if(max_index==2)
			pred="up";
		else if(max_index==3)
			pred="down";
		else
			pred="click";
                cout<<"Prediction for "<<inputz<<" is "<<pred<<" and probability is "<<max<<endl;
                if (max_index==i)
                    no_of_correct++;
        }


    }
        cout<<"Offline Testing Accuracy="<<(no_of_correct)<<endl;
		int uh=0;
		while(true){
		printf("Press Y for offline testing and N to exit\n");
		string c;
		cin>>c;
		if(c=="N")
			break;
		system("Recording_Module.exe 3 input_file.wav input_file.txt");
		generateObservationSeqOnline(codebook);
		
		inputz="OBSinput.txt";
                long double max=0;
                int max_index=0;
                for (int j = 0; j < 5; ++j) {
                    final_Lambda[j].readobservation(inputz);
                    final_Lambda[j].applyVirtirbi();
					final_Lambda[j].calculateAlphaBetaGamma();

                    if(final_Lambda[j].pmax>max)
                    {
                        max=final_Lambda[j].pmax;
                        max_index=j;

                    }
                }
                cout<<"Prediction for "<<inputz<<" is "<<uh<<" and probability is "<<max<<endl;
				cursorOperation(uh); //cursor operation
				uh=uh+1;
				uh=uh%5;


				
		}
		


    return 0;


}