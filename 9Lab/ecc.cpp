#include <bits/stdc++.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>

using namespace std;
void printvar(mpz_t var,char *a)
{
	gmp_printf("\nVariable %s: (var: %Zd)\n",a,var);
}
class Point
{
	mpz_t x;
	mpz_t y;
	public:
		Point()
		{
			mpz_init(x);
			mpz_init(y);
		}
		void setPoint_point(Point p)
		{
			mpz_set(x,p.x);
			mpz_set(y,p.y);
		}
		void setpoints(char *xstr,char *ystr)
		{
			mpz_init_set_str(x,xstr,10);
			mpz_init_set_str(y,ystr,10);
		}
		void setx(mpz_t x1)
		{
			mpz_init_set(x1,x);
		}
		void sety(mpz_t y1)
		{
			mpz_init_set(y1,y);
		}
		void setpoints_mpz(mpz_t x1,mpz_t y1)
		{
			mpz_set(x,x1);
			mpz_set(y,y1);
		}

		void printpoints()
		{
			gmp_printf("\nPoints: (x: %Zd , y: %Zd )\n",x,y);
		}

};




int point_add(Point p1,Point p2,Point *p3,mpz_t p)
{
	mpz_t minus1,two,zero;
	mpz_init_set_str(minus1,"-1",10);
	mpz_init_set_str(two,"2",10);
	mpz_init_set_str(zero,"0",10);
	// p3->printpoints();
	mpz_t x1;
	mpz_t x2;
	mpz_t y1;
	mpz_t y2;
	mpz_t y_diff;
	mpz_t y_diff_sq;
	mpz_t x_diff;
	mpz_t x1x3_diff;
	mpz_t x_diff_inv;
	mpz_t x_diff_sq;
	mpz_t slope;
	mpz_t slopesq;
	mpz_t x3,y3;
	mpz_init(x3);
	mpz_init(y3);
	// p3->printpoints();
	p1.setx(x1);
	p2.setx(x2);
	p1.sety(y1);
	p2.sety(y2);
	mpz_init(y_diff);
	mpz_init(x_diff);
	mpz_init(x1x3_diff);
	mpz_init(y_diff_sq);
	mpz_init(x_diff_sq);
	mpz_init(x_diff_inv);
	mpz_init(slope);
	mpz_init(slopesq);
	mpz_sub(y_diff,y2,y1);
	mpz_sub(x_diff,x2,x1);
	if(mpz_cmp(x_diff,zero) == 0)
	{
		printvar(x1,"x1");
		printvar(x2,"x1");
		printvar(y1,"y1");
		printvar(y2,"y2");
		return 1;
	}
	mpz_powm(x_diff_inv,x_diff,minus1,p);
	mpz_powm(x_diff_sq,x_diff_inv,two,p);
	mpz_powm(y_diff_sq,y_diff,two,p);
	mpz_mul(slopesq,y_diff_sq,x_diff_sq);
	mpz_mul(slope,y_diff,x_diff_inv);
	mpz_sub(x3,slopesq,x1);
	mpz_sub(x3,x3,x2);
	mpz_mod(x3,x3,p);
	// p3->printpoints();
	mpz_sub(x1x3_diff,x1,x3);
	mpz_mul(y3,slope,x1x3_diff);
	mpz_sub(y3,y3,y1);
	mpz_mod(y3,y3,p);
	// p3->printpoints();
	p3->setpoints_mpz(x3,y3);
	return 0;
}


