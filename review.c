#include <stdio.h>
#include <stdlib.h>
int a[100001] ;
int main(int argc, char *argv[]) {
	unsigned i,n,k,max,sum;
	while(scanf("%u%u",&n,&k)!=EOF){
		a[0]=0;
		for(i=1;i<=n;i++){
			scanf("%u",&a[i]);
			a[i]=a[i]+a[i-1];			
		}
		max=0;//因为全部是正整数 
		for(i=0;i<=k;i++){//前面q取i个(i可以取0到k个) 
			//后面为k-i个，和为a[n]-a[n-k+i] 
			sum=a[i]+(a[n]-a[n-k+i]);
			if(sum>max)
				max=sum;
		}
		printf("%u\n",max);
	}		
	return 0;
}
