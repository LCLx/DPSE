
class Graph          //用邻接列表表示的图结构;
{
public:
	std::vector<Node*>	 m_NodeArray;             //节点数组;
	int					 m_nNumEdges;
	std::vector<Clique> m_ComplexArray;	//候选的种子集合;
	std::vector<Clique> m_allComplexArray;	//12个时间点全部的
public:
	Graph(){	
	}
	Graph(GraphLoader&	rawGraph)
	{
		LoadGraphFromRawGraph(rawGraph);
	}
	void loadGraph(const char* szFileName,const char* scFileName, double inf, double outf);   //载入初始的图信息;
	void LoadGraphFromRawGraph(GraphLoader rawGraph);//根据初始保存的原始图信息来获取图结构，总的来说是根据边逐渐构建图;
	void  GetSeeds();//找到种子节点;
	void GetFirstExtension(double inf);//第一层扩展;
	void GetSecondExtension(double outf);//第二层扩展;  
	void  FilterClique();
	void  outPutComplex(const char* scFileName);
};


void Graph::loadGraph(const char* szFileName,const char* scFileName,double inf, double outf){ 
	GraphLoader	rawGraph(szFileName); //创建一个GraphLoader类的实例，并调用一个参数的构造方法来初始化该实例
	LoadGraphFromRawGraph(rawGraph);//将上一步骤得到的数据结构转化成vector链表
	GetSeeds();//得到种子节点
	GetFirstExtension(inf);//第一次扩展
	GetSecondExtension(outf);//第二次扩展
	sort(m_ComplexArray.begin(),m_ComplexArray.end(),CompCliqueSizeDcend());//排序
	FilterClique();//筛选
	outPutComplex(scFileName);//输出
}

//根据初始保存的原始图信息来获取图结构。总的来说是根据边逐渐构建图;
void Graph::LoadGraphFromRawGraph(GraphLoader rawGraph)
{
	for(int i=0;i<rawGraph.m_nArcs;i++)	//从rawGraph中读取边的信息;
	{
		int j,k;
		bool bFoundFrom=false;
		bool bFoundTo=false;

		//刚开始时节点数组是为空的，是根据原始图信息来逐渐添加节点的;
		for(j=0;j<m_NodeArray.size();j++)	 //查看边的起始节点是否存在，j保存了改边起始点的位置;
		{
			if(m_NodeArray[j]->m_szName==rawGraph.m_RawGraph[i].m_szFrom)//比较2个字符串的大小……;
			{
				bFoundFrom=true;
				break;
			}

		}		
		for(k=0;k<m_NodeArray.size();k++)					    //查看边的终止节点是否存在, k保存了改边起始点的位置;
		{
			if(m_NodeArray[k]->m_szName==rawGraph.m_RawGraph[i].m_szTo)
			{
				bFoundTo=true;
				break;
			}
		}
	
		if(bFoundFrom==false)									//如果不存在，则创建一个新的节点;
		{
			Node* pNode=new Node(m_NodeArray.size(),rawGraph.m_RawGraph[i].m_szFrom);
			m_NodeArray.push_back(pNode);
			j=m_NodeArray.size()-1;												 //j保存了在m_NodeArray中的改边起始点的位置;
		}
		if(bFoundTo==false)											//如果不存在，则创建一个新的节点;
		{
			Node* pNode=new Node(m_NodeArray.size(),rawGraph.m_RawGraph[i].m_szTo);
			m_NodeArray.push_back(pNode);
			k=m_NodeArray.size()-1;												 //k保存了改边终止点的位置;
		}

		m_NodeArray[j]->InsertArc(rawGraph.m_RawGraph[i].m_szTo,k,rawGraph.m_RawGraph[i].m_fWeight); //在链表中添加一条边;
		m_NodeArray[j]->m_iDegree++;//节点的度增加1
		m_NodeArray[k]->InsertArc(rawGraph.m_RawGraph[i].m_szFrom,j,rawGraph.m_RawGraph[i].m_fWeight);   //在链表中添加一条边;
		m_NodeArray[k]->m_iDegree++;//节点的度增加1
	}

	m_nNumEdges=rawGraph.m_nArcs;//关系的个数等价于边的个数;
	std::cout<<" the total edges are:"<<m_nNumEdges<<std::endl;
	std::cout<<" the total nodes are:"<<m_NodeArray.size()<<std::endl;
	std::cout<<"Finish the txt To graph......;"<<std::endl;
}

//此函数是得到了候选的种子集合;
void Graph::GetSeeds()
{
	double Ave_Degree = 0.0;
	double Ave_AF = 0.0;
	for (int i = 0; i < m_NodeArray.size(); i++)
	{
		Ave_Degree += m_NodeArray[i]->m_iDegree;
		Arc* pArc = m_NodeArray[i]->m_pFirst;
		while(pArc!=NULL){
			Ave_AF += pArc->m_fWeight;
			pArc = pArc->m_pNextArc;
		}		
	}
	Ave_Degree = Ave_Degree/m_NodeArray.size();//平均度
	Ave_AF = Ave_AF/(m_NodeArray.size());//平均权重


	int iHubs=0;
	for (int i = 0; i < m_NodeArray.size(); i++)
	{
		double TempAF = 0.0;
		Arc* pArc = m_NodeArray[i]->m_pFirst;
		while(pArc!=NULL){
			TempAF += pArc->m_fWeight;
			pArc = pArc->m_pNextArc;
		}	
		if (m_NodeArray[i]->m_iDegree >= Ave_Degree  || TempAF >= Ave_AF)//度大于平均度或者 权值大于平均权值
		{
			Clique pClique(iHubs);
			++iHubs;
			pClique.m_CliqueNodes.push_back(m_NodeArray[i]);
			m_ComplexArray.push_back(pClique);
		}	
	}
}

