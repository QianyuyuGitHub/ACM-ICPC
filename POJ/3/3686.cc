/*

**KM神建图**

The Windy's
Time Limit: 5000MS 		Memory Limit: 65536KB 		64bit IO Format: %I64d & %I64u 

Description

The Windy's is a world famous toy factory that owns M top-class workshop to make toys. This year the manager receives N orders for toys. The manager knows that every order will take different amount of hours in different workshops. More precisely, the i-th order will take Zij hours if the toys are making in the j-th workshop. Moreover, each order's work must be wholly completed in the same workshop. And a workshop can not switch to another order until it has finished the previous one. The switch does not cost any time.

The manager wants to minimize the average of the finishing time of the N orders. Can you help him?

Input

The first line of input is the number of test case. The first line of each test case contains two integers, N and M (1 ≤ N,M ≤ 50).
The next N lines each contain M integers, describing the matrix Zij (1 ≤ Zij ≤ 100,000) There is a blank line before each test case.

Output

For each test case output the answer on a single line. The result should be rounded to six decimal places.

Sample Input

3

3 4
100 100 100 1
99 99 99 1
98 98 98 1

3 4
1 100 100 100
99 1 99 99
98 98 1 98

3 4
1 100 100 100
1 99 99 99
98 1 98 98

Sample Output

2.000000
1.000000
1.333333

*/


/*
假设某个机器处理了k个玩具,那么对于这些玩具，有两种时间，一种是真正处理的时间，一种是等待的时间，等待的时间就是之前所有处理的玩具的时间，

假设这k个玩具真正用在加工的时间分为a1,a2,a3...ak, 那么每个玩具实际的时间是加工的时间+等待时间，分别为

a1, a1+a2, a1+a2+a3.......a1+a2+...ak   

求和之后变为 a1 *k + a2 * (k - 1) + a3 * (k - 2).... + ak

这时就发现，每个玩具之间的实际时间可以分开来算  然后求和了。

因为对每个机器，最多可以处理n个玩具，** 所以可以拆成n个点，1~n分别代表某个玩具在这个机器上倒数第几个被加工的 **

所以我们对于每个玩具i，机器j中拆的每个点k，连接一条z[i][j]*k权值的边。
*/


//Result:wizmann	3686	Accepted	896K	79MS	C++	1656B	
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

#define print(x) cout<<x<<endl
#define input(x) cin>>x
#define SIZE 54
#define INF 1<<28

int g[SIZE][SIZE*SIZE];
int fac[SIZE][SIZE];
int n,m;
char visx[SIZE],visy[SIZE*SIZE];
int linky[SIZE*SIZE];
int lx[SIZE],ly[SIZE*SIZE];
int slack;

void makeG()
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			for(int k=0;k<n;k++)
			{
				g[i][j*n+k]=-(fac[i][j]*(k+1));
			}
		}
	}
}

bool dfs(int x)
{
	visx[x]=1;
	for(int y=0;y<m*n;y++)
	{
		if(visy[y]) continue;
		int t=lx[x]+ly[y]-g[x][y];
		if(!t)
		{
			visy[y]=1;
			if(linky[y]==-1 || dfs(linky[y]))
			{
				linky[y]=x;
				return 1;
			}
		}
		else if(t<slack) slack=t;
	}
	return 0;
}

int km()
{
	memset(linky,-1,sizeof(linky));
	memset(lx,0,sizeof(lx));
	memset(ly,0,sizeof(ly));
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m*n;j++)
		{
			lx[i]=max(lx[i],g[i][j]);
		}
	}
	for(int i=0;i<n;i++)
	{
		while(1)
		{
			memset(visx,0,sizeof(visx));
			memset(visy,0,sizeof(visy));
			slack=INF;
			if(dfs(i)) break;
			for(int j=0;j<n;j++)
			{
				if(visx[j]) lx[j]-=slack;
			}
			for(int j=0;j<n*m;j++)
			{
				if(visy[j]) ly[j]+=slack;
			}
		}
	}
	int res=0;
	for(int i=0;i<n*m;i++)
	{
		if(linky[i]!=-1)
		{
			res+=lx[linky[i]]+ly[i];
		}
	}
	return res;
}


int main()
{
	//freopen("input.txt","r",stdin);
	int T;
	input(T);
	while(T--)
	{
		input(n>>m);
		memset(g,0,sizeof(g));
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<m;j++)
			{
				scanf("%d",&fac[i][j]);
			}
		}
		makeG();
		int ans=-km();
		printf("%.6f\n",(double)ans/n);
	}
	return 0;
}
