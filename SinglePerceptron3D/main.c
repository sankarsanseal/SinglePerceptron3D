//
//  main.c
//  SinglePerceptron3D
//
//  Created by Sankarsan Seal on 10/10/16.
//  Copyright © 2016 Sankarsan Seal. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GLUT/glut.h>
#include <unistd.h>


double w[4];
double mu[2][3];
double sigma[2][3];
double **x;
double *ylabel;
int n;
int current=-1;
double eta=0.3;

double xy[4][2]=
{
    {10,10},
    {-10,10},
    
    {-10,-10},
    {10,-10},
};

double sigmoid(double induced_local)
{
    double result=0;
    
    result=(1/(1+exp(-induced_local)));
    
    return result;
}

double randn (double mu, double sigma)
{
    double U1, U2, W, mult;
    static double X1, X2;
    static int call = 0;
    
    if (call == 1)
    {
        call = !call;
        return (mu + sigma * (double) X2);
    }
    
    do
    {
        U1 = -1 + ((double) rand () / RAND_MAX) * 2;
        U2 = -1 + ((double) rand () / RAND_MAX) * 2;
        W = pow (U1, 2) + pow (U2, 2);
    }
    while (W >= 1 || W == 0);
    
    mult = sqrt ((-2 * log (W)) / W);
    X1 = U1 * mult;
    X2 = U2 * mult;
    
    call = !call;
    
    return (mu + sigma * (double) X1);
}

void UpdateW()
{
    
    int i;
    double induced_local=0;
    double yvalue=0;
    double error=0;
    
    for(i=0;i<4;i++)
        induced_local+=x[current][i]*w[i];
    yvalue=sigmoid(induced_local);
    error=ylabel[current]- yvalue;
    //fprintf(stdout,"%lf \n",error);
    for(i=0;i<4;i++)
        w[i]+=eta*error*yvalue*(1-yvalue)*x[current][i];
    
   // for(i=0;i<4;i++)
     //   fprintf(stdout, "w[%d]:%lf ",i,w[i]);
    //fprintf(stdout, "\n");
    
}



void show()
{
    int i,j;
    double z[4];
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glPointSize(5.0);
    glLineWidth(3.0);
    
    glLoadIdentity();
    
    gluLookAt(	20 , 20, 20,
              0, 0,  0,
              0.0f, 1.0f,  0.0f);
    
    glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(20, 0, 0);
    
    
    
    glEnd();
    
    glBegin(GL_LINES);
    
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 20, 0);
    
    
    
    glEnd();

    
    glBegin(GL_LINES);
    
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 20);
    
    glEnd();

    
   glBegin(GL_POINTS);
    
    for(i=0;i<n;i++)
    {
        //fprintf(stdout, " i:%d c:%d\n",i , current);
        if(i!=current)
        {
            if(ylabel[i]==1.0)
                glColor3d(1.0, 1.0, 0.0);
            else if (ylabel[i]==0.0)
                glColor3d(0, 1, 1);
        }
        else
        {
            //fprintf(stdout, " i:%d c:%d\n",i , current);
            //glPointSize(15.0);
            glColor3d(1.0, 0.5, 0.5);
            //glPointSize(5.0);
        }
        
        glVertex3d(x[i][1], x[i][2], x[i][3]);
        

        
    }
    
    glEnd();
    
    current++;
    if(current==n)
        current=0;
    
    UpdateW();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for(i=0;i<4;i++)
    {

            z[i]=-(w[2]/w[3])*xy[i][1] - (w[1]/w[3])*xy[i][0] -w[0]/w[3];
        
    }
    
    glBegin(GL_QUADS);
    glColor3d(0.75, 0.75, 0.75);
    
    for(i=0;i<4;i++)
    {
        glVertex3d(xy[i][0], xy[i][1], z[i]);
    }
    
    glEnd();

    //sleep(1);
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio =  w * 1.0 / h;
    
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    
    // Reset Matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    int i,j;
    char direction='\0';
    double chance=0;
    
    srand(time(NULL));
    
    fprintf(stdout,"Enter the number of patterns:");
    fscanf(stdin,"%d", &n);
    
    for(i=0;i<2;i++)
    {
        for(j=0;j<3;j++)
        {
            if(j==0)
                direction='x';
            else if(j==1)
                direction='y';
            else if(j==2)
                direction='z';
            fprintf(stdout,"Enter the mean of class %d along %c axis:",i+1,direction);
            fscanf(stdin,"%lf%*c",&mu[i][j]);
            
            fprintf(stdout,"Enter the standard deviation of class %d along %c axis:", i+1,direction);
            fscanf(stdin, "%lf%*c",&sigma[i][j]);
        }
        
    }
    
    
    for(i=0;i<4;i++)
    {
        w[i]=(double)(rand()%10)/10;
        fprintf(stdout,"%lf ", w[i]);
        
    }
    fprintf(stdout,"\n");
    x=(double **)malloc(sizeof(double *)*n);
    ylabel=(double *)malloc(sizeof(double)*n);
    
    for(i=0;i<n;i++)
    {
        x[i]=(double*)malloc(sizeof(double)*4);
        ylabel[i]=0;
        for(j=0;j<4;j++)
            x[i][j]=0;
    }
    
    for(i=0;i<n;i++)
    {
        
        chance=randn(1, 0.5);
        if(chance>=0.5)
            ylabel[i]=1;
        else
            ylabel[i]=0;
        
      //  fprintf(stdout, "%lf, %lf\n",chance,ylabel[i]);
        
            x[i][0]=1;
            for(j=1;j<4;j++)
            {
                x[i][j]=randn(mu[(int)ylabel[i]][j-1], sigma[(int)ylabel[i]][j-1]);
               // fprintf(stdout, " %d %d %lf\n", i,j,x[i][j]);
            }
        
    }
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Single Perceptron");
    glutDisplayFunc(show);
    glutReshapeFunc(reshape);
    glutIdleFunc(show);
    
    glutMainLoop();
    
    
    return 0;
}
