bool IsIncludedInClique(Node* node, Clique clique)//判断一个节点是否属于一个团，属于时返回true，不属于时返回false;
{
	bool Ismark = false;
	for (int i=0;i<clique.m_CliqueNodes.size();i++)
	{
		if(node->m_iNode == clique.m_CliqueNodes[i]->m_iNode)
		{
			Ismark =true;
		}
	}
	return Ismark;
}

//获取团内所有边的权重之和（所有的边只能包含在团内）;
double getNodesetWeights(Clique clique)
{
	double nodeweight=0.0;
	for (int i=0;i<clique.m_CliqueNodes.size();i++)
	{
		Arc* pArc = clique.m_CliqueNodes[i]->m_pFirst;
		while (pArc != NULL)
		{
			for (int j=0;j<clique.m_CliqueNodes.size();j++)
			{
				if (pArc->m_iNodeTo == clique.m_CliqueNodes[j]->m_iNode)
				{
					nodeweight += pArc->m_fWeight;
				}
			}
			pArc =pArc->m_pNextArc;
		}
	}
	nodeweight = nodeweight/2.0;
	return nodeweight;
}

int GetCliqueEdges(Clique clique)//获取一个团内的总的边数;
{
	int EdgesInClique = 0;
	for (int i=0;i<clique.m_CliqueNodes.size();i++)
	{
		Arc* pArc = clique.m_CliqueNodes[i]->m_pFirst;
		while (pArc != NULL)
		{
			for (int j=0;j<clique.m_CliqueNodes.size();j++)
			{
				if (pArc->m_iNodeTo == clique.m_CliqueNodes[j]->m_iNode)
				{
					EdgesInClique++;
				}
			}
			pArc = pArc->m_pNextArc;
		}

	}
	return (EdgesInClique/2);
}

int FindMinVectorWeigthNode(Clique clique)
{	//找到一个团内节点权重最小的那个，返回向量容器中的节点序号;
	double min = 100;
	int marknode = 0;
	int vectormarknode = 0;
	for (int i=0;i<clique.m_CliqueNodes.size();i++)
	{	
		//std::cout<<"****   ****    ****"<<std::endl;
		double nodeWeight = 0.0;
		Arc* pArc = clique.m_CliqueNodes[i]->m_pFirst;
		while (pArc != NULL)
		{
			for (int j=0;j<clique.m_CliqueNodes.size();j++)
			{
				if (pArc->m_iNodeTo == clique.m_CliqueNodes[j]->m_iNode)
				{
					nodeWeight += pArc->m_fWeight;
				}
			}
			pArc = pArc->m_pNextArc;
		}
		if(nodeWeight < min)
		{
			vectormarknode = i;
			min = nodeWeight;
			marknode = clique.m_CliqueNodes[i]->m_iNode;
		}
	}
	return vectormarknode;
}

double getAdjEdges(Node* mNode,Clique mClique)//获取mNode与团mClique之间的权重和;
{
	Arc* pArc = mNode->m_pFirst;
	double weightAdjEdge = 0.0;

	while (pArc!=NULL)
	{
		for (int i=0;i<mClique.m_CliqueNodes.size();i++)
		{
			if(pArc->m_iNodeTo == mClique.m_CliqueNodes[i]->m_iNode)
			{
				weightAdjEdge += pArc->m_fWeight;
			}
		}
		pArc = pArc->m_pNextArc;
	}
	return weightAdjEdge;

}


