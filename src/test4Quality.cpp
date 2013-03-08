#include"Hquality.h"
#include"Mquality.h"
#include"Lquality.h"
#include<iostream>

using namespace std;

int main()
{
	Quality* q = new Hquality();
	struct Property p;
	q->set(&p);
	cout<<"test for Hquality!"<<endl;
	cout<<p.fps<<endl;
	cout<<p.size<<endl;
	cout<<p.bitRate<<endl;

	q = new Mquality();
	q->set(&p);
	cout<<"test for Mquality!"<<endl;
	cout<<p.fps<<endl;
	cout<<p.size<<endl;
	cout<<p.bitRate<<endl;

	q = new Lquality();
	q->set(&p);
	cout<<"test for Lquality!"<<endl;
	cout<<p.fps<<endl;
	cout<<p.size<<endl;
	cout<<p.bitRate<<endl;


	return 0;
}