//此函数是得到的种子进行第一次扩展，即内核扩展;
void Graph::GetFirstExtension(double inf)//inf平均权值最大限度
{

	for (int i = 0;i<m_ComplexArray.size();i++)
	{
		Arc* pArc = m_ComplexArray[i].m_CliqueNodes[0]->m_pFirst;//因为初始团里面只有一个节点，所以可以定位到下坐标0;
		while(pArc!=NULL){
			m_ComplexArray[i].m_CliqueNodes.push_back(m_NodeArray[pArc->m_iNodeTo]);
			pArc = pArc->m_pNextArc;		
		}
	}

	for (int i=0;i<m_ComplexArray.size();i++)
	{
		double TotalWeight = getNodesetWeights(m_ComplexArray[i]);
		double AveWeight = 2*TotalWeight/(m_ComplexArray[i].m_CliqueNodes.size()*(m_ComplexArray[i].m_CliqueNodes.size()-1));
		while (AveWeight < inf && m_ComplexArray[i].m_CliqueNodes.size()>2)
		{
			int m = FindMinVectorWeigthNode(m_ComplexArray[i]);		  //节点权重最小（AF）

			std::vector<Node*>::iterator begin = m_ComplexArray[i].m_CliqueNodes.begin();
			std::vector<Node*>::iterator end = m_ComplexArray[i].m_CliqueNodes.end();
			m_ComplexArray[i].m_CliqueNodes.erase(find(begin,end,m_ComplexArray[i].m_CliqueNodes[m]));

			TotalWeight = getNodesetWeights(m_ComplexArray[i]);
			AveWeight = 2*TotalWeight/(m_ComplexArray[i].m_CliqueNodes.size()*(m_ComplexArray[i].m_CliqueNodes.size()-1));
		}
	}
}

//此函数是得到的种子进行第二次扩展，即外核扩展;
void Graph::GetSecondExtension( double outf)
{
	for (int i=0;i<m_ComplexArray.size();i++)
	{
		std::vector<Node*> FromNodes;//已经找到的要扩展到团的节点，避免重复
		for (int j = 0; j<m_ComplexArray[i].m_CliqueNodes.size(); j++)
		{
			Arc* pArc = m_ComplexArray[i].m_CliqueNodes[j]->m_pFirst;
			while(pArc!=NULL)
			{
				std::vector<Node*>::iterator begin = FromNodes.begin();
				std::vector<Node*>::iterator end = FromNodes.end();
				std::vector<Node*>::iterator result = find(begin,end,m_NodeArray[pArc->m_iNodeTo]);

				if (!IsIncludedInClique(m_NodeArray[pArc->m_iNodeTo], m_ComplexArray[i]) && result == FromNodes.end())
				{
					double getAdjEdgesWeight = getAdjEdges(m_NodeArray[pArc->m_iNodeTo], m_ComplexArray[i]);
					if ((getAdjEdgesWeight/m_ComplexArray[i].m_CliqueNodes.size()) > (outf))//满足要求则添加
					{
						FromNodes.push_back( m_NodeArray[pArc->m_iNodeTo]);
					}
					//break;
				}
				pArc = pArc->m_pNextArc;		
			}		
		}
		for (int m=0;m<FromNodes.size();m++)
		{
			m_ComplexArray[i].m_CliqueNodes.push_back(FromNodes[m]);
		}
	}

}

void  Graph::FilterClique()
{ //已经按复合物大小降序排列，后者与前者相比，考虑后者要不要删
	int findNum;
	int mi;
	int mj;
	double overlap = 0.0;
	std::vector<Node*>::iterator iter;
	for (int i=0;i<m_ComplexArray.size();i++){
		m_ComplexArray[i].mark=true;
	}
	for(int i=0;i<m_ComplexArray.size()-1;i++){
		if(m_ComplexArray[i].mark==false)continue;
		for(int j=i+1;j<m_ComplexArray.size();j++){
			if(m_ComplexArray[j].mark==false)continue;
			findNum=0;
			mi=m_ComplexArray[i].m_CliqueNodes.size();
			mj=m_ComplexArray[j].m_CliqueNodes.size();
			for(int jd=0;jd<mj;jd++)
			{
				for(iter=m_ComplexArray[i].m_CliqueNodes.begin();iter!=m_ComplexArray[i].m_CliqueNodes.end();iter++)
				{	//若找到
					if((*iter)->m_szName==m_ComplexArray[j].m_CliqueNodes[jd]->m_szName)
					{
						findNum++;
						break;
					}
				}
			}
			if ((mi!=0) && (mj!=0))
			{
				overlap = (double) (findNum*findNum)/(mi*mj);//计算overlap值
			}
			if (findNum == mj||(overlap-tvOverlap>0)){//完全包含或者与已经define的tvOverlap相比较满足
				m_ComplexArray[j].mark=false;
			}
		}
	}
}

void  Graph::outPutComplex(const char* scFileName)
{//输出格式：
	//	node1  node2  ...
	//	node1  node2  ...
	//		...
	std::ofstream OutFile;
	OutFile.open(scFileName);
	if (!OutFile)
	{
		std::cout<<"Can't create OutPut file COMPLEX!"<<std::endl;
	}

	for (int i=0;i<m_ComplexArray.size();i++)
	{  
		if(m_ComplexArray[i].mark==true){
			for (int j=0;j<m_ComplexArray[i].m_CliqueNodes.size();j++)
			{ 
				OutFile<<m_ComplexArray[i].m_CliqueNodes[j]->m_szName<<'\t';

			}
			OutFile<<std::endl;
		}
	}
	if(OutFile)
		OutFile.close();
}
