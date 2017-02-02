//
//  main.cpp
//  ALGO
//
//  Created by LCL on 2017/1/24.
//  Copyright © 2017年 LCL. All rights reserved.
//


#include<stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#define tvOverlap 0.8
#define eps 0
using namespace std;
#define n_subPINs 36

#include "DataStruct.h"
#include "myFunction.h"
#include "CASEonGraph.h"
#include <string>
#include <string.h>
#include <time.h>

int main(int args,char **argv)
{
    double inf;
    double outf;
    char wpin[100]="/Users/LCL/Documents/WPIN.txt";	//静态
    
    for (inf = 0.40;inf <=0.40;inf+=0.1)
    {
        for (outf =0.50;outf <= 0.50;outf+=0.2)
        {
            char str[50] = "_Complexes_IN_";
            char s[4];
            gcvt(inf,4,s); //把double型的inf保留4位转换为字符串给s
            strcat(str,s);
            strcat(str,"_OUT_");
            gcvt(outf,2,s);//把double型的outf保留2位转换为字符串给s

            strcat(str,s);
            cout<<str<<endl;
            strcat(str,".txt");//str是生成的文件名
            
            char  SCComplexesfilename[100] ;
            strcpy(SCComplexesfilename,wpin);//把wpin的地址带入SCComplexesfilename
            SCComplexesfilename[strlen(SCComplexesfilename)-4] = '\0';
            strcat(SCComplexesfilename,str);//把str附在wpin的地址后面，形成目标文件名
            Graph graph;
            graph.loadGraph(wpin,SCComplexesfilename,inf,outf);	
        }
    }		
    
    return 0;
}