void point_double(Point p1,Point *p3,mpz_t p,mpz_t a)
{
	mpz_t minus1,two,zero;
	mpz_init_set_str(minus1,"-1",10);
	mpz_init_set_str(two,"2",10);
	// p3->printpoints();
	mpz_t x1;
	mpz_t y1;
	mpz_t x1sq;
	mpz_t y_diff;
	mpz_t y_diff_sq;
	mpz_t x_diff;
	mpz_t x1x3_diff;
	mpz_t x_diff_inv;
	mpz_t x_diff_sq;
	mpz_t slope;
	mpz_t slopesq;
	mpz_t x3,y3;
	mpz_init(x3);
	mpz_init(y3);
	// p3->printpoints();
	p1.setx(x1);
	mpz_init(x1sq);
	p1.sety(y1);
	mpz_init(y_diff);
	mpz_init(x_diff);
	mpz_init(x1x3_diff);
	mpz_init(y_diff_sq);
	mpz_init(x_diff_sq);
	mpz_init(x_diff_inv);
	mpz_init(slope);
	mpz_init(slopesq);
	mpz_powm(x1sq,x1,two,p);
	mpz_mul_ui(x1sq,x1sq,3);
	mpz_add(y_diff,x1sq,a);
	mpz_add(x_diff,y1,y1);
	mpz_powm(x_diff_inv,x_diff,minus1,p);
	mpz_powm(x_diff_sq,x_diff_inv,two,p);
	mpz_powm(y_diff_sq,y_diff,two,p);
	mpz_mul(slopesq,y_diff_sq,x_diff_sq);
	mpz_mul(slope,y_diff,x_diff_inv);
	mpz_sub(x3,slopesq,x1);
	mpz_sub(x3,x3,x1);
	mpz_mod(x3,x3,p);
	// p3->printpoints();
	mpz_sub(x1x3_diff,x1,x3);
	mpz_mul(y3,slope,x1x3_diff);
	mpz_sub(y3,y3,y1);
	mpz_mod(y3,y3,p);
	// p3->printpoints();
	p3->setpoints_mpz(x3,y3);
}


int scalarmul(Point p1,mpz_t number,mpz_t p,mpz_t a)
{
	mpz_t minus1,two,one,zero;
	printvar(number,"Number");
	mpz_init_set_str(minus1,"-1",10);
	mpz_init_set_str(one,"1",10);
	mpz_init_set_str(zero,"0",10);
	mpz_init_set_str(two,"2",10);
	mpz_t q;
	mpz_t r;
	Point double_point;
	double_point.setPoint_point(p1);
	// double_point.printpoints();
	Point add_point;
	mpz_init(q);
	mpz_init(r);
	int z;
	int flag=0;
	while(mpz_cmp(number,zero)!=0)
	{
		mpz_fdiv_q(q,number,two);
		// printvar(q,"q");
		mpz_fdiv_r(r,number,two);
		mpz_fdiv_q(number,number,two);
		// printvar(r,"r");
		// cout<<"\nAdd points";
		// add_point.printpoints();
		// cout<<"\nDouble points";
		// double_point.printpoints();
		if(mpz_cmp(r,one) == 0)
		{
			if(flag == 0)
				add_point.setPoint_point(p1);
			else
			{
				cout<<"reached";
				int l=point_add(add_point,double_point,&add_point,p);
			}
		}
		point_double(double_point,&double_point,p,a);
		// cout<<"\nAdd points after add";
		// add_point.printpoints();
		z--;
		flag=1;
	}
	cout<<"Done";
	add_point.printpoints();
	return 0;
}

int main(int argc,char **argv)
{
	mpz_t minus1,two,one,zero;
	// printvar(number,"Number");
	mpz_init_set_str(minus1,"-1",10);
	mpz_init_set_str(one,"1",10);
	mpz_init_set_str(zero,"0",10);
	mpz_init_set_str(two,"2",10);
	mpz_t p,a,b,number;
	Point p1,p2,p3;
	mpz_init_set_str(p,argv[1],10);
	mpz_init_set_str(a,argv[2],10);
	mpz_init_set_str(b,argv[3],10);
	p1.setpoints(argv[4],argv[5]);
	p2.setpoints(argv[6],argv[7]);
	mpz_init_set_str(number,argv[8],10);
	p1.printpoints();
	p2.printpoints();
	cout<<"Point addition: ";
	point_add(p1,p2,&p3,p);
	p3.printpoints();
	cout<<"\nPoint Multiplication: \n";
	point_double(p1,&p3,p,a);
	p3.printpoints();
	int l=0;
	int num=2;
	while(num <=36)
	{
		mpz_init_set_ui(number,num);
	 	scalarmul(p1,number,p,a);
		num++;
	}
	return 0;
}